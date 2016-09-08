// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

#define X3D_MAX_CLIP_PLANES 6

uniform vec4 x3d_ClipPlane [X3D_MAX_CLIP_PLANES];
// 24

varying vec3 v; // point on geometry
// 3

void
clip ()
{
	for (int i = 0; i < X3D_MAX_CLIP_PLANES; ++ i)
	{
		if (x3d_ClipPlane [i] == vec4 (0.0, 0.0, 0.0, 0.0))
			break;

		if (dot (v, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
			discard;
	}
}

void
main ()
{
	clip ();
}
