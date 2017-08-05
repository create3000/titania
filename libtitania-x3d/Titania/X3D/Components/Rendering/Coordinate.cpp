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

#include "Coordinate.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

#include <Titania/Math/Mesh/Tessellator.h>

namespace titania {
namespace X3D {

const ComponentType Coordinate::component      = ComponentType::RENDERING;
const std::string   Coordinate::typeName       = "Coordinate";
const std::string   Coordinate::containerField = "coord";

Coordinate::Fields::Fields () :
	point (new MFVec3f ())
{ }

Coordinate::Coordinate (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DCoordinateNode (),
	           fields ()
{
	addType (X3DConstants::Coordinate);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "point",    point ());

	point () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
Coordinate::create (X3DExecutionContext* const executionContext) const
{
	return new Coordinate (executionContext);
}

Box3d
Coordinate::getBBox () const
{
	return Box3f (point () .begin (), point () .end (), math::iterator_type ());
}

void
Coordinate::set1Point (const size_t index, const Vector3d & value)
{
	point () .set1Value (index, value);
}

Vector3d
Coordinate::get1Point (const size_t index)
{
	if (index < point () .size ())
		return Vector3d (point () [index] .getValue ());

	return Vector3d ();
}

Vector3f
Coordinate::getNormal (const size_t index1, const size_t index2, const size_t index3) const
{
	const size_t size = point () .size ();

	if (index1 < size and index2 < size and index3 < size)
		return Triangle3f (point () [index1],
		                   point () [index2],
		                   point () [index3]) .normal ();

	return Vector3f (0, 0, 0);
}

Vector3f
Coordinate::getNormal (const size_t index1, const size_t index2, const size_t index3, const size_t index4) const
{
	const size_t size = point () .size ();

	if (index1 < size and index2 < size and index3 < size and index4 < size)
		return math::normal <float> (point () [index1],
		                             point () [index2],
		                             point () [index3],
		                             point () [index4]);

	return Vector3f (0, 0, 0);
}

void
Coordinate::addVertex (math::tessellator <double, size_t> & tessellator, const size_t index, const size_t i) const
{
	if (index < point () .size ())
		tessellator .add_vertex (point () [index] .getValue (), i);

	else
		tessellator .add_vertex (Vector3d (), i);
}

void
Coordinate::addVertex (std::vector <Vector3d> & vertices, const size_t index) const
{
	if (index < point () .size ())
		vertices .emplace_back (point () [index] .getValue ());

	else
		vertices .emplace_back ();
}

std::vector <Vector4d>
Coordinate::getControlPoints (const MFDouble & weight) const
{
	std::vector <Vector4d> controlPoints;

	controlPoints .reserve (point () .size ());

	if (weight .size () < point () .size ())
	{
		for (size_t i = 0; i < point () .size (); i ++)
			controlPoints .emplace_back (point () [i] .getX (),
			                             point () [i] .getY (),
			                             point () [i] .getZ (),
			                             1);
	}
	else
	{
		for (size_t i = 0; i < point () .size (); i ++)
			controlPoints .emplace_back (point () [i] .getX (),
			                             point () [i] .getY (),
			                             point () [i] .getZ (),
			                             weight [i]);
	}

	return controlPoints;
}

void
Coordinate::erasePoint (const size_t index)
{
	point () .erase (point () .begin () + index);
}

} // X3D
} // titania
