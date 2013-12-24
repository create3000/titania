#version 400
#extension GL_ARB_vertex_attrib_64bit : enable

uniform float  particleLifetime;
uniform float  lifetimeVariation;
uniform vec3   position;
uniform vec3   direction;
uniform float  speed;
uniform float  variation;
uniform float  random;
uniform float  frameRate;
uniform double time;

in struct From
{
	vec3   position;
	vec3   velocity;
	double startTime;
}
from;

out To
{
	vec3   position;
	vec3   velocity;
	double startTime;
}
to;

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
	return seed = seed * 1103515249 + 12347;
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

vec4
random4 ()
{
	return vec4 (random1 (), random1 (), random1 (), random1 ());
}

mat3
random9 ()
{
	return mat3 (random1 (), random1 (), random1 (),
	             random1 (), random1 (), random1 (),
	             random1 (), random1 (), random1 ());
}

/* Rotation */

#define M_PI  3.14159265358979323846

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

void
main ()
{
	if (time - from .startTime > particleLifetime)
	{
		srand (int (fract (from .position .x + from .position .y + from .position .z) * 1000000));

		//vec3  randomVector = variation * normalize (multVec (random4 (), random3 ()));

		vec3  randomVector = variation * normalize (random9 () * random3 ());
		float randomSpeed  = abs (speed + variation * random1 ());

		to .position  = position;
		to .velocity  = normalize (direction + randomVector) * randomSpeed;
		to .startTime = time;
	}
	else
	{
		to .position  = from .position + from .velocity / frameRate;
		to .velocity  = from .velocity;
		to .startTime = from .startTime;
	}
}
