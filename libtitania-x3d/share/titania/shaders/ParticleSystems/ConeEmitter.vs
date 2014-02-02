/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform vec3  position;
uniform vec3  direction;
uniform float angle;

#pragma X3D include "X3DParticleEmitterNode.h"

/* ConeEmitter */

vec3
getRandomPosition ()
{
	return position;
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * random_normal (direction, angle);
}

void
main ()
{
	ConstantEmitter ();
}
