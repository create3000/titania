#version 330

#define ARRAY_MAX 128

uniform float deltaTime;
uniform float particleLifetime;
uniform float lifetimeVariation;

uniform vec3  position;
uniform vec3  direction;
uniform float speed;
uniform float variation;

uniform vec3  velocity [32];
uniform float turbulence [32];
uniform int   numForces;

uniform float colorKey [ARRAY_MAX];
uniform vec4  colorRamp [ARRAY_MAX];
uniform int   numColors;

layout (location = 0)
in struct From
{
	int   seed;
	float lifetime;
	vec3  position;
	vec3  velocity;
	vec4  color;
	float elapsedTime;
}
from;

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

#pragma X3D include "Bits/Math.h"
#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Quaternion.h"
#pragma X3D include "Bits/Color.h"

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
	vec3 v = from .velocity;

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
		return vec4 (1.0f, 1.0f, 1.0f, 1.0f);
		
	float fraction = elapsedTime / from .lifetime;

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
	srand (from .seed + gl_VertexID);

	if (from .elapsedTime > from .lifetime)
	{
		to .lifetime    = random_variation (particleLifetime, lifetimeVariation);
		to .position    = position;
		to .velocity    = getRandomVelocity ();
		to .elapsedTime = 0.0f;
		to .color       = getColor (0);
		to .seed        = srand ();
	}
	else
	{
		vec3  velocity    = getVelocity ();
		float elapsedTime = from .elapsedTime + deltaTime;

		to .lifetime    = from .lifetime;
		to .position    = from .position + velocity * deltaTime;
		to .velocity    = velocity;
		to .color       = getColor (elapsedTime);
		to .elapsedTime = elapsedTime;
		to .seed        = srand ();
	}
}
