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

#include "AngleGridTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Execution/X3DExecutionContext.h"

#include <complex>

namespace titania {
namespace X3D {

const ComponentType AngleGridTool::component      = ComponentType::TITANIA;
const std::string   AngleGridTool::typeName       = "AngleGridTool";
const std::string   AngleGridTool::containerField = "grid";

AngleGridTool::AngleGridTool (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGridTool ()
{
	addType (X3DConstants::AngleGridTool);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "translation",     translation ());
	addField (inputOutput, "rotation",        rotation ());
	addField (inputOutput, "scale",           scale ());
	addField (inputOutput, "dimension",       dimension ());
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

	dimension ()       = { 5, 16, 10 };
	majorLineEvery ()  = { 5, 2, 5 };
	majorLineOffset () = { 0, 0, 0 };
}

X3DBaseNode*
AngleGridTool::create (X3DExecutionContext* const executionContext) const
{
	return new AngleGridTool (executionContext);
}

void
AngleGridTool::initialize ()
{
	X3DGridTool::initialize ();

	requestAsyncLoad ({ get_tool ("AngleGridTool.x3dv") .str () });
}

Vector3d
AngleGridTool::getSnapPosition (const Vector3d & position, const bool snapY) const
{
	constexpr double offset = pi <double> / 2;

	auto translation = position;

	// Calculate snapping radius and snapping angle

	const auto complex    = std::complex <double> (translation .x (), translation .z ());
	const auto phi        = 2 * pi <double> / dimension () [1];
	const auto radius     = std::abs (complex);
	const auto angle      = std::arg (complex);
	auto       snapRadius = std::round (radius);
	auto       snapAngle  = std::round ((angle - offset) / phi) * phi + offset;

	if (std::abs (snapRadius - radius) > std::abs (snapDistance ()))
		snapRadius = radius;

	if (std::abs (snapAngle - angle) > std::abs (snapDistance () * phi) or dimension () [1] == 0)
		snapAngle = angle;

	const auto snapPolar = std::polar (snapRadius, snapAngle);

	translation .x (snapPolar .real ());
	translation .z (snapPolar .imag ());

	const auto y = getSnapPosition (translation .y ());
	
	if (snapY)
	{
		if (std::abs (y - translation .y ()) < std::abs (snapDistance ()))
			translation .y (y);
	}

	if (snapRadius == radius and snapAngle == angle and translation .y () not_eq y)
		return position;

	return translation;
}

Vector3d
AngleGridTool::getSnapPosition (const Vector3d & position, const Vector3d & direction) const
{
	return position;
}

double
AngleGridTool::getSnapPosition (const double position) const
{
	const auto o  = const_cast <AngleGridTool*> (this) -> dimension () .get1Value (2) % 2 * 0.5; // Add a half scale if dimension is odd.
	const auto p  = std::round (position);
	const auto p1 = p - o;
	const auto p2 = p + o;

	return std::abs (p1 - position) < std::abs (p2 - position) ? p1 : p2;
}

} // X3D
} // titania
