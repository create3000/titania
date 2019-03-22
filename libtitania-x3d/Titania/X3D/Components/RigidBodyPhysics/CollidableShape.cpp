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

#include "CollidableShape.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Picking/PickingHierarchyGuard.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../Geometry2D/Rectangle2D.h"
#include "../Geometry3D/Box.h"
#include "../Geometry3D/Cone.h"
#include "../Geometry3D/Cylinder.h"
#include "../Geometry3D/ElevationGrid.h"
#include "../Geometry3D/Sphere.h"
#include "../Shape/Shape.h"
#include "../Rendering/X3DGeometryNode.h"

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace titania {
namespace X3D {

const Component   CollidableShape::component      = Component ("RigidBodyPhysics", 1);
const std::string CollidableShape::typeName       = "CollidableShape";
const std::string CollidableShape::containerField = "children";

CollidableShape::Fields::Fields () :
	shape (new SFNode ())
{ }

CollidableShape::CollidableShape (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNBodyCollidableNode (),
	                    fields (),
	                    convex (false),
	                 shapeNode (),
	              geometryNode (),
	            collisionShape (),
	              triangleMesh (),
	               heightField ()
{
	addType (X3DConstants::CollidableShape);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "enabled",     enabled ());
	addField (inputOutput,    "translation", translation ());
	addField (inputOutput,    "rotation",    rotation ());
	addField (initializeOnly, "bboxSize",    bboxSize ());
	addField (initializeOnly, "bboxCenter",  bboxCenter ());
	addField (initializeOnly, "shape",       shape ());

	addChildObjects (convex, shapeNode, geometryNode);
}

X3DBaseNode*
CollidableShape::create (X3DExecutionContext* const executionContext) const
{
	return new CollidableShape (executionContext);
}

void
CollidableShape::initialize ()
{
	X3DNBodyCollidableNode::initialize ();

	enabled () .addInterest (&CollidableShape::set_collidableGeometry, this);
	shape ()   .addInterest (&CollidableShape::set_shape,              this);

	set_shape ();
}

Box3d
CollidableShape::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		const auto boundedObject = x3d_cast <X3DBoundedObject*> (shapeNode);

		if (boundedObject)
			return boundedObject -> getBBox () * getMatrix ();

		return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ()) * getMatrix ();
}

std::shared_ptr <btCollisionShape>
CollidableShape::createConvexGeometry ()
{
	const auto convexHull = std::make_shared <btConvexHullShape> ();

	for (auto & vertex : geometryNode -> getPolygonVertices ())
		convexHull -> addPoint (btVector3 (vertex .x (), vertex .y (), vertex .z ()));

	return convexHull;
}

std::shared_ptr <btCollisionShape>
CollidableShape::createConcaveGeometry ()
{
	// Triangulate.

	std::vector <Vector3d> vertices;

	geometryNode -> triangulate (nullptr, nullptr, nullptr, nullptr, &vertices);

	// Create Bullet shape.

	triangleMesh = std::make_shared <btTriangleMesh> ();

	for (size_t i = 0, size = vertices .size (); i < size; i += 3)
	{
		const auto & v0 = vertices [i + 0];
		const auto & v1 = vertices [i + 1];
		const auto & v2 = vertices [i + 2];

		triangleMesh -> addTriangle (btVector3 (v0 .x (), v0 .y (), v0 .z ()),
		                             btVector3 (v1 .x (), v1 .y (), v1 .z ()),
		                             btVector3 (v2 .x (), v2 .y (), v2 .z ()));
	}

	if (vertices .empty ())
		return nullptr;

	return std::make_shared <btBvhTriangleMeshShape> (triangleMesh .get (), false);
}

void
CollidableShape::set_shape ()
{
	if (shapeNode)
	{
		shapeNode -> isCameraObject ()   .removeInterest (const_cast <SFBool &> (isCameraObject ()));
		shapeNode -> isPickableObject () .removeInterest (const_cast <SFBool &> (isPickableObject ()));
		shapeNode -> geometry ()         .removeInterest (&CollidableShape::set_geometry, this);
	}

	shapeNode .set (x3d_cast <Shape*> (shape ()));

	if (shapeNode)
	{
		shapeNode -> isCameraObject ()   .addInterest (const_cast <SFBool &> (isCameraObject ()));
		shapeNode -> isPickableObject () .addInterest (const_cast <SFBool &> (isPickableObject ()));
		shapeNode -> geometry ()         .addInterest (&CollidableShape::set_geometry, this);

		setCameraObject   (shapeNode -> isCameraObject ());
		setPickableObject (shapeNode -> isPickableObject ());
	}
	else
	{
		setCameraObject   (false);
		setPickableObject (false);
	}

	set_geometry ();
}

void
CollidableShape::set_geometry ()
{
	if (geometryNode)
		geometryNode -> rebuilded () .removeInterest (&CollidableShape::set_collidableGeometry, this);

	if (shapeNode)
		geometryNode .set (shapeNode -> getGeometry ());
	else
		geometryNode .set (nullptr);

	if (geometryNode)
		geometryNode -> rebuilded () .addInterest (&CollidableShape::set_collidableGeometry, this);

	set_collidableGeometry ();
}

