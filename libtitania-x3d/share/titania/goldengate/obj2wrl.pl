#!/usr/bin/perl
# $Id$
#############################################################################
#                                                                           #
#  WHO:    John L. Moreland + fmb                                           #
#                                                                           #
#  WHAT:   obj2wrl                                                          #
#                                                                           #
#  WHY:    Converts a wavefront OBJ file to a VRML 2.0 file.                #
#                                                                           #
#  WHERE:  San Diego Supercomputer Center (SDSC) + GTI-UPM                  #
#                                                                           #
#  WHEN:   Wed Mar 15 13:00:23 PST 1995 (ORIGINAL)                          #
#          Fri Jul 14 15:50:20 UTC 2001 (CURRENT)                           #
#                                                                           #
#  HOW:    Perl                                                             #
#                                                                           #
#############################################################################
#                                                                           #
#  Copyright (c) 1995  San Diego Supercomputer Center (SDSC)                #
#       a division of General Atomics, San Diego, California, USA           #
#                                                                           #
#       Users and possessors of this source code are hereby granted a       #
#       nonexclusive, royalty-free copyright and design patent license to   #
#       use this code in individual software.  License is not granted for   #
#       commercial resale, in whole or in part, without prior written       #
#       permission from SDSC.  This source is provided "AS IS" without      #
#       express or implied warranty of any kind.                            #
#                                                                           #
#       For further information contact:                                    #
#               E-Mail:         info@sds.sdsc.edu                           #
#                                                                           #
#               Surface Mail:   Information Center                          #
#                               San Diego Supercomputer Center              #
#                               P.O. Box 85608                              #
#                               San Diego, CA  92138-5608                   #
#                               (619) 534-5000                              #
#                                                                           #
#############################################################################

# Found at:
# https://github.com/arnaudgelas/mesh/blob/master/lib3d/tools/obj2wrl.pl

#############################################################################
################################  SUBROUTINES  ##############################
#############################################################################


#############################################################################
#                                                                           #
#  Returns the maximum of all values passed in.                             #
#                                                                           #
#############################################################################
sub max
{
	local( $result ) = pop( @_ );
	foreach $item ( @_ )
	{
		$result = $item if $item > $result;
	}
	return( $result );
}


#############################################################################
#                                                                           #
#  Returns the minimum of all values passed in.                             #
#                                                                           #
#############################################################################
sub min
{
	local( $result ) = pop( @_ );
	foreach $item ( @_ )
	{
		$result = $item if $item < $result;
	}
	return( $result );
}


#############################################################################
#                                                                           #
#  Returns the absolute value of the number passed in.                      #
#                                                                           #
#############################################################################
sub abs
{
	local( $result ) = $_[0];
	$result *= -1 if $result < 0;
	return( $result );
}


#############################################################################
###################################  INIT  ##################################
#############################################################################


#############################################################################
# SET DEFAULT VARIABLE VALUES

$nLines = 0;	# Number of lines read from input file

$nVerts = 0;	# Number of verticies parsed out of the input file
$nPolys = 0;	# Number of face-sets parsed out of the input file
$nSNorms = 0;	# Number of surface-normals parsed out of the input file
$nTNorms = 0;	# Number of texture-normals parsed out of the input file

$input_name = "STDIN";		# The input file name
$output_name = "STDOUT";	# The output file name
$verbose_name = "/dev/null";	# The verbose-message file name

$x_max = "";		# Max of all X-Coordinate vertex values
$x_min = "";		# Min of all X-Coordinate vertex values
$y_max = "";		# Max of all Y-Coordinate vertex values
$y_min = "";		# Min of all Y-Coordinate vertex values
$z_max = "";		# Max of all Z-Coordinate vertex values
$z_min = "";		# Min of all Z-Coordinate vertex values
$x_range = 0.0;		# MaxX - MinX
$x_center = 0.0;	# (MaxX+MinX)/2
$y_range = 0.0;		# MaxY - MinY
$y_center = 0.0;	# (MaxY+MinY)/2
$z_range = 0.0;		# MaxZ - MinZ
$z_center = 0.0;	# (MaxZ+MinZ)/2
$data_max = 0.0;	# MAX OF x_max, y_max, z_max
$data_min = 0.0;	# MIN OF x_max, y_max, z_max
$data_center = 0.0;
$data_range = 0.0;

$normalize = 0;		# Should we normalize the data ? (default=NO)
$x_scale = 1.0;		# Scale factor for X-Axis
$y_scale = 1.0;		# Scale factor for Y-Axis
$z_scale = 1.0;		# Scale factor for Z-Axis


#############################################################################
# PARSE COMMAND LINE

