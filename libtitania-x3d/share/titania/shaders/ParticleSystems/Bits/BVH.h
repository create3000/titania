/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Bounded volume hierary */

uniform int bvhStride;
uniform int bvhTypeOffset;
uniform int bvhMinOffset;
uniform int bvhMaxOffset;
uniform int bvhLeftOffset;
uniform int bvhRightOffset;

#pragma X3D include "Box3.h"

#define BVH_NODE        0
#define BVH_TRIANGLE    1
#define BVH_STACK_SIZE  64

int bvhDebug     = 0;
int bvhNodeIndex = 0;

void
setBVHIndex (in int index)
{
	bvhNodeIndex = index * bvhStride;
}

int
getBVHRoot (in samplerBuffer bvh)
{
	return textureSize (bvh) / bvhStride - 1;
}

int
getBVHType (in samplerBuffer bvh)
{
	return floatBitsToInt (texelFetch (bvh, bvhNodeIndex + bvhTypeOffset) .x);
}

vec3
getBVHMin (in samplerBuffer bvh)
{
	int index = bvhNodeIndex + bvhMinOffset;

	return vec3 (texelFetch (bvh, index + 0) .x,
	             texelFetch (bvh, index + 1) .x,
	             texelFetch (bvh, index + 2) .x);
}

vec3
getBVHMax (in samplerBuffer bvh)
{
	int index = bvhNodeIndex + bvhMaxOffset;

	return vec3 (texelFetch (bvh, index + 0) .x,
	             texelFetch (bvh, index + 1) .x,
	             texelFetch (bvh, index + 2) .x);
}

int
getBVHLeft (in samplerBuffer bvh)
{
	return floatBitsToInt (texelFetch (bvh, bvhNodeIndex + bvhLeftOffset) .x);
}

int
getBVHRight (in samplerBuffer bvh)
{
	return floatBitsToInt (texelFetch (bvh, bvhNodeIndex + bvhRightOffset) .x);
}

/* Ray triangle intersection test */

int
getIntersections (in samplerBuffer bvh, in Line3 line, in samplerBuffer surfaceMap, out vec3 points [ARRAY_SIZE])
{
	int current = getBVHRoot (bvh);
	int count   = 0;
	int id      = -1;
	int stack [BVH_STACK_SIZE];

	for (int i = 0; i < BVH_STACK_SIZE; ++ i)
		stack [i] = -1;

	while (id >= 0 || current >= 0)
	{
		if (current >= 0)
		{
			setBVHIndex (current);

			if (getBVHType (bvh) == BVH_NODE)
			{
				// Node
			
				++ bvhDebug;

				if (intersect (getBVHMin (bvh), getBVHMax (bvh), line))
				{
					stack [++ id] = current;

					current = getBVHLeft (bvh);
				}
				else
					current = -1;
			}
			else
			{			
				++ bvhDebug;

				// Triangle
			
				current = -1;

				int   i = getBVHLeft (bvh);
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

			setBVHIndex (current);

			current = getBVHRight (bvh);
		}
	}

	return count;
}

int
getIntersections (in samplerBuffer bvh, in Line3 line, in samplerBuffer normalMap, in samplerBuffer surfaceMap, out vec3 normals [ARRAY_SIZE], out vec3 points [ARRAY_SIZE])
{
	int current = getBVHRoot (bvh);
	int count   = 0;
	int id      = -1;
	int stack [BVH_STACK_SIZE];

	for (int i = 0; i < BVH_STACK_SIZE; ++ i)
		stack [i] = -1;

	while (id >= 0 || current >= 0)
	{
		if (current >= 0)
		{
			setBVHIndex (current);

			if (getBVHType (bvh) == BVH_NODE)
			{
				// Node
			
				++ bvhDebug;

				if (intersect (getBVHMin (bvh), getBVHMax (bvh), line))
				{
					stack [++ id] = current;

					current = getBVHLeft (bvh);
				}
				else
					current = -1;
			}
			else
			{			
				++ bvhDebug;

				// Triangle
			
				current = -1;

				int   i = getBVHLeft (bvh);
				float u = 0, v = 0;

				vec3 a = texelFetch (surfaceMap, i) .xyz;
				vec3 b = texelFetch (surfaceMap, i + 1) .xyz;
				vec3 c = texelFetch (surfaceMap, i + 2) .xyz;

				if (intersect (line, a, b, c, u, v))
				{
					points [count] = (1 - u - v) * a + u * b + v * c;
				
					vec3 n1 = texelFetch (normalMap, i) .xyz;
					vec3 n2 = texelFetch (normalMap, i + 1) .xyz;
					vec3 n3 = texelFetch (normalMap, i + 2) .xyz;

					normals [count] = normalize ((1 - u - v) * n1 + u * n2 + v * n3);
					
					++ count;
				}
			}
		}
		else
		{
			current = stack [id --];

			setBVHIndex (current);

			current = getBVHRight (bvh);
		}
	}

	return count;
}
