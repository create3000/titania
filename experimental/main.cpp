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

//#include <Titania/Basic/Geometry/Line3.h>

#include "URI.h"
#include <Titania/Algorithm/Remove.h>
#include <Titania/Basic/Path.h>
#include <Titania/Basic/URI.h>
#include <Titania/Chrono/Now.h>
#include <Titania/Geospatial/Geodetic.h>
#include <Titania/Geospatial/ReferenceEllipsoids.h>
#include <Titania/Geospatial/UniversalTransverseMercator.h>
#include <Titania/InputOutput.h>
#include <Titania/Math/Functional.h>
#include <Titania/Math/Geometry/Box2.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Cylinder3.h>
#include <Titania/Math/Geometry/Line2.h>
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Geometry/Spheroid3.h>
#include <Titania/Math/Geometry/Triangle3.h>
#include <Titania/Math/Geometry/ConvexHull2.h>
#include <Titania/Math/Geometry/ConvexHull3.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/Math/Utility/almost_equal.h>
#include <Titania/OS.h>
#include <Titania/Stream/InputFileStream.h>
#include <Titania/Stream/InputUrlStream.h>
#include <Titania/Utility/Pass.h>
#include <Titania/String.h>
//#include <Titania/Stream/InputHTTPStream.h>

#include <Titania/Algorithm.h>
#include <Titania/LOG.h>

#include <array>
#include <cassert>
#include <complex>
#include <ctime>
#include <deque>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <giomm.h>

#include <cstdarg>

#include <Titania/String/dtoa.h>

using namespace titania;
using namespace titania::basic;
using namespace titania::math;

using Quaternionf = math::quaternion <float>;
using Vector2f    = math::vector2 <float>;
using Vector2d    = math::vector2 <double>;
using Vector3d    = math::vector3 <double>;
using Vector3f    = math::vector3 <float>;
using Vector4f    = math::vector4 <float>;
using Rotation4d  = math::rotation4 <double>;
using Rotation4f  = math::rotation4 <float>;
using Box2f       = math::box2 <float>;
using Box3f       = math::box3 <float>;
using Box3d       = math::box3 <double>;
using Cylinder3f  = math::cylinder3 <float>;
using Plane3f     = math::plane3 <float>;
using Line2d      = math::line2 <double>;
using Line2f      = math::line2 <float>;
using Line3d      = math::line3 <double>;
using Line3f      = math::line3 <float>;
using Sphere3f    = math::sphere3 <float>;
using Matrix3f    = math::matrix3 <float>;
using Matrix4d    = math::matrix4 <double>;
using Matrix4f    = math::matrix4 <float>;
using Spheroid3d  = math::spheroid3 <double>;
using ConvexHull2d = math::convex_hull2 <double>;
using ConvexHull3d = math::convex_hull3 <double>;

