#!/usr/bin/perl

use strict;
use v5.10.0;

use LWP::Simple;

sub find {
	my $name = "";

	while (<>)
	{
		print $_;
		$name = $1, last if $_ =~ /^Rendering statistics for: (.*)$/;
	}

	save ($name) if $name;
}

sub save {
	my $name = shift;
	chomp $name;
	say "Saving statistics for: ", $name;

	# Load Time
	my $line = <>;	$line =~ m/Load Time:\s*([\d.]+)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Load Time)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# App FPS
	my $line = <>;	$line =~ m/\(([\d.]+)\)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Application FPS)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# Render FPS
	my $line = <>;	$line =~ m/\(([\d.]+)\)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Renderer FPS)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# Event Time
	my $line = <>;	$line =~ m/\(([\d.]+)\)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Event Time)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# Traverse Time
	my $line = <>;	$line =~ m/\(([\d.]+)\)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Traverse Time)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";

	# Draw Time
	my $line = <>;	$line =~ m/\(([\d.]+)\)/; my $value = $1;
	return unless $value;
	my $message = "id=d1880b304f26c14a40be0fc0cd3273d76aa6634c"
		. "&" . "name=$name (Draw Time)"
		. "&" . "durations[]=$value"
	;
	#say $message;
	get "http://library.modulejs.org/module/Test/Benchmark/store.php?$message";
}

########################################################################################################################

find () while <>;
