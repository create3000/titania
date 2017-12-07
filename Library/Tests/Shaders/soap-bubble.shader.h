
precision highp float;
precision highp int;

#define SHADER_NAME ShaderMaterial
#define VERTEX_TEXTURES
#define GAMMA_FACTOR 2
#define MAX_BONES 0
#define BONE_TEXTURE
#define NUM_CLIPPING_PLANES 0

uniform mat4   modelMatrix;
uniform mat4   modelViewMatrix;
uniform mat4   projectionMatrix;
uniform mat4   viewMatrix;
uniform mat3   normalMatrix;
uniform vec3   cameraPosition;
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

#ifdef USE_COLOR
attribute vec3 color;
#endif

#ifdef USE_MORPHTARGETS
attribute vec3 morphTarget0;
attribute vec3 morphTarget1;
attribute vec3 morphTarget2;
attribute vec3 morphTarget3;
#ifdef USE_MORPHNORMALS
attribute vec3 morphNormal0;
attribute vec3 morphNormal1;
attribute vec3 morphNormal2;
attribute vec3 morphNormal3;
#else
attribute vec3 morphTarget4;
attribute vec3 morphTarget5;
attribute vec3 morphTarget6;
attribute vec3 morphTarget7;
#endif
#endif

#ifdef USE_SKINNING
attribute vec4 skinIndex;
attribute vec4 skinWeight;
#endif

uniform float mRefractionRatio;
uniform float mFresnelBias;
uniform float mFresnelScale;
uniform float mFresnelPower;
varying vec3  vReflect;
varying vec3  vRefract [3];
varying float vReflectionFactor;

void
main ()
{
	vec4 mvPosition    = modelViewMatrix * vec4 (position, 1.0);
	vec4 worldPosition = modelMatrix * vec4 (position, 1.0);
	vec3 worldNormal   = normalize (mat3 (modelMatrix [0].xyz, modelMatrix [1].xyz, modelMatrix [2].xyz) * normal);
	vec3 I             = worldPosition .xyz - cameraPosition;

	vReflect          = reflect (I, worldNormal);
	vRefract [0]      = refract (normalize (I), worldNormal, mRefractionRatio);
	vRefract [1]      = refract (normalize (I), worldNormal, mRefractionRatio * 0.99);
	vRefract [2]      = refract (normalize (I), worldNormal, mRefractionRatio * 0.98);
	vReflectionFactor = mFresnelBias + mFresnelScale * pow (1.0 + dot (normalize (I), worldNormal), mFresnelPower);
	gl_Position       = projectionMatrix * mvPosition;
}

precision highp float;
precision highp int;

#define SHADER_NAME ShaderMaterial
#define GAMMA_FACTOR 2
#define NUM_CLIPPING_PLANES 0
#define UNION_CLIPPING_PLANES 0

uniform mat4 viewMatrix;
uniform vec3 cameraPosition;

#define TONE_MAPPING

#ifndef saturate
#define saturate(a) clamp (a, 0.0, 1.0)
#endif

uniform float toneMappingExposure;
uniform float toneMappingWhitePoint;

vec3
LinearToneMapping (vec3 color)
{
	return toneMappingExposure * color;
}

vec3
ReinhardToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	return saturate (color / (vec3 (1.0) + color));
}

#define Uncharted2Helper(x) max (((x * (0.15 * x + 0.10 * 0.50) + 0.20 * 0.02) / (x * (0.15 * x + 0.50) + 0.20 * 0.30)) - 0.02 / 0.30, vec3 (0.0))

vec3
Uncharted2ToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	return saturate (Uncharted2Helper (color) / Uncharted2Helper (vec3 (toneMappingWhitePoint)));
}

vec3
OptimizedCineonToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	color  = max (vec3 (0.0), color - 0.004);
	return pow ((color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06), vec3 (2.2));
}

vec3
toneMapping (vec3 color)
{
	return LinearToneMapping (color);
}

