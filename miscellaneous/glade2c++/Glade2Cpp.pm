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
	Gtk::TreeSelection
	Gtk::TreeViewColumn
	Gtk::CellRendererText
	Gtk::CellRendererToggle
	Gtk::CellRendererPixbuf
	Gtk::TextBuffer
	Gtk::EntryCompletion
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
		empty_constructor  => $options -> {empty_constructor},
		pure_virtual       => exists $options -> {pure_virtual} ? $options -> {pure_virtual} : true,
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
	my ($self, $class, $name) = @_;

	return $self -> {prototypes} {"$class::$name"} if exists $self -> {prototypes} {"$class::$name"};

	my @result = `find /usr/include/gtkmm-3.0/gtkmm -name \\*.h -exec grep -I -P 'virtual.*?on_$name\\s*\\(' {} \\;`;
	my $handler = shift @result;
	
	chomp $handler;
	
	# Remember handler.
	$self -> {prototypes} {"$class::$name"} = $handler;

	# Append handler to DATA section.
	open THIS, ">>", __FILE__;
	say THIS "$class::$name";
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
	say $file "get$attributes{id} () const\n{ return m_" . $attributes {id} . "; }";
}

sub h_widget_getters
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});

	$self -> {windows} -> {"m_" . $attributes {id}} = 1
		if isWindow ($attributes {class});

	say $file "$attributes{class} &";
	say $file "get$attributes{id} () const\n{ return *m_" . $attributes {id} . "; }";
}

sub h_objects
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};

	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isObject ($attributes {class});

	say $file "Glib::RefPtr <$attributes{class}> m_" . $attributes {id} . ";";
}

sub h_widgets
{
	my ($self, $expat, $name, %attributes) = @_;
	my $file = $self -> {handle};
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});

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
	
	return if $name ne "object";
	return if ucfirst $attributes {id} ne $attributes {id};
	
	$attributes {class} =~ s/Gtk/Gtk::/;
	return if not isWidget ($attributes {class});
	
	say $file "m_builder -> get_widget (\"$attributes{id}\", m_" . $attributes {id} . ");";
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
	
	my $signal = "m_" . $self -> {id} . " -> signal_$attributes{name} ()";

	my $after   = exists $attributes {after} && $attributes {after} eq "yes" ? ", false" : "";
	my $swapped = exists $attributes {swapped} && $attributes {swapped} eq "yes";

	if ($swapped)
	{
		say $file "$signal .connect (sigc::bind (sigc::mem_fun (*this, &$self->{class_name}\:\:$attributes{handler}), sigc::ref (*m_" . $self -> {id} . ")) $after);";
	}
	else
	{
		say $file "$signal .connect (sigc::mem_fun (*this, &$self->{class_name}\:\:$attributes{handler}) $after);";
	}
}

sub cpp_disconnect_signals
{
	my ($self) = @_;
	my $file = $self -> {handle};
	
	say $file "for (auto & connection : m_connections)";
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
	$self -> {windows}            = { }; 

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

	# Class
	say OUT "class $self->{class_name}";
	say OUT ": public $base_class_name" if $base_class_name;
	say OUT "{";

	# Public section
	say OUT "public:";

	# Empty constructor
	if ($self->{empty_constructor})
	{
		say OUT "  $self->{class_name} () :";

		# Call base class construtor if any.
		if ($base_class_name) {
			say OUT "  $base_class_name ()";
			say OUT "  { }";
		}
	}

	say OUT "  template <class ... Arguments>";
	say OUT "  $self->{class_name} (const std::string & filename, const Arguments & ... arguments) :";
	
	# Call base class construtor if any.
	if ($base_class_name) {
		say OUT "  $base_class_name (m_widgetName, arguments ...),";
	}
	
	say OUT "   filename (filename)";

	# Constructor end begin body
	say OUT "  { create (filename); }";
	say OUT "";

	# Builder
	say OUT "  const Glib::RefPtr <Gtk::Builder> & getBuilder () const { return m_builder; }";
	say OUT "";

	# Name
	say OUT "  const std::string & getWidgetName () const { return m_widgetName; }";
	say OUT "";

	#say OUT "  void updateWidgets (const std::vector <Glib::ustring> & names) const";
	#say OUT "  { getBuilder () -> add_from_file (filename, names); }";
	#say OUT "";

	# getWidget
	say OUT "  template <class Type>";
	say OUT "  Type* createWidget (const std::string & name) const";
	say OUT "  {";
	say OUT "      getBuilder () -> add_from_file (filename, name);";
	say OUT "";
	say OUT "      Type* widget = nullptr;";
	say OUT "      m_builder -> get_widget (name, widget);";
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
	say OUT "  virtual";
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
	print compare ($h_tmp, $h_out) != 0, " ";

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
	say OUT "delete $_;" foreach keys %{$self -> {windows}};
	say OUT "}";

	# Namespaces end
	say OUT "";
	say OUT "} // $_ " foreach reverse @{$self -> {namespaces}};

	close OUT;

	#print get_file ($cpp_tmp);

	system "call_uncrustify.sh", $cpp_tmp;
	print compare ($cpp_tmp, $cpp_out) != 0, " ";

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
Box::map
  virtual void on_map();
ToggleToolButton::toggled
  virtual void on_toggled();
ToolButton::clicked
  virtual void on_clicked();
TreeView::row_activated
  virtual void on_row_activated(const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*);
Adjustment::value_changed
  virtual void on_value_changed();
Box::unmap
  virtual void on_unmap();
ComboBoxText::changed
  virtual void on_changed();
TreeView::draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
TreeSelection::changed
  virtual void on_changed();
CellRendererToggle::toggled
  virtual void on_toggled(const Glib::ustring& path);
DrawingArea::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
DrawingArea::button_release_event
  virtual bool on_button_release_event(GdkEventButton* event);
DrawingArea::configure_event
  virtual bool on_configure_event(GdkEventConfigure* event);
DrawingArea::draw
  virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
DrawingArea::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* event);
DrawingArea::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* event);
DrawingArea::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
DrawingArea::key_release_event
  virtual bool on_key_release_event(GdkEventKey* event);