void
CollidableShape::set_collidableGeometry ()
{
	if (collisionShape)
		getCompoundShape () -> removeChildShape (collisionShape .get ());

	triangleMesh .reset ();
	heightField .clear ();
	heightField .shrink_to_fit ();

	setOffset (Vector3f ());

	if (enabled () and geometryNode and geometryNode -> getGeometryType () > 1)
	{
		switch (geometryNode -> getType () .back ())
		{
			case X3DConstants::Rectangle2D:
			{
//				const auto rectangle = dynamic_cast <Rectangle2D*> (geometryNode .getValue ());
//				const auto half      = rectangle -> size () * 0.5f;
//
//				collisionShape .reset (new btBox2dShape (btVector3 (half .x (), half .y (), 0)));
//				break;

				const auto rectangle = dynamic_cast <Rectangle2D*> (geometryNode .getValue ());
				const auto size      = rectangle -> size () .getValue ();

				heightField = { 0, 0, 0, 0 };

				collisionShape .reset (new btHeightfieldTerrainShape (2, 2, heightField .data (), 1, 0, 0, 2, PHY_FLOAT, false));

				collisionShape -> setLocalScaling (btVector3 (size .x (), size .y (), 1));
				break;
			}
			case X3DConstants::Box:
			{
				const auto box  = dynamic_cast <Box*> (geometryNode .getValue ());
				const auto half = box -> size () * 0.5f;

				collisionShape .reset (new btBoxShape (btVector3 (half .x (), half .y (), half .z ())));
				break;
			}
			case X3DConstants::Cone:
			{
				const auto cone = dynamic_cast <Cone*> (geometryNode .getValue ());

				if (cone -> side () and cone -> bottom ())
					collisionShape .reset (new btConeShape (cone -> bottomRadius (), cone -> height ()));
				else
					collisionShape = createConcaveGeometry ();

				break;
			}
			case X3DConstants::Cylinder:
			{
				const auto cylinder  = dynamic_cast <Cylinder*> (geometryNode .getValue ());
				const auto radius    = cylinder -> radius ();
				const auto height1_2 = cylinder -> height () * 0.5f;

				if (cylinder -> side () and cylinder -> top () and cylinder -> bottom ())
					collisionShape .reset (new btCylinderShape (btVector3 (radius, height1_2, radius)));
				else
					collisionShape = createConcaveGeometry ();

				break;
			}
			case X3DConstants::ElevationGrid:
			{
				const auto elevationGrid = dynamic_cast <ElevationGrid*> (geometryNode .getValue ());

				if (elevationGrid -> xDimension () > 1 and elevationGrid -> zDimension () > 1)
				{
					const auto minmax = std::minmax_element (elevationGrid -> height () .begin (), elevationGrid -> height () .end ());
					const auto min    = minmax .first  not_eq elevationGrid -> height () .end () ? *minmax .first  : 0.0f;
					const auto max    = minmax .second not_eq elevationGrid -> height () .end () ? *minmax .second : 0.0f;

					heightField .assign (elevationGrid -> height () .begin (), elevationGrid -> height () .end ());
					heightField .resize (elevationGrid -> xDimension () * elevationGrid -> zDimension ());

					collisionShape .reset (new btHeightfieldTerrainShape (elevationGrid -> xDimension (),
					                                                      elevationGrid -> zDimension (),
					                                                      heightField .data (),
					                                                      1,
					                                                      min,
					                                                      max,
					                                                      1,
					                                                      PHY_FLOAT,
					                                                      true));
	
					collisionShape -> setLocalScaling (btVector3 (elevationGrid -> xSpacing (), 1, elevationGrid -> zSpacing ()));

					setOffset (Vector3f (elevationGrid -> xSpacing () * (elevationGrid -> xDimension () - 1) * 0.5f,
					                     (min + max) * 0.5f,
					                     elevationGrid -> zSpacing () * (elevationGrid -> zDimension () - 1) * 0.5f));
				}
				else
				{
					collisionShape .reset ();	
				}

				break;
			}
			case X3DConstants::Sphere:
			{
				const auto sphere = dynamic_cast <Sphere*> (geometryNode .getValue ());

				collisionShape .reset (new btSphereShape (sphere -> radius ()));
				break;
			}
			default:
			{
				if (convex)
					collisionShape = createConvexGeometry ();
				else
					collisionShape = createConcaveGeometry ();

				break;
			}
		}
	}
	else
	{
		collisionShape .reset ();	
	}

	if (collisionShape)
		getCompoundShape () -> addChildShape (getLocalTransform (), collisionShape .get ());

	addEvent ();
}

void
CollidableShape::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (not shapeNode)
		return;

	switch (type)
	{
		case TraverseType::POINTER:
		{
			HierarchyGuard guard (renderObject -> getBrowser (), this);

			renderObject -> getModelViewMatrix () .push ();
			renderObject -> getModelViewMatrix () .mult_left (getMatrix ());
	
			shapeNode -> traverse (type, renderObject);
		
			renderObject -> getModelViewMatrix () .pop ();
			break;
		}
		case TraverseType::PICKING:
		{
			PickingHierarchyGuard guard (renderObject -> getBrowser (), this);

			renderObject -> getModelViewMatrix () .push ();
			renderObject -> getModelViewMatrix () .mult_left (getMatrix ());
	
			shapeNode -> traverse (type, renderObject);
		
			renderObject -> getModelViewMatrix () .pop ();
			break;
		}
		default:
		{
			renderObject -> getModelViewMatrix () .push ();
			renderObject -> getModelViewMatrix () .mult_left (getMatrix ());
	
			shapeNode -> traverse (type, renderObject);
		
			renderObject -> getModelViewMatrix () .pop ();
			break;
		}
	}

}

CollidableShape::~CollidableShape ()
{ }

} // X3D
} // titania