vec4
LinearToLinear (in vec4 value)
{
	return value;
}

vec4
GammaToLinear (in vec4 value, in float gammaFactor)
{
	return vec4 (pow (value.xyz, vec3 (gammaFactor)), value .w);
}

vec4
LinearToGamma (in vec4 value, in float gammaFactor)
{
	return vec4 (pow (value.xyz, vec3 (1.0 / gammaFactor)), value .w);
}

vec4
sRGBToLinear (in vec4 value)
{
	return vec4 (mix (pow (value.rgb * 0.9478672986 + vec3 (0.0521327014), vec3 (2.4)), value .rgb * 0.0773993808, vec3 (lessThanEqual (value.rgb, vec3 (0.04045)))), value .w);
}

vec4
LinearTosRGB (in vec4 value)
{
	return vec4 (mix (pow (value.rgb, vec3 (0.41666)) * 1.055 - vec3 (0.055), value .rgb * 12.92, vec3 (lessThanEqual (value.rgb, vec3 (0.0031308)))), value .w);
}

vec4
RGBEToLinear (in vec4 value)
{
	return vec4 (value.rgb * exp2 (value.a * 255.0 - 128.0), 1.0);
}

vec4
LinearToRGBE (in vec4 value)
{
	float maxComponent = max (max (value.r, value .g), value .b);
	float fExp         = clamp (ceil (log2 (maxComponent)), -128.0, 127.0);

	return vec4 (value.rgb / exp2 (fExp), (fExp + 128.0) / 255.0);
}

vec4
RGBMToLinear (in vec4 value, in float maxRange)
{
	return vec4 (value.xyz * value .w * maxRange, 1.0);
}

vec4
LinearToRGBM (in vec4 value, in float maxRange)
{
	float maxRGB = max (value.x, max (value.g, value .b));
	float M      = clamp (maxRGB / maxRange, 0.0, 1.0);

	M = ceil (M * 255.0) / 255.0;
	return vec4 (value.rgb / (M * maxRange), M);
}

vec4
RGBDToLinear (in vec4 value, in float maxRange)
{
	return vec4 (value.rgb * ((maxRange / 255.0) / value .a), 1.0);
}

vec4
LinearToRGBD (in vec4 value, in float maxRange)
{
	float maxRGB = max (value.x, max (value.g, value .b));
	float D      = max (maxRange / maxRGB, 1.0);

	D = min (floor (D) / 255.0, 1.0);
	return vec4 (value.rgb * (D * (255.0 / maxRange)), D);
}

const mat3 cLogLuvM = mat3 (0.2209, 0.3390, 0.4184, 0.1138, 0.6780, 0.7319, 0.0102, 0.1130, 0.2969);
vec4
LinearToLogLuv (in vec4 value)
{
	vec3 Xp_Y_XYZp = value .rgb * cLogLuvM;

	Xp_Y_XYZp = max (Xp_Y_XYZp, vec3 (1e-6, 1e-6, 1e-6));
	vec4 vResult;
	vResult .xy = Xp_Y_XYZp .xy / Xp_Y_XYZp .z;
	float Le = 2.0 * log2 (Xp_Y_XYZp.y) + 127.0;
	vResult .w = fract (Le);
	vResult .z = (Le - (floor (vResult.w * 255.0)) / 255.0) / 255.0;
	return vResult;
}

const mat3 cLogLuvInverseM = mat3 (6.0014, -2.7008, -1.7996, -1.3320, 3.1029, -5.7721, 0.3008, -1.0882, 5.6268);
vec4
LogLuvToLinear (in vec4 value)
{
	float Le = value .z * 255.0 + value .w;
	vec3  Xp_Y_XYZp;

	Xp_Y_XYZp .y = exp2 ((Le - 127.0) / 2.0);
	Xp_Y_XYZp .z = Xp_Y_XYZp .y / value .y;
	Xp_Y_XYZp .x = value .x * Xp_Y_XYZp .z;
	vec3 vRGB = Xp_Y_XYZp .rgb * cLogLuvInverseM;
	return vec4 (max (vRGB, 0.0), 1.0);
}

