#version 400
#extension GL_ARB_vertex_attrib_64bit : enable
#extension GL_ARB_separate_shader_objects : enable

uniform double time;
uniform float  deltaTime;
uniform float  particleLifetime;
uniform float  lifetimeVariation;

uniform vec3   position;
uniform vec3   direction;
uniform float  speed;
uniform float  variation;

uniform vec3  velocity [32];
uniform float turbulence [32];
uniform int   forces;

layout (location = 0)
in struct From
{
	int    seed;
	float  lifetime;
	vec3   position;
	vec3   velocity;
	double startTime;
}
from;

out To
{
	int    seed;
	float  lifetime;
	vec3   position;
	vec3   velocity;
	double startTime;
}
to;

/* Math */

float M_PI    = 3.14159265358979323846;
float M_PI1_2 = M_PI * 0.5;
float M_PI2   = 2.0f * M_PI;

/* Random number generation */

int RAND_MAX = 0x7fffffff;
int seed     = 1;

void
srand (in int value)
{
	seed = value;
}

int
rand ()
{
	return seed = seed * 1103515245 + 12345;
}

float
random1 ()
{
	return float (rand ()) / float (RAND_MAX);
}

float
random1 (in float min, in float max)
{
	return min + fract (random1 ()) * (max - min);
}

/* 
 * http://www.jasondavies.com/maps/random-points/
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */

vec3
random_normal ()
{
	float theta = random1 () * M_PI;
	float cphi  = random1 ();
	float phi   = acos (cphi);
	float r     = sin (phi);

	return vec3 (sin (theta) * r,
	             cos (theta) * r,
	             cphi);
}

vec3
random_normal (in float angle)
{
	float theta = random1 () * M_PI;
	float cphi  = random1 (cos (angle), 1.0f);
	float phi   = acos (cphi);
	float r     = sin (phi);

	return vec3 (sin (theta) * r,
	             cos (theta) * r,
	             cphi);
}

float
random_variation (in float value, in float variation)
{
	return value + value * variation * random1 ();
}

/* Rotation */

vec4
quaternion (in vec3 fromVector, in vec3 toVector)
{
	vec3 from = normalize (fromVector);
	vec3 to   = normalize (toVector);

	float cos_angle = dot (from, to);
	vec3  crossvec  = normalize (cross (from, to));
	float crosslen  = length (crossvec);

	if (crosslen == 0.0f)
	{
		if (cos_angle > 0.0f)
			return vec4 (0.0f, 0.0f, 0.0f, 1.0f);

		else
		{
			vec3 t = cross (from, vec3 (1.0f, 0.0f, 0.0f));

			if (dot (t, t) == 0.0f)
				t = cross (from, vec3 (0.0f, 1.0f, 0.0f));

			t = normalize (t);

			return vec4 (t, 0.0f);
		}
	}
	else
	{
		crossvec *= sqrt (abs (1.0f - cos_angle) * 0.5f);
		return vec4 (crossvec, sqrt (abs (1.0f + cos_angle) * 0.5f));
	}
}

vec4
inverse (in vec4 quat)
{
	quat .xyz = -quat .xyz;
	return quat;
}

vec4
multRight (in vec4 lhs, in vec4 rhs)
{
	return vec4 (lhs .w * rhs .x +
	             lhs .x * rhs .w +
	             lhs .y * rhs .z -
	             lhs .z * rhs .y,

	             lhs .w * rhs .y +
	             lhs .y * rhs .w +
	             lhs .z * rhs .x -
	             lhs .x * rhs .z,

	             lhs .w * rhs .z +
	             lhs .z * rhs .w +
	             lhs .x * rhs .y -
	             lhs .y * rhs .x,

	             lhs .w * rhs .w -
	             lhs .x * rhs .x -
	             lhs .y * rhs .y -
	             lhs .z * rhs .z);
}

vec3
multVec (in vec4 quat, in vec3 vector)
{
	vec4 result = multRight (multRight (quat, vec4 (vector, 0.0f)), inverse (quat));
	return result .xyz;
}

/* main */

vec3
getRandomVelocity ()
{
	float randomSpeed = abs (random_variation (speed, variation));

	if (direction == vec3 (0.0f, 0.0f, 0.0f))
		return randomSpeed * random_normal ();

	return randomSpeed * direction;
}

void
main ()
{
	srand (from .seed);

	if (time - from .startTime > from .lifetime)
	{
		to .lifetime  = random_variation (particleLifetime, lifetimeVariation);
		to .position  = position;
		to .velocity  = getRandomVelocity ();
		to .startTime = time;
		to .seed      = seed;
	}
	else
	{
		vec3 v = from .velocity;

		for (int i = 0; i < forces; ++ i)
		{
			float speed = length (velocity [i]);
		
			if (speed < 1e-6f)
				continue;

			vec4 rotation = quaternion (vec3 (0.0f, 0.0f, 1.0f), velocity [i]);
			vec3 normal   = multVec (rotation, random_normal (turbulence [i]));
			v += normal * speed;
		}

		to .lifetime  = from .lifetime;
		to .position  = from .position + v * deltaTime;
		to .velocity  = v;
		to .startTime = from .startTime;
		to .seed      = seed;
	}
}
