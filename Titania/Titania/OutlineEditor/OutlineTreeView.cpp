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

namespace titania {
namespace puck {

OutlineTreeView::OutlineTreeView (const X3D::X3DSFNode <X3D::Browser> & browser) :
	   Gtk::TreeView (),
	X3DBaseInterface (),
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

	// Image

	Gtk::CellRendererPixbuf* cellrenderer_icon = Gtk::manage (new Gtk::CellRendererPixbuf ());
	treeviewcolumn_name -> pack_start (*cellrenderer_icon, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "pixbuf", 0);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "cell-background-set", 2);
	cellrenderer_icon -> property_cell_background_rgba () = selectedBackgroundColor;

	// Node typeName or field name

	Gtk::CellRendererText* cellrenderer_name = Gtk::manage (new Gtk::CellRendererText ());
	treeviewcolumn_name -> pack_start (*cellrenderer_name, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "markup", 3);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "foreground-set", 2);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "background-set", 2);
	cellrenderer_name -> property_foreground_rgba () = selectedForegroundColor;
	cellrenderer_name -> property_background_rgba () = selectedBackgroundColor;

	//	Gtk::CellRendererText* cellrenderer_test = Gtk::manage (new Gtk::CellRendererText ());
	//	treeviewcolumn_name -> pack_start (*cellrenderer_test, false);
	//	treeviewcolumn_name -> add_attribute (*cellrenderer_test, "markup", 3);
	//	treeviewcolumn_name -> add_attribute (*cellrenderer_test, "cell-background-rgba", 2);

	append_column (*treeviewcolumn_name);

	//	Gtk::TreeViewColumn* treeviewcolumn_debug = Gtk::manage (new Gtk::TreeViewColumn ("Debug"));
	//
	//	Gtk::CellRendererText* cellrenderer_debug = Gtk::manage (new Gtk::CellRendererText ());
	//	treeviewcolumn_debug -> pack_start (*cellrenderer_debug, false);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "text", 3);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "cell-background-rgba", 2);
	//
	//	append_column (*treeviewcolumn_debug);

	getBrowser () -> initialized .addInterest (this, &OutlineTreeView::set_world);
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

	selection .setSelectMultiple (keys .shift ());

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

	selection .setSelectMultiple (keys .shift ());

	return false;
}

void
OutlineTreeView::on_row_inserted (const Gtk::TreeModel::Path & path, const Gtk::TreeModel::iterator & iter)
{ }

void
OutlineTreeView::on_row_has_child_toggled (const Gtk::TreeModel::Path & path, const Gtk::TreeModel::iterator & iter)
{
	// Expand row again if it was previously expanded.

	auto userData = OutlineTreeModel::getUserData (iter);

	if (userData)
	{
		if (userData -> expanded)
			expand_row (path, false);
	}
}

bool
OutlineTreeView::on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	collapseClone (iter);
	selectFields (iter);

	// Return false to allow expansion, true to reject.
	return false;
}

void
OutlineTreeView::on_row_expanded (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	setPath (iter, path);
	setExpanded (iter, true);
	watch (iter, true);
	autoExpandFields (iter);
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
	setExpanded (iter, false);
	watch (iter, false);
	toggleExpand (iter, path);
}

void
OutlineTreeView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select (get_model () -> get_iter (path), path);
}

void
OutlineTreeView::setPath (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & value)
{
	auto userData = OutlineTreeModel::getUserData (iter);

	if (userData)
		userData -> path = value;
}

void
OutlineTreeView::setExpanded (const Gtk::TreeModel::iterator & iter, bool value)
{
	auto userData = OutlineTreeModel::getUserData (iter);

	if (userData)
		userData -> expanded = value;
}

void
OutlineTreeView::watch (const Gtk::TreeModel::iterator & iter, bool value)
{
	auto data = OutlineTreeModel::getData (iter);

	if (data -> type == OutlineIterType::X3DBaseNode)
	{
		for (const auto & child : iter -> children ())
		{
			auto data  = OutlineTreeModel::getData (child);
			auto field = static_cast <X3D::X3DFieldDefinition*> (data -> object);

			if (value)
				field -> addInterest ((Gtk::Widget*) this, &OutlineTreeView::queue_draw);

			else
				field -> removeInterest ((Gtk::Widget*) this, &OutlineTreeView::queue_draw);
		}
	}
}

