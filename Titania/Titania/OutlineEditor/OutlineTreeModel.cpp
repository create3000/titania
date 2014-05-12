/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "OutlineTreeModel.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

#include <cassert>

namespace titania {
namespace puck {

OutlineTreeModel::OutlineTreeModel (BrowserWindow* const browserWindow, const X3D::X3DExecutionContextPtr & executionContext) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	Glib::ObjectBase (typeid (OutlineTreeModel)),
	    Glib::Object (),
	  Gtk::TreeModel (),
	executionContext (executionContext),
	            tree (),
	           stamp (reinterpret_cast <long int> (this))
{
	//__LOG__ << std::endl;
}

Glib::RefPtr <OutlineTreeModel>
OutlineTreeModel::create (BrowserWindow* const browserWindow, const X3D::X3DExecutionContextPtr & executionContext)
{
	//__LOG__ << std::endl;
	return Glib::RefPtr <OutlineTreeModel> (new OutlineTreeModel (browserWindow, executionContext));
}

bool
OutlineTreeModel::iter_is_valid (const iterator & iter) const
{
	if (iter .get_stamp () not_eq stamp)
		return false;

	return get_data (iter);
}

OutlineUserDataPtr
OutlineTreeModel::get_user_data (const iterator & iter) const
{
	if (iter_is_valid (iter))
		return get_data (iter) -> get_user_data ();

	return nullptr;
}

OutlineUserDataPtr
OutlineTreeModel::get_user_data (X3D::X3DChildObject* object)
{
	return OutlineTreeData::get_user_data (object);
}

OutlineIterType
OutlineTreeModel::get_data_type (const iterator & iter)
{
	return get_data (iter) -> get_type ();
}

X3D::X3DChildObject*
OutlineTreeModel::get_object (const iterator & iter)
{
	return get_data (iter) -> get_object ();
}

std::vector <OutlineTreeData*>
OutlineTreeModel::get_parents (const iterator & iter) const
{
	return tree .get_parents (get_path (iter));
}

std::vector <X3D::Route*>
OutlineTreeModel::get_input_routes (X3D::X3DFieldDefinition* const field) const
{
	std::vector <X3D::Route*> routes;

	for (const auto & route : field -> getInputRoutes ())
	{
		try
		{
			if (route -> getExecutionContext () -> isScene () or route -> getExecutionContext () -> isProto ())
				routes .emplace_back (route);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return routes;
}

size_t
OutlineTreeModel::get_input_routes_size (X3D::X3DFieldDefinition* const field) const
{
	size_t size = 0;

	for (const auto & route : field -> getInputRoutes ())
	{
		try
		{
			size += route -> getExecutionContext () -> isScene () or route -> getExecutionContext () -> isProto ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return size;
}

std::vector <X3D::Route*>
OutlineTreeModel::get_output_routes (X3D::X3DFieldDefinition* const field) const
{
	std::vector <X3D::Route*> routes;

	for (const auto & route : field -> getOutputRoutes ())
	{
		try
		{
			if (route -> getExecutionContext () -> isScene () or route -> getExecutionContext () -> isProto ())
				routes .emplace_back (route);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return routes;
}

size_t
OutlineTreeModel::get_output_routes_size (X3D::X3DFieldDefinition* const field) const
{
	size_t size = 0;

	for (const auto & route : field -> getOutputRoutes ())
	{
		try
		{
			size += route -> getExecutionContext () -> isScene () or route -> getExecutionContext () -> isProto ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return size;
}

void
OutlineTreeModel::set_data (iterator & iter,
                            OutlineIterType type,
                            X3D::X3DChildObject* object,
                            const Path & path) const
{
	auto & node = tree .get_node (path, true);

	node .set_data (new OutlineTreeData (type, object, path));

	set_data (iter, node .get_data ());
}

void
OutlineTreeModel::set_data (iterator & iter, OutlineTreeData* data) const
{
	iter .set_stamp (stamp);
	iter .gobj () -> user_data = data;
}

OutlineTreeData*
OutlineTreeModel::get_data (const iterator & iter)
{
	return static_cast <OutlineTreeData*> (iter .gobj () -> user_data);
}

std::vector <Gtk::TreeModel::iterator>
OutlineTreeModel::get_iters (X3D::X3DChildObject* const object) const
{
	std::vector <Gtk::TreeModel::iterator> iters;

	for (const auto & node : tree .find (object))
	{
		iters .emplace_back ();
		set_data (iters .back (), node -> get_data ());
	}

	return iters;
}

OutlineTreeModel::iterator
OutlineTreeModel::append (OutlineIterType type, X3D::X3DChildObject* object)
{
	//__LOG__ << std::endl;

	Path path;
	path .push_back (tree .size ());

	iterator iter;
	set_data (iter, type, object, path);

	row_inserted (path, iter);
	row_has_child_toggled (path, iter);

	return iter;
}

OutlineTreeModel::iterator
OutlineTreeModel::append (const iterator & parent, OutlineIterType type, X3D::X3DChildObject* object)
{
	//__LOG__ << std::endl;

	Path path = get_path (parent);
	path .push_back (tree .get_node (path) .size ());

	iterator iter;
	set_data (iter, type, object, path);

	row_inserted (path, iter);
	row_has_child_toggled (path, iter);

	return iter;
}

void
OutlineTreeModel::clear ()
{
	//__LOG__ << std::endl;

	const size_t size = tree .get_children () .size ();

	tree .clear ();

	Path path;
	path .push_back (0);

	for (size_t i = 0; i < size; ++ i)
		row_deleted (path);
}

void
OutlineTreeModel::clear (const iterator & iter)
{
	Path         path = get_path (iter);
	const size_t size = tree .get_node (path) .get_children () .size ();

	tree .clear (path);

	path .push_back (0);

	for (size_t i = 0; i < size; ++ i)
		row_deleted (path);
}

Gtk::TreeModelFlags
OutlineTreeModel::get_flags_vfunc () const
{
	//__LOG__ << std::endl;
	return Gtk::TreeModelFlags (0);
}

int
OutlineTreeModel::get_n_columns_vfunc () const
{
	//__LOG__ << std::endl;
	return 2;
}

GType
OutlineTreeModel::get_column_type_vfunc (int index) const
{
	//__LOG__ << std::endl;

	switch (index)
	{
		case DATA_COLUMN:
			return data_column .type ();

		case SELECTED_COLUMN:
			return selected_column .type ();

		default:
			return 0;
	}
}

void
OutlineTreeModel::get_value_vfunc (const iterator & iter, int column, Glib::ValueBase & value) const
{
	//__LOG__ << std::endl;

	switch (column)
	{
		case DATA_COLUMN:     // OutlineTreeData
		{
			DataColumn::ValueType val;
			val .init (DataColumn::ValueType::value_type ());
			val .set (get_data (iter));

			value .init (DataColumn::ValueType::value_type ());
			value = val;

			break;
		}
		case SELECTED_COLUMN: // Selected
		{
			SelectedColumn::ValueType val;
			val .init (SelectedColumn::ValueType::value_type ());

			const auto userData       = get_user_data (iter);
			const auto parentUserData = get_user_data (iter -> parent ());

			val .set ((userData and userData -> selected & OUTLINE_SELECTED) or
			          (parentUserData and parentUserData -> selected & OUTLINE_SELECTED));

			value .init (SelectedColumn::ValueType::value_type ());
			value = val;

			break;
		}
		default:
		{
			g_assert_not_reached ();
			break;
		}
	}
}

Gtk::TreeModel::Path
OutlineTreeModel::get_path_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;
	
	assert (get_data (iter));

	return get_data (iter) -> get_path ();
}

bool
OutlineTreeModel::get_iter_vfunc (const Path & path, iterator & iter) const
{
	//__LOG__ << path .to_string () << std::endl;

	try
	{
		set_data (iter, tree .get_node (path) .get_data ());
		return true;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

int
OutlineTreeModel::iter_n_root_children_vfunc () const
{
	//__LOG__ << std::endl;
	return tree .size ();
}

bool
OutlineTreeModel::iter_nth_root_child_vfunc (int index, iterator & iter) const
{
	//__LOG__ << std::endl;

	try
	{
		Path path;
		path .push_back (index);

		set_data (iter, tree .get_node (path) .get_data ());
		return true;
	}
	catch (const std::out_of_range ())
	{
		return false;
	}
}

bool
OutlineTreeModel::iter_has_child_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		{
			return false;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					if (not static_cast <X3D::MFNode*> (field) -> empty ())
						return true;

					if (get_input_routes_size (field))
						return true;
					
					if (get_output_routes_size (field))
						return true;

					break;
				}
				default:
				{
					return true;
				}
			}

			return false;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			return true;
		}
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProto:
		case OutlineIterType::Prototype:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object (iter));

			// Prevent self referencing traversal
			
			if (is_in_parents (sfnode, iter))
				return false;
				
			// Test SFNode

			if (sfnode)
				return sfnode -> getFieldDefinitions () .size ();

			return false;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (get_object (iter));
				const auto importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode -> getValue ());
				const auto exportedNode = importedNode -> getExportedNode ();

				// Prevent self referencing traversal
				
				if (is_in_parents (exportedNode, iter))
					return false;
					
				// Test SFNode

				if (exportedNode)
					return exportedNode -> getFieldDefinitions () .size ();
			}
			catch (...)
			{ }

			return false;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (get_object (iter));
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode -> getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();

				// Prevent self referencing traversal
				
				if (is_in_parents (localNode, iter))
					return false;
					
				// Test SFNode

				if (localNode)
					return localNode -> getFieldDefinitions () .size ();
			}
			catch (...)
			{ }

			return false;
		}
		case OutlineIterType::Separator:
		{
			return false;
		}
	}

	return false;
}

bool
OutlineTreeModel::is_in_parents (const X3D::SFNode & sfnode, const iterator & iter) const
{
	for (const auto & parent : get_parents (iter))
	{
		switch (parent -> get_type ())
		{
			case OutlineIterType::X3DBaseNode:
			{
				const auto & parent_sfnode = *static_cast <X3D::SFNode*> (parent -> get_object ());

				if (sfnode == parent_sfnode)
					return true;

				break;
			}
			case OutlineIterType::ImportedNode:
			{
				try
				{
					const auto object       = static_cast <X3D::SFNode*> (parent -> get_object ());
					const auto importedNode = dynamic_cast <X3D::ImportedNode*> (object -> getValue ());
					const auto exportedNode = importedNode -> getExportedNode ();

					if (sfnode == exportedNode)
						return true;
				}
				catch (...)
				{ }

				break;
			}
			case OutlineIterType::ExportedNode:
			{
				try
				{
					const auto object       = static_cast <X3D::SFNode*> (parent -> get_object ());
					const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (object -> getValue ());
					const auto localNode    = exportedNode -> getLocalNode ();

					if (sfnode == localNode)
						return true;
				}
				catch (...)
				{ }

				break;
			}
			default:
				break;
		}
	}

	return false;
}

int
OutlineTreeModel::iter_n_children_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;

