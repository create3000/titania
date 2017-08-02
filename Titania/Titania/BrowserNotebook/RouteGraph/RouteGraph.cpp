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
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Dialogs/FileSaveDialog/FileExportImageDialog.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Editing/X3DEditor.h>

namespace titania {
namespace puck {

RouteGraph::RouteGraph (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRouteGraphInterface (get_ui ("Panels/RouteGraph.glade"), page, PanelType::ROUTE_GRAPH, id),
	         X3DRouteGraph (),
	               windows (),
	             selection (),
	                button (0),
	               pointer (),
	             sheetName (_ ("New Sheet")),
	 inputConnectorClicked (false),
	outputConnectorClicked (false),
	       matchingContext (),
	             routeNode (),
	            routeField (nullptr)
{
	getSheetName () .set_text (sheetName);

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
throw (std::runtime_error)
{
	X3D::SFNode found;

	X3D::traverse (getCurrentContext (), [&] (X3D::SFNode & node)
	{
		if (id == node -> getId ())
		{
			found = node;
			return false;
		}
		
		return true;
	},
	X3D::TRAVERSE_PROTO_DECLARATIONS |
	X3D::TRAVERSE_PROTO_DECLARATION_BODY |
	X3D::TRAVERSE_ROOT_NODES |
	X3D::TRAVERSE_INLINE_NODES |
	X3D::TRAVERSE_IMPORTED_NODES);

	if (found)
		return found;

	throw std::runtime_error ("RouteGraph::getNode");
}

void
RouteGraph::addWindow (const X3D::SFNode & node, const X3D::Vector2i & position)
{
	const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window -> node == node; });

	if (iter not_eq windows .end ())
		return;

	const auto window = std::make_shared <RouteGraphWindow> ();
	const auto widget = std::make_shared <RouteGraphNode> (node);

	windows .emplace_front (window);

	window -> node     = node;
	window -> widget   = widget;
	window -> position = position;

	widget -> signal_changed ()                  .connect (sigc::mem_fun (this, &RouteGraph::refresh));
	widget -> signal_input_connector_clicked ()  .connect (sigc::bind (sigc::mem_fun (this, &RouteGraph::on_input_connector_clicked),  window .get ()));
	widget -> signal_output_connector_clicked () .connect (sigc::bind (sigc::mem_fun (this, &RouteGraph::on_output_connector_clicked), window .get ()));
	widget -> show ();

	for (const auto & field : node -> getFieldDefinitions ())
	{
		field -> getInputRoutes ()  .addInterest (&RouteGraph::refresh, this);
		field -> getOutputRoutes () .addInterest (&RouteGraph::refresh, this);
	}

	getFixed () .put (*widget, position .x (), position .y ());
}

void
RouteGraph::setPosition (const RouteGraphWindowPtr window, const X3D::Vector2i & position)
{
	const auto & widget = window -> widget;

	window -> position = position;

	getFixed () .move (*widget, position .x (), position .y ());
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

	refresh ();
}

void
RouteGraph::clearSelection ()
{
	selection .clear ();

	refresh ();
}

void
RouteGraph::clearConnectorSelection ()
{
	if (inputConnectorClicked or outputConnectorClicked)
	{
		inputConnectorClicked  = false;
		outputConnectorClicked = false;

		matchingContext = nullptr;
		routeNode       = nullptr;
		routeField      = nullptr;

		for (const auto & window : windows)
			window -> widget -> enableConnectors ();
	}
}

void
RouteGraph::refresh ()
{
	getFixed () .queue_draw ();
}

void
RouteGraph::on_rename_sheet_activate ()
{

}

void
RouteGraph::on_align_to_grid_activate ()
{
	for (const auto & window : windows)
	{
		static constexpr double snapDistance = 40;
		static constexpr auto   offset       = X3D::Vector2d (7, 0);

		auto position = X3D::Vector2d (window -> position) + offset;

		position = X3D::round (position / snapDistance) * snapDistance - offset;

		setPosition (window, X3D::Vector2i (position));
	}
}

void
RouteGraph::on_export_sheet_activate ()
{
	try
	{
		static const std::map <int32_t, std::string> magick = {
			std::make_pair (1, "GRAY"),
			std::make_pair (3, "RGB"),
			std::make_pair (4, "RGBA"),
		};

		Gtk::OffscreenWindow window;

		window .set_size_request (getFixed () .get_width (), getFixed () .get_height ());
		getOverlay () .remove ();
		window .add (getFixed ());
		window .show ();

		// This must be done, otherwise the pixbuf is empty.
		while (Gtk::Main::events_pending ())
			Gtk::Main::iteration ();

		const auto pixbuf = window .get_pixbuf ();

		window .remove ();
		getOverlay () .add (getFixed ());

		// Convert pixbuf to magick image

		if (not pixbuf)
			throw std::runtime_error ("RouteGraph::on_export_sheet_activate: pixbuf is NULL.");

		const auto channels = pixbuf -> get_n_channels ();
		const auto width    = pixbuf -> get_width ();
		const auto height   = pixbuf -> get_height ();
		const auto data     = pixbuf -> get_pixels ();

		Magick::Image image (width, height, magick .at (channels), Magick::CharPixel, data);

		// Save magick image

		const auto dialog = std::dynamic_pointer_cast <FileExportImageDialog> (addDialog ("FileExportImageDialog", false));

		image .quality (100);

		dialog -> save (image, sheetName + ".png");
	}
	catch (const std::exception & error)
	{
		// No magick
		__LOG__ << error .what () << std::endl;
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

			try
			{
				// Add node widget
	
				addWindow (getNode (nodeId), X3D::Vector2i (x, y));
			}
			catch (const std::exception & error)
			{
				__LOG__ << error .what () << std::endl;
			}

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

	// Clear selection

	clearSelection ();

	// Clear connector selection

	clearConnectorSelection ();

	// Handle buttons

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

	// Grid

	context -> set_source_rgb (gridColor, gridColor, gridColor);

	for (int32_t y = 0; y < height; y += gridSize / 4)
		context -> rectangle (0, y, width, 1);

	for (int32_t x = 0; x < width; x += gridSize / 4)
		context -> rectangle (x, 0, 1, height);

	context -> fill ();

	// Points

	context -> set_source_rgb (gridPointColor, gridPointColor, gridPointColor);

	for (int32_t y = 0; y < height; y += gridSize)
	{
		for (int32_t x = 0; x < width; x += gridSize)
			context -> rectangle (x, y, 1, 1);
	}

	context -> fill ();

	on_draw_routes (context);

	return false;
}

void
RouteGraph::on_draw_routes (const Cairo::RefPtr <Cairo::Context> & context)
{
	context -> set_line_width (2);

	for (const auto & sourceWindow : windows)
	{
		const auto & sourceNode   = sourceWindow -> node;
		const auto & sourceWidget = sourceWindow -> widget;

		for (const auto & sourceField : sourceNode -> getFieldDefinitions ())
		{
			for (const auto & route : sourceField -> getOutputRoutes ())
			{
				try
				{
					const auto destinationNode  = route -> getDestinationNode ();
					const auto destinationField = destinationNode -> getField (route -> getDestinationField ());
					const auto iter             = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window -> node == destinationNode; });

					if (iter == windows .end ())
						continue;

					const auto & destinationWindow   = *iter;
					const auto & destinationWidget   = destinationWindow -> widget;
					const auto   sourcePosition      = X3D::Vector2d (sourceWindow -> position + sourceWidget -> getOutputPosition (sourceField));
					const auto   destinationPosition = X3D::Vector2d (destinationWindow -> position + destinationWidget -> getInputPosition (destinationField));

					// Draw sine curved route, dark or light

					const auto wp = (destinationPosition .x () - sourcePosition .x ()) * 0.5;
					const auto cf = wp > 0 ? 1.0 : 0.5;
					const auto x0 = sourcePosition .x ();
					const auto y0 = sourcePosition .y ();
					const auto x1 = sourcePosition .x () + wp;
					const auto y1 = sourcePosition .y ();
					const auto x2 = destinationPosition .x () - wp;
					const auto y2 = destinationPosition .y ();
					const auto x3 = destinationPosition .x ();
					const auto y3 = destinationPosition .y ();
 
					context -> set_source_rgb (255.0 / 255.0 * cf, 219.0 / 255.0 * cf, 165.0 / 255.0 * cf);
					context -> move_to (x0, y0);
					context -> curve_to (x1, y1, x2, y2, x3, y3);
					context -> stroke ();

					// Draw arrow

					const auto p1 = X3D::Vector2d (0, 0);
					const auto p2 = X3D::Vector2d (0, 14);
					const auto p3 = X3D::Vector2d (10.0 / 9.0 * 14, 7);
					const auto c  = X3D::negate ((p1 + p2 + p3) / 3.0);
					const auto t  = (sourcePosition + destinationPosition) / 2.0;
					const auto d  = destinationPosition - sourcePosition;
					const auto s  = std::sin (1 / d .x () * math::pi <double> * math::pi <double> / 2) * d .y () / 2.0;
					const auto a  = d .x () ? std::atan2 (1, 1 / s) : math::pi <double> / 2;

					context -> save ();
					context -> translate (t .x (), t .y ());
					context -> rotate (d .y () >= 0 ? a : a + math::pi <double>);
					context -> translate (c .x (), c .y ());
					context -> move_to (p1 .x (), p1 .y ());
					context -> line_to (p2 .x (), p2 .y ());
					context -> line_to (p3 .x (), p3 .y ());
					context -> close_path ();
					context -> fill ();
					context -> restore ();
				}
				catch (const std::exception & error)
				{ }
			}
		}
	}
}

void
RouteGraph::on_input_connector_clicked (X3D::X3DFieldDefinition* const connectorField, RouteGraphWindow* const window)
{
	if (inputConnectorClicked)
		return;

	if (outputConnectorClicked)
	{
		const auto sourceNode       = routeNode;
		const auto sourceField      = routeField -> getName ();
		const auto destinationNode  = window -> node;
		const auto destinationField = connectorField -> getName ();

		try
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Route"));
			X3D::X3DEditor::storeMatrix (destinationNode, undoStep);
			X3D::X3DEditor::addRoute (matchingContext, sourceNode, sourceField, destinationNode, destinationField, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
		}
		catch (const X3D::X3DError &)
		{ }

		clearConnectorSelection ();
	}
	else
	{
		inputConnectorClicked = true;

		matchingContext = X3D::X3DExecutionContextPtr (window -> node -> getExecutionContext ());
		routeNode       = window -> node;
		routeField      = connectorField;

		for (const auto & window : windows)
			window -> widget -> disableOutputConnectors (matchingContext, routeNode, routeField);
	}
}

void
RouteGraph::on_output_connector_clicked (X3D::X3DFieldDefinition* const connectorField, RouteGraphWindow* const window)
{
	if (outputConnectorClicked)
		return;

	if (inputConnectorClicked)
	{
		const auto sourceNode       = window -> node;
		const auto sourceField      = connectorField  -> getName ();
		const auto destinationNode  = routeNode;
		const auto destinationField = routeField-> getName ();

		try
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Route"));
			X3D::X3DEditor::storeMatrix (destinationNode, undoStep);
			X3D::X3DEditor::addRoute (matchingContext, sourceNode, sourceField, destinationNode, destinationField, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
		}
		catch (const X3D::X3DError &)
		{ }

		clearConnectorSelection ();
	}
	else
	{
		outputConnectorClicked = true;
	
		matchingContext = X3D::X3DExecutionContextPtr (window -> node -> getExecutionContext ());
		routeNode       = window -> node;
		routeField      = connectorField;
	
		for (const auto & window : windows)
			window -> widget -> disableInputConnectors (matchingContext, routeNode, routeField);
	}
}

RouteGraph::~RouteGraph ()
{
	dispose ();
}

} // puck
} // titania
