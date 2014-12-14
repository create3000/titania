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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_SFVEC3_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_SFVEC3_H__

#include <v8.h>

#include "../Context.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class T>
class SFVec3 :
	public X3DField <T>
{
public:

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
	x (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
	{ getObject (info) -> setX (value -> ToNumber () -> Value ()); }

	static
	v8::Handle <v8::Value>
	x (v8::Local <v8::String> property, const v8::AccessorInfo & info)
	{ return v8::Number::New (getObject (info) -> getX ()); }

	static
	void
	y (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
	{ getObject (info) -> setY (value -> ToNumber () -> Value ()); }

	static
	v8::Handle <v8::Value>
	y (v8::Local <v8::String> property, const v8::AccessorInfo & info)
	{ return v8::Number::New (getObject (info) -> getY ()); }

	static
	void
	z (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
	{ getObject (info) -> setZ (value -> ToNumber () -> Value ()); }

	static
	v8::Handle <v8::Value>
	z (v8::Local <v8::String> property, const v8::AccessorInfo & info)
	{ return v8::Number::New (getObject (info) -> getZ ()); }

	///  @name Functions

	static
	v8::Handle <v8::Value>
	negate (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	add (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	subtract (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multiply (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multVec (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	divide (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	divVec (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	normalize (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	cross (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	dot (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	length (const v8::Arguments &);

};

template <class T>
v8::Local <v8::FunctionTemplate>
SFVec3 <T>::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetIndexedPropertyHandler (get1Value, set1Value, hasIndex, nullptr, getIndices);
	instanceTemplate -> SetAccessor (String ("x"), x, x, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("y"), y, y, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("z"), z, z, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());

	instanceTemplate -> Set (String ("getName"),     v8::FunctionTemplate::New (getName,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"), v8::FunctionTemplate::New (getTypeName, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),     v8::FunctionTemplate::New (getType,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),  v8::FunctionTemplate::New (isReadable,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),  v8::FunctionTemplate::New (isWritable,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("negate"),      v8::FunctionTemplate::New (negate,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("add"),         v8::FunctionTemplate::New (add,         context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("subtract"),    v8::FunctionTemplate::New (subtract,    context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multiply"),    v8::FunctionTemplate::New (multiply,    context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multVec"),     v8::FunctionTemplate::New (multVec,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("divide"),      v8::FunctionTemplate::New (divide,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("divVec"),      v8::FunctionTemplate::New (divVec,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("normalize"),   v8::FunctionTemplate::New (normalize,   context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("cross"),       v8::FunctionTemplate::New (cross,       context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("dot"),         v8::FunctionTemplate::New (dot,         context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("length"),      v8::FunctionTemplate::New (length,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("toString"),    v8::FunctionTemplate::New (toString,    context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::construct (const v8::Arguments & args)
{
	if (args .IsConstructCall ())
	{
		const auto context = getContext (args);
		const auto object  = args .This ();

		switch (args .Length ())
		{
			case 0:
			{
				addObject (context, object, new T ());
				break;
			}
			case 3:
			{
				addObject (context, object, new T (args [0] -> ToNumber () -> Value (),
				                                   args [1] -> ToNumber () -> Value (),
				                                   args [2] -> ToNumber () -> Value ()));
				break;
			}
			case 1:
			{
				const auto arg = args [0];

				if (arg -> IsExternal ())
				{
					const auto value = getObject (arg);

					if (value -> getParents () .empty ())
						addObject (context, object, value);
					else
						addProperty (context, object, value);

					break;
				}

				// Proceed with next case.
			}
			default:
				return v8::ThrowException (String ("RuntimeError: wrong number of arguments."));
		}

		return v8::Undefined ();
	}

	return v8::ThrowException (v8::String::New ("RuntimeError: cannot call constructor as function."));
}

template <class T>
v8::Handle <v8::Integer>
SFVec3 <T>::hasIndex (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Integer::New (index);

	return v8::Handle <v8::Integer> ();
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::get1Value (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Number::New (getObject (info) -> get1Value (index));

	return v8::ThrowException (v8::String::New ("RuntimeError: index out of range."));
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());
		return value;
	}

	return v8::ThrowException (v8::String::New ("RuntimeError: index out of range."));
}

template <class T>
v8::Handle <v8::Array>
SFVec3 <T>::getIndices (const v8::AccessorInfo & info)
{
	const auto indices = v8::Array::New ();

	for (size_t index = 0; index < T::internal_type::size (); ++ index)
		indices -> Set (index, v8::Number::New (index));

	return indices;
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::negate (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".negate: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);

		return context -> createObject (Type (), lhs -> negate ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::add (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".add: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return context -> createObject (Type (), lhs -> add (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::subtract (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".subtract: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return context -> createObject (Type (), lhs -> subtract (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::multiply (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multiply: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = args [0] -> ToNumber () -> Value ();

		return context -> createObject (Type (), lhs -> multiply (rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::multVec (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multVec: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return context -> createObject (Type (), lhs -> multiply (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::divide (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".divide: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = args [0] -> ToNumber () -> Value ();

		return context -> createObject (Type (), lhs -> divide (rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::divVec (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".divVec: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return context -> createObject (Type (), lhs -> divide (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::normalize (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".normalize: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);

		return context -> createObject (Type (), lhs -> normalize ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::cross (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".cross: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return context -> createObject (Type (), lhs -> cross (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::dot (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".dot: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (args);
		const auto rhs     = getArg <T> (context, Type (), args, 0);

		return v8::Number::New (lhs -> dot (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFVec3 <T>::length (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".length: wrong number of arguments."));

		const auto lhs = getObject (args);

		return v8::Number::New (lhs -> length ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

extern template class SFVec3 <X3D::SFVec3d>;
extern template class SFVec3 <X3D::SFVec3f>;

using SFVec3d = SFVec3 <X3D::SFVec3d>;
using SFVec3f = SFVec3 <X3D::SFVec3f>;

} // GoogleV8
} // X3D
} // titania

#endif
