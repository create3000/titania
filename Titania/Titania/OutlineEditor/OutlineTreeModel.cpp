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

#include "OutlineTreeModel.h"

#include "../Configuration/config.h"

namespace titania {
namespace puck {

OutlineTreeModel::OutlineTreeModel (const X3D::X3DSFNode <X3D::Browser> & browser) :
	Glib::ObjectBase (typeid (OutlineTreeModel)),
	    Glib::Object (),
	  Gtk::TreeModel (),
	X3DBaseInterface (),
	executionContext (browser -> getExecutionContext ()),
	            tree (),
	           stamp (reinterpret_cast <long int> (this))
{
	//__LOG__ << std::endl;
	setBrowser (browser);

	executionContext -> getRootNodes () .addInterest (this, &OutlineTreeModel::set_rootNodes);
}

Glib::RefPtr <OutlineTreeModel>
OutlineTreeModel::create (const X3D::X3DSFNode <X3D::Browser> & browser)
{
	//__LOG__ << std::endl;
	return Glib::RefPtr <OutlineTreeModel> (new OutlineTreeModel (browser));
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
	{
		auto object = get_object (iter);

		if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
			object = static_cast <X3D::SFNode*> (object) -> getValue ();

		if (object)
			return get_user_data (object);
	}

	return NULL;
}

OutlineUserDataPtr
OutlineTreeModel::get_user_data (X3D::X3DChildObject* object) const
{
	if (not object -> getUserData ())
		object -> setUserData (X3D::UserData (new OutlineUserData ()));

	return std::static_pointer_cast <OutlineUserData> (object -> getUserData ());
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

const OutlineTreeData::parents_type &
OutlineTreeModel::get_parents (const iterator & iter)
{
	return get_data (iter) -> get_parents ();
}

size_t
OutlineTreeModel::get_index (const iterator & iter)
{
	return get_data (iter) -> get_index ();
}

void
OutlineTreeModel::set_data (iterator & iter,
                            OutlineIterType type,
                            X3D::X3DChildObject* object,
                            size_t index,
                            const OutlineTreeData::parents_type & parents) const
{
	auto & node = tree .createNode (get_path (parents, index));

	if (not node .data)
		node .data = new OutlineTreeData (type, object, index, parents);

	iter .gobj () -> user_data = node .data;
}

OutlineTreeData*
OutlineTreeModel::get_data (const iterator & iter)
{
	return static_cast <OutlineTreeData*> (iter .gobj () -> user_data);
}

X3D::FieldDefinitionArray
OutlineTreeModel::get_fields (X3D::X3DChildObject* object) const
{
	auto sfnode = static_cast <X3D::SFNode*> (object);
	auto node   = sfnode -> getValue ();

	if (node)
	{
		auto fields            = node -> getPreDefinedFields ();
		auto userDefinedFields = node -> getUserDefinedFields ();

		if (dynamic_cast <X3D::X3DNode*> (node))
			fields .insert (fields .begin () + 1, userDefinedFields .begin (), userDefinedFields .end ());

		else
			fields = std::move (userDefinedFields);

		//

		X3D::FieldDefinitionArray visibleFields;

		for (const auto & field : fields)
		{
			if (get_user_data (field) -> visible)
				visibleFields .push_back (field);
		}

		return visibleFields;
	}

	return X3D::FieldDefinitionArray ();
}

void
OutlineTreeModel::collapse_row (const Path & path)
{
	tree .removeChildren (path);
}

void
OutlineTreeModel::set_rootNodes ()
{
	//__LOG__ << std::endl;

	clear ();

	size_t size = executionContext -> getRootNodes () .size ();

	for (size_t i = 0; i < size; ++ i)
	{
		Path path;
		path .push_back (i);

		iterator iter = get_iter (path);

		row_inserted (path, iter);
		row_has_child_toggled (path, iter);
	}
}

void
OutlineTreeModel::clear ()
{
	//__LOG__ << std::endl;

	size_t size = tree .getChildren () .size ();

	tree .clear ();

	Path path;
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
	return 4;
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

			auto userData       = get_user_data (iter);
			auto parentUserData = get_user_data (iter -> parent ());

			val .set ((userData and userData -> selected)or (parentUserData and parentUserData -> selected));

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

	return get_path (get_parents (iter), get_index (iter));
}

Gtk::TreeModel::Path
OutlineTreeModel::get_path (const OutlineTreeData::parents_type & parents, size_t index) const
{
	Path path;

	for (const auto & parent : basic::adapter (parents. begin () + 1, parents .end ()))
	{
		path .push_back (parent .get_index ());
	}

	path .push_back (index);

	return path;
}

bool
OutlineTreeModel::get_iter_vfunc (const Path & path, iterator & iter) const
{
	//__LOG__ << std::endl;

	auto index = path .begin ();

	if (not iter_nth_root_child_vfunc (*index, iter))
		return false;

	for (++ index; index not_eq path .end (); ++ index)
	{
		iterator parent = iter;

		if (not iter_nth_child_vfunc (parent, *index, iter))
			return false;
	}

	return true;
}

int
OutlineTreeModel::iter_n_root_children_vfunc () const
{
	//__LOG__ << std::endl;
	return executionContext -> getRootNodes () .size ();
}

bool
OutlineTreeModel::iter_nth_root_child_vfunc (int index, iterator & iter) const
{
	//__LOG__ << std::endl;

	iter .set_stamp (stamp);

	X3D::MFNode* rootNodes = const_cast <X3D::MFNode*> (&executionContext -> getRootNodes ());

	if ((size_t) index < rootNodes -> size ())
	{
		OutlineTreeData::parents_type parents;
		parents .emplace_front (OutlineIterType::X3DField, rootNodes, 0);

		set_data (iter, OutlineIterType::X3DBaseNode, &rootNodes -> at (index), index, parents);
		return true;
	}

	return false;
}

bool
OutlineTreeModel::iter_has_child_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DFieldValue:
			return 0;

		case OutlineIterType::X3DField:
			return iter_n_children_vfunc (iter);

		case OutlineIterType::X3DBaseNode:
		{
			auto sfnode = static_cast <X3D::SFNode*> (get_object (iter));

			// Prevent self referencing traversal

			for (const auto parent : get_parents (iter))
			{
				if (parent .get_type () == OutlineIterType::X3DBaseNode)
				{
					auto parent_sfnode = static_cast <X3D::SFNode*> (parent .get_object ());

					if (*sfnode == *parent_sfnode)
						return 0;
				}
			}

			// Test SFNode

			if (*sfnode)
				return sfnode -> getFieldDefinitions () .size ();

			return 0;
		}
	}

