// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;
precision mediump int;

uniform int  x3d_NumClipPlanes;
uniform vec4 x3d_ClipPlane [x3d_MaxClipPlanes];

varying vec4 color;  // color
varying vec3 vertex; // point on geometry

void
clip ()
{
	#pragma unroll_loop
	for (int i = 0; i < x3d_MaxClipPlanes; ++ i)
	{
		if (i == x3d_NumClipPlanes)
			break;

		if (dot (vertex, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
			discard;
	}
}

void
main ()
{
	// THERE IS A BUG with x3d_NumClipPlanes AND INT UNIFORMS for the first frame in on_size_allocate.
	//clip ();

	gl_FragColor = color;
}
