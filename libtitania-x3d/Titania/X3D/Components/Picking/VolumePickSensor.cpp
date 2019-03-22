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

#include "VolumePickSensor.h"

#include "../../Browser/Picking/VolumePicker.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../RigidBodyPhysics/CollidableShape.h"

namespace titania {
namespace X3D {

const Component   VolumePickSensor::component      = Component ("Picking", 3);
const std::string VolumePickSensor::typeName       = "VolumePickSensor";
const std::string VolumePickSensor::containerField = "children";

VolumePickSensor::VolumePickSensor (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DPickSensorNode (),
	pickingGeometryNode (),
	             picker (new VolumePicker ())
{
	addType (X3DConstants::VolumePickSensor);

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOutput,    "enabled",          enabled ());
	addField (inputOutput,    "objectType",       objectType ());
	addField (initializeOnly, "intersectionType", intersectionType ());
	addField (initializeOnly, "sortOrder",        sortOrder ());
	addField (inputOutput,    "pickingGeometry",  pickingGeometry ());
	addField (inputOutput,    "pickTarget",       pickTarget ());
	addField (outputOnly,     "isActive",         isActive ());
	addField (outputOnly,     "pickedGeometry",   pickedGeometry ());

	addChildObjects (pickingGeometryNode);
}

X3DBaseNode*
VolumePickSensor::create (X3DExecutionContext* const executionContext) const
{
	return new VolumePickSensor (executionContext);
}

void
VolumePickSensor::initialize ()
{
	X3DPickSensorNode::initialize ();

	pickingGeometry () .addInterest (&VolumePickSensor::set_pickingGeometry, this);

	set_pickingGeometry ();
}

void
VolumePickSensor::set_pickingGeometry ()
{
	pickingGeometryNode = x3d_cast <X3DGeometryNode*> (pickingGeometry ());
}

void
VolumePickSensor::process ()
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
					const auto pickingBBox   = pickingGeometryNode -> getBBox () * modelMatrix;
					const auto pickingCenter = pickingBBox .center ();

					for (const auto & target : getTargets ())
					{
						const auto targetBBox = target -> geometryNode -> getBBox () * target -> modelMatrix;
		
						if (not pickingBBox .intersects (targetBBox))
							continue;

						target -> intersected = true;
						target -> distance    = distance (pickingCenter, targetBBox .center ());
					}
				}

				// Send events.

				const auto & pickedGeometries = getPickedGeometries ();
				const auto   active           = not pickedGeometries .empty ();

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () .assign (pickedGeometries .cbegin (), pickedGeometries .cend ());

				break;
			}
			case IntersectionType::GEOMETRY:
			{
				// Intersect geometries.

				for (const auto & modelMatrix : getModelMatrices ())
				{
					const auto   pickingBBox   = pickingGeometryNode -> getBBox () * modelMatrix;
					const auto   pickingCenter = pickingBBox .center ();
					const auto & pickingShape  = getPickShape (pickingGeometryNode);

					picker -> setChildShape1 (modelMatrix, pickingShape -> getCompoundShape ());

					for (const auto & target : getTargets ())
					{
						const auto   targetBBox  = target -> geometryNode -> getBBox () * target -> modelMatrix;
						const auto & targetShape = getPickShape (target -> geometryNode);
	
						picker -> setChildShape2 (target -> modelMatrix, targetShape -> getCompoundShape ());

						if (not picker -> contactTest ())
							continue;

						target -> intersected = true;
						target -> distance    = distance (pickingCenter, targetBBox .center ());
					}
				}

				// Send events.

				const auto & pickedGeometries = getPickedGeometries ();
				const auto   active           = not pickedGeometries .empty ();

				if (active not_eq isActive ())
					isActive () = active;

				if (not (pickedGeometry () .equals (pickedGeometries)))
					pickedGeometry () .assign (pickedGeometries .cbegin (), pickedGeometries .cend ());

				break;
			}
		}
	}

	X3DPickSensorNode::process ();
}

VolumePickSensor::~VolumePickSensor ()
{ }

} // X3D
} // titania
