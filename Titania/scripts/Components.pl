#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use Glib;
use File::Basename qw (dirname);

say "Generate Components";

my $titania     = "/home/holger/Projekte/Titania/Titania/run.pl";
my $components  = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Components";

my $nodes = new Glib::KeyFile ();

$nodes -> load_from_data (join ("", `$titania --list=nodes 2>/dev/null`), "none");

foreach my $node ($nodes -> get_groups ())
{
	my $componentName = $nodes -> get_string ($node, "componentName");

	next unless $componentName eq "VolumeRendering";

	say $node;

	system "mkdir", "-p", "$components/$componentName";

	open FILE, ">", "$components/$componentName/$node.x3dv";

	print FILE "#X3D V3.3 utf8 Titania

$node { }
";

	close FILE;
}