	return 0;
}

int
OutlineTreeModel::iter_n_children_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DFieldValue:
			return 0;

		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
					return static_cast <X3D::MFNode*> (field) -> size ();

				default:
					return 1;
			}

			return 0;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto sfnode = static_cast <X3D::SFNode*> (get_object (iter));

			if (*sfnode)
				return get_fields (sfnode) .size ();

			return 0;
		}
	}

	return 0;
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

	iter .set_stamp (stamp);

	auto parents = get_parents (parent);
	parents .emplace_back (get_data_type (parent), get_object (parent), get_index (parent));

	switch (get_data_type (parent))
	{
		case OutlineIterType::X3DFieldValue:
			return false;

		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (parent));

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					auto sfnode = static_cast <X3D::SFNode*> (field);

					if (index == 0)
					{
						set_data (iter, OutlineIterType::X3DBaseNode, sfnode, index, parents);
						return true;
					}

					return false;
				}
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = static_cast <X3D::MFNode*> (field);

					if ((size_t) index < mfnode -> size ())
					{
						set_data (iter, OutlineIterType::X3DBaseNode, &mfnode -> at (index), index, parents);
						return true;
					}

					return false;
				}
				default:
				{
					if (index == 0)
					{
						set_data (iter, OutlineIterType::X3DFieldValue, field, index, parents);
						return true;
					}

					return false;
				}
			}

			return false;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto fields = std::move (get_fields (get_object (parent)));

			if ((size_t) index < fields .size ())
			{
				set_data (iter, OutlineIterType::X3DField, fields [index], index, parents);
				return true;
			}

			return false;
		}
	}

	return false;
}

bool
OutlineTreeModel::iter_next_vfunc (const iterator & iter, iterator & iter_next) const
{
	//__LOG__ << std::endl;

	iter_next .set_stamp (stamp);

	size_t index = get_index (iter) + 1;

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DFieldValue:
			return false;

		case OutlineIterType::X3DField:
		{
			auto fields = std::move (get_fields (get_parents (iter) .back () .get_object ()));

			if (index < fields .size ())
			{
				set_data (iter_next, OutlineIterType::X3DField, fields [index], index, get_parents (iter));
				return true;
			}

			return false;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_parents (iter) .back () .get_object ());

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = static_cast <X3D::MFNode*> (field);

					if (index < mfnode -> size ())
					{
						set_data (iter_next, OutlineIterType::X3DBaseNode, &mfnode -> at (index), index, get_parents (iter));
						return true;
					}

					return false;
				}
				default:
					return false;
			}
		}
	}

	return false;
}

bool
OutlineTreeModel::iter_parent_vfunc (const iterator & child, iterator & iter) const
{
	//__LOG__ << std::endl;

	iter .set_stamp (stamp);

	auto parents = get_parents (child);

	if (parents .size () == 1)
		return false;

	auto type   = parents .back () .get_type ();
	auto index  = parents .back () .get_index ();
	auto object = parents .back () .get_object ();

	parents .pop_back ();

	set_data (iter, type, object, index, parents);
	return true;
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

	//executionContext -> getRootNodes () .removeInterest (this, &OutlineTreeModel::set_rootNodes);
}

} // puck
} // titania
