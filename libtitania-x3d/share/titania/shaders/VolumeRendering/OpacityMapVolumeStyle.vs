#version 300 es

precision mediump float;
precision mediump int;

uniform mat4 x3d_ProjectionMatrix;
uniform mat4 x3d_ModelViewMatrix;
uniform mat3 x3d_NormalMatrix;
uniform mat4 x3d_TextureMatrix [1];

uniform bool x3d_ColorMaterial;

in vec4 x3d_Vertex;
in vec3 x3d_Normal;
in vec4 x3d_TexCoord0;
in vec4 x3d_Color;

out vec3 vertex;
out vec3 normal;
out vec4 texCoord;

void
main ()
{
	vertex   = vec3 (x3d_ModelViewMatrix * x3d_Vertex);
	normal   = normalize (x3d_NormalMatrix * x3d_Normal);
	texCoord = x3d_TextureMatrix [0] * x3d_TexCoord0;

	gl_Position = x3d_ProjectionMatrix * x3d_ModelViewMatrix * x3d_Vertex;
}
