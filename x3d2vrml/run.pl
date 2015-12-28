#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

use File::Basename qw (dirname);

my $HOME         = $ENV{HOME};
my $PROJECT_DIR  = dirname $0;
my $SOLUTION_DIR = "$PROJECT_DIR/../";

chdir dirname $0;

$ENV {XDG_DATA_DIRS} = "$PROJECT_DIR/share:$ENV{XDG_DATA_DIRS}";

system "./bin/titania-x3d2vrml", @ARGV;
