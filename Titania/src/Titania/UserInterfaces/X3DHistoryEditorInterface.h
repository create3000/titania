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

#ifndef __TMP_GLAD2CPP_HISTORY_EDITOR_H__
#define __TMP_GLAD2CPP_HISTORY_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for HistoryEditor.
 */
class X3DHistoryEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DHistoryEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DHistoryEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DHistoryEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::ListStore> &
	getListStore () const
	{ return m_ListStore; }

	const Glib::RefPtr <Gtk::TreeModelSort> &
	getTreeModelSort () const
	{ return m_TreeModelSort; }

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
	getWorlURLColumn () const
	{ return m_WorlURLColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getWorlURLRenderer () const
	{ return m_WorlURLRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getLastAccessColumn () const
	{ return m_LastAccessColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getLastAccessRenderer () const
	{ return m_LastAccessRenderer; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::SearchEntry &
	getSearchEntry () const
	{ return *m_SearchEntry; }

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

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_search_changed () = 0;

	virtual
	bool
	on_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_column_clicked () = 0;

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

	///  @name Destruction

	virtual
	~X3DHistoryEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::ListStore> m_ListStore;
	Glib::RefPtr <Gtk::TreeModelSort> m_TreeModelSort;
	Glib::RefPtr <Gtk::TreeViewColumn> m_Icon;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_TitleColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_TitleRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_WorlURLColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_WorlURLRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_LastAccessColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_LastAccessRenderer;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::SearchEntry* m_SearchEntry;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::TreeView* m_TreeView;
	Gtk::Menu* m_Menu;
	Gtk::RadioMenuItem* m_NeverMenuItem;
	Gtk::RadioMenuItem* m_OneMonthMenuItem;
	Gtk::RadioMenuItem* m_HalfYearMenuItem;
	Gtk::RadioMenuItem* m_OneYearMenuItem;
	Gtk::RadioMenuItem* m_AlwaysMenuItem;

};

} // puck
} // titania

#endif
