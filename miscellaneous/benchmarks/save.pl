#!/usr/bin/perl

use strict;
use v5.10.0;

use LWP::Simple;

sub find {
	my $name = "";

	while (<>)
	{
		print $_; $| = 1;
		$name = $1, last if $_ =~ /^Statistics for: (.*)$/;
	}

	save ($name) if $name;
}

sub save {
	my $name = shift;
	chomp $name;
	say "Saving statistics for: ", $name;

	# Load Time
	my $line = <>;
	print $line; $| = 1;

	$line =~ m/Load Time:\s*([\d.]+)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Load Time)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# FPS
	my $line = <>;
	print $line; $| = 1;

	$line =~ m/FPS:\s*([\d.]+)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (FPS)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";
}

########################################################################################################################

find () while <>;
