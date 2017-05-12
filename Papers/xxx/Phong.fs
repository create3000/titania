// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
//
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// 
//  Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
// 
//  All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
// 
//  The copyright notice above does not evidence any actual of intended
//  publication of such source code, and is an unpublished work by create3000.
//  This material contains CONFIDENTIAL INFORMATION that is the property of
//  create3000.
// 
//  No permission is granted to copy, distribute, or create derivative works from
//  the contents of this software, in whole or in part, without the prior written
//  permission of create3000.
// 
//  NON-MILITARY USE ONLY
// 
//  All create3000 software are effectively free software with a non-military use
//  restriction. It is free. Well commented source is provided. You may reuse the
//  source in any way you please with the exception anything that uses it must be
//  marked to indicate is contains 'non-military use only' components.
// 
//  DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// 
//  Copyright 2015, 2016 Holger Seelig <holger.seelig@yahoo.de>.
// 
//  This file is part of the Cobweb Project.
// 
//  Cobweb is free software: you can redistribute it and/or modify it under the
//  terms of the GNU General Public License version 3 only, as published by the
//  Free Software Foundation.
// 
//  Cobweb is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
//  A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
//  details (a copy is included in the LICENSE file that accompanied this code).
// 
//  You should have received a copy of the GNU General Public License version 3
//  along with Cobweb.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
//  copy of the GPLv3 License.
// 
//  For Silvio, Joy and Adi.


precision mediump float;

uniform int x3d_GeometryType;

uniform vec4 x3d_ClipPlane [x3d_MaxClipPlanes];

uniform int   x3d_FogType;
uniform vec3  x3d_FogColor;
uniform float x3d_FogVisibilityRange;

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;      // true if a X3DMaterialNode is attached, otherwise false
uniform bool  x3d_ColorMaterial; // true if a X3DColorNode is attached, otherwise false

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

#ifdef X3D_SHADOWS
uniform vec3      x3d_ShadowColor [x3d_MaxLights];
uniform float     x3d_ShadowIntensity [x3d_MaxLights];
uniform float     x3d_ShadowDiffusion [x3d_MaxLights];
uniform mat4      x3d_ShadowMatrix [x3d_MaxLights];
uniform sampler2D x3d_ShadowMap [x3d_MaxLights];
#endif

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

