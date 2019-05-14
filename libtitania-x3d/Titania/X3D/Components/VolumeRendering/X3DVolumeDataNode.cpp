/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DVolumeDataNode.h"

#include "../CADGeometry/QuadSet.h"
#include "../EnvironmentalSensor/ProximitySensor.h"
#include "../Grouping/Transform.h"
#include "../Rendering/Coordinate.h"
#include "../Shape/Appearance.h"
#include "../Shape/Shape.h"
#include "../Texturing3D/TextureCoordinate3D.h"
#include "../Texturing3D/TextureTransform3D.h"

namespace titania {
namespace X3D {

X3DVolumeDataNode::Fields::Fields () :
	dimensions (new SFVec3f (1, 1, 1))
{ }

X3DVolumeDataNode::X3DVolumeDataNode () :
	         X3DChildNode (),
	     X3DBoundedObject (),
	               fields (),
	  proximitySensorNode (new ProximitySensor (getExecutionContext ())),
	        transformNode (new Transform (getExecutionContext ())),
	            shapeNode (new Shape (getExecutionContext ())),
	       appearanceNode (new Appearance (getExecutionContext ())),
	 textureTransformNode (new TextureTransform3D (getExecutionContext ())),
	         geometryNode (new QuadSet (getExecutionContext ())),
	textureCoordinateNode (new TextureCoordinate3D (getExecutionContext ())),
	       coordinateNode (new Coordinate (getExecutionContext ()))
{
	addType (X3DConstants::X3DVolumeDataNode);

	addChildObjects (proximitySensorNode,
	                 transformNode,
	                 shapeNode,
	                 appearanceNode,
	                 textureTransformNode,
	                 geometryNode,
	                 textureCoordinateNode,
	                 coordinateNode);

	setCameraObject (true);
}

void
X3DVolumeDataNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	dimensions () .addInterest (&X3DVolumeDataNode::set_dimensions, this);

	set_dimensions ();

	appearanceNode -> setPrivate (true);

	proximitySensorNode -> orientation_changed () .addInterest (transformNode -> rotation ());
	proximitySensorNode -> orientation_changed () .addInterest (textureTransformNode -> rotation ());

	proximitySensorNode -> size ()         = Vector3f (-1, -1, -1);
	transformNode -> children ()           = { shapeNode };
	shapeNode -> appearance ()             = appearanceNode;
	shapeNode -> geometry ()               = geometryNode;
	appearanceNode -> textureTransform ()  = textureTransformNode;
	textureTransformNode -> translation () = Vector3f (0.5, 0.5, 0.5);
	textureTransformNode -> center ()      = Vector3f (-0.5, -0.5, -0.5);
	geometryNode -> texCoord ()            = textureCoordinateNode;
	geometryNode -> coord ()               = coordinateNode;

	coordinateNode        -> setup ();
	textureCoordinateNode -> setup ();
	geometryNode          -> setup ();
	textureTransformNode  -> setup ();
	appearanceNode        -> setup ();
	shapeNode             -> setup ();
	transformNode         -> setup ();
	proximitySensorNode   -> setup ();
}

Box3d
X3DVolumeDataNode::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
		return Box3d (dimensions () .getValue (), Vector3d ());

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
X3DVolumeDataNode::set_dimensions ()
{
	static constexpr size_t NUM_PLANES = 200;

	const auto size    = abs (dimensions () .getValue ());
	const auto size1_2 = size / 2;

	coordinateNode -> point () .clear ();

	for (size_t i = 0; i < NUM_PLANES; ++ i)
	{
		const auto z = i / double (NUM_PLANES - 1) - 0.5;

		coordinateNode -> point () .emplace_back ( size1_2,  size1_2, size * z);
		coordinateNode -> point () .emplace_back (-size1_2,  size1_2, size * z);
		coordinateNode -> point () .emplace_back (-size1_2, -size1_2, size * z);
		coordinateNode -> point () .emplace_back ( size1_2, -size1_2, size * z);
	}

	textureCoordinateNode -> point () = coordinateNode -> point ();

	textureTransformNode -> scale () = Vector3f (1, 1, 1) / dimensions () .getValue ();
}

void
X3DVolumeDataNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	proximitySensorNode -> traverse (type, renderObject);
	transformNode       -> traverse (type, renderObject);
}

void
X3DVolumeDataNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

X3DVolumeDataNode::~X3DVolumeDataNode ()
{ }

} // X3D
} // titania
