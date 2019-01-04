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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

template <class InternalType>
struct ArrayValue :
	public X3D::X3DBase
{
	InternalType* array;
	size_t        index;
};

template <class ValueType, class InternalType>
class Array :
	public X3DArrayField
{
public:

	///  @name Member types

	using internal_type = InternalType;
	using value_type    = ValueType;
	using single_type   = typename ValueType::internal_type;

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
	bool
	enumerate (pb::pbObject* const, const pb::EnumerateType, std::string &, void* &);

	static
	bool
	hasProperty (pb::pbObject* const, const pb::Identifier &);

	static
	void
	set1Value (pb::pbObject* const, const pb::Identifier &, const pb::var &);

	static
	pb::var
	get1Value (pb::pbObject* const, const pb::Identifier &);

	///  @name Properties

	static
	pb::var
	setLength (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	getLength (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  throws pb::pbError
	template <class Class>
	static
	typename std::enable_if_t <
		not (std::is_integral_v <typename Class::internal_type::internal_type> or
		     std::is_floating_point_v <typename Class::internal_type::internal_type> or
		     std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		     std::is_same_v <typename Class::internal_type::internal_type, X3D::String>),
		typename Class::internal_type &
	>
	get1Argument (const pb::var & value)
	{
		return *peaseblossom::get1Argument <Class> (value);
	}

	///  throws pb::pbError
	template <class Class>
	static
	typename std::enable_if_t <
		std::is_integral_v <typename Class::internal_type::internal_type> or
		std::is_floating_point_v <typename Class::internal_type::internal_type> or
		std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type::internal_type, X3D::String>,
		typename Class::internal_type::internal_type
	>
	get1Argument (const pb::var & value)
	{
		return peaseblossom::get1Argument <typename Class::internal_type::internal_type> (value);
	}

	///  throws pb::pbError
	template <class Class>
	static
	typename std::enable_if_t <
		std::is_integral_v <typename Class::internal_type> or
		std::is_floating_point_v <typename Class::internal_type> or
		std::is_same_v <typename Class::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type, X3D::String>,
		typename Class::internal_type
	>
	get1Argument (const pb::var & value)
	{
		return peaseblossom::get1Argument <typename Class::internal_type> (value);
	}
	
	template <class Class>
	static
	typename std::enable_if_t <
		std::is_integral_v <Class> or
		std::is_floating_point_v <Class> or
		std::is_same_v <Class, X3D::String> or
		std::is_base_of_v <X3D::X3DFieldDefinition, Class>,
		const Class &
	>
	check (InternalType* const array, const size_t index, const Class & value)
	{
		return value;
	}

	template <class Class>
	static
	typename std::enable_if_t <
		not (std::is_integral_v <Class> or
		     std::is_floating_point_v <Class> or
		     std::is_same_v <Class, X3D::String> or
		     std::is_base_of_v <X3D::X3DFieldDefinition, Class>),
		const single_type &
	>
	check (InternalType* const array, const size_t index, const Class & value)
	{
		const auto field = new single_type (value);
		return *field;
	}

	///  @name Static members

	static const std::string typeName;
	static const pb::Callbacks callbacks;

};

template <class ValueType, class InternalType>
pb::ptr <pb::NativeFunction>
Array <ValueType, InternalType>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 0);
	const auto prototype = context -> getClass (ObjectType::X3DArrayField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	prototype -> addOwnProperty ("length",
	                             pb::undefined,
	                             pb::NONE,
	                             new pb::NativeFunction (ec, "length", getLength, 0),
	                             new pb::NativeFunction (ec, "length", setLength, 1));

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

template <class ValueType, class InternalType>
pb::var
Array <ValueType, InternalType>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
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
				array -> emplace_back (get1Argument <ValueType> (args [i]));

			setUserData <Array> (ec, object, array);
			break;
		}
	}

	return pb::undefined;
}

template <class ValueType, class InternalType>
bool
Array <ValueType, InternalType>::enumerate (pb::pbObject* const object, const pb::EnumerateType type, std::string & propertyName, void* & userData)
{
	const auto array = getObject <InternalType> (object);

	switch (type)
	{
		case pb::ENUMERATE_BEGIN:
		{
			if (array -> empty ())
				return false;

			userData = new size_t (0);
			return true;
		}
		case pb::ENUMERATE:
		{
			auto & index = *static_cast <size_t*> (userData);

			if (index < array -> size ())
			{
				propertyName = basic::to_string (index, std::locale::classic ());
				index       += 1;
				return true;
			}

			return false;
		}
		case pb::ENUMERATE_END:
		{
			delete static_cast <size_t*> (userData);
			return false;
		}
	}

	return false;
}

template <class ValueType, class InternalType>
bool
Array <ValueType, InternalType>::hasProperty (pb::pbObject* const object, const pb::Identifier & identifier)
{
	const auto index = identifier .toUInt32 ();

	if (index == pb::PROPERTY)
		return false;

	const auto array = getObject <InternalType> (object);

	return index < array -> size ();
}

template <class ValueType, class InternalType>
void
Array <ValueType, InternalType>::set1Value (pb::pbObject* const object, const pb::Identifier & identifier, const pb::var & value)
{
	try
	{
		const auto index = identifier .toUInt32 ();

		if (index == pb::PROPERTY)
			throw std::out_of_range ("Array::set1Value");

		const auto array = getObject <InternalType> (object);

		array -> set1Value (index, get1Argument <ValueType> (value));
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ".prototype[" + identifier .getString () + "]: out of memory.");
	}
}

template <class ValueType, class InternalType>
pb::var
Array <ValueType, InternalType>::get1Value (pb::pbObject* const object, const pb::Identifier & identifier)
{
	try
	{
		const auto index = identifier .toUInt32 ();

		if (index == pb::PROPERTY)
			throw std::out_of_range ("Array::get1Value");

		const auto context = getContext (object);
		const auto array   = getObject <InternalType> (object);

		return get <ValueType> (context, check <typename InternalType::value_type> (array, index, array -> get1Value (index)));
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ".prototype[" + identifier .getString () + "]: out of memory.");
	}
}

template <class ValueType, class InternalType>
pb::var
Array <ValueType, InternalType>::setLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs  = getThis <Array <ValueType, InternalType>>  (ec, object);
		const auto size = args [0] .toUInt32 ();

		if (basic::to_string (size, std::locale::classic ()) == args [0] .toString ())
		{
			lhs -> resize (size);

			return pb::undefined;
		}

		throw pb::RangeError ("Invalid array length.");
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.length is not generic.");
	}
	catch (const std::bad_alloc &)
	{
		throw pb::RuntimeError (getTypeName () + ".prototype.length: out of memory.");
	}
}

template <class ValueType, class InternalType>
pb::var
Array <ValueType, InternalType>::getLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs = getThis <Array <ValueType, InternalType>>  (ec, object);

		return lhs -> size ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.length is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania

#endif
