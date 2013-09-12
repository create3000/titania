#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;

$ENV {XDG_DATA_DIRS} = "/home/holger/Projekte/Titania/Titania/share:/home/holger/Projekte/Titania/libtitania-x3d/share:$ENV{XDG_DATA_DIRS}";

system "./x3dtidy", @ARGV;
