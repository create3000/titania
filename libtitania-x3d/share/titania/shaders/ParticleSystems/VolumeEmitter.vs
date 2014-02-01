/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform vec3          direction;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;
uniform samplerBuffer volumeMap;

#pragma X3D include "X3DParticleEmitterNode.h"
#pragma X3D include "Bits/BVH.h"

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

	vec3 points [ARRAY_SIZE];
	int  intersections = getIntersections (volumeMap, line, surfaceMap, points);

	// Debug
	//return vec3 (float (bvhDebug) / float (textureSize (surfaceAreaMap) - 1), float (gl_VertexID) / 100000.0f, 0.0f);

	if (intersections == 0)
		return vec3 (INFINITY);

	sort (points, intersections, plane3 (line .point, line .direction));

	int index = int (round (random1 (0, intersections / 2 - 1))) * 2;

	//	// Debug
	//	if (getFromElapsedTime () == 0.0f)
	//	{
	//		int maxParticles = textureSize (particleMap) / stride;
	//		int fieldSize    = int (sqrt (maxParticles));
	//
	//		return vec3 ((gl_VertexID % fieldSize) / float (fieldSize - 1),
	//		             gl_VertexID / fieldSize / float (fieldSize - 1),
	//		             0.0f);
	//	}
	//	else
	//	{
	//		return getFromPosition ();
	//	}
	//	//

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
