#!/usr/bin/perl

use strict;
use warnings;
use v5.10.0;

use File::Basename qw(basename);

sub namespace {
	my $file = shift;
	my $dir  = shift || $ENV {HOME};
	chomp $file;
	chomp $dir;

	#say $file;
	
	my $contents = `cat '$file'`;
	
	my $path = $file;
	
	$path =~ s/$dir//;
	$path =~ s|^.*?/||;
	$path =~ s|[/\.-]|_|sg;
	$path =~ s|x3d|X3D|sg;
	$path =~ s|(_\w)|uc($1)|sge;
	$path =~ s|titania|Titania|sg;
	$path =~ s|([a-z])([A-Z])|$1_$2|sg;

	my $headerId = uc "__${path}__";

	$contents =~ /#ifndef\s+(.+?)\n/;
	my $oldHeaderId = $1;
	
	return say STDERR $file, ": no header guards found!" unless $oldHeaderId;
	
	$contents =~ /#define\s+(.+?)\n/;
	my $oldHeaderId2 = $1;

	$contents =~ s/#ifndef\s+($oldHeaderId)/#ifndef $headerId/;
	$contents =~ s/#define\s+($oldHeaderId)/#define $headerId/;

	#say $contents;

	open FILE, ">$file";
	print FILE $contents;
	close FILE;

	#exit;
}

my $dir = shift @ARGV;
namespace $_, $dir foreach @ARGV;

