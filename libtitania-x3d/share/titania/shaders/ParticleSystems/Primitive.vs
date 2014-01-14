/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int  numVertices;
uniform int  geometryType;
uniform mat3 rotation;
uniform vec4 texCoord [6];

uniform samplerBuffer texCoordKeyMap;
uniform samplerBuffer texCoordRampMap;
uniform int           numTexCoord;

/* Vertex attributes */

layout (location = 0)
in struct From
{
	vec3  vertex;
	float lifetime;
	vec3  position;
	vec3  velocity;
	vec4  color;
	float elapsedTime;
}
from;

/* Transform feedback varyings */

out To
{
	vec3 position;
	vec4 color;
	vec4 texCoord;
}
to;

#pragma X3D include "Bits/Math.h"

vec4
getTexCoordValue (in int index)
{
	return texelFetch (texCoordRampMap, index);
}

vec4
getTexCoord ()
{
	if (numTexCoord == 0)
		return texCoord [gl_VertexID];

	float fraction = from .elapsedTime / from .lifetime;
	int   index0   = 0;

	interpolate (texCoordKeyMap, fraction, index0);

	return getTexCoordValue (index0 * numVertices + gl_VertexID);
}

#define LINE      1
#define TRIANGLE  2
#define QUAD      3
#define SPRITE    4

void
main ()
{
	switch (geometryType)
	{
		case LINE:
		{
			to .position = from .vertex .z * normalize (from .velocity) + from .position;
			break;
		}
		case TRIANGLE:
		case QUAD:
		{
			to .position = from .vertex + from .position;
			break;
		}
		case SPRITE:
		{
			to .position = rotation * from .vertex  + from .position;
			break;
		}
	}

	to .color    = from .color;
	to .texCoord = getTexCoord ();
}
