#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use File::Basename qw (dirname basename);

my $version = "1/1.0";
my $script  = dirname $0;
my $project = "$script/../../";
my $folder  = "$project/share/titania/Library/Models/";

my $models = "/home/holger/Projekte/Titania/Library/Models/$version/";

my @url = (
	"http://cdn.rawgit.com/create3000/titania/master/Library/Models/$version",
	"https://cdn.rawgit.com/create3000/titania/master/Library/Models/$version",
	"http://rawgit.com/create3000/titania/master/Library/Models/$version",
	"https://rawgit.com/create3000/titania/master/Library/Models/$version",
);

sub models {
	my $filename = shift;
	chomp $filename;

	return if not -f $filename;

	say $filename;

	my $subfolder = $filename;

	$subfolder =~ s/$models//;
	$subfolder = dirname $subfolder;

	my $basename  = basename $filename;
	my $name      = basename $filename, ".wrl", ".x3dv", ".x3d";
	my $x3dv      ="$folder/$subfolder/$name.x3dv";

	my $url = "";

	for (@url)
	{
		$url .= "\"$_/$subfolder/$basename\",";
	}

	system "mkdir", "-p", "$folder/$subfolder/";

	my $file = "DEF $name Transform { children Inline { url [ $url ] } }";

	open FILE, ">", $x3dv;
	say FILE $file;
	close FILE;

	say $x3dv;

	system "x3dtidy", $x3dv, $x3dv;
}

models $_ foreach `find $models -name \*.x3d`;
