// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

uniform float x3d_LinewidthScaleFactor;
// 1

uniform vec4 x3d_ClipPlane [x3d_MaxClipPlanes];
// 24

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
	for (int i = 0; i < x3d_MaxClipPlanes; ++ i)
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
	if (x3d_FogType == x3d_NoFog)
		return 1.0;

	float dV = length (v);

	if (dV >= x3d_FogVisibilityRange)
		return 0.0;

	if (x3d_FogType == x3d_LinearFog)
		return (x3d_FogVisibilityRange - dV) / x3d_FogVisibilityRange;

	if (x3d_FogType == x3d_ExponentialFog)
		return exp (-dV / (x3d_FogVisibilityRange - dV));

	return 1.0;
}

void
main ()
{
	clip ();

	float f0 = getFogInterpolant ();
	float lw = (x3d_LinewidthScaleFactor + 1.0) / 2.0;
	float t  = distance (vec2 (0.5, 0.5), gl_PointCoord) * 2.0 * lw - lw + 1.0;

	gl_FragColor .rgb = mix (x3d_FogColor, C .rgb, f0);
	gl_FragColor .a   = mix (C .a, 0.0, clamp (t, 0.0, 1.0));
}
