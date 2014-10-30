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

//namespace titania {
//namespace math {
//
//template <class Type>
//bool
//disjoint (const matrix3 <Type> & B, const vector3 <Type> & T, const vector3 <Type> & a, const vector3 <Type> & b)
//{
//	const matrix3 <Type> Bf (std::abs (B [0] [0]), std::abs (B [0] [1]), std::abs (B [0] [2]),
//	                         std::abs (B [1] [0]), std::abs (B [1] [1]), std::abs (B [1] [2]),
//	                         std::abs (B [2] [0]), std::abs (B [2] [1]), std::abs (B [2] [2]));
//
//	const matrix3 <Type> BfT = transpose (Bf);
//	const matrix3 <Type> BT  = transpose (B);
//
//	// if any of these tests are one-sided, then the polyhedra are disjoint
//
//	float t, s;
//
//	// A1 x A2 = A0
//	t = T [0] < 0 ? -T [0] : T [0];
//
//	if (t > a [0] + dot (Bf [0], b))
//		return true;
//
//	// B1 x B2 = B0
//	s = dot (BT [0], T);
//	t = s < 0 ? -s : s;
//
//	if (t > b [0] + dot (BfT [0], a))
//		return true;
//
//	// A2 x A0 = A1
//	t = T [1] < 0 ? -T [1] : T [1];
//
//	if (t > a [1] + dot (Bf [1], b))
//		return true;
//
//	// A0 x A1 = A2
//	t = T [2] < 0 ? -T [2] : T [2];
//
//	if (t > a [2] + dot (Bf [2], b))
//		return true;
//
//	// B2 x B0 = B1
//	s = dot (BT [1], T);
//	t = s < 0 ? -s : s;
//
//	if (t > b [1] + dot (BfT [1], a))
//		return true;
//
//	// B0 x B1 = B2
//	s = dot (BT [2], T);
//	t = s < 0 ? -s : s;
//
//	if (t > b [2] + dot (BfT [2], a))
//		return true;
//
//	// A0 x B0
//	s = T [2] * B [1] [0] - T [1] * B [2] [0];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [1] * Bf [2] [0] + a [2] * Bf [1] [0] +
//	         b [1] * Bf [0] [2] + b [2] * Bf [0] [1]))
//		return true;
//
//	// A0 x B1
//	s = T [2] * B [1] [1] - T [1] * B [2] [1];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [1] * Bf [2] [1] + a [2] * Bf [1] [1] +
//	         b [0] * Bf [0] [2] + b [2] * Bf [0] [0]))
//		return true;
//
//	// A0 x B2
//	s = T [2] * B [1] [2] - T [1] * B [2] [2];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [1] * Bf [2] [2] + a [2] * Bf [1] [2] +
//	         b [0] * Bf [0] [1] + b [1] * Bf [0] [0]))
//		return true;
//
//	// A1 x B0
//	s = T [0] * B [2] [0] - T [2] * B [0] [0];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [2] [0] + a [2] * Bf [0] [0] +
//	         b [1] * Bf [1] [2] + b [2] * Bf [1] [1]))
//		return true;
//
//	// A1 x B1
//	s = T [0] * B [2] [1] - T [2] * B [0] [1];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [2] [1] + a [2] * Bf [0] [1] +
//	         b [0] * Bf [1] [2] + b [2] * Bf [1] [0]))
//		return true;
//
//	// A1 x B2
//	s = T [0] * B [2] [2] - T [2] * B [0] [2];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [2] [2] + a [2] * Bf [0] [2] +
//	         b [0] * Bf [1] [1] + b [1] * Bf [1] [0]))
//		return true;
//
//	// A2 x B0
//	s = T [1] * B [0] [0] - T [0] * B [1] [0];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [1] [0] + a [1] * Bf [0] [0] +
//	         b [1] * Bf [2] [2] + b [2] * Bf [2] [1]))
//		return true;
//
//	// A2 x B1
//	s = T [1] * B [0] [1] - T [0] * B [1] [1];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [1] [1] + a [1] * Bf [0] [1] +
//	         b [0] * Bf [2] [2] + b [2] * Bf [2] [0]))
//		return true;
//
//	// A2 x B2
//	s = T [1] * B [0] [2] - T [0] * B [1] [2];
//	t = s < 0 ? -s : s;
//
//	if (t > (a [0] * Bf [1] [2] + a [1] * Bf [0] [2] +
//	         b [0] * Bf [2] [1] + b [1] * Bf [2] [0]))
//		return true;
//
//	return false;
//}
//
//template <class Type>
//bool
//intersect (const box3 <Type> & self, const box3 <Type> & other)
//{
//	// http://gamma.cs.unc.edu/FCL/fcl_docs/webpage/generated/OBB_8h-source.html
//	// http://gamma.cs.unc.edu/FCL/fcl_docs/webpage/generated/OBB_8cpp-source.html
//
//	const matrix3 <Type> m1 (self .matrix ());
//	const matrix3 <Type> m2 (other .matrix ());
//
//	const auto xAxis1 = normalize (m1 [0]);
//	const auto yAxis1 = normalize (m1 [2]);
//	const auto zAxis1 = normalize (m1 [3]);
//
//	const auto xAxis2 = normalize (m2 [0]);
//	const auto yAxis2 = normalize (m2 [1]);
//	const auto zAxis2 = normalize (m2 [2]);
//
//	const vector3 <Type> extent1 (abs (m1 [0]), abs (m1 [1]), abs (m1 [2]));
//	const vector3 <Type> extent2 (abs (m2 [0]), abs (m2 [1]), abs (m2 [2]));
//
//	//
//
//	const auto t = other .center () - self .center ();
//
//	const vector3 <Type> T (dot (t, xAxis1), dot (t, yAxis1), dot (t, zAxis1));
//
//	const matrix3 <Type> R (dot (xAxis1, xAxis2), dot (xAxis1, yAxis2), dot (xAxis1, zAxis2),
//	                        dot (yAxis1, xAxis2), dot (yAxis1, yAxis2), dot (yAxis1, zAxis2),
//	                        dot (zAxis1, xAxis2), dot (zAxis1, yAxis2), dot (zAxis1, zAxis2));
//
//	return not disjoint (R, T, extent1, extent2);
//}
//
//} // math
//} // titania

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
sat_axes (const box3 <Type> & self)
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
class SAT
{
public:

