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

#include "OutlineTreeModel.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>

#include <Titania/String.h>

#include <cassert>

namespace titania {
namespace puck {

OutlineTreeModel::OutlineTreeModel (const X3D::X3DExecutionContextPtr & executionContext) :
	   Glib::ObjectBase (typeid (OutlineTreeModel)),
	     Gtk::TreeModel (),
	Gtk::TreeDragSource (),
	  Gtk::TreeDragDest (),
	       Glib::Object (),
	   executionContext (executionContext),
	               tree (),
	              stamp (reinterpret_cast <long int> (this)),
	    show_all_routes (false)
{
	//__LOG__ << std::endl;
	//addChildObjects (executionContext);
}

Glib::RefPtr <OutlineTreeModel>
OutlineTreeModel::create (const X3D::X3DExecutionContextPtr & executionContext)
{
	//__LOG__ << std::endl;
	return Glib::RefPtr <OutlineTreeModel> (new OutlineTreeModel (executionContext));
}

bool
OutlineTreeModel::iter_is_valid (const Gtk::TreeIter & iter) const
{
	if (iter .get_stamp () not_eq stamp)
		return false;

	return get_data (iter);
}

UserDataPtr
OutlineTreeModel::get_user_data (const Gtk::TreeIter & iter) const
{
	if (iter_is_valid (iter))
		return get_data (iter) -> get_user_data ();

	return nullptr;
}

OutlineIterType
OutlineTreeModel::get_data_type (const Gtk::TreeIter & iter)
{
	return get_data (iter) -> get_type ();
}

X3D::X3DChildObject*
OutlineTreeModel::get_object (const Gtk::TreeIter & iter)
{
	return get_data (iter) -> get_object ();
}

std::vector <OutlineTreeData*>
OutlineTreeModel::get_parents (const Gtk::TreeIter & iter) const
{
	return tree .get_parents (get_path (iter));
}

std::vector <X3D::Route*>
OutlineTreeModel::get_input_routes (X3D::X3DFieldDefinition* const field) const
{
	std::vector <X3D::Route*> routes;

	for (const auto & route : field -> getInputRoutes ())
	{
		if (is_visible_route (route))
			routes .emplace_back (route);
	}

	return routes;
}

size_t
OutlineTreeModel::get_input_routes_size (X3D::X3DFieldDefinition* const field) const
{
	size_t size = 0;

	for (const auto & route : field -> getInputRoutes ())
		size += is_visible_route (route);

	return size;
}

std::vector <X3D::Route*>
OutlineTreeModel::get_output_routes (X3D::X3DFieldDefinition* const field) const
{
	std::vector <X3D::Route*> routes;

	for (const auto & route : field -> getOutputRoutes ())
	{
		if (is_visible_route (route))
			routes .emplace_back (route);
	}

	return routes;
}

size_t
OutlineTreeModel::get_output_routes_size (X3D::X3DFieldDefinition* const field) const
{
	size_t size = 0;

	for (const auto & route : field -> getOutputRoutes ())
		size += is_visible_route (route);

	return size;
}

bool
OutlineTreeModel::is_visible_route (const X3D::Route* const route) const
{
	if (route -> isConnected ())
	{
		if (route -> getScene () -> getPrivate ())
			return false;

		if (route -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DScene }))
			return true;

		if (route -> getExecutionContext () -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
			return true;

		if (show_all_routes) // show_all_routes == expandPrototypes
			return true;
	}

	return false;
}

void
OutlineTreeModel::set_data (Gtk::TreeIter & iter,
                            OutlineIterType type,
                            X3D::X3DChildObject* object,
                            const Path & path,
                            const size_t index,
                            const bool selected) const
{
	auto & node = tree .get_node (path, true);

	node .set_data (new OutlineTreeData (type, object, path, index, selected));

	set_data (iter, node .get_data ());
}

void
OutlineTreeModel::set_data (Gtk::TreeIter & iter, OutlineTreeData* data) const
{
	iter .set_stamp (stamp);
	iter .gobj () -> user_data = data;
}

OutlineTreeData*
OutlineTreeModel::get_data (const Gtk::TreeIter & iter)
{
	return static_cast <OutlineTreeData*> (iter .gobj () -> user_data);
}

std::vector <Gtk::TreeIter>
OutlineTreeModel::get_iters (X3D::X3DChildObject* const object) const
{
	std::vector <Gtk::TreeIter> iters;

	for (const auto & node : tree .find (object))
		iters .emplace_back (const_cast <OutlineTreeModel*> (this) -> get_iter (node -> get_data () -> get_path ()));

	return iters;
}

Gtk::TreeIter
OutlineTreeModel::append (OutlineIterType type, X3D::X3DChildObject* object, const size_t index, const bool selected)
{
	//__LOG__ << std::endl;

	Path path;

	path .push_back (tree .size ());

	Gtk::TreeIter iter;
	set_data (iter, type, object, path, index, selected);

	row_inserted (path, iter);
	row_has_child_toggled (path, iter);

	return iter;
}

Gtk::TreeIter
OutlineTreeModel::append (const Gtk::TreeIter & parent, OutlineIterType type, X3D::X3DChildObject* object, const size_t index, const bool selected)
{
	//__LOG__ << std::endl;

	Path path = get_path (parent);

	path .push_back (tree .get_node (path) .size ());

	Gtk::TreeIter iter;
	set_data (iter, type, object, path, index, selected);

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
OutlineTreeModel::clear (const Gtk::TreeIter & iter)
{
	Path         path = get_path (iter);
	const size_t size = tree .get_node (path) .get_children () .size ();

	tree .clear (path);

	path .push_back (0);

	for (size_t i = 0; i < size; ++ i)
		row_deleted (path);

	row_has_child_toggled (path, iter);
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
OutlineTreeModel::get_value_vfunc (const Gtk::TreeIter & iter, int column, Glib::ValueBase & value) const
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

			const auto type     = get_data_type (iter);
			auto       userData = get_user_data (iter);

			switch (type)
			{
				case OutlineIterType::X3DInputRoute:
				case OutlineIterType::X3DOutputRoute:
				{
					userData = get_user_data (iter -> parent ());
					break;
				}
				default:
					break;
			}

			val .set (userData and get_data (iter) -> getSelected ());

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
OutlineTreeModel::get_path_vfunc (const Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;

	assert (get_data (iter));

	return get_data (iter) -> get_path ();
}

bool
OutlineTreeModel::get_iter_vfunc (const Path & path, Gtk::TreeIter & iter) const
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
OutlineTreeModel::iter_nth_root_child_vfunc (int index, Gtk::TreeIter & iter) const
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
OutlineTreeModel::iter_has_child_vfunc (const Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;

	switch (get_data_type (iter))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::NULL_:
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
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object (iter));

			// Prevent self referencing traversal

			if (is_in_parents (sfnode, iter))
				return false;

			// Test SFNode

			if (sfnode)
				return not sfnode -> getFieldDefinitions () .empty ();

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
					return not exportedNode -> getFieldDefinitions () .empty ();
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
					return not localNode -> getFieldDefinitions () .empty ();
			}
			catch (...)
			{ }

			return false;
		}
	}

	return false;
}

