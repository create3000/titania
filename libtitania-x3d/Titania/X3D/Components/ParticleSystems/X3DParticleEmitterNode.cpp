/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "X3DParticleEmitterNode.h"

#include "../../Browser/ParticleSystems/Random.h"

namespace titania {
namespace X3D {

X3DParticleEmitterNode::Fields::Fields () :
	      speed (new SFFloat ()),
	  variation (new SFFloat (0.25)),
	       mass (new SFFloat ()),
	surfaceArea (new SFFloat ())
{ }

X3DParticleEmitterNode::X3DParticleEmitterNode () :
	X3DNode (),
	 fields ()
{
	addType (X3DConstants::X3DParticleEmitterNode);

	speed ()       .setUnit (UnitCategory::SPEED);
	mass ()        .setUnit (UnitCategory::MASS);
	surfaceArea () .setUnit (UnitCategory::AREA);
}

void
X3DParticleEmitterNode::addShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	shader -> addUserDefinedField (inputOutput, "speed",     new SFFloat (speed ()));
	shader -> addUserDefinedField (inputOutput, "variation", new SFFloat (variation ()));
}

void
X3DParticleEmitterNode::setShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	try
	{
		shader -> setField <SFFloat> ("speed",     std::max <float> (0, speed ()),     true);
		shader -> setField <SFFloat> ("variation", std::max <float> (0, variation ()), true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

time_type
X3DParticleEmitterNode::getRandomLifetime (const time_type particleLifetime, const time_type lifetimeVariation) const
{
	const auto v   = particleLifetime * lifetimeVariation;
	const auto min = std::max <time_type> (0, particleLifetime - v);
	const auto max = particleLifetime + v;

	return random1 (0, 1) * (max - min) + min;
}

float
X3DParticleEmitterNode::getRandomSpeed () const
{
	const auto v   = speed () * variation ();
	const auto min = std::max <float> (0, speed () - v);
	const auto max = speed () + v;

	return random1 (0, 1) * (max - min) + min;
}

Vector3f
X3DParticleEmitterNode::getSphericalRandomVelocity () const
{
	return getRandomNormal () * getRandomSpeed ();
}

Vector3f
X3DParticleEmitterNode::getRandomNormal () const
{
	const auto theta = random1 (-1, 1) * pi <float>;
	const auto cphi  = random1 (-1, 1);
	const auto phi   = std::acos (cphi);
	const auto r     = std::sin (phi);

	return Vector3f (std::sin (theta) * r,
	                 std::cos (theta) * r,
	                 cphi);
}

Vector3f
X3DParticleEmitterNode::getRandomNormalWithAngle (const float angle) const
{
	const auto theta = (random1 (0, 1) * 2 - 1) * pi <float>;
	const auto cphi  = random1 (std::cos (angle), 1);
	const auto phi   = std::acos (cphi);
	const auto r     = std::sin (phi);

	return Vector3f (std::sin (theta) * r,
	                 std::cos (theta) * r,
	                 cphi);
}

Vector3f
X3DParticleEmitterNode::getRandomNormalWithDirectionAndAngle (const Vector3f & direction, const float angle) const
{
	Rotation4f rotation (Vector3f (0, 0, 1), direction);

	return rotation * getRandomNormalWithAngle (angle);
}

Vector3f
X3DParticleEmitterNode::getRandomSurfaceNormal () const
{
	const auto theta = random1 (-1, 1) * pi <float>;
	const auto cphi  = std::pow (random1 (0, 1), 1.0/3.0);
	const auto phi   = std::acos (cphi);
	const auto r     = std::sin (phi);

	return Vector3f (std::sin (theta) * r,
	                 std::cos (theta) * r,
	                 cphi);
}

void
X3DParticleEmitterNode::animate (SoftSystem* const softSystem, const time_type deltaTime) const
{
	auto &       particles         = softSystem -> particles;
	const auto & numParticles      = softSystem -> numParticles;
	const auto & createParticles   = softSystem -> createParticles;
	const auto & particleLifetime  = softSystem -> particleLifetime;
	const auto & lifetimeVariation = softSystem -> lifetimeVariation;
	const auto & speeds            = softSystem -> speeds;            // speed of velocities
	const auto & velocities        = softSystem -> velocities;        // resulting velocities from forces
	const auto & turbulences       = softSystem -> turbulences;       // turbulences
	const auto & numForces         = softSystem -> numForces;         // number of forces
	//const auto & boundedPhysics    = softSystem -> boundedVertices .size ();
	//const auto & boundedVolume     = softSystem -> boundedVolume;

	std::vector <Rotation4f> rotations; // rotation to direction of force

	for (const auto & velocity : velocities)
		rotations .emplace_back (Vector3f (0, 0, 1), velocity .getValue ());

	for (auto & particle : particles)
	{
		const auto elapsedTime = particle .elapsedTime + deltaTime;

		if (elapsedTime > particle .lifetime)
		{
			// Create new particle or hide particle.

			particle .lifetime    = getRandomLifetime (particleLifetime, lifetimeVariation);
			particle .elapsedTime = 0;

			if (createParticles)
			{
				particle .position = getRandomPosition ();
				particle .velocity = getRandomVelocity ();
			}
			else
				particle .position = Vector3f (std::numeric_limits <float>::infinity (),
				                               std::numeric_limits <float>::infinity (),
				                               std::numeric_limits <float>::infinity ());
		}
		else
		{
			// Animate particle.

			auto & position = particle .position;
			auto & velocity = particle .velocity;

			for (size_t f = 0; f < numForces; ++ f)
			{
				velocity += speeds [f] .getValue () * (getRandomNormalWithAngle (turbulences [f]) * rotations [f]);
			}

//			if (boundedPhysics)
//			{
//				const auto fromPosition = position;
//
//				position += velocity * deltaTime;
//	
//				bounce (boundedVolume, fromPosition, position, velocity);
//			}
//			else
			{
				position += velocity * float (deltaTime);
			}

			particle .elapsedTime = elapsedTime;
		}
	}

	// Animate color if needed.

	if (softSystem -> colorMaterial)
	{
		getColors (particles,
		           softSystem -> particleSystem -> colorKey (),
		           softSystem -> colorRamp,
		           numParticles);
	}
}

void
X3DParticleEmitterNode::getColors (std::vector <SoftSystem::Particle> & particles,
                                   const MFFloat & colorKeys,
                                   const std::vector <Color4f> & colorRamp,
                                   const size_t numParticles) const
{
	const size_t size   = colorKeys .size ();
	size_t       index0 = 0;
	size_t       index1 = 0;
	float        weight = 0;

	for (auto & particle : particles)
	{
		// Determine index0, index1 and weight.

		const float fraction = particle .elapsedTime / particle .lifetime;

		if (size == 1 || fraction <= colorKeys [0])
		{
			index0 = 0;
			index1 = 0;
			weight = 0;
		}
		else if (fraction >= colorKeys [size - 1])
		{
			index0 = size - 2;
			index1 = size - 1;
			weight = 1;
		}
		else
		{
			const auto iter = std::upper_bound (colorKeys .begin (), colorKeys .end (), fraction);

			if (iter not_eq colorKeys .end ())
			{
				index1 = iter - colorKeys .begin ();
				index0 = index1 - 1;
		
				const float key0 = colorKeys [index0];
				const float key1 = colorKeys [index1];
		
				weight = clamp <float> ((fraction - key0) / (key1 - key0), 0, 1);
			}
			else
			{
				index0 = 0;
				index1 = 0;
				weight = 0;
			}
		}

		// Interpolate and set color.

		const auto & color0 = colorRamp [index0];
		const auto & color1 = colorRamp [index1];

		particle .color = lerp (color0, color1, weight);
	}
}

} // X3D
} // titania
