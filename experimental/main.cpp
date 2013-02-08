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
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Math.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/OS.h>
#include <Titania/Stream/IGZStream.h>
#include <Titania/Stream/InputFileStream.h>
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

//void
//callback1 ()
//{
//	std::clog << "callback1" << std::endl;
//}
//
//void
//callback2 ()
//{
//	std::clog << "callback2" << std::endl;
//}
//
//void
//callback3 ()
//{
//	std::clog << "callback3" << std::endl;
//}
//
//class O
//{
//public:
//
//	explicit
//	O (OutputProvider & output)
//	{
//		output .addInterest (&O::callback1, this);
//	}
//
//	void
//	callback1 ()
//	{
//		std::clog << "O::callback1" << std::endl;
//	}
//
//	virtual void
//	callback3 (std::string* const &) = 0;
//
//};
//
//void
//callback5 (const std::string & a)
//{
//	std::clog << "callback5" << std::endl;
//}
//
//class A :
//	public O
//{
//public:
//
//	explicit
//	A (OutputProvider & output) :
//		O (output),
//		output (output)
//	{
//		string = new std::string ();
//		output .addInterest (&A::callback1, *this);
//		output .addInterest (&A::callback2, *this, *string);
//		output .addInterest (&A::callback3, *this, string);
//		addInterest (&callback5);
//	}
//
//	void
//	callback1 ()
//	{
//		std::clog << "A::callback1" << std::endl;
//	}
//
//	void
//	callback2 (const std::string & a)
//	{
//		std::clog << "A::callback2" << std::endl;
//	}
//
//	virtual void
//	callback3 (std::string* const & a)
//	{
//		*a = "asdf";
//		std::clog << "A::callback3" << std::endl;
//	}
//
//	InterestId
//	addInterest (void (* requester) (const std::string &))
//	{
//		return output .addInterest (requester, *string);
//	}
//
//	OutputProvider & output;
//	std::string*     string;
//};
//
//void
//callback4 (A* const & a)
//{
//	std::clog << "callback4" << std::endl;
//}
//
//void
//test_output ()
//{
//	OutputProvider output;
//
//	auto id1 = output .addInterest (callback1);
//	auto id2 = output .addInterest (callback2);
//	auto id3 = output .addInterest (callback3);
//	auto id4 = output .addInterest (callback3);
//
//	A* a = new A (output);
//
//	output .addInterest (callback4, a);
//
//	output .processInterests ();
//	output .processInterests ();
//}

//namespace AAA {
//
//template <class Type>
//class A
//{
//public:
//
//	A ()
//	{ }
//
//	template <class Up>
//	A (const Up*)
//	{ std::clog << "Up*" << std::endl; }
//
//	A (const A*)
//	{ std::clog << "A*" << std::endl; }
//
//	std::string s;
//};
//
//}
//
//// Disco == Discusion
//
//class ComponentInfo
//{
//public:
//
//};
//
//class X3DObject;
//
//template <class Type>
//class Output
//{
//public:
//
//	Output ()
//	{ }
//
//
//protected:
//
//	void
//	operator = (const Type &)
//	{ }
//
//	friend class X3DObject;
//
//};
//
//class X3DFieldDefiniton;
//
//class X3DType
//{
//public:
//
//	const std::string &
//	getName () const
//	{ return name; }
//
//
//protected:
//
//	X3DType (const std::string name) :
//		name (name)
//	{ }
//
//	void
//	setName (const std::string & value)
//	{ name = value; }
//
//
//private:
//
//	std::string name;
//};
//
//class X3DObject :
//	public X3DType
//{
//public:
//
//	Output <X3DObject*> name_changed;
//
//	X3DObject () :
//		X3DType ("")
//	{ }
//
//	void
//	setName (const std::string & value)
//	{
//		X3DType::setName (value);
//		name_changed = this;
//	}
//
//	virtual
//	const std::string &
//	getTypeName () const { return getType () -> getName (); }
//
//	virtual
//	X3DType*
//	getType () const = 0;
//
//};
//
//class FieldType :
//	public X3DType
//{
//public:
//
//	FieldType (const std::string name) :
//		X3DType (name)
//	{ }
//
//};
//
//template <class ValueType>
//class SFNode :
//	public X3DObject
//{
//public:
//
//	SFNode () { }
//
//	virtual
//	FieldType*
//	getType () const { return &fieldType; }
//
//	//	static
//	//	FieldType*
//	//	getType () { return &fieldType; }
//
//
//private:
//
//	static FieldType & fieldType;
//
//};
//
//static FieldType SFNodeType ("SFNode");
//
//template <class Type>
//FieldType & SFNode <Type>::fieldType = SFNodeType;
//
//class SupportedComponents
//{
//public:
//
//	static
//	const ComponentInfo &
//	getComponent (const std::string & name)
//	{
//		return components .at (name);
//	}
//
//private:
//
//	static std::map <std::string, ComponentInfo> components;
//};
//
//std::map <std::string, ComponentInfo> SupportedComponents::components = { std::make_pair ("Core", ComponentInfo ()) };
//
//typedef std::vector <X3DFieldDefiniton*> X3DFieldDefinitonArray;
//
//class NodeType :
//	public X3DType
//{
//public:
//
//	NodeType (const std::string name, const ComponentInfo & component) :
//		X3DType (name),
//		component (&component),
//		fieldDefinitons ()
//	{ }
//
//	const ComponentInfo*
//	getComponent () const
//	{ return component; }
//
//	const X3DFieldDefinitonArray &
//	getFieldDefinitions () const
//	{ return fieldDefinitons; }
//
//
//private:
//
//	const ComponentInfo*         component;
//	const X3DFieldDefinitonArray fieldDefinitons;
//};
//
//class MetadataBool :
//	public X3DObject
//{
//public:
//
//	MetadataBool () { }
//
//	virtual
//	NodeType*
//	getType () const { return &nodeType; }
//
//	const ComponentInfo*
//	getComponent ()
//	{ return getType () -> getComponent (); }
//
//
//private:
//
//	static NodeType nodeType;
//
//};
//
//NodeType MetadataBool::nodeType ("MetatdataBool", SupportedComponents::getComponent ("Core"));
//
//template <class Type>
//class A
//{
//public:
//
//	Type value;
//};
//
//template <class Type>
//Type
//operator + (const A <Type> & a, const A <Type> & b)
//{
//	return a .value + b .value;
//}

