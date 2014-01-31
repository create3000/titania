/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Math */

const float NAN      = 0.0 / 0.0;
const float INFINITY = 1.0 / 0.0;

const float M_PI    = 3.14159265358979323846;
const float M_PI1_2 = M_PI / 2;
const float M_PI2   = 2.0f * M_PI;

/* Algorithms */

int
upper_bound (inout samplerBuffer array, in float value)
{
	int first = 0;
	int step  = 0;
	int count = textureSize (array);

	while (count > 0)
	{
		int index = first;

		step = count >> 1; 

		index += step;

		if (value < texelFetch (array, index) .x)
			count = step;
		else
		{
			first  = ++ index;
			count -= step + 1;
		}
	}

	return first;
}

void
interpolate (in samplerBuffer keyMap, in float fraction, inout int index0, inout int index1, inout float weight)
{
	int size = textureSize (keyMap);

	if (size == 1 || fraction <= texelFetch (keyMap, 0) .x)
	{
		index0 = 0;
		index1 = 0;
		weight = 0.0f;
		return;
	}

	if (fraction >= texelFetch (keyMap, size - 1) .x)
	{
		index0 = size - 2;
		index1 = size - 1;
		weight = 1.0f;
		return;
	}

	int index = upper_bound (keyMap, fraction);

	if (index < size)
	{
		index1 = index;
		index0 = index - 1;

		float key0 = texelFetch (keyMap, index0) .x;
		float key1 = texelFetch (keyMap, index1) .x;

		weight = clamp ((fraction - key0) / (key1 - key0), 0.0f, 1.0f);
		return;
	}

	index0 = 0;
	index1 = 0;
	weight = 0.0f;
	return;
}

void
interpolate (in samplerBuffer keyMap, in float fraction, inout int index0)
{
	int size = textureSize (keyMap);

	if (size == 1 || fraction <= texelFetch (keyMap, 0) .x)
	{
		index0 = 0;
		return;
	}

	if (fraction >= texelFetch (keyMap, size - 1) .x)
	{
		index0 = size - 2;
		return;
	}

	int index = upper_bound (keyMap, fraction);

	if (index < size)
	{
		index0 = index - 1;
		return;
	}

	index0 = 0;
	return;
}
