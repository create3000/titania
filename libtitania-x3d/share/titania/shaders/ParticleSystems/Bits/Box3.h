/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#pragma X3D include "Plane3.h"

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
