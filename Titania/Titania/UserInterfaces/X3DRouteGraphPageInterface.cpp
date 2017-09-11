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

#include "X3DRouteGraphPageInterface.h"

namespace titania {
namespace puck {

void
X3DRouteGraphPageInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DRouteGraphPageInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DRouteGraphPageInterface::create ()
{
	// Get objects.
	m_HAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("HAdjustment"));
	m_VAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Overlay", m_Overlay);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("Viewport", m_Viewport);
	m_builder -> get_widget ("Fixed", m_Fixed);
	m_builder -> get_widget ("PageNameLabel", m_PageNameLabel);
	m_builder -> get_widget ("RenamePagePopover", m_RenamePagePopover);
	m_builder -> get_widget ("PageNameBox", m_PageNameBox);
	m_builder -> get_widget ("PageNameEntry", m_PageNameEntry);
	m_builder -> get_widget ("RenamePageButton", m_RenamePageButton);

	// Connect object Gtk::Viewport with id 'Viewport'.
	m_Viewport -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_button_press_event));
	m_Viewport -> signal_button_release_event () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_button_release_event));
	m_Viewport -> signal_drag_data_received () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_drag_data_received));
	m_Viewport -> signal_motion_notify_event () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_motion_notify_event));

	// Connect object Gtk::Fixed with id 'Fixed'.
	m_Fixed -> signal_draw () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_draw), false);

	// Connect object Gtk::Entry with id 'PageNameEntry'.
	m_PageNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_page_name_changed));
	m_PageNameEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_page_name_key_press_event), false);

	// Connect object Gtk::Button with id 'RenamePageButton'.
	m_RenamePageButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRouteGraphPageInterface::on_page_name_rename_clicked));
}

X3DRouteGraphPageInterface::~X3DRouteGraphPageInterface ()
{
	delete m_Window;
}

} // puck
} // titania
