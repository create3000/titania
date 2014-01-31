/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* X3DParticleEmitterNode.h */

#define FORCES_MAX  32

uniform float deltaTime;
uniform float particleLifetime;
uniform float lifetimeVariation;

uniform float speed;
uniform float variation;

uniform samplerBuffer colorKeyMap;
uniform samplerBuffer colorRampMap;
uniform int           numColors;

uniform vec3  velocity [FORCES_MAX];
uniform float turbulence [FORCES_MAX];
uniform int   numForces;

uniform int           boundedVolume;
uniform samplerBuffer boundedNormalMap;
uniform samplerBuffer boundedSurfaceMap;
uniform samplerBuffer boundedVolumeMap;

/* Transform feedback varyings */

out To
{
	int   seed;
	float lifetime;
	vec3  position;
	vec3  velocity;
	vec4  color;
	float elapsedTime;
	float distance;
}
to;

#pragma X3D include "Bits/ParticleMap.h"
#pragma X3D include "Bits/Math.h"
#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Color.h"
#pragma X3D include "Bits/OddEvenMergeSort.h"
#pragma X3D include "Bits/BVH.h"

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

/* main */

vec3
getRandomPosition ();

vec3
getRandomVelocity ();

float
getRandomLifetime ()
{
	float v = particleLifetime * lifetimeVariation;

	return random1 (max (0.0f, particleLifetime - v), particleLifetime + v);
}

float
getRandomSpeed ()
{
	float v = speed * variation;

	return random1 (max (0.0f, speed - v), speed + v);
}

vec3
getVelocity ()
{
	vec3 v = getFromVelocity ();

	for (int i = 0; i < numForces; ++ i)
	{
		float speed = length (velocity [i]);
	
		if (speed < 1e-7f)
			continue;

		v += random_normal (velocity [i], turbulence [i]) * speed;
	}

	return v;
}

/* CombSort: sort points along a line */

void
sort (inout vec3 normals [INTERSECTIONS_SIZE], inout vec3 points [INTERSECTIONS_SIZE], in int length, in Line3 line)
{
	Plane3 plane = plane3 (line .point, line .direction);

	const float shrinkFactor = 0.801711847137793;

	int  gap       = length;
	bool exchanged = false;

	do
	{
		exchanged = false;
		
		if (gap > 1)
			gap = int (gap * shrinkFactor);

		for (int i = 0; i + gap < length; ++ i)
		{
			int j = i + gap;
		
			if (distance (plane, points [j]) < distance (plane, points [i]))
			{
				vec3 tmp = points [i];
				points [i] = points [j];
				points [j] = tmp;

				tmp         = normals [i];
				normals [i] = normals [j];
				normals [j] = tmp;

				exchanged = true;
			}
		}
	}
	while (exchanged || gap > 1);
}

int
upper_bound (inout vec3 points [INTERSECTIONS_SIZE], in int count, in float value, in Line3 line)
{
	Plane3 plane = plane3 (line .point, line .direction);

	int first = 0;
	int step  = 0;

	while (count > 0)
	{
		int index = first;

		step = count >> 1; 

		index += step;

		if (value < distance (plane, points [index]))
			count = step;
		else
		{
			first  = ++ index;
			count -= step + 1;
		}
	}

	return first;
}

bool
bounce (in vec3 fromPosition, in vec3 toPosition, in vec3 velocity, in float deltaTime)
{
	if (boundedVolume == 0)
		return false;

	Line3 line = line3 (fromPosition, toPosition);

	vec3 normals [INTERSECTIONS_SIZE];
	vec3 points  [INTERSECTIONS_SIZE];
	int  intersections = getIntersections (boundedVolumeMap, line, boundedNormalMap, boundedSurfaceMap, normals, points);

	if (intersections > 0)
	{
		sort (normals, points, intersections, line);

		int index = upper_bound (points, intersections, 0, line);
		
		if (index < intersections)
		{
			Plane3 plane = plane3 (points [index], normals [index]);

			if (sign (distance (plane, fromPosition)) != sign (distance (plane, toPosition)))
			{
				to .velocity = reflect (velocity, normals [index]);	
				to .position = points [index] + to .velocity * deltaTime;
				return true;
			}
		}
	
		//to .position .x = -1.0f;
	}

	return false;
}

void
animate ()
{
	vec3 velocity     = getVelocity ();
	vec3 fromPosition = getFromPosition ();
	vec3 toPosition   = fromPosition + velocity * deltaTime;

	if (bounce (fromPosition, toPosition, velocity, deltaTime))
		return;

	to .position = toPosition;
	to .velocity = velocity;
}

vec4
getColorValue (in int index)
{
	return texelFetch (colorRampMap, index);
}

vec4
getColor (in float elapsedTime, in float lifetime)
{
	if (numColors == 0)
		return vec4 (1.0f);

	float fraction = elapsedTime / lifetime;

	int   index0 = 0;
	int   index1 = 0;
	float weight = 0.0f;

	interpolate (colorKeyMap, fraction, index0, index1, weight);

	return clerp (getColorValue (index0), getColorValue (index1), weight);
}

void
main ()
{
	setParticleIndex (odd_even_merge_sort (gl_VertexID));

	srand (getFromSeed ());

	float elapsedTime = getFromElapsedTime () + deltaTime;

	if (elapsedTime > getFromLifetime ())
	{
		to .lifetime    = getRandomLifetime ();
		to .position    = getRandomPosition ();
		to .velocity    = getRandomVelocity ();
		to .color       = getColor (0.0f, to .lifetime);
		to .elapsedTime = 0.0f;
		to .distance    = getDistance (to .position);
		to .seed        = srand ();
	}
	else
	{
		animate ();
		to .lifetime    = getFromLifetime ();
		to .color       = getColor (elapsedTime, to .lifetime);
		to .elapsedTime = elapsedTime;
		to .distance    = getDistance (to .position);
		to .seed        = srand ();
	}

	// Debug
	//to .position .y = getFromElapsedTime () / particleLifetime;
	//to .position .z = float (gl_VertexID) / float (textureSize (particleMap) / stride);
}
