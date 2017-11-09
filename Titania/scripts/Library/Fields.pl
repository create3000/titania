#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;

use Glib;

my $keyFile = new Glib::KeyFile ();
$keyFile -> load_from_data (join ("", `titania -l fields`), "none");

foreach my $name ($keyFile -> get_groups ())
{
	say $name;

	next if $name eq "Fields.pl";

	my $prototypes = "/home/holger/Projekte/Library/Prototypes/Fields";

	system "mkdir", "-p", $prototypes;

	my $value     = $keyFile -> get_string ($name, "value");
	my $proto     = "$prototypes/$name.x3dv";
	my $proto_x3d = "$prototypes/$name.x3d";

	open FILE, ">", $proto;

	say FILE "#X3D V3.3 utf8 Titania

PROTO $name [
  inputOnly  SFTime   set_triggerTime
  inputOutput  $name  keyValue $value
  outputOnly   $name  value_changed
]
{
  DEF $name Script {
    inputOnly     SFTime set_triggerTime IS set_triggerTime
    inputOutput   $name  keyValue IS keyValue
    outputOnly    $name  value_changed IS value_changed

    url \"ecmascript:

function set_triggerTime (value, time)
{
	value_changed = keyValue;
}
\"
  }
}
";


	close FILE;

	system "titania", $proto, "-e", $proto;
	system "titania", $proto, "-e", $proto_x3d;


	my $extern_prototypes = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Prototypes/Fields";

	system "mkdir", "-p", $extern_prototypes;


	my $x3dv = "$extern_prototypes/$name.x3dv";

	open FILE, ">", $x3dv;

	say FILE "#X3D V3.3 utf8 Titania

EXTERNPROTO $name [
  inputOutput  SFTime triggerTime
  inputOutput  $name  keyValue
  outputOnly   $name  value_changed
]
[
   \"https://cdn.rawgit.com/create3000/Library/1.0.3/Prototypes/Fields/$name.x3d\",
   \"http://cdn.rawgit.com/create3000/Library/1.0.3/Prototypes/Fields/$name.x3d\",
   \"https://rawgit.com/create3000/Library/1.0.3/Prototypes/Fields/$name.x3d\",
   \"http://rawgit.com/create3000/Library/1.0.3/Prototypes/Fields/$name.x3d\",
]

$name { }
";

	close FILE;

	system "titania", $x3dv, "-e", $x3dv;
}
