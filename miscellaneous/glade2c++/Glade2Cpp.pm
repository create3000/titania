#!/usr/bin/perl
package Glade2Cpp;
use strict;
use warnings;
use v5.10.0;

use XML::Parser;
use File::Basename qw (basename dirname);
use File::Spec;

use constant true  => 1;
use constant false => 0;

my %windows = ();

$windows {$_} = true foreach qw(
	Gtk::Window
	Gtk::FileChooserDialog
	Gtk::MessageDialog
	Gtk::Dialog
);

my %objects = ();

$objects {$_} = true foreach qw(
	Gtk::FileFilter
	Gtk::AccelGroup
	Gtk::Action
	Gtk::Adjustment
	Gtk::IconFactory
	Gtk::RadioAction
	Gtk::RecentAction
	Gtk::ToggleAction
	Gtk::ListStore
	Gtk::TreeStore
	Gtk::TextBuffer
	Gtk::TreeViewColumn
	Gtk::CellRendererText
	Gtk::CellRendererPixbuf
);

sub new
{
	my ($class, $options) = @_;
	my $self = {
		namespaces         => exists $options -> {namespaces}   ? $options -> {namespaces}   : [ ],
		base_class         => exists $options -> {base_class}   ? $options -> {base_class}   : "",
		class_prefix       => exists $options -> {class_prefix} ? $options -> {class_prefix} : "",
		class_suffix       => exists $options -> {class_suffix} ? $options -> {class_suffix} : "",
		suffixes           => [ qw (.glade .ui .xml) ],
		pure_virtual       =>  exists $options -> {pure_virtual} ? $options -> {pure_virtual} : true,
		h_signal_handler   => { },
		cpp_signal_handler => { },
		class              => "",
		object             => "",
		id                 => "",
		prototypes         => { grep { not /^\s*$/ } map { chomp; $_ } <DATA> },
		windows            => { },
	};
	bless $self, $class;
	return $self;
}

sub getPrototype
{
	my ($self, $name) = @_;
	
	return $self -> {prototypes} {$name} if exists $self -> {prototypes} {$name};

	my @result = `find /usr/include/gtkmm-3.0/gtkmm -name \\*.h -exec grep -I -P 'virtual.*?on_$name\\s*\\(' {} \\;`;
	my $handler = shift @result;
	
	chomp $handler;
	
	# Remember handler.
	$self -> {prototypes} {$name} = $handler;

	# Append handler to DATA section.
	open THIS, ">>", __FILE__;
	say THIS $name;
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

#	return unless ucfirst $attributes {id} eq $attributes {id};

sub h_object_getters
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isObject ($attributes {class});

	say $file "const Glib::RefPtr <$attributes{class}> &";
	say $file "get$attributes{id} () const\n{ return m_" . lcfirst ($attributes {id}) . "; }";
}

sub h_widget_getters
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});

	$self -> {windows} -> {"m_" . lcfirst ($attributes {id})} = 1
		if isWindow ($attributes {class});

	say $file "$attributes{class} &";
	say $file "get$attributes{id} () const\n{ return *m_" . lcfirst ($attributes {id}) . "; }";
}

sub h_objects
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isObject ($attributes {class});

	say $file "Glib::RefPtr <$attributes{class}> m_" . lcfirst ($attributes {id}) . ";";
}

sub h_widgets
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});

	say $file "$attributes{class}* m_" . lcfirst ($attributes {id}) . ";";
}

sub h_signal_handler
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};

	return if $name ne "signal";
	return if exists $self -> {h_signal_handler} {$attributes {handler}};
	
	$self -> {h_signal_handler} {$attributes {handler}} = 1;

	$attributes {name} =~ s/-/_/sgo;

	my $prototype = $self -> getPrototype ($attributes {name});
	
	unless ($prototype)
	{
		say "\t missing prototype for '", $attributes {name} . "'.";
		return;
	}

	$prototype =~ s/(virtual)\s/$1\n/s;
	$prototype =~ s/on_($attributes{name})/\n$attributes{handler}/s;
	$prototype =~ s/;/ = 0;/s if $self -> {pure_virtual};

	say $file $prototype;
}

sub cpp_get_objects
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isObject ($attributes {class});
	
	say $file "m_" . lcfirst ($attributes {id}) . " = Glib::RefPtr <$attributes{class}>::cast_dynamic (m_builder -> get_object (\"$attributes{id}\"));";
}

sub cpp_get_widgets
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});
	
	say $file "m_builder -> get_widget (\"$attributes{id}\", m_" . lcfirst ($attributes {id}) . ");";
	say $file "m_" . lcfirst ($attributes {id}) . " -> set_name (\"" . $attributes {id} . "\");";
}

