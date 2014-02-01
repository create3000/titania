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

/* Find find the first point that is farther to the plane than value. */
int
upper_bound (inout vec3 points [ARRAY_SIZE], in int count, in float value, in Plane3 plane)
{
	int first = 0;
	int step  = 0;

	while (count > 0)
	{
		int index = first;

		step = count >> 1; 

		index += step;

		if (value < distance (plane, points [index]))
			count = step;
		else
		{
			first  = ++ index;
			count -= step + 1;
		}
	}

	return first;
}

/* CombSort: sort points in distance to a plane. */
void
sort (inout vec3 array [ARRAY_SIZE], in int length, in Plane3 plane)
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


/* CombSort: sort points and normals in distance to a plane. */
void
sort (inout vec3 normals [ARRAY_SIZE], inout vec3 points [ARRAY_SIZE], in int length, in Plane3 plane)
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
		
			if (distance (plane, points [j]) < distance (plane, points [i]))
			{
				vec3 tmp = points [i];
				points [i] = points [j];
				points [j] = tmp;

				tmp         = normals [i];
				normals [i] = normals [j];
				normals [j] = tmp;

				exchanged = true;
			}
		}
	}
	while (exchanged || gap > 1);
}
