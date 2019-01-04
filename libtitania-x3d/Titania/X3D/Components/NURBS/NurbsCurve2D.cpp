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

#include "NurbsCurve2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string NurbsCurve2D::componentName  = "NURBS";
const std::string NurbsCurve2D::typeName       = "NurbsCurve2D";
const std::string NurbsCurve2D::containerField = "children";

NurbsCurve2D::Fields::Fields () :
	tessellation (new SFInt32 ()),
	      closed (new SFBool ()),
	       order (new SFInt32 (3)),
	        knot (new MFDouble ()),
	      weight (new MFDouble ())
{ }

NurbsCurve2D::NurbsCurve2D (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DNurbsControlCurveNode (),
	                  fields ()
{
	addType (X3DConstants::NurbsCurve2D);

	addField (inputOutput,    "metadata",     metadata ());
	addField (inputOutput,    "tessellation", tessellation ());
	addField (initializeOnly, "closed",       closed ());
	addField (initializeOnly, "order",        order ());
	addField (initializeOnly, "knot",         knot ());
	addField (inputOutput,    "weight",       weight ());
	addField (inputOutput,    "controlPoint", controlPoint ());
}

X3DBaseNode*
NurbsCurve2D::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsCurve2D (executionContext);
}

std::vector <float>
NurbsCurve2D::getKnots (const MFDouble & knot, const int32_t order, const int32_t dimension) const
{
	std::vector <float> knots (knot .cbegin (), knot .cend ());

	bool generateUniform = true;

	if (knots .size () == size_t (dimension + order))
	{
		generateUniform = false;

		size_t consecutiveKnots = 0;

		for (size_t i = 1; i < knots .size (); ++ i)
		{
			if (knots [i] == knots [i - 1])
				++ consecutiveKnots;
			else
				consecutiveKnots = 0;

			if (consecutiveKnots > size_t (order - 1))
				generateUniform = true;

			if (knots [i - 1] > knots [i])
				generateUniform = true;
		}
	}

	if (generateUniform)
	{
		knots .resize (dimension + order);

		for (size_t i = 0, size = knots .size (); i < size; ++ i)
			knots [i] = (float) i / (size - 1);
	}

	return knots;
}

std::vector <Vector3f>
NurbsCurve2D::getControlPoints () const
{
	std::vector <Vector3f> controlPoints;

	controlPoints .reserve (controlPoint () .size ());
	
	if (weight () .size () < controlPoint () .size ())
	{
		for (size_t i = 0; i < controlPoint () .size (); i ++)
		{
			controlPoints .emplace_back (controlPoint () [i] .x (),
			                             controlPoint () [i] .y (),
			                             1);
		}
	}
	else
	{
		for (size_t i = 0; i < controlPoint () .size (); i ++)
		{
			controlPoints .emplace_back (controlPoint () [i] .x (), 
			                             controlPoint () [i] .y (), 
			                             weight () [i]);
		}
	}

	return controlPoints;
}

void
NurbsCurve2D::draw (GLUnurbs* nurbsRenderer) const
{
	if (order () < 2)
		return;

	if (controlPoint () .size () < (size_t) order ())
		return;

	std::vector <float>    knots         = getKnots (knot (), order (), controlPoint () .size ());
	std::vector <Vector3f> controlPoints = getControlPoints ();

	gluNurbsCurve (nurbsRenderer,
	               knots .size (), knots .data (),
	               3, controlPoints [0] .data (),
	               order (),
	               GLU_MAP1_TRIM_3);
}

} // X3D
} // titania
