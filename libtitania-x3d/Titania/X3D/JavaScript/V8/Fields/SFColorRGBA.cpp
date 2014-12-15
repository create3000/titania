/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 *	instanceTemplate -> Set (String ("toString"),    v8::FunctionTemplate::New (toString,    context) -> GetFunction (), getFunctionAttributes ());

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

#include "SFColorRGBA.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <>
const std::string X3DObject <X3D::SFColorRGBA>::typeName = "SFColorRGBA";

template <>
const ObjectType X3DObject <X3D::SFColorRGBA>::type = ObjectType::SFColorRGBA;

v8::Local <v8::FunctionTemplate>
SFColorRGBA::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetIndexedPropertyHandler (get1Value, set1Value, hasIndex, nullptr, getIndices);
	instanceTemplate -> SetAccessor (String ("r"), r, r, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("g"), g, g, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("b"), b, b, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("a"), a, a, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());

	instanceTemplate -> Set (String ("getName"),     v8::FunctionTemplate::New (getName,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"), v8::FunctionTemplate::New (getTypeName, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),     v8::FunctionTemplate::New (getType,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),  v8::FunctionTemplate::New (isReadable,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),  v8::FunctionTemplate::New (isWritable,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("setHSV"),      v8::FunctionTemplate::New (setHSV,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getHSV"),      v8::FunctionTemplate::New (getHSV,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("toString"),    v8::FunctionTemplate::New (toString,    context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

v8::Handle <v8::Value>
SFColorRGBA::construct (const v8::Arguments & args)
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
			case 4:
			{
				addObject (context, object, new T (args [0] -> ToNumber () -> Value (),
				                                   args [1] -> ToNumber () -> Value (),
				                                   args [2] -> ToNumber () -> Value (),
				                                   args [3] -> ToNumber () -> Value ()));
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

	return v8::ThrowException (String ("RuntimeError: cannot call constructor as function."));
}

v8::Handle <v8::Integer>
SFColorRGBA::hasIndex (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Integer::New (index);

	return v8::Handle <v8::Integer> ();
}

v8::Handle <v8::Value>
SFColorRGBA::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());
		return value;
	}

	return v8::ThrowException (String ("RuntimeError: index out of range."));
}

v8::Handle <v8::Value>
SFColorRGBA::get1Value (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < T::internal_type::size ())
		return v8::Number::New (getObject (info) -> get1Value (index));

	return v8::ThrowException (String ("RuntimeError: index out of range."));
}

v8::Handle <v8::Array>
SFColorRGBA::getIndices (const v8::AccessorInfo & info)
{
	const auto indices = v8::Array::New ();

	for (size_t index = 0; index < T::internal_type::size (); ++ index)
		indices -> Set (index, v8::Number::New (index));

	return indices;
}

v8::Handle <v8::Value>
SFColorRGBA::setHSV (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 3)
			return v8::ThrowException (String (TypeName () + ".setHSV: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto h       = args [0] -> ToNumber () -> Value ();
		const auto s       = args [1] -> ToNumber () -> Value ();
		const auto v       = args [2] -> ToNumber () -> Value ();

		lhs -> setHSV (h, s, v);

		return v8::Undefined ();
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFColorRGBA::getHSV (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getHSV: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		float h, s, v;
		lhs -> getHSV (h, s, v);

		const auto array = v8::Array::New (3);

		array -> Set (0, v8::Number::New (h));
		array -> Set (1, v8::Number::New (s));
		array -> Set (2, v8::Number::New (v));

		return array;
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

} // GoogleV8
} // X3D
} // titania
