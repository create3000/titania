#!/usr/bin/perl
    eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
	if $running_under_some_shell;
#!/usr/bin/perl

use strict;

use math::Trig;
use math::Quaternion;

my $SEGMENTS = 300;
my $RADIUS   = 3;

my @crossSection;
my @spine;

for (my $i=0; $i <= $SEGMENTS; ++$i)
{
	my $u1     = $i / $SEGMENTS;
	my $theta1 = 2 * pi * $u1;
	my $x1     = sin($theta1);
	my $y1     = -cos($theta1);
	
	push @crossSection, [$x1, $y1];
}

for (my $i=0; $i <= $SEGMENTS; ++$i)
{
	my $u1     = $i / $SEGMENTS;
	my $theta1 = 2 * pi * $u1;
	my $x1     = sin($theta1) * $RADIUS;
	my $y1     = -cos($theta1) * $RADIUS;
	
	push @spine, [$x1, 0, $y1];
}

print_extrusion("ExtrusionXZ.wrl", \@crossSection, \@spine);

sub print_extrusion
{
	my $filename     = shift;
	my $crossSection = shift;
	my $spine = shift;
	
	open FILE, ">$filename";

	print FILE <<END;
#VRML V2.0 utf8 CosmoWorlds V1.0

Group {
  children Shape {
    appearance Appearance {
      material Material { }
      texture ImageTexture {
        url "Pixi.png"
      }
    }
    geometry Extrusion {
      beginCap FALSE
      endCap FALSE
      ccw FALSE
      creaseAngle 3.14159
      crossSection [ 
END

	foreach (@$crossSection)
	{
		printf FILE "%6f %6f, ", @$_;
	}

	print FILE <<END;
      ]
      spine [
END

	foreach (@$spine)
	{
		printf FILE "%6f %6f %6f, ", @$_;
	}

	print FILE <<END;
      ]
    }
  }
}
END
	close FILE;
}
