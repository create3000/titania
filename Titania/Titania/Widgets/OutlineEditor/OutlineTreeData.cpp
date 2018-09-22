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

#include "OutlineTreeData.h"

#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>

namespace titania {
namespace puck {

OutlineTreeData::OutlineTreeData (const OutlineIterType type_, X3D::X3DChildObject* const object_, const Gtk::TreeModel::Path & path_, const size_t index, const bool selected) :
	   Glib::Object (),
	         parent (),
	         object (object_),
	           type (type_),
	           path (path_),
	          index (index),
	       selected (selected),
	   inputs_below (),
	   inputs_above (),
	  outputs_below (),
	  outputs_above (),
	self_connection (false),
	    connections ()
{
	switch (type)
	{
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			object -> addParent (&parent);
			break;
		}
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		case OutlineIterType::Separator:
		{
			parent = X3D::SFNode (object);
			object = &parent;
			break;
		}
		default:
			break;
	}

	try
	{
		switch (type)
		{
			case OutlineIterType::ImportedNode:
			{
				const auto importedNode = dynamic_cast <X3D::ImportedNode*> (parent .getValue ());
				const auto exportedNode = importedNode -> getExportedNode ();
				parent -> setUserData (exportedNode -> getUserData <UserData> ());
				break;
			}
			case OutlineIterType::ExportedNode:
			{
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (parent .getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();
				parent -> setUserData (localNode -> getUserData <UserData> ());
				break;
			}
			default:
				break;
		}
	}
	catch (...)
	{ }

	get_user_data () -> paths .emplace (path);
}

bool
OutlineTreeData::is (X3D::X3DChildObject* const value) const
{
	switch (type)
	{
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
			return parent .getValue () == value;

		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto importedNode = dynamic_cast <X3D::ImportedNode*> (parent .getValue ());
				const auto exportedNode = importedNode -> getExportedNode ();

				if (value == exportedNode)
					return true;
			}
			catch (...)
			{ }
			
			return value == parent .getValue ();
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (parent .getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();

				if (value == localNode)
					return true;
			}
			catch (...)
			{ }

			return value == parent .getValue ();
		}
		default:
			break;
	}

	return object == value;
}

UserDataPtr
OutlineTreeData::get_user_data () const
{
	auto object = get_object ();

	switch (type)
	{
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
			object = parent .getValue ();
			break;
		default:
			break;
	}

	if (object)
		return object -> getUserData <UserData> ();

	return get_object () -> getUserData <UserData> ();
}

///  @name Destruction

OutlineTreeData::~OutlineTreeData ()
{
	get_user_data () -> paths .erase (path);

	switch (type)
	{
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			object -> removeParent (&parent);
			break;
		}
		default:
			break;
	}
}

} // puck
} // titania
