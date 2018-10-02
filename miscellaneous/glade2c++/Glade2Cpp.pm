#!/usr/bin/perl

package Glade2Cpp;
use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use XML::Parser;
use File::Basename qw (basename dirname);
use File::Spec;
use File::Compare;
use Tie::IxHash;

use constant true  => 1;
use constant false => 0;

my %windows = ();

$windows {$_} = true foreach qw(
	Gtk::Window
	Gtk::ApplicationWindow
	Gtk::FileChooserDialog
	Gtk::ColorChooserDialog
	Gtk::FontChooserDialog
	Gtk::RecentChooserDialog
	Gtk::AppChooserDialog
	Gtk::MessageDialog
	Gtk::AboutDialog
	Gtk::Dialog
	Gtk::Assistant
	Gtk::ColorSelectionDialog
);

my %objects = ();

$objects {$_} = true foreach qw(
	Gtk::FileFilter
	Gtk::RecentFilter
	Gtk::RecentManager
	Gtk::AccelGroup
	Gtk::ActionGroup
	Gtk::Action
	Gtk::Adjustment
	Gtk::IconFactory
	Gtk::RadioAction
	Gtk::RecentAction
	Gtk::ToggleAction
	Gtk::ListStore
	Gtk::TreeStore
	Gtk::TreeModelFilter
	Gtk::TreeModelSort
	Gtk::TreeSelection
	Gtk::TreeViewColumn
	Gtk::CellRendererCombo
	Gtk::CellRendererText
	Gtk::CellRendererToggle
	Gtk::CellRendererPixbuf
	Gtk::TextBuffer
	Gtk::EntryCompletion
	Gsv::Buffer
);

my $plugins = {
	"GtkSourceView" => { path => "gtksourceviewmm.h", class => "Gsv::View", init => "Gsv::init ();" },
};

sub new
{
	my ($class, $options) = @_;

	my $self = {
		namespaces          => exists $options -> {namespaces}   ? $options -> {namespaces}   : [ ],
		base_class          => exists $options -> {base_class}   ? $options -> {base_class}   : "",
		class_prefix        => exists $options -> {class_prefix} ? $options -> {class_prefix} : "",
		class_suffix        => exists $options -> {class_suffix} ? $options -> {class_suffix} : "",
		suffixes            => [ qw (.glade .ui .xml) ],
		empty_constructor   => $options -> {empty_constructor},
		pure_virtual        => exists $options -> {pure_virtual} ? $options -> {pure_virtual} : true,
		derived_directories => exists $options -> {derived_directories} ? $options -> {derived_directories} : [ ],
		h_signal_handler    => { },
		cpp_signal_handler  => { },
		class               => "",
		object              => "",
		id                  => "",
		prototypes          => { grep { not /^\s*$/ } map { chomp; $_ } <DATA> },
		windows             => { },
	};

	bless $self, $class;
	return $self;
}

sub getPrototype
{
	my ($self, $class, $name) = @_;

	return $self -> {prototypes} {"$class\::$name"} if exists $self -> {prototypes} {"$class\::$name"};

	my @result = `find /usr/include/gtkmm-3.0/gtkmm -name \\*.h -exec grep -I -P 'virtual.*?on_$name\\s*\\(' {} \\;`;
	my $handler = shift @result;
	
	chomp $handler;
	
	# Remember handler.
	$self -> {prototypes} {"$class\:\:$name"} = $handler;

	# Append handler to DATA section.
	open THIS, ">>", __FILE__;
	say THIS "$class\:\:$name";
	say THIS $handler;
	close THIS;
	
	return $handler;
}

sub isWindow
{
	my $class = shift;
	return exists $windows {$class};
}

sub isObject
{
	my $class = shift;
	return exists $objects {$class};
}

sub isWidget
{
	return not isObject @_;
}

sub getClass
{
	my $class  = shift;
	my $plugin = $plugins -> {$class};

	return $plugin -> {class} if $plugin;

	$class =~ s/Gtk/Gtk::/;
	
	return $class;
}

