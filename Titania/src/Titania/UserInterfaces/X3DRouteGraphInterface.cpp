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

#include "X3DRouteGraphInterface.h"

namespace titania {
namespace puck {

void
X3DRouteGraphInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DRouteGraphInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DRouteGraphInterface::create ()
{
	// Get objects.
	m_HAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("HAdjustment"));
	m_VAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("MenuBar", m_MenuBar);
	m_builder -> get_widget ("RouteGraphMenuItem", m_RouteGraphMenuItem);
	m_builder -> get_widget ("NewPageMenuItem", m_NewPageMenuItem);
	m_builder -> get_widget ("RenamePageMenuItem", m_RenamePageMenuItem);
	m_builder -> get_widget ("AddConnectedNodesMenuItem", m_AddConnectedNodesMenuItem);
	m_builder -> get_widget ("AlignToGridMenuItem", m_AlignToGridMenuItem);
	m_builder -> get_widget ("ExportPageMenuItem", m_ExportPageMenuItem);
	m_builder -> get_widget ("ClosePageMenuItem", m_ClosePageMenuItem);
	m_builder -> get_widget ("EditMenuItem", m_EditMenuItem);
	m_builder -> get_widget ("DeleteMenuItem", m_DeleteMenuItem);
	m_builder -> get_widget ("SelectAllMenuItem", m_SelectAllMenuItem);
	m_builder -> get_widget ("DeselectAllMenuItem", m_DeselectAllMenuItem);
	m_builder -> get_widget ("EditToolBar", m_EditToolBar);
	m_builder -> get_widget ("NewLogicButton", m_NewLogicButton);
	m_builder -> get_widget ("RenamePageButton", m_RenamePageButton);
	m_builder -> get_widget ("DeleteButton", m_DeleteButton);
	m_builder -> get_widget ("ClosePageButton", m_ClosePageButton);
	m_builder -> get_widget ("Notebook", m_Notebook);

	// Connect object Gtk::ImageMenuItem with id 'NewPageMenuItem'.
	m_NewPageMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_new_page_activate));
	m_RenamePageMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_rename_page_activate));

	// Connect object Gtk::MenuItem with id 'AlignToGridMenuItem'.
	m_AlignToGridMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_align_to_grid_activate));

	// Connect object Gtk::ImageMenuItem with id 'ExportPageMenuItem'.
	m_ExportPageMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_export_page_activate));
	m_ClosePageMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_close_page_activate));
	m_DeleteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_delete_activate));
	m_SelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_select_all_activate));

	// Connect object Gtk::MenuItem with id 'DeselectAllMenuItem'.
	m_DeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_deselect_all_activate));

	// Connect object Gtk::ToolButton with id 'NewLogicButton'.
	m_NewLogicButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_new_page_clicked));
	m_RenamePageButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_rename_page_clicked));
	m_DeleteButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_delete_clicked));
	m_ClosePageButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_close_page_clicked));

	// Connect object Gtk::Notebook with id 'Notebook'.
	m_Notebook -> signal_page_reordered () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_page_reordered));
	m_Notebook -> signal_switch_page () .connect (sigc::mem_fun (this, &X3DRouteGraphInterface::on_switch_page));
}

X3DRouteGraphInterface::~X3DRouteGraphInterface ()
{
	delete m_Window;
}

} // puck
} // titania