vec4
mapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
envMapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
emissiveMapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
linearToOutputTexel (vec4 value)
{
	return LinearToLinear (value);
}

uniform samplerCube tCube;
varying vec3        vReflect;
varying vec3        vRefract [3];
varying float       vReflectionFactor;

void
main ()
{
	vec4 reflectedColor = textureCube (tCube, vec3 (-vReflect.x, vReflect .yz));
	vec4 refractedColor = vec4 (1.0);

	refractedColor .r = textureCube (tCube, vec3 (-vRefract [0].x, vRefract [0].yz)).r;
	refractedColor .g = textureCube (tCube, vec3 (-vRefract [1].x, vRefract [1].yz)).g;
	refractedColor .b = textureCube (tCube, vec3 (-vRefract [2].x, vRefract [2].yz)).b;
	gl_FragColor     = mix (refractedColor, reflectedColor, clamp (vReflectionFactor, 0.0, 1.0));
}

precision highp float;
precision highp int;

#define SHADER_NAME ShaderMaterial
#define VERTEX_TEXTURES
#define GAMMA_FACTOR 2
#define MAX_BONES 0
#define BONE_TEXTURE
#define FLIP_SIDED
#define NUM_CLIPPING_PLANES 0

uniform mat4   modelMatrix;
uniform mat4   modelViewMatrix;
uniform mat4   projectionMatrix;
uniform mat4   viewMatrix;
uniform mat3   normalMatrix;
uniform vec3   cameraPosition;
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

#ifdef USE_COLOR
attribute vec3 color;
#endif

#ifdef USE_MORPHTARGETS
attribute vec3 morphTarget0;
attribute vec3 morphTarget1;
attribute vec3 morphTarget2;
attribute vec3 morphTarget3;
#ifdef USE_MORPHNORMALS
attribute vec3 morphNormal0;
attribute vec3 morphNormal1;
attribute vec3 morphNormal2;
attribute vec3 morphNormal3;
#else
attribute vec3 morphTarget4;
attribute vec3 morphTarget5;
attribute vec3 morphTarget6;
attribute vec3 morphTarget7;
#endif
#endif

#ifdef USE_SKINNING
attribute vec4 skinIndex;
attribute vec4 skinWeight;
#endif

varying vec3 vWorldPosition;

#define PI 3.14159265359
#define PI2 6.28318530718
#define PI_HALF 1.5707963267949
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6
#define saturate(a) clamp (a, 0.0, 1.0)
#define whiteCompliment(a) (1.0 - saturate (a))

float
pow2 (const in float x)
{
	return x * x;
}

float
pow3 (const in float x)
{
	return x * x * x;
}

float
pow4 (const in float x)
{
	float x2 = x * x; return x2 * x2;
}

float
average (const in vec3 color)
{
	return dot (color, vec3 (0.3333));
}

highp float
rand (const in vec2 uv)
{
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float       dt = dot (uv.xy, vec2 (a, b)), sn = mod (dt, PI);

	return fract (sin (sn) * c);
}

struct IncidentLight
{
	vec3 color;
	vec3 direction;
	bool visible;

};
struct ReflectedLight
{
	vec3 directDiffuse;
	vec3 directSpecular;
	vec3 indirectDiffuse;
	vec3 indirectSpecular;

};
struct GeometricContext
{
	vec3 position;
	vec3 normal;
	vec3 viewDir;

};
vec3
transformDirection (in vec3 dir, in mat4 matrix)
{
	return normalize ((matrix * vec4 (dir, 0.0)).xyz);
}

vec3
inverseTransformDirection (in vec3 dir, in mat4 matrix)
{
	return normalize ((vec4 (dir, 0.0) * matrix) .xyz);
}

