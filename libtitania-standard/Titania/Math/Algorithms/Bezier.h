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

#ifndef __TITANIA_MATH_ALGORITHMS_BEZIER_H__
#define __TITANIA_MATH_ALGORITHMS_BEZIER_H__

#include "../Numbers/Vector2.h"
#include "../Functional.h"
#include "../Utility/almost_equal.h"

#include <Titania/LOG.h>

namespace titania {
namespace math {

/**
 *  Class to represent bezier curve algorithms.
 */
class bezier
{
public:

	/**
	 * De Casteljau (bezier) algorithm contributed by Jed Soane,
	 * evaluates a quadratic or conic (second degree) curve.
	 */
	template <class Vector, class Container>
	static
	void
	quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, const typename Vector::value_type & tolerance, Container & points);

	/**
	 * De Casteljau (bezier) algorithm contributed by Jed Soane,
	 * evaluates a cubic (third degree) curve.
	 */
	template <class Vector, class Container>
	static
	void
	cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, const typename Vector::value_type & tolerance, Container & points);

	/**
	 * SVG arc curve.
	 */
	template <class Type, class Container>
	static
	void
	arc_curve (const vector2 <Type> & p0,
	           Type rx,
	           Type ry,
	           Type xAxisRotation,
	           const bool largeArcFlag,
	           const bool sweepFlag,
	           const vector2 <Type> & p1,
	           const size_t bezier_steps,
	           const Type & tolerance,
	           Container & points);


private:

	template <class Vector, class Container>
	static
	void
	add_point (const Vector & point, const typename Vector::value_type & tolerance, Container & points);

};

template <class Vector, class Container>
void
bezier::quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, const typename Vector::value_type & tolerance, Container & points)
{
	const auto bezier_steps_1 = bezier_steps - 1;

	for (size_t i = 1; i < bezier_steps; ++ i)
	{
		const auto t = static_cast <typename Vector::value_type> (i) / bezier_steps_1;

		const auto U = (1 - t) * A + t * B;
		const auto V = (1 - t) * B + t * C;
		const auto P = (1 - t) * U + t * V;

		add_point (P, tolerance, points);
	}
}

// http://ericeastwood.com/blog/25/curves-and-arcs-quadratic-cubic-elliptical-svg-implementations
template <class Vector, class Container>
void
bezier::cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, const typename Vector::value_type & tolerance, Container & points)
{
	const auto bezier_steps_1 = bezier_steps - 1;

	for (size_t i = 0; i < bezier_steps; ++ i)
	{
		const auto t = static_cast <typename Vector::value_type> (i) / bezier_steps_1;

		const auto U = (1 - t) * A + t * B;
		const auto V = (1 - t) * B + t * C;
		const auto W = (1 - t) * C + t * D;

		const auto M = (1 - t) * U + t * V;
		const auto N = (1 - t) * V + t * W;
		const auto P = (1 - t) * M + t * N;

		add_point (P, tolerance, points);
	}
}

