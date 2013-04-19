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

#include "OutlineTreeView.h"

#include "../OutlineEditor/OutlineTreeModel.h"
#include <Titania/X3D.h>

namespace titania {
namespace puck {

OutlineTreeView::OutlineTreeView (const X3D::SFNode <X3D::Browser> & browser) :
	   Gtk::TreeView (), 
	X3DBaseInterface ()  
{
	setBrowser (browser);

	set_headers_visible (true);
	set_enable_search (false);
	get_selection () -> set_mode (Gtk::SELECTION_NONE);

	Gtk::TreeViewColumn* treeviewcolumn_name = Gtk::manage (new Gtk::TreeViewColumn ("Name"));

	// Image

	Gtk::CellRendererPixbuf* cellrenderer_icon = Gtk::manage (new Gtk::CellRendererPixbuf ());
	treeviewcolumn_name -> pack_start (*cellrenderer_icon, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "pixbuf", 0);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "cell-background-gdk", 2);

	// Node typeName or field name

	Gtk::CellRendererText* cellrenderer_name = Gtk::manage (new Gtk::CellRendererText ());
	treeviewcolumn_name -> pack_start (*cellrenderer_name, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "markup", 3);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "cell-background-gdk", 2);

//	Gtk::CellRendererText* cellrenderer_test = Gtk::manage (new Gtk::CellRendererText ());
//	treeviewcolumn_name -> pack_start (*cellrenderer_test, false);
//	treeviewcolumn_name -> add_attribute (*cellrenderer_test, "markup", 3);
//	treeviewcolumn_name -> add_attribute (*cellrenderer_test, "cell-background-gdk", 2);

	append_column (*treeviewcolumn_name);

	//	Gtk::TreeViewColumn* treeviewcolumn_debug = Gtk::manage (new Gtk::TreeViewColumn ("Debug"));
	//
	//	Gtk::CellRendererText* cellrenderer_debug = Gtk::manage (new Gtk::CellRendererText ());
	//	treeviewcolumn_debug -> pack_start (*cellrenderer_debug, false);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "text", 3);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "cell-background-gdk", 2);
	//
	//	append_column (*treeviewcolumn_debug);

	getBrowser () -> initialized .addInterest (this, &OutlineTreeView::set_world);
}

void
OutlineTreeView::set_world ()
{
	set_model (OutlineTreeModel::create (getBrowser ()));
}

bool
OutlineTreeView::on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	auto data = (OutlineTreeModel::Data*) iter .gobj () -> user_data;
	
	if (data -> type == OutlineTreeModel::DataType::X3DBaseNode)
	{
		auto userData = OutlineTreeModel::getUserData (iter);
		
		if (userData)
		{
			if (userData -> expanded)
			{
				userData -> showAllFields = true;
				collapse_row (userData -> path);
			}
		}
	}

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

void
OutlineTreeView::on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	setExpanded (iter, false);
	watch (iter, false);
	toggleExpand (iter);
}

void
OutlineTreeView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select (get_model () -> get_iter (path));
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
	auto data = (OutlineTreeModel::Data*) iter .gobj () -> user_data;
	
	if (data -> type == OutlineTreeModel::DataType::X3DBaseNode)
	{
		auto   children = iter -> children ();
		size_t size     = children .size ();
		
		for (size_t i = 0; i < size; ++ i)
		{
			auto child = children [i];
	
			auto data  = (OutlineTreeModel::Data*) child .gobj () -> user_data;
			auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);
			
			if (value)
				field -> addInterest ((Gtk::Widget*) this, &OutlineTreeView::queue_draw);
				
			else
				field -> removeInterest ((Gtk::Widget*) this, &OutlineTreeView::queue_draw);
		}
	}
}

