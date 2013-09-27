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
#include "X3DLibraryViewInterface.h"

namespace titania {
namespace puck {

const std::string X3DLibraryViewInterface::m_widgetName = "LibraryView";

void
X3DLibraryViewInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_treeStore    = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("TreeStore"));
	m_file         = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("File"));
	m_iconRenderer = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconRenderer"));
	m_nameRenderer = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("ScrolledWindow", m_scrolledWindow);
	m_scrolledWindow -> set_name ("ScrolledWindow");
	m_builder -> get_widget ("TreeView", m_treeView);
	m_treeView -> set_name ("TreeView");

	// Connect object Gtk::Box with id 'Widget'.
	connections .emplace_back (m_widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DLibraryViewInterface::on_map)));

	// Connect object Gtk::TreeView with id 'TreeView'.
	connections .emplace_back (m_treeView -> signal_row_activated () .connect (sigc::mem_fun (*this, &X3DLibraryViewInterface::on_row_activated)));

	// Call construct handler of base class.
	construct ();
}

} // puck
} // titania
