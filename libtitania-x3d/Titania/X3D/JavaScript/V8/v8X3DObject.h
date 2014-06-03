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

template <class Type>
inline
Type
get_object (const v8::Local <v8::Object> & object)
{
	return static_cast <Type> (v8::Handle <v8::External>::Cast (object -> GetInternalField (0)) -> Value ());
}

template <class Type>
inline
Type
get_object (const v8::Arguments & args)
throw (X3D::Error <X3D::INVALID_FIELD>)
{
	const auto className = v8::Handle <v8::String>::Cast (args .Data ());

	if (args .This () -> GetConstructorName () == className)
		return static_cast <Type> (v8::Handle <v8::External>::Cast (args .This () -> GetInternalField (0)) -> Value ());
	
	throw X3D::Error <X3D::INVALID_FIELD> ("Must be called with object of type " + get_utf8_string (className) + ".");
}

template <class Type>
inline
Type
get_object (const v8::AccessorInfo & info)
{
	return static_cast <Type> (v8::Handle <v8::External>::Cast (info .This () -> GetInternalField (0)) -> Value ());
}

class Object
{
protected:

	template <class Type>
	static
	void
	realize (Context* const, const v8::Local <v8::Object> &, Type* const);

	template <class Type>
	static
	v8::Handle <v8::Value>
	toString (const v8::Arguments &);

	template <class Type>
	static
	void
	finalize (v8::Persistent <v8::Value>, void*);

};

template <class Type>
void
Object::realize (Context* const context, const v8::Local <v8::Object> & object, Type* const child)
{
	v8::V8::AdjustAmountOfExternalAllocatedMemory (sizeof (Type));

	auto persistent = v8::Persistent <v8::Object>::New (object);

	persistent .MakeWeak (context, finalize <Type>);

	object -> SetInternalField (0, v8::External::New (child));

	context -> addObject (child, persistent);
}

template <class Type>
v8::Handle <v8::Value>
Object::toString (const v8::Arguments & args)
{
	try
	{
		const auto field = get_object <Type*> (args);

		X3D::Generator::NicestStyle ();

		return make_v8_string (field -> toString ());
	}
	catch (const X3D::X3DError & error)
	{
		return v8::ThrowException (make_v8_string (error .what ()));
	}
}

template <class Type>
void
Object::finalize (v8::Persistent <v8::Value> value, void* parameter)
{
	__LOG__ << std::endl;

	const auto context = static_cast <Context*> (parameter);
	const auto child   = get_object <Type*> (value -> ToObject ());

	v8::V8::AdjustAmountOfExternalAllocatedMemory (-sizeof (Type));

	context -> removeObject (child);
}

} // GoogleV8
} // X3D
} // titania

#endif
