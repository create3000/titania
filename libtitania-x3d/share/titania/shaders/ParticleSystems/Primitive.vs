#version 330

uniform int geometryType;

layout (location = 0)
in struct From
{
	vec3 vertex;
	vec3 position;
	vec3 velocity;
	vec4 color;
}
from;

out To
{
	vec3 position;
	vec4 color;
}
to;

#define LINE      1
#define TRIANGLE  2
#define QUAD      3
#define SPRITE    4

void
main ()
{
	switch (geometryType)
	{
		case LINE:
		{
			to .position = from .vertex .z * normalize (from .velocity) + from .position;
			break;
		}
		case TRIANGLE:
		case QUAD:
		case SPRITE:
		{
			to .position = from .vertex + from .position;
			break;
		}
	}

	to .color = from .color;
}