vec3
projectOnPlane (in vec3 point, in vec3 pointOnPlane, in vec3 planeNormal)
{
	float distance = dot (planeNormal, point - pointOnPlane);

	return -distance * planeNormal + point;
}

float
sideOfPlane (in vec3 point, in vec3 pointOnPlane, in vec3 planeNormal)
{
	return sign (dot (point - pointOnPlane, planeNormal));
}

vec3
linePlaneIntersect (in vec3 pointOnLine, in vec3 lineDirection, in vec3 pointOnPlane, in vec3 planeNormal)
{
	return lineDirection * (dot (planeNormal, pointOnPlane - pointOnLine) / dot (planeNormal, lineDirection)) + pointOnLine;
}

mat3
transposeMat3 (const in mat3 m)
{
	mat3 tmp;

	tmp [ 0 ] = vec3 (m [ 0 ] .x, m [ 1 ] .x, m [ 2 ] .x);
	tmp [ 1 ] = vec3 (m [ 0 ] .y, m [ 1 ] .y, m [ 2 ] .y);
	tmp [ 2 ] = vec3 (m [ 0 ] .z, m [ 1 ] .z, m [ 2 ] .z);
	return tmp;
}

float
linearToRelativeLuminance (const in vec3 color)
{
	vec3 weights = vec3 (0.2126, 0.7152, 0.0722);

	return dot (weights, color .rgb);
}

void
main ()
{
	vWorldPosition = transformDirection (position, modelMatrix);

	vec3 transformed = vec3 (position);

	vec4 mvPosition = modelViewMatrix * vec4 (transformed, 1.0);
	gl_Position = projectionMatrix * mvPosition;

	gl_Position .z = gl_Position .w;
}

precision highp float;
precision highp int;

#define SHADER_NAME ShaderMaterial
#define GAMMA_FACTOR 2
#define FLIP_SIDED
#define NUM_CLIPPING_PLANES 0
#define UNION_CLIPPING_PLANES 0

uniform mat4 viewMatrix;
uniform vec3 cameraPosition;

#define TONE_MAPPING

#ifndef saturate
#define saturate(a) clamp (a, 0.0, 1.0)
#endif

uniform float toneMappingExposure;
uniform float toneMappingWhitePoint;

vec3
LinearToneMapping (vec3 color)
{
	return toneMappingExposure * color;
}

vec3
ReinhardToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	return saturate (color / (vec3 (1.0) + color));
}

#define Uncharted2Helper(x) max (((x * (0.15 * x + 0.10 * 0.50) + 0.20 * 0.02) / (x * (0.15 * x + 0.50) + 0.20 * 0.30)) - 0.02 / 0.30, vec3 (0.0))
vec3
Uncharted2ToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	return saturate (Uncharted2Helper (color) / Uncharted2Helper (vec3 (toneMappingWhitePoint)));
}

vec3
OptimizedCineonToneMapping (vec3 color)
{
	color *= toneMappingExposure;
	color  = max (vec3 (0.0), color - 0.004);
	return pow ((color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06), vec3 (2.2));
}

vec3
toneMapping (vec3 color)
{
	return LinearToneMapping (color);
}

vec4
LinearToLinear (in vec4 value)
{
	return value;
}

vec4
GammaToLinear (in vec4 value, in float gammaFactor)
{
	return vec4 (pow (value.xyz, vec3 (gammaFactor)), value .w);
}

vec4
LinearToGamma (in vec4 value, in float gammaFactor)
{
	return vec4 (pow (value.xyz, vec3 (1.0 / gammaFactor)), value .w);
}

vec4
sRGBToLinear (in vec4 value)
{
	return vec4 (mix (pow (value.rgb * 0.9478672986 + vec3 (0.0521327014), vec3 (2.4)), value .rgb * 0.0773993808, vec3 (lessThanEqual (value.rgb, vec3 (0.04045)))), value .w);
}

