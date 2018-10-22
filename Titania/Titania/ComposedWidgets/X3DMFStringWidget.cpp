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

#include "X3DMFStringWidget.h"

namespace titania {
namespace puck {

X3DMFStringWidget::X3DMFStringWidget (X3DBaseInterface* const editor,
                                      Gtk::TreeView & treeView,
                                      const Glib::RefPtr <Gtk::CellRendererText> & cellRenderer,
                                      Gtk::Button & addButton,
                                      Gtk::Button & removeButton,
                                      const std::string & name,
                                      const Glib::ustring & defaultValue) :
	X3DComposedWidget (editor),
	         treeView (treeView),
	          columns (),
	        listStore (Gtk::ListStore::create (columns)),
	     cellRenderer (cellRenderer),
	        addButton (addButton),
	     removeButton (removeButton),
	            nodes (),
	             name (name),
	     displayValue (defaultValue),
	     defaultValue (defaultValue),
	           string (),
	         undoStep (),
	           buffer (),
	            focus (false),
	          editing (false)
{
	addChildObjects (nodes, string, buffer);

	buffer .addInterest (&X3DMFStringWidget::set_buffer, this);

	treeView .set_reorderable (true);

	treeView .signal_focus_in_event ()              .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_focus_in_event));
	treeView .signal_focus_out_event ()             .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_focus_out_event));
	treeView .signal_key_press_event ()             .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_key_press_event), false);
	treeView .signal_drag_data_received ()          .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_drag_data_received));
	treeView .get_selection () -> signal_changed () .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_selection_changed));
	cellRenderer -> signal_editing_started ()       .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_editing_started));
	cellRenderer -> signal_edited ()                .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_edited));
	cellRenderer -> signal_editing_canceled ()      .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_editing_canceled));
	addButton .signal_clicked ()                    .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_add_clicked));
	removeButton .signal_clicked ()                 .connect (sigc::mem_fun (this, &X3DMFStringWidget::on_remove_clicked));

	treeView .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);
	treeView .enable_model_drag_dest   ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_COPY);
	treeView .set_model (listStore);
}

void
X3DMFStringWidget::setDefaultValue (const std::string & first, const std::string & second)
{
	displayValue = first;
	defaultValue = second;
}

void
X3DMFStringWidget::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .removeInterest (&X3DMFStringWidget::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .addInterest (&X3DMFStringWidget::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
X3DMFStringWidget::set1Value (const size_t index, const Glib::ustring & text)
{
	Gtk::TreePath path;

	path .push_back (index);

	set1Value (path, text);
}

void
X3DMFStringWidget::set1Value (const Gtk::TreePath & path, const Glib::ustring & text)
{
	const auto & value = text .empty () ? defaultValue : text;

	if (string .get1Value (path .front ()) == value)
		return;

	string .set1Value (path .front (), value == displayValue ? defaultValue : value);

	// Update list store.

	const auto iter = listStore -> get_iter (path);
	(*iter) [columns .string] = value;

	on_string_changed ();
}

void
X3DMFStringWidget::append (const Glib::ustring & value)
{
	const auto iter = listStore -> append ();

	(*iter) [columns .string] = value .length () ? value : displayValue;

	string .emplace_back (value == displayValue ? defaultValue : value);

	on_string_changed ();
}

bool
X3DMFStringWidget::getAccelerators () const
{
	return not (focus or editing);
}

bool
X3DMFStringWidget::on_focus_in_event (GdkEventFocus* event)
{
	focus = true;
	getBrowserWindow () -> setAccelerators (getAccelerators ());
	return false;
}

bool
X3DMFStringWidget::on_focus_out_event (GdkEventFocus* event)
{
	focus = false;
	getBrowserWindow () -> setAccelerators (getAccelerators ());
	return false;
}

bool
X3DMFStringWidget::on_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_BackSpace or event -> keyval == GDK_KEY_Delete)
	{
		if (treeView .get_selection () -> get_selected_rows () .empty ())
			return false;

		on_remove_clicked ();
		return true;
	}

	return false;
}

