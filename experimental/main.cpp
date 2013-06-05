// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

//#include <Titania/Basic/Geometry/Line3.h>

#include "URI.h"
#include <Titania/Algorithm/Remove.h>
#include <Titania/Basic/Path.h>
#include <Titania/Basic/URI.h>
#include <Titania/Chrono/Now.h>
#include <Titania/Math/Functional.h>
#include <Titania/Math/Geometry/Box2.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Cylinder3.h>
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/OS.h>
#include <Titania/Stream/IGZFilter.h>
#include <Titania/Stream/InputFileStream.h>
#include <Titania/Utility/Pass.h>
//#include <Titania/Stream/InputHTTPStream.h>

#include <Titania/Algorithm.h>
#include <Titania/LOG.h>

#include <array>
#include <cassert>
#include <complex>
#include <deque>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>
#include <regex>

using namespace titania;
using namespace titania::basic;

void
print_time (double time)
{
	std::clog
		<< std::setiosflags (std::ios::fixed)
		<< std::setprecision (std::numeric_limits <double>::digits10)
		<< time
		<< std::endl;
}

/*basic::ifilestream
 * get_stream ()
 * {
 * titania::TestURI ();
 *
 * basic::uri url0 = std::move (basic::uri ("http://titania.create3000.de"));
 * basic::uri url1 = std::move (basic::uri ("http://titania.create3000.de/documentation/examples/hello_world.wrl"));
 *
 * basic::uri url2 = "file:///home/holger/Projekte/Titania/Titania/share/titania/puck/pages/about/home.wrl";
 * basic::uri url3 = "holger/Titania/Titania/share/titania/puck/pages/about/home.wrl";
 *
 * basic::uri url4 = "http://lockalhost:1235";
 * basic::uri url5 = "about";
 *
 * basic::uri url6 = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
 *
 * //
 *
 * //
 *
 * //
 *
 * std::clog << "resolve: " << basic::uri ("h1.wrl",    url0) << std::endl;
 * std::clog << "resolve: " << basic::uri ("../h1.wrl", url0) << std::endl;
 * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
 * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
 *
 * std::clog << "resolve: " << basic::uri ("h1.wrl",    url1) << std::endl;
 * std::clog << "resolve: " << basic::uri ("../h1.wrl", url1) << std::endl;
 * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url1) << std::endl;
 *
 * std::clog << "resolve: " << basic::uri ("h1.wrl",    url2) << std::endl;
 * std::clog << "resolve: " << basic::uri ("../h1.wrl", url2) << std::endl;
 * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url2) << std::endl;
 *
 * std::clog << "resolve: " << basic::uri ("h1.wrl",    url3) << std::endl;
 * std::clog << "resolve: " << basic::uri ("../h1.wrl", url3) << std::endl;
 * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url3) << std::endl;
 *
 * std::clog << "box: " << math::box3 <float> () .max () << std::endl;
 * std::clog << "box: " << math::box3 <float> () .size () << std::endl;
 *
 * //
 *
 * basic::uri         url = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
 * basic::ifilestream stream (basic::http::method::GET, url5);
 *
 * if (stream)
 * {
 *    // Request Headers
 *
 *    stream .request_header ("User-Agent", "Horst Jochen Scholz Graupner");
 *
 *    std::clog << "Request Headers" << std::endl;
 *
 *    for (const auto & request_header : stream .request_headers ())
 *       std::clog << request_header .first << ": " << request_header .second << std::endl;
 *
 *    std::clog << std::endl;
 *
 *    // Send
 *
 *    stream .send ();
 * }
 * else
 *    std::clog << "Open fails." << std::endl;
 *
 * return stream;
 * }
 */

void
test_path (const basic::path & path)
{
	std::clog << std::endl;
	std::clog << "path:   " << path << std::endl;
	std::clog << "parent: " << path .remove_dot_segments () .parent () << std::endl;
	std::clog << "remove: " << path .remove_dot_segments () << std::endl;

}

typedef math::quaternion <float> Quaternionf;
typedef math::vector2 <float>    Vector2f;
typedef math::vector3 <float>    Vector3f;
typedef math::rotation4 <float>  Rotation4f;
typedef math::matrix4 <float>    Matrix4f;
typedef math::box2 <float>       Box2f;
typedef math::box3 <float>       Box3f;
typedef math::cylinder3 <float>  Cylinder3f;
typedef math::plane3 <float>     Plane3f;
typedef math::line3 <float>      Line3f;
typedef math::sphere3 <float>    Sphere3f;

#include <v8.h>

// https://www.homepluspower.info/2010/06/v8-javascript-engine-tutorial-part-1.html
// https://www.homepluspower.info/2010/06/v8-javascript-engine-tutorial-part-2.html
// http://athile.net/library/wiki/index.php/Library/V8/Tutorial
// http://www.codeproject.com/Articles/29109/Using-V8-Google-s-Chrome-JavaScript-Virtual-Machin
// http://v8.googlecode.com/svn/trunk/samples/shell.cc
// http://stackoverflow.com/questions/11387015/calling-a-v8-javascript-function-from-c-with-an-argument

