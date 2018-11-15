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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_ARGUMENTS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_ARGUMENTS_H__

#include "Context.h"
#include "ObjectType.h"

#include <Titania/PeaseBlossom/pb.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

///  throws std::out_of_range
inline
Context*
getContext (pb::pbObject* const object)
{
	return object -> getUserData <Context*> (0);
}

///  throws std::out_of_range
inline
Context*
getContext (pb::ptr <pb::pbObject> const object)
{
	return getContext (object .get ());
}

///  throws std::out_of_range
inline
Context*
getContext (const pb::ptr <pb::pbExecutionContext> & ec)
{
	return ec -> getUserData <Context*> (0);
}

///  throws std::out_of_range
template <class Type>
inline
Type*
getObject (pb::pbObject* const object)
{
	return object -> getUserData <Type*> (1);
}

///  throws std::out_of_range
template <class Type>
inline
Type*
getObject (const pb::ptr <pb::pbObject> & object)
{
	return getObject <Type> (object .get ());
}

///  throws std::out_of_range
template <class Type>
inline
Type*
getObject (const pb::var & value)
{
	return getObject <Type> (value .getObject () .get ());
}

///  throws std::invalid_argument
template <class Class>
inline
typename Class::internal_type*
getThis (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & value)
{
	try
	{
		const auto context = getContext (ec);

		if (context -> getClass (Class::getType ()) -> hasInstance (value))
			return getObject <typename Class::internal_type> (value);
	}
	catch (const std::out_of_range &)
	{ }

	throw std::invalid_argument (Class::getTypeName ());
}

///  throws pb::pbError
template <class Class>
typename std::enable_if <
	not (std::is_integral <typename Class::internal_type::internal_type>::value or
        std::is_floating_point <typename Class::internal_type::internal_type>::value or
        std::is_same <typename Class::internal_type::internal_type, std::string>::value or
        std::is_same <typename Class::internal_type::internal_type, X3D::String>::value),
	typename Class::internal_type*
>::type
get1Argument (const pb::var & value)
{
	try
	{
		if (value .isObject ())
		{
			const auto & object  = value .getObject ();
			const auto   context = getContext (object);

			if (context -> getClass (Class::getType ()) -> hasInstance (object))
				return getObject <typename Class::internal_type> (object);
		}
	}
	catch (const std::out_of_range &)
	{ }

	throw pb::TypeError ("Type of value is invalid, must be " + Class::getTypeName () + ".");
}

///  throws pb::pbError
template <class Class>
inline
typename std::enable_if <
	not (std::is_integral <typename Class::internal_type::internal_type>::value or
        std::is_floating_point <typename Class::internal_type::internal_type>::value or
        std::is_same <typename Class::internal_type::internal_type, std::string>::value or
        std::is_same <typename Class::internal_type::internal_type, X3D::String>::value),
	typename Class::internal_type*
>::type
get1Argument (const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		return get1Argument <Class> (args [index]);
	}
	catch (const pb::TypeError &)
	{
		throw pb::TypeError ("Type of argument " + std::to_string (index + 1) + " is invalid, must be " + Class::getTypeName () + ".");
	}
}

///  throws pb::pbError
template <class Type>
inline
typename std::enable_if <
	std::is_integral <Type>::value or
   std::is_floating_point <Type>::value or
   std::is_same <Type, std::string>::value or
   std::is_same <Type, X3D::String>::value,
	Type
>::type
get1Argument (const pb::var & value)
{
	throw pb::TypeError ("get1Argument");
}

///  throws pb::pbError
template <>
inline
bool
get1Argument <bool> (const pb::var & value)
{
	return value .toBoolean ();
}

///  throws pb::pbError
template <>
inline
double
get1Argument <double> (const pb::var & value)
{
	return value .toNumber ();
}

///  throws pb::pbError
template <>
inline
float
get1Argument <float> (const pb::var & value)
{
	return value .toNumber ();
}

///  throws pb::pbError
template <>
inline
int32_t
get1Argument <int32_t> (const pb::var & value)
{
	return value .toInt32 ();
}

///  throws pb::pbError
template <>
inline
uint32_t
get1Argument <uint32_t> (const pb::var & value)
{
	return value .toUInt32 ();
}

///  throws pb::pbError
template <>
inline
std::string
get1Argument <std::string> (const pb::var & value)
{
	return value .toString ();
}

///  throws pb::pbError
template <>
inline
X3D::String
get1Argument <X3D::String> (const pb::var & value)
{
	return value .toString ();
}

///  throws pb::pbError
template <class Type>
inline
typename std::enable_if <
	std::is_integral <Type>::value or
   std::is_floating_point <Type>::value or
   std::is_same <Type, std::string>::value or
   std::is_same <Type, X3D::String>::value,
	Type
>::type
get1Argument (const std::vector <pb::var> & args, const size_t index)
{
	return get1Argument <Type> (args [index]);
}

} // peaseblossom
} // X3D
} // titania

#endif
