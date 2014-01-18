/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform int           solid;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;

#pragma X3D include "X3DParticleEmitterNode.h"

/* PolylineEmitter */

vec3 direction = vec3 (0.0f, 1.0f, 0.0f);

vec3
getRandomPosition ()
{
	if (pointEmitter == 1)
		return vec3 (0.0f);

	int numSurfaceAreas = textureSize (surfaceAreaMap);

	float fraction = random1 (0, texelFetch (surfaceAreaMap, numSurfaceAreas - 1) .x);

	int   index0 = 0;
	int   index1 = 0;
	int   index2 = 0;
	float weight = 0.0f;

	interpolate (surfaceAreaMap, fraction, index0, index1, weight);

	index0 *= 3;
	index1  = index0 + 1;
	index2  = index0 + 2;
	
	vec3 normal1 = texelFetch (normalMap, index0) .xyz;
	vec3 normal2 = texelFetch (normalMap, index1) .xyz;
	vec3 normal3 = texelFetch (normalMap, index2) .xyz;

	vec3 vertex1 = texelFetch (surfaceMap, index0) .xyz;
	vec3 vertex2 = texelFetch (surfaceMap, index1) .xyz;
	vec3 vertex3 = texelFetch (surfaceMap, index2) .xyz;

	// Random barycentric coordinates.

	float u = fract (random1 ());
	float v = fract (random1 ());

	if (u + v > 1.0f)
	{
		u = 1.0f - u;
		v = 1.0f - v;
	}

	float t = 1 - u - v;

	// Calculate direction and position

	direction = normalize (t * normal1 + u * normal2 + v * normal3);

	if (solid == 0 && random1 () > 0.0f)
		direction = -direction;
	
	return t * vertex1 + u * vertex2 + v * vertex3;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}
