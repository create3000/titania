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

namespace titania {
namespace puck {

OutlineTreeModel::OutlineTreeModel (const X3D::X3DSFNode <X3D::Browser> & browser) :
	Glib::ObjectBase (typeid (OutlineTreeModel)),
	    Glib::Object (),
	  Gtk::TreeModel (),
	executionContext (browser -> getExecutionContext ()),
	           stamp ((long int)                          this),
	            tree ()
{
	//__LOG__ << std::endl;

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
OutlineTreeModel::set_rootNodes ()
{
	//__LOG__ << std::endl;

	size_t size = tree .getChildren () .size ();

	tree .clear ();

	for (int i = size; i >= 0; -- i)
	{
		Path path;
		path .push_back (i);

		row_deleted (path);
	}

	size = executionContext -> getRootNodes () .size ();

	for (size_t i = 0; i < size; ++ i)
	{
		Path path;
		path .push_back (i);

		iterator iter = get_iter (path);

		row_inserted (path, iter);
		row_has_child_toggled (path, iter);
	}
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
			return debug_column .type ();
		default:
			return 0;
	}
}

void
OutlineTreeModel::get_value_vfunc (const iterator & iter, int column, Glib::ValueBase & value) const
{
	//__LOG__ << std::endl;

	auto data = getData (iter);

	switch (column)
	{
		case 0:
		{
			icon_column_type::ValueType val;
			val .init (icon_column_type::ValueType::value_type ());

			switch (data -> type)
			{
				case OutlineIterType::X3DFieldValue:
				{
					val .set (noneImage);
					break;
				}
				case OutlineIterType::X3DField:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);
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

			value .init (icon_column_type::ValueType::value_type ());
			value = val;

			break;
		}
		case 1:
		{
			data_column_type::ValueType val;
			val .init (data_column_type::ValueType::value_type ());
			val .set (data);

			value .init (data_column_type::ValueType::value_type ());
			value = val;

			break;
		}
		case 2:
		{
			selected_color_column_type::ValueType val;
			val .init (selected_color_column_type::ValueType::value_type ());

			auto userData       = getUserData (iter);
			auto parentUserData = getUserData (data -> object);

			val .set ((userData and userData -> selected) or parentUserData -> selected);

			value .init (selected_color_column_type::ValueType::value_type ());
			value = val;

			break;
		}
		case 3:
		{
			debug_column_type::ValueType val;
			val .init (debug_column_type::ValueType::value_type ());

			switch (data -> type)
			{
				case OutlineIterType::X3DFieldValue:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);

					val .set (field -> toString ());

					break;
				}
				case OutlineIterType::X3DField:
				{
					auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);

					val .set (field -> getName ());

					break;
				}
				case OutlineIterType::X3DBaseNode:
				{
					auto sfnode = static_cast <X3D::SFNode*> (data -> object);

					if (*sfnode)
						val .set ("<b>" + sfnode -> getNodeTypeName () + "</b> " + sfnode -> getNodeName ());

					else
						val .set ("<b>NULL</b>");

					break;
				}
			}

			value .init (debug_column_type::ValueType::value_type ());
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

	auto data = getData (iter);

	return get_path (data -> parents, data -> index);
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

		setData (iter, OutlineIterType::X3DBaseNode, &rootNodes -> at (index), index, parents);
		return true;
	}

	return false;
}

