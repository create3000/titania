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
#ifndef __TMP_GLAD2CPP_LAYER_EDITOR_H__
#define __TMP_GLAD2CPP_LAYER_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DLayerEditorInterface :
	public X3DEditorInterface
{
public:

	X3DLayerEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLayerEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::ListStore> &
	getLayerListStore () const
	{ return m_LayerListStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getLayerSelection () const
	{ return m_LayerSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getVisibilityColumn () const
	{ return m_VisibilityColumn; }

	const Glib::RefPtr <Gtk::CellRendererToggle> &
	getVisibilityCellRenderer () const
	{ return m_VisibilityCellRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTypeNameColumn () const
	{ return m_TypeNameColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getNameColumn () const
	{ return m_NameColumn; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Expander &
	getLayerSetExpander () const
	{ return *m_LayerSetExpander; }

	Gtk::ScrolledWindow &
	getLayerScrolledWindow () const
	{ return *m_LayerScrolledWindow; }

	Gtk::TreeView &
	getLayerTreeView () const
	{ return *m_LayerTreeView; }

	Gtk::Box &
	getMoveLayerBox () const
	{ return *m_MoveLayerBox; }

	Gtk::Button &
	getTopButton () const
	{ return *m_TopButton; }

	Gtk::Button &
	getUpButton () const
	{ return *m_UpButton; }

	Gtk::Button &
	getDownButton () const
	{ return *m_DownButton; }

	Gtk::Button &
	getBottomButton () const
	{ return *m_BottomButton; }

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_layer_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) = 0;

	virtual
	void
	on_layer_selection_changed () = 0;

	virtual
	void
	on_visibility_toggled (const Glib::ustring & path) = 0;

	virtual
	void
	on_top_clicked () = 0;

	virtual
	void
	on_up_clicked () = 0;

	virtual
	void
	on_down_clicked () = 0;

	virtual
	void
	on_bottom_clicked () = 0;

	virtual
	~X3DLayerEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::ListStore>          m_LayerListStore;
	Glib::RefPtr <Gtk::TreeSelection>      m_LayerSelection;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_VisibilityColumn;
	Glib::RefPtr <Gtk::CellRendererToggle> m_VisibilityCellRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_TypeNameColumn;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_NameColumn;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::Button*                           m_IndexButton;
	Gtk::Expander*                         m_LayerSetExpander;
	Gtk::ScrolledWindow*                   m_LayerScrolledWindow;
	Gtk::TreeView*                         m_LayerTreeView;
	Gtk::Box*                              m_MoveLayerBox;
	Gtk::Button*                           m_TopButton;
	Gtk::Button*                           m_UpButton;
	Gtk::Button*                           m_DownButton;
	Gtk::Button*                           m_BottomButton;

};

} // puck
} // titania

#endif
