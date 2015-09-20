#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use utf8;

use Glib;

my $prototypes = "/home/holger/Projekte/Titania/Titania/share/titania/Prototypes/0.1/";

sub convert {
	my $x3dv = shift;
	chomp $x3dv;

	say $x3dv;

	my $x3d = $x3dv;
	$x3d =~ s/\.x3dv$/.x3d/;

	system "x3dtidy", $x3dv, $x3d;
}

convert $_ foreach `find $prototypes -name \*.x3dv`;
