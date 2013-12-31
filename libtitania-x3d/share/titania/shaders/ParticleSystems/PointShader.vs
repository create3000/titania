#version 400
#extension GL_ARB_vertex_attrib_64bit : enable
#extension GL_ARB_separate_shader_objects : enable

uniform double time;
uniform float  colorKey [128];
uniform vec4   colorRamp [128];
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

/* main */

void
main ()
{
	to .position = from .position;
	
	if (colors == 0)
		to .color = vec4 (1.0f, 1.0f, 1.0f, 1.0f);

	else
	{
		to .color = colorRamp [0];
	}
}
