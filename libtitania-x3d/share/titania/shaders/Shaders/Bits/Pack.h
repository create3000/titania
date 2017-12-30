/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

#ifdef TITANIA
float
unpack (in vec4 color)
{
	return color .z;
}
#endif

#ifdef X_ITE
vec4
pack (in float value)
{
	const vec4 bitSh = vec4 (256.0 * 256.0 * 256.0,
	                         256.0 * 256.0,
	                         256.0,
	                         1.0);

	const vec4 bitMsk = vec4 (0.0,
	                          1.0 / 256.0,
	                          1.0 / 256.0,
	                          1.0 / 256.0);

	vec4 comp = fract (value * bitSh);

	comp -= comp.xxyz * bitMsk;

	return comp;
}

float
unpack (vec4 color)
{
	const vec4 bitShifts = vec4 (1.0 / (256.0 * 256.0 * 256.0),
	                             1.0 / (256.0 * 256.0),
	                             1.0 / 256.0,
	                             1.0);

	return dot (color, bitShifts);
}
#endif
