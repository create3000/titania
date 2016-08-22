// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

uniform float x3d_LinewidthScaleFactor;
// 2

#define MAX_CLIP_PLANES 6

uniform vec4 x3d_ClipPlane [MAX_CLIP_PLANES];
// 24

#define NO_FOG           0
#define LINEAR_FOG       1
#define EXPONENTIAL_FOG  2
#define EXPONENTIAL2_FOG 3

uniform int   x3d_FogType;
uniform vec3  x3d_FogColor;
uniform float x3d_FogVisibilityRange;
// 5

varying vec4 C; // color
varying vec3 v; // point on geometry
// 5

void
clip ()
{
	for (int i = 0; i < MAX_CLIP_PLANES; ++ i)
	{
		if (x3d_ClipPlane [i] == vec4 (0.0, 0.0, 0.0, 0.0))
			break;

		if (dot (v, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
			discard;
	}
}

float
getFogInterpolant ()
{
	if (x3d_FogType == NO_FOG)
		return 1.0;

	float dV = length (v);

	if (dV >= x3d_FogVisibilityRange)
		return 0.0;

	if (x3d_FogType == LINEAR_FOG)
		return (x3d_FogVisibilityRange - dV) / x3d_FogVisibilityRange;

	if (x3d_FogType == EXPONENTIAL_FOG)
		return exp (-dV / (x3d_FogVisibilityRange - dV));

	return 1.0;
}

void
main ()
{
	clip ();

	float f0 = getFogInterpolant ();

	gl_FragColor .rgb = mix (x3d_FogColor, C .rgb, f0);
	gl_FragColor .a   = C .a;
}
