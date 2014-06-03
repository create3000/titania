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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_V8X3DOBJECT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_V8X3DOBJECT_H__

#include <v8.h>

#include "../../Base/X3DChildObject.h"
#include "v8ObjectType.h"
#include "v8String.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

namespace InternalField {
//
constexpr int OBJECT_TYPE = 0;
constexpr int OBJECT      = 1;
constexpr int SIZE        = 2;

};

// XXX: Put this in X3DObject.cpp
static
std::string
to_string (ObjectType value)
{
	return std::to_string (int (value));
}

template <class Type, ObjectType OBJECT_TYPE>
class X3DObject
{
public:

	static
	v8::Handle <v8::Value>
	create (Context* const, Type* const);


protected:

	///  @name Member access
	
	static
	const std::string &
	getTypeName ()
	{ return typeName; }

	static
	Type*
	getObject (const v8::AccessorInfo & info)
	{
		return getObject (info .This ());
	}

	static
	Type*
	getObject (const v8::Arguments &)
	throw (X3D::Error <X3D::INVALID_FIELD>);

	static
	Type*
	getObject (const v8::Local <v8::Object> & object)
	{
		return getObject (object -> GetInternalField (InternalField::OBJECT));
	}

	static
	Type*
	getObject (const v8::Handle <v8::Value> & value)
	{
		return static_cast <Type*> (v8::Handle <v8::External>::Cast (value) -> Value ());
	}

	///  @name Construction

	static
	v8::Local <v8::FunctionTemplate>
	createFunctionTemplate (Context* const context, v8::InvocationCallback);

	static
	void
	realize (Context* const, const v8::Local <v8::Object> &, Type* const);

	///  @name Operations

	static
	v8::Handle <v8::Value>
	toString (const v8::Arguments &);

	///  @name Destruction

	static
	void
	finalize (v8::Persistent <v8::Value>, void*);


private:

	///  @name Member access

	static
	ObjectType
	getObjectType (const v8::Local <v8::Object> & object)
	{
		return ObjectType (size_t (v8::Handle <v8::External>::Cast (object -> GetInternalField (InternalField::OBJECT_TYPE)) -> Value ()));
	}
	
	///  @name Static members

	static const std::string typeName;

};

template <class Type, ObjectType OBJECT_TYPE>
const std::string X3DObject <Type, OBJECT_TYPE>::typeName = "X3DObject";

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Value>
X3DObject <Type, OBJECT_TYPE>::create (Context* const context, Type* const field)
{
	try
	{
		return context -> getObject (field);
	}
	catch (const std::out_of_range &)
	{
		return context -> createObject (OBJECT_TYPE, field);
	}
}

template <class Type, ObjectType OBJECT_TYPE>
Type*
X3DObject <Type, OBJECT_TYPE>::getObject (const v8::Arguments & args)
throw (X3D::Error <X3D::INVALID_FIELD>)
{
	if (args .This () -> InternalFieldCount () == InternalField::SIZE)
	{
		if (getObjectType (args .This ()) == OBJECT_TYPE)
			return getObject (args .This ());
	}

	throw X3D::Error <X3D::INVALID_FIELD> ("RuntimeError: function must be called with object of type " + to_string (OBJECT_TYPE) + ".");
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Local <v8::FunctionTemplate>
X3DObject <Type, OBJECT_TYPE>::createFunctionTemplate (Context* const context, v8::InvocationCallback callback)
{
	const auto className        = make_v8_string (typeName);
	const auto functionTemplate = v8::FunctionTemplate::New ();

	functionTemplate -> SetCallHandler (callback, v8::External::New (context));	
	functionTemplate -> SetClassName (className);

	functionTemplate -> InstanceTemplate () -> SetInternalFieldCount (InternalField::SIZE);

	return functionTemplate;
}

template <class Type, ObjectType OBJECT_TYPE>
void
X3DObject <Type, OBJECT_TYPE>::realize (Context* const context, const v8::Local <v8::Object> & object, Type* const field)
{
	auto persistent = v8::Persistent <v8::Object>::New (object);

	persistent .MakeWeak (context, finalize);

	object -> SetInternalField (InternalField::OBJECT_TYPE, v8::External::New (reinterpret_cast <void*> (OBJECT_TYPE)));
	object -> SetInternalField (InternalField::OBJECT,      v8::External::New (field));

	context -> addObject (field, persistent);
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Value>
X3DObject <Type, OBJECT_TYPE>::toString (const v8::Arguments & args)
{
	try
	{
		const auto field = getObject (args);

		X3D::Generator::NicestStyle ();

		return make_v8_string (field -> toString ());
	}
	catch (const X3D::X3DError & error)
	{
		return v8::ThrowException (make_v8_string (error .what ()));
	}
}

template <class Type, ObjectType OBJECT_TYPE>
void
X3DObject <Type, OBJECT_TYPE>::finalize (v8::Persistent <v8::Value> value, void* parameter)
{
	__LOG__ << std::endl;

	const auto context = static_cast <Context*> (parameter);
	const auto field   = getObject (value -> ToObject ());

	context -> removeObject (field);
}

} // GoogleV8
} // X3D
} // titania

#endif