void
OutlineTreeView::autoExpandFields (const Gtk::TreeModel::iterator & iter)
{
	auto data = (OutlineTreeModel::Data*) iter .gobj () -> user_data;
	
	switch (data -> type)
	{
		case OutlineTreeModel::DataType::X3DFieldValue:
			break;
			
		case OutlineTreeModel::DataType::X3DField:
		{
			auto   children = iter -> children ();
			size_t size     = children .size ();
			
			for (size_t i = 0; i < size; ++ i)
			{
				auto child = children [i];
				
				auto userData = OutlineTreeModel::getUserData (child);
				
				if (userData)
					if (userData -> expanded)
						expand_row (Gtk::TreePath (child), false);
			}
			
			break;
		}
		case OutlineTreeModel::DataType::X3DBaseNode:
		{
			auto   children = iter -> children ();
			size_t size     = children .size ();
			
			for (size_t i = 0; i < size; ++ i)
			{
				auto child = children [i];
		
				auto data  = (OutlineTreeModel::Data*) child .gobj () -> user_data;
				auto field = dynamic_cast <X3D::X3DFieldDefinition*> (data -> object);
				
				switch (field -> getType ())
				{
					case X3D::X3DConstants::SFNode:
					{
						auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (field);
						
						if (*sfnode)
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
						if (OutlineTreeModel::getUserData (field) -> expanded)
							expand_row (Gtk::TreePath (child), false);
						break;
					}
				}
			}
		}
	}
}

void
OutlineTreeView::toggleExpand (const Gtk::TreeModel::iterator & iter)
{
	auto data = (OutlineTreeModel::Data*) iter .gobj () -> user_data;
	
	if (data -> type == OutlineTreeModel::DataType::X3DBaseNode)
	{
		auto userData = OutlineTreeModel::getUserData (iter);
		
		if (userData)
		{
			userData -> showAllFields = not userData -> showAllFields;
			userData -> tainted       = true;

			if (userData -> showAllFields)
				expand_row (Gtk::TreePath (iter), false);
		}
	}
}

void
OutlineTreeView::select (const Gtk::TreeModel::iterator & iter)
{
	auto data = (OutlineTreeModel::Data*) iter .gobj () -> user_data;
	
	if (data -> type == OutlineTreeModel::DataType::X3DBaseNode)
	{
		clearSelection ();
	
		auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (data -> object);
		auto node   = sfnode -> getValue ();
	
		if (node)
		{
			toggleSelection (node);
			selection .push_back (*sfnode);
		}

		queue_draw ();
	}
}

void
OutlineTreeView::clearSelection ()
{
	for (const auto & node : selection)
		toggleSelection (node .getValue ());
	
	selection .clear ();
}

void
OutlineTreeView::toggleSelection (X3D::X3DBaseNode* node)
{
	auto userData = OutlineTreeModel::getUserData (node);
	
	select (node, not userData -> selected);
}

void
OutlineTreeView::select (X3D::X3DBaseNode* node, bool value)
{
	X3D::ChildObjectSet objects;
	
	select (node, value, objects);
}

void
OutlineTreeView::select (X3D::X3DBaseNode* node, bool value, X3D::ChildObjectSet & seen)
{
	if (node)
	{
		if (not seen .insert (node) .second)
			return;
	
		// Select node
		
		auto userData = OutlineTreeModel::getUserData (node);	
		
		if (userData -> selected == value)
			return;
		
		userData -> selected = value;
		
		// Select children
	
		for (const auto & field : node -> getFieldDefinitions ())
			select (field, value, seen);
	}
}

void
OutlineTreeView::select (X3D::X3DFieldDefinition* field, bool value, X3D::ChildObjectSet & seen)
{
	// Select field
		
	auto userData = OutlineTreeModel::getUserData (field);
	if (userData -> selected == value)
		return;
	
	userData -> selected = value;
	
	// Select children

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
		{
			auto sfnode = dynamic_cast <X3D::SFNode <X3D::X3DBaseNode>*> (field);
			select (sfnode -> getValue (), value, seen);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			auto mfnode = dynamic_cast <X3D::MFNode*> (field);
			
			for (auto & sfnode : *mfnode)
				select (&sfnode, value, seen);
			
			break;
		}
		default:
			break;
	}
}

} // puck
} // titania
