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

#include "GeoCoordinate.h"

#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Mesh/Tessellator.h>

namespace titania {
namespace X3D {

const std::string GeoCoordinate::componentName  = "Geospatial";
const std::string GeoCoordinate::typeName       = "GeoCoordinate";
const std::string GeoCoordinate::containerField = "coord";

GeoCoordinate::Fields::Fields () :
	point (new MFVec3d ())
{ }

GeoCoordinate::GeoCoordinate (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DCoordinateNode (),
	X3DGeospatialObject (),
	             fields (),
	             points (),
	             origin ()
{
	addType (X3DConstants::GeoCoordinate);

	addField (inputOutput,    "metadata",  metadata ());
	addField (initializeOnly, "geoOrigin", geoOrigin ());
	addField (initializeOnly, "geoSystem", geoSystem ());
	addField (inputOutput,    "point",     point ());

	point () .setUnit (UnitCategory::LENGTH);
	point () .isGeospatial (true);
}

X3DBaseNode*
GeoCoordinate::create (X3DExecutionContext* const executionContext) const
{
	return new GeoCoordinate (executionContext);
}

void
GeoCoordinate::initialize ()
{
	X3DCoordinateNode::initialize ();
	X3DGeospatialObject::initialize ();

	addInterest (&GeoCoordinate::eventsProcessed, this);

	eventsProcessed ();
}

void
GeoCoordinate::eventsProcessed ()
{
	//XXX: use std::transform???

	points .clear ();

	for (const auto & p : point ())
		points .emplace_back (getCoord (p));

	origin = getCoord (Vector3f ());
}

Box3d
GeoCoordinate::getBBox () const
{
	return Box3d (points .cbegin (), points .cend (), math::iterator_type ());
}

void
GeoCoordinate::set1Point (const size_t index, const Vector3d & value)
{
	if (index >= points .size ())
		points .resize (index + 1);

	points [index] = value;	

	point () .set1Value (index, getGeoCoord (value));
}

Vector3d
GeoCoordinate::get1Point (const size_t index) const
{
	if (index < points .size ())
		return points [index];

	return origin;
}

// Normal calculation must take place in the coordinate node to get correct geo mormal.
Vector3f
GeoCoordinate::getNormal (const size_t index1, const size_t index2, const size_t index3) const
{
	const size_t size = points .size ();

	if (index1 < size and index2 < size and index3 < size)
		return Triangle3f (points [index1], points [index2], points [index3]) .normal ();

	return Vector3f (0, 0, 0);
}

// Normal calculation must take place in the coordinate node to get correct geo mormal.
Vector3f
GeoCoordinate::getNormal (const size_t index1, const size_t index2, const size_t index3, const size_t index4) const
{
	const size_t size = points .size ();

	if (index1 < size and index2 < size and index3 < size and index4 < size)
		return math::normal (points [index1],
		                     points [index2],
		                     points [index3],
		                     points [index4]);

	return Vector3f (0, 0, 0);
}

void
GeoCoordinate::addVertex (math::tessellator <double, size_t> & tessellator, const size_t index, const size_t i) const
{
	if (index < points .size ())
		tessellator .add_vertex (points [index], i);

	else
		tessellator .add_vertex (origin, i);
}

void
GeoCoordinate::addVertex (std::vector <Vector3d> & vertices, const size_t index) const
{
	if (index < points .size ())
		vertices .emplace_back (points [index]);

	else
		vertices .emplace_back (origin);
}

void
GeoCoordinate::erasePoint (const size_t index)
{
	point () .erase (point () .begin () + index);
}

void
GeoCoordinate::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DCoordinateNode::dispose ();
}

} // X3D
} // titania
