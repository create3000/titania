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

#include "CollisionCollection.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"
#include "../RigidBodyPhysics/X3DNBodyCollisionSpaceNode.h"

namespace titania {
namespace X3D {

const Component   CollisionCollection::component      = Component ("RigidBodyPhysics", 1);
const std::string CollisionCollection::typeName       = "CollisionCollection";
const std::string CollisionCollection::containerField = "collider";

CollisionCollection::Fields::Fields () :
	                 enabled (new SFBool (true)),
	       appliedParameters (new MFString ({ "BOUNCE" })),
	                  bounce (new SFFloat ()),
	          minBounceSpeed (new SFFloat (0.1)),
	    frictionCoefficients (new SFVec2f ()),
	            surfaceSpeed (new SFVec2f ()),
	             slipFactors (new SFVec2f ()),
	softnessConstantForceMix (new SFFloat (0.0001)),
	 softnessErrorCorrection (new SFFloat (0.8)),
	             collidables (new MFNode ())
{ }

CollisionCollection::CollisionCollection (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	          X3DChildNode (),
	                fields (),
	       collidableNodes (),
	appliedParametersTypes ()
{
	addType (X3DConstants::CollisionCollection);

	addField (inputOutput, "metadata",                 metadata ());
	addField (inputOutput, "enabled",                  enabled ());

	addField (inputOutput, "appliedParameters",        appliedParameters ());

	addField (inputOutput, "bounce",                   bounce ());
	addField (inputOutput, "minBounceSpeed",           minBounceSpeed ());
	addField (inputOutput, "frictionCoefficients",     frictionCoefficients ());
	addField (inputOutput, "surfaceSpeed",             surfaceSpeed ());
	addField (inputOutput, "slipFactors",              slipFactors ());
	addField (inputOutput, "softnessConstantForceMix", softnessConstantForceMix ());
	addField (inputOutput, "softnessErrorCorrection",  softnessErrorCorrection ());

	addField (inputOutput, "collidables",              collidables ());

	addChildObjects (collidableNodes);

	// Units

	minBounceSpeed ()           .setUnit (UnitCategory::SPEED);
	surfaceSpeed ()             .setUnit (UnitCategory::SPEED);
	softnessConstantForceMix () .setUnit (UnitCategory::FORCE);
}

X3DBaseNode*
CollisionCollection::create (X3DExecutionContext* const executionContext) const
{
	return new CollisionCollection (executionContext);
}

void
CollisionCollection::initialize ()
{
	X3DChildNode::initialize ();

	appliedParameters () .addInterest (&CollisionCollection::set_appliedParameters, this);
	collidables ()       .addInterest (&CollisionCollection::set_collidables, this);

	set_appliedParameters ();
	set_collidables ();
}

void
CollisionCollection::set_appliedParameters ()
{
	static const std::map <std::string, AppliedParametersType> appliedParametersIndex = {
		std::pair ("BOUNCE",                 AppliedParametersType::BOUNCE),
		std::pair ("USER_FRICTION",          AppliedParametersType::USER_FRICTION),
		std::pair ("FRICTION_COEFFICIENT-2", AppliedParametersType::FRICTION_COEFFICIENT_2),
		std::pair ("ERROR_REDUCTION",        AppliedParametersType::ERROR_REDUCTION),
		std::pair ("CONSTANT_FORCE",         AppliedParametersType::CONSTANT_FORCE),
		std::pair ("SPEED-1",                AppliedParametersType::SPEED_1),
		std::pair ("SPEED-2",                AppliedParametersType::SPEED_2),
		std::pair ("SLIP-1",                 AppliedParametersType::SLIP_1),
		std::pair ("SLIP-2",                 AppliedParametersType::SLIP_2),
	};

	appliedParametersTypes .clear ();

	for (const auto & value : basic::make_const_range (appliedParameters ()))
	{
		try
		{
			appliedParametersTypes .emplace (appliedParametersIndex .at (value));
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
CollisionCollection::set_collidables ()
{
	// Get collidable nodes.

	collidableNodes .clear ();

	for (const auto & node : collidables ())
	{
		const auto collidableNode = x3d_cast <X3DNBodyCollidableNode*> (node);

		if (collidableNode)
		{
			collidableNodes .emplace_back (collidableNode);
			continue;
		}

		const auto collisionSpaceNode = x3d_cast <X3DNBodyCollisionSpaceNode*> (node);

		if (collisionSpaceNode)
		{
			for (const auto & collidableNode : collisionSpaceNode -> getCollidables ())
				collidableNodes .emplace_back (collidableNode);
		}
	}
}

} // X3D
} // titania
