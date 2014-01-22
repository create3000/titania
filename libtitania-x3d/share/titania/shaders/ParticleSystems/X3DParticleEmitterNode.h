/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* X3DParticleEmitterNode.h */

#define FORCES_MAX  32

uniform float deltaTime;
uniform float particleLifetime;
uniform float lifetimeVariation;

uniform float speed;
uniform float variation;

uniform vec3  velocity [FORCES_MAX];
uniform float turbulence [FORCES_MAX];
uniform int   numForces;

uniform samplerBuffer colorKeyMap;
uniform samplerBuffer colorRampMap;
uniform int           numColors;

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

	srand (getFromSeed () + gl_VertexID);

	if (getFromElapsedTime () > getFromLifetime ())
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
		vec3  velocity    = getVelocity ();
		float elapsedTime = getFromElapsedTime () + deltaTime;

		to .lifetime    = getFromLifetime ();
		to .position    = getFromPosition () + velocity * deltaTime;
		to .velocity    = velocity;
		to .color       = getColor (elapsedTime, to .lifetime);
		to .elapsedTime = elapsedTime;
		to .distance    = getDistance (to .position);
		to .seed        = srand ();
	}
}
