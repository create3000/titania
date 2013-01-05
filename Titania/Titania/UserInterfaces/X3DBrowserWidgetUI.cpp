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
 ******************************************************************************/

#include "X3DBrowserWidgetUI.h"

namespace titania {
namespace puck {

const std::string X3DBrowserWidgetUI::m_widgetName = "BrowserWidget";

void
X3DBrowserWidgetUI::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_iconFactory = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));

	// Get widgets.
	m_builder -> get_widget ("MessageDialog", m_messageDialog);
	m_builder -> get_widget ("Window", m_window);
	m_builder -> get_widget ("Widget", m_widget);
	m_builder -> get_widget ("NavigationBar", m_navigationBar);
	m_builder -> get_widget ("ForwardButton", m_forwardButton);
	m_builder -> get_widget ("BackButton", m_backButton);
	m_builder -> get_widget ("ReloadButton", m_reloadButton);
	m_builder -> get_widget ("HomeButton", m_homeButton);
	m_builder -> get_widget ("LocationEntry", m_locationEntry);
	m_builder -> get_widget ("ToolBar", m_toolBar);
	m_builder -> get_widget ("VPaned", m_vPaned);
	m_builder -> get_widget ("HPaned", m_hPaned);
	m_builder -> get_widget ("SurfaceBox", m_surfaceBox);
	m_builder -> get_widget ("ArrowButton", m_arrowButton);
	m_builder -> get_widget ("HandButton", m_handButton);
	m_builder -> get_widget ("LookAtAllButton", m_lookAtAllButton);
	m_builder -> get_widget ("LookAtButton", m_lookAtButton);
	m_builder -> get_widget ("Footer", m_footer);
	m_builder -> get_widget ("FooterCloseButton", m_footerCloseButton);
	m_builder -> get_widget ("FooterNotebook", m_footerNotebook);
	m_builder -> get_widget ("ConsoleBox", m_consoleBox);
	m_builder -> get_widget ("SideBar", m_sideBar);
	m_builder -> get_widget ("SideBarCloseButton", m_sideBarCloseButton);
	m_builder -> get_widget ("SideBarNotebook", m_sideBarNotebook);
	m_builder -> get_widget ("HistoryEditorBox", m_historyEditorBox);
	m_builder -> get_widget ("ViewpointEditorBox", m_viewpointEditorBox);
	m_builder -> get_widget ("OutlineEditorBox", m_outlineEditorBox);
	m_builder -> get_widget ("StatusBar", m_statusBar);

	// Connect object Gtk::MessageDialog with id 'MessageDialog'.
	connections .emplace_back (m_messageDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::messageDialogResponse)));

	// Connect object Gtk::VBox with id 'Widget'.
	connections .emplace_back (m_widget -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_unmap)));
	connections .emplace_back (m_widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_map)));

	// Connect object Gtk::ToolButton with id 'ReloadButton'.
	connections .emplace_back (m_reloadButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_reload)));
	connections .emplace_back (m_homeButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_home)));

	// Connect object Gtk::Entry with id 'LocationEntry'.
	connections .emplace_back (m_locationEntry -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_locationEntry_activate)));
	connections .emplace_back (m_locationEntry -> signal_icon_release () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_locationEntryIcon_activate)));

	// Connect object Gtk::RadioToolButton with id 'ArrowButton'.
	connections .emplace_back (m_arrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_arrow_button_toggled)));
	connections .emplace_back (m_handButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_hand_button_toggled)));

	// Connect object Gtk::ToolButton with id 'LookAtAllButton'.
	connections .emplace_back (m_lookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_look_at_all_clicked)));

	// Connect object Gtk::ToggleToolButton with id 'LookAtButton'.
	connections .emplace_back (m_lookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_look_at_toggled)));

	// Call construct handler of base class.
	construct ();
}

void
X3DBrowserWidgetUI::dispose ()
{
	for (auto & connection : connections)
		connection .disconnect ();
}

} // puck
} // titania
