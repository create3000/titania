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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_X3DARRAY_FIELD_H__

#include "Context.h"
#include "String.h"
#include "X3DField.h"

#include <v8.h>

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class T, class S>
class X3DArrayField :
	public X3DField <T>
{
public:

	using value_type  = T;
	using single_type = S;

	using X3DField <T>::TypeName;
	using X3DField <T>::Type;

	static
	v8::Local <v8::FunctionTemplate>
	initialize (const v8::Local <v8::External> &);


private:

	using X3DField <T>::createFunctionTemplate;
	using X3DField <T>::getPropertyAttributes;
	using X3DField <T>::getFunctionAttributes;
	using X3DField <T>::addProperty;
	using X3DField <T>::addObject;
	using X3DField <T>::getObject;
	using X3DField <T>::getName;
	using X3DField <T>::getTypeName;
	using X3DField <T>::getType;
	using X3DField <T>::isReadable;
	using X3DField <T>::isWritable;
	using X3DField <T>::toString;

	///  @name Construction

	static
	v8::Handle <v8::Value>
	construct (const v8::Arguments &);

	///  @name Member access

	static
	v8::Handle <v8::Integer>
	hasIndex (uint32_t, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	set1Value (uint32_t, v8::Local <v8::Value>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	get1Value (uint32_t, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Array>
	getIndices (const v8::AccessorInfo &);

	///  @name Properties

	static
	void
	length (v8::Local <v8::String>, v8::Local <v8::Value>, const v8::AccessorInfo &);

	static
	v8::Handle <v8::Value>
	length (v8::Local <v8::String>, const v8::AccessorInfo &);

};

template <class T, class S>
v8::Local <v8::FunctionTemplate>
X3DArrayField <T, S>::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetIndexedPropertyHandler (get1Value, set1Value, hasIndex, nullptr, getIndices, context);
	instanceTemplate -> SetAccessor (String ("length"), length, length, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());

	instanceTemplate -> Set (String ("getName"),     v8::FunctionTemplate::New (getName,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"), v8::FunctionTemplate::New (getTypeName, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),     v8::FunctionTemplate::New (getType,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),  v8::FunctionTemplate::New (isReadable,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),  v8::FunctionTemplate::New (isWritable,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("toString"),    v8::FunctionTemplate::New (toString,    context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

template <class T, class S>
v8::Handle <v8::Value>
X3DArrayField <T, S>::construct (const v8::Arguments & args)
{
	if (args .IsConstructCall ())
	{
		const auto context = getContext (args);
		const auto object  = args .This ();

		if (args .Length ())
		{
			const auto arg = args [0];

			if (arg -> IsExternal ())
			{
				const auto value = getObject (arg);

				if (value -> getParents () .empty ())
					addObject (context, object, value);
				else
					addProperty (context, object, value);

				return v8::Undefined ();
			}
		}

		addObject (context, object, new T ());

		for (size_t i = 0, size = args .Length (); i < size; ++ i)
			object -> Set (i, args [i]);

		return v8::Undefined ();
	}

	return v8::ThrowException (String ("RuntimeError: cannot call constructor as function."));
}

template <class T, class S>
v8::Handle <v8::Integer>
X3DArrayField <T, S>::hasIndex (uint32_t index, const v8::AccessorInfo & info)
{
	const auto array = getObject (info);

	if (index < array -> size ())
		return v8::Integer::New (index);

	return v8::Handle <v8::Integer> ();
}

template <class T, class S>
v8::Handle <v8::Value>
X3DArrayField <T, S>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		const auto context = getContext (info);
		const auto array   = getObject (info);

		if (context -> getClass (S::Type ()) -> HasInstance (value))
		{
			array -> set1Value (index, *getArgument <typename S::value_type> (value));

			return value;
		}

		return v8::ThrowException (String ("RuntimeError: couldn't assign value for index " + std::to_string (index) + ", value has wrong type, must be " + S::TypeName () + "."));
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <class T, class S>
v8::Handle <v8::Value>
X3DArrayField <T, S>::get1Value (uint32_t index, const v8::AccessorInfo & info)
{
	try
	{
		const auto context = getContext (info);
		const auto array   = getObject (info);

		return S::create (context, &array -> get1Value (index));
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <class T, class S>
v8::Handle <v8::Array>
X3DArrayField <T, S>::getIndices (const v8::AccessorInfo & info)
{
	const auto array   = getObject (info);
	const auto indices = v8::Array::New ();

	for (size_t index = 0, size = array -> size (); index < size; ++ index)
		indices -> Set (index, v8::Number::New (index));

	return indices;
}

template <class T, class S>
void
X3DArrayField <T, S>::length (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> resize (value -> ToUint32 () -> Value ());
	}
	catch (const std::bad_alloc &)
	{
		//return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <class T, class S>
v8::Handle <v8::Value>
X3DArrayField <T, S>::length (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	return v8::Uint32::New (getObject (info) -> size ());
}

} // GoogleV8
} // X3D
} // titania

#endif
