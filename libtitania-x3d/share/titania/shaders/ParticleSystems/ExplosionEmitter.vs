/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int  reset;
uniform vec3 position;

#pragma X3D include "X3DParticleEmitterNode.h"

/* PointEmitter */

vec3
getRandomPosition ()
{
	return position;
}

vec3
getRandomVelocity ()
{
	return getRandomSpeed () * random_normal ();
}

void
main ()
{
	ExplosionEmitter ();
}