//template <class Type>
//class SFVec3f
//{
//public:
//
//	operator const std::complex <Type> & () { return value; }
//
//	std::complex <Type> value;
//
//};

//class X3DObject
//{
//public:
//
//	virtual
//	X3DObject*
//	test ()
//	{
//		return this;
//	}
//
//};
//
//class X3DBasicNode :
//	virtual public X3DObject
//{
//public:
//
//};
//
//class X3DNode :
//	public X3DBasicNode, virtual public X3DObject
//{
//public:
//
//};
//
//class X3DPrototypeInstance :
//	public X3DNode
//{
//public:
//
//	virtual
//	X3DNode*
//	getRootNode ()
//	{
//		return this;
//	}
//
//};
//
////template <class Up, class Type>
////struct Cast
////{
////	static
////	Type*
////	from (Up* value)
////	{
////		std::clog << "Cast <Up, Type>" << std::endl;
////		return dynamic_cast <Type*> (value);
////	}
////
////};
//
//template <class Type, class Up>
//struct cast
//{
//	static
//	Type*
//	node (Up* const value)
//	{
//		std::clog << "cast <Type, Up>" << std::endl;
//		return dynamic_cast <Type*> (value);
//	}
//
//};
//
//template <class Type>
//struct cast <Type, Type>
//{
//	static
//	Type*
//	node (Type* const value)
//	{
//		std::clog << "cast <Type, Type>" << std::endl;
//		return value;
//	}
//
//};
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//template <class Up>
//struct cast <X3DBasicNode, Up>
//{
//	static
//	X3DBasicNode*
//	node (Up* const value)
//	{
//		std::clog << "cast <X3DBasicNode, Up>" << std::endl;
//		return value;
//	}
//
//};
//
//template <>
//struct cast <X3DBasicNode, X3DBasicNode>
//{
//	static
//	X3DBasicNode*
//	node (X3DBasicNode* const value)
//	{
//		std::clog << "cast <X3DBasicNode, X3DBasicNode>" << std::endl;
//		return value;
//	}
//
//};
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//template <>
//struct cast <X3DPrototypeInstance, X3DPrototypeInstance>
//{
//	static
//	X3DPrototypeInstance*
//	node (X3DPrototypeInstance* const value)
//	{
//		std::clog << "cast <X3DPrototypeInstance, X3DPrototypeInstance>" << std::endl;
//		return value;
//	}
//
//};
//
//template <>
//struct cast <X3DBasicNode, X3DPrototypeInstance>
//{
//	static
//	X3DBasicNode*
//	node (X3DPrototypeInstance* const value)
//	{
//		std::clog << "cast <X3DBasicNode, X3DPrototypeInstance>" << std::endl;
//		return value;
//	}
//
//};
//
//template <class Type>
//struct cast <Type, X3DPrototypeInstance>
//{
//	static
//	Type*
//	node (X3DPrototypeInstance* const value)
//	{
//		std::clog << "cast <Type, X3DPrototypeInstance>" << std::endl;
//		return dynamic_cast <Type*> (value -> getRootNode ());
//	}
//
//};
//
//template <class Type>
//struct cast <Type, X3DBasicNode>
//{
//	static
//	Type*
//	node (X3DBasicNode* const value)
//	{
//		std::clog << "cast <Type, X3DBasicNode>" << std::endl;
//		return dynamic_cast <Type*> (value);
//	}
//
//};
//
////template <>
////struct Cast <X3DPrototypeInstance, X3DBasicNode>
////{
////	static
////	X3DBasicNode*
////	from (X3DPrototypeInstance* value)
////	{
////		std::clog << "Cast <X3DPrototypeInstance, X3DBasicNode>" << std::endl;
////		return value;
////	}
////
////};

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

