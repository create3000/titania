/* Random number generation */

const int RAND_MAX = 0x7fffffff;
int seed           = 1;

int
srand ()
{
	return seed;
}

void
srand (in int value)
{
	seed = value;
}

int
rand ()
{
	return seed = seed * 1103515245 + 12345;
}

float
random1 ()
{
	return float (rand ()) / float (RAND_MAX);
}

float
random1 (in float min, in float max)
{
	return min + fract (random1 ()) * (max - min);
}

/* 
 * http://www.jasondavies.com/maps/random-points/
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */

vec3
random_normal ()
{
	float theta = random1 () * M_PI;
	float cphi  = random1 ();
	float phi   = acos (cphi);
	float r     = sin (phi);

	return vec3 (sin (theta) * r,
	             cos (theta) * r,
	             cphi);
}

vec3
random_normal (in float angle)
{
	float theta = random1 () * M_PI;
	float cphi  = random1 (cos (angle), 1.0f);
	float phi   = acos (cphi);
	float r     = sin (phi);

	return vec3 (sin (theta) * r,
	             cos (theta) * r,
	             cphi);
}

float
random_variation (in float value, in float variation)
{
	return value + value * variation * random1 ();
}
