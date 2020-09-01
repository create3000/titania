#!/usr/bin/perl

use strict;
use warnings;
use v5.10.0;
use utf8;
use open qw/:std :utf8/;

sub packages
{
    my $target = shift;
    my $yaml   = shift;

    $yaml =~ /$target:.*?\n((?:\s*-.*?\n)+)/sg;

    my $match = $1;

    $match =~ s/-\s//sgo;
    $match =~ s/#.*?\n//sgo;

    return grep /\w+/, split /\s+/, $match;
}

my $yaml     = `cat snap/snapcraft.yaml`;
my @packages = ();

push @packages, packages ("build-packages", $yaml);
push @packages, packages ("stage-packages", $yaml);

system "sudo", "apt-get", "install", @packages;
