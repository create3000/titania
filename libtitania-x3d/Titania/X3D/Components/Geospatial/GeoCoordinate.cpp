/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "GeoCoordinate.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

const std::string GeoCoordinate::componentName  = "Geospatial";
const std::string GeoCoordinate::typeName       = "GeoCoordinate";
const std::string GeoCoordinate::containerField = "coord";

GeoCoordinate::Fields::Fields () :
	geoSystem (new MFString ({ "GD", "WE" })),
	    point (new MFVec3d ()),
	geoOrigin (new SFNode ())
{ }

GeoCoordinate::GeoCoordinate (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DCoordinateNode (),
	           fields (),
	   referenceFrame ()
{
	addField (inputOutput,    "metadata",  metadata ());
	addField (initializeOnly, "geoSystem", geoSystem ());
	addField (inputOutput,    "point",     point ());
	addField (initializeOnly, "geoOrigin", geoOrigin ());
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

	geoSystem () .addInterest (this, &GeoCoordinate::set_geoSystem);

	set_geoSystem ();
}

void
GeoCoordinate::set_geoSystem ()
{
	referenceFrame = Geospatial::getReferenceFrame (geoSystem ());
}

Box3f
GeoCoordinate::getBBox () const
{
	return Box3d (point () .begin (), point () .end (), math::iterator_type ());
}

Vector3f
GeoCoordinate::getNormal (const size_t index1, const size_t index2, const size_t index3) const
{
	const size_t size = point () .size ();

	if (index1 < size and index2 < size and index3 < size)
		return math::normal (referenceFrame -> convert (point () [index1]),
		                     referenceFrame -> convert (point () [index2]),
		                     referenceFrame -> convert (point () [index3]));

	return Vector3f (0, 0, 1);
}

void
GeoCoordinate::addVertex (opengl::tessellator <size_t> & tessellator, const size_t index, const size_t i) const
{
	if (index < point () .size ())
		tessellator .add_vertex (referenceFrame -> convert (point () [index]), i);

	else
		tessellator .add_vertex (referenceFrame -> convert (Vector3f ()), i);
}

void
GeoCoordinate::addVertex (std::vector <Vector3f> & vertices, const size_t index) const
{
	if (index < point () .size ())
		vertices .emplace_back (referenceFrame -> convert (point () [index]));

	else
		vertices .emplace_back (referenceFrame -> convert (Vector3f ()));
}

std::vector <Vector4f>
GeoCoordinate::getControlPoints (const MFDouble & weight) const
{
	std::vector <Vector4f> controlPoints;

	controlPoints .reserve (point () .size ());

	if (weight .size () < point () .size ())
	{
		for (size_t i = 0; i < point () .size (); i ++)
		{
			const auto p = referenceFrame -> convert (point () [i]);

			controlPoints .emplace_back (p .x (), p .y (), p .z (), 1);
		}
	}
	else
	{
		for (size_t i = 0; i < point () .size (); i ++)
		{
			const auto p = referenceFrame -> convert (point () [i]);

			controlPoints .emplace_back (p . x (), p . y (), p . z (), weight [i]);
		}
	}

	return controlPoints;
}

} // X3D
} // titania
