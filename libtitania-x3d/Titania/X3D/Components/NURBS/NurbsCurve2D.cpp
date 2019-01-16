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

size_t
NurbsCurve2D::getTessellation (const size_t dimension) const
{
	// Return a tessellation one less to the standard to get the right tessellation.

	if (tessellation () > 0)
		return tessellation () + 1;
		
	if (tessellation () < 0)
		return -tessellation () * dimension + 1;

	return 2 * dimension + 1;
}

bool
NurbsCurve2D::getClosed (const size_t order,
                         const size_t dimension,
                         const std::vector <double> & knot,
                         const std::vector <double> & weight) const
{
	if (not closed ())
		return false;

	const auto haveWeights = weight .size () == controlPoint () .size ();

	// Check if first and last weights are unitary.

	if (haveWeights)
	{
		if (weight .front () not_eq weight .back ())
			return false;
	}

	// Check if first and last point are coincident.

	if (controlPoint () .front () not_eq controlPoint () .back ())
		return false;

	// Check if knots are periodic.

	if (knot .size () == dimension + order)
	{
		{
			size_t count = 1;
	
			for (size_t i = 1, size = order; i < size; ++ i)
			{
				count += knot [i] == knot .front ();
			}
	
			if (count == order)
				return false;
		}

		{
			size_t count = 1;
	
			for (size_t i = knot .size () - order, size = knot .size () - 1; i < size; ++ i)
			{
				count += knot [i] == knot .back ();
			}

			if (count == order)
				return false;
		}
	}

	return true;
}

std::vector <float>
NurbsCurve2D::getKnots (const bool closed,
                        const size_t order,
                        const size_t dimension,
                        const std::vector <double> & knot) const
{
	std::vector <float> knots (knot .cbegin (), knot .cend ());

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.

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

			if (consecutiveKnots > order - 1)
				generateUniform = true;

			if (knots [i - 1] > knots [i])
				generateUniform = true;
		}
	}

	if (generateUniform)
	{
		knots .resize (dimension + order);

		for (size_t i = 0, size = knots .size (); i < size; ++ i)
			knots [i] = float (i) / (size - 1);
	}

	if (closed)
	{
		for (size_t i = 1, size = order - 1; i < size; ++ i)
			knots .emplace_back (knots .back () + (knots [i] - knots [i - 1]));
	}

	return knots;
}

std::vector <Vector3f>
NurbsCurve2D::getControlPoints (const bool closed,
                                const size_t order,
                                const size_t dimension,
                                const std::vector <double> & weight) const
{
	std::vector <Vector3f> controlPoints;

	if (weight .size () not_eq dimension)
	{
		for (size_t i = 0; i < dimension; ++ i)
		{
			const auto & p = controlPoint () [i];

			controlPoints .emplace_back (p .x (),
			                             p .y (),
			                             1);
		}

		if (closed)
		{
			for (size_t i = 1, size = order - 1; i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}
	else
	{
		for (size_t i = 0; i < dimension; ++ i)
		{
			const auto & p = controlPoint () [i];

			controlPoints .emplace_back (p .x (),
			                             p .y (),
			                             weight [i]);
		}

		if (closed)
		{
			for (size_t i = 1, size = order - 1; i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}

	return controlPoints;
}

std::vector <Vector2f>
NurbsCurve2D::tessellate () const
{
	if (order () < 2)
		return { };

	if (controlPoint () .size () < (size_t) order ())
		return { };

	// Order and dimension are now positive numbers.

	const auto closed        = getClosed (order (), controlPoint () .size (), knot (), weight ());
	auto       controlPoints = getControlPoints (closed, order (), controlPoint () .size (), weight ());

	// Knots

	auto       knots = getKnots (closed, order (), controlPoint () .size (), knot ());
	const auto scale = knots .back () - knots .front ();

	// Tessellate

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, scale ? getTessellation (knots .size () - order ()) / scale : 1);

	tessellator .begin_curve ();

	tessellator .nurbs_curve (knots .size (), knots .data (),
	                          3, controlPoints [0] .data (),
	                          order (),
	                          GL_MAP1_VERTEX_3);

	tessellator .end_curve ();

	// End tessellation

	// Lines

	const auto & lines = tessellator .lines ();
	auto         curve = std::vector <Vector2f> ();

	for (size_t i = 0, size = lines .size (); i < size; i += 2)
	{
		const auto & vertex = lines [i];

		curve .emplace_back (vertex .x (), vertex .y ());
	}

	const auto & vertex = lines .back ();

	curve .emplace_back (vertex .x (), vertex .y ());

	return curve;
}

void
NurbsCurve2D::trim (nurbs_tessellator & tessellator) const
{
	if (order () < 2)
		return;

	if (controlPoint () .size () < (size_t) order ())
		return;

	const auto closed        = getClosed (order (), controlPoint () .size (), knot (), weight ());
	auto       controlPoints = getControlPoints (closed, order (), controlPoint () .size (), weight ());
	auto       knots         = getKnots (closed, order (), controlPoint () .size (), knot ());

	tessellator .nurbs_curve (knots .size (), knots .data (),
	                          3, controlPoints [0] .data (),
	                          order (),
	                          GLU_MAP1_TRIM_3);
}

} // X3D
} // titania
