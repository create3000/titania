/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Color */

vec4
hsva (in float h, in float s, in float v, in float a)
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
		return hsva (mix (source [H], destination [H], t),
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

		return hsva (h,
		             mix (source [S], destination [S], t),
		             mix (source [V], destination [V], t),
		             mix (source [A], destination [A], t));
	}
}
