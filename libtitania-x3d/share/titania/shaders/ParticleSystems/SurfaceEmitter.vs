/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform int           solid;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;

#pragma X3D include "X3DParticleEmitterNode.h"

/* PolylineEmitter */

vec3 direction = vec3 (0.0f);

vec3
getRandomPosition ()
{
	if (pointEmitter == 1)
		return vec3 (0.0f);

	ivec3 index = random_triangle (surfaceAreaMap);
	
	vec3 normal1 = texelFetch (normalMap, index .x) .xyz;
	vec3 normal2 = texelFetch (normalMap, index .y) .xyz;
	vec3 normal3 = texelFetch (normalMap, index .z) .xyz;

	vec3 vertex1 = texelFetch (surfaceMap, index .x) .xyz;
	vec3 vertex2 = texelFetch (surfaceMap, index .y) .xyz;
	vec3 vertex3 = texelFetch (surfaceMap, index .z) .xyz;

	// Random barycentric coordinates.

	vec3 coord = random_barycentric ();

	// Calculate direction and position

	direction = normalize (coord .x * normal1 + coord .y * normal2 + coord .z * normal3);

	if (solid == 0 && random1 () > 0.0f)
		direction = -direction;

	return coord .x * vertex1 + coord .y * vertex2 + coord .z * vertex3;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}
