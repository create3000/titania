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

#ifndef __TITANIA_COMPOSED_WIDGETS_MFSTRING_URLWIDGET_H__
#define __TITANIA_COMPOSED_WIDGETS_MFSTRING_URLWIDGET_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

#include "../Base/X3DUserInterface.h"
#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"

namespace titania {
namespace puck {

class MFStringWidget :
	public X3DComposedWidget
{
public:

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~MFStringWidget ()
	{ dispose (); }


protected:

	///  @name Construction

	MFStringWidget (Gtk::TreeView &,
	                const Glib::RefPtr <Gtk::CellRendererText> &,
	                Gtk::Button &,
	                Gtk::Button &,
	                const std::string &,
	                const Glib::ustring &);

	///  @name Member access

	Gtk::TreeView &
	getTreeView ()
	{ return treeView; }

	const Gtk::TreeView &
	getTreeView () const
	{ return treeView; }

	void
	set1Value (const size_t, const Glib::ustring &);

	void
	set1Value (const Gtk::TreePath &, const Glib::ustring &);
	
	const Glib::ustring &
	get1Value (const size_t index)
	{ return string .get1Value (index); }

	///  @name Operations

	void
	append (const Glib::ustring &);

	///  @name Event handler

	virtual
	void
	on_add_clicked ();

	virtual
	void
	set_buffer ();


private:

	class Columns :
		public Gtk::TreeModel::ColumnRecord
	{
	public:

		Columns ()
		{
			add (string);
		}

		Gtk::TreeModelColumn <Glib::ustring> string;

	};

	///  @name Event handler

	void
	on_selection_changed ();

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &,
	                       int, int,
	                       const Gtk::SelectionData &,
	                       guint,
	                       guint);

	void
	on_edited (const Glib::ustring &, const Glib::ustring &);

	void
	on_remove_clicked ();

	void
	on_string_changed ();

	void
	set_field ();

	void
	connect (const X3D::MFString &);

	///  @name Members

	Gtk::TreeView &                            treeView;
	Columns                                    columns;
	Glib::RefPtr <Gtk::ListStore>              listStore;
	const Glib::RefPtr <Gtk::CellRendererText> cellRenderer;
	Gtk::Button &                              addButton;
	Gtk::Button &                              removeButton;
	X3D::MFNode                                nodes;
	const std::string                          name;
	const Glib::ustring                        defaultValue;
	X3D::MFString                              string;
	UndoStepPtr                                undoStep;
	X3D::SFTime                                buffer;

};

inline
MFStringWidget::MFStringWidget (Gtk::TreeView & treeView,
                                const Glib::RefPtr <Gtk::CellRendererText> & cellRenderer,
                                Gtk::Button & addButton,
                                Gtk::Button & removeButton,
                                const std::string & name,
                                const Glib::ustring & defaultValue) :
	X3DComposedWidget (),
	       treeView (treeView),
	        columns (),
	      listStore (Gtk::ListStore::create (columns)),
	   cellRenderer (cellRenderer),
	      addButton (addButton),
	   removeButton (removeButton),
	          nodes (),
	           name (name),
	   defaultValue (defaultValue),
	         string (),
	       undoStep (),
	         buffer ()
{
	addChildren (buffer);
	buffer .addInterest (this, &MFStringWidget::set_buffer);

	treeView .signal_drag_data_received ()          .connect (sigc::mem_fun (*this, &MFStringWidget::on_drag_data_received));
	treeView .get_selection () -> signal_changed () .connect (sigc::mem_fun (*this, &MFStringWidget::on_selection_changed));
	cellRenderer -> signal_edited ()                .connect (sigc::mem_fun (*this, &MFStringWidget::on_edited));
	addButton .signal_clicked ()                    .connect (sigc::mem_fun (*this, &MFStringWidget::on_add_clicked));
	removeButton .signal_clicked ()                 .connect (sigc::mem_fun (*this, &MFStringWidget::on_remove_clicked));

	treeView .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	treeView .enable_model_drag_dest ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_MOVE);
	treeView .set_model (listStore);

	setup ();
}

