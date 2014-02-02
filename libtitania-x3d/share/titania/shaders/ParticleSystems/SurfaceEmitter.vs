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

	vec3 position;

	random_point_on_surface (surfaceAreaMap, surfaceMap, normalMap, position, direction);

	if (solid == 0 && random1 () > 0.0f)
		direction = -direction;

	return position;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}

void
main ()
{
	ConstantEmitter ();
}
