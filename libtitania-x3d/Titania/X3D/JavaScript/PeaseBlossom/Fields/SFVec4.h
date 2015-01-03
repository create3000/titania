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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"

#include "../Arguments.h"
#include "../ObjectType.h"
#include <Titania/PeaseBlossom/pb.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class Type>
class SFVec4
{
public:

	static
	constexpr ObjectType
	getType ()
	{ throw std::invalid_argument ("SFVec4::getType"); }

	static
	const pb::Identifier &
	getIdentifier ()
	{ return identifier; }

	static
	pb::ptr <pb::NativeFunction>
	initialize (Context* const context, const pb::ptr <pb::Program> & ec, const pb::ptr <pb::pbObject> & global)
	{
		using namespace std::placeholders;

		const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getIdentifier () .getName (), construct, nullptr, 4);
		const auto prototype = function -> getObject ("prototype");

		global -> addPropertyDescriptor (getIdentifier () .getName (), function, pb::NONE);

		prototype -> addPropertyDescriptor ("add",      pb::make_ptr <pb::NativeFunction> (ec, "add",      add,      1), pb::NONE);
		prototype -> addPropertyDescriptor ("toString", pb::make_ptr <pb::NativeFunction> (ec, "toString", toString, 0), pb::NONE);

		return function;
	}


protected:

	///  @name Construction

	template <class T>
	static
	pb::var
	create (const pb::ptr <pb::pbExecutionContext> & ec, Type* const field)
	{
		const auto object = ec -> createObject (T::getIdentifier ());

		setUserData <T> (ec, object, field);

		return object;
	}

	template <class T>
	static
	void
	setUserData (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & value, X3D::X3DFieldDefinition* const field)
	{
		const auto   context  = getContext (ec);
		const auto & object   = value .getObject ();
		auto &       userData = object -> getUserData ();

		userData .reserve (3);

		userData .emplace_back ((void*) T::getType ());
		userData .emplace_back (field);
		userData .emplace_back (context);
		object -> setDispose (dispose);

		field -> addParent (context);
	}

private:

	static
	void
	dispose (pb::pbObject* const object)
	{
		const auto context = getContext (object);
		const auto field   = getObject <X3D::X3DFieldDefinition> (object);

		field -> removeParent (context);
	}

private:

	static
	pb::var
	construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
	{
		switch (args .size ())
		{
			case 0:
			{
				setUserData <SFVec4> (ec, object, new Type ());
				break;
			}
			case 4:
			{
				setUserData <SFVec4> (ec, object, new Type (
				                         args [0] .toNumber (),
				                         args [1] .toNumber (),
				                         args [2] .toNumber (),
				                         args [3] .toNumber ()
				                         ));
				break;
			}
			default:
				throw pb::Error ("wrong number of arguments.");
		}

		return pb::Undefined ();
	}

	///  @name Functions

	static
	pb::var
	add (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
	{
		if (args .empty ())
			throw pb::Error ("wrong number of arguments.");

		const auto & lhs = *object .getObject () -> getUserData <Type*> (1);
		const auto & rhs = *args [0] .getObject () -> getUserData <Type*> (1);

		return create <SFVec4> (ec, lhs .add (rhs));
	}

	static
	pb::var
	toString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> &)
	{
		return object .getObject () -> getUserData <Type*> (1) -> toString ();
	}

	///  @name Members

	static const std::string    typeName;
	static const pb::Identifier identifier;

};

using SFVec4d = SFVec4 <X3D::SFVec4d>;
using SFVec4f = SFVec4 <X3D::SFVec4f>;

template <>
constexpr ObjectType
SFVec4d::getType ()
{ return ObjectType::SFVec4d; }

template <>
constexpr ObjectType
SFVec4f::getType ()
{ return ObjectType::SFVec4f; }

} // peaseblossom
} // X3D
} // titania

#endif