sub cpp_signals
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	if ($name eq "object")
	{
		$self -> {class} = $attributes {class};
		$self -> {id}    = $attributes {id};

		$self -> {class} =~ s/Gtk/Gtk::/;
		return;
	}
	
	return if $name ne "signal";
	
	if ($self -> {object} ne $self -> {class})
	{
		$self -> {object} = $self -> {class};
		
		say $file "";
		say $file "// Connect object $self->{class} with id '$self->{id}'.";
	}
	
	$attributes {name} =~ s/-/_/sgo;
	
	my $signal = "m_" . lcfirst ($self -> {id}) . " -> signal_$attributes{name} ()";

	if (exists $attributes {after})
	{
		my $after = $attributes {after} eq "yes" ? "false" : "true";
		say $file "connections .emplace_back ($signal .connect (sigc::mem_fun (*this, &$self->{class_name}\:\:$attributes{handler}), $after));";
	}
	else
	{
		say $file "connections .emplace_back ($signal .connect (sigc::mem_fun (*this, &$self->{class_name}\:\:$attributes{handler})));";
	}
}

sub cpp_disconnect_signals
{
	my ($self) = @_;
	my $file = $self -> {handle};
	
	say $file "for (auto & connection : connections)";
	say $file "   connection .disconnect ();";
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
	$prototype =~ s/\s*virtual\s+(\w+)/\\t$1\\n\\t$self->{class_name}::/s;
	$prototype =~ s/\(/ (/s;

	my $function  = $self -> getPrototype ($attributes {name});
	$function =~ s/^\s+//s;
	$function =~ s/;//s;
	$function =~ s/on_($attributes{name})/$attributes{handler}/s;
	$function =~ s/virtual\s+(\w+)\s+/$1\n$self->{class_name}::/s;
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

	$self -> {h_signal_handler}   = { };
	$self -> {cpp_signal_handler} = { };
	$self -> {class_name}         = "$self->{class_prefix}${name}$self->{class_suffix}";
	
	say $self -> {class_name};

	my $h_tmp   = "/tmp/glad2cpp.$name.h";
	my $h_out   = "$directory/$self->{class_name}.h";

	my $cpp_tmp = "/tmp/glad2cpp.$name.cpp";
	my $cpp_out = "$directory/$self->{class_name}.cpp";

	my $base_class_name = $self -> {base_class} ? basename $self -> {base_class}, ".h" : "";
	my $base_path       = File::Spec -> abs2rel ($self -> {base_class}, dirname $h_out) ;

	my $input = get_file ($filename);


	# Header file
	open OUT, ">", $h_tmp;
	$self -> {handle} = *OUT;

	# Includes
	say OUT "";
	say OUT "#ifndef " . uc "_$name\_$self->{class_name}\_H_";
	say OUT "#define " . uc "_$name\_$self->{class_name}\_H_";

	say OUT "";
	say OUT "#include <gtkmm.h>";
	say OUT "#include <string>";
	say OUT "#include \"$base_path\"" if $base_class_name;
	say OUT "";
	
	# Namespace
	say OUT "namespace $_ {" foreach @{$self -> {namespaces}};
	say OUT "";

	# Using namespace
	say OUT "using namespace Gtk;";
	say OUT "";

	# Class
	say OUT "class $self->{class_name}";
	say OUT ": public $base_class_name" if $base_class_name;
	say OUT "{";

	# Public section
	say OUT "public:";

	# Constructor
	say OUT "  template <class ... Arguments>";
	say OUT "  $self->{class_name} (const std::string & filename, const Arguments & ... arguments) :";
	
	# Call base class construtor if any.
	if ($base_class_name) {
		say OUT "   $base_class_name (m_widgetName, arguments ...),";
	}
	
	say OUT "filename (filename),";
	say OUT "connections ()";

	# Constructor end begin body
	say OUT "{ create (filename); }";
	say OUT "";

	# Builder
	say OUT "  const Glib::RefPtr <Gtk::Builder> & getBuilder () const { return m_builder; }";
	say OUT "";

	# Name
	say OUT "  const std::string & getWidgetName () const { return m_widgetName; }";
	say OUT "";
	
	# updateWidget
	say OUT "  void updateWidget (const std::string & name) const";
	say OUT "  { getBuilder () -> add_from_file (filename, name); }";
	say OUT "";

	# getWidget
	say OUT "  template <class Type>";
	say OUT "  Type* getWidget (const std::string & name) const";
	say OUT "  {";
	say OUT "      Type* widget = nullptr;";
	say OUT "      m_builder -> get_widget (name, widget);";
	say OUT "      widget -> set_name (name);";
	say OUT "      return widget;";
	say OUT "  }";
	say OUT "";

	# Object getters
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_object_getters (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Widget getters
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_widget_getters (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');

	# Virtual signal handlers
	say OUT "";
	$parser = new XML::Parser (Handlers => {Start => sub { $self -> h_signal_handler (@_) }});
	$parser -> parse ($input, ProtocolEncoding => 'UTF-8');
	
	# Dispose
	#say OUT "  virtual";
	#say OUT "  void";
	#say OUT "  dispose ();";

	# Destructor
	say OUT "  ~$self->{class_name} ();";
	say OUT "";

#	say OUT "protected:";
#	say OUT "Glib::RefPtr <Gtk::Builder> &";
#	say OUT "getBuilder () const { return m_builder; }";

	# Private section
	say OUT "private:";
	say OUT "";
	
	say OUT "  void";
	say OUT "  create (const std::string &);";
	say OUT "";

	say OUT "  static const std::string m_widgetName;";
	say OUT "";

	say OUT "  std::string filename;";
	say OUT "  std::deque <sigc::connection> connections;";
	say OUT "  Glib::RefPtr <Gtk::Builder> m_builder;";

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
	system "mv", $h_tmp, $h_out
		if get_file ($h_tmp) ne get_file ($h_out);


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

	# Static members
	say OUT "const std::string $self->{class_name}\::m_widgetName = \"$name\";";
	say OUT "";

	# create
	say OUT "void";
	say OUT "$self->{class_name}\::create (const std::string & filename)";
	say OUT "{";
	
	# Initialize members
	say OUT "// Create Builder.";
	say OUT "m_builder = Gtk::Builder::create_from_file (filename);";
	say OUT "";

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

	# Call construct
	if ($base_class_name)
	{
		say OUT "";
		say OUT "// Call construct handler of base class.";
		say OUT "construct ();";
	}

	# Constructor end
	say OUT "}";
	say OUT "";

	# Signal handler outlines
	unless ($self -> {pure_virtual})
	{
		$parser = new XML::Parser (Handlers => {Start => sub { $self -> cpp_signal_handler (@_) }});
		$parser -> parse ($input, ProtocolEncoding => 'UTF-8');
	}
	
	# Dispose
	#say OUT "void";
	#say OUT "$self->{class_name}\::dispose ()";
	#say OUT "{";
	#$self -> cpp_disconnect_signals ();	
	#say OUT "}";

	# Destructor
	say OUT "$self->{class_name}\::~$self->{class_name} ()";
	say OUT "{";
	say OUT "delete $_;" foreach keys %{$self->{windows}};
	say OUT "}";

	# Namespaces end
	say OUT "";
	say OUT "} // $_ " foreach reverse @{$self -> {namespaces}};

	close OUT;

	#print get_file ($cpp_tmp);

	system "call_uncrustify.sh", $cpp_tmp;
	system "mv", $cpp_tmp, $cpp_out
		if get_file ($cpp_tmp) ne get_file ($cpp_out);
}



sub get_file
{
	my $filename = shift;
	return (join '', `cat '$filename'`);
}

1;
__DATA__
response
  virtual void on_response(int response_id);
map_event
  virtual bool on_map_event(GdkEventAny* event);
delete_event
  virtual bool on_delete_event(GdkEventAny* event);
unmap_event
  virtual bool on_unmap_event(GdkEventAny* event);
activate
  virtual void on_activate();
toggled
  virtual void on_toggled();
clicked
  virtual void on_clicked();
editing_done
  virtual void on_editing_done();
value_changed
  virtual void on_value_changed();
row_activated
  virtual void on_row_activated(const TreeModel::Path& path, TreeViewColumn* column);
icon_release
  virtual void on_icon_release(EntryIconPosition icon_position, const GdkEventButton* event);
select
  virtual void on_select();
realize
  virtual void on_realize();
map
  virtual void on_map();
unmap
  virtual void on_unmap();
show
  virtual void on_show();
switch_page
  virtual void on_switch_page(Widget* page, guint page_num);
page_reordered
  virtual void on_page_reordered(Widget* page, guint page_num);
page_added
  virtual void on_page_added(Widget* page, guint page_num);
page_removed
  virtual void on_page_removed(Widget* page, guint page_num);
key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
key_release_event
  virtual bool on_key_release_event(GdkEventKey* event);
drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const SelectionData& selection_data, guint info, guint time);
button_release_event
  virtual bool on_button_release_event(GdkEventButton* event);
draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
changed
  virtual void on_changed();

