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

#include "NURBS.h"

#include "../../Components/Rendering/X3DCoordinateNode.h"

namespace titania {
namespace X3D {

size_t
NURBS::getTessellation (const size_t tessellation, const size_t dimension)
{
	// Return a tessellation one less to the standard to get the right tessellation.

	if (tessellation > 0)
		return tessellation + 1;
		
	if (tessellation < 0)
		return -tessellation * dimension + 1;

	return 2 * dimension + 1;
}

bool
NURBS::isPeriodic (const size_t order,
                   const size_t dimension,
                   const std::vector <double> & knot)
{
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

bool
NURBS::getClosed (const size_t order,
                  const std::vector <double> & knot,
                  const std::vector <double> & weight,
                  const std::vector <Vector2d> & controlPoint)
{
	const auto dimension   = controlPoint .size ();
	const auto haveWeights = weight .size () == dimension;

	// Check if first and last weights are unitary.

	if (haveWeights)
	{
		if (weight .front () not_eq weight .back ())
			return false;
	}

	// Check if first and last point are coincident.

	if (controlPoint .front () not_eq controlPoint .back ())
		return false;

	// Check if knots are periodic.

	if (not isPeriodic (order, dimension, knot))
		return false;

	return true;
}

bool
NURBS::getClosed (const size_t order,
                  const std::vector <double> & knot,
                  const std::vector <double> & weight,
                  const X3DPtr <X3DCoordinateNode> & controlPointNode)
{
	const auto dimension   = controlPointNode -> getSize ();
	const auto haveWeights = weight .size () == dimension;

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

	if (not isPeriodic (order, dimension, knot))
		return false;

	return true;
}

bool
NURBS::getUClosed (const size_t uOrder,
                   const size_t uDimension,
                   const size_t vDimension,
                   const std::vector <double> & uKnot,
                   const std::vector <double> & weight,
                   const X3DPtr <X3DCoordinateNode> & controlPointNode)
{
	const auto haveWeights = weight .size () == controlPointNode -> getSize ();

	for (size_t v = 0, size = vDimension; v < size; ++ v)
	{
		const auto first = v * uDimension;
		const auto last  = v * uDimension + uDimension - 1;

		// Check if first and last weights are unitary.

		if (haveWeights)
		{
			if (weight [first] not_eq weight [last])
				return false;
		}

		// Check if first and last point are coincident.

		if (controlPointNode -> get1Point (first) not_eq controlPointNode -> get1Point (last))
			return false;
	}

	// Check if knots are periodic.

	if (not isPeriodic (uOrder, uDimension, uKnot))
		return false;

	return true;
}

bool
NURBS::getVClosed (const size_t vOrder,
                   const size_t uDimension,
                   const size_t vDimension,
                   const std::vector <double> & vKnot,
                   const std::vector <double> & weight,
                   const X3DPtr <X3DCoordinateNode> & controlPointNode)
{
	const auto haveWeights = weight .size () == controlPointNode -> getSize ();

	for (size_t u = 0, size = uDimension; u < size; ++ u)
	{
		const auto first = u;
		const auto last  = (vDimension - 1) * uDimension + u;

		// Check if first and last weights are unitary.

		if (haveWeights)
		{
			if (weight [first] not_eq weight [last])
				return false;
		}

		// Check if first and last point are coincident.

		if (controlPointNode -> get1Point (first) not_eq controlPointNode -> get1Point (last))
			return false;
	}

	// Check if knots are periodic.

	if (not isPeriodic (vOrder, vDimension, vKnot))
		return false;

	return true;
}

std::vector <float>
NURBS::getKnots (const bool closed,
                 const size_t order,
                 const size_t dimension,
                 const std::vector <double> & knot)
{
	std::vector <float> knots (knot .cbegin (), knot .cend ());

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.

	bool generateUniform = true;

	if (knots .size () == size_t (dimension + order))
	{
		generateUniform = false;

		size_t consecutiveKnots = 0;

		for (size_t i = 1, size = knots .size (); i < size; ++ i)
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
NURBS::getControlPoints (const bool closed,
                         const size_t order,
                         const std::vector <double> & weight,
                         const std::vector <Vector2d> & controlPoint)
{
	std::vector <Vector3f> controlPoints;

	const auto dimension = controlPoint .size ();

	if (weight .size () not_eq dimension)
	{
		for (size_t i = 0; i < dimension; ++ i)
		{
			const auto & p = controlPoint [i];

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
			const auto & p = controlPoint [i];

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

std::vector <Vector4f>
NURBS::getControlPoints (const bool closed,
                         const size_t order,
                         const std::vector <double> & weight,
                         const X3DPtr <X3DCoordinateNode> & controlPointNode)
{
	std::vector <Vector4f> controlPoints;

	const auto dimension = controlPointNode -> getSize ();

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

std::vector <Vector4f>
NURBS::getControlPoints (const bool uClosed,
                         const bool vClosed,
                         const size_t uOrder,
                         const size_t vOrder,
                         const size_t uDimension,
                         const size_t vDimension,
                         const std::vector <double> & weight,
                         const X3DPtr <X3DCoordinateNode> & controlPointNode)
{
	std::vector <Vector4f> controlPoints;

	if (weight .size () not_eq controlPointNode -> getSize ())
	{
		for (size_t v = 0, i = 0; v < vDimension; ++ v)
		{
			for (size_t u = 0; u < uDimension; ++ u, ++ i)
			{
				const auto p = controlPointNode -> get1Point (i);

				controlPoints .emplace_back (p .x (),
				                             p .y (),
				                             p .z (),
				                             1);
			}

			if (uClosed)
			{
				const auto first = controlPoints .size () - uDimension;

				for (size_t i = 1, size = uOrder - 1; i < size; ++ i)
					controlPoints .emplace_back (controlPoints [first + i]);
			}
		}

		if (vClosed)
		{
			for (size_t i = (uDimension + uClosed * (uOrder - 2)), size = (uDimension + uClosed * (uOrder - 2)) * (vOrder - 1); i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}
	else
	{
		for (size_t v = 0, i = 0; v < vDimension; ++ v)
		{
			for (size_t u = 0; u < uDimension; ++ u, ++ i)
			{
				const auto p = controlPointNode -> get1Point (i);

				controlPoints .emplace_back (p .x (),
				                             p .y (),
				                             p .z (),
				                             weight [i]);
			}

			if (uClosed)
			{
				const auto first = controlPoints .size () - uDimension;

				for (size_t i = 1, size = uOrder - 1; i < size; ++ i)
					controlPoints .emplace_back (controlPoints [first + i]);
			}
		}

		if (vClosed)
		{
			for (size_t i = (uDimension + uClosed * (uOrder - 2)), size = (uDimension + uClosed * (uOrder - 2)) * (vOrder - 1); i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}

	return controlPoints;
}

} // X3D
} // titania
