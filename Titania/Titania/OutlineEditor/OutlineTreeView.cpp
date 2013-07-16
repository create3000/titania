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

#include "OutlineTreeView.h"

#include "../OutlineEditor/OutlineTreeModel.h"
#include "../OutlineEditor/CellRenderer/OutlineCellRenderer.h"

namespace titania {
namespace puck {

OutlineTreeView::OutlineTreeView (const X3D::X3DSFNode <X3D::Browser> & browser) :
	   Gtk::TreeView (),
	X3DBaseInterface (),
	           model (),
	       selection (this, browser),
	            keys ()
{
	setBrowser (browser);

	set_headers_visible (true);
	set_enable_search (false);
	get_selection () -> set_mode (Gtk::SELECTION_NONE);
	get_style_context () -> add_class ("OutlineTreeView");

	// Columns

	auto selectedForegroundColor = get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	auto selectedBackgroundColor = get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);

	Gtk::TreeViewColumn* treeviewcolumn_name = Gtk::manage (new Gtk::TreeViewColumn ("Name"));
	treeviewcolumn_name -> set_expand (false);

	// Image

	Gtk::CellRendererPixbuf* cellrenderer_icon = Gtk::manage (new Gtk::CellRendererPixbuf ());
	treeviewcolumn_name -> pack_start (*cellrenderer_icon, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "pixbuf",              OutlineTreeModel::ICON_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);
	//treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "visible",           OutlineTreeModel::SELECTED_COLUMN);
	cellrenderer_icon -> property_cell_background_rgba () = selectedBackgroundColor;

	// Node typeName or field name

	Gtk::CellRendererText* cellrenderer_name = Gtk::manage (new Gtk::CellRendererText ());
	treeviewcolumn_name -> pack_start (*cellrenderer_name, true);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "markup",         OutlineTreeModel::NAME_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "foreground-set", OutlineTreeModel::SELECTED_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "background-set", OutlineTreeModel::SELECTED_COLUMN);
	cellrenderer_name -> property_foreground_rgba () = selectedForegroundColor;
	cellrenderer_name -> property_background_rgba () = selectedBackgroundColor;

//	// AccessType Image
//
//	Gtk::CellRendererPixbuf* cellrenderer_access_type = Gtk::manage (new Gtk::CellRendererPixbuf ());
//	treeviewcolumn_name -> pack_start (*cellrenderer_access_type, false);
//	treeviewcolumn_name -> add_attribute (*cellrenderer_access_type, "pixbuf",              OutlineTreeModel::ACCESS_TYPE_ICON_COLUMN);
//	treeviewcolumn_name -> add_attribute (*cellrenderer_access_type, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);
//	cellrenderer_access_type -> property_cell_background_rgba () = selectedBackgroundColor;

//	// CellRenderer
//	OutlineCellRenderer* cellrenderer = Gtk::manage (new OutlineCellRenderer ());
//	treeviewcolumn_name -> pack_start (*cellrenderer, false);
//	treeviewcolumn_name -> add_attribute (*cellrenderer, "tree-data",           OutlineTreeModel::DATA_COLUMN);
//	treeviewcolumn_name -> add_attribute (*cellrenderer, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);
//	cellrenderer -> property_cell_background_rgba () = selectedBackgroundColor;

	// Append column

	append_column (*treeviewcolumn_name);
	
	// Register browser interest

	getBrowser () -> initialized .addInterest (this, &OutlineTreeView::set_world);
}

void
OutlineTreeView::set_model (const Glib::RefPtr <OutlineTreeModel> & value)
{
	model = value;
	Gtk::TreeView::set_model (value);
}

OutlineUserDataPtr
OutlineTreeView::get_user_data (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_user_data (iter);
}

OutlineUserDataPtr
OutlineTreeView::get_user_data (X3D::X3DChildObject* object) const
{
	return get_model () -> get_user_data (object);
}

OutlineIterType
OutlineTreeView::get_data_type (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_data_type (iter);
}

X3D::X3DChildObject*
OutlineTreeView::get_object (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_object (iter);
}

void
OutlineTreeView::set_path (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & value)
{
	auto userData = get_user_data (iter);

	if (userData)
		userData -> path = value;
}

Gtk::TreeModel::Path
OutlineTreeView::get_path (const Gtk::TreeModel::iterator & iter) const
{
	auto userData = get_user_data (iter);

	if (userData)
		return userData -> path;

	return Gtk::TreeModel::Path ();
}

void
OutlineTreeView::set_expanded (const Gtk::TreeModel::iterator & iter, bool value)
{
	auto userData = get_user_data (iter);

	if (userData)
		userData -> expanded = value;
}

