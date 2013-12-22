#version 330

layout (location = 0) in float lifetime;
layout (location = 1) in vec3  position;
layout (location = 2) in vec3  velocity;

out float lifetime0;
out vec3  position0;
out vec3  velocity0;

void main()
{
	lifetime0 = lifetime;
	position0 = position + vec3 (0.01, 0.0, 0.0);
	velocity0 = velocity;
}
