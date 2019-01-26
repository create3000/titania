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

#include "WindPhysicsModel.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Browser/ParticleSystems/Random.h"

namespace titania {
namespace X3D {

const Component   WindPhysicsModel::component      = Component ("ParticleSystems", 1);
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
	addType (X3DConstants::WindPhysicsModel);

	addField (inputOutput, "metadata",   metadata ());
	addField (inputOutput, "enabled",    enabled ());
	addField (inputOutput, "direction",  direction ());
	addField (inputOutput, "speed",      speed ());
	addField (inputOutput, "gustiness",  gustiness ());
	addField (inputOutput, "turbulence", turbulence ());

	speed () .setUnit (UnitCategory::SPEED);
}

X3DBaseNode*
WindPhysicsModel::create (X3DExecutionContext* const executionContext) const
{
	return new WindPhysicsModel (executionContext);
}

float
WindPhysicsModel::getRandomSpeed () const
{
	const float s           = std::max <float> (0, speed ());
	const float variation   = s * std::max <float> (0, gustiness ());

	return random1 (std::max (0.0f, s - variation), s + variation);
}

void
WindPhysicsModel::addForce (X3DParticleEmitterNode* const emitter, MFVec3f & force, MFFloat & turbulence) const
{
	if (enabled ())
	{
		const float randomSpeed = getRandomSpeed ();
		const float pressure    = std::pow (10, 2 * std::log (randomSpeed)) * 0.64615;

		const Vector3f normal = direction () == Vector3f () ? random_normal () : normalize (direction () .getValue ());

		force      .emplace_back (emitter -> surfaceArea () * pressure * normal);
		turbulence .emplace_back (pi <float> * std::clamp <float> (this -> turbulence (), 0, 1));
	}
}

} // X3D
} // titania
