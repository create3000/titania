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
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Geometry/Spheroid3.h>
#include <Titania/Math/Geometry/Triangle3.h>
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

//void
//print_time (double time)
//{
//	std::clog
//		<< std::setiosflags (std::ios::fixed)
//		<< std::setprecision (std::numeric_limits <double>::digits10)
//		<< time
//		<< std::endl;
//}
//
///*basic::ifilestream
// * get_stream ()
// * {
// * titania::TestURI ();
// *
// * basic::uri url0 = std::move (basic::uri ("http://titania.create3000.de"));
// * basic::uri url1 = std::move (basic::uri ("http://titania.create3000.de/documentation/examples/hello_world.wrl"));
// *
// * basic::uri url2 = "file:///home/holger/Projekte/Titania/Titania/share/titania/puck/pages/about/home.wrl";
// * basic::uri url3 = "holger/Titania/Titania/share/titania/puck/pages/about/home.wrl";
// *
// * basic::uri url4 = "http://lockalhost:1235";
// * basic::uri url5 = "about";
// *
// * basic::uri url6 = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
// *
// * //
// *
// * //
// *
// * //
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url1) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url1) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url1) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url2) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url2) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url2) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url3) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url3) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url3) << std::endl;
// *
// * std::clog << "box: " << math::box3 <float> () .max () << std::endl;
// * std::clog << "box: " << math::box3 <float> () .size () << std::endl;
// *
// * //
// *
// * basic::uri         url = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
// * basic::ifilestream stream (basic::http::method::GET, url5);
// *
// * if (stream)
// * {
// *    // Request Headers
// *
// *    stream .request_header ("User-Agent", "Horst Jochen Scholz Graupner");
// *
// *    std::clog << "Request Headers" << std::endl;
// *
// *    for (const auto & request_header : stream .request_headers ())
// *       std::clog << request_header .first << ": " << request_header .second << std::endl;
// *
// *    std::clog << std::endl;
// *
// *    // Send
// *
// *    stream .send ();
// * }
// * else
// *    std::clog << "Open fails." << std::endl;
// *
// * return stream;
// * }
// */
//
//void
//test_path (const basic::path & path)
//{
//	std::clog << std::endl;
//	std::clog << "path:   " << path << std::endl;
//	std::clog << "parent: " << path .remove_dot_segments () .parent () << std::endl;
//	std::clog << "remove: " << path .remove_dot_segments () << std::endl;
//
//}

using namespace titania::math;

using Quaternionf = math::quaternion <float>;
using Vector2f    = math::vector2 <float>;
using Vector3d    = math::vector3 <double>;
using Vector3f    = math::vector3 <float>;
using Vector4f    = math::vector4 <float>;
using Rotation4d  = math::rotation4 <double>;
using Rotation4f  = math::rotation4 <float>;
using Box2f       = math::box2 <float>;
using Box3f       = math::box3 <float>;
using Cylinder3f  = math::cylinder3 <float>;
using Plane3f     = math::plane3 <float>;
using Line3d      = math::line3 <double>;
using Line3f      = math::line3 <float>;
using Sphere3f    = math::sphere3 <float>;
using Matrix3f    = math::matrix3 <float>;
using Matrix4d    = math::matrix4 <double>;
using Matrix4f    = math::matrix4 <float>;
using Spheroid3d  = math::spheroid3 <double>;

class A
{
public:

	A ()
	{
		throw 1;
	}

	A (int)
	{
		__LOG__ << std::endl;
	}

};

class B :
	virtual public A
{
public:

	B () :
	   A ()
	{
		__LOG__ << std::endl;
	}

};

class C :
	virtual public B
{
public:

	C (int i) :
	   A (i)
	{
		__LOG__ << std::endl;
	}

};

class At :
	virtual public A
{
public:

	At () :
	   A ()
	{
		__LOG__ << std::endl;
	}

};

class Bt :
	virtual public B,
	public At
{
public:

	Bt () :
	   B (),
	   At ()
	{
		__LOG__ << std::endl;
	}

};

class Ct :
	virtual public C,
	public Bt
{
public:

	Ct (int i) :
	   A (i),
	   C (i),
	   Bt ()
	{
		__LOG__ << std::endl;
	}

};

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

	Ct (123);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
