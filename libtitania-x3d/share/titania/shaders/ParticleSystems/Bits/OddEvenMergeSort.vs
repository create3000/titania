/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

/* Transform feedback varyings */

out To
{
	int   seed;
	float lifetime;
	vec3  position;
	vec3  velocity;
	vec4  color;
	float elapsedTime;
}
to;

#pragma X3D include "ParticleMap.h"

/* Odd even merge sort */

uniform vec4 Param;
uniform int  size;

/* contents of the uniform data fields */
#define TwoStage       Param .x
#define Pass_mod_Stage Param .y
#define TwoStage_PmS_1 Param .z
#define Pass           Param .w

int
odd_even_merge_sort (in int self)
{
	// My position within the range to merge.
	float j = floor (mod (self, TwoStage));

	if ((j < Pass_mod_Stage) || (j > TwoStage_PmS_1))
	{
		// Must copy.
		return self;
	}

	// Must sort.

	bool compare = true;
	int  partner = self;

	if (mod ((j + Pass_mod_Stage) / Pass, 2.0) < 1.0)
	{
		// We are on the left side -> compare with partner on the right.
		partner += int (Pass);
	}
	else
	{
		// We are on the right side -> compare with partner on the left.
		compare  = false;
		partner -= int (Pass);
	}

	if (partner < size)
		// On the left its a < operation, on the right its a >= operation.
		return (getDistance (self) < getDistance (partner)) == compare ? self : partner;

	return self;
}

/* main */

void
main ()
{
	setParticleIndex (odd_even_merge_sort (gl_VertexID));

	to .lifetime    = getFromLifetime ();
	to .position    = getFromPosition ();
	to .velocity    = getFromVelocity ();
	to .color       = getFromColor ();
	to .elapsedTime = getFromElapsedTime ();
	to .seed        = getFromSeed ();
}
