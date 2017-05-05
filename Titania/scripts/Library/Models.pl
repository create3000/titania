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


my $models = "/home/holger/Projekte/Library/Models/";


my @url = (
	"http://cdn.rawgit.com/create3000/Library/master/Models",
	"https://cdn.rawgit.com/create3000/Library/master/Models",
	"http://rawgit.com/create3000/Library/master/Models",
	"https://rawgit.com/create3000/Library/master/Models",
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
	my $name      = basename $filename, ".wrl", ".x3dv", ".x3d", ".wrz", ".x3dvz", ".x3dz";
	my $x3dv      ="$folder/$subfolder/$name.x3dv";

	$name =~ s/[\.]/-/sgo;
	$name =~ s/^(\d)/n$1/sgo;

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

	system "titania", $x3dv, "-e", $x3dv;
}

models $_ foreach `find $models -name \*.x3d`;
models $_ foreach `find $models -name \*.x3dz`;
models $_ foreach `find $models -name \*.x3dv`;
models $_ foreach `find $models -name \*.x3dvz`;
models $_ foreach `find $models -name \*.wrl`;
models $_ foreach `find $models -name \*.wrz`;
