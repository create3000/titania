/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "WindPhysicsModel.h"

#include "../../Execution/X3DExecutionContext.h"
#include "ParticleSystem.h"

namespace titania {
namespace X3D {

const std::string WindPhysicsModel::componentName  = "ParticleSystems";
const std::string WindPhysicsModel::typeName       = "WindPhysicsModel";
const std::string WindPhysicsModel::containerField = "physics";

WindPhysicsModel::Fields::Fields () :
	 direction (new SFVec3f ()),
	     speed (new SFFloat (0.1)),
	 gustiness (new SFFloat (0.1)),
	turbulence (new SFFloat ())
{ }

WindPhysicsModel::WindPhysicsModel (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticlePhysicsModelNode (),
	                     fields ()
{
	addField (inputOutput, "metadata",   metadata ());
	addField (inputOutput, "enabled",    enabled ());
	addField (inputOutput, "direction",  direction ());
	addField (inputOutput, "speed",      speed ());
	addField (inputOutput, "gustiness",  gustiness ());
	addField (inputOutput, "turbulence", turbulence ());
}

X3DBaseNode*
WindPhysicsModel::create (X3DExecutionContext* const executionContext) const
{
	return new WindPhysicsModel (executionContext);
}

Vector3f
WindPhysicsModel::getForce (X3DParticleEmitterNode* const emitter) const
{
	float randomSpeed = std::abs (getRandomValue (speed (), gustiness ()));
	float pressure    = std::pow (10, 2 * std::log (randomSpeed)) * 0.64615;

	Vector3f dir = direction () == Vector3f () ? getRandomNormal () : normalize (direction () .getValue ());

	return emitter -> surfaceArea () * pressure * dir;
}

float
WindPhysicsModel::getRandomValue (float value, float variation)
{
	return value + value * variation * ParticleSystem::random1 ();
}

Vector3f
WindPhysicsModel::getRandomNormal ()
{
	float theta = ParticleSystem::random1 () * M_PI;
	float phi   = std::acos (ParticleSystem::random1 ());
	float r     = std::sin (phi);

	return Vector3f (-std::sin (theta) * r,
	                  std::cos (phi),
	                 -std::cos (theta) * r);
}

} // X3D
} // titania
