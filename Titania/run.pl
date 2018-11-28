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

if (($min > 16 and $min < 20) or 0)
{
	unless (-e "$HOME/.config/Titania.O")
	{
		system "mv", "$HOME/.config/Titania", "$HOME/.config/Titania.O";
	}
}

$ENV {PATH}            = "$SOLUTION_DIR/Titania:$ENV{PATH}";
$ENV {LD_LIBRARY_PATH} = "/usr/local/lib";
$ENV {XDG_DATA_DIRS}   = "$SOLUTION_DIR/Titania/share:$SOLUTION_DIR/libtitania-x3d/share:$ENV{XDG_DATA_DIRS}";
$ENV {GDK_SYNCHRONIZE} = "1";

my $dirname = dirname $0;

system "$dirname/titania", @ARGV;

if (-d "$HOME/.config/Titania.O")
{
	system "rm", "-r", "$HOME/.config/Titania";
	system "mv", "$HOME/.config/Titania.O", "$HOME/.config/Titania";
}
