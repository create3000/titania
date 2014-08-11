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

#ifndef __TITANIA_X3D_TOOLS_CORE_X3DBASE_TOOL_H__
#define __TITANIA_X3D_TOOLS_CORE_X3DBASE_TOOL_H__

#include "../Core/X3DToolObject.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DBaseTool :
	public Type, public X3DToolObject
{
public:

	///  @name Member access

	virtual
	Type*
	getId () final override
	{ return node; }

	virtual
	const Type*
	getId () const final override
	{ return node; }

	virtual
	void
	setName (const std::string & value) final override
	{ node -> setName (value); }

	virtual
	const std::string &
	getName () const final override
	{ return node -> getName (); }

	virtual
	const Output &
	name_changed () const final override
	{ return node -> name_changed (); }

	virtual
	void
	setUserData (const UserDataPtr & value) final override
	{ node -> setUserData (value); }

	virtual
	const UserDataPtr &
	getUserData () const final override
	{ return node -> getUserData (); }

	virtual
	SFBool &
	isLive () final override
	{ return node -> isLive (); }

	virtual
	const SFBool &
	isLive () const final override
	{ return node -> isLive (); }

	virtual
	void
	addTool () override
	{ }

	virtual
	void
	removeTool (const bool = false) override;

	virtual
	void
	beginUpdate ()
	throw (Error <DISPOSED>) final override
	{ return node -> beginUpdate (); }

	virtual
	void
	endUpdate ()
	throw (Error <DISPOSED>) final override
	{ return node -> endUpdate (); }

	virtual
	void
	addEvent () final override
	{ }
	
	virtual
	void
	traverse (const TraverseType) override
	{ }

	///  @name Destruction

	virtual
	const Output &
	shutdown () const final override
	{ return node -> shutdown (); }

	virtual
	const Output &
	disposed () const final override
	{ return node -> disposed (); }

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DBaseTool (Type* const = nullptr);

	virtual
	void
	initialize () override;

	///  @name Member access

	Type*
	getNode ()
	{ return node; }

	const Type*
	getNode () const
	{ return node; }


private:

	///  @name Event handling

	virtual
	void
	addEvent (X3DChildObject* const) final override
	{ }

	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &) final override
	{ }

	///  @name Members

	Type* const node;

};

template <class Type>
X3DBaseTool <Type>::X3DBaseTool (Type* const node) :
	         Type (node -> getExecutionContext ()),
	X3DToolObject (),
	         node (node)
{
	isPrivate (node -> isPrivate ());

	node -> addParent (this);
	node -> isPrivate (true);

	for (auto & field : node -> getPreDefinedFields ())
		addField (field -> getAccessType (), field -> getName (), *field);

	for (auto & field : node -> getUserDefinedFields ())
		addUserDefinedField (field -> getAccessType (), field -> getName (), field);
}

template <class Type>
void
X3DBaseTool <Type>::initialize ()
{
	X3DBaseNode::initialize ();
	X3DToolObject::initialize ();
}

template <class Type>
void
X3DBaseTool <Type>::removeTool (const bool)
{
	Type::removeTool (node);
}

template <class Type>
void
X3DBaseTool <Type>::dispose ()
{
	node -> isPrivate (isPrivate ());
	node -> removeParent (this);

	X3DToolObject::dispose ();
	Type::dispose ();
}

} // X3D
} // titania

#endif
