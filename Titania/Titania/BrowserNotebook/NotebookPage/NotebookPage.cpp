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
#include "../RenderPanel/RenderPanel.h"
#include "../RouteGraph/RouteGraph.h"

namespace titania {
namespace puck {

NotebookPage::NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	         X3DNotebookPage (startUrl),
	                   boxes ({ &getBox1 (), &getBox2 (), &getBox3 (), &getBox4 () }),
	                  panels (4),
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
	set_panel (0, getPanelType (0), getBox1 ());
	set_panel (1, getPanelType (1), getBox2 ());
	set_panel (2, getPanelType (2), getBox3 ());
	set_panel (3, getPanelType (3), getBox4 ());
}

void
NotebookPage::initialized ()
{
	X3DNotebookPage::initialized ();

	if (getBrowserWindow () -> getEditing ())
	{
		const auto worldInfo  = createWorldInfo (getScene ());
		const auto activeView = math::clamp (worldInfo -> getMetaData <int32_t> ("/Titania/Page/activeView", 1), 0, 4);

		for (int32_t i = 0, size = panels .size (); i < size; ++ i)
			panels [i] -> setFocus (i == activeView);

		setActiveView (activeView);
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
NotebookPage::on_key_release_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_space:
		{
			if (not getBrowserWindow () -> getEditing ())
				return false;

			setMultiView (not getMultiView ());
			setModified (true);

			return true;
		}
		default:
			return false;
	}

	return false;
}

void
NotebookPage::setPanelType (const size_t id, const PanelType panelType)
{
	static const std::map <PanelType, std::string> panelTypes = {
		std::make_pair (PanelType::BROWSER_PANEL, "BROWSER_PANEL"),
		std::make_pair (PanelType::RENDER_PANEL,  "RENDER_PANEL"),
		std::make_pair (PanelType::ROUTE_GRAPH,   "ROUTE_GRAPH"),
	};

	auto panelsArray = createWorldInfo (getScene ()) -> getMetaData <X3D::MFString> ("/Titania/Page/panels");

	panelsArray .resize (4, X3D::SFString ("BROWSER_PANEL"));

	if (panelsArray [id] not_eq panelTypes .at (panelType))
	{
		panelsArray [id] = panelTypes .at (panelType);
	
		createWorldInfo (getScene ()) -> setMetaData ("/Titania/Page/panels", panelsArray);
	
		setModified (true);
	}
}

PanelType
NotebookPage::getPanelType (const size_t id) const
{
	try
	{
		static const std::map <std::string, PanelType> panelTypes = {
			std::make_pair ("BROWSER_PANEL", PanelType::BROWSER_PANEL),
			std::make_pair ("RENDER_PANEL",  PanelType::RENDER_PANEL),
			std::make_pair ("ROUTE_GRAPH",   PanelType::ROUTE_GRAPH),
		};

		const auto panelsArray = getWorldInfo (getScene ()) -> getMetaData <X3D::MFString> ("/Titania/Page/panels");

		return panelTypes .at (panelsArray .at (id));
	}
	catch (const std::exception &)
	{
		return PanelType::BROWSER_PANEL;
	}
}

void
NotebookPage::setPanel (const size_t id, const PanelType panelType, Gtk::Viewport & box)
{
	setPanelType (id, panelType);

	set_panel (id, panelType, box);
}

void
NotebookPage::set_panel (const size_t id, const PanelType panelType, Gtk::Viewport & box)
{
	auto & panel = panels [id];

	box .remove ();

	switch (panelType)
	{
		case PanelType::BROWSER_PANEL:
		{
		   panel = std::make_unique <BrowserPanel> (getBrowserWindow (), this, id);
			break;
		}
		case PanelType::RENDER_PANEL:
		{
		   panel = std::make_unique <RenderPanel> (getBrowserWindow (), this, id);
			break;
		}
		case PanelType::ROUTE_GRAPH:
		{
		   panel = std::make_unique <RouteGraph> (getBrowserWindow (), this, id);
			break;
		}
	}


	panel -> getWidget () .reparent (box);

	panel -> getPanelType () .addInterest (&NotebookPage::setPanel, this, id, std::ref (panel -> getPanelType ()), std::ref (box));
	panel -> hasFocus ()     .addInterest (&NotebookPage::set_focus, this, id);
}

void
NotebookPage::set_focus (const size_t id)
{
	if (panels [id] -> hasFocus ())
		setActiveView (id);
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
   panels .clear ();

	dispose ();
}

} // puck
} // titania