vec4
LinearTosRGB (in vec4 value)
{
	return vec4 (mix (pow (value.rgb, vec3 (0.41666)) * 1.055 - vec3 (0.055), value .rgb * 12.92, vec3 (lessThanEqual (value.rgb, vec3 (0.0031308)))), value .w);
}

vec4
RGBEToLinear (in vec4 value)
{
	return vec4 (value.rgb * exp2 (value.a * 255.0 - 128.0), 1.0);
}

vec4
LinearToRGBE (in vec4 value)
{
	float maxComponent = max (max (value.r, value .g), value .b);
	float fExp         = clamp (ceil (log2 (maxComponent)), -128.0, 127.0);

	return vec4 (value.rgb / exp2 (fExp), (fExp + 128.0) / 255.0);
}

vec4
RGBMToLinear (in vec4 value, in float maxRange)
{
	return vec4 (value.xyz * value .w * maxRange, 1.0);
}

vec4
LinearToRGBM (in vec4 value, in float maxRange)
{
	float maxRGB = max (value.x, max (value.g, value .b));
	float M      = clamp (maxRGB / maxRange, 0.0, 1.0);

	M = ceil (M * 255.0) / 255.0;
	return vec4 (value.rgb / (M * maxRange), M);
}

vec4
RGBDToLinear (in vec4 value, in float maxRange)
{
	return vec4 (value.rgb * ((maxRange / 255.0) / value .a), 1.0);
}

vec4
LinearToRGBD (in vec4 value, in float maxRange)
{
	float maxRGB = max (value.x, max (value.g, value .b));
	float D      = max (maxRange / maxRGB, 1.0);

	D = min (floor (D) / 255.0, 1.0);
	return vec4 (value.rgb * (D * (255.0 / maxRange)), D);
}

const mat3 cLogLuvM = mat3 (0.2209, 0.3390, 0.4184, 0.1138, 0.6780, 0.7319, 0.0102, 0.1130, 0.2969);

vec4
LinearToLogLuv (in vec4 value)
{
	vec3 Xp_Y_XYZp = value .rgb * cLogLuvM;

	Xp_Y_XYZp = max (Xp_Y_XYZp, vec3 (1e-6, 1e-6, 1e-6));
	vec4 vResult;
	vResult .xy = Xp_Y_XYZp .xy / Xp_Y_XYZp .z;
	float Le = 2.0 * log2 (Xp_Y_XYZp.y) + 127.0;
	vResult .w = fract (Le);
	vResult .z = (Le - (floor (vResult.w * 255.0)) / 255.0) / 255.0;
	return vResult;
}

const mat3 cLogLuvInverseM = mat3 (6.0014, -2.7008, -1.7996, -1.3320, 3.1029, -5.7721, 0.3008, -1.0882, 5.6268);

vec4
LogLuvToLinear (in vec4 value)
{
	float Le = value .z * 255.0 + value .w;
	vec3  Xp_Y_XYZp;

	Xp_Y_XYZp .y = exp2 ((Le - 127.0) / 2.0);
	Xp_Y_XYZp .z = Xp_Y_XYZp .y / value .y;
	Xp_Y_XYZp .x = value .x * Xp_Y_XYZp .z;
	vec3 vRGB = Xp_Y_XYZp .rgb * cLogLuvInverseM;
	return vec4 (max (vRGB, 0.0), 1.0);
}

vec4
mapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
envMapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
emissiveMapTexelToLinear (vec4 value)
{
	return LinearToLinear (value);
}

vec4
linearToOutputTexel (vec4 value)
{
	return LinearToLinear (value);
}

uniform samplerCube tCube;
uniform float       tFlip;
uniform float       opacity;
varying vec3        vWorldPosition;

void
main ()
{
	gl_FragColor    = textureCube (tCube, vec3 (tFlip * vWorldPosition .x, vWorldPosition .yz));
	gl_FragColor .a *= opacity;
}

