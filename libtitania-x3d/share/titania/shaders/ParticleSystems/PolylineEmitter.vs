/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform vec3          direction;
uniform samplerBuffer polylineMap;
uniform samplerBuffer lengthMap;

#pragma X3D include "X3DParticleEmitterNode.h"

/* PolylineEmitter */

vec3
getRandomPosition ()
{
	if (pointEmitter == 1)
		return vec3 (0.0f);

	int numLength = textureSize (lengthMap);

	float fraction = random1 (0, texelFetch (lengthMap, numLength - 1) .x);

	int   index0 = 0;
	int   index1 = 0;
	float weight = 0.0f;

	interpolate (lengthMap, fraction, index0, index1, weight);

	index0 *= 2;
	index1  = index0 + 1;
	
	vec3 vertex1 = texelFetch (polylineMap, index0) .xyz;
	vec3 vertex2 = texelFetch (polylineMap, index1) .xyz;

	return vertex1 + (vertex2 - vertex1) * weight;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}
