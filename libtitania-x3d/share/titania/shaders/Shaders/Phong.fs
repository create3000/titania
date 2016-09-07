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

uniform int   x3d_LightType [MAX_LIGHTS];
uniform bool  x3d_LightOn [MAX_LIGHTS];
uniform vec3  x3d_LightColor [MAX_LIGHTS];
uniform float x3d_LightIntensity [MAX_LIGHTS];
uniform float x3d_LightAmbientIntensity [MAX_LIGHTS];
uniform vec3  x3d_LightAttenuation [MAX_LIGHTS];
uniform vec3  x3d_LightLocation [MAX_LIGHTS];
uniform vec3  x3d_LightDirection [MAX_LIGHTS];
uniform float x3d_LightRadius [MAX_LIGHTS];
uniform float x3d_LightBeamWidth [MAX_LIGHTS];
uniform float x3d_LightCutOffAngle [MAX_LIGHTS];
// 19 * MAX_LIGHTS

uniform vec3      x3d_ShadowColor [MAX_LIGHTS];
uniform float     x3d_ShadowIntensity [MAX_LIGHTS];
uniform float     x3d_ShadowDiffusion [MAX_LIGHTS];
uniform mat4      x3d_ShadowMatrix [MAX_LIGHTS];
uniform sampler2D x3d_ShadowMap [MAX_LIGHTS];
// 22 * MAX_LIGHTS = 176

uniform bool x3d_SeparateBackColor;

uniform float x3d_AmbientIntensity;
uniform vec3  x3d_DiffuseColor;
uniform vec3  x3d_SpecularColor;
uniform vec3  x3d_EmissiveColor;
uniform float x3d_Shininess;
uniform float x3d_Transparency;

uniform float x3d_BackAmbientIntensity;
uniform vec3  x3d_BackDiffuseColor;
uniform vec3  x3d_BackSpecularColor;
uniform vec3  x3d_BackEmissiveColor;
uniform float x3d_BackShininess;
uniform float x3d_BackTransparency;

#define MAX_TEXTURES 1
#define NO_TEXTURE   0
#define TEXTURE_2D   2
#define TEXTURE_CUBE 4

uniform int         x3d_TextureType [MAX_TEXTURES]; // true if a X3DTexture2DNode is attached, otherwise false
uniform sampler2D   x3d_Texture [MAX_TEXTURES];
uniform samplerCube x3d_CubeMapTexture [MAX_TEXTURES];

varying vec4 C;  // color
varying vec4 t;  // texCoord
varying vec3 vN; // normalized normal vector at this point on geometry
varying vec3 v;  // point on geometry

#pragma X3D include "Bits/Random.h"
#pragma X3D include "Bits/Plane3.h"

Plane3 plane = plane3 (v, vN);