	static
	bool
	overlaps (const std::vector <vector3 <Type>> & axes,
	          const std::vector <vector3 <Type>> & points1,
	          const std::vector <vector3 <Type>> & points2)
	{
		for (const auto & axis : axes)
		{
			Type min1, max1, min2, max2;

			projection (axis, points1, min1, max1);
			projection (axis, points2, min2, max2);

			if (not overlaps (min1, max1, min2, max2))
				return false;
		}

		return true;
	}

private:

	static
	void
	projection (const vector3 <Type> & axis, const std::vector <vector3 <Type>>     & points, Type & min, Type & max)
	{
		min = std::numeric_limits <Type>::infinity ();
		max = -min;

		for (const auto & point : points)
		{
			// Just dot it to get the min and max along this axis.
			// NOTE: the axis must be normalized to get accurate projections to calculate the MTV, but if it is only needed to
			// know whether it overlaps, every axis can be used.

			const Type dotVal = dot (point, axis);

			if (dotVal < min)
				min = dotVal;

			if (dotVal > max)
				max = dotVal;
		}
	}

	static
	bool
	overlaps (const Type min1, const Type max1, const Type min2, const Type max2)
	{
		return is_between (min2, min1, max1) or is_between (min1, min2, max2);
	}

	static
	inline
	bool
	is_between (const Type value, const Type lowerBound, const Type upperBound)
	{
		return lowerBound <= value and value <= upperBound;
	}
};

template <class Type>
bool
intersect (const box3 <Type> & self, const box3 <Type> & other)
{
	// http://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection

	const std::vector <vector3 <Type>>  points1 = points (self);
	const std::vector <vector3 <Type>>  points2 = points (other);
	const std::vector <vector3 <Type>>  axes1   = sat_axes (self);

	if (not SAT <Type>::overlaps (axes1, points1, points2))
		return false;

	const std::vector <vector3 <Type>>  axes2 = sat_axes (other);

	if (not SAT <Type>::overlaps (axes2, points1, points2))
		return false;

	std::vector <vector3 <Type>>  axes3;
	axes3 .reserve (axes1 .size () * points2 .size ());

	for (const auto & axis : axes1)
		for (const auto & point : points2)
			axes3 .emplace_back (cross (axis, point));

	if (not SAT <Type>::overlaps (axes3, points1, points2))
		return false;

	std::vector <vector3 <Type>>  axes4;
	axes4 .reserve (axes2 .size () * points1 .size ());

	for (const auto & axis : axes2)
		for (const auto & point : points1)
			axes4 .emplace_back (cross (axis, point));

	if (not SAT <Type>::overlaps (axes4, points1, points2))
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