DrawingArea::motion_notify_event
  virtual bool on_motion_notify_event(GdkEventMotion* event);
DrawingArea::scroll_event
  virtual bool on_scroll_event(GdkEventScroll* event);
Entry::changed
  virtual void on_changed();
Entry::delete_text
  virtual void on_delete_text(int start_pos, int end_pos);
Entry::insert_text
  virtual void on_insert_text(const Glib::ustring& text, int* position);
SpinButton::value_changed
  virtual void on_value_changed();
MenuItem::activate
  virtual void on_activate();
ImageMenuItem::activate
  virtual void on_activate();
CheckMenuItem::toggled
  virtual void on_toggled();
Button::clicked
  virtual void on_clicked();
ScrolledWindow::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
Dialog::delete_event
  virtual bool on_delete_event(GdkEventAny* event);
CheckButton::toggled
  virtual void on_toggled();
SearchEntry::focus_in_event
  virtual bool on_focus_in_event(GdkEventFocus* event);
SearchEntry::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* event);
SearchEntry::icon_release
  virtual void on_icon_release(Gtk::EntryIconPosition icon_position, const GdkEventButton* event);
SearchEntry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
SearchEntry::key_release_event
  virtual bool on_key_release_event(GdkEventKey* event);
ToggleButton::toggled
  virtual void on_toggled();
ToggleAction::toggled
  virtual void on_toggled();
RadioAction::changed
  virtual void on_changed(const Glib::RefPtr<Gtk::RadioAction>& current);
RadioAction::activate
  virtual void on_activate();
ApplicationWindow::focus_out_event
  virtual bool on_focus_out_event(GdkEventFocus* event);
ApplicationWindow::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
ApplicationWindow::key_release_event
  virtual bool on_key_release_event(GdkEventKey* event);
MenuBar::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
Box::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
ToolButton::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
Entry::icon_release
  virtual void on_icon_release(Gtk::EntryIconPosition icon_position, const GdkEventButton* event);
Entry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
Notebook::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
Notebook::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
Notebook::page_reordered
  virtual void on_page_reordered(Gtk::Widget* page, guint page_num);
Notebook::switch_page
  virtual void on_switch_page(Gtk::Widget* page, guint page_num);
RadioToolButton::toggled
  virtual void on_toggled();
MenuToolButton::clicked
  virtual void on_clicked();
EntryCompletion::match_selected
  virtual bool on_match_selected(const Gtk::TreeModel::iterator& iter);
SearchEntry::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
Box::key_press_event
  virtual bool on_key_press_event(GdkEventKey* event);
Box::key_release_event
  virtual bool on_key_release_event(GdkEventKey* event);
RadioToolButton::toggled
  virtual void on_toggled();
ToggleButton::toggled
  virtual void on_toggled();
EventBox::enter_notify_event
  virtual bool on_enter_notify_event(GdkEventCrossing* event);
RadioMenuItem::activate
  virtual void on_activate();
RadioButton::clicked
  virtual void on_clicked();
TextBuffer::changed
  virtual void on_changed();
TreeView::drag_data_received
  virtual void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
CellRendererText::edited
  virtual void on_edited(const Glib::ustring& path, const Glib::ustring& new_text);
RadioAction::toggled
  virtual void on_toggled();
RadioMenuItem::toggled
  virtual void on_toggled();
Box::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
TreeView::button_press_event
  virtual bool on_button_press_event(GdkEventButton* event);
TreeView::button_release_event
  virtual bool on_button_release_event(GdkEventButton* event);
Button::activate
  virtual void on_activate();
Box::style_updated
  virtual void on_style_updated();
