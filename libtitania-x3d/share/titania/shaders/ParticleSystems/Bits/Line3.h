/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

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

/* Line intersect triangle */

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
