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
#include "ObjectType.h"
#include "String.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class T>
class X3DObject
{
public:

	///  @name Member access
	
	static
	const std::string &
	TypeName ()
	{ return typeName; }
	
	static
	ObjectType
	Type ()
	{ return type; }

	///  @name Construction
	
	static
	v8::Handle <v8::Value>
	create (Context* const, T* const)
	throw (std::out_of_range);


protected:

	///  @name Construction

	static
	v8::Local <v8::FunctionTemplate>
	createFunctionTemplate (const v8::Local <v8::External> & context, v8::InvocationCallback);

	///  @name Attributes

	static
	constexpr v8::PropertyAttribute
	getPropertyAttributes ()
	{ return v8::PropertyAttribute (v8::DontDelete | v8::DontEnum); }

	static
	constexpr v8::PropertyAttribute
	getFunctionAttributes ()
	{ return v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum); }

	///  @name Object access

	static
	void
	addProperty (Context* const, const v8::Local <v8::Object> &, T* const);

	static
	void
	addObject (Context* const, const v8::Local <v8::Object> &, T* const);

	static
	T*
	getObject (const v8::AccessorInfo & info)
	throw (std::out_of_range)
	{ return getObject (info .This ()); }

	static
	T*
	getObject (const v8::Arguments & args)
	throw (std::invalid_argument,
          std::out_of_range)
   { return getObject (getContext (args), args); }

	static
	T*
	getObject (Context* const, const v8::Arguments &)
	throw (std::invalid_argument,
          std::out_of_range);

	///  @name Object access

	static
	T*
	getObject (const v8::Local <v8::Object> &)
	throw (std::out_of_range);

	static
	T*
	getObject (const v8::Handle <v8::Value> & value)
	{ return static_cast <T*> (v8::Handle <v8::External>::Cast (value) -> Value ()); }

	///  @name Functions

	static
	v8::Handle <v8::Value>
	getName (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getTypeName (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	toString (const v8::Arguments &);

	///  @name Destruction

	static
	void
	finalizeProperty (v8::Persistent <v8::Value>, void*);

	static
	void
	finalize (v8::Persistent <v8::Value>, void*);


private:
	
	///  @name Static members

	static const std::string typeName;
	static const ObjectType  type;

};

template <class T>
v8::Handle <v8::Value>
X3DObject <T>::create (Context* const context, T* const field)
throw (std::out_of_range)
{
	try
	{
		return context -> getObject (field);
	}
	catch (const std::out_of_range &)
	{
		return context -> createObject (type, field);
	}
}

template <class T>
v8::Local <v8::FunctionTemplate>
X3DObject <T>::createFunctionTemplate (const v8::Local <v8::External> & context, v8::InvocationCallback callback)
{
	const auto className        = String (typeName);
	const auto functionTemplate = v8::FunctionTemplate::New ();

	functionTemplate -> SetCallHandler (callback, context);	
	functionTemplate -> SetClassName (className);

	functionTemplate -> InstanceTemplate () -> SetInternalFieldCount (1);

	return functionTemplate;
}

template <class T>
void
X3DObject <T>::addProperty (Context* const context, const v8::Local <v8::Object> & object, T* const field)
{
	object -> SetInternalField (0, v8::External::New (field));

	auto persistent = v8::Persistent <v8::Object>::New (object);

	persistent .MakeWeak (context, finalizeProperty);

	context -> addObject (field, persistent);
}

template <class T>
void
X3DObject <T>::addObject (Context* const context, const v8::Local <v8::Object> & object, T* const field)
{
	object -> SetInternalField (0, v8::External::New (field));

	auto persistent = v8::Persistent <v8::Object>::New (object);

	persistent .MakeWeak (field, finalize);
}

template <class T>
T*
X3DObject <T>::getObject (Context* const context, const v8::Arguments & args)
throw (std::invalid_argument,
       std::out_of_range)
{
	const auto object = args .This ();

	if (context -> getClass (type) -> HasInstance (object))
		return getObject (object);

	throw std::invalid_argument ("RuntimeError: function must be called with object of type " + typeName + ".");
}

template <class T>
T*
X3DObject <T>::getObject (const v8::Local <v8::Object> & object)
throw (std::out_of_range)
{
	const auto value = object -> GetInternalField (0);

	if (value -> IsExternal ())
		return getObject (value);
	
	throw std::out_of_range ("getObject");
}

template <class T>
v8::Handle <v8::Value>
X3DObject <T>::getName (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getTypeName: wrong number of arguments."));

		const auto lhs = getObject (args);

		return String (lhs -> getName ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
X3DObject <T>::getTypeName (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getTypeName: wrong number of arguments."));

		const auto lhs = getObject (args);

		return String (lhs -> getTypeName ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
X3DObject <T>::toString (const v8::Arguments & args)
{
	try
	{
		const auto field = getObject (args);

		X3D::Generator::NicestStyle ();

		return String (field -> toString ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
void
X3DObject <T>::finalizeProperty (v8::Persistent <v8::Value> value, void* parameter)
{
	const auto context = static_cast <Context*> (parameter);
	const auto field   = getObject (value -> ToObject ());

	context -> removeObject (field);
}

template <class T>
void
X3DObject <T>::finalize (v8::Persistent <v8::Value> value, void* parameter)
{
	const auto object = static_cast <T*> (parameter);
	
	object -> dispose ();
	object -> addDisposedObject (object);

	__LOG__ << object -> getTypeName () << std::endl;
}

template <class T>
inline
T*
getArgument (const v8::Local <v8::Value> & value)
{
	return static_cast <T*> (v8::Handle <v8::External>::Cast (value -> ToObject () -> GetInternalField (0)) -> Value ());
}

template <class T>
typename T::value_type*
getArgument (Context* const context, const v8::Arguments & args, const size_t index)
throw (std::invalid_argument,
       std::out_of_range)
{
	const auto value = args [index];

	if (context -> getClass (T::Type ()) -> HasInstance (value))
		return getArgument <typename T::value_type> (value);

	throw std::invalid_argument ("RuntimeError: parameter " + std::to_string (index + 1) + " has wrong type, must be " + T::TypeName () + ".");
}

} // GoogleV8
} // X3D
} // titania

#endif
