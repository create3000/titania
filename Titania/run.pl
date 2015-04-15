#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use File::Basename qw (dirname);

my $HOME        = $ENV{HOME};
my $PROJECT_DIR = "/home/holger/Projekte/Titania";

my $min = `date +'%M'`; chomp $min;

if ($min > 16 and $min < 20)
{
	unless (-e "$HOME/.config/Titania.O")
	{
		say `gconftool-2 --dump /apps/titania > $HOME/.gconf-titania-backup.xml`;
		system "gconftool-2", "--recursive-unset", "/apps/titania";

		system "mv", "$HOME/.config/Titania", "$HOME/.config/Titania.O";
	}
}

#$ENV {LIBOVERLAY_SCROLLBAR} = 0;
$ENV {GTK_MODULES}      = "overlay-scrollbar:unity-gtk-module";
$ENV {UBUNTU_MENUPROXY} = 1;
$ENV {XDG_DATA_DIRS}    = "$PROJECT_DIR/Titania/share:$PROJECT_DIR/libtitania-x3d/share:$PROJECT_DIR/x3d2vrml/share:$ENV{XDG_DATA_DIRS}";
#$ENV {LANG} = "C";

chdir dirname $0;
system "./titania", @ARGV;

if (-d "$HOME/.config/Titania.O")
{
	system "gconftool-2", "--recursive-unset", "/apps/titania";
	system "gconftool-2", "--load", "$HOME/.gconf-titania-backup.xml";

	system "rm", "-r", "$HOME/.config/Titania";
	system "mv", "$HOME/.config/Titania.O", "$HOME/.config/Titania";
}
