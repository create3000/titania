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
#include <Titania/Math/Utility/Normal.h>

namespace titania {
namespace X3D {

const std::string GeoCoordinate::componentName  = "Geospatial";
const std::string GeoCoordinate::typeName       = "GeoCoordinate";
const std::string GeoCoordinate::containerField = "coord";

GeoCoordinate::Fields::Fields () :
	geoOrigin (new SFNode ()),
	geoSystem (new MFString ({ "GD", "WE" })),
	    point (new MFVec3d ())
{ }

GeoCoordinate::GeoCoordinate (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DCoordinateNode (),
	           fields ()
{
	addField (inputOutput,    "metadata",  metadata ());
	addField (initializeOnly, "geoOrigin", geoOrigin ());
	addField (initializeOnly, "geoSystem", geoSystem ());
	addField (inputOutput,    "point",     point ());
}

X3DBaseNode*
GeoCoordinate::create (X3DExecutionContext* const executionContext) const
{
	return new GeoCoordinate (executionContext);
}

Vector3f
GeoCoordinate::getNormal (size_t index1, size_t index2, size_t index3) const
{
	return math::normal <double> (point () [index1],
	                              point () [index2],
	                              point () [index3]);
}

void
GeoCoordinate::addVertex (opengl::tesselator <size_t> & tesselator, int32_t index, size_t i) const
{
	if (index > -1)
		tesselator .add_vertex (point () [index] .getValue (), i);

	else
		tesselator .add_vertex (Vector3f (), i);
}

void
GeoCoordinate::addVertex (std::vector <Vector3f> & vertices, int32_t index) const
{
	if (index > -1)
		vertices .emplace_back (point () [index] .getValue ());

	else
		vertices .emplace_back ();
}

std::vector <Vector4f>
GeoCoordinate::getControlPoints (const MFDouble & weight) const
{
	std::vector <Vector4f> controlPoints;

	controlPoints .reserve (point () .size ());
	
	if (weight .size () < point () .size ())
	{
		for (size_t i = 0; i < point () .size (); i ++)
			controlPoints .emplace_back (point () [i] .getX (), point () [i] .getY (), point () [i] .getZ (), 1);
	}
	else
	{
		for (size_t i = 0; i < point () .size (); i ++)
			controlPoints .emplace_back (point () [i] .getX (), point () [i] .getY (), point () [i] .getZ (), weight [i]);
	}

	return controlPoints;
}

void
GeoCoordinate::resize (size_t size)
{
	if (point () .size () < size)
		point () .resize (size);
}

} // X3D
} // titania
