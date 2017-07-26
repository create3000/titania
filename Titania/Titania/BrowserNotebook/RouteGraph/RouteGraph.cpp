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

#include "../NotebookPage/NotebookPage.h"

#include "../../Configuration/config.h"

namespace titania {
namespace puck {

RouteGraph::RouteGraph (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRouteGraphInterface (get_ui ("Panels/RouteGraph.glade"), page, PanelType::ROUTE_GRAPH, id),
	         X3DRouteGraph (),
	         titleRenderer ()
{
	titleRenderer .property_text ()           = _ ("New Sheet");
	titleRenderer .property_size_points ()    = 18;
	titleRenderer .property_foreground ()     = "white";
	titleRenderer .property_foreground_set () = true;

	// Drag & drop targets

	getViewport () .drag_dest_set ({ Gtk::TargetEntry ("TITANIA_NODE_ID", Gtk::TARGET_SAME_APP) }, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	setup ();
}

void
RouteGraph::initialize ()
{
	X3DRouteGraphInterface::initialize ();
}

void
RouteGraph::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                   int x, int y,
                                   const Gtk::SelectionData & selection_data,
                                   guint info,
                                   guint time)
{
	__LOG__ << std::endl;

	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		__LOG__ << selection_data .get_data_type () << std::endl;

		if (selection_data .get_data_type () == "TITANIA_NODE_ID")
		{
			size_t nodeId = 0;

			std::istringstream isstream (selection_data .get_data_as_string ());

			isstream .imbue (std::locale::classic ());

			isstream >> nodeId;

			__LOG__ << nodeId << std::endl;

			context -> drag_finish (true, false, time);
			return;
		}
	}

	context -> drag_finish (false, false, time);
}

bool
RouteGraph::on_button_press_event (GdkEventButton* event) 
{
	__LOG__ << std::endl;

	if (event -> button == 2)
	{

	}

	return false;
}

bool
RouteGraph::on_motion_notify_event (GdkEventMotion* event) 
{
	__LOG__ << std::endl;

	return false;
}

bool
RouteGraph::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto flags  = Gtk::CellRendererState (0);
	const auto width  = getFixed () .get_width ();
	const auto height = getFixed () .get_height ();

	// Background

	context -> set_source_rgb (0, 0, 0);
	context -> rectangle (0, 0, width, height);
	context -> fill ();

	// Grid

	static constexpr auto gridSize  = 40;
	static constexpr auto gridColor = 0.38823529;

	const auto xGridOffset = (width % gridSize) / 2;
	const auto yGridOffset = (height % gridSize) / 2;

	context -> set_source_rgb (gridColor, gridColor, gridColor);

	for (int32_t y = yGridOffset; y < height; y += gridSize)
	{
		for (int32_t x = xGridOffset; x < width; x += gridSize)
			context -> rectangle (x, y, 1, 1);
	}

	context -> fill ();

	// Title

	const Gdk::Rectangle titleArea (gridSize / 2, 0, width, gridSize + 4);
	titleRenderer .render (context, getFixed (), titleArea, titleArea, flags);

	return false;
}

RouteGraph::~RouteGraph ()
{
	dispose ();
}

} // puck
} // titania
