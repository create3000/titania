#!/usr/bin/perl
# -*- Mode: Perl; coding: utf-8; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*-

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

use Glib;
use List::MoreUtils qw(uniq);

my $f = Glib::KeyFile -> new;
my $d = `run --list nodes 2>/dev/null`;

$f -> load_from_data ($d, "none");

my @fields = ();

foreach my $node ($f -> get_groups ())
{
	next if $f -> get_string ($node, "componentName") =~ /Titania|X_ITE/;

	foreach my $field ($f -> get_keys ($node))
	{
		next if $field =~ /containerField|componentName|componentLevel/;
		
		push @fields, $field;
	}
}

@fields = sort {$a cmp $b} uniq @fields;

foreach my $field (@fields)
{
	say "\"$field\",";
}
