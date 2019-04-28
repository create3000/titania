#!/usr/bin/perl

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

my $browser = `cat ../libtitania-x3d/Titania/X3D/Browser/X3DBrowser.cpp`;

$browser =~ /version\s*=\s*"(.*?)"/;

say $1;