template <class Type>
math::box2 <Type>
minimum_bounding_rectangle (const std::vector <vector2 <Type>> & convex_hull)
{
	math::box2 <Type> rectangle;

	// Find most left and most right points.

	const auto size = convex_hull .size ();

	const auto resultX = std::minmax_element (convex_hull .begin (),
	                                          convex_hull .end (),
	                                          [] (const vector2 <Type> & a, const vector2 <Type> & b)
                                             { return a .x () < b .x (); });

	// Find most lower and most upper points.

	const auto resultY = std::minmax_element (convex_hull .begin (),
	                                          convex_hull .end (),
	                                          [] (const vector2 <Type> & a, const vector2 <Type> & b)
                                             { return a .y () < b .y (); });

	std::vector <size_t> indices; // Index of hull point on line.

	indices .emplace_back (resultY .second - convex_hull .begin ());
	indices .emplace_back (resultX .second - convex_hull .begin ());
	indices .emplace_back (resultY .first  - convex_hull .begin ());
	indices .emplace_back (resultX .first  - convex_hull .begin ());

	// Construct initial bounding rectangle lines.

	//          a
	//    <------------
	//    |           |
	//  d |           | b
	//    |           |
	//    ------------>
	//          c

	const auto yMax = convex_hull [indices [0]] .y ();
	const auto xMax = convex_hull [indices [1]] .x ();
	const auto yMin = convex_hull [indices [2]] .y ();
	const auto xMin = convex_hull [indices [3]] .x ();

	std::vector <line2 <Type>> lines;

	lines .emplace_back (vector2 <Type> (xMax, yMax), vector2 <Type> (xMin, yMax), point_type ());
	lines .emplace_back (vector2 <Type> (xMax, yMin), vector2 <Type> (xMax, yMax), point_type ());
	lines .emplace_back (vector2 <Type> (xMin, yMin), vector2 <Type> (xMax, yMin), point_type ());
	lines .emplace_back (vector2 <Type> (xMin, yMax), vector2 <Type> (xMin, yMin), point_type ());


	//for (size_t i = 0; i < 4; ++ i)
	//	std::clog << indices [i] << " ";
	//std::clog << " : ";

	//const Type xSize = lines [0] .distance (convex_hull [indices [2]]);
	//const Type ySize = lines [1] .distance (convex_hull [indices [3]]);
	//std::clog << xSize * ySize << std::endl;

	// Rotate lines about vertex with the smalles angle to the next vertex.

	Type min_area = std::numeric_limits <Type>::infinity ();
	Type rotation = 0;

	while (rotation < M_PI)
	{
		Type max_cos_theta = -std::numeric_limits <Type>::infinity ();
		int  maxIndex      = -1;

		for (size_t i = 0; i < 4; ++ i)
		{
			const auto & A = convex_hull [indices [i]];
			const auto & B = convex_hull [(indices [i] + 1) % size];
	
			const auto cos_theta = dot (lines [i] .direction (), normalize (B - A));
	
			if (cos_theta > max_cos_theta)
			{
				max_cos_theta = cos_theta;
				maxIndex      = i;
			}
		}

		// Rotate lines.

		const auto theta = std::acos (clamp <Type> (max_cos_theta, 0, 1));

		matrix3 <Type> matrix;

		for (size_t i = 0; i < 4; ++ i)
		{
			matrix .set (vector2 <Type> (), theta, vector2 <Type> (1, 1), Type (0), convex_hull [indices [i]]);

			lines [i] .mult_line_matrix (matrix);
		}

		rotation += theta;

		// Assign next point to line with closest angle.

		indices [maxIndex] = (indices [maxIndex] + 1) % size;

		//for (size_t i = 0; i < 4; ++ i)
		//	std::clog << indices [i] << " ";
		//std::clog << " : ";

		auto       xAxis = lines [0] .perpendicular_vector (convex_hull [indices [2]]);
		auto       yAxis = lines [1] .perpendicular_vector (convex_hull [indices [3]]);
		const Type area  = abs (xAxis) * abs (yAxis);

		if (area < min_area)
		{
			min_area = area;

			// Determine center.

			vector2 <Type> A, B;

			lines [0] .closest_point (lines [1], A);
			lines [2] .closest_point (lines [3], B);

			const auto center = (A + B) / Type (2);

			// Build minimum area rectangle.

			xAxis /= Type (2);
			yAxis /= Type (2);

			rectangle .matrix (matrix3 <Type> (xAxis  .x (), xAxis  .y (), 0,
			                                   yAxis  .x (), yAxis  .y (), 0,
			                                   center .x (), center .y (), 1));
		}

		//std::clog << area << std::endl;
	}

	//std::clog << std::endl;

	return rectangle;
}

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;
	std::clog << std::boolalpha;
	std::clog << std::setprecision (std::numeric_limits <float>::digits10);
	//std::clog << std::setprecision (std::numeric_limits <double>::digits10);
	std::clog .imbue (std::locale (""));

	std::locale::global (std::locale (""));

	#ifdef _GLIBCXX_PARALLEL
	std::clog << "in parallel mode ..." << std::endl;
	#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static std::default_random_engine
	random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

	std::uniform_real_distribution <double> random (0, 1);

	std::vector <Vector2d> points = {
		Vector2d (0, 0),
		Vector2d (4, 0),
		Vector2d (7, -1),
		Vector2d (7, 5),
		Vector2d (2, 2),
		Vector2d (0, 4),
		Vector2d (2, 7),
//		Vector2d (9, 8),
	};

	points .clear ();

	for (size_t i = 0; i < 10000000; ++ i)
		points .emplace_back (random (random_engine), random (random_engine));

	const auto t0 = chrono::now ();

	const auto hull = ConvexHull2d (points);

	std::clog << chrono::now () - t0 << std::endl;

	for (const auto & i : hull .indices ())
	   std::clog << i << std::endl;

	const auto rectangle = minimum_bounding_rectangle (hull .polygon ());

	std::clog << std::endl;
	std::clog << rectangle .area () << std::endl;
	std::clog << rectangle .matrix () << std::endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
