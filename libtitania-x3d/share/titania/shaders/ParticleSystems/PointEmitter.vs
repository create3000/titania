#version 400
#extension GL_ARB_vertex_attrib_64bit : enable

uniform double time;
uniform float  deltaTime;
uniform float  particleLifetime;
uniform float  lifetimeVariation;
uniform vec3   position;
uniform vec3   direction;
uniform float  speed;
uniform float  variation;
uniform vec3   velocity;

in struct From
{
	float  lifetime;
	vec3   position;
	vec3   velocity;
	double startTime;
}
from;

out To
{
	float  lifetime;
	vec3   position;
	vec3   velocity;
	double startTime;
}
to;

/* Math */

float M_PI    = 3.14159265358979323846;
float M_PI1_2 = M_PI / 2;
float M_PI2   = 2 * M_PI;

/* Random number generation */

#define RAND_MAX 0x7fffffff

int seed = 1;

void
srand (in int value)
{
	seed = value & RAND_MAX;
}

int
rand ()
{
	return seed *= 1103515245 + 12345;
}

float
random1 ()
{
	return float (rand ()) / float (RAND_MAX);
}

vec3
random3 ()
{
	return vec3 (random1 (), random1 (), random1 ());
}

/* main */

float
getRandomValue (in float value, in float variation)
{
	return value + value * variation * random1 ();
}

/* 
 * http://www.jasondavies.com/maps/random-points/
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */

vec3
getRandomNormal ()
{
	float theta = random1 () * M_PI;
	float phi   = acos (random1 ());
	float r     = sin (phi);

	return vec3 (-sin (theta) * r,
	              cos (phi),
	             -cos (theta) * r);
}

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (getRandomValue (speed, variation));

	if (direction == vec3 (0, 0, 0))
		return randomSpeed * getRandomNormal ();

	return randomSpeed * direction;
}

void
main ()
{
	if (time - from .startTime > from .lifetime)
	{
		srand (int (fract (from .position .x + from .position .y + from .position .z + time) * 1000000000));

		to .lifetime  = getRandomValue (particleLifetime, lifetimeVariation);
		to .position  = position;
		to .velocity  = getRandomVelocity ();
		to .startTime = time;
	}
	else
	{
		to .lifetime  = from .lifetime;
		to .position  = from .position + from .velocity * deltaTime;
		to .velocity  = from .velocity + velocity;
		to .startTime = from .startTime;
	}
}
