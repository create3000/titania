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

#include "ArcClose2D.h"

#include "../../Browser/Geometry2D/ArcClose2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <complex>

namespace titania {
namespace X3D {

const std::string ArcClose2D::componentName  = "Geometry2D";
const std::string ArcClose2D::typeName       = "ArcClose2D";
const std::string ArcClose2D::containerField = "geometry";

ArcClose2D::Fields::Fields () :
	closureType (new SFString ("PIE")),
	 startAngle (new SFFloat ()),
	   endAngle (new SFFloat (1.570796)),
	     radius (new SFFloat (1)),
	      solid (new SFBool (true))
{ }

ArcClose2D::ArcClose2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "closureType", closureType ());
	addField (inputOutput,    "startAngle",  startAngle ());
	addField (inputOutput,    "endAngle",    endAngle ());
	addField (initializeOnly, "radius",      radius ());
	addField (initializeOnly, "solid",       solid ());
}

X3DBaseNode*
ArcClose2D::create (X3DExecutionContext* const executionContext) const
{
	return new ArcClose2D (executionContext);
}

void
ArcClose2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getBrowserOptions () -> arcClose2DOptions () .addInterest (this, &ArcClose2D::set_properties);
}

float
ArcClose2D::getAngle ()
{
	float start = math::interval <float> (startAngle (), 0, M_PI2);
	float end   = math::interval <float> (endAngle (),   0, M_PI2);

	if (start == end)
		return M_PI2;

	float difference = std::min (std::abs (end - start), float (M_PI2));

	if (start > end)
		return M_PI2 - difference;

	return difference;
}

void
ArcClose2D::set_properties ()
{
	update ();
}

void
ArcClose2D::build ()
{
	const ArcClose2DOptions* properties = getBrowser () -> getBrowserOptions () -> arcClose2DOptions ();

	float  difference = getAngle ();
	size_t segments   = std::ceil (difference / properties -> minAngle ());
	float  angle      = difference / segments;

	size_t elements = solid () ? 1 : 2;
	size_t vertices = segments + 2;
	size_t reserve  = elements * vertices;

	getTexCoord () .reserve (reserve);
	getNormals  () .reserve (reserve);
	getVertices () .reserve (reserve);

	if (difference < float (2 * M_PI))
	{
		// If it is a arc, add a center point otherwise it is a circle.

		if (closureType () not_eq "CHORD")
		{
			getTexCoord () .emplace_back (0.5, 0.5, 0, 1);
			getNormals  () .emplace_back (0, 0, 1);
			getVertices () .emplace_back (0, 0, 0);
		}

		++ segments;
	}

	for (size_t n = 0; n < segments; ++ n)
	{
		float theta = startAngle () + angle * n;

		auto texCoord = std::polar (0.5f, theta) + std::complex <float> (0.5f, 0.5f);
		auto point    = std::polar (std::abs (radius ()), theta);

		getTexCoord () .emplace_back (texCoord .real (), texCoord .imag (), 0, 1);
		getNormals  () .emplace_back (0, 0, 1);
		getVertices () .emplace_back (point .real (), point .imag (), 0);
	}

	addElements (GL_POLYGON, getVertices () .size ());
	setSolid (true);

	if (not solid ())
		addMirrorVertices (GL_POLYGON, false);
}

} // X3D
} // titania