bool
OutlineTreeModel::is_in_parents (const X3D::SFNode & sfnode, const Gtk::TreeIter & iter) const
{
	for (const auto & parent : get_parents (iter))
	{
		switch (parent -> get_type ())
		{
			case OutlineIterType::X3DBaseNode :
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
OutlineTreeModel::iter_n_children_vfunc (const Gtk::TreeIter & iter) const
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
OutlineTreeModel::iter_children_vfunc (const Gtk::TreeIter & parent, Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;

	return iter_nth_child_vfunc (parent, 0, iter);
}

bool
OutlineTreeModel::iter_nth_child_vfunc (const Gtk::TreeIter & parent, int index, Gtk::TreeIter & iter) const
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
OutlineTreeModel::iter_next_vfunc (const Gtk::TreeIter & iter, Gtk::TreeIter & iter_next) const
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
OutlineTreeModel::iter_parent_vfunc (const Gtk::TreeIter & child, Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;

	auto path = get_path (child);

	if (path .size () > 1 and path .up ())
	{
		set_data (iter, tree .get_node (path) .get_data ());
		return true;
	}

	return false;
}

void
OutlineTreeModel::ref_node_vfunc (const Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::unref_node_vfunc (const Gtk::TreeIter & iter) const
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_changed (const Path & path, const Gtk::TreeIter & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_inserted (const Path & path, const Gtk::TreeIter & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_has_child_toggled (const Path & path, const Gtk::TreeIter & iter)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_row_deleted (const Path & path)
{
	//__LOG__ << std::endl;
}

void
OutlineTreeModel::on_rows_reordered (const Path & path, const Gtk::TreeIter & iter, int* new_order)
{
	//__LOG__ << std::endl;
}

bool
OutlineTreeModel::row_draggable_vfunc (const Path & path) const
{
	//__LOG__ << std::endl;

	const auto iter = const_cast <OutlineTreeModel*> (this) -> get_iter (path);

	switch (get_data_type (iter))
	{
//		case OutlineIterType::NULL_:
//		{
//			if (path .size () == 1)
//				return true;
//
//			const auto nodeIter = iter -> parent () -> parent ();
//
//			switch (get_data_type (nodeIter))
//			{
//				case OutlineIterType::ProtoDeclaration:
//				case OutlineIterType::X3DBaseNode:
//				case OutlineIterType::ExportedNode:
//				{
//					const auto & node             = *static_cast <X3D::SFNode*> (get_object (nodeIter));
//					const auto   executionContext = node -> getExecutionContext ();
//
//					if (executionContext == get_execution_context ())
//						return true;
//
//					if (executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
//					{
//						return executionContext -> getExecutionContext () == get_execution_context ();
//					}
//
//					return false;
//				}
//				default:
//					break;
//			}
//
//			break;
//		}
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::X3DBaseNode:
		{
			const auto & node = *static_cast <X3D::SFNode*> (get_object (iter));

			// If the node is anywhere in a X3DPrototypeInstance, reject.
			{
				auto executionContext = node -> getExecutionContext ();

				do
				{
					if (executionContext -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
						return false;

					executionContext = executionContext -> getExecutionContext ();
				}
				while (executionContext -> getExecutionContext () not_eq executionContext);
			}

			// If execution context is the current, allow.
			if (node -> getExecutionContext () == get_execution_context ())
				return true;

			// If the node is in a proto and the proto is in the current execution context, allow.
			{
				auto executionContext = node -> getExecutionContext ();

				while (executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
				{
					executionContext = executionContext -> getExecutionContext ();

					if (executionContext == get_execution_context ())
						return true;
				}
			}

			// Otherwise reject.
			return false;
		}
		default:
			break;
	}

	return false;
}

bool
OutlineTreeModel::drag_data_get_vfunc (const Path & path, Gtk::SelectionData & selection_data) const
{
	return true;
}

bool
OutlineTreeModel::row_drop_possible_vfunc (const Path & path, const Gtk::SelectionData & selection_data) const
{
	return true;
}

OutlineTreeModel::~OutlineTreeModel ()
{
	//__LOG__ << std::endl;
}

} // puck
} // titania
