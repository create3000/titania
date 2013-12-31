#version 400
#extension GL_ARB_vertex_attrib_64bit : enable
#extension GL_ARB_separate_shader_objects : enable

uniform double time;

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
	to .color    = vec4 (1.0f, 0.0f, 0.0f, 0.05f);
}
