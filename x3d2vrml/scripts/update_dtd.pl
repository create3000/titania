#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use File::Basename qw (dirname basename);
use LWP::Simple;

my $scripts = dirname $0;
my $folder  = "$scripts/../share/titania/dtd/";

my @dtd = qw (
	http://www.web3d.org/specifications/x3d-3.0.dtd
	http://www.web3d.org/specifications/x3d-3.1.dtd
	http://www.web3d.org/specifications/x3d-3.2.dtd
	http://www.web3d.org/specifications/x3d-3.3.dtd
);

foreach (@dtd)
{
	say $_;

	my $basename = basename $_;
	my $content  = get ($_);

	open FILE, ">", "$folder/$basename";
	say FILE $content;
	close FILE;
}

# XXX
# LayoutLayer containerField layers
