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

#include "AxonometricGridTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType AxonometricGridTool::component      = ComponentType::TITANIA;
const std::string   AxonometricGridTool::typeName       = "AxonometricGridTool";
const std::string   AxonometricGridTool::containerField = "grid";

AxonometricGridTool::Fields::Fields () :
  angle (new SFVec2f (radians (60.0), radians (60.0)))
{ }

AxonometricGridTool::AxonometricGridTool (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGridTool (),
	     fields ()
{
	//addType (X3DConstants::AxonometricGridTool);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "enabled",         enabled ());
	addField (inputOutput, "translation",     translation ());
	addField (inputOutput, "rotation",        rotation ());
	addField (inputOutput, "scale",           scale ());
	addField (inputOutput, "dimension",       dimension ());
	addField (inputOutput, "angle",           angle ());
	addField (inputOutput, "majorLineEvery",  majorLineEvery ());
	addField (inputOutput, "majorLineOffset", majorLineOffset ());
	addField (inputOutput, "color",           color ());
	addField (inputOutput, "lineColor",       lineColor ());
	addField (inputOutput, "majorLineColor",  majorLineColor ());
	addField (inputOutput, "snapToCenter",    snapToCenter ());
	addField (inputOutput, "snapDistance",    snapDistance ());
	addField (outputOnly,  "isActive",        isActive ());

	angle () .setUnit (UnitCategory::ANGLE);

	dimension ()       = { 10, 10 };
	majorLineEvery ()  = { 5, 5, 5, 5 };
	majorLineOffset () = { 0, 0, 0, 0 };
}

X3DBaseNode*
AxonometricGridTool::create (X3DExecutionContext* const executionContext) const
{
	return new AxonometricGridTool (executionContext);
}

void
AxonometricGridTool::initialize ()
{
	X3DGridTool::initialize ();

	requestAsyncLoad ({ get_tool ("AxonometricGridTool.x3dv") .str () });
}

void
AxonometricGridTool::realize ()
{
	X3DGridTool::realize ();

	try
	{
		auto & set_angle = getToolNode () -> getField <SFVec2f> ("set_angle");
		angle ()  .addInterest (set_angle);
		set_angle .addInterest (angle ());
		set_angle = angle ();
	}
	catch (const X3DError & error)
	{ }
}

/**
 * @returns Barycentric coordinates (u, v, w) for @a point with respect to triangle (a, b, c).
 * @param point  in cartesian coordinate system.
 * @param a      first point of triangle.
 * @param b      second point of triangle.
 * @param c      third point of triangle.\n
 * Type is any type supporting copy constructions.
 */
template <class Type>
vector3 <Type>
barycentric (const vector3 <Type> & point,
             const vector3 <Type> & a,
             const vector3 <Type> & b,
             const vector3 <Type> & c)
{
	const auto v0 = b - a;
	const auto v1 = c - a;
	const auto v2 = point - a;

	const auto d00   = dot (v0, v0);
	const auto d01   = dot (v0, v1);
	const auto d11   = dot (v1, v1);
	const auto d20   = dot (v2, v0);
	const auto d21   = dot (v2, v1);
	const auto denom = d00 * d11 - d01 * d01;

	const auto v = (d11 * d20 - d01 * d21) / denom;
	const auto t = (d00 * d21 - d01 * d20) / denom;
	const auto u = 1 - v - t;

	return vector3 <Type> (u, v, t);
}

Vector3d
AxonometricGridTool::getSnapPosition (const Vector3d & position, const bool snapY)
{
	auto translation = position;

	const auto angles = Vector3d (angle () [0], angle () [1], pi <double> - angle () [0] - angle () [1]);
	const auto u      = std::sin (angles [1]) / std::sin (angles [2]);
	const auto v      = 1;
	const auto t      = std::sin (angles [0]) / std::sin (angles [2]);
	const auto A      = Vector3d (0, 0, 0);
	const auto B      = Vector3d (v, 0, 0);
	const auto C      = Vector3d (u, 0, 0) * Rotation4d (0, 1, 0, angles [0]);

	const auto coord = barycentric (translation, A, B, C);

	__LOG__ << coord << std::endl;

	return translation;
}

Vector3d
AxonometricGridTool::getSnapPosition (const Vector3d & position, const Vector3d & direction)
{
	return position;
}

} // X3D
} // titania
