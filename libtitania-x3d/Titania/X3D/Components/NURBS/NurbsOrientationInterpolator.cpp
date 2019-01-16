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

#include "NurbsOrientationInterpolator.h"

#include "../../Bits/Cast.h"
#include "../../Browser/NURBS/NURBS.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Interpolation/OrientationInterpolator.h"
#include "../Rendering/X3DCoordinateNode.h"

#include <Titania/Math/Mesh/NurbsTessellator.h>
#include <cassert>

namespace titania {
namespace X3D {

const std::string NurbsOrientationInterpolator::componentName  = "NURBS";
const std::string NurbsOrientationInterpolator::typeName       = "NurbsOrientationInterpolator";
const std::string NurbsOrientationInterpolator::containerField = "children";

const size_t NurbsOrientationInterpolator::TESSELLATION = 16;

NurbsOrientationInterpolator::Fields::Fields () :
	 set_fraction (new SFFloat ()),
	        order (new SFInt32 (3)),
	         knot (new MFDouble ()),
	       weight (new MFDouble ()),
	 controlPoint (new SFNode ()),
	value_changed (new SFRotation ())
{ }

NurbsOrientationInterpolator::NurbsOrientationInterpolator (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	          fields (),
	controlPointNode (),
	    interpolator (new OrientationInterpolator (executionContext)),
	          buffer ()
{
	addType (X3DConstants::NurbsOrientationInterpolator);

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
NurbsOrientationInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsOrientationInterpolator (executionContext);
}

void
NurbsOrientationInterpolator::initialize ()
{
	X3DChildNode::initialize ();

	order ()        .addInterest (&NurbsOrientationInterpolator::build,            this);
	knot ()         .addInterest (&NurbsOrientationInterpolator::build,            this);
	weight ()       .addInterest (&NurbsOrientationInterpolator::build,            this);
	controlPoint () .addInterest (&NurbsOrientationInterpolator::set_controlPoint, this);

	set_fraction () .addInterest (interpolator -> set_fraction ());
	interpolator -> value_changed () .addInterest (value_changed ());

	buffer .addInterest (&NurbsOrientationInterpolator::set_buffer, this);

	set_controlPoint ();

	interpolator -> setup ();
}

bool
NurbsOrientationInterpolator::getClosed (const size_t order,
                                         const std::vector <double> & knot,
                                         const std::vector <double> & weight,
                                         const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getClosed (order, knot, weight, controlPointNode);
}

std::vector <float>
NurbsOrientationInterpolator::getKnots (const bool closed,
                                        const size_t order,
                                        const size_t dimension,
                                        const std::vector <double> & knot) const
{
	return NURBS::getKnots (closed, order, dimension, knot);
}

std::vector <Vector4f>
NurbsOrientationInterpolator::getControlPoints (const bool closed,
                                                const size_t order,
                                                const std::vector <double> & weight,
                                                const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getControlPoints (closed, order, weight, controlPointNode);
}

void
NurbsOrientationInterpolator::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (this);

	build ();
}

void
NurbsOrientationInterpolator::build ()
{
	buffer .addEvent ();
}

void
NurbsOrientationInterpolator::set_buffer ()
{
	if (order () < 2)
		return;

	if (not controlPointNode)
		return;

	if (controlPointNode -> getSize () < size_t (order ()))
		return;

	// Order and dimension are now positive numbers.

	const auto closed        = getClosed (order (), knot (), weight (), controlPointNode);
	auto       controlPoints = getControlPoints (closed, order (), weight (), controlPointNode);

	// Knots

	auto       knots = getKnots (closed, order (), controlPointNode -> getSize (), knot ());
	const auto scale = knots .back () - knots .front ();

	assert ((knots .size () - order ()) == controlPointNode -> getSize ());

	// Tessellate

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, TESSELLATION * controlPointNode -> getSize () / scale);

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
		const auto direction = Vector3d (lines [i + 1] - lines [i]);
		const auto rotation  = Rotation4d (Vector3d (0, 0, 1), direction);

		interpolator -> key ()      .emplace_back (knots .front () + (float (i) / float (size - (2 * not closed))) * scale);
		interpolator -> keyValue () .emplace_back (rotation);
	}

	if (closed)
	{
		interpolator -> key ()      .emplace_back (knots .front () + scale);
		interpolator -> keyValue () .emplace_back (interpolator -> keyValue () .front ());
	}
}

NurbsOrientationInterpolator::~NurbsOrientationInterpolator ()
{ }

} // X3D
} // titania
