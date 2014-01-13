#version 330

#define FORCES_MAX  32

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

uniform samplerBuffer colorKeyMap;
uniform samplerBuffer colorRampMap;

/* Transform feedback varyings */

out To
{
	int   seed;
	float lifetime;
	vec3  position;
	vec3  velocity;
	vec4  color;
	float elapsedTime;
	float distance;
}
to;

#pragma X3D include "Bits/ParticleMap.h"
#pragma X3D include "Bits/Math.h"
#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Quaternion.h"
#pragma X3D include "Bits/Color.h"

/* Odd even merge sort */

uniform vec4 sortParam;
uniform int  sortSize;

/* Contents of the uniform data fields */
#define TwoStage       sortParam .x
#define Pass_mod_Stage sortParam .y
#define TwoStage_PmS_1 sortParam .z
#define Pass           sortParam .w

int
odd_even_merge_sort (in int self)
{
	if (self < sortSize)
	{
		// My position within the range to merge.
		float j = floor (mod (self, TwoStage));

		if ((j < Pass_mod_Stage) || (j > TwoStage_PmS_1))
		{
			// Must copy.
			return self;
		}

		// Must sort.

		float compare = 1.0f;
		int   partner = self;

		if (mod ((j + Pass_mod_Stage) / Pass, 2.0) < 1.0)
		{
			// We are on the left side -> compare with partner on the right.
			partner += int (Pass);
		}
		else
		{
			// We are on the right side -> compare with partner on the left.
			compare  = -1.0f;
			partner -= int (Pass);
		}

		if (partner < sortSize)
			// On the left its a < operation, on the right its a >= operation.
			return getDistance (self) * compare < getDistance (partner) * compare ? self : partner;
	}

	return self;
}

/* main */

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (random_variation (speed, variation));

	if (direction == vec3 (0.0f))
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
getColorValue (in int index)
{
	return texelFetch (colorRampMap, index);
}

vec4
getColor (in float elapsedTime)
{
	int numColors = textureSize (colorKeyMap);

	if (numColors == 0)
		return vec4 (1.0f);

	int   index0 = 0;
	int   index1 = 0;
	float weight = 0;

	interpolate (colorKeyMap, elapsedTime / getFromLifetime (), index0, index1, weight);

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
		to .color       = getColor (0);
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
		to .color       = getColor (elapsedTime);
		to .elapsedTime = elapsedTime;
		to .distance    = getDistance (to .position);
		to .seed        = srand ();
	}
}
