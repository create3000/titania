#version 330

uniform int  geometryType;
uniform mat3 rotation;
uniform vec4 texCoord [6];

/* Vertex attributes */

layout (location = 0)
in struct From
{
	vec3 vertex;
	vec3 position;
	vec3 velocity;
	vec4 color;
}
from;

/* Transform feedback varyings */

out To
{
	vec3 position;
	vec4 color;
	vec4 texCoord;
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
		{
			to .position = from .vertex + from .position;
			break;
		}
		case SPRITE:
		{
			to .position = rotation * from .vertex  + from .position;
			break;
		}
	}

	to .color    = from .color;
	to .texCoord = texCoord [gl_VertexID];
}