void
OutlineTreeView::selectFields (const Gtk::TreeModel::iterator & iter)
{
	auto data = OutlineTreeModel::getData (iter);

	if (data -> type == OutlineIterType::X3DBaseNode)
	{
		auto data         = OutlineTreeModel::getData (iter);
		auto nodeUserData = OutlineTreeModel::getUserData (iter);
		auto sfnode       = static_cast <X3D::SFNode*> (data -> object);
		auto node         = sfnode -> getValue ();
		
		bool visible = false;

		for (const auto & field : node -> getFieldDefinitions ())
		{
			auto userData = OutlineTreeModel::getUserData (field);

			if (nodeUserData -> showAllFields)
				userData -> visible = true;
			
			else
			{
				if (not field -> isInitializeable ())
				{
					userData -> visible = false;
					continue;
				}

				if (node -> isDefaultValue (field))
				{
					userData -> visible = false;
					continue;
				}

				userData -> visible = true;
				visible = true;
			}
		}
		
		if (not visible)
		{
			for (const auto & field : node -> getFieldDefinitions ())
			{
				auto userData = OutlineTreeModel::getUserData (field);
					userData -> visible = true;
			}
			
			nodeUserData -> showAllFields = true;
		}
	}
}

void
OutlineTreeView::autoExpandFields (const Gtk::TreeModel::iterator & iter)
{
	auto data = OutlineTreeModel::getData (iter);

	switch (data -> type)
	{
		case OutlineIterType::X3DFieldValue:
			break;

		case OutlineIterType::X3DField:
		{
			for (const auto & child : iter -> children ())
			{
				auto userData = OutlineTreeModel::getUserData (child);

				if (userData)
				{
					if (userData -> expanded)
						expand_row (Gtk::TreePath (child), false);
				}
			}

			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			for (const auto & child : iter -> children ())
			{
				auto data     = OutlineTreeModel::getData (child);
				auto userData = OutlineTreeModel::getUserData (child);
				auto field    = static_cast <X3D::X3DFieldDefinition*> (data -> object);

				switch (field -> getType ())
				{
					case X3D::X3DConstants::SFNode:
					{
						auto sfnode = static_cast <X3D::SFNode*> (field);

						if (*sfnode or userData -> expanded)
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
						if (userData -> expanded)
							expand_row (Gtk::TreePath (child), false);

						break;
					}
				}
			}
		}
	}
}

void
OutlineTreeView::toggleExpand (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	auto data = OutlineTreeModel::getData (iter);

	if (data -> type == OutlineIterType::X3DBaseNode)
	{
		auto userData = OutlineTreeModel::getUserData (iter);

		if (userData)
		{
			Glib::RefPtr <OutlineTreeModel>::cast_dynamic (get_model ()) -> collapse_row (path, iter);

			userData -> showAllFields = not userData -> showAllFields;

			if (userData -> showAllFields)
				expand_row (Gtk::TreePath (iter), false);
		}
	}
}

void
OutlineTreeView::collapseClone (const Gtk::TreeModel::iterator & iter)
{
	auto data = OutlineTreeModel::getData (iter);

	if (data -> type == OutlineIterType::X3DBaseNode)
	{
		auto userData = OutlineTreeModel::getUserData (iter);

		if (userData)
		{
			if (userData -> expanded)
			{
				collapse_row (userData -> path);

				if (userData -> showAllFields)
					collapse_row (userData -> path);
			}
		}
	}
}

void
OutlineTreeView::select (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	auto data = OutlineTreeModel::getData (iter);

	if (data -> type == OutlineIterType::X3DBaseNode)
	{
		Glib::RefPtr <OutlineTreeModel>::cast_dynamic (get_model ()) -> collapse_row (path, iter);

		selection .select (*static_cast <X3D::SFNode*> (data -> object));
	}
}

OutlineTreeView::~OutlineTreeView ()
{ }

} // puck
} // titania