	try
	{
		return tree .get_node (get_path (iter)) .size ();
	}
	catch (const std::out_of_range &)
	{
		return 0;
	}
}

bool
OutlineTreeModel::iter_children_vfunc (const iterator & parent, iterator & iter) const
{
	//__LOG__ << std::endl;

	return iter_nth_child_vfunc (parent, 0, iter);
}

bool
OutlineTreeModel::iter_nth_child_vfunc (const iterator & parent, int index, iterator & iter) const
{
	//__LOG__ << std::endl;

	try
	{
		Path path = get_path (parent);
		path .push_back (index);

		set_data (iter, tree .get_node (path) .get_data ());
		return true;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

bool
OutlineTreeModel::iter_next_vfunc (const iterator & iter, iterator & iter_next) const
{
	//__LOG__ << std::endl;

	try
	{
		Path path = get_path (iter);
		path .next ();

		set_data (iter_next, tree .get_node (path) .get_data ());
		return true;
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

bool
OutlineTreeModel::iter_parent_vfunc (const iterator & child, iterator & iter) const
{
	//__LOG__ << std::endl;

	auto path = get_path (child);

	if (path .up ())
	{
		set_data (iter, tree .get_node (path) .get_data ());
		return true;
	}

	return false;
}

void
OutlineTreeModel::ref_node_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::unref_node_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_changed (const Path & path, const iterator & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_inserted (const Path & path, const iterator & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_has_child_toggled (const Path & path, const iterator & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_deleted (const Path & path)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_rows_reordered (const Path & path, const iterator & iter, int* new_order)
{
	//__LOG__ << std::endl;
}

OutlineTreeModel::~OutlineTreeModel ()
{
	//__LOG__ << std::endl;
}

} // puck
} // titania
