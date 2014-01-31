/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#pragma X3D include "Plane3.h"

/* Box3 */

/* const */ vec3 BOX3_NORMALS [5] = vec3 [ ] (
	vec3 (0.0f,  0.0f,  1.0f), // front
	vec3 (0.0f,  0.0f, -1.0f), // back
	vec3 (0.0f,  1.0f,  0.0f), // top
	vec3 (0.0f, -1.0f,  0.0f), // bottom
	vec3 (1.0f,  0.0f,  0.0f)  // right
);

bool
intersect (in vec3 min, in vec3 max, in Line3 line)
{
	vec3 intersection = vec3 (0.0f);

	for (int i = 0; i < 5; ++ i)
	{
		if (intersect (plane3 ((i & 1) == 1 ? min : max, BOX3_NORMALS [i]), line, intersection))
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
