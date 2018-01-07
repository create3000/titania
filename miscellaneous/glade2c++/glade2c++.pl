#!/usr/bin/perl
use strict;
use warnings;
use v5.10.0;

# -e  generate empty constructor
# -v  pure virtual functions
# -n  namespaces, can be use multiple times
# -p  class prefix
# -s  class suffix
# -b  path to base class
# -o  output directory

BEGIN {
	use File::Basename qw (dirname);
	push @INC, dirname ($0);
}

use Glade2Cpp;
use Getopt::Long;

my @namespaces        = ();
my $base_class        = "";
my $class_prefix      = "";
my $class_suffix      = "";
my $empty_constructor = 0;
my $pure_virtual      = 0;
my $derived_directory = $ENV {HOME};
my $output_directory  = $ENV {HOME};

sub main
{
	say "Starting glade2c++:";
	
	GetOptions ("namespace=s@"   => \@namespaces,
	            "base-class=s"   => \$base_class,
	            "prefix=s"       => \$class_prefix,
	            "suffix=s"       => \$class_suffix,
	            "empty"          => \$empty_constructor,
	            "virtual"        => \$pure_virtual,
	            "derived=s"      => \$derived_directory,
	            "output=s"       => \$output_directory);

	my $glade2cpp = new Glade2Cpp ({ namespaces        => [ split /,/, join ',', @namespaces ],
                                    base_class        => $base_class,
                                    class_prefix      => $class_prefix,
                                    class_suffix      => $class_suffix,
                                    empty_constructor => $empty_constructor,
	                                 pure_virtual      => $pure_virtual,
	                                 derived_directory => $derived_directory });

	$glade2cpp -> generate ($_, $output_directory) foreach @ARGV;

	say "glade2c++ done.";
}

main ();

__END__
	$glade2cpp -> generate ("/home/holger/Projekte/Titania/Puck/share/titania/puck/ui/Puck.glade",
	                        "/home/holger/Projekte/Titania/Puck/puck/Interfaces");

