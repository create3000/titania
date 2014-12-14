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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_SFMATRIX4_H__

#include <v8.h>

#include "../Context.h"
#include "../X3DField.h"
#include "SFRotation4.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class T>
class SFMatrix4 :
	public X3DField <T>
{
public:

	using value_type = T;

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

	///  @name Functions

	static
	v8::Handle <v8::Value>
	setTransform (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	getTransform (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	transpose (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	inverse (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multLeft (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multRight (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multVecMatrix (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multMatrixVec (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multDirMatrix (const v8::Arguments &);

	static
	v8::Handle <v8::Value>
	multMatrixDir (const v8::Arguments &);

};

template <class T>
v8::Local <v8::FunctionTemplate>
SFMatrix4 <T>::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetIndexedPropertyHandler (get1Value, set1Value, hasIndex, nullptr, getIndices);

	instanceTemplate -> Set (String ("getName"),       v8::FunctionTemplate::New (getName,       context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"),   v8::FunctionTemplate::New (getTypeName,   context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),       v8::FunctionTemplate::New (getType,       context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),    v8::FunctionTemplate::New (isReadable,    context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),    v8::FunctionTemplate::New (isWritable,    context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("setTransform"),  v8::FunctionTemplate::New (setTransform,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTransform"),  v8::FunctionTemplate::New (getTransform,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("transpose"),     v8::FunctionTemplate::New (transpose,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("inverse"),       v8::FunctionTemplate::New (inverse,       context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multLeft"),      v8::FunctionTemplate::New (multLeft,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multRight"),     v8::FunctionTemplate::New (multRight,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multVecMatrix"), v8::FunctionTemplate::New (multVecMatrix, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multMatrixVec"), v8::FunctionTemplate::New (multMatrixVec, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multDirMatrix"), v8::FunctionTemplate::New (multDirMatrix, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("multMatrixDir"), v8::FunctionTemplate::New (multMatrixDir, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("toString"),      v8::FunctionTemplate::New (toString,      context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::construct (const v8::Arguments & args)
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
			case 16:
			{
				addObject (context, object, new T (args [ 0] -> ToNumber () -> Value (),
				                                   args [ 1] -> ToNumber () -> Value (),
				                                   args [ 2] -> ToNumber () -> Value (),
				                                   args [ 3] -> ToNumber () -> Value (),
				                                   args [ 4] -> ToNumber () -> Value (),
				                                   args [ 5] -> ToNumber () -> Value (),
				                                   args [ 6] -> ToNumber () -> Value (),
				                                   args [ 7] -> ToNumber () -> Value (),
				                                   args [ 8] -> ToNumber () -> Value (),
				                                   args [ 9] -> ToNumber () -> Value (),
				                                   args [10] -> ToNumber () -> Value (),
				                                   args [11] -> ToNumber () -> Value (),
				                                   args [12] -> ToNumber () -> Value (),
				                                   args [13] -> ToNumber () -> Value (),
				                                   args [14] -> ToNumber () -> Value (),
				                                   args [15] -> ToNumber () -> Value ()));
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
SFMatrix4 <T>::hasIndex (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Integer::New (index);

	return v8::Handle <v8::Integer> ();
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::get1Value (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Number::New (getObject (info) -> get1Value (index));

	return v8::ThrowException (v8::String::New ("RuntimeError: index out of range."));
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
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
SFMatrix4 <T>::getIndices (const v8::AccessorInfo & info)
{
	const auto indices = v8::Array::New ();

	for (size_t index = 0; index < T::internal_type::size (); ++ index)
		indices -> Set (index, v8::Number::New (index));

	return indices;
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::setTransform (const v8::Arguments & args)
{
	try
	{
		if (args .Length () > 5)
			return v8::ThrowException (String (TypeName () + ".setTransform: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		typename T::vector3_type    translation;
		typename T::rotation4f_type rotation;
		typename T::vector3_type    scale (1, 1, 1);
		typename T::rotation4f_type scaleOrientation;
		typename T::vector3_type    center;

		try
		{
			translation = *getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0);
		}
		catch (const std::exception &)
		{ }

		try
		{
			rotation = *getArgument <SFRotation4 <typename T::rotation4f_type>> (context, args, 1);
		}
		catch (const std::exception &)
		{ }

		try
		{
			scale = *getArgument <SFVec3 <typename T::vector3_type>> (context, args, 2);
		}
		catch (const std::exception &)
		{ }

		try
		{
			scaleOrientation = *getArgument <SFRotation4 <typename T::rotation4f_type>> (context, args, 3);
		}
		catch (const std::exception &)
		{ }

		try
		{
			center = *getArgument <SFVec3 <typename T::vector3_type>> (context, args, 4);
		}
		catch (const std::exception &)
		{ }

		lhs -> setTransform (translation, rotation, scale, scaleOrientation, center);

		return v8::Undefined ();
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::getTransform (const v8::Arguments & args)
{
	try
	{
		if (args .Length () > 5)
			return v8::ThrowException (String (TypeName () + ".getTransform: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		typename T::vector3_type    translation;
		typename T::rotation4f_type rotation;
		typename T::vector3_type    scale;
		typename T::rotation4f_type scaleOrientation;
		typename T::vector3_type    center;

		try
		{
			center = *getArgument <SFVec3 <typename T::vector3_type>> (context, args, 4);
		}
		catch (const std::exception &)
		{ }

		lhs -> getTransform (translation, rotation, scale, scaleOrientation, center);

		try
		{
			getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0) -> setValue (translation);
		}
		catch (const std::exception &)
		{ }

		try
		{
			getArgument <SFRotation4 <typename T::rotation4f_type>> (context, args, 1) -> setValue (rotation);
		}
		catch (const std::exception &)
		{ }

		try
		{
			getArgument <SFVec3 <typename T::vector3_type>> (context, args, 2) -> setValue (scale);
		}
		catch (const std::exception &)
		{ }

		try
		{
			getArgument <SFRotation4 <typename T::rotation4f_type>> (context, args, 3) -> setValue (scaleOrientation);
		}
		catch (const std::exception &)
		{ }

		return v8::Undefined ();
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::transpose (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".transpose: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		return context -> createObject (Type (), lhs -> transpose ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::inverse (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".inverse: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		return context -> createObject (Type (), lhs -> inverse ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multLeft (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multLeft: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFMatrix4> (context, args, 0);

		return context -> createObject (Type (), lhs -> multLeft (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multRight (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multRight: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFMatrix4> (context, args, 0);

		return context -> createObject (Type (), lhs -> multRight (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multVecMatrix (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multVecMatrix: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0);

		return context -> createObject (Type (), lhs -> multVecMatrix (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multMatrixVec (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multMatrixVec: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0);

		return context -> createObject (Type (), lhs -> multMatrixVec (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multDirMatrix (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multDirMatrix: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0);

		return context -> createObject (Type (), lhs -> multDirMatrix (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

template <class T>
v8::Handle <v8::Value>
SFMatrix4 <T>::multMatrixDir (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 1)
			return v8::ThrowException (String (TypeName () + ".multMatrixDir: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto rhs     = getArgument <SFVec3 <typename T::vector3_type>> (context, args, 0);

		return context -> createObject (Type (), lhs -> multMatrixDir (*rhs));
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

extern template class SFMatrix4 <X3D::SFMatrix4d>;
extern template class SFMatrix4 <X3D::SFMatrix4f>;

using SFMatrix4d = SFMatrix4 <X3D::SFMatrix4d>;
using SFMatrix4f = SFMatrix4 <X3D::SFMatrix4f>;

} // GoogleV8
} // X3D
} // titania

#endif
