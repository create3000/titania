/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#pragma X3D include "Quaternion.h"
#pragma X3D include "Math.h"

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

// Return a uniform distributed random integral number in the interval [RAND_MIN, RAND_MAX].
int
rand ()
{
	return seed = seed * 1103515245 + 12345;
}

// Return a uniform distributed random floating point number in the interval [-1, 1].
float
random1 ()
{
	return float (rand ()) / float (RAND_MAX);
}

// Return a uniform distributed random floating point number in the interval [min, max].
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
	return mult_quat_vec (rotation, random_normal (angle));
}

// Generate a random normal for VolumeEmitter.

vec3
random_normal (in vec3 direction)
{
	float theta = random1 () * M_PI;
	float cphi  = pow (fract (random1 ()), 1 / 3.0f); // I don't why this, but its almost uniform.
	float phi   = acos (cphi);
	float r     = sin (phi);

	vec3 normal = vec3 (sin (theta) * r,
	                    cos (theta) * r,
	                    cphi);

	vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), direction);
	return mult_quat_vec (rotation, normal);
}

// Returns uniform distributed random barycentric coordinates for interpolation on a triangle.
// Use point = rand .x * vertex1 + rand .y * vertex2 + rand .z * vertex3 to get the resulting point.
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

/* Select random triangle from surfaceAreaMap. */

ivec3
random_triangle (in samplerBuffer surfaceAreaMap)
{
	int numSurfaceAreas = textureSize (surfaceAreaMap);

	float fraction = random1 (0, texelFetch (surfaceAreaMap, numSurfaceAreas - 1) .x);

	int   index0 = 0;
	int   index1 = 0;
	float weight = 0.0f;

	interpolate (surfaceAreaMap, fraction, index0, index1, weight);

	index0 *= 3;

	return ivec3 (index0, index0 + 1, index0 + 2);
}

/* Select random point from surfaceAreaMap. */

void
random_point_on_surface (in samplerBuffer surfaceAreaMap, in samplerBuffer surfaceMap, in samplerBuffer normalMap, out vec3 position, out vec3 normal)
{
	ivec3 index = random_triangle (surfaceAreaMap);

	vec3 vertex1 = texelFetch (surfaceMap, index .x) .xyz;
	vec3 vertex2 = texelFetch (surfaceMap, index .y) .xyz;
	vec3 vertex3 = texelFetch (surfaceMap, index .z) .xyz;

	vec3 normal1 = texelFetch (normalMap, index .x) .xyz;
	vec3 normal2 = texelFetch (normalMap, index .y) .xyz;
	vec3 normal3 = texelFetch (normalMap, index .z) .xyz;

	// Random barycentric coordinates.

	vec3 coord = random_barycentric ();

	// Calculate direction and position

	normal   = normalize (coord .x * normal1 + coord .y * normal2 + coord .z * normal3);
	position = coord .x * vertex1 + coord .y * vertex2 + coord .z * vertex3;
}
