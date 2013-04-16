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

OutlineTreeModel::OutlineTreeModel (const X3D::SFNode <X3D::Browser> & browser) :
	Glib::ObjectBase (typeid (OutlineTreeModel)),               
	    Glib::Object (),                                        
	  Gtk::TreeModel (),                                        
	executionContext (browser -> getExecutionContext ()),       
	  selected_color ("#99BDFF"),                               
	           stamp ((long int)                          this) 
{
	//std::clog << "OutlineTreeModel" << std::endl;

	baseNodeImage = Gdk::Pixbuf::create_from_file (get_icon ("Node.png"));

	for (const auto & field : browser -> getSupportedFields ())
		fieldTypeImages [field -> getType ()] = Gdk::Pixbuf::create_from_file (get_icon (field -> getTypeName () + ".png"));
}

Glib::RefPtr <OutlineTreeModel>
OutlineTreeModel::create (const X3D::SFNode <X3D::Browser> & browser)
{
	//std::clog << "create" << std::endl;
	return Glib::RefPtr <OutlineTreeModel> (new OutlineTreeModel (browser));
}

Gtk::TreeModelFlags
OutlineTreeModel::get_flags_vfunc () const
{
	//std::clog << "get_flags_vfunc" << std::endl;
	return Gtk::TreeModelFlags (0);
}

int
OutlineTreeModel::get_n_columns_vfunc () const
{
	//std::clog << "get_n_columns_vfunc" << std::endl;
	return 4;
}

GType
OutlineTreeModel::get_column_type_vfunc (int index) const
{
	//std::clog << "get_column_type_vfunc" << std::endl;

	switch (index)
	{
		case 0:
			return icon_column .type ();
		case 1:
			return data_column .type ();
		case 2:
			return background_color_column .type ();
		case 3:
			return debug_column .type ();
		default:
			return 0;
	}
}

void
OutlineTreeModel::get_value_vfunc (const OutlineTreeModel::iterator & iter, int column, Glib::ValueBase & value) const
{
	//std::clog << "get_value_vfunc" << std::endl;

	auto data = (OutlineEditorData*) iter .gobj () -> user_data;

	switch (column)
	{
		case 0:
		{
			icon_column_type::ValueType val;
			val .init (icon_column_type::ValueType::value_type ());

			switch (data -> type)
			{
				case OutlineEditorDataType::X3DField:
				{
					auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);
					auto iter  = fieldTypeImages .find (field -> getType ());

					if (iter not_eq fieldTypeImages .end ())
						val .set (iter -> second);

					break;
				}
				case OutlineEditorDataType::X3DBaseNode:
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
			background_color_column_type::ValueType val;
			val .init (background_color_column_type::ValueType::value_type ());

			//val .set(selected_color);

			value .init (background_color_column_type::ValueType::value_type ());
			value = val;

			break;
		}
		case 3:
		{
			debug_column_type::ValueType val;
			val .init (debug_column_type::ValueType::value_type ());

			switch (data -> type)
			{
				case OutlineEditorDataType::X3DField:
				{
					auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);

					val .set (field -> getName () .first ());

					break;
				}
				case OutlineEditorDataType::X3DBaseNode:
				{
					auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (data -> object);

					if (*sfnode)
						val .set (sfnode -> getNodeTypeName () + " " + sfnode -> getNodeName ());

					else
						val .set ("NULL");

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
	//std::clog << "get_path_vfunc" << std::endl;

	auto data = (OutlineEditorData*) iter .gobj () -> user_data;

	Path path;

	for (const auto & parent : basic::adapter (data -> parents. begin () + 1, data -> parents .end ()))
	{
		path .push_back (parent .index);
	}
	
	path .push_back (data -> index);

	return path;
}

bool
OutlineTreeModel::get_iter_vfunc (const Path & path, iterator & iter) const
{
	//std::clog << "get_iter_vfunc" << std::endl;

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
	//std::clog << "iter_n_root_children_vfunc" << std::endl;
	return executionContext -> getRootNodes () .size ();
}

bool
OutlineTreeModel::iter_nth_root_child_vfunc (int index, iterator & iter) const
{
	//std::clog << "iter_nth_root_child_vfunc" << std::endl;

	iter .set_stamp (stamp);

	X3D::MFNode* rootNodes = const_cast <X3D::MFNode*> (&executionContext -> getRootNodes ());

	if ((size_t) index < rootNodes -> size ())
	{
		auto parents = OutlineEditorData::parents_type ();
		parents .emplace_front (OutlineEditorDataType::X3DField, rootNodes, 0);

		iter .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DBaseNode, &rootNodes -> at (index), index, parents);
		return true;
	}

	return false;
}

bool
OutlineTreeModel::iter_has_child_vfunc (const iterator & iter) const
{
	//std::clog << "iter_has_child_vfunc" << std::endl;

	return iter_n_children_vfunc (iter);
}

int
OutlineTreeModel::iter_n_children_vfunc (const iterator & iter) const
{
	//std::clog << "iter_n_children_vfunc" << std::endl;

	auto data = (OutlineEditorData*) iter .gobj () -> user_data;

	switch (data -> type)
	{
		case OutlineEditorDataType::X3DField:
		{
			auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					return 1;
				}
				case X3D::X3DConstants::MFNode:
				{
					return dynamic_cast <X3D::MFNode*> (field) -> size ();
				}
				default:
					return 0;
			}

			return 0;
		}
		case OutlineEditorDataType::X3DBaseNode:
		{
			auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (data -> object);

			if (*sfnode)
				return sfnode -> getFieldDefinitions () .size ();

			return 0;
		}
	}

	return 0;
}

