#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use File::Basename qw (dirname basename);

my $script      = dirname $0;
my $project     = "$script/../../";
my $geometry    = `cat '$script/CubeMapTextures-Geometry.x3dv'`;
my $spheres     = "$project/share/titania/Library/CubeMapTextures/";
my $backgrounds = "$project/share/titania/Library/Backgrounds/Texture";


my $textures = "/home/holger/Projekte/Library/Textures/CubeMapTextures/";


my @url = (
	"http://cdn.rawgit.com/create3000/Library/1.0.2/Textures/CubeMapTextures",
	"https://cdn.rawgit.com/create3000/Library/1.0.2/Textures/CubeMapTextures",
	"http://rawgit.com/create3000/Library/1.0.2/Textures/CubeMapTextures",
	"https://rawgit.com/create3000/Library/1.0.2/Textures/CubeMapTextures",
);

sub url {
	my $name = shift;
	my $side = shift;
	my $url = "";

	for (@url)
	{
		$url .= "\"$_/$name/$name-$side.jpg\",";
	}

	return $url;
}

sub textures {
	my $filename = shift;
	chomp $filename;

	return if not -d $filename;
	return if $filename eq $textures;

	say $filename;

	my $basename   = basename $filename;
	my $sphere     = "$spheres/$basename.x3dv";
	my $background = "$backgrounds/$basename.x3dv";

	say $sphere;

	my $front  = url ($basename, "front");
	my $back   = url ($basename, "back");
	my $left   = url ($basename, "left");
	my $right  = url ($basename, "right");
	my $top    = url ($basename, "top");
	my $bottom = url ($basename, "bottom");

	system "mkdir", "-p", "$spheres/";
	system "mkdir", "-p", "$backgrounds/";

	open FILE, ">", $sphere;
	
	say FILE "
DEF $basename Transform {
  children Shape {
    appearance Appearance {
      material Material { }
      texture ComposedCubeMapTexture {
        front ImageTexture {
          url [ $front ]
        }
        back ImageTexture {
          url [ $back ]
        }
        left ImageTexture {
          url [ $left ]
        }
        right ImageTexture {
          url [ $right ]
        }
        top ImageTexture {
          url [ $top ]
        }
        bottom ImageTexture {
          url [ $bottom ]
        }
      }
    }
    geometry $geometry
}}
";

	close FILE;

	say $sphere;

	system "x3dtidy", $sphere, $sphere;






	open FILE, ">", $background;
	say FILE "
META \"titania magic\" \"Background\"

DEF $basename TextureBackground {
	frontTexture ImageTexture {
		url [ $front ]
		textureProperties DEF _0 TextureProperties {
			minificationFilter \"DEFAULT\"
			magnificationFilter \"DEFAULT\"
			boundaryModeS \"CLAMP_TO_EDGE\"
			boundaryModeT \"CLAMP_TO_EDGE\"
			boundaryModeR \"CLAMP_TO_EDGE\"
			textureCompression \"DEFAULT\"
		}
	}
	backTexture ImageTexture {
		url [ $back ]
		textureProperties USE _0
	}
	leftTexture ImageTexture {
		url [ $left ]
		textureProperties USE _0
	}
	rightTexture ImageTexture {
		url [ $right ]
		textureProperties USE _0
	}
	topTexture ImageTexture {
		url [ $top ]
		textureProperties USE _0
	}
	bottomTexture ImageTexture {
		url [ $bottom ]
		textureProperties USE _0
	}
}
";

	close FILE;

	system "x3dtidy", $background, $background;
}

textures $_ foreach `find $textures`;
