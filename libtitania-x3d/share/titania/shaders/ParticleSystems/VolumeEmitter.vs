/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform vec3          direction;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;

#pragma X3D include "X3DParticleEmitterNode.h"
#pragma X3D include "Bits/TriangleTree.h"

/* CombSort: sort points along a line */

void
sort (inout vec3 array [INTERSECTIONS_SIZE], in int length, in Line3 line)
{
	Plane3 plane = plane3 (line .point, line .direction);

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

vec3
getRandomPosition ()
{
	if (pointEmitter == 1)
		return vec3 (0.0f);

	vec3 point;
	vec3 normal;

	random_point_on_surface (surfaceAreaMap, surfaceMap, normalMap, point, normal);

	Line3 line = Line3 (point, random_normal (normal));

	vec3 points [INTERSECTIONS_SIZE];
	int  intersections = getTriangleTreeIntersections (line, surfaceMap, points);

	//return vec3 (float (debug) / float (textureSize (surfaceAreaMap) - 1), float (gl_VertexID) / 100000.0f, 0.0f);

	if (intersections == 0)
		return vec3 (INFINITY);

	sort (points, intersections, line);

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
