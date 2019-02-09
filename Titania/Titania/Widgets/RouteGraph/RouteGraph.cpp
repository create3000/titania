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

#include "RouteGraph.h"

#include "RouteGraphPage.h"

#include "../../Configuration/config.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

RouteGraph::RouteGraph (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DRouteGraphInterface (get_ui ("Widgets/RouteGraph.glade")),
	         X3DRouteGraph (),
	                 pages (),
	           currentPage (),
	              changing (false)
{
	setup ();
}

void
RouteGraph::configure ()
{
	X3DRouteGraphInterface::configure ();
	X3DRouteGraph::configure ();

	getAddConnectedNodesMenuItem () .set_active (getConfig () -> getItem <bool> ("addConnectedNodes", true));
}

void
RouteGraph::initialize ()
{
	X3DRouteGraphInterface::initialize ();
	X3DRouteGraph::initialize ();

	getBrowserWindow () -> getCurrentPage () -> getMainBrowser () -> initialized () .addInterest (&RouteGraph::set_scene, this);

	set_scene ();
}

void
RouteGraph::setCurrentPage (const size_t pageNumber, const bool modify)
{
	try
	{
		currentPage = pages .at (pageNumber);

		changing = true;

		getNotebook () .set_current_page (pageNumber);

		changing = false;

		if (modify)
		{
			createWorldInfo (getCurrentScene ()) -> setMetaData <int32_t> ("/Titania/RouteGraph/currentPage", pageNumber);

			getBrowserWindow () -> getCurrentPage () -> setModified (true);
		}
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

RouteGraphPagePtr
RouteGraph::createPage ()
{
	// Find new page name.

	std::string pageName;

	size_t i = 0;

	for (;;)
	{
		pageName = _ ("New Logic") + " " + basic::to_string (++ i, std::locale::classic ());

		const auto count = std::count_if (pages .begin (), pages .end (), [&] (const RouteGraphPagePtr & page)
		{
			return page -> getPageName () == pageName;
		});

		if (count == 0)
			break;
	}

	// Append page and bring to front.

	return appendPage (pageName);
}

RouteGraphPagePtr
RouteGraph::appendPage (const std::string & pageName)
{
	const auto page = std::make_shared <RouteGraphPage> (getBrowserWindow (), this, pageName);

	pages .emplace_back (page);

	changing = true;

	getNotebook () .append_page (page -> getWidget (), pageName);
	getNotebook () .set_tab_reorderable (page -> getWidget (), true);
	getNotebook () .set_menu_label_text (page -> getWidget (), pageName);

	changing = false;

	return page;
}

void
RouteGraph::closePage (const RouteGraphPagePtr page)
{
	pages .erase (std::remove (pages .begin (), pages .end (), page), pages .end ());

	getNotebook () .remove_page (page -> getWidget ());

	if (pages .empty ())
		appendPage (_ ("New Logic"));
}

void
RouteGraph::savePages ()
{
	createWorldInfo (getCurrentScene ()) -> removeMetaData ("/Titania/RouteGraph");

	for (const auto & page : pages)
		page -> save ();

	setCurrentPage (getNotebook () .get_current_page ());
}

void
RouteGraph::set_scene ()
{
	for (const auto & page : pages)
		getNotebook () .remove_page (page -> getWidget ());

	pages .clear ();

	try
	{
		const auto worldInfo = getWorldInfo (getCurrentScene ());
		const auto metaPages = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/RouteGraph/pages");

		for (size_t i = 0, size = metaPages .size (); i < size; ++ i)
			appendPage (_ ("Logic")) -> open ();

		setCurrentPage (worldInfo -> getMetaData <int32_t> ("/Titania/RouteGraph/currentPage"), false);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	if (pages .empty ())
	{
		appendPage (_ ("New Logic"));
		setCurrentPage (0, false);
	}
}

void
RouteGraph::on_new_page_activate ()
{
	createPage ();
	setCurrentPage (pages .size () - 1);
}

void
RouteGraph::on_rename_page_activate ()
{
	getCurrentPage () -> on_rename_page_activate ();
}

void
RouteGraph::on_align_to_grid_activate ()
{
	getCurrentPage () -> on_align_to_grid_activate ();
}

void
RouteGraph::on_export_page_activate ()
{
	getCurrentPage () -> on_export_page_activate ();
}

void
RouteGraph::on_delete_activate ()
{
	getCurrentPage () -> on_delete_activate ();
}

void
RouteGraph::on_select_all_activate ()
{
	getCurrentPage () -> on_select_all_activate ();
}

void
RouteGraph::on_deselect_all_activate ()
{
	getCurrentPage () -> on_deselect_all_activate ();
}

void
RouteGraph::on_close_page_activate ()
{
	const auto dialog = createDialog <MessageDialog> ("MessageDialog");

	dialog -> setType (Gtk::MESSAGE_QUESTION);
	dialog -> setMessage (_ ("Do you realy want to close logic »" + getCurrentPage () -> getPageName () + "«?"));
	dialog -> setText (_ ("The whole page will be irrevocably removed!"));

	if (dialog -> run () not_eq Gtk::RESPONSE_OK)
		return;

	closePage (getCurrentPage ());

	savePages ();
}

void
RouteGraph::on_new_page_clicked ()
{
	on_new_page_activate ();
}

void
RouteGraph::on_rename_page_clicked ()
{
	on_rename_page_activate ();
}

void
RouteGraph::on_delete_clicked ()
{
	on_delete_activate ();
}

void
RouteGraph::on_close_page_clicked ()
{
	on_close_page_activate ();
}

void
RouteGraph::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	if (changing)
		return;

	setCurrentPage (pageNumber);
}

void
RouteGraph::on_page_reordered (Gtk::Widget* widget, guint pageNumber)
{
	const auto iter = std::find_if (pages .begin (), pages .end (), [&] (const RouteGraphPagePtr & page) { return &page -> getWidget () == widget; });

	if (iter == pages .end ())
		return;

	auto page = std::move (*iter);

	pages .erase (iter);
	pages .emplace (pages .begin () + pageNumber, std::move (page));

	savePages ();
}

void
RouteGraph::store ()
{
	getConfig () -> setItem <bool> ("addConnectedNodes", getAddConnectedNodesMenuItem () .get_active ());

	X3DRouteGraph::store ();
	X3DRouteGraphInterface::store ();
}

RouteGraph::~RouteGraph ()
{
	dispose ();
}

} // puck
} // titania