while ( @ARGV )
{
	$option = shift( @ARGV );

	if ( $option eq "-infile" )
	{
		$input_name = shift( @ARGV );
	}
	elsif ( $option eq "-outfile" )
	{
		$output_name = shift( @ARGV );
	}
	elsif ( $option eq "-verbose" )
	{
		$verbose_name = "STDERR";
	}
	elsif ( $option eq "-normalize" )
	{
		$normalize = 1;
	}
	elsif ( $option eq "-xscale" )
	{
		$x_scale = shift( @ARGV );
	}
	elsif ( $option eq "-yscale" )
	{
		$y_scale = shift( @ARGV );
	}
	elsif ( $option eq "-zscale" )
	{
		$z_scale = shift( @ARGV );
	}
	elsif ( ! ( $option =~ /^-/ ) )
	{
		# Allow input and output file names to be specified
		# without the lame option flags.  Just parse filenames out.
		if ( $input_name eq "STDIN" )
		{
			if ( ! -f $option )
			{
				print STDERR "Infile does not exist: $option\n";
				exit( 1 );
			}
			$input_name = $option;
		}
		elsif ( $output_name eq "STDOUT" )
		{
			$output_name = $option;
		}
		else
		{
			print STDERR "Too many files specified: $option\n";
			exit( 1 );
		}
	}
	else
	{
		print STDERR "Unknown option $option\n";
		print STDERR "Usage: $0 [arguments]\n";
		print STDERR "   [-infile inputfile]\n";
		print STDERR "       default is STDIN\n";
		print STDERR "   [-outfile outputfile]\n";
		print STDERR "       default is STDOUT\n";
		print STDERR "   -verbose\n";
		print STDERR "       default is OFF\n";
		print STDERR "   -normalize\n";
		print STDERR "       default is OFF\n";
		print STDERR "   -xscale float\n";
		print STDERR "       default is 1.0\n";
		print STDERR "   -yscale float\n";
		print STDERR "       default is 1.0\n";
		print STDERR "   -zscale float\n";
		print STDERR "       default is 1.0\n";
		exit( 1 );
	}
}

if ( $input_name eq "STDIN" )
{
	open( INPUT, "<&STDIN" );
}
else
{
	open( INPUT, "<$input_name" );
}

if ( $output_name eq "STDOUT" )
{
	open( OUTPUT, ">&STDOUT" );
}
else
{
	open( OUTPUT, ">$output_name" );
}

if ( $verbose_name eq "STDERR" )
{
	open( VERBOSE, ">&STDERR" );
}
else
{
	open( VERBOSE, ">$verbose_name" );
}

print VERBOSE "Using $input_name for input and $output_name for output...\n";


#############################################################################
###################################  MAIN  ##################################
#############################################################################


#############################################################################
# READ THE FILE INTO OUR "OBJ" LIST

print VERBOSE "Reading $input_name...  ";

@OBJ = <INPUT>;
$nLines = $#OBJ + 1;

print VERBOSE "($nLines lines read)\n";

close( INPUT );


#############################################################################
# PARSE OUT: VERTICES, POLYGONS, SURFACE NORMALS, AND TEXTURE NORMALS

print VERBOSE "Extracting data...\n";

