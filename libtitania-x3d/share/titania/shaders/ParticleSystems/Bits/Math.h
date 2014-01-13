/* Math */

const float NAN      = 0.0 / 0.0;
const float INFINITY = 1.0 / 0.0;

const float M_PI    = 3.14159265358979323846;
const float M_PI1_2 = M_PI / 2;
const float M_PI2   = 2.0f * M_PI;

/* Algorithms */

float
degree (in float value)
{
	return value * (180 / M_PI);
}

int
upper_bound (in samplerBuffer array, in float value)
{
	int first = 0;
	int step  = 0;
	int count = textureSize (array);

	while (count > 0)
	{
		int index = first;

		step = count / 2; 

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

	if (size == 1 || fraction <= texelFetch (colorKeyMap, 0) .x)
	{
		index0 = 0;
		index1 = 0;
		weight = 0;
		return;
	}

	if (fraction >= texelFetch (colorKeyMap, size - 1) .x)
	{
		index0 = size - 2;
		index1 = size - 1;
		weight = 1;
		return;
	}

	int index = upper_bound (colorKeyMap, fraction);

	if (index < size)
	{
		index1 = index;
		index0 = index1 - 1;

		float key0 = texelFetch (colorKeyMap, index0) .x;
		float key1 = texelFetch (colorKeyMap, index1) .x;

		weight = clamp ((fraction - key0) / (key1 - key0), 0.0f, 1.0f);
		return;
	}

	index0 = 0;
	index1 = 0;
	weight = 0;
	return;
}