sub h_derived
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} = getClass ($attributes {class});
	return if not isWidget ($attributes {class});
	return unless $attributes {id} =~ s/^\w+\.//;

	my $path;

	for (@{ $self -> {derived_directories} })
	{
		my $abs_path = "$_/$attributes{id}.h";

		next unless -f $abs_path;

		$path = File::Spec -> abs2rel ($abs_path, $self -> {directory});
		last;
	}

	die "Couldn't find derived widget '$attributes{id}' in path."
		unless ($path);

	$self -> {derived_h} -> {$path} = 1;
}

sub h_plugin
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	my $plugin = $plugins -> {$attributes {class}};
	return unless $plugin;

	$self -> {plugin_h} -> {$plugin -> {path}} = $plugin;
}

sub h_object_getters
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} = getClass ($attributes {class});
	return if not isObject ($attributes {class});

	say $file "const Glib::RefPtr <$attributes{class}> &";
	say $file "get$attributes{id} () const\n{ return m_" . $attributes {id} . "; }";
}

sub h_widget_getters
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};

	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} = getClass ($attributes {class});
	return if not isWidget ($attributes {class});
	$attributes {class} = $1 if $attributes {id} =~ s/\.(\w+)$//;

	$self -> {windows} -> {"m_" . $attributes {id}} = 1
		if isWindow ($attributes {class});

	say $file "$attributes{class} &";
	say $file "get$attributes{id} () const\n{ return *m_" . $attributes {id} . "; }";
}

sub h_objects
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} = getClass ($attributes {class});
	return if not isObject ($attributes {class});

	say $file "Glib::RefPtr <$attributes{class}> m_" . $attributes {id} . ";";
}

sub h_widgets
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} = getClass ($attributes {class});
	return if not isWidget ($attributes {class});
	$attributes {class} = $1 if $attributes {id} =~ s/\.(\w+)$//;

	say $file "$attributes{class}* m_" . $attributes {id} . ";";
}

sub h_signal_handler
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	if ($name eq "object")
	{
		$self -> {class} = $attributes {class};
		$self -> {class} =~ s/Gtk//;
		return;
	}

	return if $name ne "signal";
	return if exists $self -> {h_signal_handler} {$attributes {handler}};
	
	$self -> {h_signal_handler} {$attributes {handler}} = 1;

	$attributes {name} =~ s/-/_/sgo;

	my $prototype = $self -> getPrototype ($self -> {class}, $attributes {name});
	
	unless ($prototype)
	{
		say "\t missing prototype for '", $attributes {name} . "'.";
		return;
	}

	return
		if exists $attributes {swapped} && $attributes {swapped} eq "yes";

	$prototype =~ s/(virtual)\s/$1\n/s;
	$prototype =~ s/on_($attributes{name})/\n$attributes{handler}/s;

	#$prototype =~ s/\)/, Gtk\:\:$self->{class} &)/s
	#	unless $prototype =~ s/\(\s*\)/Gtk\:\:$self->{class} &)/s;

	$prototype =~ s/;/ = 0;/s
		if $self -> {pure_virtual};

	say $file $prototype;
}

sub cpp_get_objects
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isObject ($attributes {class});
	
	say $file "m_" . $attributes {id} . " = Glib::RefPtr <$attributes{class}>::cast_dynamic (m_builder -> get_object (\"$attributes{id}\"));";
}

sub cpp_get_widgets
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return unless $attributes {id};
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});
	
	my $id = $attributes{id};

	if ($attributes{id} =~ s/\.(\w+)$//)
	{
		say $file "m_builder -> get_widget_derived (\"$id\", m_" . $attributes {id} . ");";
	}
	else
	{
		say $file "m_builder -> get_widget (\"$id\", m_" . $attributes {id} . ");";
	}
}

