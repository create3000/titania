/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* X3DParticleEmitterNode.h */

#define FORCES_MAX  32
#define ARRAY_SIZE  32

uniform float deltaTime;
uniform int   createParticles;
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
	vec3 toVelocity = getFromVelocity ();

	for (int i = 0; i < numForces; ++ i)
	{
		float speed = length (velocity [i]);
	
		if (speed < 1e-7f)
			continue;

		toVelocity += random_normal (velocity [i], turbulence [i]) * speed;
	}

	return toVelocity;
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
bounce (in vec3 fromPosition, in vec3 toPosition, in vec3 velocity)
{
	if (boundedVolume == 0)
		return;

	Line3 line = line3 (fromPosition, toPosition);

	vec3 normals [ARRAY_SIZE];
	vec3 points  [ARRAY_SIZE];
	int  intersections = getIntersections (boundedVolumeMap, line, boundedNormalMap, boundedSurfaceMap, normals, points);

	if (intersections > 0)
	{
		Plane3 plane = plane3 (line .point, line .direction);

		sort (normals, points, intersections, plane);

		int index = upper_bound (points, intersections, 0, plane);
		
		if (index < intersections)
		{
			Plane3 plane = plane3 (points [index], normals [index]);

			if (sign (distance (plane, fromPosition)) != sign (distance (plane, toPosition)))
			{
				to .velocity = reflect (velocity, normals [index]);	
				to .position = points [index] + normalize (to .velocity) * length (points [index] - fromPosition);
				return;
			}
		}
	}
}

void
animate ()
{
	vec3 fromPosition = getFromPosition ();

	to .velocity = getVelocity ();
	to .position = fromPosition + to .velocity * deltaTime;

	bounce (fromPosition, to .position, to .velocity);
}

void
ConstantEmitter ()
{
	setParticleIndex (odd_even_merge_sort (gl_VertexID));

	srand (getFromSeed ());

	float elapsedTime = getFromElapsedTime () + deltaTime;

	if (elapsedTime > getFromLifetime ())
	{
		to .lifetime    = getRandomLifetime ();
		to .position    = createParticles == 1 ? getRandomPosition () : vec3 (INFINITY);
		to .velocity    = getRandomVelocity ();
		to .elapsedTime = 0.0f;
	}
	else
	{
		animate ();

		to .lifetime    = getFromLifetime ();
		to .elapsedTime = elapsedTime;
	}

	to .color    = getColor (to .elapsedTime, to .lifetime);
	to .distance = getDistance (to .position);
	to .seed     = srand ();

	// Debug
	//to .position .y = getFromElapsedTime () / particleLifetime;
	//to .position .z = float (gl_VertexID) / float (textureSize (particleMap) / stride);
}
