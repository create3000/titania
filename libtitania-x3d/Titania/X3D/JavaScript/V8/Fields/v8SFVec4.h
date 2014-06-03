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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_V8SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_V8SFVEC4_H__

#include <v8.h>

#include "../v8Context.h"
#include "../v8X3DField.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <class Type, ObjectType OBJECT_TYPE>
class SFVec4 :
	public X3DField <Type, OBJECT_TYPE>
{
public:

	static
	void
	initialize (Context* const);


private:

	using X3DField <Type, OBJECT_TYPE>::getObject;
	using X3DField <Type, OBJECT_TYPE>::createFunctionTemplate;
	using X3DField <Type, OBJECT_TYPE>::realize;
	using X3DField <Type, OBJECT_TYPE>::toString;

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

	static
	void
	w (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
	{ getObject (info) -> setW (value -> ToNumber () -> Value ()); }

	static
	v8::Handle <v8::Value>
	w (v8::Local <v8::String> property, const v8::AccessorInfo & info)
	{ return v8::Number::New (getObject (info) -> getW ()); }

};

template <class Type, ObjectType OBJECT_TYPE>
void
SFVec4 <Type, OBJECT_TYPE>::initialize (Context* const context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetIndexedPropertyHandler (get1Value, set1Value, hasIndex, nullptr, getIndices);
	instanceTemplate -> SetAccessor (make_v8_string ("x"), x, x, v8::Handle <v8::Value> (), v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete | v8::DontEnum));
	instanceTemplate -> SetAccessor (make_v8_string ("y"), y, y, v8::Handle <v8::Value> (), v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete | v8::DontEnum));
	instanceTemplate -> SetAccessor (make_v8_string ("z"), z, z, v8::Handle <v8::Value> (), v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete | v8::DontEnum));
	instanceTemplate -> SetAccessor (make_v8_string ("w"), w, w, v8::Handle <v8::Value> (), v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete | v8::DontEnum));

	instanceTemplate -> Set (make_v8_string ("toString"), v8::FunctionTemplate::New (toString) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));

	context -> addClass (OBJECT_TYPE, functionTemplate -> GetFunction ());
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Integer>
SFVec4 <Type, OBJECT_TYPE>::hasIndex (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < Type::internal_type::size ())
		return v8::Integer::New (index);

	return v8::Handle <v8::Integer> ();
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Value>
SFVec4 <Type, OBJECT_TYPE>::get1Value (uint32_t index, const v8::AccessorInfo & info)
{
	if (index < Type::internal_type::size ())
		return v8::Number::New (getObject (info) -> get1Value (index));

	return v8::ThrowException (v8::String::New ("RuntimeError: index out of range."));
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Value>
SFVec4 <Type, OBJECT_TYPE>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	if (index < Type::internal_type::size ())
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());
		return value;
	}

	return v8::ThrowException (v8::String::New ("RuntimeError: index out of range."));
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Array>
SFVec4 <Type, OBJECT_TYPE>::getIndices (const v8::AccessorInfo & info)
{
	const auto indices = v8::Array::New ();

	for (size_t index = 0; index < Type::internal_type::size (); ++ index)
		indices -> Set (v8::Number::New (index), v8::Number::New (index));

	return indices;
}

template <class Type, ObjectType OBJECT_TYPE>
v8::Handle <v8::Value>
SFVec4 <Type, OBJECT_TYPE>::construct (const v8::Arguments & args)
{
	__LOG__ << std::endl;

	if (args .IsConstructCall ())
	{
		const auto context = get_context (args);
		const auto object  = args .This ();

		switch (args .Length ())
		{
			case 0:
			{
				realize (context, object, new Type ());
				break;
			}
			case 4:
			{
				realize (context, object, new Type (args [0] -> ToNumber () -> Value (),
				                                    args [1] -> ToNumber () -> Value (),
				                                    args [2] -> ToNumber () -> Value (),
				                                    args [3] -> ToNumber () -> Value ()));
				break;
			}
			case 1:
			{
				if (args [0] -> IsExternal ())
				{
					realize (context, object, getObject (args [0]));
					break;
				}

				// Proceed with next case.
			}
			default:
				return v8::ThrowException (make_v8_string ("RuntimeError: wrong number of arguments."));
		}

		return v8::Undefined ();
	}

	return v8::ThrowException (v8::String::New ("RuntimeError: cannot call constructor as function."));
}

extern template class SFVec4 <X3D::SFVec4d, ObjectType::SFVec4d>;
extern template class SFVec4 <X3D::SFVec4f, ObjectType::SFVec4f>;

using SFVec4d = SFVec4 <X3D::SFVec4d, ObjectType::SFVec4d>;
using SFVec4f = SFVec4 <X3D::SFVec4f, ObjectType::SFVec4f>;

} // GoogleV8
} // X3D
} // titania

#endif
