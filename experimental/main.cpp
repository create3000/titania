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
#include <Titania/Basic/URI.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Math.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/OS.h>
#include <Titania/Stream/InputFileStream.h>
#include <Titania/Stream/InputHTTPStream.h>

#include <Titania/Bits/Algorithm.h>

#include <array>
#include <cassert>
#include <complex>
#include <deque>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <vector>

using namespace titania;

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

basic::ifilestream
get_stream ()
{
	titania::TestURI ();

	basic::uri url0 = std::move (basic::uri ("http://titania.create3000.de"));
	basic::uri url1 = std::move (basic::uri ("http://titania.create3000.de/documentation/examples/hello_world.wrl"));

	basic::uri url2 = "file:///home/holger/Projekte/Titania/Puck/share/titania/puck/pages/about/home.wrl";
	basic::uri url3 = "holger/Titania/Puck/share/titania/puck/pages/about/home.wrl";

	basic::uri url4 = "http://lockalhost:1235";
	basic::uri url5 = "about";

	basic::uri url6 = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";

	//

	//

	//

	std::clog << "resolve: " << basic::uri ("h1.wrl",    url0) << std::endl;
	std::clog << "resolve: " << basic::uri ("../h1.wrl", url0) << std::endl;
	std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
	std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;

	std::clog << "resolve: " << basic::uri ("h1.wrl",    url1) << std::endl;
	std::clog << "resolve: " << basic::uri ("../h1.wrl", url1) << std::endl;
	std::clog << "resolve: " << basic::uri ("/h1.wrl",   url1) << std::endl;

	std::clog << "resolve: " << basic::uri ("h1.wrl",    url2) << std::endl;
	std::clog << "resolve: " << basic::uri ("../h1.wrl", url2) << std::endl;
	std::clog << "resolve: " << basic::uri ("/h1.wrl",   url2) << std::endl;

	std::clog << "resolve: " << basic::uri ("h1.wrl",    url3) << std::endl;
	std::clog << "resolve: " << basic::uri ("../h1.wrl", url3) << std::endl;
	std::clog << "resolve: " << basic::uri ("/h1.wrl",   url3) << std::endl;

	std::clog << "box: " << math::box3 <float> () .max () << std::endl;
	std::clog << "box: " << math::box3 <float> () .size () << std::endl;

	//

	basic::uri         url = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
	basic::ifilestream stream (basic::http::method::GET, url5);

	if (stream)
	{
		// Request Headers

		stream .request_header ("User-Agent", "Horst Jochen Scholz Graupner");

		std::clog << "Request Headers" << std::endl;

		for (const auto & request_header : stream .request_headers ())
			std::clog << request_header .first << ": " << request_header .second << std::endl;

		std::clog << std::endl;

		// Send

		stream .send ();
	}
	else
		std::clog << "Open fails." << std::endl;

	return stream;
}

#include <Titania/Bits/String/Split.h>
#include <algorithm>
#include <iterator>
#include <list>

namespace titania {
namespace basic {

/**
 *  Template to represent paths.
 *  This can be file system paths or even urn paths.
 */
template <class StringT>
class basic_path :
	public std::list <StringT>
{
public:

	typedef StringT                      string_type;
	typedef typename StringT::value_type char_type;
	typedef typename StringT::size_type  size_type;
	typedef std::list <StringT>          array_type;

	typedef typename array_type::const_iterator const_iterator;

	using array_type::size;
	using array_type::begin;
	using array_type::end;
	using array_type::front;
	using array_type::back;
	using array_type::pop_front;
	using array_type::pop_back;

	///  @name Constructors

	explicit
	basic_path (const StringT & separator) :
		std::list <StringT> (),
		value ({ separator, false, false })
	{ }

	basic_path (const StringT & path, const StringT & separator);

	///  @name Element access

	const string_type &
	separator () const { return value .separator; }

	bool
	leading_separator () const { return value .leading_separator; }

	bool
	trailing_separator () const { return value .trailing_separator; }

	basic_path
	root () const;

	basic_path
	parent () const;


private:

	struct Value
	{
		string_type separator;
		bool leading_separator;
		bool trailing_separator;
	};

	basic_path (const Value & value) :
		std::list <StringT> (),
		value (value)
	{ }

	basic_path (array_type && array, const Value & value) :
		std::list <StringT> (array),
		value (value)
	{ }

	basic_path (const_iterator first, const_iterator last, const Value & value) :
		std::list <StringT> (first, last),
		value (value)
	{ }

	Value value;

};

template <class StringT>
basic_path <StringT>::basic_path (const StringT & path, const StringT & separator) :
	std::list <StringT> (std::move (basic_split <StringT, std::list> (path, separator))),
	value ({ separator, false, false })
{
	if (size ())
	{
		if (front () == string_type ())
		{
			pop_front ();
			value .leading_separator = true;
		}
	}

	if (size () > 1)
	{
		if (back () == string_type ())
		{
			pop_back ();
			value .trailing_separator = true;
		}
	}
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::root () const
{
	return basic_path (Value { separator (), true, false });
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::parent () const
{
	switch (size ())
	{
		case 0:
		case 1:
		{
			if (leading_separator ())
				return root ();

			return basic_path (array_type { ".." }, { separator (), false, false });
		}

		default:
			return basic_path (begin (), -- end (), { separator (), leading_separator (), true });
	}

}

///  @relates basic_path
///  @name Input/Output operations

///@{
///  Insertion operator for URI values.
template <class StringT, class Traits>
inline
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> & ostream, const basic_path <StringT> & path)
{
	if (path .leading_separator ())
		ostream << path .separator ();

	if (path .size ())
	{
		if (path .size () > 1)
		{
			std::copy (path .begin (),
			           -- path .end (),
			           std::ostream_iterator <StringT> (ostream, path .separator () .c_str ()));
		}

		ostream << path .back ();
	}

	if (path .trailing_separator ())
		ostream << path .separator ();

	return ostream;
}

///@}

typedef basic_path <std::string>  path;
typedef basic_path <std::wstring> wpath;

} // basic
} // titania

int
main ()
{
	std::clog << "Starting main ..." << std::endl;

	std::clog << basic::path ("/") << std::endl;
	std::clog << basic::path ("/", "/") << std::endl;
	std::clog << basic::path ("home", "/") << std::endl;
	std::clog << basic::path ("/home/holger/Projekte/Titania/Puck/share/titania/puck///pages/about/home.wrl/", "/") << std::endl;
	std::clog << basic::path ("home/holger/Projekte/Titania/Puck/share/titania/puck///pages/about/home.wrl/", "/") << std::endl;

	std::clog << basic::path ("/") .parent () << std::endl;
	std::clog << basic::path ("/", "/") .parent () << std::endl;
	std::clog << basic::path ("home", "/") .parent () << std::endl;
	std::clog << basic::path ("/home/holger/Projekte/Titania/Puck/share/titania/puck///pages/about/home.wrl/", "/") .parent () << std::endl;
	std::clog << basic::path ("home/holger/Projekte/Titania/Puck/share/titania/puck///pages/about/home.wrl/", "/") .parent () << std::endl;

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

	std::clog << "Function main done." << std::endl;
	return 0;
}
