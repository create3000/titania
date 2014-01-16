/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* X3DParticleEmitterNode.h */

#define FORCES_MAX  32

uniform float deltaTime;
uniform float particleLifetime;
uniform float lifetimeVariation;

uniform float speed;
uniform float variation;

uniform vec3  velocity [FORCES_MAX];
uniform float turbulence [FORCES_MAX];
uniform int   numForces;

uniform samplerBuffer colorKeyMap;
uniform samplerBuffer colorRampMap;
uniform int           numColors;

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
