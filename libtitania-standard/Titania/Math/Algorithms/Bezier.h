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
	quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, Container & points);

	/**
	 * De Casteljau (bezier) algorithm contributed by Jed Soane,
	 * evaluates a cubic (third degree) curve.
	 */
	template <class Vector, class Container>
	static
	void
	cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, Container & points);

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
	           Container & points);

private:

	template <class Type>
	static
	vector2 <Type>
	arc_point (const vector2 <Type> & p0, Type rx, Type ry, Type xAxisRotation, const bool largeArcFlag, const bool sweepFlag, const vector2 <Type> & p1, const Type t);

};

template <class Vector, class Container>
void
bezier::quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, Container & points)
{
	const auto bezier_steps_1 = bezier_steps - 1;

	for (size_t i = 1; i < bezier_steps; ++ i)
	{
		const auto t = static_cast <typename Vector::value_type> (i) / bezier_steps_1;

		const auto U = (1 - t) * A + t * B;
		const auto V = (1 - t) * B + t * C;

		const auto P = (1 - t) * U + t * V;

		if (points .empty () or (P not_eq points .back () and P not_eq points .front ()))
		{
			points .emplace_back (P);
		}
	}
}

// http://ericeastwood.com/blog/25/curves-and-arcs-quadratic-cubic-elliptical-svg-implementations
template <class Vector, class Container>
void
bezier::cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, Container & points)
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

		if (points .empty () or (P not_eq points .back () and P not_eq points .front ()))
		{
			points .emplace_back (P);
		}
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
                   const size_t bezier_steps,
                   Container & points)
{
	const auto bezier_steps_1 = bezier_steps - 1;

	for (size_t i = 0; i < bezier_steps; ++ i)
	{
		const auto t = Type (i) / bezier_steps_1;

		const auto P = arc_point (p0, rx, ry, xAxisRotation, largeArcFlag, sweepFlag, p1, t);

		if (points .empty () or (P not_eq points .back () and P not_eq points .front ()))
		{
			points .emplace_back (P);
		}
	}
}

template <class Type>
vector2 <Type>
bezier::arc_point (const vector2 <Type> & p0, Type rx, Type ry, Type xAxisRotation, const bool largeArcFlag, const bool sweepFlag, const vector2 <Type> & p1, const Type t)
{
	// In accordance to: http://www.w3.org/TR/SVG/implnote.html#ArcOutOfRangeParameters

	rx = std::abs (rx);
	ry = std::abs (ry);

	xAxisRotation = interval <Type> (xAxisRotation, 0, 2 * pi <Type>);

	// If the endpoints are identical, then this is equivalent to omitting the elliptical arc segment entirely.
	if (p0 == p1)
		return p0;

	// If rx = 0 or ry = 0 then this arc is treated as a straight line segment joining the endpoints.    
	if (rx == 0 || ry == 0)
		return lerp (p0, p1, t);

	// Following "Conversion from endpoint to center parameterization"
	// http://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter
	
	// Step #1: Compute transformedPoint
	const auto d = (p0 - p1) / Type (2);

	const auto transformedPoint = vector2 <Type> (std::cos (xAxisRotation) * d .x () + std::sin (xAxisRotation) * d .y (),
		                                           std::sin (xAxisRotation) * d .x () + std::cos (xAxisRotation) * d .y ());

	// Ensure radii are large enough
	const auto radiiCheck = std::pow (transformedPoint .x (), 2) / std::pow (rx, 2) + std::pow (transformedPoint .y (), 2) / std::pow (ry, 2);
	
	if (radiiCheck > 1) {
		rx = std::sqrt (radiiCheck) * rx;
		ry = std::sqrt (radiiCheck) * ry;
	}

	// Step #2: Compute transformedCenter
	const auto cSquareNumerator = std::pow (rx, 2) * std::pow (ry, 2) - std::pow (rx, 2) * std::pow (transformedPoint .y (), 2) - std::pow (ry, 2) * std::pow (transformedPoint .x (), 2);
	const auto cSquareRootDenom = std::pow (rx, 2) * std::pow (transformedPoint .y (), 2) + std::pow (ry, 2) * std::pow (transformedPoint .x (), 2);
	auto       cRadicand        = cSquareNumerator / cSquareRootDenom;

	// Make sure this never drops below zero because of precision
	cRadicand = cRadicand < 0 ? 0 : cRadicand;

	const auto cCoef = (largeArcFlag not_eq sweepFlag ? 1 : -1) * std::sqrt (cRadicand);

	const auto transformedCenter = vector2 <Type> (cCoef * (+(rx * transformedPoint .y ()) / ry),
	                                               cCoef * (-(ry * transformedPoint .x ()) / rx));

	// Step #3: Compute center
	const auto center = vector2 <Type> (std::cos (xAxisRotation) * transformedCenter .x () - std::sin (xAxisRotation) * transformedCenter .y () + ((p0 .x () + p1 .x ()) / 2),
	                                    std::sin (xAxisRotation) * transformedCenter .x () + std::cos (xAxisRotation) * transformedCenter .y () + ((p0 .y () + p1 .y ()) / 2));

	// Step #4: Compute start/sweep angles
	// Start angle of the elliptical arc prior to the stretch and rotate operations.
	// Difference between the start and end angles
	const auto startVector = vector2 <Type> ((transformedPoint .x () - transformedCenter .x ()) / rx,
	                                         (transformedPoint .y () - transformedCenter .y ()) / ry);

	const auto startAngle = std::acos (dot (vector2 <Type> (1, 0), normalize (startVector)));

	const auto endVector = vector2 <Type> ((-transformedPoint .x () - transformedCenter .x ()) / rx,
	                                       (-transformedPoint .y () - transformedCenter .y ()) / ry);

	auto sweepAngle = std::acos (dot (normalize (startVector), normalize (endVector)));

	if (not sweepFlag and sweepAngle > 0)
		sweepAngle -= 2 * pi <Type>;

	else if (sweepFlag and sweepAngle < 0)
		sweepAngle += 2 * pi <Type>;

	// We use % instead of `mod(..)` because we want it to be -360deg to 360deg(but actually in radians)
	sweepAngle = interval (sweepAngle, -2 * pi <Type>, 2 * pi <Type>);

	// From http://www.w3.org/TR/SVG/implnote.html#ArcParameterizationAlternatives
	const auto angle             = startAngle + (sweepAngle * t);
	const auto ellipseComponentX = rx * std::cos (angle);
	const auto ellipseComponentY = ry  *std::sin (angle);

	const auto point = vector2 <Type> (std::cos (xAxisRotation) * ellipseComponentX - std::sin (xAxisRotation) * ellipseComponentY + center .x (),
	                                   std::sin (xAxisRotation) * ellipseComponentX + std::cos (xAxisRotation) * ellipseComponentY + center .y ());

	return point;
}

} // math
} // titania

#endif