bool
OutlineTreeView::get_expanded (const Gtk::TreeModel::iterator & iter) const
{
	auto userData = get_user_data (iter);

	if (userData)
		return userData -> expanded;
	
	return false;
}

void
OutlineTreeView::set_expand_all (const Gtk::TreeModel::iterator & iter, bool value)
{
	auto userData = get_user_data (iter);

	if (userData)
		userData -> expand_all = value;
}

bool
OutlineTreeView::get_expand_all (const Gtk::TreeModel::iterator & iter) const
{
	auto userData = get_user_data (iter);

	if (userData)
		return userData -> expand_all;
	
	return false;
}

void
OutlineTreeView::set_animated (const Gtk::TreeModel::iterator & iter, bool value)
{
	auto userData = get_user_data (iter);

	if (userData)
		userData -> animated = value;
}

bool
OutlineTreeView::get_animated (const Gtk::TreeModel::iterator & iter) const
{
	auto userData = get_user_data (iter);

	if (userData)
		return userData -> animated;
	
	return false;
}

void
OutlineTreeView::set_world ()
{
	selection .clear ();

	set_model (OutlineTreeModel::create (getBrowser ()));

	get_model () -> signal_row_has_child_toggled () .connect (sigc::mem_fun (this, &OutlineTreeView::on_row_has_child_toggled));
}

bool
OutlineTreeView::on_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			keys .shift_l = true;
			break;
		case GDK_KEY_Shift_R:
			keys .shift_r = true;
			break;
		default:
			break;
	}

	selection .set_select_multiple (keys .shift ());

	return false;
}

bool
OutlineTreeView::on_key_release_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			keys .shift_l = false;
			break;
		case GDK_KEY_Shift_R:
			keys .shift_r = false;
			break;
		default:
			break;
	}

	selection .set_select_multiple (keys .shift ());

	return false;
}

void
OutlineTreeView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select (get_model () -> get_iter (path), path);
}

void
OutlineTreeView::on_row_inserted (const Gtk::TreeModel::Path & path, const Gtk::TreeModel::iterator & iter)
{ }

void
OutlineTreeView::on_row_has_child_toggled (const Gtk::TreeModel::Path & path, const Gtk::TreeModel::iterator & iter)
{
	// Expand row again if it was previously expanded.

	if (get_expanded (iter))
		expand_row (path, false);
}

bool
OutlineTreeView::on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	//__LOG__ << std::endl;

	collapse_clone (iter);

	select_fields (iter, path);

	// Return false to allow expansion, true to reject.
	return false;
}

void
OutlineTreeView::on_row_expanded (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	//__LOG__ << std::endl;

	// Set expanded first to prevent loop with clones.
	set_expanded (iter, true);
	set_path (iter, path);
	set_animated (iter, false);

	watch (iter, path);

	auto_expand_fields (iter);
}

bool
OutlineTreeView::on_test_collapse_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	// Return false to allow collapse, true to reject.
	return false;
}

void
OutlineTreeView::on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	//__LOG__ << std::endl;

	unwatch_tree (path);
	get_model () -> collapse_row (path);

	set_expanded (iter, false);

	toggle_expand (iter, path);
}

void
OutlineTreeView::watch (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DFieldValue:
			break;
		case OutlineIterType::X3DField:
		{
			auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));
	 
			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					field -> addInterest (this, &OutlineTreeView::collapse_field, field, path, 1);
					break;
				}
				case X3D::X3DConstants::MFNode:
				{
					field -> addInterest (this, &OutlineTreeView::collapse_field, field, path, static_cast <X3D::MFNode*> (field) -> size ());
					break;
				}
				default:
				{
					field -> addInterest (this, &OutlineTreeView::set_field, path);
					break;
				}
			}
			
			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			for (const auto & child : iter -> children ())
			{
				switch (static_cast <X3D::X3DFieldDefinition*> (get_object (child)) -> getType ())
				{
					case X3D::X3DConstants::MFNode:
					{
						X3D::MFNode* field = static_cast <X3D::MFNode*> (get_object (child));
						
						field -> addInterest (this, &OutlineTreeView::set_children, get_model () -> get_path (child));

						break;
					}
					default:
						break;
				}
			}
			break;
		}
	}
}

void
OutlineTreeView::unwatch_tree (const Gtk::TreeModel::Path & path)
{
	unwatch_tree (true, get_model () -> get_tree () .getNode (path));
}

