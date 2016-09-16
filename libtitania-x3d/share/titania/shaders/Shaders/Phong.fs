// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

uniform int x3d_GeometryType;
// 1

uniform vec4 x3d_ClipPlane [x3d_MaxClipPlanes];
// 24

uniform int   x3d_FogType;
uniform vec3  x3d_FogColor;
uniform float x3d_FogVisibilityRange;
// 5

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;      // true if a X3DMaterialNode is attached, otherwise false
uniform bool  x3d_ColorMaterial; // true if a X3DColorNode is attached, otherwise false
// 3

uniform int   x3d_LightType [x3d_MaxLights];
uniform bool  x3d_LightOn [x3d_MaxLights];
uniform vec3  x3d_LightColor [x3d_MaxLights];
uniform float x3d_LightIntensity [x3d_MaxLights];
uniform float x3d_LightAmbientIntensity [x3d_MaxLights];
uniform vec3  x3d_LightAttenuation [x3d_MaxLights];
uniform vec3  x3d_LightLocation [x3d_MaxLights];
uniform vec3  x3d_LightDirection [x3d_MaxLights];
uniform float x3d_LightRadius [x3d_MaxLights];
uniform float x3d_LightBeamWidth [x3d_MaxLights];
uniform float x3d_LightCutOffAngle [x3d_MaxLights];
// 19 * x3d_MaxLights

uniform vec3      x3d_ShadowColor [x3d_MaxLights];
uniform float     x3d_ShadowIntensity [x3d_MaxLights];
uniform float     x3d_ShadowDiffusion [x3d_MaxLights];
uniform mat4      x3d_ShadowMatrix [x3d_MaxLights];
uniform sampler2D x3d_ShadowMap [x3d_MaxLights];
// 22 * x3d_MaxLights = 176

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