bool
OutlineTreeModel::iter_has_child_vfunc (const iterator & iter) const
{
	//__LOG__ << std::endl;

	auto data = getData (iter);

	switch (data -> type)
	{
		case OutlineIterType::X3DFieldValue:
			return 0;

		case OutlineIterType::X3DField:
			return iter_n_children_vfunc (iter);

		case OutlineIterType::X3DBaseNode:
		{
			auto sfnode = static_cast <X3D::SFNode*> (data -> object);

			// Prevent self referencing traversal

			for (const auto parent : data -> parents)
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

	auto data = getData (iter);

	switch (data -> type)
	{
		case OutlineIterType::X3DFieldValue:
			return 0;

		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				case X3D::X3DConstants::MFBool:
				case X3D::X3DConstants::MFColor:
				case X3D::X3DConstants::MFColorRGBA:
				case X3D::X3DConstants::MFDouble:
				case X3D::X3DConstants::MFFloat:
				case X3D::X3DConstants::MFImage:
				case X3D::X3DConstants::MFInt32:
				case X3D::X3DConstants::MFMatrix3d:
				case X3D::X3DConstants::MFMatrix3f:
				case X3D::X3DConstants::MFMatrix4d:
				case X3D::X3DConstants::MFMatrix4f:
				case X3D::X3DConstants::MFRotation:
				case X3D::X3DConstants::MFString:
				case X3D::X3DConstants::MFTime:
				case X3D::X3DConstants::MFVec2d:
				case X3D::X3DConstants::MFVec2f:
				case X3D::X3DConstants::MFVec3d:
				case X3D::X3DConstants::MFVec3f:
				case X3D::X3DConstants::MFVec4d:
				case X3D::X3DConstants::MFVec4f:
					return 1;

				case X3D::X3DConstants::MFNode:
					return static_cast <X3D::MFNode*> (field) -> size ();

				default:
					return 0;
			}

			return 0;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto sfnode = static_cast <X3D::SFNode*> (data -> object);

			if (*sfnode)
				return getFields (sfnode) .size ();

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

	auto data = getData (parent);

	auto parents = data -> parents;
	parents .emplace_back (data -> type, data -> object, data -> index);

	switch (data -> type)
	{
		case OutlineIterType::X3DFieldValue:
			return false;

		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					auto sfnode = static_cast <X3D::SFNode*> (field);

					if (index < 1)
					{
						setData (iter, OutlineIterType::X3DBaseNode, sfnode, index, parents);
						return true;
					}

					return false;
				}
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = static_cast <X3D::MFNode*> (field);

					if ((size_t) index < mfnode -> size ())
					{
						setData (iter, OutlineIterType::X3DBaseNode, &mfnode -> at (index), index, parents);
						return true;
					}

					return false;
				}
				case X3D::X3DConstants::MFBool:
				case X3D::X3DConstants::MFColor:
				case X3D::X3DConstants::MFColorRGBA:
				case X3D::X3DConstants::MFDouble:
				case X3D::X3DConstants::MFFloat:
				case X3D::X3DConstants::MFImage:
				case X3D::X3DConstants::MFInt32:
				case X3D::X3DConstants::MFMatrix3d:
				case X3D::X3DConstants::MFMatrix3f:
				case X3D::X3DConstants::MFMatrix4d:
				case X3D::X3DConstants::MFMatrix4f:
				case X3D::X3DConstants::MFRotation:
				case X3D::X3DConstants::MFString:
				case X3D::X3DConstants::MFTime:
				case X3D::X3DConstants::MFVec2d:
				case X3D::X3DConstants::MFVec2f:
				case X3D::X3DConstants::MFVec3d:
				case X3D::X3DConstants::MFVec3f:
				case X3D::X3DConstants::MFVec4d:
				case X3D::X3DConstants::MFVec4f:
				{
					if (index == 0)
					{
						setData (iter, OutlineIterType::X3DFieldValue, field, index, parents);
						return true;
					}

					return false;
				}

				default:
					return false;
			}

			return false;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto fields = std::move (getFields (data -> object));

			if ((size_t) index < fields .size ())
			{
				setData (iter, OutlineIterType::X3DField, fields [index], index, parents);
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

	auto   data  = getData (iter);
	size_t index = data -> index + 1;

	switch (data -> type)
	{
		case OutlineIterType::X3DFieldValue:
			return false;

		case OutlineIterType::X3DField:
		{
			auto fields = std::move (getFields (data -> parents .back () .object));

			if (index < fields .size ())
			{
				setData (iter_next, OutlineIterType::X3DField, fields [index], index, data -> parents);
				return true;
			}

			return false;
		}
		case OutlineIterType::X3DBaseNode:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (data -> parents .back () .object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = static_cast <X3D::MFNode*> (field);

					if (index < mfnode -> size ())
					{
						setData (iter_next, OutlineIterType::X3DBaseNode, &mfnode -> at (index), index, data -> parents);
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

	auto data    = getData (child);
	auto parents = data -> parents;

	if (parents .size () == 1)
		return false;

	auto type   = parents .back () .type;
	auto index  = parents .back () .index;
	auto object = parents .back () .object;

	parents .pop_back ();

	setData (iter, type, object, index, parents);
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

void
OutlineTreeModel::collapse_row (const Path & path)
{
	tree .removeChildren (path);
}

X3D::FieldDefinitionArray
OutlineTreeModel::getFields (X3D::X3DChildObject* object)
{
	auto sfnode = static_cast <X3D::SFNode*> (object);
	auto node   = sfnode -> getValue ();

	auto fields            = node -> getPreDefinedFields ();
	auto userDefinedFields = node -> getUserDefinedFields ();

	if (fields .size ())
		fields .insert (fields .begin () + 1, userDefinedFields .begin (), userDefinedFields .end ());

	else
		fields = std::move (userDefinedFields);

	//

	X3D::FieldDefinitionArray visibleFields;

	for (const auto & field : fields)
	{
		if (getUserData (field) -> visible)
			visibleFields .push_back (field);
	}

	if (visibleFields .size ())
		return visibleFields;

	return fields;
}

OutlineUserData*
OutlineTreeModel::getUserData (const iterator & iter)
{
	auto data   = getData (iter);
	auto object = data -> object;

	if (data -> type == OutlineIterType::X3DBaseNode)
		object = static_cast <X3D::SFNode*> (object) -> getValue ();

	if (object)
		return getUserData (object);

	return NULL;
}

OutlineUserData*
OutlineTreeModel::getUserData (X3D::X3DChildObject* object)
{
	if (not object -> getUserData ())
		object -> setUserData (new OutlineUserData ());

	return static_cast <OutlineUserData*> (object -> getUserData ());
}

void
OutlineTreeModel::setData (iterator & iter,
                           OutlineIterType type,
                           X3D::X3DChildObject* object,
                           size_t index,
                           const OutlineIterData::parents_type & parents) const
{
	auto & node = tree .getNode (get_path (parents, index));

	if (not node .data)
		node .data = new OutlineIterData (type, object, index, parents);

	iter .gobj () -> user_data = node .data;
}

OutlineIterData*
OutlineTreeModel::getData (const iterator & iter)
{
	return static_cast <OutlineIterData*> (iter .gobj () -> user_data);
}

OutlineTreeModel::~OutlineTreeModel ()
{
	//__LOG__ << std::endl;
}

} // puck
} // titania
