#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

exit 1 if (system ("make", "version") != 0);

my $commit = `git rev-parse HEAD`;
chomp $commit;

chdir "..";
system "cp", "Titania/Titania/share/appdata/de.create3000.titania.appdata.xml", "de.create3000.titania/de.create3000.titania.appdata.xml";
system "cp", "Titania/Titania/share/applications/de.create3000.titania.desktop", "de.create3000.titania/de.create3000.titania.desktop";

chdir "de.create3000.titania";

my $json = `cat de.create3000.titania.json`;

$json =~ s/("name":\s*"titania".*?"commit":\s*")(.*?)(")/$1$commit$3/sg;

open JSON, ">", "de.create3000.titania.json";
print JSON $json;
close JSON;

system "git", "commit", "-am", "Updated FlatPak.";
system "git", "push", "origin", "master";

