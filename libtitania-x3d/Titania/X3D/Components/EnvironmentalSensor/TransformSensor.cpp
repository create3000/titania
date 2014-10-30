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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "TransformSensor.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Tools/EnvironmentalSensor/TransformSensorTool.h"
#include "../Grouping/X3DBoundedObject.h"

#include <Titania/Math/Algorithms/SAT.h>
#include <array>

namespace titania {
namespace math {

template <class Type>
std::vector <vector3 <Type>>
points (const box3 <Type> & self)
{
	std::vector <vector3 <Type>>  points;
	points .reserve (8);

	const auto x = self .matrix () .x ();
	const auto y = self .matrix () .y ();
	const auto z = self .matrix () .z ();

	const auto r1 = y + z;
	const auto r2 = z - y;

	points .emplace_back (x + r1);
	points .emplace_back (x + r2);
	points .emplace_back (r1 - x);
	points .emplace_back (r2 - x);

	points .emplace_back (-points [0]);
	points .emplace_back (-points [1]);
	points .emplace_back (-points [2]);
	points .emplace_back (-points [3]);

	for (auto & point : points)
		point += self .center ();

	return points;
}

template <class Type>
std::vector <vector3 <Type>>
planes (const box3 <Type> & self)
{
	// The sat axes for the box are the normal vectors of its faces. It is not needed to normalize these axes.

	std::vector <vector3 <Type>>  axes;
	axes .reserve (3);

	const auto x = self .matrix () .x ();
	const auto y = self .matrix () .y ();
	const auto z = self .matrix () .z ();

	axes .emplace_back (cross (y, z));
	axes .emplace_back (cross (z, x));
	axes .emplace_back (cross (x, y));

	return axes;
}

template <class Type>
std::array <vector3 <Type>, 3>
axes (const box3 <Type> & self)
{
	return std::array <vector3 <Type>, 3> {
		self .matrix () .x (),
		self .matrix () .y (),
		self .matrix () .z ()
	};
}

template <class Type>
bool
intersect (const box3 <Type> & self, const box3 <Type> & other)
{
	// http://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection

	const std::vector <vector3 <Type>>  points1 = points (self);
	const std::vector <vector3 <Type>>  points2 = points (other);

	// Test the three planes spanned by the normal vectors of the faces of the first parallelepiped.

	if (not sat::overlaps (planes (self), points1, points2))
		return false;

	// Test the three planes spanned by the normal vectors of the faces of the second parallelepiped.

	if (not sat::overlaps (planes (other), points1, points2))
		return false;

	// Test the nine other planes spanned by the edges of each parallelepiped.

	std::vector <vector3 <Type>>  axes9;
	
	for (const auto & axis1 : axes (self))
	{
		for (const auto & axis2 : axes (other))
			axes9 .emplace_back (cross (axis1, axis2));
	}

	if (not sat::overlaps (axes9, points1, points2))
		return false;

	return true;
}

} // math
} // titania

namespace titania {
namespace X3D {

const ComponentType TransformSensor::component      = ComponentType::ENVIRONMENTAL_SENSOR;
const std::string   TransformSensor::typeName       = "TransformSensor";
const std::string   TransformSensor::containerField = "children";

TransformSensor::Fields::Fields () :
	   position_changed (new SFVec3f ()),
	orientation_changed (new SFRotation ()),
	       targetObject (new SFNode ())
{ }

TransformSensor::TransformSensor (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentalSensorNode (),
	                    fields (),
	          targetObjectNode (),
	                   visible (false)
{
	addType (X3DConstants::TransformSensor);

	addField (inputOutput, "metadata",            metadata ());
	addField (inputOutput, "enabled",             enabled ());
	addField (inputOutput, "size",                size ());
	addField (inputOutput, "center",              center ());
	addField (outputOnly,  "enterTime",           enterTime ());
	addField (outputOnly,  "exitTime",            exitTime ());
	addField (outputOnly,  "isActive",            isActive ());
	addField (outputOnly,  "position_changed",    position_changed ());
	addField (outputOnly,  "orientation_changed", orientation_changed ());
	addField (inputOutput, "targetObject",        targetObject ());
}

X3DBaseNode*
TransformSensor::create (X3DExecutionContext* const executionContext) const
{
	return new TransformSensor (executionContext);
}

void
TransformSensor::initialize ()
{
	X3DEnvironmentalSensorNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (this, &TransformSensor::set_enabled);
	isLive () .addInterest (this, &TransformSensor::set_enabled);

	enabled ()      .addInterest (this, &TransformSensor::set_enabled);
	size ()         .addInterest (this, &TransformSensor::set_enabled);
	targetObject () .addInterest (this, &TransformSensor::set_targetObject);

	set_targetObject ();
}

void
TransformSensor::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> sensors ()         .removeInterest (this, &TransformSensor::update);
	getExecutionContext () -> isLive () .removeInterest (this, &TransformSensor::set_enabled);

	X3DEnvironmentalSensorNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &TransformSensor::set_enabled);

		set_enabled ();
	}
}

void
TransformSensor::set_enabled ()
{
	if (enabled () and isLive () and getExecutionContext () -> isLive () and targetObjectNode and size () not_eq Vector3f ())
		getBrowser () -> sensors () .addInterest (this, &TransformSensor::update);

	else
		getBrowser () -> sensors () .removeInterest (this, &TransformSensor::update);
}

void
TransformSensor::set_targetObject ()
{
	targetObjectNode .set (x3d_cast <X3DBoundedObject*> (targetObject ()));

	set_enabled ();
}

void
TransformSensor::update ()
{
	if (visible)
	{
		const auto sourceBBox = Box3f (size (), center ());
		const auto targetBBox = targetObjectNode -> getBBox ();

		if (size () == Vector3f (-1, -1, -1) or math::intersect (sourceBBox, targetBBox))
		{
			if (not isActive ())
			{
				isActive ()  = true;
				enterTime () = getCurrentTime ();
			}

			Vector3f   translation;
			Rotation4f rotation;

			targetBBox .matrix () .get (translation, rotation);

			if (translation not_eq position_changed ())
				position_changed () = translation;

			if (rotation not_eq orientation_changed ())
				orientation_changed () = rotation;
		}
		else
		{
			if (isActive ())
			{
				isActive () = false;
				exitTime () = getCurrentTime ();
			}
		}

		visible = false;
	}
	else
	{
		if (isActive ())
		{
			isActive () = false;
			exitTime () = getCurrentTime ();
		}
	}
}

void
TransformSensor::traverse (const TraverseType type)
{
	// Store whether the sensor is part of the »visible« scene graph.

	if (type == TraverseType::CAMERA)
		visible = true;
}

void
TransformSensor::addTool ()
{
	X3DEnvironmentalSensorNode::addTool (new TransformSensorTool (this));
}

} // X3D
} // titania