sub cpp_signals
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	if ($name eq "object")
	{
		$attributes {id} =~ s/\.(\w+)$// if $attributes {id};

		$self -> {class} = $attributes {class};
		$self -> {id}    = $attributes {id};

		$self -> {class} =~ s/Gtk/Gtk::/;
		return;
	}
	
	return if $name ne "signal";

	if ($self -> {object} ne $self -> {class})
	{
		if (! $self->{id})
		{
			warn "\n";
			warn "*" x 80, "\n";
			warn "\n";
			warn "$self->{class} has no id!\n";
			warn "\n";
			warn "*" x 80, "\n";
		}

		$self -> {object} = $self -> {class};
		
		say $file "";
		say $file "// Connect object $self->{class} with id '$self->{id}'.";
	}
	
	$attributes {name} =~ s/-/_/sgo;
	
	my $signal  = "m_" . $self -> {id} . " -> signal_$attributes{name} ()";
	my $after   = exists $attributes {after} && $attributes {after} eq "yes" ? ", false" : "";
	my $swapped = exists $attributes {swapped} && $attributes {swapped} eq "yes";

	if ($swapped)
	{
		say $file "$signal .connect (sigc::bind (sigc::mem_fun (this, &$self->{class_name}\:\:$attributes{handler}), sigc::ref (*m_" . $self -> {id} . ")) $after);";
		#say $file "m_connections .emplace_back ($signal .connect (sigc::bind (sigc::mem_fun (this, &$self->{class_name}\:\:$attributes{handler}), sigc::ref (*m_" . $self -> {id} . ")) $after));";
	}
	else
	{
		say $file "$signal .connect (sigc::mem_fun (this, &$self->{class_name}\:\:$attributes{handler}) $after);";
		#say $file "m_connections .emplace_back ($signal .connect (sigc::mem_fun (this, &$self->{class_name}\:\:$attributes{handler}) $after));";
	}
}