#include <Titania/InputOutput.h>

std::string
create_string ()
{
	return std::string (200000000, '#');
}

template <typename First, typename Second, int Third>
class SomeType;

template <class Type>
using Array = std::deque <Type*>;

template <class Type>
void
set_test ()
{
	int N = 10000;
	int n = 10000;

	std::vector <int*> a;

	for (int i = 0; i < n; ++ i)
		a .emplace_back (new int ());

	Type set;

	auto t0 = chrono::now ();
	{
		for (int k = 0; k < N; ++ k)
			for (int i = 0; i < n; ++ i)
				set .insert (a [i]);

	}
	__LOG__ << chrono::now () - t0 << std::endl;

	t0 = chrono::now ();
	{
		for (int k = 0; k < N; ++ k)
			for (int i = 0; i < n; ++ i)
				set .find (a [i]);

	}
	__LOG__ << chrono::now () - t0 << std::endl;
}

template <class Type>
class range_iterator
{
public:

	typedef range_iterator <Type>       iterator;
	typedef range_iterator <const Type> const_iterator;

	typedef std::bidirectional_iterator_tag iterator_category;

	typedef Type*        pointer;
	typedef Type         value_type;
	typedef Type &       reference;
	typedef const Type & const_reference;

	typedef size_t size_type;
	typedef size_t difference_type;

	range_iterator (Type index, Type step) :
		m_index (index),
		m_step (step)
	{ }

	range_iterator (const range_iterator & iter) :
		m_index (iter .m_index),
		m_step (iter .m_step)
	{ }

	range_iterator &
	operator = (const range_iterator & iter)
	{
		m_index = iter .m_index;
		m_step  = iter .m_step;
		return *this;
	}

	reference
	operator * ()
	{ return m_index; }

	const const_reference
	operator * () const
	{ return m_index; }

	pointer
	operator -> ()
	{ return &m_index; }

	const pointer
	operator -> () const
	{ return &m_index; }

	range_iterator &
	operator ++ ()
	{
		m_index += m_step;
		return *this;
	}

	range_iterator
	operator ++ (int)
	{
		range_iterator tmp = *this;

		m_index += m_step;
		return tmp;
	}

	range_iterator &
	operator -- ()
	{
		m_index -= m_step;
		return *this;
	}

	range_iterator
	operator -- (int)
	{
		range_iterator tmp = *this;

		m_index -= m_step;
		return tmp;
	}

	range_iterator &
	operator += (difference_type n)
	{
		m_index += n * m_step;
		return *this;
	}

	range_iterator &
	operator -= (difference_type n)
	{
		m_index -= n * m_step;
		return *this;
	}

private:

	Type m_index;
	Type m_step;

};

template <class Type>
inline
range_iterator <Type>
operator + (const range_iterator <Type> & iter, size_t n)
{
	return range_iterator <Type> (iter) += n;
}

template <class Type>
inline
range_iterator <Type>
operator + (size_t n, const range_iterator <Type> & iter)
{
	return range_iterator <Type> (iter) += n;
}

template <class Type>
inline
range_iterator <Type>
operator - (const range_iterator <Type> & iter, size_t n)
{
	return range_iterator <Type> (iter) -= n;
}

template <class Type>
inline
range_iterator <Type>
operator - (size_t n, const range_iterator <Type> & iter)
{
	return range_iterator <Type> (iter) -= n;
}

template <class Type>
inline
typename range_iterator <Type>::difference_type
operator - (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return lhs .base () - rhs .base ();
}

template <class Type>
inline
bool
operator == (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs == *rhs;
}

template <class Type>
inline
bool
operator not_eq (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs not_eq * rhs;
}

template <class Type>
inline
bool
operator < (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs < *rhs;
}

template <class Type>
inline
bool
operator > (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs > *rhs;
}

template <class Type>
inline
bool
operator >= (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs >= *rhs;
}

template <class Type>
inline
bool
operator <= (const range_iterator <Type> & lhs, const range_iterator <Type> & rhs)
{
	return *lhs <= *rhs;
}

template <class Type>
class range
{
public:

	typedef typename range_iterator <Type>::iterator       iterator;
	typedef typename range_iterator <Type>::const_iterator const_iterator;

	range (Type begin, Type end, Type step = 1) :
		m_begin (begin),
		m_end (end),
		m_step (step)
	{ }

	iterator
	begin ()
	{ return iterator (m_begin, m_step); }

	const_iterator
	begin () const
	{ return const_iterator (m_begin, m_step); }

	const_iterator
	cbegin () const
	{ return const_iterator (m_begin, m_step); }

	iterator
	end ()
	{ return iterator (m_end, m_step); }

	const_iterator
	end () const
	{ return const_iterator (m_end, m_step); }

	const_iterator
	cend () const
	{ return const_iterator (m_end, m_step); }


private:

