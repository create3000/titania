#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use File::Basename qw (dirname);

my $HOME         = $ENV{HOME};
my $PROJECT_DIR  = dirname $0;
my $SOLUTION_DIR = "$PROJECT_DIR/../";

my $min = `date +'%M'`; chomp $min;

if (($min > 16 and $min < 200) or 0)
{
	unless (-e "$HOME/.config/Titania.O")
	{
		system "mv", "$HOME/.config/Titania", "$HOME/.config/Titania.O";
	}
}

#$ENV {LIBOVERLAY_SCROLLBAR} = 0;
$ENV {GTK_MODULES}      = "overlay-scrollbar:unity-gtk-module";
$ENV {UBUNTU_MENUPROXY} = 1;
$ENV {PATH}             = "$SOLUTION_DIR/x3d2vrml:$SOLUTION_DIR/x3ddidy:$SOLUTION_DIR/titania-info:$SOLUTION_DIR/x3d2vrml/bin:$SOLUTION_DIR/x3dtidy:$ENV{PATH}";
$ENV {XDG_DATA_DIRS}    = "$SOLUTION_DIR/x3d2vrml/share:$SOLUTION_DIR/Titania/share:$SOLUTION_DIR/libtitania-x3d/share:$SOLUTION_DIR/x3d2vrml/share:$ENV{XDG_DATA_DIRS}";
#$ENV {LANG} = "C";

chdir dirname $0;
system "./titania", @ARGV;

if (-d "$HOME/.config/Titania.O")
{
	system "rm", "-r", "$HOME/.config/Titania";
	system "mv", "$HOME/.config/Titania.O", "$HOME/.config/Titania";
}
