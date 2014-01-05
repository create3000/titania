#version 330

layout (location = 0)
in struct From
{
	vec3 vertex;
	vec3 position;
	vec4 color;
}
from;

out To
{
	vec3 position;
	vec4 color;
}
to;

void
main ()
{
	to .position = from .vertex + from .position;
	to .color    = from .color;
}
