#version 330

#define COLOR_MAX 128

uniform float colorKey [COLOR_MAX];
uniform vec4  colorRamp [COLOR_MAX];
uniform int   colors;

layout (location = 0)
in struct From
{
	float lifetime;
	vec3  position;
	float elapsedTime;
}
from;

out To
{
	vec3 position;
	vec4 color;
}
to;

/* Math */

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
upper_bound (in float array [COLOR_MAX], in int count, in float value)
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

/* Color */

vec4
HSVA (in float h, in float s, in float v, in float a)
{
	// H is given on [0, 2 * Pi]. S and V are given on [0, 1].
	// RGB are each returned on [0, 1].

	v = clamp (v, 0.0f, 1.0f);

	if (s == 0.0f)
	{
		// achromatic (grey)
		return vec4 (v, v, v, a);
	}

	s = clamp (s, 0.0f, 1.0f);

	float w = degree (h) / 60.0f; // sector 0 to 5

	float i = floor (w);
	float f = w - i;              // factorial part of h
	float p = v * (1 - s);
	float q = v * (1 - s * f);
	float t = v * (1 - s * (1 - f));

	switch (int (i))
	{
		case 0:
			return vec4 (v, t, p, a);

		case 1:
			return vec4 (q, v, p, a);

		case 2:
			return vec4 (p, v, t, a);

		case 3:
			return vec4 (p, q, v, a);

		case 4:
			return vec4 (t, p, v, a);

		default:
			return vec4 (v, p, q, a);
	}
}

#define H 0
#define S 1
#define V 2
#define A 3

vec4
clerp (in vec4 source, in vec4 destination, in float t)
{
	float range = abs (destination [H] - source [H]);

	if (range <= M_PI)
	{
		return HSVA (mix (source [H], destination [H], t),
		             mix (source [S], destination [S], t),
		             mix (source [V], destination [V], t),
		             mix (source [A], destination [A], t));
	}
	else
	{
		float step = (M_PI2 - range) * t;
		float h    = source [H] < destination [H] ? source [H] - step : source [H] + step;

		if (h < 0.0f)
			h += M_PI2;

		else if (h > M_PI2)
			h -= M_PI2;

		return HSVA (h,
		             mix (source [S], destination [S], t),
		             mix (source [V], destination [V], t),
		             mix (source [A], destination [A], t));
	}
}

/* main */

void
interpolate (in int index0, in int index1, in float weight)
{
	to .color = clerp (colorRamp [index0], colorRamp [index1], weight);
}

void
set_fraction (in float fraction)
{
	if (colors == 1 || fraction <= colorKey [0])
	{
		interpolate (0, 0, 0);
		return;
	}

	if (fraction >= colorKey [colors - 1])
	{
		interpolate (colors - 2, colors - 1, 1);
		return;
	}

	int index = upper_bound (colorKey, colors, fraction);

	if (index < colors)
	{
		int index1 = index;
		int index0 = index1 - 1;

		float weight = (fraction - colorKey [index0]) / (colorKey [index1] - colorKey [index0]);

		interpolate (index0, index1, clamp (weight, 0.0f, 1.0f));
	}
}

void
main ()
{
	to .position = from .position;

	// Delete this and use not this shader
	if (colors == 0)
		to .color = vec4 (1.0f, 1.0f, 1.0f, 1.0f);

	else
		set_fraction (from .elapsedTime / from .lifetime);
}
