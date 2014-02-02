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
ExplosionEmitter ()
{
	setParticleIndex (odd_even_merge_sort (gl_VertexID));

	srand (getFromSeed ());

	float elapsedTime = getFromElapsedTime () + deltaTime;

	if (reset == 1)
	{
		to .lifetime    = getRandomLifetime ();
		to .position    = createParticles == 1 ? getRandomPosition () : vec3 (INFINITY);
		to .velocity    = getRandomVelocity ();
		to .elapsedTime = 0.0f;
	}
	else if (elapsedTime > getFromLifetime ())
	{
		to .lifetime    = getFromLifetime ();
		to .position    = vec3 (INFINITY);
		to .velocity    = vec3 (0.0f);
		to .elapsedTime = elapsedTime;
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
}

void
main ()
{
	ExplosionEmitter ();
}