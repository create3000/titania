#version 330

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
precision highp int;
#else
precision mediump float;
precision mediump int;
#endif

in vec4 color;  // color
in vec3 vertex; // point on geometry

out vec4 x3d_FragColor;

#pragma X3D include "include/ClipPlanes.glsl"

void
main ()
{
	clip ();

	x3d_FragColor = color;
}