void
X3DMFStringWidget::on_selection_changed ()
{
	removeButton .set_sensitive (not treeView .get_selection () -> get_selected_rows () .empty ());
}

void
X3DMFStringWidget::on_editing_started (Gtk::CellEditable* const, const Glib::ustring &)
{
	editing = true;
	getBrowserWindow () -> setAccelerators (getAccelerators ());
}

void
X3DMFStringWidget::on_edited (const Glib::ustring & pathString, const Glib::ustring & text)
{
	const auto path = Gtk::TreePath (pathString);

	set1Value (Gtk::TreePath (pathString), text);

	editing = false;
	getBrowserWindow () -> setAccelerators (getAccelerators ());
}

void
X3DMFStringWidget::on_editing_canceled ()
{
	editing = false;
	getBrowserWindow () -> setAccelerators (getAccelerators ());
}

void
X3DMFStringWidget::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                          int x, int y,
                                          const Gtk::SelectionData & selection_data,
                                          guint info,
                                          guint time)
{
	const auto   selected = treeView .get_selection () -> get_selected ();
	const size_t index    = listStore -> get_path (selected) .front ();

	// Update list store.

	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;
	size_t                    dest = 0;

	if (treeView .get_dest_row_at_pos (x, y, destinationPath, position))
	{
		auto destination = listStore -> get_iter (destinationPath);

		dest = listStore -> get_path (destination) .front ();

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
				listStore -> move (selected, destination);
				break;
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				listStore -> move (selected, ++ destination);
				++ dest;
				break;
		}
	}
	else
	{
		const auto children = listStore -> children ();
		listStore -> move (selected, children .end ());
		dest = children .size ();
	}

	// Move value.

	// Insert

	const auto fromIter = string .begin () + index;
	const auto toIter   = string .begin () + dest;

	string .emplace (toIter, std::move (*fromIter));

	// Erase

	if (index < dest)
		string .erase (string .begin () + index);
	else
		string .erase (string .begin () + (index + 1));

	// Finish

	on_string_changed ();

	context -> drag_finish (true, false, time);
}

void
X3DMFStringWidget::on_add_clicked ()
{
	append (defaultValue);
}

void
X3DMFStringWidget::on_remove_clicked ()
{
	const auto   selected = treeView .get_selection () -> get_selected ();
	const size_t index    = listStore -> get_path (selected) .front ();

	// Update list store.

	listStore -> erase (selected);

	// Remove value.

	string .erase (string .begin () + index);

	on_string_changed ();
}

void
X3DMFStringWidget::on_string_changed ()
{
	if (nodes .empty ())
		return;

	// Change node field

	undoStep .reset ();

	addUndoFunction <X3D::MFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFString> (name);

			field .removeInterest (&X3DMFStringWidget::set_field, this);
			field .addInterest (&X3DMFStringWidget::connect, this);

			field = string;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFString> (nodes, name, undoStep);

	set_string (string);
}

void
X3DMFStringWidget::set_field ()
{
	buffer .addEvent ();
}

void
X3DMFStringWidget::set_buffer ()
{
	undoStep .reset ();

	const auto pair = getArray <X3D::MFString> (nodes, name);

	string = pair .first;

	listStore -> clear ();

	if (pair .second > -2)
	{
		for (const auto & value : basic::make_const_range (string))
		{
			const auto iter = listStore -> append ();
			(*iter) [columns .string] = value .length () ? value : displayValue;
		}
	}

	treeView  .set_sensitive (pair .second not_eq -2);
	addButton .set_sensitive (pair .second not_eq -2);

	set_string (string);
}

void
X3DMFStringWidget::connect (const X3D::MFString & field)
{
	field .removeInterest (&X3DMFStringWidget::connect, this);
	field .addInterest (&X3DMFStringWidget::set_field, this);
}

X3DMFStringWidget::~X3DMFStringWidget ()
{
	if (treeView .has_focus ())
		getCurrentBrowser () -> grab_focus ();

	dispose ();
}

} // puck
} // titania
