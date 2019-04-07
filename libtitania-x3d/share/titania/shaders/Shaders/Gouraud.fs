// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

#ifdef X3D_LOGARITHMIC_DEPTH_BUFFER
#extension GL_EXT_frag_depth : enable
#endif

precision mediump float;
precision mediump int;

uniform int x3d_GeometryType;

uniform int  x3d_NumClipPlanes;
uniform vec4 x3d_ClipPlane [x3d_MaxClipPlanes];

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;      // true if a X3DMaterialNode is attached, otherwise false
uniform bool  x3d_ColorMaterial; // true if a X3DColorNode is attached, otherwise false

uniform int         x3d_NumTextures;
uniform int         x3d_TextureType [x3d_MaxTextures]; // x3d_None, x3d_TextureType2D or x3d_TextureTypeCubeMapTexture
uniform sampler2D   x3d_Texture2D [x3d_MaxTextures];
uniform samplerCube x3d_CubeMapTexture [x3d_MaxTextures];

uniform vec4 x3d_MultiTextureColor;
uniform x3d_MultiTextureParameters x3d_MultiTexture [x3d_MaxTextures];

uniform x3d_TextureCoordinateGeneratorParameters x3d_TextureCoordinateGenerator [x3d_MaxTextures];  

uniform x3d_FogParameters x3d_Fog;

varying float fogDepth;    // fog depth
varying vec4  frontColor;  // color
varying vec4  backColor;   // color
varying vec4  texCoord0;   // texCoord0
varying vec4  texCoord1;   // texCoord1
varying vec3  normal;      // normal vector at this point on geometry
varying vec3  vertex;      // point on geometry
varying vec3  localNormal; // normal vector at this point on geometry in local coordinates
varying vec3  localVertex; // point on geometry in local coordinates

#ifdef X3D_LOGARITHMIC_DEPTH_BUFFER
uniform float x3d_LogarithmicFarFactor1_2;
varying float depth;
#endif

#pragma X3D include "Include/Texture.h"

void
clip ()
{
	for (int i = 0; i < x3d_MaxClipPlanes; ++ i)
	{
		if (i == x3d_NumClipPlanes)
			break;

		if (dot (vertex, x3d_ClipPlane [i] .xyz) - x3d_ClipPlane [i] .w < 0.0)
			discard;
	}
}

float
getFogInterpolant ()
{
	// Returns 0.0 for fog color and 1.0 for material color.

	if (x3d_Fog .type == x3d_None)
		return 1.0;

	float visibilityRange = x3d_Fog .fogCoord ? fogDepth : x3d_Fog .visibilityRange;

	if (visibilityRange <= 0.0)
		return 1.0;

	float dV = length (x3d_Fog .matrix * vertex);

	if (dV >= visibilityRange)
		return 0.0;

	if (x3d_Fog .type == x3d_LinearFog)
		return (visibilityRange - dV) / visibilityRange;

	if (x3d_Fog .type == x3d_ExponentialFog)
		return exp (-dV / (visibilityRange - dV));

	return 1.0;
}

vec3
getFogColor (const in vec3 color)
{
	return mix (x3d_Fog .color, color, getFogInterpolant ());
}

void
main ()
{
 	clip ();

	vec4 finalColor = gl_FrontFacing ? frontColor : backColor;

	if (x3d_NumTextures > 0)
	{
		if (x3d_Lighting)
		{
			finalColor = getTextureColor (finalColor, vec4 (1.0));
		}
		else
		{
			if (x3d_ColorMaterial)
			{
				finalColor = getTextureColor (finalColor, vec4 (1.0));
			}
			else
			{
				finalColor = getTextureColor (vec4 (1.0), vec4 (1.0));
			}
		}
	}

	gl_FragColor .rgb = getFogColor (finalColor .rgb);
	gl_FragColor .a   = finalColor .a;

	#ifdef X3D_LOGARITHMIC_DEPTH_BUFFER
	//http://outerra.blogspot.com/2013/07/logarithmic-depth-buffer-optimizations.html
	if (x3d_LogarithmicFarFactor1_2 > 0.0)
		gl_FragDepthEXT = log2 (depth) * x3d_LogarithmicFarFactor1_2;
	else
		gl_FragDepthEXT = gl_FragCoord .z;
	#endif
}
