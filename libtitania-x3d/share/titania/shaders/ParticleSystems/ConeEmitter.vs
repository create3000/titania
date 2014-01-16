/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform vec3  position;
uniform vec3  direction;
uniform float angle;

#pragma X3D include "Bits/X3DParticleEmitterNode.h"
#pragma X3D include "Bits/ParticleMap.h"
#pragma X3D include "Bits/Math.h"
#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Quaternion.h"
#pragma X3D include "Bits/Color.h"
#pragma X3D include "Bits/OddEvenMergeSort.h"

/* ConeEmitter */

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (random_variation (speed, variation));

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), direction);
	vec3 normal   = multVec (rotation, random_normal (angle));

	return randomSpeed * normal;
}

/* main */

vec3
getVelocity ()
{
	vec3 v = getFromVelocity ();

	for (int i = 0; i < numForces; ++ i)
	{
		float speed = length (velocity [i]);
	
		if (speed < 1e-6f)
			continue;

		vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), velocity [i]);
		vec3 normal   = multVec (rotation, random_normal (turbulence [i]));
		v += normal * speed;
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
		to .lifetime    = random_variation (particleLifetime, lifetimeVariation);
		to .position    = position;
		to .velocity    = getRandomVelocity ();
		to .color       = getColor (0.0f, to .lifetime);
		to .elapsedTime = 0.0f;
		to .distance    = getDistance (position);
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