void
OutlineTreeView::unwatch_tree (bool root, const OutlineNode & treeNode)
{
	unwatch (root, treeNode .data);

	for (const auto & child : treeNode .children)
		unwatch_tree (false, child);
}

void
OutlineTreeView::unwatch (bool root, const OutlineTreeData* data)
{
	if (data -> type () == OutlineIterType::X3DField)
	{
		auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object ());

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				field -> removeInterest (this, &OutlineTreeView::collapse_field);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				field -> removeInterest (this, &OutlineTreeView::collapse_field);

				if (not root)
					field -> removeInterest (this, &OutlineTreeView::set_children);

				break;
			}
			default:
			{
				field -> removeInterest (this, &OutlineTreeView::set_field);
				break;
			}
		}
	}
}

void
OutlineTreeView::set_field (const Gtk::TreeModel::Path & path)
{
	Gtk::TreeModel::Path child = path;
	child .push_back (0);
	
	Gtk::TreeModel::iterator iter = get_model () -> get_iter (child);
	
	get_model () -> row_changed (child, iter);
}

void
OutlineTreeView::collapse_field (X3D::X3DFieldDefinition* const field, const Gtk::TreeModel::Path & path, size_t size)
{
	__LOG__ << X3D::SFTime (chrono::now ()) << std::endl;
	
	Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

	set_animated (iter, true);	
	set_expanded (iter, false);
	
	// Remove all rows
	
	Gtk::TreeModel::Path child = path;
	child .push_back (0);
	
	for (size_t i = 0; i < size; ++ i)
		get_model () -> row_deleted (child);
		
	//

	unwatch_tree (path);
	get_model () -> collapse_row (path);

	// Add expander or watch for children

	get_model () -> row_has_child_toggled (path, iter);
}

void
OutlineTreeView::set_children (const Gtk::TreeModel::Path & path)
{
	Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

	get_model () -> row_has_child_toggled (path, iter);
}

void
OutlineTreeView::collapse_clone (const Gtk::TreeModel::iterator & iter)
{
	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	{
		if (get_expanded (iter))
		{
			set_expand_all (iter, false);
		
			collapse_row (get_path (iter));
		}
	}
}

void
OutlineTreeView::select_fields (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	{
		auto nodeUserData = get_user_data (iter);

		if (nodeUserData)
		{	
			auto sfnode = static_cast <X3D::SFNode*> (get_object (iter));
			auto node   = sfnode -> getValue ();
			
			if (node)
			{
				// Select visible fields

				if (get_expand_all (iter))
				{
					for (const auto & field : node -> getFieldDefinitions ())
						get_user_data (field)-> visible = true;

					set_expand_all (iter, false);
				}
				else
				{
					bool visible = false;

					for (const auto & field : node -> getFieldDefinitions ())
					{
						if (not field -> isInitializeable () or node -> isDefaultValue (field))
						{
							get_user_data (field)-> visible = false;
							continue;
						}

						get_user_data (field)-> visible = true;
						visible = true;
					}
					
					if (visible)
						set_expand_all (iter, true);

					else
					{
						for (const auto & field : node -> getFieldDefinitions ())
							get_user_data (field)-> visible = true;

						set_expand_all (iter, false);
					}
				}
			}
		}
	}
}

void
OutlineTreeView::toggle_expand (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	{
		if (get_expand_all (iter))
			expand_row (path, false);
	}
}

void
OutlineTreeView::auto_expand_fields (const Gtk::TreeModel::iterator & parent)
{
	switch (get_data_type (parent))
	{
		case OutlineIterType::X3DFieldValue:
			break;

		case OutlineIterType::X3DField:
		{
			for (const auto & child : parent -> children ())
			{
				if (get_expanded (child))
					expand_row (Gtk::TreePath (child), false);
			}

			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			for (const auto & child : parent -> children ())
			{
				auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (child));

				if (get_animated (child))
					continue;

				switch (field -> getType ())
				{
					case X3D::X3DConstants::SFNode:
					{
						auto sfnode = static_cast <X3D::SFNode*> (field);

						if (*sfnode or get_expanded (child))
							expand_row (Gtk::TreePath (child), false);

						break;
					}
					case X3D::X3DConstants::MFNode:
					{
						expand_row (Gtk::TreePath (child), false);
						break;
					}
					default:
					{
						if (get_expanded (child))
							expand_row (Gtk::TreePath (child), false);

						break;
					}
				}
			}
		}
	}
}

void
OutlineTreeView::select (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	selection .select (iter, path);
}

OutlineTreeView::~OutlineTreeView ()
{ }

} // puck
} // titania
