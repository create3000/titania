// -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-

struct x3d_FogParameters {
	mediump int   type;
	mediump vec3  color;
	mediump float visibilityRange;
};

//uniform x3d_FogParameters x3d_Fog;

struct x3d_LightSourceParameters {
	mediump int   type;
	mediump vec3  color;
	mediump float intensity;
	mediump float ambientIntensity;
	mediump vec3  attenuation;
	mediump vec3  location;
	mediump vec3  direction;
	mediump float radius;
	mediump float beamWidth;
	mediump float cutOffAngle;
	// Shadow
	mediump vec3  shadowColor;
	mediump float shadowIntensity;
	mediump float shadowBias;
	mediump mat4  shadowMatrix;
	mediump int   shadowMapSize;
};

//uniform x3d_LightSourceParameters x3d_LightSource [x3d_MaxLights];

struct x3d_MaterialParameters  
{   
	mediump float ambientIntensity;
	mediump vec3  diffuseColor;
	mediump vec3  specularColor;
	mediump vec3  emissiveColor;
	mediump float shininess;
	mediump float transparency;
};

//uniform x3d_MaterialParameters x3d_FrontMaterial;  
//uniform x3d_MaterialParameters x3d_BackMaterial;        
