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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_X3DFIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_X3DFIELD_H__

#include "../../Basic/X3DField.h"

#include "Arguments.h"
#include "ObjectType.h"

#include <Titania/PeaseBlossom/pb.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

class X3DField
{
public:

	///  @name Member types

	using internal_type = X3D::X3DFieldDefinition;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ return ObjectType::X3DField; }

	static
	const std::string &
	getTypeName ()
	{ return typeName; }

	///  @name Construction

	static
	pb::ptr <pb::NativeFunction>
	initialize (Context* const, const pb::ptr <pb::Program> &);

	template <class Class>
	static
	pb::var
	get (Context* const, typename Class::internal_type* const);


protected:

	///  @name Construction

	template <class Class>
	static
	pb::var
	create (const pb::ptr <pb::pbExecutionContext> &, typename Class::internal_type* const);

	template <class Class>
	static
	void
	setUserData (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & value, typename Class::internal_type* const field)
	{ setUserData <Class> (ec, value .getObject (), field); }

	template <class Class>
	static
	void
	setUserData (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, typename Class::internal_type* const);

	template <class Class>
	static
	void
	dispose (pb::pbObject* const);


private:

	///  @name Functions

	static
	pb::var
	getName (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, const std::vector <pb::var> &);

	static
	pb::var
	getTypeName_ (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, const std::vector <pb::var> &);

	static
	pb::var
	getType_ (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, const std::vector <pb::var> &);

	static
	pb::var
	toString (const pb::ptr <pb::pbExecutionContext> &, pb::pbObject* const, const std::vector <pb::var> &);

	template <class Class>
	static
	X3D::X3DChildObject*
	getKey (typename Class::internal_type* const field)
	{ return field; }

	///  @name Static members

	static const std::string typeName;

};

template <class Class>
pb::var
X3DField::get (Context* const context, typename Class::internal_type* const field)
{
	const auto object = context -> getObject (getKey <Class> (field));

	if (object)
		return object;

	return create <Class> (context -> getProgram (), field);
}

template <class Class>
pb::var
X3DField::create (const pb::ptr <pb::pbExecutionContext> & ec, typename Class::internal_type* const field)
{
	const auto context = getContext (ec);
	const auto object  = context -> getClass (Class::getType ()) -> createInstance (ec);

	setUserData <Class> (ec, object, field);

	return object;
}

template <class Class>
void
X3DField::setUserData (const pb::ptr <pb::pbExecutionContext> & ec, pb::pbObject* const object, typename Class::internal_type* const field)
{
	const auto context  = getContext (ec);
	auto &     userData = object -> getUserData ();
	const bool loose    = field -> getParents () .empty () and getKey <Class> (field) == field;

	userData .reserve (3);
	userData .emplace_back (context);
	userData .emplace_back (field);
	userData .emplace_back ((void*) loose);
	object -> setCallbacks (Class::getCallbacks ());

	if (loose)
		field -> addParent (context);
	else
		context -> addObject (getKey <Class> (field), object);
}

template <class Class>
void
X3DField::dispose (pb::pbObject* const object)
{
	const auto context = getContext (object);
	const auto field   = getObject <typename Class::internal_type> (object);

	if (object -> getUserData <size_t> (2))
		field -> removeParent (context);
	else
		context -> removeObject (getKey <Class> (field));
}

} // peaseblossom
} // X3D
} // titania

#endif
