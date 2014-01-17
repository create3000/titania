/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform vec3 position;
uniform vec3 direction;

#pragma X3D include "Bits/X3DParticleEmitterNode.h"

/* PointEmitter */

vec3
getRandomPosition ()
{
	return position;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (random_variation (speed, variation));

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}
