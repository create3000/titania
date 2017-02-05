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

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"
#include "../RigidBodyPhysics/X3DNBodyCollisionSpaceNode.h"

namespace titania {
namespace X3D {

const ComponentType CollisionCollection::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   CollisionCollection::typeName       = "CollisionCollection";
const std::string   CollisionCollection::containerField = "collider";

CollisionCollection::Fields::Fields () :
	                 enabled (new SFBool ()),
	       appliedParameters (new MFString ({ "BOUNCE" })),
	                  bounce (new SFFloat ()),
	          minBounceSpeed (new SFFloat ()),
	    frictionCoefficients (new SFVec2f ()),
	            surfaceSpeed (new SFVec2f ()),
	             slipFactors (new SFVec2f ()),
	softnessConstantForceMix (new SFFloat ()),
	 softnessErrorCorrection (new SFFloat ()),
	             collidables (new MFNode ())
{ }

CollisionCollection::CollisionCollection (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      X3DChildNode (),
	            fields (),
	   collidableNodes (),
	collidableNodesSet ()
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

	collidables () .addInterest (&CollisionCollection::set_collidables, this);

	set_collidables ();
}

void
CollisionCollection::set_collidables ()
{
	std::vector <X3DNBodyCollidableNode*> value;

	for (const auto & node : collidables ())
	{
		const auto collidableNode = x3d_cast <X3DNBodyCollidableNode*> (node);
		
		if (collidableNode)
			value .emplace_back (collidableNode);

		//const auto collisionSpaceNode = x3d_cast <X3DNBodyCollisionSpaceNode*> (node);
		
		//if (collisionSpaceNode)
		//	value .emplace_back (collisionSpaceNode);
	}

	collidableNodes .set (value .begin (), value .end ());

	collidableNodesSet .clear ();
	collidableNodesSet .insert (collidableNodes .begin (), collidableNodes .end ());
}

} // X3D
} // titania
