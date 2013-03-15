#!/usr/bin/perl
package Glade2Cpp;
use strict;
use warnings;
use v5.10.0;

use XML::Parser;
use File::Basename qw (basename dirname);

sub parse {
	my (%xml) = @_;
	
	X3D (@{$xml {X3D}});
}

sub X3D {
	my ($attributes, %tags) = @_;
	
	#head ($tags {head});
	
	tags (%tags);	
}

sub tags {
	while (@_)
	{
		my $tag     = shift;
		my $content = shift;

		if ($tag)
		{
			tag ($tag, @$content);
		}
		else
		{
			# Text node
		}
	}
}

sub tag {
	my ($name, $attributes, @tags) = @_;
	
	say ": ", $name;
	
	tags (@tags);
}

eval {
	my $parser = new XML::Parser (Style => 'Tree');
	my $xml    = $parser -> parse (*STDIN);

	parse (@$xml);
};

print STDERR $@ if $@;

