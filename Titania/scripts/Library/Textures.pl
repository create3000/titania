#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use File::Basename qw (dirname basename);

my $script  = dirname $0;
my $project = "$script/../../";
my $folder  = "$project/share/titania/Library/Textures/";


my $textures = "/home/holger/Projekte/Library/Textures/";


my @url = (
	"http://cdn.rawgit.com/create3000/Library/master/Textures",
	"https://cdn.rawgit.com/create3000/Library/master/Textures",
	"http://rawgit.com/create3000/Library/master/Textures",
	"https://rawgit.com/create3000/Library/master/Textures",
);

sub textures {
	my $filename = shift;
	chomp $filename;

	return if not -f $filename;

	say $filename;

	my $subfolder = $filename;

	$subfolder =~ s/$textures//;
	$subfolder = dirname $subfolder;

	my $basename  = basename $filename;
	my $name      = basename $filename, ".jpg", ".png";
	my $x3dv      ="$folder/$subfolder/$name.x3dv";

	my $url = "";

	for (@url)
	{
		$url .= "\"$_/$subfolder/$basename\",";
	}

	system "mkdir", "-p", "$folder/$subfolder/";

	my $file = `x3dtidy '$filename'`;

	$file =~ s/url\s+\[.*?\]/url [ $url ]/sgo;

	open FILE, ">", $x3dv;
	say FILE $file;
	close FILE;

	say $x3dv;

	system "x3dtidy", $x3dv, $x3dv;
}

textures $_ foreach `find $textures`;
