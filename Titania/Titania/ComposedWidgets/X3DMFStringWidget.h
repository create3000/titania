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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DMFSTRING_WIDGET_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DMFSTRING_WIDGET_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class X3DMFStringWidget :
	public X3DComposedWidget
{
public:

	///  @name Member access

	const std::string &
	getName ()
	{ return name; }

	void
	setDefaultValue (const std::string &, const std::string &);

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~X3DMFStringWidget () override;


protected:

	///  @name Construction

	X3DMFStringWidget (X3DBaseInterface* const editor,
	                   Gtk::TreeView &,
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

	///  @name Event handlers

	virtual
	void
	on_add_clicked ();

	virtual
	void
	on_remove_clicked ();

	virtual
	void
	set_buffer ();

	virtual
	void
	set_string (const X3D::MFString &)
	{ }


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

	///  @name Member access

	bool
	getAccelerators () const;

	///  @name Event handlers

	bool
	on_focus_in_event (GdkEventFocus* event);

	bool
	on_focus_out_event (GdkEventFocus* event);

	bool
	on_key_press_event (GdkEventKey* event);

	void
	on_selection_changed ();

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &,
	                       int, int,
	                       const Gtk::SelectionData &,
	                       guint,
	                       guint);


	void
	on_editing_started (Gtk::CellEditable* const, const Glib::ustring &);
	
	void
	on_edited (const Glib::ustring &, const Glib::ustring &);

	void
	on_editing_canceled ();

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
	Glib::ustring                              displayValue;
	Glib::ustring                              defaultValue;
	X3D::MFString                              string;
	X3D::UndoStepPtr                           undoStep;
	X3D::SFTime                                buffer;
	bool                                       focus;
	bool                                       editing;

};

} // puck
} // titania

#endif