	Type m_begin;
	Type m_end;
	Type m_step;

};

class A
{
public:
};

class B :
	virtual public A
{
public:
};

class C :
	virtual public A
{
public:
};

class D :
	public C
{
public:
};

class E :
	public D, virtual public A
{
public:
};

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;

	#ifdef _GLIBCXX_PARALLEL
	std::clog << "in parallel mode ..." << std::endl;
	#endif

	E* e = new E ();
	A* a = e;
			
	std::clog << a << std::endl;

	std::clog << "Function main done." << std::endl;
	exit (0);
	return 0;
}

//	test_path (basic::path ("/"));
//	test_path (basic::path ("/", "/"));
//	test_path (basic::path ("home", "/"));
//	test_path (basic::path ("/home/holger/Projekte/Titania/Titania/share/titania/puck///pages/about/home.wrl/", "/"));
//	test_path (basic::path ("home/holger/Projekte/Titania/Titania/share/titania/puck///pages/about/home.wrl/", "/"));
//	test_path (basic::path ("/home/holger/Projekte/../../../.././${Version}/pages/about/home.wrl/..", "/"));

//	basic::ifilestream stream = get_stream ();
//
//	if (stream)
//	{
//		// Response
//
//		std::clog
//			<< "Response" << std::endl
//			<< "Http-Version: " << stream .http_version () << std::endl
//			<< "Status: "       << stream .status ()       << std::endl
//			<< "Reason: "       << stream .reason ()       << std::endl
//			<<  std::endl;
//
//		// Response Headers
//
//		std::clog << "Response Headers" << std::endl;
//		for (const auto & response_header : stream .response_headers ())
//			std::clog << response_header .first << ": " << response_header .second << std::endl;
//		std::clog <<  std::endl;
//
//
//		std::ostringstream osstream;
//		osstream << stream .rdbuf ();
//
//		std::clog << "'" << osstream .str () << "'" << std::endl;
//	}

//	sql::sqlite3 db (os::home () + "/test.db");
//
//	db .query ("DROP TABLE IF EXISTS test");
//	db .query ("CREATE TABLE IF NOT EXISTS test (title TEXT, location TEXT)");
//
//	db .query ("INSERT INTO test (title, location) VALUES ('tit1', 'loc1')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit2', 'loc2')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit3', 'loc3')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit4', 'loc4')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit5', 'loc5')");
//
//
//	for (const auto & item : db .query_assoc ("SELECT * FROM test"))
//		std::clog << item .at ("title") << ", " << item .at ("location") << std::endl;
//	std::clog << std::endl;
//
//
//	db .query ("INSERT INTO test (title, location) VALUES ('tit6', 'loc6')");
//
//	db .query_array ("UPDATE test SET title = 'tit10', location = 'loc10' WHERE title = 'tit6'");
//
//
//	for (const auto & item : db .query_assoc ("SELECT * FROM test WHERE title = 'tit10'"))
//		std::clog << item .at ("title") << ", " << item .at ("location") << std::endl;
//	std::clog << std::endl;
//

//	gegl_init (&argc, &argv);
//
//
//	const char* path = "/home/holger/Projekte/Titania/Library/Tests/Texturing/images/colors/bw.png";
//	std::ifstream      stream (path);
//	std::ostringstream sstream;
//	sstream << stream .rdbuf ();
//
//	auto image = sstream .str ();
//
//	std::clog << "############" << image .size () << std::endl;
//
//	GeglNode* gegl = gegl_node_new ();
//
//
//	GeglNode* load = gegl_node_new_child (gegl,
//                            "operation", "gegl:load",
//                            "path",      path,
//                            NULL);
//
//	//	GeglNode* scale  = gegl_node_new_child (gegl,
//	//                                    "operation", "gegl:scale",
//	//                                    "width", 200,
//	//                                    "height", 50,
//	//                                    NULL);
//
//	GeglBuffer* outputBuffer;
//	GeglNode*   output = gegl_node_new_child (gegl,
//	                                          "operation", "gegl:buffer-sink",
//	                                          "buffer", &outputBuffer,
//	                                          NULL);
//
//	gegl_node_link_many (load, output, NULL);
//	gegl_node_process (output);
//
//	auto babl = gegl_buffer_get_format (outputBuffer);
//
//	std::clog << babl -> format .components << std::endl;
//	std::clog << gegl_buffer_get_width (outputBuffer) << " " << gegl_buffer_get_height (outputBuffer) << std::endl;
//
//	std::vector <uint16_t> array (gegl_buffer_get_width (outputBuffer) * gegl_buffer_get_height (outputBuffer) * 2);
//
//	gegl_node_blit (load, 1, NULL
//	                 babl_format ("Y u8"),
//	                 array .data (),
//	                 GEGL_AUTO_ROWSTRIDE,
//	                 GEGL_BLIT_DEFAULT);
//
//	g_object_unref (outputBuffer);
//	g_object_unref (gegl);
//	gegl_exit ();
