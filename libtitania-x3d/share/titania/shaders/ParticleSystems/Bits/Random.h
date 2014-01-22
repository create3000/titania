/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#pragma X3D include "Quaternion.h"

/* Random number generation */

const int RAND_MAX = int (0x7fffffff);
const int RAND_MIN = int (0x80000000);

int seed = 1;

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

// Return a uniform distributed random integer number between RAND_MIN and RAND_MAX.
int
rand ()
{
	return seed = seed * 1103515245 + 12345;
}

// Return a uniform distributed random float number between -1 and 1.
float
random1 ()
{
	return float (rand ()) / float (RAND_MAX);
}

// Return a uniform distributed random float number between min and max.
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

vec3
random_normal (in vec3 direction, in float angle)
{
	vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), direction);
	return multVec (rotation, random_normal (angle));
}

/* Generate a random normal for VolumeEmitter. */

vec3
random_normal (in vec3 direction)
{
	float theta = random1 () * M_PI;
	float cphi  = pow (random1 (), 1 / 3.0f); /* I don't why this, but its almost uniform. */
	float phi   = acos (cphi);
	float r     = sin (phi);

	vec3 normal = vec3 (sin (theta) * r,
	                    cos (theta) * r,
	                    cphi);

	vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), direction);
	return multVec (rotation, normal);
}

// Returns uniform distributed random barycentric coordinates for interpolation on a triangle.
// Use point = coord .x * vertex1 + coord .y * vertex2 + coord .z * vertex3 to get the resulting point.
vec3
random_barycentric ()
{
	// Random barycentric coordinates.
	float u = fract (random1 ());
	float v = fract (random1 ());

	if (u + v > 1.0f)
	{
		u = 1.0f - u;
		v = 1.0f - v;
	}

	float t = 1 - u - v;

	return vec3 (t, u, v);
}
