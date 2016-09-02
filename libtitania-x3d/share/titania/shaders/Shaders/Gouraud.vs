// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

precision mediump float;

// 225 uniforms

uniform mat4 x3d_TextureMatrix [1];
uniform mat3 x3d_NormalMatrix;
uniform mat4 x3d_ProjectionMatrix;
uniform mat4 x3d_ModelViewMatrix;
// 3 * 16 + 9

uniform float x3d_LinewidthScaleFactor;
uniform bool  x3d_Lighting;      // true if a X3DMaterialNode is attached, otherwise false
uniform bool  x3d_ColorMaterial; // true if a X3DColorNode is attached, otherwise false
// 3

#define MAX_LIGHTS        8
#define NO_LIGHT          0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT       2
#define SPOT_LIGHT        3

uniform int   x3d_LightType [MAX_LIGHTS]; // 0: No, 1: DirectionalLight, 2: PointLight, 3: SpotLight
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

uniform bool x3d_SeparateBackColor;
// 1

uniform float x3d_AmbientIntensity;
uniform vec3  x3d_DiffuseColor;
uniform vec3  x3d_SpecularColor;
uniform vec3  x3d_EmissiveColor;
uniform float x3d_Shininess;
uniform float x3d_Transparency;
// 12

uniform float x3d_BackAmbientIntensity;
uniform vec3  x3d_BackDiffuseColor;
uniform vec3  x3d_BackSpecularColor;
uniform vec3  x3d_BackEmissiveColor;
uniform float x3d_BackShininess;
uniform float x3d_BackTransparency;
// 12

attribute vec4 x3d_Color;
attribute vec4 x3d_TexCoord;
attribute vec3 x3d_Normal;
attribute vec4 x3d_Vertex;
// 15, max 16

varying vec4  frontColor; // color
varying vec4  backColor;  // color
varying vec4  t;          // texCoord
varying vec4  v;          // point on geometry
// 15, max 16

float
getSpotFactor (int lightType, float cutOffAngle, float beamWidth, vec3 L, vec3 d)
{
	if (lightType != SPOT_LIGHT)
		return 1.0;

	float spotAngle = acos (clamp (dot (-L, d), -1.0, 1.0));
	
	if (spotAngle >= cutOffAngle)
		return 0.0;
	else if (spotAngle <= beamWidth)
		return 1.0;

	return (spotAngle - cutOffAngle) / (beamWidth - cutOffAngle);
}

vec4
getMaterial (vec3 N,
             vec3 v,
             float x3d_AmbientIntensity,
             vec3  x3d_DiffuseColor,
             vec3  x3d_SpecularColor,
             vec3  x3d_EmissiveColor,
             float x3d_Shininess,
             float x3d_Transparency)
{  
	vec3 V = normalize (-v); // normalized vector from point on geometry to viewer's position

	// Calculate diffuseFactor & alpha

	vec3  diffuseFactor = vec3 (1.0, 1.0, 1.0);
	float alpha         = 1.0 - x3d_Transparency;

	if (x3d_ColorMaterial)
	{
		diffuseFactor  = x3d_Color .rgb;
		alpha         *= x3d_Color .a;
	}
	else
		diffuseFactor = x3d_DiffuseColor;

	vec3 ambientTerm = diffuseFactor * x3d_AmbientIntensity;

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
			vec3 d = x3d_LightDirection [i];
			vec3 c = x3d_LightAttenuation [i];
			vec3 L = di ? -d : normalize (vL);
			vec3 H = normalize (L + V); // specular term

			vec3  diffuseTerm    = diffuseFactor * max (dot (N, L), 0.0);
			float specularFactor = bool (x3d_Shininess) ? pow (max (dot (N, H), 0.0), x3d_Shininess * 128.0) : 1.0;
			vec3  specularTerm   = x3d_SpecularColor * specularFactor;

			float attenuation = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);
			float spot        = getSpotFactor (lightType, x3d_LightCutOffAngle [i], x3d_LightBeamWidth [i], L, d);
		
			vec3 lightFactor  = (attenuation * spot) * x3d_LightColor [i];
			vec3 ambientLight = (lightFactor * x3d_LightAmbientIntensity [i]) * ambientTerm;

			lightFactor *= x3d_LightIntensity [i];
			finalColor  += ambientLight + lightFactor * (diffuseTerm + specularTerm);
		}
	}

	finalColor += x3d_EmissiveColor;

	return vec4 (clamp (finalColor, 0.0, 1.0), alpha);
}

void
main ()
{
	gl_PointSize = x3d_LinewidthScaleFactor;

	t = x3d_TextureMatrix [0] * x3d_TexCoord;
	v = x3d_ModelViewMatrix * x3d_Vertex;

	gl_Position = x3d_ProjectionMatrix * v;

	if (x3d_Lighting)
	{
		vec3 N = normalize (x3d_NormalMatrix * x3d_Normal);

		float ambientIntensity = x3d_AmbientIntensity;
		vec3  diffuseColor     = x3d_DiffuseColor;
		vec3  specularColor    = x3d_SpecularColor;
		vec3  emissiveColor    = x3d_EmissiveColor;
		float shininess        = x3d_Shininess;
		float transparency     = x3d_Transparency;

		frontColor = getMaterial (N, v .xyz,
		                          ambientIntensity,
		                          diffuseColor,
		                          specularColor,
		                          emissiveColor,
		                          shininess,
		                          transparency);

		if (x3d_SeparateBackColor)
		{
			ambientIntensity = x3d_BackAmbientIntensity;
			diffuseColor     = x3d_BackDiffuseColor;
			specularColor    = x3d_BackSpecularColor;
			emissiveColor    = x3d_BackEmissiveColor;
			shininess        = x3d_BackShininess;
			transparency     = x3d_BackTransparency;
		}
			
		backColor = getMaterial (-N, v .xyz,
		                         ambientIntensity,
		                         diffuseColor,
		                         specularColor,
		                         emissiveColor,
		                         shininess,
		                         transparency);
	}
	else
	{
	   frontColor = backColor = x3d_ColorMaterial ? x3d_Color : vec4 (1.0, 1.0, 1.0, 1.0);
	}
}