void
clip ()
{
	for (int i = 0; i < x3d_MaxClipPlanes; ++ i)
	{
		if (x3d_ClipPlane [i] == x3d_NoneClipPlane)
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

#ifdef X3D_SHADOWS
float
unpack (in vec4 color)
{
	return color .z;
}

float
getShadowDepth (in int index, in vec2 shadowCoord)
{
	#if x3d_MaxShadows > 0
	if (index == 0)
		return unpack (texture2D (x3d_ShadowMap [0], shadowCoord));
	#endif

	#if x3d_MaxShadows > 1
	if (index == 1)
		return unpack (texture2D (x3d_ShadowMap [1], shadowCoord));
	#endif

	#if x3d_MaxShadows > 2
	if (index == 2)
		return unpack (texture2D (x3d_ShadowMap [2], shadowCoord));
	#endif

	#if x3d_MaxShadows > 3
	if (index == 3)
		return unpack (texture2D (x3d_ShadowMap [3], shadowCoord));
	#endif

	#if x3d_MaxShadows > 4
	if (index == 4)
		return unpack (texture2D (x3d_ShadowMap [4], shadowCoord));
	#endif

	#if x3d_MaxShadows > 5
	if (index == 5)
		return unpack (texture2D (x3d_ShadowMap [5], shadowCoord));
	#endif

	#if x3d_MaxShadows > 6
	if (index == 6)
		return unpack (texture2D (x3d_ShadowMap [6], shadowCoord));
	#endif

	#if x3d_MaxShadows > 7
	if (index == 7)
		return unpack (texture2D (x3d_ShadowMap [7], shadowCoord));
	#endif

	return 0.0;
}

float
getShadowIntensity (in int index, in int lightType, in float shadowIntensity, in float shadowDiffusion, in mat4 shadowMatrix, in Plane3 plane, in float lightAngle)
{
	#define SHADOW_TEXTURE_EPS 0.01
	#define SHADOW_BIAS_OFFSET 0.002
	#define SHADOW_BIAS_FACTOR 0.004
		
	float shadowBias = SHADOW_BIAS_OFFSET + SHADOW_BIAS_FACTOR * (1.0 - abs (lightAngle));

	if (lightType == x3d_PointLight)
	{
		mat4 rotationProjectionBias [6];
		rotationProjectionBias [0] = mat4 (-0.1666666666666667, -0.25, -1.0001250156269537, -1.0, 0, 0.1443375672974065, 0.0, 0.0, -0.09622504486493763, 0.0, 0.0, 0.0, 0.0, 0.0, -0.12501562695336918, 0.0);
		rotationProjectionBias [1] = mat4 (0.16666666666666666, 0.25, 1.0001250156269537, 1.0, 0, 0.1443375672974065, 0.0, 0.0, 0.09622504486493771, 0.0, 0.0, 0.0, 0.0, 0.0, -0.12501562695336918, 0.0);
		rotationProjectionBias [2] = mat4 (0.09622504486493766, 0.0, 0.0, 0.0, 0.0, 0.1443375672974065, 0.0, 0.0, -0.16666666666666666, -0.25, -1.0001250156269532, -1.0, 0.0, 0.0, -0.12501562695336918, 0.0);
		rotationProjectionBias [3] = mat4 (-0.09622504486493766, 0.0, 0.0, 0.0, 0, 0.1443375672974065, 0.0, 0.0, 0.16666666666666666, 0.25, 1.0001250156269532, 1.0, 0.0, 0.0, -0.12501562695336918, 0.0);
		rotationProjectionBias [4] = mat4 (0.09622504486493766, 0.0, 0.0, 0.0, -0.16666666666666669, -0.25, -1.0001250156269537, -1.0, 0.0, -0.14433756729740646, 0.0, 0.0, 0.0, 0, -0.12501562695336918, 0.0);
		rotationProjectionBias [5] = mat4 (0.09622504486493766, 0.0, 0.0, 0.0, 0.16666666666666669, 0.25, 1.0001250156269537, 1.0, 0.0, 0.14433756729740657, 0.0, 0.0, 0.0, 0.0, -0.12501562695336918, 0.0);

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

		for (int m = 0; m < 6; ++ m)
		{
			for (int i = 0; i < x3d_ShadowSamples; ++ i)
			{
				if (samples >= x3d_ShadowSamples)
					return shadowIntensity * float (value) / float (x3d_ShadowSamples);

				vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
				vec4  shadowCoord = rotationProjectionBias [m] * shadowMatrix * vec4 (vertex, 1.0);
				float bias        = shadowBias / shadowCoord .w; // 0.005 / shadowCoord .w;

				shadowCoord .xyz /= shadowCoord .w;

				if (shadowCoord .x < SHADOW_TEXTURE_EPS || shadowCoord .x > 1.0 / 3.0 - SHADOW_TEXTURE_EPS)
					continue;

				if (shadowCoord .y < SHADOW_TEXTURE_EPS || shadowCoord .y > 1.0 / 2.0 - SHADOW_TEXTURE_EPS)
					continue;

				if (shadowCoord .z >= 1.0)
					continue;

				if (getShadowDepth (index, shadowCoord .xy + offsets [m]) < shadowCoord .z - bias)
				{
					++ value;
				}

				// We definitely have a shadow sample.
				++ samples;
			}
		}

		return shadowIntensity * float (value) / float (x3d_ShadowSamples);
	}

	int value = 0;

	for (int i = 0; i < x3d_ShadowSamples; ++ i)
	{
		vec3  vertex      = closest_point (plane, v + random3 () * shadowDiffusion);
		vec4  shadowCoord = shadowMatrix * vec4 (vertex, 1.0);
		float bias        = shadowBias / shadowCoord .w; // 0.005 / shadowCoord .w;

		shadowCoord .xyz /= shadowCoord .w;

		if (shadowCoord .z >= 1.0)
			continue;

		if (getShadowDepth (index, shadowCoord .xy) < shadowCoord .z - bias)
		{
			++ value;
		}
	}

	return shadowIntensity * float (value) / float (x3d_ShadowSamples);
}
#endif

vec4
getMaterialColor ()
{
	if (x3d_Lighting)
	{
		Plane3 plane = plane3 (v, vN);

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
			if (x3d_TextureType [0] != x3d_NoneTexture)
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
			if (x3d_TextureType [0] != x3d_NoneTexture)
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

			if (lightType == x3d_NoneLight)
				break;

			vec3  vL = x3d_LightLocation [i] - v;
			float dL = length (vL);
			bool  di = lightType == x3d_DirectionalLight;

			if (di || dL <= x3d_LightRadius [i])
			{
				vec3 d = x3d_LightDirection [i];
				vec3 c = x3d_LightAttenuation [i];
				vec3 L = di ? -d : normalize (vL);      // Normalized vector from point on geometry to light source i position.
				vec3 H = normalize (L + V);             // Specular term

				float lightAngle     = dot (N, L);      // Angle between normal and light ray.
				vec3  diffuseTerm    = diffuseFactor * clamp (lightAngle, 0.0, 1.0);
				float specularFactor = shininess > 0.0 ? pow (max (dot (N, H), 0.0), shininess * 128.0) : 1.0;
				vec3  specularTerm   = specularColor * specularFactor;

				float attenuationFactor           = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);
				float spotFactor                  = lightType == x3d_SpotLight ? getSpotFactor (x3d_LightCutOffAngle [i], x3d_LightBeamWidth [i], L, d) : 1.0;
				float attenuationSpotFactor       = attenuationFactor * spotFactor;
				vec3  ambientColor                = x3d_LightAmbientIntensity [i] * ambientTerm;
				vec3  ambientDiffuseSpecularColor = ambientColor + x3d_LightIntensity [i] * (diffuseTerm + specularTerm);

				#ifdef X3D_SHADOWS

				if (x3d_ShadowIntensity [i] > 0.0 && lightAngle > 0.0)
				{
					float shadowIntensity = getShadowIntensity (i, lightType, x3d_ShadowIntensity [i], x3d_ShadowDiffusion [i], x3d_ShadowMatrix [i], plane, lightAngle);
	
					finalColor += attenuationSpotFactor * (mix (x3d_LightColor [i], x3d_ShadowColor [i], shadowIntensity) * ambientDiffuseSpecularColor);
				}
				else
					finalColor += attenuationSpotFactor * (x3d_LightColor [i] * ambientDiffuseSpecularColor);

				#else

					finalColor += attenuationSpotFactor * (x3d_LightColor [i] * ambientDiffuseSpecularColor);

				#endif
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
			if (x3d_TextureType [0] != x3d_NoneTexture)
			{
				vec4 T = getTextureColor ();

				finalColor = T * C;
			}
			else
				finalColor = C;
		}
		else
		{
			if (x3d_TextureType [0] != x3d_NoneTexture)
				finalColor = getTextureColor ();
		}

		return finalColor;
	}
}

vec3
getFogColor (in vec3 color)
{
	if (x3d_FogType == x3d_NoneFog)
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
	seed (int (fract (dot (v, v)) * float (RAND_MAX)));

	clip ();

	gl_FragColor = getMaterialColor ();

	gl_FragColor .rgb = getFogColor (gl_FragColor .rgb);
}