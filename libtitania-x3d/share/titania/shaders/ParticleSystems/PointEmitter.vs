#version 330

#define FORCES_MAX  32
#define ARRAY_MAX   128

uniform float deltaTime;
uniform float particleLifetime;
uniform float lifetimeVariation;

uniform vec3  position;
uniform vec3  direction;
uniform float speed;
uniform float variation;

uniform vec3  velocity [FORCES_MAX];
uniform float turbulence [FORCES_MAX];
uniform int   numForces;

uniform float colorKey [ARRAY_MAX];
uniform vec4  colorRamp [ARRAY_MAX];
uniform int   numColors;

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

#pragma X3D include "Bits/ParticleMap.h"
#pragma X3D include "Bits/Math.h"
#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Quaternion.h"
#pragma X3D include "Bits/Color.h"

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
	// my position within the range to merge
	float j = floor (mod (self, TwoStage));

	if ((j < Pass_mod_Stage) || (j > TwoStage_PmS_1))
	{
		// must copy
		return self;
	}

	// must sort

	bool compare = true;
	int  partner = self;

	if (mod ((j + Pass_mod_Stage) / Pass, 2.0) < 1.0)
	{
		// we are on the left side -> compare with partner on the right
		partner += int (Pass);
	}
	else
	{
		// we are on the right side -> compare with partner on the left
		compare  = false;
		partner -= int (Pass);
	}

	// on the left its a < operation, on the right its a >= operation
	return (getDistance (self) < getDistance (partner)) == compare ? self : partner;
}

/* main */

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (random_variation (speed, variation));

	if (direction == vec3 (0.0f, 0.0f, 0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}

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
clerp (in int index0, in int index1, in float weight)
{
	return clerp (colorRamp [index0], colorRamp [index1], weight);
}

vec4
getColor (in float elapsedTime)
{
	if (numColors == 0)
		return vec4 (1.0f);

	float fraction = elapsedTime / getFromLifetime ();

	if (numColors == 1 || fraction <= colorKey [0])
		return clerp (0, 0, 0);

	if (fraction >= colorKey [numColors - 1])
		return clerp (numColors - 2, numColors - 1, 1);

	int index = upper_bound (colorKey, numColors, fraction);

	if (index < numColors)
	{
		int index1 = index;
		int index0 = index1 - 1;

		float weight = (fraction - colorKey [index0]) / (colorKey [index1] - colorKey [index0]);

		return clerp (index0, index1, clamp (weight, 0.0f, 1.0f));
	}

	return clerp (0, 0, 0);
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
		to .color       = getColor (0);
		to .elapsedTime = 0.0f;
		to .seed        = srand ();
	}
	else
	{
		vec3  velocity    = getVelocity ();
		float elapsedTime = getFromElapsedTime () + deltaTime;

		to .lifetime    = getFromLifetime ();
		to .position    = getFromPosition () + velocity * deltaTime;
		to .velocity    = velocity;
		to .color       = getColor (elapsedTime);
		to .elapsedTime = elapsedTime;
		to .seed        = srand ();
	}
}