for ( $i=0; $i<$nLines; $i++ )
{
	chop( $line = $OBJ[$i] );

	next if ( $line =~ /^#/o );  # Skip comments
	next if ( $line =~ /^$/o );  # Skip blank lines

	if ( $line =~ /^v /o )
	{
		( $v, $x, $y, $z, @junk ) = split( /\s+/o, $line );
		$vertices{$nVerts} = join( " ", $x, $y, $z );

		if ( $x_max eq "" )
		{
			$x_max = $x;
			$x_min = $x;
			$y_max = $y;
			$y_min = $y;
			$z_max = $z;
			$z_min = $z;
		}
		else
		{
			$x_max = &max( $x_max, $x );
			$x_min = &min( $x_min, $x );
			$y_max = &max( $y_max, $y );
			$y_min = &min( $y_min, $y );
			$z_max = &max( $z_max, $z );
			$z_min = &min( $z_min, $z );
		}

		$nVerts++;
	}
	elsif ( $line =~ /^f /o )
	{
		@temp1 = split( /\s+/o, $line );
		shift( @temp1 );  # Toss the "f "
		@temp2 = ();
		for ( $j=0; $j<=$#temp1; $j++ )
		{
			( $index, @junk ) = split( /\//o, $temp1[$j] );
			$index -= 1;
			if ( $input_name =~ /eel/o ) { $index += 1; } # HACK
			@temp2 = ( @temp2, $index );
		}

		$connections{$nPolys} = join( " ", @temp2 );

		$nPolys++;
	}
	elsif ( $line =~ /^vt /o )
	{
	}
	elsif ( $line =~ /^vn /o )
	{
	}
	else
	{
	}

}

print VERBOSE "   Vertices: $nVerts\n";
print VERBOSE "   Polygons: $nPolys\n";

#############################################################################
# CALCULATE DATA STATISTICS

print VERBOSE "Calculating data statistics...\n";

$x_range = &abs( $x_max - $x_min );
$x_center = ($x_max + $x_min) / 2.0;

$y_range = &abs( $y_max - $y_min );
$y_center = ($y_max + $y_min) / 2.0;

$z_range = &abs( $z_max - $z_min );
$z_center = ($z_max + $z_min) / 2.0;

$data_max = &max( $x_max, $y_max, $z_max );
$data_min = &min( $x_min, $y_min, $z_min );
$data_center = ($data_max + $data_min) / 2.0;
$data_range = &max( $x_range, $y_range, $z_range );

$normalization = $data_range / 2.0;

print VERBOSE "   x_max = $x_max\n";
print VERBOSE "   x_min = $x_min\n";
print VERBOSE "   x_center = $x_center\n";
print VERBOSE "   x_range = $x_range\n";
print VERBOSE "\n";
print VERBOSE "   y_max = $y_max\n";
print VERBOSE "   y_min = $y_min\n";
print VERBOSE "   y_center = $y_center\n";
print VERBOSE "   y_range = $y_range\n";
print VERBOSE "\n";
print VERBOSE "   z_max = $z_max\n";
print VERBOSE "   z_min = $z_min\n";
print VERBOSE "   z_center = $z_center\n";
print VERBOSE "   z_range = $z_range\n";
print VERBOSE "\n";
print VERBOSE "   data_max = $data_max\n";
print VERBOSE "   data_min = $data_min\n";
print VERBOSE "   data_center = $data_center\n";
print VERBOSE "   data_range = $data_range\n";
print VERBOSE "Done calculating data statistics.\n";

#############################################################################
# WRITE VRML 2.0

print VERBOSE "Writting $output_name...\n";

print OUTPUT "#VRML V2.0 utf8\n\n";
#print OUTPUT "WorldInfo {\n";
#print OUTPUT "  title \"Untitled (yeah, right, ";
#print OUTPUT "yet another conceptual art masterpiece).\"\n";
#print OUTPUT "  info [ \"Created by obj2wrl @ ";
#print OUTPUT `date -u +'%Y%m%d%H%M%S UTC." ]'`;
#print OUTPUT "}\n\n";
#print OUTPUT "NavigationInfo { type \"EXAMINE\" }\n\n";
#print OUTPUT "Background { skyColor 0.3 0.0 0.1 }\n\n";
print OUTPUT "Transform { children [\n";
print OUTPUT "  Shape {\n";
print OUTPUT "    appearance Appearance {\n";
print OUTPUT "      material Material {\n";
#print OUTPUT "        diffuseColor  0.2 1.0 0.8\n";
#print OUTPUT "        specularColor 0.5 0.1 0.5\n";
#print OUTPUT "        shininess     0.88\n";
print OUTPUT "      } # Material\n";
print OUTPUT "    } # Appearance\n";
print OUTPUT "    geometry IndexedFaceSet {\n";

#############################################################################
# WRITE VERTEX DATA

print OUTPUT "      coord Coordinate { point [\n";

for ( $i=0; $i<$nVerts; $i++ )
{
	# DEREFERENCE A VERTEX

	@vert = split( /\s+/, $vertices{$i} );

	if ( $normalize == 1 )
	{
		# CENTER THE DATA
		$vert[0] -= $x_center;
		$vert[1] -= $y_center;
		$vert[2] -= $z_center;

		# NORMALIZE THE DATA
		$vert[0] /= $normalization;
		$vert[1] /= $normalization;
		$vert[2] /= $normalization;
	}

	# SCALE THE DATA
	if ( $x_scale != 1.0 ) { $vert[0] *= $x_scale; }
	if ( $y_scale != 1.0 ) { $vert[1] *= $y_scale; }
	if ( $z_scale != 1.0 ) { $vert[2] *= $z_scale; }

	# OUTPUT THE DATA

	printf( OUTPUT "\t%f\t%f\t%f\n", $vert[0], $vert[1], $vert[2] );

}
print OUTPUT "      ] } # point, Coordinate\n";

#############################################################################
# WRITE CONNECTIVITY DATA

print OUTPUT "      coordIndex [\n";

for ( $i=0; $i<$nPolys; $i++ )
{
	# EXTRACT THE NEXT SET OF VERTICES FOR A NEW QUAD

	@cons = split( /\s+/, $connections{$i} );

	for ( $j=0; $j<=$#cons; $j++ )
	{
		print OUTPUT "\t$cons[$j]";
	}
	print OUTPUT "\t-1\n";
}

print OUTPUT "      ] # coordIndex\n";

#############################################################################
# DONE

print OUTPUT "    } # IndexedFaceSet\n";
print OUTPUT "  } # Shape\n";
print OUTPUT "] } # children, Transform\n\n";

close( OUTPUT );

print VERBOSE "Done writing $output_name...\n";

print VERBOSE "Done\n";

