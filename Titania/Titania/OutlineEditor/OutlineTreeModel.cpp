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

#include "../Configuration/config.h"
#include <Titania/X3D/Parser/RegEx.h>

namespace titania {
namespace puck {

OutlineTreeModel::OutlineTreeModel (const X3D::X3DSFNode <X3D::Browser> & browser) :
	Glib::ObjectBase (typeid (OutlineTreeModel)),
	    Glib::Object (),
	  Gtk::TreeModel (),
	//X3DBaseInterface (),
	executionContext (browser -> getExecutionContext ()),
	           stamp ((long int) this),
	            tree ()
{
	//__LOG__ << std::endl;
	//setBrowser (browser);

	noneImage     = Gdk::Pixbuf::create_from_file (get_icon ("none.png"));
	baseNodeImage = Gdk::Pixbuf::create_from_file (get_icon ("Node.png"));

	for (const auto & field : browser -> getSupportedFields ())
		fieldTypeImages [field -> getType ()] = Gdk::Pixbuf::create_from_file (get_icon (field -> getTypeName () + ".png"));

	executionContext -> getRootNodes () .addInterest (this, &OutlineTreeModel::set_rootNodes);
}

Glib::RefPtr <OutlineTreeModel>
OutlineTreeModel::create (const X3D::X3DSFNode <X3D::Browser> & browser)
{
	//__LOG__ << std::endl;
	return Glib::RefPtr <OutlineTreeModel> (new OutlineTreeModel (browser));
}

void
OutlineTreeModel::collapse_row (const Path & path)
{
	tree .removeChildren (path);
}

OutlineUserDataPtr
OutlineTreeModel::get_user_data (const iterator & iter)
{
	auto object = get_object (iter);

	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
		object = static_cast <X3D::SFNode*> (object) -> getValue ();

	if (object)
		return get_user_data (object);

	return NULL;
}

OutlineUserDataPtr
OutlineTreeModel::get_user_data (X3D::X3DChildObject* object)
{
	if (not object -> getUserData ())
		object -> setUserData (X3D::UserData (new OutlineUserData ()));

	return std::static_pointer_cast <OutlineUserData> (object -> getUserData ());
}

OutlineIterType
OutlineTreeModel::get_data_type (const iterator & iter)
{
	return get_data (iter) -> type;
}

X3D::X3DChildObject*
OutlineTreeModel::get_object (const iterator & iter)
{
	return get_data (iter) -> object;
}

const OutlineIterData::parents_type &
OutlineTreeModel::get_parents (const iterator & iter)
{
	return get_data (iter) -> parents;
}

size_t
OutlineTreeModel::get_index (const iterator & iter)
{
	return get_data (iter) -> index;
}

void
OutlineTreeModel::set_data (iterator & iter,
                           OutlineIterType type,
                           X3D::X3DChildObject* object,
                           size_t index,
                           const OutlineIterData::parents_type & parents) const
{
	auto & node = tree .createNode (get_path (parents, index));

	if (not node .data)
		node .data = new OutlineIterData (type, object, index, parents);

	iter .gobj () -> user_data = node .data;
}

OutlineIterData*
OutlineTreeModel::get_data (const iterator & iter)
{
	return static_cast <OutlineIterData*> (iter .gobj () -> user_data);
}

X3D::FieldDefinitionArray
OutlineTreeModel::get_fields (X3D::X3DChildObject* object)
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
		case 0:
			return icon_column .type ();
		case 1:
			return data_column .type ();
		case 2:
			return selected_color_column .type ();
		case 3:
			return name_column .type ();
		case 4:
			return access_type_image_column .type ();
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
		case 0: // Row type image
		{
			IconColumn::ValueType val;
			val .init (IconColumn::ValueType::value_type ());

			switch (get_data_type (iter))
			{
				case OutlineIterType::X3DFieldValue:
				{
					val .set (noneImage);
					break;
				}
				case OutlineIterType::X3DField:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));
					auto iter  = fieldTypeImages .find (field -> getType ());

