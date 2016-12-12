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

#include "CollisionSensor.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/ParticleSystems/BVH.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/CollisionCollection.h"
#include "../RigidBodyPhysics/Contact.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"

namespace titania {
namespace X3D {

const ComponentType CollisionSensor::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   CollisionSensor::typeName       = "CollisionSensor";
const std::string   CollisionSensor::containerField = "children";

CollisionSensor::Fields::Fields () :
	     collider (new SFNode ()),
	intersections (new MFNode ()),
	     contacts (new MFNode ())
{ }

CollisionSensor::CollisionSensor (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSensorNode (),
	       fields (),
	 colliderNode ()
{
	addType (X3DConstants::CollisionSensor);

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOutput, "enabled",       enabled ());
	addField (outputOnly,  "isActive",      isActive ());
	addField (inputOutput, "collider",      collider ());
	addField (outputOnly,  "intersections", intersections ());
	addField (outputOnly,  "contacts",      contacts ());

	addField (VRML_V2_0, "collidables", "collider");

	addChildren (colliderNode);
}

X3DBaseNode*
CollisionSensor::create (X3DExecutionContext* const executionContext) const
{
	return new CollisionSensor (executionContext);
}

void
CollisionSensor::initialize ()
{
	X3DSensorNode::initialize ();

	getBrowser () -> finished () .addInterest (this, &CollisionSensor::update);

	collider () .addInterest (this, &CollisionSensor::set_collider);

	set_collider ();
}

void
CollisionSensor::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> finished () .removeInterest (this, &CollisionSensor::update);

	X3DSensorNode::setExecutionContext (executionContext);

	getBrowser () -> finished () .addInterest (this, &CollisionSensor::update);

	set_collider ();
}

void
CollisionSensor::set_collider ()
{
	colliderNode = x3d_cast <CollisionCollection*> (collider ());

	if (not colliderNode)
		colliderNode = getBrowser () -> getDefaultCollisionCollection ();
}

void
CollisionSensor::update ()
{
	if (not colliderNode -> enabled ())
		return;

	std::vector <Vector3d>               triangles;
	X3DPtrArray <Contact>                contactNodes;
	X3DPtrArray <X3DNBodyCollidableNode> intersectionNodes;

	for (const auto & collidableNode1 : colliderNode -> getCollidables ())
	{
		try
		{
			const auto & collidableGeometry1 = collidableNode1 -> getCollidableGeometry ();
			const auto & matrix1             = collidableGeometry1 .matrix;
	
			for (const auto & collidableNode2 : colliderNode -> getCollidables ())
			{
				try
				{
					if (collidableNode1 == collidableNode2)
						continue;

					triangles .clear ();

					const auto & collidableGeometry2 = collidableNode2 -> getCollidableGeometry ();
					const auto & matrix2             = collidableGeometry2 .matrix;
					const auto   matrix              = matrix1 * inverse (matrix2);

					const bool intersects = collidableGeometry1 .bvh -> intersects (collidableGeometry2 .bbox,
					                                                                collidableGeometry2 .points,
					                                                                collidableGeometry2 .edges,
					                                                                collidableGeometry2 .normals,
					                                                                matrix,
					                                                                triangles);

					if (intersects)
					{
						//__LOG__ << SFTime (chrono::now ()) << " : " << triangles .size () << " : " << collidableGeometry1 .bbox << " : " << triangles [0] << " : " << triangles [1] << " : " << triangles [2] << std::endl;

						Vector3d contactPosition;
						Vector3d contactNormal;
						double   contactDepth = 0;
						Box3d    contactBox (triangles .begin (), triangles .end (), iterator_type ());

						for (size_t i = 0, size = triangles .size (); i < size; i += 3)
						{
							contactPosition += triangles [i];
							contactPosition += triangles [i + 1];
							contactPosition += triangles [i + 2];

							contactNormal += normal (triangles [i], triangles [i + 1], triangles [i + 2]);
						}

						contactPosition /= triangles .size ();
						contactNormal   /= triangles .size () / 3;
						contactDepth     = abs (contactBox .size ());

						// Create Contact node.

						X3DPtr <Contact> contactNode = new Contact (getExecutionContext ());

						contactNode -> position ()                 = contactPosition;
						contactNode -> contactNormal ()            = contactNormal;
						contactNode -> depth ()                    = contactDepth;
						contactNode -> appliedParameters ()        = colliderNode -> appliedParameters ();
						contactNode -> bounce ()                   = colliderNode -> bounce ();
						contactNode -> minBounceSpeed ()           = colliderNode -> minBounceSpeed ();
						//contactNode -> frictionDirection ()        = ; // TODO
						contactNode -> frictionCoefficients ()     = colliderNode -> frictionCoefficients ();
						contactNode -> surfaceSpeed ()             = colliderNode -> surfaceSpeed ();
						contactNode -> slipCoefficients ()         = colliderNode -> slipFactors ();              // XXX: coefficients <-> factors???
						contactNode -> softnessConstantForceMix () = colliderNode -> softnessConstantForceMix ();
						contactNode -> softnessErrorCorrection ()  = colliderNode -> softnessErrorCorrection ();
						contactNode -> geometry1 ()                = collidableNode1;
						contactNode -> geometry2 ()                = collidableNode2;
						contactNode -> body1 ()                    = collidableNode1 -> getBody ();
						contactNode -> body2 ()                    = collidableNode2 -> getBody ();

						contactNode -> setup ();

						contactNodes      .emplace_back (std::move (contactNode));
						intersectionNodes .emplace_back (collidableNode1);
					}
				}
				catch (const X3DError &)
				{ }
				catch (const std::domain_error &)
				{ }
			}
		}
		catch (const X3DError &)
		{ }
	}

	// Send events

	if (not contactNodes .empty ())
		contacts () = std::move (contactNodes);

	if (not intersectionNodes .empty ())
		intersections () = std::move (intersectionNodes);
}

} // X3D
} // titania
