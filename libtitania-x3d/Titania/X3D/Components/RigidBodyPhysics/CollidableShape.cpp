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

#include "../../Browser/Core/Cast.h"
#include "../../Browser/ParticleSystems/BVH.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Shape/Shape.h"
#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

const ComponentType CollidableShape::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   CollidableShape::typeName       = "CollidableShape";
const std::string   CollidableShape::containerField = "children";

CollidableShape::Fields::Fields () :
	shape (new SFNode ())
{ }

CollidableShape::CollidableShape (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNBodyCollidableNode (),
	                    fields (),
	                 shapeNode (),
	              geometryNode (),
	        collidableGeometry ()
{
	addType (X3DConstants::CollidableShape);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "enabled",     enabled ());
	addField (inputOutput,    "translation", translation ());
	addField (inputOutput,    "rotation",    rotation ());
	addField (initializeOnly, "bboxSize",    bboxSize ());
	addField (initializeOnly, "bboxCenter",  bboxCenter ());
	addField (initializeOnly, "shape",       shape ());

	addChildren (shapeNode, geometryNode);
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

	shape () .addInterest (this, &CollidableShape::set_shape);
	addInterest (this, &CollidableShape::eventsProcessed);

	set_shape ();
	eventsProcessed ();
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

const CollidableGeometry &
CollidableShape::getCollidableGeometry () const
throw (Error <INVALID_NODE>)
{
	if (geometryNode)
	{
		collidableGeometry .matrix = getMatrix ();

		return collidableGeometry;
	}

	throw Error <INVALID_NODE> ("CollidableShape::getCollidableGeometry");
}

void
CollidableShape::set_shape ()
{
	if (shapeNode)
	{
		shapeNode -> isCameraObject () .removeInterest (const_cast <SFBool &> (isCameraObject ()));
		shapeNode -> geometry () .removeInterest (this, &CollidableShape::set_geometry);
	}

	shapeNode .set (x3d_cast <Shape*> (shape ()));

	if (shapeNode)
	{
		shapeNode -> isCameraObject () .addInterest (const_cast <SFBool &> (isCameraObject ()));
		shapeNode -> geometry () .addInterest (this, &CollidableShape::set_geometry);

		setCameraObject (shapeNode -> isCameraObject ());
	}
	else
		setCameraObject (false);

	set_geometry ();
}

void
CollidableShape::set_geometry ()
{
	if (geometryNode)
		geometryNode -> removeInterest (this, &CollidableShape::set_collidableGeometry);

	if (shapeNode)
		geometryNode = shapeNode -> getGeometry ();

	if (geometryNode)
		geometryNode -> addInterest (this, &CollidableShape::set_collidableGeometry);

	set_collidableGeometry ();
}

void
CollidableShape::set_collidableGeometry ()
{
	if (geometryNode)
	{
		collidableGeometry .points  .clear ();
		collidableGeometry .edges   .clear ();
		collidableGeometry .normals .clear ();

		// Triangulate geometry

		std::vector <Color4f>  colors;
		TexCoordArray          texCoords;
		std::vector <Vector3f> normals;
		std::vector <Vector3d> vertices;

		geometryNode -> triangulate (colors, texCoords, normals, vertices);

		// BBox

		collidableGeometry .bbox = geometryNode -> getBBox ();

		// Points

		auto & points = collidableGeometry .points;

		points = vertices;

		std::sort (points .begin (), points .end ());
		points .erase (std::unique (points .begin (), points .end ()), points .end ());

		// Edges

		auto & edges = collidableGeometry .edges;

		for (size_t i = 0, size = vertices .size (); i < size; i += 3)
		{
			edges .emplace_back (vertices [i + 0] - vertices [i + 1]);
			edges .emplace_back (vertices [i + 1] - vertices [i + 2]);
			edges .emplace_back (vertices [i + 2] - vertices [i + 0]);
		}

		std::sort (edges .begin (), edges .end ());
		edges .erase (std::unique (edges .begin (), edges .end ()), edges .end ());
		
		// Face normals

		auto & faceNormals = collidableGeometry .normals;

		for (size_t i = 0, size = vertices .size (); i < size; i += 3)
		{
			faceNormals .emplace_back (normal (vertices [i], vertices [i + 1], vertices [i + 2]));
		}

		// Create BVH

		collidableGeometry .bvh .reset (new BVH <double> (std::move (vertices)));
	}
}

void
CollidableShape::eventsProcessed ()
{
	setMatrix (translation () .getValue (), rotation () .getValue ());
}

void
CollidableShape::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (shapeNode)
	{
		renderObject -> getModelViewMatrix () .push ();
		renderObject -> getModelViewMatrix () .mult_left (getMatrix ());

		shapeNode -> traverse (type, renderObject);

		renderObject -> getModelViewMatrix () .pop ();
	}
}

CollidableShape::~CollidableShape ()
{ }

} // X3D
} // titania
