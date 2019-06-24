#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use File::Basename qw (basename);

system "inkscape", "--file=$_", "--export-area-page", "--export-png=16x12/" . basename ($_, ".svg") . ".png" foreach <*.svg>;
system "cp", $_, "16x12"  foreach <*.png>;

