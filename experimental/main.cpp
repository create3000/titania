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
#include <Titania/Math/Geometry/MinimumBoundingBox.h>
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



#include <Titania/External/ApproxMVBB/ComputeApproxMVBB.hpp>



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

void
c2 ()
{
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

	__LOG__ << chrono::now () - t0 << std::endl;

//	std::clog << "indices" << std::endl;
//
//	for (const auto & i : hull .indices ())
//	   std::clog << i << std::endl;
//
//	std::clog << hull .indices () .front () << std::endl;

//	std::clog << "points" << std::endl;
//
//	for (const auto & p : points)
//	   std::clog << p << " 0" << std::endl;

	const auto rectangle = minimum_bounding_rectangle (points);

	std::clog << "minimum_bounding_rectangle" << std::endl;
	std::clog << rectangle .area () << std::endl;
	std::clog << rectangle .matrix () << std::endl;

	Vector2d t, s;
	double r, so;

	rectangle .matrix () .get (t, r, s, so);

	std::clog << "translation " << t << " 0" << std::endl;
	std::clog << "rotation 0 0 1 " << r << std::endl;
	std::clog << "scale " << s << " 1" << std::endl;
	std::clog << "scaleOrientation 0 0 1 " << so << std::endl;
}

void
c3 ()
{
	static std::default_random_engine
	random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

	std::uniform_real_distribution <double> random (0, 1);

	std::vector <Vector3d> points = {
		Vector3d ( 0,  0,  0),
		Vector3d (10,  1,  0),
		Vector3d ( 5,  0, 10),
		Vector3d ( 5, 10,  5),
		Vector3d ( 5,  5,  5),
		Vector3d ( 9,  9,  1),
	};

	points .clear ();

	for (size_t i = 0; i < 10; ++ i)
		points .emplace_back (random (random_engine), random (random_engine), random (random_engine));

//	points .clear ();
//	points .emplace_back (0.299762, 0.112957, 0.925728);
//	points .emplace_back (0.845464, 0.875754, 0.825147);
//	points .emplace_back (0.278331, 0.850379, 0.394512);
//	points .emplace_back (0.722046, 0.0514562, 0.786488);
//	points .emplace_back (0.620087, 0.634706, 0.852658);
//	points .emplace_back (0.87786, 0.136387, 0.518589);
//	points .emplace_back (0.667547, 0.299141, 0.37488);
//	points .emplace_back (0.434318, 0.808717, 0.109771);
//	points .emplace_back (0.0413684, 0.28985, 0.233673);
//	points .emplace_back (0.0440758, 0.547199, 0.712377);
//	points .emplace_back (0.254143, 0.587672, 0.537923);
//	points .emplace_back (0.512051, 0.0926852, 0.868097);
//	points .emplace_back (0.944505, 0.0608654, 0.408236);
//	points .emplace_back (0.0816362, 0.686754, 0.404032);
//	points .emplace_back (0.479898, 0.583161, 0.937021);
//	points .emplace_back (0.490455, 0.527928, 0.956052);
//	points .emplace_back (0.184698, 0.456343, 0.252178);
//	points .emplace_back (0.790426, 0.300747, 0.340217);
//	points .emplace_back (0.220696, 0.875444, 0.0236178);
//	points .emplace_back (0.4874, 0.866877, 0.362435);
//	points .emplace_back (0.512323, 0.198834, 0.66448);
//	points .emplace_back (0.73467, 0.605727, 0.597446);
//	points .emplace_back (0.678826, 0.692204, 0.0191986);
//	points .emplace_back (0.133617, 0.166842, 0.095324);
//	points .emplace_back (0.935348, 0.513842, 0.847092);
//	points .emplace_back (0.161007, 0.349554, 0.840331);
//	points .emplace_back (0.332821, 0.199886, 0.449233);
//	points .emplace_back (0.918002, 0.733274, 0.00402979);
//	points .emplace_back (0.917489, 0.694442, 0.683605);
//	points .emplace_back (0.973403, 0.552721, 0.822822);
//	points .emplace_back (0.688724, 0.97242, 0.325954);
//	points .emplace_back (0.653919, 0.421424, 0.451289);
//	points .emplace_back (0.10771, 0.853436, 0.584074);
//	points .emplace_back (0.957254, 0.947965, 0.841273);
//	points .emplace_back (0.289764, 0.307012, 0.939318);
//	points .emplace_back (0.443442, 0.226953, 0.135272);
//	points .emplace_back (0.840547, 0.495168, 0.0303948);
//	points .emplace_back (0.85049, 0.174303, 0.5736);
//	points .emplace_back (0.0482315, 0.30827, 0.340197);
//	points .emplace_back (0.260099, 0.847352, 0.350623);
//	points .emplace_back (0.298535, 0.356953, 0.736569);
//	points .emplace_back (0.167938, 0.154554, 0.642529);
//	points .emplace_back (0.778559, 0.187809, 0.191166);
//	points .emplace_back (0.0483332, 0.0812443, 0.977055);
//	points .emplace_back (0.995842, 0.938725, 0.647341);
//	points .emplace_back (0.0234194, 0.603708, 0.851963);
//	points .emplace_back (0.121549, 0.760695, 0.199242);
//	points .emplace_back (0.0569826, 0.0974144, 0.351115);
//	points .emplace_back (0.0777417, 0.87356, 0.0648838);
//	points .emplace_back (0.907207, 0.317932, 0.0104374);
//	points .emplace_back (0.487803, 0.353859, 0.709807);
//	points .emplace_back (0.12795, 0.681203, 0.65556);
//	points .emplace_back (0.6278, 0.277555, 0.569901);
//	points .emplace_back (0.595975, 0.10264, 0.723581);
//	points .emplace_back (0.391178, 0.868147, 0.704141);
//	points .emplace_back (0.553298, 0.568055, 0.667811);
//	points .emplace_back (0.8273, 0.0561591, 0.887006);
//	points .emplace_back (0.161191, 0.982374, 0.250708);
//	points .emplace_back (0.0732191, 0.193945, 0.776326);
//	points .emplace_back (0.364302, 0.499529, 0.0420523);
//	points .emplace_back (0.41539, 0.891428, 0.802893);
//	points .emplace_back (0.642932, 0.200814, 0.172055);
//	points .emplace_back (0.721282, 0.198793, 0.676477);
//	points .emplace_back (0.734384, 0.634168, 0.485145);
//	points .emplace_back (0.717689, 0.191748, 0.982694);
//	points .emplace_back (0.761511, 0.152149, 0.194897);
//	points .emplace_back (0.686227, 0.146546, 0.462261);
//	points .emplace_back (0.911642, 0.607058, 0.965587);
//	points .emplace_back (0.812723, 0.652217, 0.693736);
//	points .emplace_back (0.84787, 0.542965, 0.776714);
//	points .emplace_back (0.916868, 0.1618, 0.671124);
//	points .emplace_back (0.435471, 0.0843187, 0.0181375);
//	points .emplace_back (0.186674, 0.262721, 0.0297853);
//	points .emplace_back (0.535308, 0.292771, 0.522125);
//	points .emplace_back (0.821577, 0.287666, 0.483793);
//	points .emplace_back (0.698163, 0.844771, 0.287069);
//	points .emplace_back (0.61581, 0.603116, 0.573858);
//	points .emplace_back (0.435521, 0.286786, 0.687748);
//	points .emplace_back (0.37651, 0.579673, 0.351395);
//	points .emplace_back (0.368372, 0.669104, 0.698033);
//	points .emplace_back (0.621409, 0.286805, 0.103234);
//	points .emplace_back (0.481825, 0.327171, 0.587572);
//	points .emplace_back (0.733257, 0.798039, 0.277687);
//	points .emplace_back (0.777426, 0.900101, 0.195861);
//	points .emplace_back (0.510813, 0.189559, 0.017044);
//	points .emplace_back (0.671469, 0.82914, 0.685163);
//	points .emplace_back (0.427982, 0.528701, 0.827541);
//	points .emplace_back (0.33837, 0.712342, 0.147975);
//	points .emplace_back (0.600209, 0.0997037, 0.457652);
//	points .emplace_back (0.473929, 0.110897, 0.29496);
//	points .emplace_back (0.119914, 0.222093, 0.0416803);
//	points .emplace_back (0.648538, 0.0237108, 0.0673029);
//	points .emplace_back (0.346485, 0.924598, 0.209065);
//	points .emplace_back (0.472286, 0.608929, 0.903441);
//	points .emplace_back (0.0321624, 0.310906, 0.986967);
//	points .emplace_back (0.216268, 0.665765, 0.864416);
//	points .emplace_back (0.311015, 0.0713416, 0.00101028);
//	points .emplace_back (0.841774, 0.304211, 0.340616);
//	points .emplace_back (0.113851, 0.991539, 0.828853);
//	points .emplace_back (0.111647, 0.710093, 0.615291);

//	points .clear ();
//	points .emplace_back (0.629167, 0.339149, 0.139768);
//	points .emplace_back (0.406852, 0.929386, 0.937681);
//	points .emplace_back (0.436667, 0.316243, 0.843655);
//	points .emplace_back (0.423373, 0.250087, 0.327245);
//	points .emplace_back (0.272361, 0.174687, 0.387482);
//	points .emplace_back (0.664399, 0.439624, 0.617736);
//	points .emplace_back (0.1147, 0.640273, 0.801625);
//	points .emplace_back (0.948225, 0.44755, 0.805848);
//	points .emplace_back (0.707887, 0.367739, 0.526084);
//	points .emplace_back (0.802713, 0.605654, 0.556566);


	const auto hull = ConvexHull3d (points);

	std::clog << "indices" << std::endl;

	for (const auto & i : hull .indices ())
	   std::clog << i << std::endl;

	std::clog << "points" << std::endl;

	for (const auto & p : points)
	   std::clog << p << std::endl;

	const auto box = minimum_bounding_box (points);

	std::clog << "minimum_bounding_box " << box .matrix () << std::endl;

	Vector3d t, s;
	Rotation4d r, so;

	box .matrix () .get (t, r, s, so);

	std::clog << "translation " << t << std::endl;
	std::clog << "rotation " << r << std::endl;
	std::clog << "scale " << s << std::endl;
	std::clog << "scaleOrientation " << so << std::endl;
}

void
f ()
{
	Box3d b (Vector3d (2, 2, 2), Vector3d ());

	__LOG__ << b .intersects (Vector3d (0, 0, 0), Vector3d (0.5, 0, 0), Vector3d (0, 0.5, 0)) << std::endl;
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

	f ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
