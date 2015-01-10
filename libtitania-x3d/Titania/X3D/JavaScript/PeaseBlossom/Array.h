/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_ARRAY_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_ARRAY_H__

#include "X3DArrayField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class Type, class InternalType>
class Array :
	public X3DArrayField
{
public:

	///  @name Member types

	using internal_type = InternalType;
	using value_type    = Type;
	using single_type   = typename InternalType::value_type;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ throw std::invalid_argument ("Array::getType"); }

	static
	const std::string &
	getTypeName ()
	{ return typeName; }

	static
	const pb::Callbacks &
	getCallbacks ()
	{ return callbacks; }

	///  @name Construction

	static
	pb::ptr <pb::NativeFunction>
	initialize (Context* const, const pb::ptr <pb::Program> &);


private:

	///  @name Construction

	static
	pb::var
	construct (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Member access

	static
	void
	set1Value (pb::pbObject* const, const pb::Identifier &, const pb::var &);

	static
	pb::var
	get1Value (pb::pbObject* const, const pb::Identifier &);

	///  @name Properties

	static
	pb::var
	getLength (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	setLength (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	template <class Class>
	static
	typename std::enable_if <
		not (std::is_integral <typename Class::internal_type::internal_type>::value or
	        std::is_floating_point <typename Class::internal_type::internal_type>::value or
	        std::is_same <typename Class::internal_type::internal_type, std::string>::value or
	        std::is_same <typename Class::internal_type::internal_type, X3D::String>::value),
		typename Class::internal_type &
	>::type
	get1Argument (const pb::var & value)
	throw (pb::pbException)
	{
		return *peaseblossom::get1Argument <Class> (value);
	}

	///  @name Static members

	static const std::string   typeName;
	static const pb::Callbacks callbacks;

};

template <class Type, class InternalType>
pb::ptr <pb::NativeFunction>
Array <Type, InternalType>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 4);
	const auto prototype = context -> getClass (ObjectType::X3DArrayField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	prototype -> addOwnProperty ("length",
	                             pb::Undefined,
	                             pb::NONE,
	                             new pb::NativeFunction (ec, "length", getLength, 0),
	                             new pb::NativeFunction (ec, "length", setLength, 1));

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

template <class Type, class InternalType>
pb::var
Array <Type, InternalType>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <Array> (ec, object, new InternalType ());
			break;
		}
		default:
		{
			const auto array = new InternalType ();

			for (size_t i = 0, size = args .size (); i < size; ++ i)
				array -> emplace_back (get1Argument <Type> (args [i]));

			setUserData <Array> (ec, object, array);
			break;
		}
	}

	return pb::Undefined;
}

template <class Type, class InternalType>
void
Array <Type, InternalType>::set1Value (pb::pbObject* const object, const pb::Identifier & propertyName, const pb::var & value)
{
	try
	{
		const auto index = propertyName .toUInt32 ();

		if (propertyName .isIndex (index))
		{
			const auto array = getObject <InternalType> (object);

			array -> set1Value (index, get1Argument <Type> (value));

			return;
		}

		throw std::out_of_range ("Array::set1Value");
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ": out of memory.");
	}
}

template <class Type, class InternalType>
pb::var
Array <Type, InternalType>::get1Value (pb::pbObject* const object, const pb::Identifier & propertyName)
{
	try
	{
		const auto index = propertyName .toUInt32 ();

		if (propertyName .isIndex (index))
		{
			const auto context = getContext (object);
			const auto array   = getObject <InternalType> (object);

			return get <Type> (context, &array -> get1Value (index));
		}

		throw std::out_of_range ("Array::get1Value");
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ": out of memory.");
	}
}

template <class Type, class InternalType>
pb::var
Array <Type, InternalType>::setLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs  = getObject <InternalType> (object);
		const auto size = args [0] .toUInt32 ();

		if (basic::to_string (size) == args [0] .toString ())
		{
			lhs -> resize (size);

			return pb::Undefined;
		}

		throw pb::RangeError ("Invalid array length.");
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ".prototype.length: out of memory.");
	}
}

template <class Type, class InternalType>
pb::var
Array <Type, InternalType>::getLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto lhs = getObject <InternalType> (object);

	return lhs -> size ();
}

} // peaseblossom
} // X3D
} // titania

#endif
