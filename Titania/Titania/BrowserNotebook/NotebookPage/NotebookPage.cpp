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

#include "NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include "../BrowserPanel/BrowserPanel.h"

namespace titania {
namespace puck {

NotebookPage::NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	         X3DNotebookPage (startUrl),
	                   boxes ({ &getBox1 (), &getBox2 (), &getBox3 (), &getBox4 () }),
	                  panel1 (),
	                  panel2 (),
	                  panel3 (),
	                  panel4 (),
	              activeView (1),
	               multiView (false)
{
	unparent (getWidget ());

	setup ();
}

void
NotebookPage::initialize ()
{
	X3DNotebookPage::initialize ();
}

void
NotebookPage::loaded ()
{
	setPanel (0, panel1, PanelType::BROWSER_PANEL, getBox1 ());
	setPanel (1, panel2, PanelType::BROWSER_PANEL, getBox2 ());
	setPanel (2, panel3, PanelType::BROWSER_PANEL, getBox3 ());
	setPanel (3, panel4, PanelType::BROWSER_PANEL, getBox4 ());
}

void
NotebookPage::initialized ()
{
	X3DNotebookPage::initialized ();

	if (getBrowserWindow () -> getEditing ())
	{
		const auto worldInfo = createWorldInfo (getScene ());

		setActiveView (math::clamp (worldInfo -> getMetaData <int32_t> ("/Titania/Page/activeView", 1), 0, 4));
		setMultiView (math::clamp (worldInfo -> getMetaData <int32_t> ("/Titania/Page/multiView"), 0, 1));
	}
}

void
NotebookPage::on_map ()
{
	getBrowserWindow () -> getEditing () .addInterest (&NotebookPage::set_editing, this);

	set_editing ();
}

void
NotebookPage::on_unmap ()
{
	getBrowserWindow () -> getEditing () .addInterest (&NotebookPage::set_editing, this);
}

void
NotebookPage::set_editing ()
{
	if (getMultiView () and not getBrowserWindow () -> getEditing ())
	{
		setActiveView (1);
		setMultiView (not getMultiView ());
	}
}

bool
NotebookPage::on_box1_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 0);
}

bool
NotebookPage::on_box2_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 1);
}

bool
NotebookPage::on_box3_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 2);
}

bool
NotebookPage::on_box4_key_release_event (GdkEventKey* event)
{
	return on_box_key_release_event (event, 3);
}

bool
NotebookPage::on_box_key_release_event (GdkEventKey* event, const size_t index)
{
	switch (event -> keyval)
	{
		case GDK_KEY_space:
		{
			if (not getBrowserWindow () -> getEditing ())
				return false;
		
			setActiveView (index);
			setMultiView (not getMultiView ());

			return true;
		}
		default:
			return false;
	}

	return false;
}

void
NotebookPage::setPanel (const size_t id, std::unique_ptr <BrowserPanel> & panel, const PanelType panelType, Gtk::Viewport & box)
{
	box .remove ();

	switch (panelType)
	{
		case PanelType::BROWSER_PANEL:
		case PanelType::RENDER_PANEL:
		{
		   panel = std::make_unique <BrowserPanel> (getBrowserWindow (), this, id);
			break;
		}
//		case PanelType::RENDER_PANEL:
//		{
//			break;
//		}
	}

	panel -> getWidget () .reparent (box);

	panel -> getPanelType () .addInterest (&NotebookPage::setPanel, this, id, std::ref (panel), std::ref (panel -> getPanelType ()), std::ref (box));
}

void
NotebookPage::setActiveView (const size_t value)
{
	activeView = value;

	createWorldInfo (getScene ()) -> setMetaData <int32_t> ("/Titania/Page/activeView", activeView);
}

void
NotebookPage::setMultiView (const bool value)
{
	multiView = value;

	createWorldInfo (getScene ()) -> setMetaData <int32_t> ("/Titania/Page/multiView", multiView);

	for (size_t i = 0, size = boxes .size (); i < size; ++ i)
	{
		boxes [i] -> set_visible (multiView or i == getActiveView ());
	}
 }

void
NotebookPage::shutdown ()
{
	X3DNotebookPage::shutdown ();
}

NotebookPage::~NotebookPage ()
{
   panel1 .reset ();
   panel2 .reset ();
   panel3 .reset ();
   panel4 .reset ();

	dispose ();
}

} // puck
} // titania
