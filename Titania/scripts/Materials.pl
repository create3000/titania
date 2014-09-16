#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use List::Util qw (max);

my $materials = "/home/holger/X3D/development/data/materials";
#my $library   = "/home/holger/Projekte/Titania/Titania/share/titania/Library/Materials";
my $library   = "/home/holger/Schreibtisch/Materials";

foreach my $filename (`ls -C1 '$materials'`)
{
	print $filename;	
	chomp $filename;

	my $collection = ucfirst ($filename);

	say $collection;
	system "mkdir", "-p", "$library/$collection";
	
	foreach my $material (`ls -C1 '$materials/$filename'`)
	{
		chomp $material;

		$material =~ /.*?\.(\d+)/sgo;
		my $i = $1 + 1;

		my $file = join "", `cat '$materials/$filename/$material'`;

		say $file;

		$file =~ /\{(.*?)\}/sgo;
		my $values = $1;
		
		
		if ($values =~ /ambientColor\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)/sgo)
		{
			my $ambientIntensity = max ($1, $2, $3);
		
			$values =~ s/ambientColor.*?\n/ambientIntensity $ambientIntensity/sgo;
		}

		my $filename = "$library/$collection/$collection$i.x3dv";
	
		open FILE, ">", $filename;
		
		say FILE "#X3D V3.3 utf8 Titania

META \"titania magic\" \"Material\"

Transform {
  children Shape {
    appearance Appearance {
    material DEF $collection$i Material {
      $values
    }
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
