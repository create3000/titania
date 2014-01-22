/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Particle map */

uniform int ttStride;
uniform int ttTypeOffset;
uniform int ttMinOffset;
uniform int ttMaxOffset;
uniform int ttLeftOffset;
uniform int ttRightOffset;

uniform samplerBuffer triangleTreeMap;

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
