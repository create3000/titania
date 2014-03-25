#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;

use Glib;

my $fields = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Prototypes/Fields";

my $keyFile = new Glib::KeyFile ();
$keyFile -> load_from_data (join ("", `titania-info -f 2>/dev/null`), "none");

foreach my $name ($keyFile -> get_groups ())
{
	say $name;	

	next if $name eq "Fields.pl";

	my $folder = "$fields/$name";

	system "mkdir", "-p", $folder;

	my $value = $keyFile -> get_string ($name, "value");
	my $proto = "$fields/$name/$name.x3dv";

	open FILE, ">", $proto;

	say FILE "#X3D V3.3 utf8 Titania

PROTO $name [
  eventIn      SFTime set_triggerTime
  exposedField $name  keyValue $value
  eventOut     $name  value_changed
]
{
  DEF $name Script {
    eventIn       SFTime set_triggerTime IS set_triggerTime
    exposedField  $name  keyValue IS keyValue
    eventOut      $name  value_changed IS value_changed

    url \"javascript:
function set_triggerTime (value, time)
{
	value_changed = keyValue;
}
\"
  }
}
";

	close FILE;

	system "x3dtidy", $proto, $proto;
	
	
	
	my $externproto = "$fields/$name/ExternProto.x3dv";

	open FILE, ">", $externproto;

	say FILE "#X3D V3.3 utf8 Titania

EXTERNPROTO $name [
  eventIn      SFTime set_triggerTime
  exposedField $name  keyValue
  eventOut     $name  value_changed
]
[
	\"$name.x3dv\"
	\"/usr/share/titania/Library/Prototypes/Fields/$name/$name.x3dv\"
]

$name { }
";

	close FILE;

	system "x3dtidy", $externproto, $externproto;
}

