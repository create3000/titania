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

#include "RouteGraphNode.h"
#include "../NotebookPage/NotebookPage.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Bits/Traverse.h>

namespace titania {
namespace puck {

RouteGraph::RouteGraph (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRouteGraphInterface (get_ui ("Panels/RouteGraph.glade"), page, PanelType::ROUTE_GRAPH, id),
	         X3DRouteGraph (),
	               windows (),
	             selection (),
	                button (0),
	               pointer ()
{
	getOverlay () .add_overlay (getSheetName ());

	// Drag & drop targets

	getViewport () .drag_dest_set ({ Gtk::TargetEntry ("TITANIA_NODE_ID", Gtk::TARGET_SAME_APP) }, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	setup ();
}

void
RouteGraph::initialize ()
{
	X3DRouteGraphInterface::initialize ();
}

X3D::SFNode
RouteGraph::getNode (const size_t id) const
{
	X3D::SFNode found;

	X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
	{
		if (id == node -> getId ())
		{
			found = node;
			return false;
		}
		
		return true;
	},
	X3D::TRAVERSE_INLINE_NODES);

	return found;
}

void
RouteGraph::addNode (const X3D::SFNode & node, const X3D::Vector2i & position)
{
	const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window -> node == node; });

	if (iter not_eq windows .end ())
		return;

	const auto window = std::make_shared <RouteGraphWindow> ();
	const auto widget = std::make_shared <RouteGraphNode> (node);

	window -> node     = node;
	window -> widget   = widget;
	window -> position = position;

	widget -> show ();

	getFixed () .put (*widget, position .x (), position .y ());

	windows .emplace_front (window);
}

void
RouteGraph::setPosition (const RouteGraphWindowPtr window, const X3D::Vector2i & position)
{
	const auto & widget = window -> widget;

	window -> position = position;

	getFixed () .move (*widget, position .x (), position .y ());

	//getFixed () .remove (*widget);
	//getFixed () .put (*widget, position .x (), position .y ());
}

void
RouteGraph::bringToFront (const RouteGraphWindowPtr window)
{
	const auto & widget   = window -> widget;
	const auto & position = window -> position;

	getFixed () .remove (*widget);
	getFixed () .put (*widget, position .x (), position .y ());

	windows .erase (std::remove (windows .begin (), windows .end (), window), windows .end ());
	windows .emplace_front (window);
}

void
RouteGraph::setSelection (const RouteGraphWindowPtr window)
{
	selection .clear ();
	selection .emplace_back (window);
	bringToFront (window);
}

void
RouteGraph::on_align_to_grid_activate ()
{
	for (const auto & window : windows)
	{
		static constexpr double snapDistance = 20;

		auto position = X3D::Vector2d (window -> position);

		position = X3D::round (position / snapDistance) * snapDistance;

		window -> position = X3D::Vector2i (position);
	}
}

void
RouteGraph::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                   int x, int y,
                                   const Gtk::SelectionData & selection_data,
                                   guint info,
                                   guint time)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		if (selection_data .get_data_type () == "TITANIA_NODE_ID")
		{
			// Parse node id.

			size_t nodeId = 0;

			std::istringstream isstream (selection_data .get_data_as_string ());

			isstream .imbue (std::locale::classic ());

			isstream >> nodeId;

			// Add node widget

			const auto node = getNode (nodeId);

			if (node)
				addNode (node, X3D::Vector2i (x, y));

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

	button = event -> button;

	if (button == 1)
	{
		// Prepare move widget

		pointer = X3D::Vector2i (event -> x, event -> y);

		for (const auto & window : windows)
		{
			const auto & widget   = window -> widget;
			const auto & position = window -> position;
			const auto   width    = widget -> get_width ();
			const auto   height   = widget -> get_height ();
			const auto   box      = X3D::Box2i (position, position + X3D::Vector2i (width, height), X3D::extents_type ());

			if (not box .intersects (pointer))
				continue;

			setSelection (window);

			// Prepare move

			widget -> setConnectorsSensitive (false);	

			return true;
		}
	}

	return false;
}

bool
RouteGraph::on_button_release_event (GdkEventButton* event) 
{
	__LOG__ << std::endl;

	button = 0;

	for (const auto & window : selection)
	{
		const auto & widget = window -> widget;

		widget -> setConnectorsSensitive (true);	
	}

	return false;
}

bool
RouteGraph::on_motion_notify_event (GdkEventMotion* event) 
{
	__LOG__ << std::endl;

	if (button == 1)
	{
		const auto translation = X3D::Vector2i (event -> x, event -> y) - pointer;

		for (const auto & window : selection)
		{
			auto position = window -> position;

			position += translation;
			position  = X3D::max (X3D::Vector2i (), position);

			setPosition (window, position);
		}

		pointer = X3D::Vector2i (event -> x, event -> y);

		return true;
	}

	return false;
}

bool
RouteGraph::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto width  = getFixed () .get_width ();
	const auto height = getFixed () .get_height ();

	// Background

	context -> set_source_rgb (0, 0, 0);
	context -> rectangle (0, 0, width, height);
	context -> fill ();

	// Grid with points

	static constexpr auto gridSize       = 40;
	static constexpr auto gridColor      = 14.0 / 255.0;
	static constexpr auto gridPointColor = 99.0 / 255.0;

	const auto xGridOffset = (getScrolledWindow () .get_width ()  % gridSize) / 2;
	const auto yGridOffset = (getScrolledWindow () .get_height () % gridSize) / 2;

	// Grid

	context -> set_source_rgb (gridColor, gridColor, gridColor);

	for (int32_t y = yGridOffset; y < height; y += gridSize / 4)
		context -> rectangle (0, y, width, 1);

	for (int32_t x = xGridOffset; x < width; x += gridSize / 4)
		context -> rectangle (x, 0, 1, height);

	context -> fill ();

	// Points

	context -> set_source_rgb (gridPointColor, gridPointColor, gridPointColor);

	for (int32_t y = yGridOffset; y < height; y += gridSize)
	{
		for (int32_t x = xGridOffset; x < width; x += gridSize)
			context -> rectangle (x, y, 1, 1);
	}

	context -> fill ();

	return false;
}

RouteGraph::~RouteGraph ()
{
	dispose ();
}

} // puck
} // titania
