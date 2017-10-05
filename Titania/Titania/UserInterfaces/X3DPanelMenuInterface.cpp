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

#include "X3DPanelMenuInterface.h"

namespace titania {
namespace puck {

void
X3DPanelMenuInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DPanelMenuInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DPanelMenuInterface::create ()
{
	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("BrowserPanelMenuItem", m_BrowserPanelMenuItem);
	m_builder -> get_widget ("RenderPanelMenuItem", m_RenderPanelMenuItem);
	m_builder -> get_widget ("MultiViewMenuItem", m_MultiViewMenuItem);
	m_builder -> get_widget ("Window", m_Window);

	// Connect object Gtk::MenuItem with id 'BrowserPanelMenuItem'.
	m_BrowserPanelMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DPanelMenuInterface::on_browser_panel_activate));
	m_RenderPanelMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DPanelMenuInterface::on_render_panel_activate));

	// Connect object Gtk::CheckMenuItem with id 'MultiViewMenuItem'.
	m_MultiViewMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPanelMenuInterface::on_multi_view_toggled));
}

X3DPanelMenuInterface::~X3DPanelMenuInterface ()
{
	delete m_Window;
}

} // puck
} // titania
