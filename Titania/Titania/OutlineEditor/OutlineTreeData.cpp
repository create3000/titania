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

#include "OutlineTreeData.h"

namespace titania {
namespace puck {

OutlineTreeData::OutlineTreeData (OutlineIterType type, X3D::X3DChildObject* object, const Gtk::TreeModel::Path & path) :
	   Glib::Object (),
	         object (object),
	           type (type),
	           path (path),
	   inputs_below (),
	   inputs_above (),
	  outputs_below (),
	  outputs_above (),
	self_connection (false),
	    connections ()
{
	if (type == OutlineIterType::X3DBaseNode)
		this -> object = new X3D::SFNode (static_cast <X3D::SFNode*> (object) -> getValue ());

	get_user_data () -> paths .emplace (path);
}

OutlineTreeData::OutlineTreeData (const OutlineTreeData & value) :
	OutlineTreeData (value .type, value .object, value .path)
{ }

bool
OutlineTreeData::is (X3D::X3DChildObject* const value) const
{
	if (type == OutlineIterType::X3DBaseNode)
		return static_cast <X3D::SFNode*> (object) -> getValue () == value;

	return object == value;
}

OutlineUserDataPtr
OutlineTreeData::get_user_data () const
{
	auto object = get_object ();

	if (type == OutlineIterType::X3DBaseNode)
		object = static_cast <X3D::SFNode*> (object) -> getValue ();

	if (object)
		return get_user_data (object);

	return get_user_data (get_object ());
}

OutlineUserDataPtr
OutlineTreeData::get_user_data (X3D::X3DChildObject* object)
{
	if (not object -> getUserData ())
		object -> setUserData (X3D::UserDataPtr (new OutlineUserData ()));

	return std::static_pointer_cast <OutlineUserData> (object -> getUserData ());
}

///  @name Destruction

OutlineTreeData::~OutlineTreeData ()
{
	get_user_data () -> paths .erase (path);

	if (type == OutlineIterType::X3DBaseNode)
		delete object;
}

} // puck
} // titania
