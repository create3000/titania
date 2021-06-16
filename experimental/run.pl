#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use Getopt::Long;
use File::Basename qw (dirname);

$SIG {__WARN__} = sub {}; # catch unkown options

my $HOME         = $ENV {HOME};
my $PROJECT_DIR  = dirname $0;
my $SOLUTION_DIR = "$PROJECT_DIR/../";

$ENV {PATH}            = "$SOLUTION_DIR/Titania:$ENV{PATH}";
$ENV {LD_LIBRARY_PATH} = "/usr/local/lib";
$ENV {XDG_DATA_DIRS}   = "$SOLUTION_DIR/Titania/share:$SOLUTION_DIR/libtitania-x3d/share:$ENV{XDG_DATA_DIRS}";
$ENV {GDK_SYNCHRONIZE} = "1";
#$ENV {GTK_DEBUG}      = "interactive";

delete $ENV {GDK_PIXBUF_MODULE_FILE};
delete $ENV {GDK_PIXBUF_MODULEDIR};

my $dirname = dirname $0;

system "$dirname/experimental", @ARGV;
#system "lldb", "$dirname/experimental", @ARGV;
