/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#pragma X3D include "Line3.h"

/* Plane */

struct Plane3
{
	vec3  normal;
	float distanceFromOrigin;
};

Plane3
plane3 (in vec3 point, in vec3 normal)
{
	return Plane3 (normal, dot (normal, point));
}

float
distance (in Plane3 plane, in vec3 point)
{
	return dot (point, plane .normal) - plane .distanceFromOrigin;
}

/* Plane intersect line */

bool
intersect (in Plane3 plane, in Line3 line, out vec3 point)
{
	point = vec3 (0.0f);

	// Check if the line is parallel to the plane.
	float theta = dot (line .direction, plane .normal);

	// Plane and line are parallel.
	if (theta == 0.0f)
		return false;

	// Plane and line are not parallel. The intersection point can be calculated now.
	float t = (plane .distanceFromOrigin - dot (plane .normal, line .point)) / theta;

	point = line .point + line .direction * t;

	return true;
}
