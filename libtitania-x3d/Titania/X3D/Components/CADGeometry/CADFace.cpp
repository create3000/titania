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

#include "CADFace.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Picking/PickingHierarchyGuard.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   CADFace::component      = Component ("CADGeometry", 2);
const std::string CADFace::typeName       = "CADFace";
const std::string CADFace::containerField = "children";

CADFace::Fields::Fields () :
	shape (new SFNode ())
{ }

CADFace::CADFace (X3DExecutionContext* const executionContext) :
	                 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DProductStructureChildNode (),
	            X3DBoundedObject (),
	                      fields (),
	                   shapeNode ()
{
	addType (X3DConstants::CADFace);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "name",       name ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());
	addField (inputOutput,    "shape",      shape ());

	addChildObjects (shapeNode);
}

X3DBaseNode*
CADFace::create (X3DExecutionContext* const executionContext) const
{
	return new CADFace (executionContext);
}

void
CADFace::initialize ()
{
	X3DProductStructureChildNode::initialize ();
	X3DBoundedObject::initialize ();

	shape () .addInterest (&CADFace::set_shape, this);

	set_shape ();
}

void
CADFace::set_shape ()
{
	try
	{
		if (shapeNode)
		{
			shapeNode -> isCameraObject ()   .removeInterest (&CADFace::setCameraObject,  static_cast <X3DChildNode*> (this));
			shapeNode -> isPickableObject () .removeInterest (&CADFace::isPickableObject, static_cast <X3DChildNode*> (this));
		}

		shapeNode = nullptr;

		if (not shape ())
			return;

		const auto innerNode = shape () -> getInnerNode ();

		for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::LOD:
				case X3DConstants::Transform:
				case X3DConstants::X3DShapeNode:
				{
					shapeNode = dynamic_cast <X3DChildNode*> (innerNode);

					shapeNode -> isCameraObject ()   .addInterest (&CADFace::setCameraObject,  static_cast <X3DChildNode*> (this));
					shapeNode -> isPickableObject () .addInterest (&CADFace::isPickableObject, static_cast <X3DChildNode*> (this));

					setCameraObject   (shapeNode -> isCameraObject ());
					setPickableObject (shapeNode -> isPickableObject ());
					break;
				}
				default:
					continue;
			}

			break;
		}
	}
	catch (const X3DError &)
	{ }
}

Box3d
CADFace::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		const auto boundedObject = x3d_cast <X3DBoundedObject*> (shapeNode);

		if (boundedObject)
			return boundedObject -> getBBox ();

		return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
CADFace::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (not shapeNode)
		return;

	switch (type)
	{
		case TraverseType::POINTER:
		{
			HierarchyGuard guard (renderObject -> getBrowser (), this);
		
			shapeNode -> traverse (type, renderObject);
			break;
		}
		case TraverseType::PICKING:
		{
			PickingHierarchyGuard guard (renderObject -> getBrowser (), this);

			shapeNode -> traverse (type, renderObject);
			break;
		}
		default:
		{
			shapeNode -> traverse (type, renderObject);
			break;
		}
	}
}

void
CADFace::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DProductStructureChildNode::dispose ();
}

} // X3D
} // titania
