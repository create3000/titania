#version 400
#extension GL_ARB_vertex_attrib_64bit : enable
#extension GL_ARB_separate_shader_objects : enable

#define COLOR_MAX 128

uniform double time;
uniform float  colorKey [COLOR_MAX];
uniform vec4   colorRamp [COLOR_MAX];
uniform int    colors;

layout (location = 0)
in struct From
{
	float  lifetime;
	vec3   position;
	double startTime;
}
from;

out To
{
	vec3 position;
	vec4 color;
}
to;

/* Math */

float M_PI    = 3.14159265358979323846;
float M_PI1_2 = M_PI * 0.5;
float M_PI2   = 2.0f * M_PI;

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

float
lerp (in float source, in float destination, in float t)
{
	return source + t * (destination - source);
}

float
fmod (in float x, in float y)
{
	return x - trunc (x / y) * y;
}

float
interval (in float value, in float low, in float high)
{
	if (value >= high)
		return fmod (value - low, high - low) + low;

	if (value < low)
		return fmod (value - high, high - low) + high;

	return value;
}

/* Color */

vec4
HSVA (in float h, in float s, in float v, in float a)
{
	// H is given on [0, 2 * Pi]. S and V are given on [0, 1].
	// RGB are each returned on [0, 1].

	float _v = clamp (v, 0.0f, 1.0f);

	if (s == 0.0f)
	{
		// achromatic (grey)
		return vec4 (_v, _v, _v, a);
	}

	float _s = clamp (s, 0.0f, 1.0f);

	float w = degree (interval (h, 0, M_PI2)) / 60.0f; // sector 0 to 5

	float i = floor (w);
	float f = w - i;                                                         // factorial part of h
	float p = _v * (1 - _s);
	float q = _v * (1 - _s * f);
	float t = _v * (1 - _s * (1 - f));

	switch (int (i))
	{
		case 0:
			return vec4 (_v, t, p, a);

		case 1:
			return vec4 (q, _v, p, a);

		case 2:
			return vec4 (p, _v, t, a);

		case 3:
			return vec4 (p, q, _v, a);

		case 4:
			return vec4 (t, p, _v, a);

		default:
			return vec4 (_v, p, q, a);
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
		return HSVA (lerp (source [H], destination [H], t),
		             lerp (source [S], destination [S], t),
		             lerp (source [V], destination [V], t),
		             lerp (source [A], destination [A], t));
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
		             lerp (source [S], destination [S], t),
		             lerp (source [V], destination [V], t),
		             lerp (source [A], destination [A], t));
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
	
	if (colors == 0)
		to .color = vec4 (1.0f, 1.0f, 1.0f, 1.0f);

	else
		set_fraction (float (time - from .startTime) / from .lifetime);
}
