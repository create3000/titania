#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

my $min = `date +'%M'`; chomp $min;

if ($min > 15 and $min < 20)
{
	system "gconftool-2", "--recursive-unset", "/apps/titania";
	system "mv", "/home/holger/.config/Titania", "/home/holger/.config/Titania.O";
}

$ENV {XDG_DATA_DIRS} = "/home/holger/Projekte/Titania/Titania/share:/home/holger/Projekte/Titania/libtitania-x3d/share:$ENV{XDG_DATA_DIRS}";

system "./titania";

if (-d "/home/holger/.config/Titania.O")
{
	system "rm", "-r", "/home/holger/.config/Titania";
	system "mv", "/home/holger/.config/Titania.O", "/home/holger/.config/Titania";
}
