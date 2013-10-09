#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use File::Basename qw (dirname);

chdir dirname $0;

my $min = `date +'%M'`; chomp $min;

if ($min > 15 and $min < 20)
{
	say `gconftool-2 --dump /apps/titania > /home/holger/.gconf-titania-backup.xml`;
	system "gconftool-2", "--recursive-unset", "/apps/titania";

	unless (-e "/home/holger/.config/Titania.O")
	{
		system "mv", "/home/holger/.config/Titania", "/home/holger/.config/Titania.O";
	}
}

$ENV {UBUNTU_MENUPROXY} = "libappmenu.so";
$ENV {XDG_DATA_DIRS}    = "/home/holger/Projekte/Titania/Titania/share:/home/holger/Projekte/Titania/libtitania-x3d/share:$ENV{XDG_DATA_DIRS}";

system "./titania";

if (-d "/home/holger/.config/Titania.O")
{
	system "gconftool-2", "--recursive-unset", "/apps/titania";
	system "gconftool-2", "--load", "/home/holger/.gconf-titania-backup.xml";

	system "rm", "-r", "/home/holger/.config/Titania";
	system "mv", "/home/holger/.config/Titania.O", "/home/holger/.config/Titania";
}
