/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Particle map */

uniform mat4 modelViewMatrix;
uniform int  stride;
uniform int  seedOffset;
uniform int  lifetimeOffset;
uniform int  positionOffset;
uniform int  velocityOffset;
uniform int  colorOffset;
uniform int  elapsedTimeOffset;
uniform int  distanceOffset;

uniform samplerBuffer particleMap;

int particleIndex = 0;

void
setParticleIndex (in int vertexId)
{
	particleIndex = vertexId * stride;
}

int
getFromSeed ()
{
	return floatBitsToInt (texelFetch (particleMap, particleIndex + seedOffset) .x);
}

float
getFromLifetime ()
{
	return texelFetch (particleMap, particleIndex + lifetimeOffset) .x;
}

vec3
getFromPosition ()
{
	int index = particleIndex + positionOffset;

	return vec3 (texelFetch (particleMap, index + 0) .x,
	             texelFetch (particleMap, index + 1) .x,
	             texelFetch (particleMap, index + 2) .x);
}

vec3
getFromVelocity ()
{
	int index = particleIndex + velocityOffset;

	return vec3 (texelFetch (particleMap, index + 0) .x,
	             texelFetch (particleMap, index + 1) .x,
	             texelFetch (particleMap, index + 2) .x);
}

vec4
getFromColor ()
{
	int index = particleIndex + colorOffset;

	return vec4 (texelFetch (particleMap, index + 0) .x,
	             texelFetch (particleMap, index + 1) .x,
	             texelFetch (particleMap, index + 2) .x,
	             texelFetch (particleMap, index + 3) .x);
}

float
getFromElapsedTime ()
{
	return texelFetch (particleMap, particleIndex + elapsedTimeOffset) .x;
}

float
getDistance (in int vertexId)
{
	setParticleIndex (vertexId);

	return texelFetch (particleMap, particleIndex + distanceOffset) .x;
}

float
getDistance (in vec3 position)
{
	vec4 p = modelViewMatrix * vec4 (position, 1);

	return p .z; // p .z / p .w
}
