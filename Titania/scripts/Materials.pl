#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

my $materials = "/home/holger/Projekte/Titania/Library/Materials";
my $library   = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Materials";

foreach my $filename (`ls -C1 '$materials'`)
{
	print $filename;	
	chomp $filename;

	$filename =~ /(.*?)\.(?:wrl|x3dv)$/sgo;
	my $collection = ucfirst ($1);

	say $collection;
	system "mkdir", "-p", "$library/$collection";

	my $file      = join "", `cat '$materials/$filename'`;
	my @materials = ( $file =~ /(Material\s*{.*?\})/sgo );
	
	my $i = 1;
	
	foreach my $material (@materials)
	{
		my $filename = "$library/$collection/$collection$i.x3dv";
	
		open FILE, ">", $filename;
		
		say FILE "#X3D V3.3 utf8 Titania

META \"titania magic\" \"Material\"

Transform {
  children Shape {
    appearance Appearance {
    material DEF $collection$i $material
  }
  geometry Sphere { }
  }
}
";
	
		close FILE;

		system "x3dtidy", $filename, $filename;

		++ $i;	
	}
}