typedef math::vector2 <float>   Vector2f;
typedef math::vector3 <float>   Vector3f;
typedef math::rotation4 <float> Rotation4f;
typedef math::matrix4 <float>   Matrix4f;
typedef math::box3 <float>      Box3f;

namespace Test {

template <class ForwardIterator, class RangeIterator>
ForwardIterator
remove (ForwardIterator first, ForwardIterator last, RangeIterator rfirst, RangeIterator rlast)
{
	std::multiset <typename RangeIterator::value_type> range (rfirst, rlast);

	if (range .empty ())
		return last;

	size_t count = 0;

	for ( ; first not_eq last; ++ first)
	{
		auto item = range .find (*first);

		if (item not_eq range .end ())
		{
			range .erase (item);
			++ count;
			break;
		}
	}

	while (range .size ())
	{
		auto second = first + count;

		for ( ; second not_eq last; ++ first, ++ second)
		{
			auto item = range .find (*second);

			if (item not_eq range .end ())
			{
				range .erase (item);
				++ count;
				goto LOOP;
			}

			*first = std::move (*second);
		}

		break;

LOOP:;
	}

	for (auto second = first + count; second not_eq last; ++ first, ++ second)
	{
		*first = std::move (*second);
	}

	return first;
}

}

///  Map @a value in the interval (fromLow;fromHigh) to the interval (toLow;toHigh).
template <class Type>
constexpr Type
project (const Type & value, const Type & fromLow, const Type & fromHigh, const Type & toLow, const Type & toHigh)
{
	return toLow + ((value - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow);
}

#include <v8.h>

static
void
test (Matrix4f & m)
{
	m *= Matrix4f ();
}

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;

	#ifdef _GLIBCXX_PARALLEL
	std::clog << "in parallel mode ..." << std::endl;
	#endif
	
	__LOG__ << basic::uri ("file:///home/holger/Projekte/Titania/Library/Examples/icts/labyrinth.wrl") .scheme () .size () << std::endl;
	__LOG__ << basic::uri ("/home/holger/Projekte/Titania/Titania/share/titania/ui/icons/icon-bw.svg") .transform ("file:///home/holger/Projekte/Titania/Library/Examples/icts/labyrinth.wrl") << std::endl;
	__LOG__ << bool (basic::ifilestream (basic::http::method::GET, "")) << std::endl;

	if (0)
	{
		//__gnu_parallel::_Settings s;
		//s .algorithm_strategy = __gnu_parallel::force_parallel;
		//s .for_each_minimal_n = 1;
		//__gnu_parallel::_Settings::set (s);

		#define N 100000
		#define VECTOR_SIZE 1000

		std::deque <Matrix4f> a1 (VECTOR_SIZE);

		/////////////////////////////////////////////

		auto t0 = chrono::now ();

		/////////////////////////////////////////////

		for (int n = 0; n < N; ++ n)
			std::for_each (a1 .begin (), a1 .end (), test);

		/////////////////////////////////////////////

		print_time (chrono::now () - t0);
		t0 = chrono::now ();

		/////////////////////////////////////////////

		for (int n = 0; n < N; ++ n)
			for (auto a : a1)
				test (a);

		/////////////////////////////////////////////

		print_time (chrono::now () - t0);
		t0 = chrono::now ();

		/////////////////////////////////////////////

		for (int n = 0; n < N; ++ n)
			for (size_t i = 0, size = a1 .size (); i < size; ++ i)
				test (a1 [i]);

		/////////////////////////////////////////////

		print_time (chrono::now () - t0);
		t0 = chrono::now ();
	}

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
