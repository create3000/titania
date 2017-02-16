#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;

use Glib;

my $keyFile = new Glib::KeyFile ();
$keyFile -> load_from_data (join ("", `/home/holger/Projekte/Titania/Titania/run.pl --list=fields 2>/dev/null`), "none");

foreach my $group ($keyFile -> get_groups ())
{
	say $group;

	my @fields = $keyFile->get_keys ($group);

	for my $field (@fields)
	{
		say "\t$field";
	}


	#system "x3dtidy", $x3dv, $x3dv;
}
