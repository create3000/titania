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

#include "OutlineTreeViewEditor.h"

#include "../Browser/BrowserWindow.h"
#include "CellRenderer/OutlineCellRenderer.h"
#include "OutlineTreeModel.h"

#include <Titania/String/Trim.h>

namespace titania {
namespace puck {

const std::string OutlineTreeViewEditor::dragDataType = "titania/outline-tree/row";

OutlineTreeViewEditor::OutlineTreeViewEditor (BrowserWindow* const browserWindow) :
	  Glib::ObjectBase (typeid (OutlineTreeViewEditor)),
	  X3DBaseInterface (browserWindow),
	X3DOutlineTreeView (),
	         selection (browserWindow, this)
{
	set_name ("OutlineTreeViewEditor");

	get_cellrenderer () -> signal_edited () .connect (sigc::mem_fun (this, &OutlineTreeViewEditor::on_edited));

	// Drag targets
	std::vector <Gtk::TargetEntry> source_targets = {
		Gtk::TargetEntry (get_drag_data_type (), Gtk::TARGET_SAME_WIDGET)
	};

	drag_source_set (source_targets, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	// Drop targets
	std::vector <Gtk::TargetEntry> dest_targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list"),
		Gtk::TargetEntry (get_drag_data_type (), Gtk::TARGET_SAME_WIDGET)
	};

	drag_dest_set (dest_targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	set_reorderable (true);
}

void
OutlineTreeViewEditor::on_drag_begin (const Glib::RefPtr <Gdk::DragContext> & context)
{
	getBrowser () -> endUpdate ();
}

void
OutlineTreeViewEditor::on_drag_end (const Glib::RefPtr <Gdk::DragContext> & context)
{
	getBrowser () -> beginUpdate ();
}

void
OutlineTreeViewEditor::on_drag_data_delete (const Glib::RefPtr <Gdk::DragContext> & context)
{
	__LOG__ << std::endl;
}

void
OutlineTreeViewEditor::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		if (selection_data .get_data_type () == get_drag_data_type ())
		{
			TreeModel::Path      path;
			TreeViewDropPosition pos;

			if (get_dest_row_at_pos (x, y, path, pos))
			{
				__LOG__ << path .to_string () << std::endl;
				__LOG__ << selection_data .get_data_as_string () << std::endl;

				if (context -> get_suggested_action () == Gdk::ACTION_MOVE)
				{
					__LOG__ << "move" << std::endl;
					context -> drag_finish (true, true, time);
				}
				else
				{
					__LOG__ << "link" << std::endl;
					context -> drag_finish (true, false, time);
				}
			}
		}
	}

	X3DOutlineTreeView::on_drag_data_received (context, x, y, selection_data, info, time);
	return;

//	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
//	{
//		if (selection_data .get_data_type () == DND_OUTLINE_TREE_ROW)
//		{
//			__LOG__ << selection_data .get_data_as_string () << std::endl;
//
//			context -> drag_finish (true, false, time);
//			return;
//		}
//
//		if (selection_data .get_data_type () == "text/uri-list")
//		{
//			auto uri = selection_data .get_uris ();
//			
//			if (uri .size ())
//			{
//				getBrowserWindow () -> import (Glib::uri_unescape_string (uri [0]));
//
//				context -> drag_finish (true, false, time);
//				return;
//			}
//		}
//
//		if (selection_data .get_data_type () == "text/plain" or selection_data .get_data_type () == "STRING")
//		{
//			getBrowserWindow () -> import (Glib::uri_unescape_string (basic::trim (selection_data .get_data_as_string ())));
//
//			context -> drag_finish (true, false, time);
//			return;
//		}
//	}
//
//	context -> drag_finish (false, false, time);
}

void
OutlineTreeViewEditor::on_rename_node_activate ()
{
	//__LOG__ << std::endl;

	//
	//	Gtk::TreeModel::Path path;
	//	Gtk::TreeViewColumn* column = nullptr;
	//	int                  cell_x = 0;
	//	int                  cell_y = 0;
	//
	//	get_path_at_pos (x, y, path, column, cell_x, cell_y);
	//
	//	if (path .size ())
	//	{
	//		Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);
	//
	//		if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	//		{
	//			set_cursor (path, *column, true);
	//			return;
	//		}
	//	}
}

void
OutlineTreeViewEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select_node (get_model () -> get_iter (path), path);

	set_cursor (path, *column, true);
}

bool
OutlineTreeViewEditor::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 1:
		{
			if (select_field (event -> x, event -> y))
				return true;

			break;
		}
		case 3:
		{
			//			getPopupMenu () .popup (event -> button, event -> time);
			//			return true;
			break;
		}
		default:
			break;
	}

	return Gtk::TreeView::on_button_press_event (event);
}

void
OutlineTreeViewEditor::select_node (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	{
		selection .set_select_multiple (get_shift_key ());

		selection .select (*static_cast <X3D::SFNode*> (get_object (iter)));
	}
}

bool
OutlineTreeViewEditor::select_field (int x, int y)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	get_path_at_pos (x, y, path, column, cell_x, cell_y);

	if (path .size ())
	{
		Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

		if (get_data_type (iter) == OutlineIterType::X3DFieldValue)
		{
			getBrowserWindow () -> setEnableMenus (false);
			unwatch_tree (iter);
			set_cursor (path, *column, true);
			return true;
		}
	}

	return false;
}

void
OutlineTreeViewEditor::on_edited (const Glib::ustring & string_path, const Glib::ustring & text)
{
	Gtk::TreeModel::Path     path (string_path);
	Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

	get_model () -> row_changed (path, iter);
	watch (iter, path);

	getBrowserWindow () -> setEdited (true);
	getBrowserWindow () -> setEnableMenus (true);
}

} // puck
} // titania
