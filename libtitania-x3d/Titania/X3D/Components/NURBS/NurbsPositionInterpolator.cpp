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

#include "NurbsPositionInterpolator.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Interpolation/PositionInterpolator.h"
#include "../Rendering/X3DCoordinateNode.h"

#include <Titania/Math/Mesh/NurbsTessellator.h>
#include <cassert>

namespace titania {
namespace X3D {

const std::string NurbsPositionInterpolator::componentName  = "NURBS";
const std::string NurbsPositionInterpolator::typeName       = "NurbsPositionInterpolator";
const std::string NurbsPositionInterpolator::containerField = "children";

NurbsPositionInterpolator::Fields::Fields () :
	 set_fraction (new SFFloat ()),
	        order (new SFInt32 (3)),
	         knot (new MFDouble ()),
	       weight (new MFDouble ()),
	 controlPoint (new SFNode ()),
	value_changed (new SFVec3f ())
{ }

NurbsPositionInterpolator::NurbsPositionInterpolator (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	          fields (),
	controlPointNode (),
	    interpolator (new PositionInterpolator (executionContext)),
	          buffer ()
{
	addType (X3DConstants::NurbsPositionInterpolator);

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOnly,   "set_fraction",  set_fraction ());
	addField (inputOutput, "order",         order ());
	addField (inputOutput, "knot",          knot ());
	addField (inputOutput, "weight",        weight ());
	addField (inputOutput, "controlPoint",  controlPoint ());
	addField (outputOnly,  "value_changed", value_changed ());

	addChildObjects (controlPointNode,
	                 interpolator,
	                 buffer);
}

X3DBaseNode*
NurbsPositionInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsPositionInterpolator (executionContext);
}

void
NurbsPositionInterpolator::initialize ()
{
	X3DChildNode::initialize ();

	order ()        .addInterest (&NurbsPositionInterpolator::build,            this);
	knot ()         .addInterest (&NurbsPositionInterpolator::build,            this);
	weight ()       .addInterest (&NurbsPositionInterpolator::build,            this);
	controlPoint () .addInterest (&NurbsPositionInterpolator::set_controlPoint, this);

	set_fraction () .addInterest (interpolator -> set_fraction ());
	interpolator -> value_changed () .addInterest (value_changed ());

	buffer .addInterest (&NurbsPositionInterpolator::set_buffer, this);

	set_controlPoint ();

	interpolator -> setup ();
}

bool
NurbsPositionInterpolator::getClosed (const size_t order,
                                      const size_t dimension,
                                      const std::vector <double> & knot,
                                      const std::vector <double> & weight) const
{
	const auto haveWeights = weight .size () == controlPointNode -> getSize ();

	// Check if first and last weights are unitary.

	if (haveWeights)
	{
		if (weight .front () not_eq weight .back ())
			return false;
	}

	// Check if first and last point are coincident.

	if (controlPointNode -> get1Point (0) not_eq controlPointNode -> get1Point (dimension - 1))
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
NurbsPositionInterpolator::getKnots (const bool closed,
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

std::vector <Vector4f>
NurbsPositionInterpolator::getControlPoints (const bool closed,
                                             const size_t order,
                                             const size_t dimension,
                                             const std::vector <double> & weight) const
{
	std::vector <Vector4f> controlPoints;

	if (weight .size () not_eq dimension)
	{
		for (size_t i = 0; i < dimension; ++ i)
		{
			const auto p = controlPointNode -> get1Point (i);

			controlPoints .emplace_back (p .x (),
			                             p .y (),
			                             p .z (),
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
			const auto p = controlPointNode -> get1Point (i);

			controlPoints .emplace_back (p .x (),
			                             p .y (),
			                             p .z (),
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

void
NurbsPositionInterpolator::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (this);

	build ();
}

void
NurbsPositionInterpolator::build ()
{
	buffer .addEvent ();
}

void
NurbsPositionInterpolator::set_buffer ()
{
	if (order () < 2)
		return;

	if (not controlPointNode)
		return;

	if (controlPointNode -> getSize () < size_t (order ()))
		return;

	// Order and dimension are now positive numbers.

	const auto dimension     = controlPointNode -> getSize ();
	const auto closed        = getClosed (order (), dimension, knot (), weight ());
	auto       controlPoints = getControlPoints (closed, order (), dimension, weight ());

	// Knots

	auto       knots = getKnots (closed, order (), dimension, knot ());
	const auto scale = knots .back () - knots .front ();

	assert ((knots .size () - order ()) == dimension);

	// Tessellate

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, 16 * dimension / scale);

	tessellator .begin_curve ();

	tessellator .nurbs_curve (knots .size (), knots .data (),
	                          4, controlPoints [0] .data (),
	                          order (),
	                          GL_MAP1_VERTEX_4);

	tessellator .end_curve ();

	// End tessellation

	// Lines

	const auto & lines = tessellator .lines ();

	interpolator -> key ()      .clear ();
	interpolator -> keyValue () .clear ();

	for (size_t i = 0, size = lines .size (); i < size; i += 2)
	{
		interpolator -> key ()      .emplace_back (knots .front () + (float (i) / float (size)) * scale);
		interpolator -> keyValue () .emplace_back (lines [i]);
	}

	if (closed)
	{
		interpolator -> key ()      .emplace_back (knots .front () + scale);
		interpolator -> keyValue () .emplace_back (lines .front ());
	}
	else
	{
		interpolator -> key ()      .emplace_back (knots .front () + scale);
		interpolator -> keyValue () .emplace_back (lines .back ());
	}
}

NurbsPositionInterpolator::~NurbsPositionInterpolator ()
{ }

} // X3D
} // titania