inline
void
MFStringWidget::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .removeInterest (this, &MFStringWidget::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .addInterest (this, &MFStringWidget::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
MFStringWidget::set1Value (const size_t index, const Glib::ustring & text)
{
	Gtk::TreePath path;

	path .push_back (index);

	set1Value (path, text);
}

inline
void
MFStringWidget::set1Value (const Gtk::TreePath & path, const Glib::ustring & text)
{
	const auto & value = text .empty () ? defaultValue : text;

	if (string .get1Value (path .front ()) == value)
		return;

	string .set1Value (path .front (), value);

	// Update list store.

	const auto iter = listStore -> get_iter (path);
	(*iter) [columns .string] = value;

	on_string_changed ();
}

inline
void
MFStringWidget::append (const Glib::ustring & value)
{
	const auto iter = listStore -> append ();

	(*iter) [columns .string] = value;

	string .emplace_back (value);

	on_string_changed ();
}

inline
void
MFStringWidget::on_selection_changed ()
{
	removeButton .set_sensitive (not treeView .get_selection () -> get_selected_rows () .empty ());
}

inline
void
MFStringWidget::on_edited (const Glib::ustring & pathString, const Glib::ustring & text)
{
	const auto path = Gtk::TreePath (pathString);

	set1Value (Gtk::TreePath (pathString), text);
}

inline
void
MFStringWidget::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
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

	const auto value = string [index];

	if (index < dest)
	{
		string .insert (string .begin () + dest, value);
		string .erase (string .begin () + index);
	}
	else
	{
		string .erase (string .begin () + index);
		string .insert (string .begin () + dest, value);
	}

	on_string_changed ();

	context -> drag_finish (false, false, time);
}

inline
void
MFStringWidget::on_add_clicked ()
{
	append (defaultValue);
}

inline
void
MFStringWidget::on_remove_clicked ()
{
	const auto   selected = treeView .get_selection () -> get_selected ();
	const size_t index    = listStore -> get_path (selected) .front ();

	// Update list store.

	listStore -> erase (selected);

	// Remove value.

	string .erase (string .begin () + index);

	on_string_changed ();
}

inline
void
MFStringWidget::on_string_changed ()
{
	// Change node field

	undoStep .reset ();

	addUndoFunction <X3D::MFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFString> (name);

			field .removeInterest (this, &MFStringWidget::set_field);
			field .addInterest (this, &MFStringWidget::connect);

			field = string;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFString> (nodes, name, undoStep);
}

inline
void
MFStringWidget::set_field ()
{
	buffer .addEvent ();
}

inline
void
MFStringWidget::set_buffer ()
{
	const auto pair = getArray <X3D::MFString> (nodes, name);

	string = pair .first;

	listStore -> clear ();

	if (pair .second > -2)
	{
		for (const auto & value : pair .first)
		{
			const auto iter = listStore -> append ();
			(*iter) [columns .string] = value .getValue ();
		}
	}

	treeView     .set_sensitive (pair .second not_eq -2);
	addButton    .set_sensitive (pair .second not_eq -2);
	removeButton .set_sensitive (false);
}

inline
void
MFStringWidget::connect (const X3D::MFString & field)
{
	field .removeInterest (this, &MFStringWidget::connect);
	field .addInterest (this, &MFStringWidget::set_field);
}

class MFStringURLWidget :
	public MFStringWidget
{
public:

	///  @name Construction

	MFStringURLWidget (X3DUserInterface* const,
	                   Gtk::TreeView &,
	                   const Glib::RefPtr <Gtk::CellRendererText> &,
	                   Gtk::Button &,
	                   Gtk::Button &,
                      const Glib::RefPtr <Gtk::TreeViewColumn> &,
 	                   const std::string &);


private:

	///  @name Event handlers

	virtual
	void
	on_add_clicked () final override;

	bool
	on_button_release_event (GdkEventButton*);
	
	virtual
	void
	set_buffer () final override;

	///  @name Members

	X3DUserInterface* const                  userInterface;
	const Glib::RefPtr <Gtk::TreeViewColumn> URLChooserColumn;
	std::unique_ptr <FileOpenDialog>         fileOpenDialog;
};

inline
MFStringURLWidget::MFStringURLWidget (X3DUserInterface* const userInterface,
                                      Gtk::TreeView & treeView,
                                      const Glib::RefPtr <Gtk::CellRendererText> & cellRenderer,
                                      Gtk::Button & addButton,
                                      Gtk::Button & removeButton,
                                      const Glib::RefPtr <Gtk::TreeViewColumn> & URLChooserColumn,
                                      const std::string & name) :
	X3DBaseInterface (userInterface -> getBrowserWindow (), userInterface -> getBrowser ()),
	  MFStringWidget (treeView, cellRenderer, addButton, removeButton, name, ""),
	   userInterface (userInterface),
	URLChooserColumn (URLChooserColumn),
	  fileOpenDialog ()
{
	treeView .signal_button_release_event () .connect (sigc::mem_fun (*this, &MFStringURLWidget::on_button_release_event));
}

inline
void
MFStringURLWidget::on_add_clicked ()
{
	fileOpenDialog .reset (new FileOpenDialog (getBrowserWindow ()));

	fileOpenDialog -> getWindow () .set_transient_for (userInterface -> getWindow ());
	fileOpenDialog -> getWindow () .set_modal (true);
	fileOpenDialog -> getRelativePathBox () .set_visible (true);

	if (fileOpenDialog -> run ())
	{
		auto URL = fileOpenDialog -> getURL ();

		if (fileOpenDialog -> getRelativePathSwitch () .get_active ())
			URL = getExecutionContext () -> getWorldURL () .relative_path (URL);

		append (URL .str ());
	}

	fileOpenDialog -> quit ();
	fileOpenDialog .reset ();
}

inline
bool
MFStringURLWidget::on_button_release_event (GdkEventButton* event)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (not path .size ())
		return false;

	if (column not_eq URLChooserColumn .operator -> ())
		return false;

	// Choose new URL
	
	fileOpenDialog .reset (new FileOpenDialog (getBrowserWindow ()));

	fileOpenDialog -> getWindow () .set_transient_for (userInterface -> getWindow ());
	fileOpenDialog -> getWindow () .set_modal (true);
	fileOpenDialog -> getRelativePathBox () .set_visible (true);

	// Set URL

	basic::uri URL = getExecutionContext () -> getWorldURL () .transform (get1Value (path .front ()) .raw ());
	
	fileOpenDialog -> setURL (URL);

	// Run

	if (fileOpenDialog -> run ())
	{
		URL = fileOpenDialog -> getURL ();

		if (fileOpenDialog -> getRelativePathSwitch () .get_active ())
			URL = getExecutionContext () -> getWorldURL () .relative_path (URL);

		set1Value (path .front (), URL .str ());
	}

	fileOpenDialog -> quit ();
	fileOpenDialog .reset ();
	return true;
}

inline
void
MFStringURLWidget::set_buffer ()
{
	if (fileOpenDialog)
		fileOpenDialog -> quit ();

	MFStringWidget::set_buffer ();
}

} // puck
} // titania

#endif
