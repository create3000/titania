/* Math */

const float M_PI    = 3.14159265358979323846;
const float M_PI1_2 = M_PI / 2;
const float M_PI2   = 2.0f * M_PI;

/* Algorithms */

#ifndef ARRAY_MAX
#define ARRAY_MAX 128
#endif

float
degree (in float value)
{
	return value * (180 / M_PI);
}

int
upper_bound (in float array [ARRAY_MAX], in int count, in float value)
{
	int first = 0;
	int step  = 0;

	while (count > 0)
	{
		int index = first;

		step = count / 2; 

		index += step;

		if (value < array [index])
			count = step;
		else
		{
			first = ++ index;
			count -= step + 1;
		}
	}

	return first;
}
