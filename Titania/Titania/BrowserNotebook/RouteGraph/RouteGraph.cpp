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
#include "../NotebookPage/NotebookPage.h"

#include "../../Configuration/config.h"
#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

RouteGraph::RouteGraph (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRouteGraphInterface (get_ui ("Panels/RouteGraph.glade"), page, PanelType::ROUTE_GRAPH, id),
	         X3DRouteGraph (),
	                 pages (),
	           currentPage ()
{
	setup ();
}

void
RouteGraph::initialize ()
{
	X3DRouteGraphInterface::initialize ();

	currentPage = appendPage (_ ("New Logic"));
}

int32_t
RouteGraph::getPageNumber (const RouteGraphPagePtr & page) const
{
	return getNotebook () .page_num (page -> getWidget ());
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

	currentPage = appendPage (pageName);

	getNotebook () .set_current_page (getPageNumber (currentPage));

	return currentPage;
}

RouteGraphPagePtr
RouteGraph::appendPage (const std::string & sheetName)
{
	const auto page = std::make_shared <RouteGraphPage> (getBrowserWindow (), sheetName);

	pages .emplace_back (page);

	getNotebook () .append_page (page -> getWidget (), sheetName);
	getNotebook () .set_tab_reorderable (page -> getWidget (), true);
	getNotebook () .set_menu_label_text (page -> getWidget (), sheetName);

	return page;
}

void
RouteGraph::on_new_page_activate ()
{
	createPage ();
}

void
RouteGraph::on_rename_page_activate ()
{
	currentPage -> on_rename_page_activate ();
}

void
RouteGraph::on_align_to_grid_activate ()
{
	currentPage -> on_align_to_grid_activate ();
}

void
RouteGraph::on_export_page_activate ()
{
	currentPage -> on_export_page_activate ();
}

void
RouteGraph::on_delete_activate ()
{
	currentPage -> on_delete_activate ();
}

void
RouteGraph::on_select_all_activate ()
{
	currentPage -> on_select_all_activate ();
}

void
RouteGraph::on_deselect_all_activate ()
{
	currentPage -> on_deselect_all_activate ();
}

void
RouteGraph::on_close_page_activate ()
{
}

RouteGraph::~RouteGraph ()
{
	dispose ();
}

} // puck
} // titania
