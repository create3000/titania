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

#include "PointPickSensor.h"

#include "../../Browser/Picking/VolumePicker.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/PointSet.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../RigidBodyPhysics/CollidableShape.h"

namespace titania {
namespace X3D {

const Component   PointPickSensor::component      = Component ("Picking", 1);
const std::string PointPickSensor::typeName       = "PointPickSensor";
const std::string PointPickSensor::containerField = "children";

PointPickSensor::Fields::Fields () :
	pickedPoint (new MFVec3f ())
{ }

PointPickSensor::PointPickSensor (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DPickSensorNode (),
	             fields (),
	pickingGeometryNode (),
	             picker (new VolumePicker ()),
	     compoundShapes ()
{
	addType (X3DConstants::PointPickSensor);

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOutput,    "enabled",          enabled ());
	addField (inputOutput,    "objectType",       objectType ());
	addField (initializeOnly, "intersectionType", intersectionType ());
	addField (initializeOnly, "sortOrder",        sortOrder ());
	addField (inputOutput,    "pickingGeometry",  pickingGeometry ());
	addField (inputOutput,    "pickTarget",       pickTarget ());
	addField (outputOnly,     "isActive",         isActive ());
	addField (outputOnly,     "pickedPoint",      pickedPoint ());
	addField (outputOnly,     "pickedGeometry",   pickedGeometry ());

	addChildObjects (pickingGeometryNode);
}

X3DBaseNode*
PointPickSensor::create (X3DExecutionContext* const executionContext) const
{
	return new PointPickSensor (executionContext);
}

void
PointPickSensor::initialize ()
{
	X3DPickSensorNode::initialize ();

	pickingGeometry () .addInterest (&PointPickSensor::set_pickingGeometry, this);

	set_pickingGeometry ();
}

void
PointPickSensor::set_pickingGeometry ()
{
	if (pickingGeometryNode)
		pickingGeometryNode -> rebuilded () .removeInterest (&PointPickSensor::set_geometry, this);

	pickingGeometryNode = x3d_cast <PointSet*> (pickingGeometry ());

	if (pickingGeometryNode)
		pickingGeometryNode -> rebuilded () .addInterest (&PointPickSensor::set_geometry, this);

	set_geometry ();
}

void
PointPickSensor::set_geometry ()
{
	compoundShapes .resize (0);

	if (not pickingGeometryNode)
		return;

	const auto & coord = pickingGeometryNode -> getCoord ();

	if (not coord)
		return;

	for (size_t i = 0, size = coord -> getSize (); i < size; ++ i)
	{
		const auto compoundShape = std::make_shared <btCompoundShape> ();
		const auto sphereShape   = std::make_shared <btSphereShape> (1e-5);
		const auto point         = coord -> get1Point (i);

		auto t = btTransform ();

		t .setOrigin (btVector3 (point .x (), point .y (), point .z ()));

		compoundShape -> addChildShape (t, sphereShape. get ());

		compoundShapes .emplace_back (compoundShape, sphereShape, point);
	}
}

void
PointPickSensor::process ()
{
	if (pickingGeometryNode)
	{
		switch (getIntersectionType ())
		{
			case IntersectionType::BOUNDS:
			{
				// Intersect bboxes.

				for (const auto & modelMatrix : getModelMatrices ())
				{
					const auto pickingBBox = pickingGeometryNode -> getBBox () * modelMatrix;

					for (const auto & target : getTargets ())
					{
						const auto targetBBox = target -> geometryNode -> getBBox () * target -> modelMatrix;
		
						if (not pickingBBox .intersects (targetBBox))
							continue;

						target -> intersected = true;
						target -> distance    = distance (pickingBBox .center (), targetBBox .center ());
					}
				}

				// Send events.

				auto &     pickedGeometries = getPickedGeometries ();
				const auto active           = not pickedGeometries .empty ();

				pickedGeometries .remove (nullptr);

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () = pickedGeometries;

				break;
			}
			case IntersectionType::GEOMETRY:
			{
				// Intersect geometries.

				MFVec3f pickedPoints;

				for (const auto & modelMatrix : getModelMatrices ())
				{
					const auto pickingBBox = pickingGeometryNode -> getBBox () * modelMatrix;

					auto translation = Vector3f ();
					auto rotation    = Rotation4f ();
					auto scale       = Vector3f ();

					modelMatrix .get (translation, rotation, scale);

					const auto transform    = picker -> getTransform (translation, rotation);
					const auto localScaling = btVector3 (scale .x (), scale .y (), scale .z ());

					for (const auto & [compoundShape, sphereShape, point] : compoundShapes)
					{
						picker -> setChildShape1 (transform, localScaling, compoundShape);

						for (const auto & target : getTargets ())
						{
							const auto   targetBBox  = target -> geometryNode -> getBBox () * target -> modelMatrix;
							const auto & targetShape = getPickShape (target -> geometryNode);
		
							picker -> setChildShape2 (target -> modelMatrix, targetShape -> getCompoundShape ());
	
							if (not picker -> contactTest ())
								continue;
	
							target -> intersected = true;
							target -> distance    = distance (pickingBBox .center (), targetBBox .center ());

							pickedPoints .emplace_back (point);
						}
					}
				}

				// Send events.

				auto &     pickedGeometries = getPickedGeometries ();
				const auto active           = not pickedGeometries .empty ();

				pickedGeometries .remove (nullptr);

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () = pickedGeometries;

				if (not (pickedPoint () .equals (pickedPoints)))
					pickedPoint () = pickedPoints;

				break;
			}
		}
	}

	X3DPickSensorNode::process ();
}

PointPickSensor::~PointPickSensor ()
{ }

} // X3D
} // titania