void
clip ()
{
	for (int i = 0; i < MAX_CLIP_PLANES; ++ i)
	{
		if (x3d_ClipPlane [i] == vec4 (0.0, 0.0, 0.0, 0.0))
			break;

		if (dot (v, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
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
getSpotFactor (in float cutOffAngle, in float beamWidth, in vec3 L, in vec3 d)
{
	float spotAngle = acos (clamp (dot (-L, d), -1.0, 1.0));
	
	if (spotAngle >= cutOffAngle)
		return 0.0;
	else if (spotAngle <= beamWidth)
		return 1.0;

	return (spotAngle - cutOffAngle) / (beamWidth - cutOffAngle);
}

float
getShadowIntensity (in int lightType, in vec3 location, in float shadowIntensity, in float shadowDiffusion, in mat4 shadowMatrix, in sampler2D shadowMap, in float angle)
{
	#define SHADOW_SAMPLES 8

	if (lightType == POINT_LIGHT)
	{
		// The projection bias matrix should be a uniform but this would require MAX_LIGHTS * 16 floats.
		mat4 projectionBias = mat4 (0.0909926, 0.0, 0.0, 0.0, 0.0, 0.0606617, 0.0, 0.0, -0.25, -0.166667, -1.0, -1.0, 0.0, 0.0, -0.125, 0.0);

		mat4 rotations [6];
		rotations [0] = mat4 ( 0.0, 0.0,  1.0, 0.0, 0.0, 1.0,  0.0, 0.0, -1.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		rotations [1] = mat4 ( 0.0, 0.0, -1.0, 0.0, 0.0, 1.0,  0.0, 0.0,  1.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		rotations [2] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  1.0, 0.0,  0.0, -1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		rotations [3] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 0.0, -1.0, 0.0,  0.0,  1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		rotations [4] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 1.0,  0.0, 0.0,  0.0,  0.0,  1.0, 0.0, 0.0, 0.0, 0.0, 1.0); 
		rotations [5] = mat4 (-1.0, 0.0,  0.0, 0.0, 0.0, 1.0,  0.0, 0.0,  0.0,  0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0); 

		vec2 offsets [6];
		offsets [0] = vec2 (0.0, 0.0);
		offsets [1] = vec2 (0.5, 0.0);
		offsets [2] = vec2 (0.0, 1.0 / 3.0);
		offsets [3] = vec2 (0.5, 1.0 / 3.0);
		offsets [4] = vec2 (0.0, 2.0 / 3.0);
		offsets [5] = vec2 (0.5, 2.0 / 3.0);

		int value = 0;
	
		for (int m = 0, s = 0; m < 6 && s < SHADOW_SAMPLES; ++ m)
		{
			for (int i = 0; i < SHADOW_SAMPLES; ++ i)
			{
				vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
				vec4  shadowCoord = projectionBias * rotations [m] * shadowMatrix * vec4 (vertex, 1.0);
				float bias        = 0.005 / shadowCoord .w; // 0.005 * tan (acos (angle))
	
				shadowCoord .xyz /= shadowCoord .w;

				if (shadowCoord .x < 0.0 || shadowCoord .x > 1.0 / 2.0)
					continue;
		
				if (shadowCoord .y < 0.0 || shadowCoord .y > 1.0 / 3.0)
					continue;

				if (shadowCoord .z >= 1.0)
					continue;

				if (texture2D (shadowMap, shadowCoord .xy + offsets [m]) .z < shadowCoord .z - bias)
				{
					++ value;
				}

				// We definitely have a shadow sample.
				++ s;
			}
		}

		return shadowIntensity * min (float (value), float (SHADOW_SAMPLES)) / float (SHADOW_SAMPLES);
	}

	int value = 0;

	for (int i = 0; i < SHADOW_SAMPLES; ++ i)
	{
		vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
		vec4  shadowCoord = shadowMatrix * vec4 (vertex, 1.0);
		float bias        = 0.005 / shadowCoord .w; // 0.005 * tan (acos (angle))

		shadowCoord .xyz /= shadowCoord .w;

		if (shadowCoord .z >= 1.0)
			continue;

		if (texture2D (shadowMap, shadowCoord .xy) .z < shadowCoord .z - bias)
		{
			++ value;
		}
	}

	return shadowIntensity * float (value) / float (SHADOW_SAMPLES);
}

vec4
getMaterialColor ()
{
	if (x3d_Lighting)
	{
		vec3  N  = normalize (gl_FrontFacing ? vN : -vN);
		vec3  V  = normalize (-v); // normalized vector from point on geometry to viewer's position
		float dV = length (v);

		// Calculate diffuseFactor & alpha

		bool frontColor = gl_FrontFacing || ! x3d_SeparateBackColor;

		float ambientIntensity = frontColor ? x3d_AmbientIntensity : x3d_BackAmbientIntensity;
		vec3  diffuseColor     = frontColor ? x3d_DiffuseColor     : x3d_BackDiffuseColor;
		vec3  specularColor    = frontColor ? x3d_SpecularColor    : x3d_BackSpecularColor;
		vec3  emissiveColor    = frontColor ? x3d_EmissiveColor    : x3d_BackEmissiveColor;
		float shininess        = frontColor ? x3d_Shininess        : x3d_BackShininess;
		float transparency     = frontColor ? x3d_Transparency     : x3d_BackTransparency;

		vec3  diffuseFactor = vec3 (1.0, 1.0, 1.0);
		float alpha         = 1.0 - transparency;

		if (x3d_ColorMaterial)
		{
			if (x3d_TextureType [0] != NO_TEXTURE)
			{
				vec4 T = getTextureColor ();

				diffuseFactor  = T .rgb * C .rgb;
				alpha         *= T .a;
			}
			else
				diffuseFactor = C .rgb;

			alpha *= C .a;
		}
		else
		{
			if (x3d_TextureType [0] != NO_TEXTURE)
			{
				vec4 T = getTextureColor ();

				diffuseFactor  = T .rgb * diffuseColor;
				alpha         *= T .a;
			}
			else
				diffuseFactor = diffuseColor;
		}

		vec3 ambientTerm = diffuseFactor * ambientIntensity;

		// Apply light sources

		vec3 finalColor = vec3 (0.0, 0.0, 0.0);

		for (int i = 0; i < MAX_LIGHTS; ++ i)
		{
			int lightType = x3d_LightType [i];

			if (lightType == NO_LIGHT)
				break;

			vec3  vL = x3d_LightLocation [i] - v;
			float dL = length (vL);
			bool  di = lightType == DIRECTIONAL_LIGHT;

			if (di || dL <= x3d_LightRadius [i])
			{
				vec3  d = x3d_LightDirection [i];
				vec3  c = x3d_LightAttenuation [i];
				vec3  L = di ? -d : normalize (vL);      // Normalized vector from point on geometry to light source i position.
				vec3  H = normalize (L + V);             // Specular term
				float a = clamp (dot (N, L), 0.0, 1.0);  // Angle between normal and light ray.

				vec3  diffuseTerm    = diffuseFactor * a;
				float specularFactor = bool (shininess) ? pow (max (dot (N, H), 0.0), shininess * 128.0) : 1.0;
				vec3  specularTerm   = specularColor * specularFactor;

				float attenuation          = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);
				float spot                 = lightType == SPOT_LIGHT ? getSpotFactor (x3d_LightCutOffAngle [i], x3d_LightBeamWidth [i], L, d) : 1.0;
				vec3  lightColor           = (attenuation * spot) * x3d_LightColor [i];
				vec3  ambientColor         = x3d_LightAmbientIntensity [i] * ambientTerm;
				vec3  diffuseSpecularColor = diffuseTerm + specularTerm;
				vec3  color                = lightColor * (ambientColor + x3d_LightIntensity [i] * diffuseSpecularColor);

				if (x3d_ShadowIntensity [i] >= 0.0)
				{
					float shadowIntensity = getShadowIntensity (lightType, x3d_LightLocation [i], x3d_ShadowIntensity [i], x3d_ShadowDiffusion [i], x3d_ShadowMatrix [i], x3d_ShadowMap [i], a);
					vec3  shadowColor     = lightColor * ambientColor + diffuseSpecularColor * x3d_ShadowColor [i];
	
					finalColor += mix (color, shadowColor, shadowIntensity);
				}
				else
					finalColor += color;
			}
		}

		finalColor += emissiveColor;

		return vec4 (finalColor, alpha);
	}
	else
	{
		vec4 finalColor = vec4 (1.0, 1.0, 1.0, 1.0);
	
		if (x3d_ColorMaterial)
		{
			if (x3d_TextureType [0] != NO_TEXTURE)
			{
				vec4 T = getTextureColor ();

				finalColor = T * C;
			}
			else
				finalColor = C;
		}
		else
		{
			if (x3d_TextureType [0] != NO_TEXTURE)
				finalColor = getTextureColor ();
		}

		return finalColor;
	}
}

vec3
getFogColor (in vec3 color)
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

	gl_FragColor = getMaterialColor ();

	gl_FragColor .rgb = getFogColor (gl_FragColor .rgb);
}