bool
OutlineTreeModel::iter_children_vfunc (const iterator & parent, iterator & iter) const
{
	//std::clog << "iter_children_vfunc" << std::endl;

	return iter_nth_child_vfunc (parent, 0, iter);
}

bool
OutlineTreeModel::iter_nth_child_vfunc (const iterator & parent, int index, iterator & iter) const
{
	//std::clog << "iter_nth_child_vfunc" << std::endl;

	iter .set_stamp (stamp);

	auto* data = (OutlineEditorData*) parent .gobj () -> user_data;

	auto parents = data -> parents;
	parents .emplace_back (data -> type, data -> object, data -> index);

	switch (data -> type)
	{
		case OutlineEditorDataType::X3DField:
		{
			auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (field);

					if (index < 1)
					{
						iter .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DBaseNode, sfnode, index, parents);
						return true;
					}

					return false;
				}
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = dynamic_cast <X3D::MFNode*> (field);

					if ((size_t) index < mfnode -> size ())
					{
						iter .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DBaseNode, &mfnode -> at (index), index, parents);
						return true;
					}

					return false;
				}
				default:
					return false;
			}

			return false;
		}
		case OutlineEditorDataType::X3DBaseNode:
		{
			auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (data -> object);
			auto node   = sfnode -> getValue ();

			if ((size_t) index < node -> getFieldDefinitions () .size ())
			{
				iter .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DField, getFields (node) [index], index, parents);
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
	//std::clog << "iter_next_vfunc" << std::endl;

	iter_next .set_stamp (stamp);

	auto   data  = (OutlineEditorData*) iter .gobj () -> user_data;
	size_t index = data -> index + 1;

	switch (data -> type)
	{
		case OutlineEditorDataType::X3DField:
		{
			auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (data -> parents .back () .object);
			auto node   = sfnode -> getValue ();

			if (index < node -> getFieldDefinitions () .size ())
			{
				auto                  field     = getFields (node) [index];
				iter_next .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DField, field, index, data -> parents);
				return true;
			}

			return false;
		}
		case OutlineEditorDataType::X3DBaseNode:
		{
			auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> parents .back () .object);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::MFNode:
				{
					auto mfnode = dynamic_cast <X3D::MFNode*> (field);

					if (index < mfnode -> size ())
					{
						iter_next .gobj () -> user_data = new OutlineEditorData (OutlineEditorDataType::X3DBaseNode, &mfnode -> at (index), index, data -> parents);
						return true;
					}
				}
				default:
					return false;
			}

			return false;
		}
	}

	return false;
}

bool
OutlineTreeModel::iter_parent_vfunc (const iterator & child, iterator & iter) const
{
	//std::clog << "iter_parent_vfunc" << std::endl;

	iter .set_stamp (stamp);

	auto data = (OutlineEditorData*) child .gobj () -> user_data;

	auto parents = data -> parents;

	if (parents .size () == 1)
		return false;

	OutlineEditorDataType type   = parents .back () .type;
	int                   index  = parents .back () .index;
	X3D::X3DObject*       object = parents .back () .object;

	parents .pop_back ();

	iter .gobj () -> user_data = new OutlineEditorData (type, object, index, parents);
	return true;
}

void
OutlineTreeModel::ref_node_vfunc (const iterator & iter) const
{
	//std::clog << "ref_node_vfunc" << std::endl;
}

void
OutlineTreeModel::unref_node_vfunc (const iterator & iter) const
{
	//std::clog << "unref_node_vfunc" << std::endl;
}

X3D::FieldDefinitionArray
OutlineTreeModel::getFields (const X3D::X3DBaseNode* const node)
{
	X3D::FieldDefinitionArray fields            = std::move (node -> getPreDefinedFields ());
	X3D::FieldDefinitionArray userDefinedFields = std::move (node -> getUserDefinedFields ());

	if (fields .size ())
		fields .insert (fields .begin () + 1, userDefinedFields .begin (), userDefinedFields .end ());

	else
		fields = std::move (userDefinedFields);

	return fields;
}

OutlineTreeModel::~OutlineTreeModel ()
{
	//std::clog << "~OutlineTreeModel" << std::endl;
}

} // puck
} // titania
