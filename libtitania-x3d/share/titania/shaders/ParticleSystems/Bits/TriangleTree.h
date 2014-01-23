/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Particle map */

uniform int ttStride;
uniform int ttTypeOffset;
uniform int ttMinOffset;
uniform int ttMaxOffset;
uniform int ttLeftOffset;
uniform int ttRightOffset;

uniform samplerBuffer triangleTreeMap;

#pragma X3D include "Box3.h"

#define TRIANGLE_TREE_NODE      0
#define TRIANGLE_TREE_TRIANGLE  1

int ttNodeIndex = 0;

int
getTriangleTreeRoot ()
{
	return textureSize (triangleTreeMap) / ttStride - 1;
}

void
setTriangleTreeIndex (in int index)
{
	ttNodeIndex = index * ttStride;
}

int
getTriangleTreeType ()
{
	return floatBitsToInt (texelFetch (triangleTreeMap, ttNodeIndex + ttTypeOffset) .x);
}

vec3
getTriangleTreeMin ()
{
	int index = ttNodeIndex + ttMinOffset;

	return vec3 (texelFetch (triangleTreeMap, index + 0) .x,
	             texelFetch (triangleTreeMap, index + 1) .x,
	             texelFetch (triangleTreeMap, index + 2) .x);
}

vec3
getTriangleTreeMax ()
{
	int index = ttNodeIndex + ttMaxOffset;

	return vec3 (texelFetch (triangleTreeMap, index + 0) .x,
	             texelFetch (triangleTreeMap, index + 1) .x,
	             texelFetch (triangleTreeMap, index + 2) .x);
}

int
getTriangleTreeLeft ()
{
	return floatBitsToInt (texelFetch (triangleTreeMap, ttNodeIndex + ttLeftOffset) .x);
}

int
getTriangleTreeRight ()
{
	return floatBitsToInt (texelFetch (triangleTreeMap, ttNodeIndex + ttRightOffset) .x);
}

/* Ray triangle intersection test */

#define INTERSECTIONS_SIZE  32
#define TT_STACK_SIZE       64

int debug = 0;

int
getTriangleTreeIntersections (in Line3 line, in samplerBuffer surfaceMap, out vec3 points [INTERSECTIONS_SIZE])
{
	int current = getTriangleTreeRoot ();
	int count   = 0;
	int id      = -1;
	int stack [TT_STACK_SIZE];

	for (int i = 0; i < TT_STACK_SIZE; ++ i)
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
					points [count ++] = (1 - u - v) * a + u * b + v * c;
			}
		}
		else
		{
			current = stack [id --];

			setTriangleTreeIndex (current);

			current = getTriangleTreeRight ();
		}
	}

	return count;
}
