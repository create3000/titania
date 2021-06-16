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

#include "X3DHistoryEditorInterface.h"

namespace titania {
namespace puck {

void
X3DHistoryEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DHistoryEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DHistoryEditorInterface::create ()
{
	// Get objects.
	m_ListStore          = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ListStore"));
	m_TreeModelSort      = Glib::RefPtr <Gtk::TreeModelSort>::cast_dynamic (m_builder -> get_object ("TreeModelSort"));
	m_Icon               = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("Icon"));
	m_IconRenderer       = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconRenderer"));
	m_TitleColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TitleColumn"));
	m_TitleRenderer      = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("TitleRenderer"));
	m_WorlURLColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("WorlURLColumn"));
	m_WorlURLRenderer    = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("WorlURLRenderer"));
	m_LastAccessColumn   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("LastAccessColumn"));
	m_LastAccessRenderer = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("LastAccessRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("SearchEntry", m_SearchEntry);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("TreeView", m_TreeView);
	m_builder -> get_widget ("Menu", m_Menu);
	m_builder -> get_widget ("NeverMenuItem", m_NeverMenuItem);
	m_builder -> get_widget ("OneMonthMenuItem", m_OneMonthMenuItem);
	m_builder -> get_widget ("HalfYearMenuItem", m_HalfYearMenuItem);
	m_builder -> get_widget ("OneYearMenuItem", m_OneYearMenuItem);
	m_builder -> get_widget ("AlwaysMenuItem", m_AlwaysMenuItem);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_unmap));

	// Connect object Gtk::SearchEntry with id 'SearchEntry'.
	m_SearchEntry -> signal_search_changed () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_search_changed));

	// Connect object Gtk::TreeView with id 'TreeView'.
	m_TreeView -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_button_press_event), false);
	m_TreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_row_activated));

	// Connect object Gtk::TreeViewColumn with id 'Icon'.
	m_Icon -> signal_clicked () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_column_clicked));
	m_TitleColumn -> signal_clicked () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_column_clicked));
	m_WorlURLColumn -> signal_clicked () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_column_clicked));
	m_LastAccessColumn -> signal_clicked () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_column_clicked));

	// Connect object Gtk::RadioMenuItem with id 'NeverMenuItem'.
	m_NeverMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_never_toggled));
	m_OneMonthMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_on_month_toggled));
	m_HalfYearMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_half_year_toggled));
	m_OneYearMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_one_year_toggled));
	m_AlwaysMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DHistoryEditorInterface::on_always_toggled));
}

X3DHistoryEditorInterface::~X3DHistoryEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
