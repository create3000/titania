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
#include "v8String.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

enum class ObjectType
{
	Default,
	SFVec4d,
	SFVec4f

};

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

protected:

	///  @name Member access

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
		return static_cast <Type*> (v8::Handle <v8::External>::Cast (object -> GetInternalField (InternalField::OBJECT)) -> Value ());
	}

	///  @name Construction

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

	static
	ObjectType
	getObjectType (const v8::Local <v8::Object> & object)
	{
		return ObjectType (size_t (v8::Handle <v8::External>::Cast (object -> GetInternalField (InternalField::OBJECT_TYPE)) -> Value ()));
	}

};

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

	throw X3D::Error <X3D::INVALID_FIELD> ("Function must be called with object of type " + to_string (OBJECT_TYPE) + ".");
}

template <class Type, ObjectType OBJECT_TYPE>
void
X3DObject <Type, OBJECT_TYPE>::realize (Context* const context, const v8::Local <v8::Object> & object, Type* const child)
{
	v8::V8::AdjustAmountOfExternalAllocatedMemory (sizeof (Type));

	auto persistent = v8::Persistent <v8::Object>::New (object);

	persistent .MakeWeak (context, finalize);

	object -> SetInternalField (InternalField::OBJECT_TYPE, v8::External::New (reinterpret_cast <void*> (OBJECT_TYPE)));
	object -> SetInternalField (InternalField::OBJECT,      v8::External::New (child));

	context -> addObject (child, persistent);
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
	const auto child   = getObject (value -> ToObject ());

	v8::V8::AdjustAmountOfExternalAllocatedMemory (-sizeof (Type));

	context -> removeObject (child);
}

} // GoogleV8
} // X3D
} // titania

#endif
