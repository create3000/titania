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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFNODE_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFNODE_H__

#include "../../../Fields/SFNode.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

class SFNode :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = X3D::SFNode;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ return ObjectType::SFNode; }

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
	setProperty (pb::pbObject* const, const pb::Identifier &, const pb::var &);

	static
	pb::var
	getProperty (pb::pbObject* const, const pb::Identifier &);

	///  @name Functions

	static
	pb::var
	getNodeName (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	getNodeType (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	getFieldDefinitions (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	toVRMLString (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	toXMLString (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	toString (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Static members

	static const std::string typeName;
	static const pb::Callbacks callbacks;

};

template <>
inline
X3D::X3DChildObject*
X3DField::getKey <SFNode> (X3D::SFNode* const field)
{ return field -> getValue (); }

template <>
inline
pb::var
X3DField::get <SFNode> (Context* const context, const X3D::SFNode & field)
{
	if (field .getValue ())
	{
		const auto object = context -> getObject (getKey <SFNode> (const_cast <X3D::SFNode*> (&field)));

		if (object)
			return object;

		return create <SFNode> (context -> getProgram (), new X3D::SFNode (field));
	}

	return nullptr;
}

} // peaseblossom
} // X3D
} // titania

#endif