uniform int         x3d_TextureType [x3d_MaxTextures]; // true if a X3DTexture2DNode is attached, otherwise false
uniform sampler2D   x3d_Texture2D [x3d_MaxTextures];
uniform samplerCube x3d_CubeMapTexture [x3d_MaxTextures];

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
	for (int i = 0; i < x3d_MaxClipPlanes; ++ i)
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
	if (x3d_TextureType [0] == x3d_TextureType2D)
	{
		if (x3d_GeometryType == x3d_Geometry3D || gl_FrontFacing)
			return texture2D (x3d_Texture2D [0], vec2 (t));
		
		// If dimension is x3d_Geometry2D the texCoords must be flipped.
		return texture2D (x3d_Texture2D [0], vec2 (1.0 - t .s, t .t));
	}

	if (x3d_TextureType [0] == x3d_TextureTypeCubeMapTexture)
	{
		if (x3d_GeometryType == x3d_Geometry3D || gl_FrontFacing)
			return textureCube (x3d_CubeMapTexture [0], vec3 (t));
		
		// If dimension is x3d_Geometry2D the texCoords must be flipped.
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
getShadowIntensity (in int lightType, in float shadowIntensity, in float shadowDiffusion, in mat4 shadowMatrix, in sampler2D shadowMap, in float angle)
{
	if (lightType == x3d_PointLight)
	{
		#define SHADOW_TEXTURE_EPS 0.01
		
		// The projection bias matrix should be a uniform but this would require x3d_MaxLights * 16 floats.
		mat4 projectionBias = mat4 (0.09622504486493766, 0.0, 0.0, 0.0, 0.0, 0.1443375672974065, 0.0, 0.0, -0.16666666666666666, -0.25, -1.0001250156269532, -1.0, 0.0, 0.0, -0.12501562695336918, 0.0); // fov: 120deg, 1000m

		// Normals of the point light cube.
		mat4 rotations [6];
		rotations [0] = mat4 ( 0.0, 0.0,  1.0, 0.0, 0.0, 1.0,  0.0, 0.0, -1.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0); // left
		rotations [1] = mat4 ( 0.0, 0.0, -1.0, 0.0, 0.0, 1.0,  0.0, 0.0,  1.0,  0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0); // right
		rotations [2] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 1.0,  0.0, 0.0,  0.0,  0.0,  1.0, 0.0, 0.0, 0.0, 0.0, 1.0); // back
		rotations [3] = mat4 (-1.0, 0.0,  0.0, 0.0, 0.0, 1.0,  0.0, 0.0,  0.0,  0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0); // front
		rotations [4] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 0.0,  1.0, 0.0,  0.0, -1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0); // bottom
		rotations [5] = mat4 ( 1.0, 0.0,  0.0, 0.0, 0.0, 0.0, -1.0, 0.0,  0.0,  1.0,  0.0, 0.0, 0.0, 0.0, 0.0, 1.0); // top

		// Offsets to the shadow map.
		vec2 offsets [6];
		offsets [0] = vec2 (0.0,       0.0);
		offsets [1] = vec2 (1.0 / 3.0, 0.0);
		offsets [2] = vec2 (2.0 / 3.0, 0.0);
		offsets [3] = vec2 (0.0,       0.5);
		offsets [4] = vec2 (1.0 / 3.0, 0.5);
		offsets [5] = vec2 (2.0 / 3.0, 0.5);

		int value   = 0;
		int samples = 0;

		for (int m = 0, s = 0; m < 6; ++ m)
		{
			if (samples >= x3d_ShadowSamples)
				break;

			for (int i = 0; i < x3d_ShadowSamples; ++ i)
			{
				vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
				vec4  shadowCoord = projectionBias * rotations [m] * shadowMatrix * vec4 (vertex, 1.0);
				float bias        = (0.001 + 0.004 * (1.0 - abs (angle))) / shadowCoord .w; // 0.005 / shadowCoord .w;

				shadowCoord .xyz /= shadowCoord .w;

				if (shadowCoord .x < SHADOW_TEXTURE_EPS || shadowCoord .x > 1.0 / 3.0 - SHADOW_TEXTURE_EPS)
					continue;

				if (shadowCoord .y < SHADOW_TEXTURE_EPS || shadowCoord .y > 1.0 / 2.0 - SHADOW_TEXTURE_EPS)
					continue;

				if (shadowCoord .z >= 1.0)
					continue;

				if (texture2D (shadowMap, shadowCoord .xy + offsets [m]) .z < shadowCoord .z - bias)
				{
					++ value;
				}

				// We definitely have a shadow sample.
				++ samples;
			}
		}

		return shadowIntensity * min (float (value), float (x3d_ShadowSamples)) / float (x3d_ShadowSamples);
	}

	int value = 0;

	for (int i = 0; i < x3d_ShadowSamples; ++ i)
	{
		vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
		vec4  shadowCoord = shadowMatrix * vec4 (vertex, 1.0);
		float bias        = (0.001 + 0.004 * (1.0 - abs (angle))) / shadowCoord .w; // 0.005 / shadowCoord .w;

		shadowCoord .xyz /= shadowCoord .w;

		if (shadowCoord .z >= 1.0)
			continue;

		if (texture2D (shadowMap, shadowCoord .xy) .z < shadowCoord .z - bias)
		{
			++ value;
		}
	}

	return shadowIntensity * float (value) / float (x3d_ShadowSamples);
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
			if (x3d_TextureType [0] != x3d_NoTexture)
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
			if (x3d_TextureType [0] != x3d_NoTexture)
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

		for (int i = 0; i < x3d_MaxLights; ++ i)
		{
			int lightType = x3d_LightType [i];

			if (lightType == x3d_NoLight)
				break;

			vec3  vL = x3d_LightLocation [i] - v;
			float dL = length (vL);
			bool  di = lightType == x3d_DirectionalLight;

			if (di || dL <= x3d_LightRadius [i])
			{
				vec3  d = x3d_LightDirection [i];
				vec3  c = x3d_LightAttenuation [i];
				vec3  L = di ? -d : normalize (vL);      // Normalized vector from point on geometry to light source i position.
				vec3  H = normalize (L + V);             // Specular term
				float a = dot (N, L);                    // Angle between normal and light ray.

				vec3  diffuseTerm    = diffuseFactor * clamp (a, 0.0, 1.0);
				float specularFactor = shininess > 0.0 ? pow (max (dot (N, H), 0.0), shininess * 128.0) : 1.0;
				vec3  specularTerm   = specularColor * specularFactor;

				float attenuationFactor           = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);
				float spotFactor                  = lightType == x3d_SpotLight ? getSpotFactor (x3d_LightCutOffAngle [i], x3d_LightBeamWidth [i], L, d) : 1.0;
				float attenuationSpotFactor       = attenuationFactor * spotFactor;
				vec3  ambientColor                = x3d_LightAmbientIntensity [i] * ambientTerm;
				vec3  ambientDiffuseSpecularColor = ambientColor + x3d_LightIntensity [i] * (diffuseTerm + specularTerm);

				if (x3d_ShadowIntensity [i] > 0.0 && a > 0.0)
				{
					float shadowIntensity = getShadowIntensity (lightType, x3d_ShadowIntensity [i], x3d_ShadowDiffusion [i], x3d_ShadowMatrix [i], x3d_ShadowMap [i], a);
	
					finalColor += attenuationSpotFactor * (mix (x3d_LightColor [i], x3d_ShadowColor [i], shadowIntensity) * ambientDiffuseSpecularColor);
				}
				else
					finalColor += attenuationSpotFactor * (x3d_LightColor [i] * ambientDiffuseSpecularColor);
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
			if (x3d_TextureType [0] != x3d_NoTexture)
			{
				vec4 T = getTextureColor ();

				finalColor = T * C;
			}
			else
				finalColor = C;
		}
		else
		{
			if (x3d_TextureType [0] != x3d_NoTexture)
				finalColor = getTextureColor ();
		}

		return finalColor;
	}
}

vec3
getFogColor (in vec3 color)
{
	if (x3d_FogType == x3d_NoFog)
		return color;

	float dV = length (v);

	if (dV >= x3d_FogVisibilityRange)
		return x3d_FogColor;

	if (x3d_FogType == x3d_LinearFog)
		return mix (x3d_FogColor, color, (x3d_FogVisibilityRange - dV) / x3d_FogVisibilityRange);

	if (x3d_FogType == x3d_ExponentialFog)
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
