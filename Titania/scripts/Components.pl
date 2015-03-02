#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use Glib;
use File::Basename qw (dirname);

say "Generate Components";

my $TitaniaInfo = "/home/holger/Projekte/Titania/titania-info/titania-info";
my $components  = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Components";

system "rm", "-r", $components if -e $components;
system "mkdir", "-p", $components;

my $nodes = new Glib::KeyFile ();
$nodes -> load_from_data (join ("", `$TitaniaInfo -i=nodes 2>/dev/null`), "none");

foreach my $node ($nodes -> get_groups ())
{
	say $node;

	my $componentName = $nodes -> get_string ($node, "componentName");

	system "mkdir", "-p", "$components/$componentName";

	open FILE, ">", "$components/$componentName/$node.x3dv";

	print FILE "#X3D V3.3 utf8 Titania

$node { }
";

	close FILE;
}
