#!/usr/bin/perl
package X3D::Parser;
use strict;
use warnings;
use v5.10.0;

use XML::Parser;
use XML::DTDParser;
use LWP::Simple;

use Data::Dumper;


sub new
{
	my ($class) = @_;
	
	chdir "/home/holger/Projekte/Titania/miscellaneous/logo";

	my $self = {
		dtd => ParseDTD get ("file:///home/holger/Arbeitsfläche/x3d-3.3.dtd")
	};
	
	say Dumper ($self -> {dtd});

	bless $self, $class;
	return $self;
}

sub parse {
	my ($this, %xml) = @_;
	
	$this -> X3D (@{$xml {X3D}});
}

sub X3D {
	my ($this, $attributes, %tags) = @_;
	
	#head ($tags {head});
	
	$this -> tags (%tags);	
}

sub tags {
	my $this = shift;

	while (@_)
	{
		my $tag     = shift;
		my $content = shift;

		if ($tag)
		{
			$this -> tag ($tag, @$content);
		}
		else
		{
			# Text node
		}
	}
}

sub tag {
	my ($this, $name, $attributes, @tags) = @_;
	
	say ": ", $name;
	
	say ": ", $this -> {dtd} -> {$name};
	
	$this -> tags (@tags);
}


1;
package main;

open FILE, "/home/holger/Projekte/Titania/miscellaneous/logo/logo.x3d";

eval {
	my $parser = new XML::Parser (Style => 'Tree');
	my $xml    = $parser -> parse (*FILE);

	new X3D::Parser () -> parse (@$xml);
};

close FILE;

print STDERR $@ if $@;

