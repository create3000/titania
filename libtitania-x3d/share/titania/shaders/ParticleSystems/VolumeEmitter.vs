/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform vec3          direction;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;

#pragma X3D include "X3DParticleEmitterNode.h"
#pragma X3D include "Bits/TriangleTree.h"

/*  */

/* CombSort */

void
sort (inout vec3 array [32], in int length, in Plane3 plane)
{
	const float shrinkFactor = 0.801711847137793;

	int  gap       = length;
	bool exchanged = false;

	do
	{
		exchanged = false;
		
		if (gap > 1)
			gap = int (gap * shrinkFactor);

		for (int i = 0; i + gap < length; ++ i)
		{
			int j = i + gap;
		
			if (distance (plane, array [j]) < distance (plane, array [i]))
			{
				vec3 tmp = array [i];

				array [i] = array [j];
				array [j] = tmp;

				exchanged = true;
			}
		}
	}
	while (exchanged || gap > 1);
}

/* PolylineEmitter */

void
random_point_on_surface (in samplerBuffer surfaceAreaMap, in samplerBuffer surfaceMap, in samplerBuffer normalMap, out vec3 position, out vec3 normal)
{
	ivec3 index = random_triangle (surfaceAreaMap);

	vec3 vertex1 = texelFetch (surfaceMap, index .x) .xyz;
	vec3 vertex2 = texelFetch (surfaceMap, index .y) .xyz;
	vec3 vertex3 = texelFetch (surfaceMap, index .z) .xyz;

	vec3 normal1 = texelFetch (normalMap, index .x) .xyz;
	vec3 normal2 = texelFetch (normalMap, index .y) .xyz;
	vec3 normal3 = texelFetch (normalMap, index .z) .xyz;

	// Random barycentric coordinates.

	vec3 coord = random_barycentric ();

	// Calculate direction and position

	normal   = normalize (coord .x * normal1 + coord .y * normal2 + coord .z * normal3);
	position = coord .x * vertex1 + coord .y * vertex2 + coord .z * vertex3;
}

vec3
getRandomPosition ()
{
	if (pointEmitter == 1)
		return vec3 (0.0f);

	vec3 point;
	vec3 normal;

	random_point_on_surface (surfaceAreaMap, surfaceMap, normalMap, point, normal);

	Line3 line = Line3 (point, random_normal (normal, M_PI1_2 * 0.66666));

	vec3 points [32];
	int  intersections = getTriangleTreeIntersections (line, surfaceMap, points);

	//return vec3 (float (debug) / float (textureSize (surfaceAreaMap) - 1), float (gl_VertexID) / 100000.0f, 0.0f);

	if (intersections == 0)
		return vec3 (INFINITY);

	sort (points, intersections, plane3 (line .point, line .direction));
	
	int index = int (round (random1 (0, intersections / 2 - 1))) * 2;

	return points [index] + (points [index + 1] - points [index]) * fract (random1 ());
}

vec3
getRandomVelocity ()
{
	float randomSpeed = getRandomSpeed ();

	if (direction == vec3 (0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}
