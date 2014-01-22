/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/
#version 330

uniform int           pointEmitter;
uniform vec3          direction;
uniform samplerBuffer normalMap;
uniform samplerBuffer surfaceMap;
uniform samplerBuffer surfaceAreaMap;

#pragma X3D include "X3DParticleEmitterNode.h"
#pragma X3D include "Bits/TriangleTree.h"

/* Line */

struct Line3 {
	vec3 point;
	vec3 direction;
};

Line3
line3 (in vec3 point1, in vec3 point2)
{
	return Line3 (point1, normalize (point2 - point1));
}

bool
intersect (in Line3 line, in vec3 a, in vec3 b, in vec3 c, inout float u, inout float v)
{
	// find vectors for two edges sharing vert0
	vec3 edge1 = b - a;
	vec3 edge2 = c - a;

	// begin calculating determinant - also used to calculate U parameter
	vec3 pvec = cross (line .direction, edge2);

	// if determinant is near zero, ray lies in plane of triangle
	float det = dot (edge1, pvec);

	// Non culling intersection

	if (det == 0.0f)
		return false;

	float inv_det = 1.0f / det;

	// calculate distance from vert0 to ray point
	vec3 tvec = line .point - a;

	// calculate U parameter and test bounds
	u = dot (tvec, pvec) * inv_det;

	if (u < 0.0f || u > 1.0f)
		return false;

	// prepare to test V parameter
	vec3 qvec = cross (tvec, edge1);

	// calculate V parameter and test bounds
	v = dot (line .direction, qvec) * inv_det;

	if (v < 0.0f || u + v > 1.0f)
		return false;

	return true;
}

int
intersect (in Line3 line, in samplerBuffer surfaceMap, out vec3 points [32])
{
	int index       = 0;
	int numVertices = textureSize (surfaceMap);

	float u = 0, v = 0;

	for (int i = 0; i < numVertices; i += 3)
	{
		vec3 a = texelFetch (surfaceMap, i) .xyz;
		vec3 b = texelFetch (surfaceMap, i + 1) .xyz;
		vec3 c = texelFetch (surfaceMap, i + 2) .xyz;
		
		if (intersect (line, a, b, c, u, v))
			points [index ++] = (1 - u - v) * a + u * b + v * c;
	}

	return index;
}

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

/* Box3 */

const vec3 box3_normals [5] = vec3 [ ] (
	vec3 ( 0.0f,  0.0f,  1.0f),
	vec3 ( 0.0f,  0.0f, -1.0f),
	vec3 ( 0.0f,  1.0f,  0.0f),
	vec3 ( 0.0f, -1.0f,  0.0f),
	vec3 ( 1.0f,  0.0f,  0.0f)
);

bool
intersect (in vec3 min, in vec3 max, in Line3 line)
{
	vec3 center = (min + max) / 2.0f;

	vec3 points [5] = vec3 [ ] (
		vec3 (center .x, center .y, max .z), // right
		vec3 (center .x, center .y, min .z), // left

		vec3 (center .x, max .y, center .z), // top
		vec3 (center .x, min .y, center .z), // bottom

		vec3 (max .x, center .y, center .z)  // front
	);

	vec3 intersection = vec3 (0.0f);

	for (int i = 0; i < 5; ++ i)
	{
		if (intersect (plane3 (points [i], box3_normals [i]), line, intersection))
		{
			switch (i)
			{
				case 0:
				case 1:

					if (intersection .x >= min .x && intersection .x <= max .x &&
					    intersection .y >= min .y && intersection .y <= max .y)
						return true;

					break;
				case 2:
				case 3:

					if (intersection .x >= min .x && intersection .x <= max .x &&
					    intersection .z >= min .z && intersection .z <= max .z)
						return true;

					break;
				case 4:

					if (intersection .y >= min .y && intersection .y <= max .y &&
					    intersection .z >= min .z && intersection .z <= max .z)
						return true;

					break;
			}
		}
	}

	return false;
}

/*  */

int debug = 0;

int
getTriangleTreeIntersections (in Line3 line, in samplerBuffer surfaceMap, out vec3 points [32])
{
	int current = getTriangleTreeRoot ();
	int index   = 0;
	int id      = -1;
	int stack [32];

	for (int i = 0; i < 32; ++ i)
		stack [i] = -1;

	while (id >= 0 || current >= 0)
	{
		if (current >= 0)
		{
			setTriangleTreeIndex (current);

			if (getTriangleTreeType () == TRIANGLE_TREE_NODE)
			{
				// Node
			
				++ debug;

				if (intersect (getTriangleTreeMin (), getTriangleTreeMax (), line))
				{
					stack [++ id] = current;

					current = getTriangleTreeLeft ();
				}
				else
					current = -1;
			}
			else
			{			
				++ debug;

				// Triangle
			
				current = -1;

				int   i = getTriangleTreeLeft ();
				float u = 0, v = 0;

				vec3 a = texelFetch (surfaceMap, i) .xyz;
				vec3 b = texelFetch (surfaceMap, i + 1) .xyz;
				vec3 c = texelFetch (surfaceMap, i + 2) .xyz;

				if (intersect (line, a, b, c, u, v))
					points [index ++] = (1 - u - v) * a + u * b + v * c;
			}
		}
		else
		{
			current = stack [id --];

			setTriangleTreeIndex (current);

			current = getTriangleTreeRight ();
		}
	}

	return index;
}

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

	//return vec3 (float (debug) / float (textureSize (surfaceAreaMap) - 1), 0.1f, 0.0f);

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
