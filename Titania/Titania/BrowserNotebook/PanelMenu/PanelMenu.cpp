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

#include "PanelMenu.h"

#include "../NotebookPage/NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

PanelMenu::PanelMenu (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const PanelType panelType_) :
	     X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DPanelMenuInterface (get_ui ("Widgets/PanelMenu.glade")),
	                 page (page),
	            panelType (panelType_),
	            menuItems ({ std::make_pair (PanelType::BROWSER_PANEL, &getBrowserPanelMenuItem ()),
	                         std::make_pair (PanelType::RENDER_PANEL,  &getRenderPanelMenuItem ()) }),
	             changing (false)
{
	addChildObjects (panelType);

	getWidget () .set_accel_group (getBrowserWindow () -> getAccelGroup ());

	Gtk::AccelMap::add_entry ("<PanelMenu>/Multi View", GDK_KEY_space, Gdk::ModifierType (0));

	menuItems .at (panelType) -> get_style_context () -> add_class ("titania-menu-item-selected");

	setup ();
}

void
PanelMenu::initialize ()
{
	X3DPanelMenuInterface::initialize ();

	page -> getMultiView () .addInterest (this, &PanelMenu::set_multi_view);

	set_multi_view ();
}

void
PanelMenu::on_browser_panel_activate ()
{
	panelType = PanelType::BROWSER_PANEL;
}

void
PanelMenu::on_render_panel_activate ()
{
	panelType = PanelType::RENDER_PANEL;
}

void
PanelMenu::set_multi_view ()
{
	changing = true;

	getMultiViewMenuItem () .set_active (page -> getMultiView ());

	changing = false;
}

void
PanelMenu::on_multi_view_toggled ()
{
	if (changing)
		return;

	page -> setMultiView (getMultiViewMenuItem () .get_active ());
}

PanelMenu::~PanelMenu ()
{
	dispose ();
}

} // puck
} // titania
 