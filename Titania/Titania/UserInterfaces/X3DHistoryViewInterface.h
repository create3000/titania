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
#ifndef __TMP_GLAD2CPP_HISTORY_VIEW_H__
#define __TMP_GLAD2CPP_HISTORY_VIEW_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DHistoryViewInterface :
	public X3DUserInterface
{
public:

	X3DHistoryViewInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DHistoryViewInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
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
	getListStore () const
	{ return m_ListStore; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getIcon () const
	{ return m_Icon; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getIconRenderer () const
	{ return m_IconRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTitleColumn () const
	{ return m_TitleColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getTitleRenderer () const
	{ return m_TitleRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getLocationColumn () const
	{ return m_LocationColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getLocationRenderer () const
	{ return m_LocationRenderer; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	Gtk::Menu &
	getMenu () const
	{ return *m_Menu; }

	Gtk::RadioMenuItem &
	getNeverMenuItem () const
	{ return *m_NeverMenuItem; }

	Gtk::RadioMenuItem &
	getOneMonthMenuItem () const
	{ return *m_OneMonthMenuItem; }

	Gtk::RadioMenuItem &
	getHalfYearMenuItem () const
	{ return *m_HalfYearMenuItem; }

	Gtk::RadioMenuItem &
	getOneYearMenuItem () const
	{ return *m_OneYearMenuItem; }

	Gtk::RadioMenuItem &
	getAlwaysMenuItem () const
	{ return *m_AlwaysMenuItem; }

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	bool
	on_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) = 0;

	virtual
	void
	on_never_toggled () = 0;

	virtual
	void
	on_on_month_toggled () = 0;

	virtual
	void
	on_half_year_toggled () = 0;

	virtual
	void
	on_one_year_toggled () = 0;

	virtual
	void
	on_always_toggled () = 0;

	virtual
	~X3DHistoryViewInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	std::deque <sigc::connection>          m_connections;
	Glib::RefPtr <Gtk::ListStore>          m_ListStore;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_Icon;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_TitleColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_TitleRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_LocationColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_LocationRenderer;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::ScrolledWindow*                   m_ScrolledWindow;
	Gtk::TreeView*                         m_TreeView;
	Gtk::Menu*                             m_Menu;
	Gtk::RadioMenuItem*                    m_NeverMenuItem;
	Gtk::RadioMenuItem*                    m_OneMonthMenuItem;
	Gtk::RadioMenuItem*                    m_HalfYearMenuItem;
	Gtk::RadioMenuItem*                    m_OneYearMenuItem;
	Gtk::RadioMenuItem*                    m_AlwaysMenuItem;

};

} // puck
} // titania

#endif
