/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "NurbsTrimmedSurface.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string NurbsTrimmedSurface::componentName  = "NURBS";
const std::string NurbsTrimmedSurface::typeName       = "NurbsTrimmedSurface";
const std::string NurbsTrimmedSurface::containerField = "geometry";

NurbsTrimmedSurface::Fields::Fields () :
	   addTrimmingContour (new MFNode ()),
	removeTrimmingContour (new MFNode ()),
	      trimmingContour (new MFNode ())
{ }

NurbsTrimmedSurface::NurbsTrimmedSurface (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DNurbsSurfaceGeometryNode (),
	                     fields ()
{
	addField (inputOutput,    "metadata",              metadata ());
	addField (inputOutput,    "uTessellation",         uTessellation ());
	addField (inputOutput,    "vTessellation",         vTessellation ());
	addField (initializeOnly, "uClosed",               uClosed ());
	addField (initializeOnly, "vClosed",               vClosed ());
	addField (initializeOnly, "solid",                 solid ());
	addField (initializeOnly, "uOrder",                uOrder ());
	addField (initializeOnly, "vOrder",                vOrder ());
	addField (initializeOnly, "uDimension",            uDimension ());
	addField (initializeOnly, "vDimension",            vDimension ());
	addField (initializeOnly, "uKnot",                 uKnot ());
	addField (initializeOnly, "vKnot",                 vKnot ());
	addField (inputOutput,    "weight",                weight ());
	addField (inputOutput,    "texCoord",              texCoord ());
	addField (inputOutput,    "controlPoint",          controlPoint ());
	addField (inputOnly,      "addTrimmingContour",    addTrimmingContour ());
	addField (inputOnly,      "removeTrimmingContour", removeTrimmingContour ());
	addField (inputOutput,    "trimmingContour",       trimmingContour ());
}

X3DBaseNode*
NurbsTrimmedSurface::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsTrimmedSurface (executionContext);
}

void
NurbsTrimmedSurface::initialize ()
{
	X3DNurbsSurfaceGeometryNode::initialize ();

	addTrimmingContour ()    .addInterest (this, &NurbsTrimmedSurface::set_addTrimmingContour);
	removeTrimmingContour () .addInterest (this, &NurbsTrimmedSurface::set_removeTrimmingContour);
	trimmingContour ()       .addInterest (this, &NurbsTrimmedSurface::set_trimmingContour);

	set_trimmingContour ();
}

void
NurbsTrimmedSurface::set_addTrimmingContour ()
{

}

void
NurbsTrimmedSurface::set_removeTrimmingContour ()
{

}

void
NurbsTrimmedSurface::set_trimmingContour ()
{

}

std::vector <Contour2D*>
NurbsTrimmedSurface::getContours () const
{
	std::vector <Contour2D*> contours;

	for (const auto & node : trimmingContour ())
	{
		auto contour = x3d_cast <Contour2D*> (node);
		
		if (contour)
			contours .emplace_back (contour);
	}

	if (is_odd (contours .size ()))
		contours .pop_back ();

	return contours;
}

void
NurbsTrimmedSurface::trimSurface (GLUnurbs* nurbsRenderer) const
{
	std::vector <Contour2D*> contours = std::move (getContours ());
	
	for (size_t i = 0, size = contours .size (); i < size; i += 2)
	{
		size_t i1 = i, i2 = i + 1;

		if (contours [i1] -> isEmpty () or contours [i2] -> isEmpty ())
			continue;

		if (not contours [i1] -> isClosed () or not contours [i2] -> isClosed ())
			continue;

		if (not contours [i1] -> getBBox () .contains (contours [i2] -> getBBox ()))
			continue;

		contours [i1] -> trimSurface (nurbsRenderer);
		contours [i2] -> trimSurface (nurbsRenderer);
	}
}

} // X3D
} // titania
