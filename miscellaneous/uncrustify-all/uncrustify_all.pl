#!/usr/bin/perl

use strict;
use warnings;
use v5.10.0;
use open qw/:std :utf8/;

my $dir = "-/home/holger/Projekte/Titania/libtitania-x3d/Titania/X3D";

$dir = $ARGV [0] if @ARGV and -d $ARGV [0];

sub namespace {
	my $file = shift;
	chomp $file;

	say $file;
	system "call_uncrustify.sh '$file'";

	#exit;
}

namespace $_ foreach `find $dir -name \\*.cpp -exec echo {} \\;`;
namespace $_ foreach `find $dir -name \\*.h   -exec echo {} \\;`;

__END__
