#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;

use Glib;

my $keyFile = new Glib::KeyFile ();
$keyFile -> load_from_data (join ("", `titania-info -f 2>/dev/null`), "none");

foreach my $name ($keyFile -> get_groups ())
{
	say $name;

	next if $name eq "Fields.pl";

	my $prototypes = "/home/holger/Projekte/Library/Prototypes/Fields";

	system "mkdir", "-p", $prototypes;

	my $value = $keyFile -> get_string ($name, "value");
	my $proto = "$prototypes/$name.x3dv";

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


	my $extern_prototypes = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Prototypes/Fields";

	system "mkdir", "-p", $extern_prototypes;


	my $x3dv = "$extern_prototypes/$name.x3dv";

	open FILE, ">", $x3dv;

	say FILE "#X3D V3.3 utf8 Titania

EXTERNPROTO $name [
  eventIn      SFTime set_triggerTime
  exposedField $name  keyValue
  eventOut     $name  value_changed
]
[
   \"https://cdn.rawgit.com/create3000/Library/1.0.2/Prototypes/Fields/$name.x3dv\",
   \"http://cdn.rawgit.com/create3000/Library/1.0.2/Prototypes/Fields/$name.x3dv\",
   \"https://rawgit.com/create3000/Library/1.0.2/Prototypes/Fields/$name.x3dv\",
   \"http://rawgit.com/create3000/Library/1.0.2/Prototypes/Fields/$name.x3dv\",
]

$name { }
";

	close FILE;

	system "x3dtidy", $x3dv, $x3dv;
}
