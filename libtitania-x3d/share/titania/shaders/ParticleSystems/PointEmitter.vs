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

uniform vec3  velocity [32];
uniform float turbulence [32];
uniform int   forces;

in struct From
{
	int    seed;
	float  lifetime;
	vec3   position;
	vec3   velocity;
	double startTime;
}
from;

out To
{
	int    seed;
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
	return min + (random1 () * 0.5 + 0.5) * (max - min);
}

vec3
random3 ()
{
	return vec3 (random1 (), random1 (), random1 ());
}

/* Rotation */

vec4
quaternion (in vec3 fromVector, in vec3 toVector)
{
	vec3 from = normalize (fromVector);
	vec3 to   = normalize (toVector);

	float cos_angle = dot (from, to);
	vec3  crossvec  = normalize (cross (from, to));
	float crosslen  = length (crossvec);

	if (crosslen == 0.0)
	{
		if (cos_angle > 0.0)
			return vec4 (0.0, 0.0, 0.0, 1.0);

		else
		{
			vec3 t = cross (from, vec3 (1.0, 0.0, 0.0));

			if (dot (t, t) == 0.0)
				t = cross (from, vec3 (0.0, 1.0, 0.0));

			t = normalize (t);

			return vec4 (t, 0.0);
		}
	}
	else
	{
		crossvec *= sqrt (abs (1.0 - cos_angle) * 0.5);
		return vec4 (crossvec, sqrt (abs (1.0 + cos_angle) / 2.0));
	}
}

vec4
inverse (in vec4 quat)
{
	quat .xyz = -quat .xyz;
	return quat;
}

vec4
multRight (in vec4 lhs, in vec4 rhs)
{
	return vec4 (lhs .w * rhs .x +
	             lhs .x * rhs .w +
	             lhs .y * rhs .z -
	             lhs .z * rhs .y,

	             lhs .w * rhs .y +
	             lhs .y * rhs .w +
	             lhs .z * rhs .x -
	             lhs .x * rhs .z,

	             lhs .w * rhs .z +
	             lhs .z * rhs .w +
	             lhs .x * rhs .y -
	             lhs .y * rhs .x,

	             lhs .w * rhs .w -
	             lhs .x * rhs .x -
	             lhs .y * rhs .y -
	             lhs .z * rhs .z);
}

vec3
multVec (in vec4 quat, in vec3 vector)
{
	vec4 result = multRight (multRight (quat, vec4 (vector, 0.0)), inverse (quat));
	return result .xyz;
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
getRandomNormal (in float angle)
{
	float theta = random1 () * M_PI;
	float cphi  = random1 (cos (angle), 1);
	float phi   = acos (cphi);
	float r     = sin (phi);

	return vec3 (sin (theta) * r,
	             cos (theta) * r,
	             cphi);
}

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (getRandomValue (speed, variation));

	if (direction == vec3 (0, 0, 0))
		return randomSpeed * getRandomNormal (M_PI);

	return randomSpeed * direction;
}

void
main ()
{
	srand (from .seed);

	if (time - from .startTime > from .lifetime)
	{
		to .lifetime  = getRandomValue (particleLifetime, lifetimeVariation);
		to .position  = position;
		to .velocity  = getRandomVelocity ();
		to .startTime = time;
		to .seed      = seed;
	}
	else
	{
		vec3 v = from .velocity;

		for (int i = 0; i < forces; ++ i)
		{
			if (length (velocity [i]) < 0.000001)
				continue;

			vec4 rotation = quaternion (vec3 (0, 0, 1), velocity [i]);
			vec3 normal   = multVec (rotation, getRandomNormal (M_PI * turbulence [i]));
			v += normal * length (velocity [i]);
		}

		to .lifetime  = from .lifetime;
		to .position  = from .position + v * deltaTime;
		to .velocity  = v;
		to .startTime = from .startTime;
		to .seed      = seed;
	}
}
