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

	if (index == 0)
		return icon_column .type ();
	else if (index == 1)
		return data_column .type ();
	else if (index == 2)
		return background_color_column .type ();
	else if (index == 3)
		return debug_column .type ();
	else
		return 0;
}

void
OutlineTreeModel::get_value_vfunc (const OutlineTreeModel::iterator & iter, int column, Glib::ValueBase & value) const
{
	//std::clog << "get_value_vfunc" << std::endl;

	if (iter_is_valid (iter))
	{
		Data* data = (Data*) iter .gobj () -> user_data;

		switch (column)
		{
			case 0:
			{
				icon_column_type::ValueType val;
				val .init (icon_column_type::ValueType::value_type ());

				if (dynamic_cast <X3D::X3DBaseNode*> (data -> object))
					val .set (baseNodeImage);

				else if (dynamic_cast <X3D::X3DFieldDefinition*> (data -> object))
				{
					auto iter = fieldTypeImages .find (dynamic_cast <X3D::X3DFieldDefinition*> (data -> object) -> getType ());

					if (iter not_eq fieldTypeImages .end ())
					{
						val .set (iter -> second);
					}
				}
				else if (data -> object == 0 and dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (data -> sfnode))
					val .set (baseNodeImage);

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

				if (dynamic_cast <X3D::X3DBaseNode*> (data -> object))
					val .set (dynamic_cast <X3D::X3DBaseNode*> (data -> object) -> getTypeName () + " " + dynamic_cast <X3D::X3DBaseNode*> (data -> object) -> getName ());

				else if (dynamic_cast <X3D::X3DFieldDefinition*> (data -> object))
					val .set (dynamic_cast <X3D::X3DFieldDefinition*> (data -> object) -> getName () .first ());

				else if (data -> object == NULL and dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (data -> sfnode))
					val .set ("NULL");

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
	else
	{
		switch (column)
		{
			case 0:
				value .init (icon_column_type::ValueType::value_type ());
				break;
			case 1:
				value .init (data_column_type::ValueType::value_type ());
				break;
			case 2:
				value .init (background_color_column_type::ValueType::value_type ());
				break;
			case 3:
				value .init (debug_column_type::ValueType::value_type ());
				break;
			default:
				g_assert_not_reached ();
				break;
		}
	}
}

bool
OutlineTreeModel::iter_next_vfunc (const iterator & iter, iterator & iter_next) const
{
	//std::clog << "iter_next_vfunc" << std::endl;

	iter_next = iterator ();

	if (iter_is_valid (iter))
	{
		Data* data = (Data*) iter .gobj () -> user_data;
		int   i    = data -> index + 1;

		X3D::X3DObject* parent = data -> parents [0] .object;

		if (dynamic_cast <X3D::X3DBaseNode*> (parent))
		{
			auto node = dynamic_cast <X3D::X3DBaseNode*> (parent);

			if ((size_t) i < node -> getFieldDefinitions () .size ())
			{
				iter_next .set_stamp (stamp);
				iter_next .gobj () -> user_data = (void*) new Data (i, node -> getFieldDefinitions () [i], 0, data -> parents);
				return true;
			}
		}
		else if (dynamic_cast <X3D::MFNode*> (parent))
		{
			auto mfnode = dynamic_cast <X3D::MFNode*> (parent);

			if ((size_t) i < mfnode -> size ())
			{
				auto value = dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (&mfnode -> get1Value (i)) -> getValue ();

				iter_next .set_stamp (stamp);
				iter_next .gobj () -> user_data = (void*) new Data (i, value, &mfnode -> get1Value (i), data -> parents);
				return true;
			}
		}
		else if (dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (parent))
		{
			auto sfnode = dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (parent);

			if (i < 1)
			{
				iter_next .set_stamp (stamp);
				iter_next .gobj () -> user_data = (void*) new Data (i, sfnode -> getValue (), sfnode, data -> parents);
				return true;
			}
		}
	}

	return false;
}

bool
OutlineTreeModel::iter_children_vfunc (const iterator & parent, iterator & iter) const
{
	//std::clog << "iter_children_vfunc" << std::endl;

	return iter_nth_child_vfunc (parent, 0, iter);
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

	if (iter_is_valid (iter))
	{
		Data* data = (Data*) iter .gobj () -> user_data;

		if (dynamic_cast <X3D::X3DBaseNode*> (data -> object))
		{
			X3D::X3DBaseNode* node = dynamic_cast <X3D::X3DBaseNode*> (data -> object);

			Data::parents_type::const_iterator parent;

			for (parent = data -> parents .begin (); parent not_eq data -> parents .end (); ++ parent)
			{
				if (data -> object == (*parent) .object)
					return 0;
			}

			return node -> getFieldDefinitions () .size ();

		}
		else if (dynamic_cast <X3D::MFNode*> (data -> object))
		{
			return dynamic_cast <X3D::MFNode*> (data -> object) -> size ();
		}
		else if (dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (data -> object))
		{
			return 1;
		}
	}

	return 0;
}

int
OutlineTreeModel::iter_n_root_children_vfunc () const
{
	//std::clog << "iter_n_root_children_vfunc" << std::endl;
	return executionContext -> getRootNodes () .size ();
}

bool
OutlineTreeModel::iter_nth_child_vfunc (const iterator & parent, int n, iterator & iter) const
{
	//std::clog << "iter_nth_child_vfunc" << std::endl;

	iter = iterator ();  // invalid by default -- we have no children

	if (iter_is_valid (parent))
	{
		Data* data = (Data*) parent .gobj () -> user_data;

		Data::parents_type parents = data -> parents;
		parents .push_front (Item (data -> index, data -> object, data -> sfnode));

		if (dynamic_cast <X3D::X3DBaseNode*> (data -> object))
		{
			auto node = dynamic_cast <X3D::X3DBaseNode*> (data -> object);

			if ((size_t) n < node -> getFieldDefinitions () .size ())
			{
				iter .set_stamp (stamp);
				iter .gobj () -> user_data = (void*) new Data (n, node -> getFieldDefinitions () [n], 0, parents);
				return true;
			}
		}
		else if (dynamic_cast <X3D::MFNode*> (data -> object))
		{
			auto mfnode = dynamic_cast <X3D::MFNode*> (data -> object);

			if ((size_t) n < mfnode -> size ())
			{
				auto value = dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (&mfnode -> get1Value (n)) -> getValue ();

				iter .set_stamp (stamp);
				iter .gobj () -> user_data = (void*) new Data (n, value, &mfnode -> get1Value (n), parents);
				return true;
			}
		}
		else if (dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (data -> object))
		{
			auto sfnode = dynamic_cast <X3D::X3DField <X3D::X3DBaseNode*>*> (data -> object);

			if (n < 1)
			{
				iter .set_stamp (stamp);
				iter .gobj () -> user_data = (void*) new Data (n, sfnode -> getValue (), sfnode, parents);
				return true;
			}
		}
	}

	return false;
}

bool
OutlineTreeModel::iter_nth_root_child_vfunc (int n, iterator & iter) const
{
	//std::clog << "iter_nth_root_child_vfunc" << std::endl;

	iter = iterator ();   // invalid by default.

	X3D::MFNode* parent = const_cast <X3D::MFNode*> (&executionContext -> getRootNodes ());

	if ((size_t) n < parent -> size ())
	{
		iter .set_stamp (stamp);

		X3D::X3DObject*    object = parent -> at (n);
		X3D::X3DObject*    data   = &parent -> at (n);
		Data::parents_type parents;
		parents .push_front (Item (-1, parent, 0));

		iter .gobj () -> user_data = (void*) new Data (n, object, data, parents);
		return true;
	}

	return false;
}

bool
OutlineTreeModel::iter_parent_vfunc (const iterator & child, iterator & iter) const
{
	//std::clog << "iter_parent_vfunc" << std::endl;

	iter = iterator ();

	if (iter_is_valid (child))
	{
		Data* data = (Data*) child .gobj () -> user_data;

		Data::parents_type parents = data -> parents;

		int             i      = parents [0] .index;
		X3D::X3DObject* object = parents [0] .object;
		X3D::X3DObject* sfnode = parents [0] .sfnode;

		if (object == &executionContext -> getRootNodes ())
			return false;

		parents .pop_front ();

		iter .set_stamp (stamp);
		iter .gobj () -> user_data = (void*) new Data (i, object, sfnode, parents);
		return true;
	}

	return false;
}

Gtk::TreeModel::Path
OutlineTreeModel::get_path_vfunc (const iterator & iter) const
{
	//std::clog << "get_path_vfunc" << std::endl;

	Path path;

	if (iter_is_valid (iter))
	{
		Data* data = (Data*) iter .gobj () -> user_data;

		path .push_front (data -> index);

		Data::parents_type::const_iterator parent;

		for (parent = data -> parents .begin (); parent not_eq data -> parents .end () - 1; ++ parent)
		{
			path .push_front ((*parent) .index);
		}

	}

	return path;
}

bool
OutlineTreeModel::get_iter_vfunc (const Path & path, iterator & iter) const
{
	//std::clog << "get_iter_vfunc" << std::endl;

	Path::const_iterator index = path .begin ();

	//std::clog << "get_iter_vfunc " << *index << std::endl;

	if (not iter_nth_root_child_vfunc (*index, iter))
		return false;

	for (++ index; index not_eq path .end (); ++ index)
	{
		//std::clog << "get_iter_vfunc " << *index << std::endl;
		iterator parent = iter;

		if (not iter_nth_child_vfunc (parent, *index, iter))
			return false;
	}

	return true;
}

bool
OutlineTreeModel::iter_is_valid (const iterator & iter) const
{
	if (iter .get_stamp () not_eq stamp)
	{
		return false;
	}

	// GTK2 TODO: not sure why we do this but it's done in the gtkmm
	// GTK2 example...
	// GTK2 return Gtk::TreeModel::iter_is_valid (iter);
	return true;
}

OutlineTreeModel::~OutlineTreeModel ()
{
	//std::clog << "~OutlineTreeModel" << std::endl;
}

} // puck
} // titania
