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

uniform int   x3d_LightType [MAX_LIGHTS]; // 0: DirectionalLight, 1: PointLight, 2: SpotLight
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

float
getFogInterpolant ()
{
	if (x3d_FogType == NO_FOG)
		return 1.0;

	float dV = length (v);

	if (dV >= x3d_FogVisibilityRange)
		return 0.0;

	if (x3d_FogType == LINEAR_FOG)
		return (x3d_FogVisibilityRange - dV) / x3d_FogVisibilityRange;

	if (x3d_FogType == EXPONENTIAL_FOG)
		return exp (-dV / (x3d_FogVisibilityRange - dV));

	return 1.0;
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

void
main ()
{
	clip ();

	float f0 = getFogInterpolant ();

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
			int t = x3d_LightType [i];

			if (t != NO_LIGHT)
			{
				vec3  vL = x3d_LightLocation [i] - v;
				float dL = length (vL);
				bool  di = t == DIRECTIONAL_LIGHT;

				if (di || dL <= x3d_LightRadius [i])
				{
					vec3 d = x3d_LightDirection [i];
					vec3 c = x3d_LightAttenuation [i];
					vec3 L = di ? -d : normalize (vL);
					vec3 H = normalize (L + V); // specular term
	
					vec3  diffuseTerm    = diffuseFactor * max (dot (N, L), 0.0);
					float specularFactor = bool (shininess) ? pow (max (dot (N, H), 0.0), shininess * 128.0) : 1.0;
					vec3  specularTerm   = specularColor * specularFactor;
	
					float attenuation = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);
					float spot        = 1.0;
	
					if (t == SPOT_LIGHT)
					{
						float spotAngle   = acos (clamp (dot (-L, d), -1.0, 1.0));
						float cutOffAngle = x3d_LightCutOffAngle [i];
						float beamWidth   = x3d_LightBeamWidth [i];
						
						if (spotAngle >= cutOffAngle)
							spot = 0.0;
						else if (spotAngle <= beamWidth)
							spot = 1.0;
						else
							spot = (spotAngle - cutOffAngle) / (beamWidth - cutOffAngle);
					}
	
					finalColor += (attenuation * spot) * x3d_LightColor [i] *
					              (x3d_LightAmbientIntensity [i] * ambientTerm +
					               x3d_LightIntensity [i] * (diffuseTerm + specularTerm));
				}
			}
			else
				break;
		}

		finalColor += emissiveColor;

		gl_FragColor = vec4 (finalColor, alpha);
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

		gl_FragColor = finalColor;
	}

	gl_FragColor .rgb = mix (x3d_FogColor, gl_FragColor .rgb, f0);
}
