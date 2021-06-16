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

#include "X3DBindableNodeListInterface.h"

namespace titania {
namespace puck {

void
X3DBindableNodeListInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DBindableNodeListInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DBindableNodeListInterface::create ()
{
	// Get objects.
	m_ListStore               = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ListStore"));
	m_TypeNameColumn          = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TypeNameColumn"));
	m_TypeNameCellRenderer    = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("TypeNameCellRenderer"));
	m_NameColumn              = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("NameColumn"));
	m_NameCellRenderer        = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameCellRenderer"));
	m_DescriptionColumn       = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("DescriptionColumn"));
	m_DescriptionCellRenderer = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("DescriptionCellRenderer"));
	m_PadColumn               = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("PadColumn"));
	m_BindColumn              = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("BindColumn"));
	m_BindCellRenderer        = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("BindCellRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderNotebook", m_HeaderNotebook);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("Image", m_Image);
	m_builder -> get_widget ("Label", m_Label);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("TreeView", m_TreeView);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DBindableNodeListInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DBindableNodeListInterface::on_unmap));

	// Connect object Gtk::TreeView with id 'TreeView'.
	m_TreeView -> signal_button_release_event () .connect (sigc::mem_fun (this, &X3DBindableNodeListInterface::on_button_release_event));
	m_TreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DBindableNodeListInterface::on_row_activated));
}

X3DBindableNodeListInterface::~X3DBindableNodeListInterface ()
{
	delete m_Window;
}

} // puck
} // titania