sub cpp_signal_handler
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};

	return if $name ne "signal";
	return if exists $self -> {cpp_signal_handler} {$attributes {handler}};

	$self -> {cpp_signal_handler} {$attributes {handler}} = 1;

	$attributes {name} =~ s/-/_/;

	my $prototype = $self -> getPrototype ($attributes {name});
	$prototype =~ s/on_($attributes{name})/$attributes{handler}/s;
	$prototype =~ s/\s*virtual\s+(\w+)/\\t$1\\n\\t$self->{class_name}\:\:/s;
	$prototype =~ s/\(/ (/s;

	my $function  = $self -> getPrototype ($attributes {name});
	$function =~ s/^\s+//s;
	$function =~ s/;//s;
	$function =~ s/on_($attributes{name})/$attributes{handler}/s;
	$function =~ s/virtual\s+(\w+)\s+/$1\n$self->{class_name}\:\:/s;
	my $return_type = $1;

	$function .= "\n{\n";
	$function .= "std::clog\n";
	$function .= "<< \"Signal handler called:\" << std::endl\n";
	$function .= "<< \"$prototype\" << std::endl;\n";
	$function .= "\nreturn false; // Emit the signal and proceed with standard behavior.\n"
		if $return_type eq "bool";
	$function .= "}";

	say $file $function;
}

sub generate
{
   my ($self, $filename, $directory) = @_;
	
	my $parser;
	my $basename = basename $filename;
	my $name     = basename $filename, @{$self -> {suffixes}};

	$self -> {directory}          = $directory;
	$self -> {h_signal_handler}   = { };
	$self -> {cpp_signal_handler} = { };
	$self -> {class_name}         = "$self->{class_prefix}${name}$self->{class_suffix}";
	$self -> {windows}            = { }; 
	$self -> {derived_h}          = { };
	$self -> {plugin_h}           = { };


	tie %{ $self -> {h_signal_handler} },   'Tie::IxHash';
	tie %{ $self -> {cpp_signal_handler} }, 'Tie::IxHash';
	tie %{ $self -> {windows} },            'Tie::IxHash';

	print $self -> {class_name}, " ";

	my $h_tmp   = "/tmp/glad2cpp.$name.h";
	my $h_out   = "$directory/$self->{class_name}.h";

	my $cpp_tmp = "/tmp/glad2cpp.$name.cpp";
	my $cpp_out = "$directory/$self->{class_name}.cpp";

	my $base_class_name = $self -> {base_class} ? basename $self -> {base_class}, ".h" : "";
	my $base_path       = File::Spec -> abs2rel ($self -> {base_class}, dirname $h_out) ;



	my $input = get_file ($filename);

	# Derived
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_derived (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Plugin
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_plugin (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');



	# Header file
	open OUT, ">", $h_tmp;
	$self -> {handle} = *OUT;

	# Includes
	say OUT "";
	say OUT "#ifndef " . uc "_$name\_$self->{class_name}\_H_";
	say OUT "#define " . uc "_$name\_$self->{class_name}\_H_";

	say OUT "";
	say OUT "#include \"$base_path\"" if $base_class_name;

	say OUT "";
	say OUT "#include \"$_\""
		foreach (sort keys %{ $self -> {derived_h} });

	say OUT "";
	say OUT "#include <gtkmm.h>";
	say OUT "#include <string>";

	say OUT "";
	say OUT "#include <$_>"
		foreach (sort keys %{ $self -> {plugin_h} });
	
	# Namespace
	say OUT "";
	say OUT "namespace $_ {" foreach @{$self -> {namespaces}};

	# Class
	say OUT "";
	say OUT "/**";
	say OUT " *  Gtk Interface for $name.";
	say OUT "*/";

	say OUT "class $self->{class_name}";
	say OUT ": public $base_class_name" if $base_class_name;
	say OUT "{";

	# Public section
	say OUT "public:";

	# Empty constructor
	say OUT "///  \@name Construction";
	say OUT "";

	if ($self->{empty_constructor})
	{
		say OUT "  $self->{class_name} () :";

		# Call base class construtor if any.
		if ($base_class_name) {
			say OUT "  $base_class_name ()";
			say OUT "  { }";
		}
	}

	# Constructor.
	say OUT "template <class ... Arguments>";
	say OUT "$self->{class_name} (const std::string & filename, const Arguments & ... arguments) :";
	# Call base class construtor if any.
	if ($base_class_name) {
		say OUT "$base_class_name (arguments ...)";
	}
	say OUT "{ create (filename); }";
	say OUT "";

	# Constructor.
	say OUT "template <class ... Arguments>";
	say OUT "$self->{class_name} (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :";
	# Call base class construtor if any.
	if ($base_class_name) {
		say OUT "$base_class_name (arguments ...)";
	}
	say OUT "{ create (filenames); }";
	say OUT "";

	# Member access
	say OUT "///  \@name Member access";
	say OUT "";

	# Builder
	say OUT "  const Glib::RefPtr <Gtk::Builder> & getBuilder () const";
	say OUT "  { return m_builder; }";

	#say OUT "  void updateWidgets (const std::vector <Glib::ustring> & names) const";
	#say OUT "  { getBuilder () -> add_from_file (filename, names); }";
	#say OUT "";

	# createWidget
	#say OUT "  template <class Type>";
	#say OUT "  Type* createWidget (const std::string & name) const";
	#say OUT "  {";
	#say OUT "      getBuilder () -> add_from_file (filename, name);";
	#say OUT "";
	#say OUT "      Type* widget = nullptr;";
	#say OUT "      m_builder -> get_widget (name, widget);";
	#say OUT "      return widget;";
	#say OUT "  }";
	#say OUT "";

	# Object getters
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_object_getters (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Widget getters
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_widget_getters (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	say OUT "///  \@name Signal handlers";
	say OUT "";

	# Virtual signal handlers
	say OUT "";
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_signal_handler (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');
	
	say OUT "///  \@name Destruction";
	say OUT "";

	# dispose
	#say OUT "  virtual";
	#say OUT "  void";
	#$base_class_name ? say OUT "  dispose () override;" : say OUT "  dispose ();";

	# Destructor
	say OUT "  virtual";
	print OUT "  ~$self->{class_name} ()";
	print OUT " override" if $base_class_name;
	say OUT ";";
	say OUT "";

#	say OUT "protected:";
#	say OUT "Glib::RefPtr <Gtk::Builder> &";
#	say OUT "getBuilder () const { return m_builder; }";

	# Private section
	say OUT "private:";
	say OUT "";

	say OUT "///  \@name Construction";
	say OUT "";

	say OUT "  void";
	say OUT "  create (const std::string &);";
	say OUT "";

	say OUT "  void";
	say OUT "  create (std::initializer_list <std::string>);";
	say OUT "";

	say OUT "  void";
	say OUT "  create ();";
	say OUT "";

	say OUT "///  \@name Static members";
	say OUT "";

	say OUT "///  \@name Members";
	say OUT "";

	#say OUT "  std::string filename;";
	say OUT "  Glib::RefPtr <Gtk::Builder> m_builder;";
	#say OUT "  std::deque <sigc::connection> m_connections;";

	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_objects (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');
		
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_widgets (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Class end
	say OUT "};";

	# Namespace end
	say OUT "";
	say OUT "} // $_ " foreach reverse @{$self -> {namespaces}};

	say OUT "";
	say OUT "#endif";

	close OUT;

	#print get_file ($h_tmp);

 	system "call_uncrustify.sh", $h_tmp;
	print compare ($h_tmp, $h_out) != 0 ? ".h" : "", " ";

	system "mv", $h_tmp, $h_out
		if compare ($h_tmp, $h_out) != 0;

	# Source file
	open OUT, ">", $cpp_tmp;
	$self -> {handle} = *OUT;

	# Includes
	say OUT "";
	say OUT "#include \"$self->{class_name}.h\"";
	say OUT "";
	
	unless ($self -> {pure_virtual})
	{
		say OUT "#include <iostream>";
		say OUT "";
	}

	# Namespace
	say OUT "namespace $_ {" foreach @{$self -> {namespaces}};
	say OUT "";

	# create
	say OUT "void";
	say OUT "$self->{class_name}\::create (const std::string & filename)";
	say OUT "{";
	foreach (sort keys %{ $self -> {plugin_h} })
	{
		say OUT $self -> {plugin_h} -> {$_} -> {init}
			if $self -> {plugin_h} -> {$_} -> {init};
	}
	say OUT "" if keys %{ $self -> {plugin_h} };
	say OUT "// Create Builder.";
	say OUT "m_builder = Gtk::Builder::create_from_file (filename);";
	say OUT "";
	say OUT "create ();";
	say OUT "}";
	say OUT "";

	# create
	say OUT "void";
	say OUT "$self->{class_name}\::create (std::initializer_list <std::string> filenames)";
	say OUT "{";
	foreach (sort keys %{ $self -> {plugin_h} })
	{
		say OUT $self -> {plugin_h} -> {$_} -> {init}
			if $self -> {plugin_h} -> {$_} -> {init};
	}
	say OUT "" if keys %{ $self -> {plugin_h} };
	say OUT "// Create Builder.";
	say OUT "m_builder = Gtk::Builder::create ();";
	say OUT "";
	say OUT "for (const auto & filename : filenames)";
	say OUT "m_builder -> add_from_file (filename);";
	say OUT "";
	say OUT "create ();";
	say OUT "}";
	say OUT "";

	# Initialize members
	say OUT "void";
	say OUT "$self->{class_name}\::create ()";
	say OUT "{";

	# Store objects
	say OUT "// Get objects.";
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> cpp_get_objects (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Store widgets
	say OUT "";
	say OUT "// Get widgets.";
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> cpp_get_widgets (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Connect signal handler
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> cpp_signals (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Constructor end
	say OUT "}";
	say OUT "";

	# Signal handler outlines
	unless ($self -> {pure_virtual})
	{
		$parser = new XML::Parser (Handlers => {Start => sub { $self -> cpp_signal_handler (@_) }});
		$parser -> parse ($input, ProtocolEncoding => 'UTF-8');
	}

	# dispose
	#say OUT "void";
	#say OUT "$self->{class_name}\:\:dispose ()";
	#say OUT "{";
	#say OUT "for (auto & connection : m_connections)";
	#say OUT "   connection .disconnect ();";
	#say OUT "";
	#say OUT "${base_class_name}::dispose ();" if $base_class_name;
	#say OUT "}";

	# Destructor
	say OUT "$self->{class_name}\:\:~$self->{class_name} ()";
	say OUT "{";
	say OUT "delete $_;" foreach keys %{$self -> {windows}};
	say OUT "}";

	# Namespaces end
	say OUT "";
	say OUT "} // $_ " foreach reverse @{$self -> {namespaces}};

	close OUT;

	#print get_file ($cpp_tmp);

	system "call_uncrustify.sh", $cpp_tmp;
	print compare ($cpp_tmp, $cpp_out) != 0 ? ".cpp" : "", " ";

	system "mv", $cpp_tmp, $cpp_out
		if compare ($cpp_tmp, $cpp_out) != 0;

	print "\n";
}



sub get_file
{
	my $filename = shift;
	return (join '', `cat '$filename'`);
}

1;
__DATA__
ToggleAction::toggled
  virtual void on_toggled();
ImageMenuItem::activate
  virtual void on_activate();
MenuItem::activate
  virtual void on_activate();
CheckMenuItem::toggled
  virtual void on_toggled();
ApplicationWindow::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* gdk_event);
ApplicationWindow::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
ApplicationWindow::key_release_event
  virtual bool on_key_release_event(GdkEventKey* key_event);
Box::style_updated
  virtual void on_style_updated();
MenuBar::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Box::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
ToolButton::clicked
  virtual void on_clicked();
ToolButton::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Entry::icon_release
  virtual void on_icon_release(Gtk::EntryIconPosition icon_position, const GdkEventButton* event);
Entry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
Notebook::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Notebook::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
Notebook::switch_page
  virtual void on_switch_page(Gtk::Widget* page, guint page_number);
RadioToolButton::toggled
  virtual void on_toggled();
ToggleToolButton::toggled
  virtual void on_toggled();
RadioButton::toggled
  virtual void on_toggled();
RadioButton::clicked
  virtual void on_clicked();
Button::clicked
  virtual void on_clicked();
Adjustment::value_changed
  virtual void on_value_changed();
Box::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
Box::key_release_event
  virtual bool on_key_release_event(GdkEventKey* key_event);
Entry::changed
  virtual void on_changed();
Entry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
FileChooserDialog::response
  virtual void on_response(int response_id);
ComboBoxText::changed
  virtual void on_changed();
CheckButton::toggled
  virtual void on_toggled();
ToggleButton::toggled
  virtual void on_toggled();
ToggleButton::clicked
  virtual void on_clicked();
Box::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Entry::delete_text
  virtual void on_delete_text(int start_pos, int end_pos);
Entry::insert_text
  virtual void on_insert_text(const Glib::ustring& text, int* position);
TreeView::row_activated
  virtual void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
Box::map
  virtual void on_map();
Box::unmap
  virtual void on_unmap();
SearchEntry::search_changed
  virtual void on_search_changed();
TreeView::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
TreeViewColumn::clicked
  virtual void on_clicked();
RadioMenuItem::toggled
  virtual void on_toggled();
TreeView::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
TreeSelection::changed
  virtual void on_changed();
Expander::map
  virtual void on_map();
Expander::unmap
  virtual void on_unmap();
SpinButton::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* focus_event);
EventBox::enter_notify_event
  virtual bool on_enter_notify_event(GdkEventCrossing* crossing_event);
FileChooserButton::file_set
  virtual void on_file_set();
TreeView::test_expand_row
  virtual bool on_test_expand_row(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
TreeView::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
TreeView::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* focus_event);
TreeView::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* gdk_event);
TreeView::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
CellRendererToggle::toggled
  virtual void on_toggled(const Glib::ustring& path);
CellRendererText::edited
  virtual void on_edited(const Glib::ustring& path, const Glib::ustring& new_text);
EntryCompletion::match_selected
  virtual bool on_match_selected(const Gtk::TreeModel::iterator& iter);
SearchEntry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
SpinButton::value_changed
  virtual void on_value_changed();
Window::map
  virtual void on_map();
RadioMenuItem::toggled
  virtual void on_toggled();
Revealer::map
  virtual void on_map();
Revealer::unmap
  virtual void on_unmap();
Button::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Button::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
Button::motion_notify_event
  virtual bool on_motion_notify_event(GdkEventMotion* motion_event);
RadioButton::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
ToggleButton::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
RadioMenuItem::toggled
  virtual void on_toggled();
ScrolledWindow::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Dialog::delete_event
  virtual bool on_delete_event(GdkEventAny* any_event);
SpinButton::value_changed
  virtual void on_value_changed();
SearchEntry::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* focus_event);
SearchEntry::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* gdk_event);
SearchEntry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
SearchEntry::key_release_event
  virtual bool on_key_release_event(GdkEventKey* key_event);
SearchEntry::icon_release
  virtual void on_icon_release(Gtk::EntryIconPosition icon_position, const GdkEventButton* event);
SearchEntry::insert_text
  virtual void on_insert_text(const Glib::ustring& text, int* position);
Viewport::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
Viewport::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
Viewport::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
Viewport::motion_notify_event
  virtual bool on_motion_notify_event(GdkEventMotion* motion_event);
Fixed::draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
TextBuffer::mark_set
  virtual void on_mark_set(const Gtk::TextBuffer::iterator& location, const Glib::RefPtr<Gtk::TextBuffer::Mark>& mark);
Notebook::page_reordered
  virtual void on_page_reordered(Gtk::Widget* page, guint page_num);
TreeView::draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
DrawingArea::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
DrawingArea::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
DrawingArea::configure_event
  virtual bool on_configure_event(GdkEventConfigure* configure_event);
DrawingArea::draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
DrawingArea::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* focus_event);
DrawingArea::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* gdk_event);
DrawingArea::key_press_event
  virtual bool on_key_press_event(GdkEventKey* key_event);
DrawingArea::key_release_event
  virtual bool on_key_release_event(GdkEventKey* key_event);
DrawingArea::motion_notify_event
  virtual bool on_motion_notify_event(GdkEventMotion* motion_event);
DrawingArea::scroll_event
  virtual bool on_scroll_event(GdkEventScroll* scroll_event);
TreeView::row_expanded
  virtual void on_row_expanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
ComboBox::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
ComboBox::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
ComboBox::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
ComboBox::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
ComboBox::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
ComboBox::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
ComboBox::button_press_event
  virtual bool on_button_press_event(GdkEventButton* button_event);
ComboBox::button_release_event
  virtual bool on_button_release_event(GdkEventButton* release_event);
ComboBox::changed
   virtual void on_changed();
Entry::key_release_event
  virtual bool on_key_release_event(GdkEventKey* key_event);
TreeView::row_collapsed
  virtual void on_row_collapsed(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
TreeView::drag_data_get
  virtual void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
ScrolledWindow::size_allocate
  virtual void on_size_allocate(Gtk::Allocation& allocation);
TextView::size_allocate
  virtual void on_size_allocate(Gtk::Allocation& allocation);
RadioAction::toggled
  virtual void on_toggled();