template <class Type, class Container>
void
bezier::arc_curve (const vector2 <Type> & p0,
                   Type rx,
                   Type ry,
                   Type xAxisRotation,
                   const bool largeArcFlag,
                   const bool sweepFlag,
                   const vector2 <Type> & p1,
                   const size_t circle_dimension,
                   const Type & tolerance,
                   Container & points)
{
	// https://ericeastwood.com/blog/25/curves-and-arcs-quadratic-cubic-elliptical-svg-implementations
	// See https://www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes.

	// If the endpoints are identical, then this is equivalent to omitting the elliptical arc segment entirely.
	if (p0 == p1)
	{	
		add_point (p0, tolerance, points);
		return;
	}

	// In accordance to: http://www.w3.org/TR/SVG/implnote.html#ArcOutOfRangeParameters

	rx = std::abs (rx);
	ry = std::abs (ry);

	// If rx = 0 or ry = 0 then this arc is treated as a straight line segment joining the endpoints.    
	if (rx == 0 || ry == 0)
	{	
		add_point (p0, tolerance, points);
		add_point (p1, tolerance, points);
		return;
	}

	const auto rx2 = rx * rx;
	const auto ry2 = ry * ry;

	// In accordance to: http://www.w3.org/TR/SVG/implnote.html#ArcOutOfRangeParameters

	xAxisRotation = interval <Type> (xAxisRotation, 0, 2 * pi <Type>);

	const auto sinRotation = std::sin (xAxisRotation);
	const auto cosRotation = std::cos (xAxisRotation);

	// Following "Conversion from endpoint to center parameterization"
	// http://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter
	
	// Step #1: Compute transformedPoint
	const auto d = (p0 - p1) / Type (2);

	const auto transformedPoint = vector2 <Type> ( cosRotation * d .x () + sinRotation * d .y (),
		                                           -sinRotation * d .x () + cosRotation * d .y ());

	const auto transformedPoint2 = transformedPoint * transformedPoint;

	// Ensure radii are large enough
	const auto radiiCheck = transformedPoint2 .x () / rx2 + transformedPoint2 .y () / ry2;
	
	if (radiiCheck > 1)
	{
		rx = std::sqrt (radiiCheck) * rx;
		ry = std::sqrt (radiiCheck) * ry;
	}

	// Step #2: Compute transformedCenter
	const auto cSquareNumerator = rx2 * ry2 - rx2 * transformedPoint2 .y () - ry2 * transformedPoint2 .x ();
	const auto cSquareRootDenom =             rx2 * transformedPoint2 .y () + ry2 * transformedPoint2 .x ();
	auto       cRadicand        = cSquareNumerator / cSquareRootDenom;

	// Make sure this never drops below zero because of precision
	cRadicand = cRadicand < 0 ? 0 : cRadicand;

	const auto cCoef = (largeArcFlag not_eq sweepFlag ? 1 : -1) * std::sqrt (cRadicand);

	const auto transformedCenter = vector2 <Type> ( cCoef * rx * transformedPoint .y () / ry,
	                                               -cCoef * ry * transformedPoint .x () / rx);

	// Step #3: Compute center
	const auto center = vector2 <Type> (cosRotation * transformedCenter .x () - sinRotation * transformedCenter .y () + ((p0 .x () + p1 .x ()) / 2),
	                                    sinRotation * transformedCenter .x () + cosRotation * transformedCenter .y () + ((p0 .y () + p1 .y ()) / 2));

	// Step #4: Compute start/sweep angles
	const auto startVector = vector2 <Type> ((transformedPoint .x () - transformedCenter .x ()) / rx,
	                                         (transformedPoint .y () - transformedCenter .y ()) / ry);

	const auto endVector = vector2 <Type> ((-transformedPoint .x () - transformedCenter .x ()) / rx,
	                                       (-transformedPoint .y () - transformedCenter .y ()) / ry);

	const auto get_angle  = [ ] (double x) { return x > 0 ? x : 2 * pi <Type> + x; }; // transform angle to range [0, 2pi]
	const auto startAngle = get_angle (std::atan2 (startVector .y (), startVector .x ()));
	const auto endAngle   = get_angle (std::atan2 (endVector   .y (), endVector   .x ()));

	auto sweepAngle = endAngle - startAngle;

	if (largeArcFlag)
	{
		// sweepAngle must be positive
		if (sweepAngle < 0)
			sweepAngle += 2 * pi <Type>;
	}
	else
	{
		// sweepAngle must be negative
		if (sweepAngle > 0)
			sweepAngle -= 2 * pi <Type>;
	}

	if (not sweepFlag and sweepAngle > 0)
		sweepAngle -= 2 * pi <Type>;

	else if (sweepFlag and sweepAngle < 0)
		sweepAngle += 2 * pi <Type>;

	// Interpolate:

	const auto bezier_steps   = std::max <int32_t> (4, std::abs (sweepAngle) * circle_dimension / (2 * pi <Type>));
	const auto bezier_steps_1 = bezier_steps - 1;

	add_point (p0, tolerance, points);

	for (int32_t i = 1; i < bezier_steps_1; ++ i)
	{
		const auto t = Type (i) / bezier_steps_1;

		// From http://www.w3.org/TR/SVG/implnote.html#ArcParameterizationAlternatives
		const auto angle = startAngle + (sweepAngle * t);
		const auto x     = rx * std::cos (angle);
		const auto y     = ry * std::sin (angle);
	
		const auto point = vector2 <Type> (cosRotation * x - sinRotation * y + center .x (),
		                                   sinRotation * x + cosRotation * y + center .y ());

		add_point (point, tolerance, points);
	}

	add_point (p1, tolerance, points);
}

template <class Vector, class Container>
inline
void
bezier::add_point (const Vector & point, const typename Vector::value_type & tolerance, Container & points)
{
	if (points .empty () or (not almost_equal (point, points .back (), tolerance) and not almost_equal (point, points .front (), tolerance)))
	{
		points .emplace_back (point);
	}
}

} // math
} // titania

#endif
