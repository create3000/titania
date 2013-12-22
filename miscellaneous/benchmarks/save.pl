#!/usr/bin/perl

use strict;
use v5.10.0;

use LWP::Simple;

open LOG, ">>", "$ENV{HOME}/.xsession-errors";

sub debug {
	print @_;
	print LOG @_;

	$| = 1;
	select ((select (LOG), $|=1) [0])
}

sub find {
	my $name = "";

	debug (@_);

	while (<>)
	{
		debug ($_);
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
	debug ($line);

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
	debug ($line);

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

find ($_) while <>;

close LOG;