					if (iter not_eq fieldTypeImages .end ())
						val .set (iter -> second);

					break;
				}
				case OutlineIterType::X3DBaseNode:
				{
					val .set (baseNodeImage);
					break;
				}
			}

			value .init (IconColumn::ValueType::value_type ());
			value = val;

			break;
		}
		case 1: // OutlineIterData
		{
			DataColumn::ValueType val;
			val .init (DataColumn::ValueType::value_type ());
			val .set (get_data (iter));

			value .init (DataColumn::ValueType::value_type ());
			value = val;

			break;
		}
		case 2: // Selected
		{
			SelectedColorColumn::ValueType val;
			val .init (SelectedColorColumn::ValueType::value_type ());

			auto userData       = get_user_data (iter);
			auto parentUserData = get_user_data (get_object (iter));

			val .set ((userData and userData -> selected) or parentUserData -> selected);

			value .init (SelectedColorColumn::ValueType::value_type ());
			value = val;

			break;
		}
		case 3: // Value, name or name and typeName
		{
			NameColumn::ValueType val;
			val .init (NameColumn::ValueType::value_type ());

			switch (get_data_type (iter))
			{
				case OutlineIterType::X3DFieldValue:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

					val .set (Glib::Markup::escape_text (field -> toString ()));

					break;
				}
				case OutlineIterType::X3DField:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

					val .set (Glib::Markup::escape_text (field -> getName ()));

					break;
				}
				case OutlineIterType::X3DBaseNode:
				{
					auto sfnode = static_cast <X3D::SFNode*> (get_object (iter));

					if (*sfnode)
					{
						X3D::X3DBaseNode* node = sfnode -> getValue ();
						
						std::string typeName  = Glib::Markup::escape_text (node -> getTypeName ());
						std::string name      = Glib::Markup::escape_text (node -> getName ());
						size_t      numClones = node -> getNumClones ();
						
						X3D::RegEx::_Num .Replace ("", &name);

						std::string string ="<b>" + typeName + "</b> " + name;

						if (numClones > 1)
							string += " [" + std::to_string (numClones) + "]";

						val .set (string);
					}

					else
						val .set ("<b>NULL</b>");

					break;
				}
			}

			value .init (NameColumn::ValueType::value_type ());
			value = val;

			break;
		}
		case 4: // AccesType image
		{
			AccessTypeImageColumn::ValueType val;
			val .init (AccessTypeImageColumn::ValueType::value_type ());

			switch (get_data_type (iter))
			{
				case OutlineIterType::X3DFieldValue:
				{
					val .set (noneImage);
					break;
				}
				case OutlineIterType::X3DField:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));
					auto iter  = fieldTypeImages .find (field -> getType ());

					if (iter not_eq fieldTypeImages .end ())
						val .set (iter -> second);

					break;
				}
				case OutlineIterType::X3DBaseNode:
				{
					val .set (baseNodeImage);
					break;
				}
			}

			value .init (AccessTypeImageColumn::ValueType::value_type ());
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
OutlineTreeModel::get_path (const OutlineIterData::parents_type & parents, size_t index) const
{
	Path path;

	for (const auto & parent : basic::adapter (parents. begin () + 1, parents .end ()))
	{
		path .push_back (parent .index);
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
		OutlineIterData::parents_type parents;
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
				if (parent .type == OutlineIterType::X3DBaseNode)
				{
					auto parent_sfnode = static_cast <X3D::SFNode*> (parent .object);

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
			auto fields = std::move (get_fields (get_parents (iter) .back () .object));

			if (index < fields .size ())
			{
				set_data (iter_next, OutlineIterType::X3DField, fields [index], index, get_parents (iter));
				return true;
			}

			return false;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_parents (iter) .back () .object);

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

	auto type   = parents .back () .type;
	auto index  = parents .back () .index;
	auto object = parents .back () .object;

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

	executionContext -> getRootNodes () .removeInterest (this, &OutlineTreeModel::set_rootNodes);
}

} // puck
} // titania
