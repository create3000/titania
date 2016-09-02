// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

#define GEOMETRY_2D 2
#define GEOMETRY_3D 3

uniform int x3d_GeometryType;
// 1

#define MAX_CLIP_PLANES 6

uniform vec4 x3d_ClipPlane [MAX_CLIP_PLANES];
// 24

#define NO_FOG           0
#define LINEAR_FOG       1
#define EXPONENTIAL_FOG  2
#define EXPONENTIAL2_FOG 3

uniform int   x3d_FogType;
uniform vec3  x3d_FogColor;
uniform float x3d_FogVisibilityRange;
// 5

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;      // true if a X3DMaterialNode is attached, otherwise false
uniform bool  x3d_ColorMaterial; // true if a X3DColorNode is attached, otherwise false
// 3

#define MAX_LIGHTS        8
#define NO_LIGHT          0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT       2
#define SPOT_LIGHT        3

uniform int x3d_LightType [MAX_LIGHTS]; // 0: No, 1: DirectionalLight, 2: PointLight, 3: SpotLight

uniform vec3      x3d_ShadowColor [MAX_LIGHTS];
uniform float     x3d_ShadowIntensity [MAX_LIGHTS];
uniform float     x3d_ShadowDiffusion [MAX_LIGHTS];
uniform mat4      x3d_ShadowMatrix [MAX_LIGHTS];
uniform sampler2D x3d_ShadowMap [MAX_LIGHTS];
// 176

#define MAX_TEXTURES 1
#define NO_TEXTURE   0
#define TEXTURE_2D   2
#define TEXTURE_CUBE 4

uniform int         x3d_TextureType [MAX_TEXTURES]; // NO_TEXTURE, TEXTURE_2D or TEXTURE_CUBE
uniform sampler2D   x3d_Texture [MAX_TEXTURES];
uniform samplerCube x3d_CubeMapTexture [MAX_TEXTURES];
// 3

varying vec4 frontColor; // color
varying vec4 backColor;  // color
varying vec4 t;          // texCoord
varying vec4 v;          // point on geometry
// 16, max 16

#pragma X3D include "Bits/Random.h"

void
clip ()
{
	for (int i = 0; i < MAX_CLIP_PLANES; ++ i)
	{
		if (x3d_ClipPlane [i] == vec4 (0.0, 0.0, 0.0, 0.0))
			break;

		if (dot (v .xyz, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
			discard;
	}
}

vec4
getTextureColor ()
{
	if (x3d_TextureType [0] == TEXTURE_2D)
	{
		if (x3d_GeometryType == GEOMETRY_3D || gl_FrontFacing)
			return texture2D (x3d_Texture [0], vec2 (t));
		
		// If dimension is GEOMETRY_2D the texCoords must be flipped.
		return texture2D (x3d_Texture [0], vec2 (1.0 - t .s, t .t));
	}

 	if (x3d_TextureType [0] == TEXTURE_CUBE)
	{
		if (x3d_GeometryType == GEOMETRY_3D || gl_FrontFacing)
			return textureCube (x3d_CubeMapTexture [0], vec3 (t));
		
		// If dimension is GEOMETRY_2D the texCoords must be flipped.
		return textureCube (x3d_CubeMapTexture [0], vec3 (1.0 - t .s, t .t, t .z));
	}
 
	return vec4 (1.0, 1.0, 1.0, 1.0);
}

float
getShadowIntensity (sampler2D shadowMap, vec4 shadowCoord, float shadowDiffusion)
{
	#define SAMPLES 16

	float bias  = 0.005;
	int   value = 0;

	for (int i = 0; i < SAMPLES; ++ i)
	{
		if (texture2D (shadowMap, (shadowCoord .xy / shadowCoord .w) + random2 () * shadowDiffusion). z < (shadowCoord.z - bias) / shadowCoord .w)
		{
			++ value;
		}
	}

	return float (value) / float (SAMPLES);
}

vec3
getShadowColor (vec3 color)
{
	float colorIntensity = 1.0;
	vec3  shadowColor    = vec3 (0.0, 0.0, 0.0);

	for (int i = 0; i < MAX_LIGHTS; ++ i)
	{
		int lightType = x3d_LightType [i];

		if (lightType == NO_LIGHT)
			break;

		if (lightType == POINT_LIGHT)
			continue;

		if (x3d_ShadowIntensity [i] <= 0.0)
			continue;

		//float bias          = max (0.05 * (1.0 - dot (normal, lightDir)), 0.005);

		vec4  shadowCoord     = x3d_ShadowMatrix [i] * v;
		float shadowIntensity = x3d_ShadowIntensity [i] * getShadowIntensity (x3d_ShadowMap [i], shadowCoord, x3d_ShadowDiffusion [i]);

		colorIntensity *= 1.0 - shadowIntensity;
		shadowColor    += shadowIntensity * x3d_ShadowColor [i];
	}

	return mix (shadowColor, color, colorIntensity);
}

vec3
getFogColor (vec3 color)
{
	if (x3d_FogType == NO_FOG)
		return color;

	float dV = length (v);

	if (dV >= x3d_FogVisibilityRange)
		return x3d_FogColor;

	if (x3d_FogType == LINEAR_FOG)
		return mix (x3d_FogColor, color, (x3d_FogVisibilityRange - dV) / x3d_FogVisibilityRange);

	if (x3d_FogType == EXPONENTIAL_FOG)
		return mix (x3d_FogColor, color, exp (-dV / (x3d_FogVisibilityRange - dV)));

	return color;
}

void
main ()
{
 	clip ();

	vec4 finalColor = gl_FrontFacing ? frontColor : backColor;

	if (x3d_TextureType [0] != NO_TEXTURE)
	{
		if (x3d_Lighting)
			finalColor *= getTextureColor ();
		else
		{
			if (x3d_ColorMaterial)
				finalColor *= getTextureColor ();
			else
				finalColor = getTextureColor ();
		}
	}

	gl_FragColor .rgb = getFogColor (getShadowColor (finalColor .rgb));
	gl_FragColor .a   = finalColor .a;
}
