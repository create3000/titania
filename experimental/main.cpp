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
#include <Titania/Math/Numbers/Hash.h>
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

#include "Vector4.h"

using namespace titania;
using namespace titania::basic;
using namespace titania::math;

using Quaternionf  = math::quaternion <float>;
using Vector2f     = math::vector2 <float>;
using Vector2d     = math::vector2 <double>;
using Vector3d     = math::vector3 <double>;
using Vector3f     = math::vector3 <float>;
using Vector4d     = math::vector4 <double>;
using Vector4f     = math::vector4 <float>;
using Quaternion4d = quaternion <double>;
using Quaternion4f = quaternion <float>;
using Rotation4d   = math::rotation4 <double>;
using Rotation4f   = math::rotation4 <float>;
using Box2f        = math::box2 <float>;
using Box3f        = math::box3 <float>;
using Box3d        = math::box3 <double>;
using Cylinder3f   = math::cylinder3 <float>;
using Plane3f      = math::plane3 <float>;
using Line2d       = math::line2 <double>;
using Line2f       = math::line2 <float>;
using Line3d       = math::line3 <double>;
using Line3f       = math::line3 <float>;
using Sphere3f     = math::sphere3 <float>;
using Matrix2d     = math::matrix2 <double>;
using Matrix2f     = math::matrix2 <float>;
using Matrix3d     = math::matrix3 <double>;
using Matrix3f     = math::matrix3 <float>;
using Matrix4d     = math::matrix4 <double>;
using Matrix4f     = math::matrix4 <float>;
using Spheroid3d   = math::spheroid3 <double>;
using ConvexHull2d = math::convex_hull2 <double>;
using ConvexHull3d = math::convex_hull3 <double>;


class A
{
public:

	constexpr
	A () :
		v ()
	{ }

	const Vector3d &
	f () const
	{ return v; };

	Vector3d v;


};
// Octave transform matrix from Alexander Alekseev aka TDM 
constexpr auto octave_m = Matrix2d (1.6,1.2,-1.2,1.6);

void
FractalNoise (int index, Vector2d vertex)
{
   float m = 1.5;
   float w = 0.5;

	auto xy = vertex;

	switch (index)
	{
		case 0:
			std::clog << "Amplitude" << std::endl;
			break;
		case 1:
			std::clog << "Frequency" << std::endl;
			break;
		case 2:
			std::clog << "Speed" << std::endl;
			break;
		default:
			break;
	}

   for (int i = 0; i < 6; i++)
   {
		auto amplitude1 = m * 0.15;
		auto amplitude2 = w * 0.25;
		auto frequency  = xy / vertex;
		auto speed1     = Vector2d (0.511, 0.511) / frequency;
		auto speed2     = Vector2d (0.333, 0.333) / frequency;
			
		switch (index)
		{
			case 0:
				std::clog << amplitude1 << std::endl;
				std::clog << amplitude2 << std::endl;
				break;
			case 1:
				std::clog << frequency << std::endl;
				std::clog << frequency << std::endl;
				break;
			case 2:
				std::clog << speed1 << std::endl;
				std::clog << speed2 << std::endl;
				break;
			default:
				break;
		}
	
		//xy = vertex * frequency [i] - s * frequency [i]

      //f += Noise(xy+time*0.511) * m * 0.15;
      //f += Noise(xy.yx-time*0.333) * w * 0.25;
      w *= 0.5;
      m *= 0.25;
      xy = octave_m * xy;
   }
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

	Matrix4d a (1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6);
	Matrix4d b (1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6);

	std::cout << std::endl;
	std::cout << a << std::endl;

	for (const auto & v : a)
		std::cout << v << std::endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
