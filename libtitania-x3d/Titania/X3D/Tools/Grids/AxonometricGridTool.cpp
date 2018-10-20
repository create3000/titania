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

#include <Titania/Math/Algorithms/Barycentric.h>

namespace titania {
namespace X3D {

const ComponentType AxonometricGridTool::component      = ComponentType::TITANIA;
const std::string   AxonometricGridTool::typeName       = "AxonometricGridTool";
const std::string   AxonometricGridTool::containerField = "grid";

AxonometricGridTool::Fields::Fields () :
  angle (new SFVec2d (radians (60.0), radians (60.0)))
{ }

AxonometricGridTool::AxonometricGridTool (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGridTool (),
	     fields ()
{
	addType (X3DConstants::AxonometricGridTool);

	addField (inputOutput, "metadata",        metadata ());
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
	addField (inputOutput, "snapping",        snapping ());
	addField (inputOutput, "snapToCenter",    snapToCenter ());
	addField (inputOutput, "snapDistance",    snapDistance ());
	addField (inputOutput, "collision",       collision ());
	addField (outputOnly,  "isActive",        isActive ());

	angle () .setUnit (UnitCategory::ANGLE);

	dimension ()       = { 5, 10 };
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
	try
	{
		X3DGridTool::realize ();
	
		auto & set_angle = getToolNode () -> getField <SFVec2d> ("set_angle");
		angle ()  .addInterest (set_angle);
		set_angle .addInterest (angle ());
		set_angle .addEventObject (angle ()); // TODO: use normal assign, and remove addEventObject from X3DField.
	}
	catch (const X3DError & error)
	{ }
}

Vector3d
AxonometricGridTool::getSnapPosition (const Vector3d & position, const bool snapY) const
{
	auto p = position;

	p .y (0);

	// Construct simplex.

	const auto angles = Vector3d (angle () [0], angle () [1], pi <double> - angle () [0] - angle () [1]);
	const auto u      = std::sin (angles [1]) / std::sin (angles [2]);
	const auto v      = 1;
	const auto As     = Vector3d (0, 0, 0);
	const auto Bs     = Vector3d (v, 0, 0);
	const auto Cs     = Vector3d (u, 0, 0) * Rotation4d (0, 1, 0, angles [0]);

	// Construct triangle.

	const auto triangle = barycentric_triangle (to_barycentric (position, As, Bs, Cs));
	const auto A        = from_barycentric (std::get <0> (triangle), As, Bs, Cs);
	const auto B        = from_barycentric (std::get <1> (triangle), As, Bs, Cs);
	const auto C        = from_barycentric (std::get <2> (triangle), As, Bs, Cs);

	// Find closest point.

	const auto vertices = std::array <Vector3d, 3> ({ A, B, C });
	const auto vD       = std::array <double, 3> ({ abs (A - p), abs (B - p), abs (C - p) });
	const auto iter     = std::min_element (vD .cbegin (), vD .cend ());

	// Test all three vertices of the triangle.

	if (*iter < std::abs (snapDistance ()))
	{
		p = vertices [iter - vD .cbegin ()];
	}
	else
	{
		// Test all three edges of the triangle.

		const auto uV     = A - C;
		const auto vV     = B - A;
		const auto tV     = C - B;
		const auto nU     = normalize (Vector3d (uV .z (), 0, -uV .x ()));
		const auto nV     = normalize (Vector3d (vV .z (), 0, -vV .x ()));
		const auto nT     = normalize (Vector3d (tV .z (), 0, -tV .x ()));
		const auto pU     = Plane3d (A, nU);
		const auto pV     = Plane3d (B, nV);
		const auto pT     = Plane3d (C, nT);
		const auto planes = std::array <Plane3d, 3> ({ pU, pV, pT });
		const auto eD     = std::array <double, 3> ({ std::abs (pU .distance (p)), std::abs (pV .distance (p)), std::abs (pT .distance (p)) });
		const auto iter   = std::min_element (eD .cbegin (), eD .cend ());

		if (*iter < std::abs (snapDistance ()))
			p = planes [iter - eD .cbegin ()] .closest_point (p);
	}

	{
		// snapping y-Axis.

		const auto o  = const_cast <AxonometricGridTool*> (this) -> dimension () .get1Value (1) % 2 * 0.5; // Add a half scale if dimension is odd.
		const auto yr = std::round (position .y ());
		const auto p1 = yr - o;
		const auto p2 = yr + o;
		const auto y  = std::abs (p1 - position .y ()) < std::abs (p2 - position .y ()) ? p1 : p2;

		if (std::abs (y - position .y ()) < std::abs (snapDistance ()))
			p .y (y);
		else
			p .y (position .y ());
	}

	return p;
}

Vector3d
AxonometricGridTool::getSnapPosition (const Vector3d & position, const Vector3d & direction) const
{
	return position;
}

} // X3D
} // titania
