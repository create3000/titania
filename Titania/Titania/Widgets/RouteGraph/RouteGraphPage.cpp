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

#include "RouteGraphPage.h"

#include "RouteGraph.h"
#include "RouteGraphNode.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileSaveDialog/FileExportImageDialog.h"

#include <Titania/String.h>
#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Editing/X3DEditor.h>

#include <random>

namespace titania {
namespace puck {

static constexpr auto   GRID_SIZE        = 40;
static constexpr auto   SMALL_GRID_SIZE  = 10;
static constexpr auto   GRID_COLOR       = 14.0 / 255.0;
static constexpr auto   GRID_POINT_COLOR = 99.0 / 255.0;
static constexpr double SNAP_DISTANCE    = GRID_SIZE;
static constexpr auto   SNAP_OFFSET      = X3D::Vector2d (7, 0);

RouteGraphPage::RouteGraphPage (X3DBrowserWindow* const browserWindow, RouteGraph* const routeGraph, const std::string & pageName) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DRouteGraphPageInterface (get_ui ("Widgets/RouteGraphPage.glade")),
	         X3DRouteGraphPage (),
	                routeGraph (routeGraph),
	                   windows (),
	                 selection (),
	            routeSelection (),
	                    button (0),
	                  position (),
	             startPosition (),
	               endPosition (),
	                motionType (MotionType::DEFAULT),
	                  pageName (pageName),
	     inputConnectorClicked (false),
	    outputConnectorClicked (false),
	           matchingContext (),
	                 routeNode (),
	                routeField (nullptr)
{
	unparent (getWidget ());

	getScrolledWindow () .get_hscrollbar () -> set_visible (false);
	getScrolledWindow () .get_vscrollbar () -> set_visible (false);

	getViewport () .drag_dest_set ({ Gtk::TargetEntry ("titania/node-id", Gtk::TARGET_SAME_APP) }, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	setup ();
}

void
RouteGraphPage::configure ()
{
	X3DRouteGraphPageInterface::configure ();
	X3DRouteGraphPage::configure ();
}

void
RouteGraphPage::initialize ()
{
	X3DRouteGraphPageInterface::initialize ();
	X3DRouteGraphPage::initialize ();

	setPageName (pageName, false);
}

int32_t
RouteGraphPage::getPageNumber () const
{
	return routeGraph -> getNotebook () .page_num (getWidget ());
}

void
RouteGraphPage::setPageName (const std::string & value, const bool modify)
{
	pageName = value;

	getPageNameLabel () .set_text (pageName);

	routeGraph -> getNotebook () .set_tab_label_text  (getWidget (), pageName);
	routeGraph -> getNotebook () .set_menu_label_text (getWidget (), pageName);

	if (not modify)
		return;

	createWorldInfo (getCurrentScene ()) -> setMetaData (getMetaKey () + "/pageName", pageName);

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

std::string
RouteGraphPage::getMetaKey () const
{
	return "/Titania/RouteGraph/pages/Page" + basic::to_string (getPageNumber (), std::locale::classic ());
}

void
RouteGraphPage::open ()
{
	try
	{
		const auto worldInfo      = getWorldInfo (getCurrentScene ());
		const auto key            = getMetaKey ();
		const auto nodePositions  = worldInfo -> getMetaData <X3D::MFVec2d> (key + "/nodePositions");
		const auto nodes          = worldInfo -> getMetaData <X3D::MFNode> (key + "/nodes");
		const auto scrollPosition = worldInfo -> getMetaData <X3D::Vector2d> (key + "/scrollPosition");

		setPageName (worldInfo -> getMetaData (key + "/pageName", getPageName ()), false);

		for (size_t i = 0, size = nodes .size (); i < size; ++ i)
		{
			const auto & position = i < nodePositions .size () ? nodePositions [i] : X3D::Vector2d ();
			const auto & node     = nodes [i];

			if (not node)
				continue;

			addNode (node, X3D::Vector2i (position));
		}

		Glib::signal_idle () .connect_once (sigc::mem_fun (this, &RouteGraphPage::resize));
		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &RouteGraphPage::setScrollPosition), scrollPosition, false));
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
RouteGraphPage::queueSave () 
{
	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &RouteGraphPage::resize));
	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &RouteGraphPage::save));
}

void
RouteGraphPage::save ()
{
	const auto worldInfo      = createWorldInfo (getCurrentScene ());
	const auto key            = getMetaKey ();

	worldInfo -> setMetaData (key + "/pageName",       getPageName ());
	worldInfo -> setMetaData (key + "/scrollPosition", getScrollPosition ());

	X3D::MFVec2d nodePositions;
	X3D::MFNode  nodes;

	for (const auto & window : windows)
	{
		nodePositions .emplace_back (getPosition (window));
		nodes         .emplace_back (window -> node);
	}

	worldInfo -> setMetaData (key + "/nodePositions", nodePositions);
	worldInfo -> setMetaData (key + "/nodes",         nodes);

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

X3D::SFNode
RouteGraphPage::getNode (const size_t id) const
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

	throw std::runtime_error ("RouteGraphPage::getNode");
}

void
RouteGraphPage::addConnectedNodes (const X3D::SFNode & node, const X3D::Vector2i & position)
{
	std::map <int32_t, X3D::MFNode> columns;
	std::set <X3D::SFNode>          nodes;

	getConnectedNodes (node -> getExecutionContext (), node, 0, columns, nodes);

	int32_t x = position .x ();

	for (const auto & column : columns)
	{
		int32_t y = position .y ();

		for (const auto & node : column .second)
		{
			addNode (node, X3D::Vector2i (x, y));

			y += 400;
		}

		x += 300;
	}

	Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &RouteGraphPage::on_place_nodes), columns, position));
}

void
RouteGraphPage::on_place_nodes (const std::map <int32_t, X3D::MFNode> & columns, const X3D::Vector2i & position)
{
	static std::uniform_real_distribution <double>
	random (-1, 1);

	static std::default_random_engine
	random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

	const auto undoStep = columns .size () > 1
	                      ? std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Add Nodes To Logic »%s«"), getPageName () .c_str ()))
	                      : std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Add Node To Logic »%s«"), getPageName () .c_str ()));

	undoStep -> addUndoFunction (&RouteGraphPage::queueSave, this);

	int32_t x = position .x ();

	for (const auto & column : columns)
	{
		int32_t width = 0;
		int32_t y     = position .y ();

		for (const auto & node : column .second)
		{
			const auto   rposition = X3D::Vector2i (x + random (random_engine) * GRID_SIZE, y + random (random_engine) * GRID_SIZE);
			const auto   window    = addNode (node, rposition);
			const auto & widget    = window -> widget;

			undoStep -> addUndoFunction (&RouteGraphPage::removeNode, this, node);
			undoStep -> addRedoFunction (&RouteGraphPage::addNode,    this, node, rposition);

			setPosition (window, rposition);

			y += widget -> get_height ();
			y += GRID_SIZE * 2;

			width = std::max (width, widget -> get_width ());
		}

		x += width;
		x += GRID_SIZE * 4;
	}

	undoStep -> addRedoFunction (&RouteGraphPage::queueSave, this);

	queueSave ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
RouteGraphPage::getConnectedNodes (X3D::X3DExecutionContext* const executionContext,
                               const X3D::SFNode & node,
                               const int32_t index,
                               std::map <int32_t, X3D::MFNode> & columns,
                               std::set <X3D::SFNode> & nodes) const
{
	if (not nodes .emplace (node) .second)
		return; 

	if (node -> getExecutionContext () not_eq executionContext and not executionContext -> isImportedNode (node))
		return;

	columns [index] .emplace_back (node);

	for (const auto & field : node -> getFieldDefinitions ())
	{
		for (const auto & route : field -> getInputRoutes ())
			getConnectedNodes (executionContext, route -> getSourceNode (), index - 1, columns, nodes);

		for (const auto & route : field -> getOutputRoutes ())
			getConnectedNodes (executionContext, route -> getDestinationNode (), index + 1, columns, nodes);
	}
}

RouteGraphPage::RouteGraphWindowPtr
RouteGraphPage::addNode (const X3D::SFNode & node, const X3D::Vector2i & position)
{
	const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window -> node == node; });

	if (iter not_eq windows .end ())
		return *iter;

	const auto window = std::make_shared <RouteGraphWindow> ();
	const auto widget = std::make_shared <RouteGraphNode> (node);

	windows .emplace_front (window);

	window -> node   = node;
	window -> widget = widget;

	// Node

	node -> isLive () .addInterest (&RouteGraphPage::set_live, this, window);

	for (const auto & field : node -> getFieldDefinitions ())
	{
		field -> getInputRoutes ()  .addInterest (&RouteGraphPage::set_routes, this);
		field -> getOutputRoutes () .addInterest (&RouteGraphPage::set_routes, this);
	}

	// Widget

	widget -> signal_select_node_clicked ()      .connect (sigc::bind (sigc::mem_fun (this, &RouteGraphPage::on_select_node_clicked),      window .get ()));
	widget -> signal_input_connector_clicked ()  .connect (sigc::bind (sigc::mem_fun (this, &RouteGraphPage::on_input_connector_clicked),  window .get ()));
	widget -> signal_output_connector_clicked () .connect (sigc::bind (sigc::mem_fun (this, &RouteGraphPage::on_output_connector_clicked), window .get ()));
	widget -> signal_changed ()                  .connect (sigc::mem_fun (this, &RouteGraphPage::refresh));
	widget -> show ();

	// Put widget on fixed.

	getFixed () .put (*widget, position .x (), position .y ());

	return window;
}

void
RouteGraphPage::removeNode (const X3D::SFNode & node)
{
	const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window -> node == node; });

	if (iter == windows .end ())
		return;

	removeWindow (*iter);
}

void
RouteGraphPage::removeWindow (const RouteGraphWindowPtr window)
{
	const auto iter = std::find (windows .begin (), windows .end (), window);

	if (iter == windows .end ())
		return;

	// Node

	const auto & node = window -> node;

	node -> isLive () .removeInterest (&RouteGraphPage::set_live, this);

	for (const auto & field : node -> getFieldDefinitions ())
	{
		field -> getInputRoutes ()  .removeInterest (&RouteGraphPage::set_routes, this);
		field -> getOutputRoutes () .removeInterest (&RouteGraphPage::set_routes, this);
	}

	// Widget

	getFixed () .remove (*window -> widget);

	windows .erase (iter);
}

void
RouteGraphPage::setPosition (const RouteGraphWindowPtr window, const X3D::Vector2i & position)
{
	const auto & widget = window -> widget;

	getFixed () .move (*widget, position .x (), position .y ());
}

X3D::Vector2i
RouteGraphPage::getPosition (const RouteGraphWindowPtr window) const
{
	const auto & widget = window -> widget;
	const auto   x      = getFixed () .child_property_x (*widget) .get_value ();
	const auto   y      = getFixed () .child_property_y (*widget) .get_value ();

	return X3D::Vector2i (x, y);
}

X3D::Box2i
RouteGraphPage::getBBox (const RouteGraphWindowPtr & window) const
{
	const auto & widget   = window -> widget;
	const auto & position = getPosition (window);
	const auto   width    = widget -> get_width ();
	const auto   height   = widget -> get_height ();

	return X3D::Box2i (position, position + X3D::Vector2i (width, height), X3D::extents_type ());
}

void
RouteGraphPage::setScrollPosition (const X3D::Vector2d & scrollPosition, const bool modify)
{
	getScrolledWindow () .get_hadjustment () -> set_value (scrollPosition .x ());
	getScrolledWindow () .get_vadjustment () -> set_value (scrollPosition .y ());

	if (not modify)
		return;

	createWorldInfo (getCurrentScene ()) -> setMetaData (getMetaKey () + "/scrollPosition", scrollPosition);

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

X3D::Vector2d
RouteGraphPage::getScrollPosition () const
{
	const auto x = getScrolledWindow () .get_hadjustment () -> get_value ();
	const auto y = getScrolledWindow () .get_vadjustment () -> get_value ();

	return X3D::Vector2d (x, y);
}

void
RouteGraphPage::bringToFront (const RouteGraphWindowPtr window)
{
	const auto & widget   = window -> widget;
	const auto & position = getPosition (window);

	getFixed () .remove (*widget);
	getFixed () .put (*widget, position .x (), position .y ());

	windows .erase (std::remove (windows .begin (), windows .end (), window), windows .end ());
	windows .emplace_front (window);
}

bool
RouteGraphPage::isSelected (const RouteGraphWindowPtr & window) const
{
	const auto iter = std::find_if (selection .begin (), selection .end (), [&] (const RouteGraphWindowPtr & selectedWindow) { return window == selectedWindow; });

	return iter not_eq selection .end ();
}

void
RouteGraphPage::addSelection (const RouteGraphWindowArray & windows)
{
	for (const auto & window : windows)
		window -> widget -> setSelected (true);

	selection .insert (selection .end (), windows .begin (), windows .end ());

	if (not windows .empty ())
		bringToFront (windows .back ());

	refresh ();
}

void
RouteGraphPage::setSelection (const RouteGraphWindowArray & windows)
{
	clearSelection ();

	addSelection (windows);
}

void
RouteGraphPage::removeSelection (const RouteGraphWindowArray & windows)
{
	for (const auto & window : windows)
		window -> widget -> setSelected (false);

	const auto iter = std::remove_if (selection .begin (), selection .end (), [&] (const RouteGraphWindowPtr & selectedWindow)
	{
		const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & window) { return window == selectedWindow; });
	
		return iter not_eq windows .end ();
	});

	selection .erase (iter, selection .end ());
	
	refresh ();
}

void
RouteGraphPage::clearSelection ()
{
	for (const auto & window : selection)
		window -> widget -> setSelected (false);

	selection .clear ();

	refresh ();
}

bool
RouteGraphPage::isRouteSelected (const X3D::RoutePtr & route) const
{
	const auto iter = std::find_if (routeSelection .begin (), routeSelection .end (), [&] (const X3D::RoutePtr & selectedRoute) { return route == selectedRoute; });

	return iter not_eq routeSelection .end ();
}

void
RouteGraphPage::addRouteSelection (const RouteArray & routes)
{
	routeSelection .insert (routeSelection .end (), routes .begin (), routes .end ());

	refresh ();
}

void
RouteGraphPage::setRouteSelection (const RouteArray & routes)
{
	clearRouteSelection ();

	addRouteSelection (routes);
}

void
RouteGraphPage::removeRouteSelection (const RouteArray & routes)
{
	const auto iter = std::remove_if (routeSelection .begin (), routeSelection .end (), [&] (const X3D::RoutePtr & selectedRoute)
	{
		const auto iter = std::find_if (routes .begin (), routes .end (), [&] (const X3D::RoutePtr & route) { return route == selectedRoute; });
	
		return iter not_eq routes .end ();
	});

	routeSelection .erase (iter, routeSelection .end ());

	refresh ();
}

void
RouteGraphPage::clearRouteSelection ()
{
	routeSelection .clear ();

	refresh ();
}

void
RouteGraphPage::clearConnectorSelection ()
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
RouteGraphPage::resize ()
{
	auto bbox = X3D::Box2d ();

	for (const auto & window : windows)
		bbox += X3D::Box2d (getBBox (window));

	const auto extents = bbox .extents ();
	const auto max     = extents .second;

	getFixed () .set_size_request (max .x () + GRID_SIZE, max .y () + GRID_SIZE);
}

void
RouteGraphPage::refresh ()
{
	getFixed () .queue_draw ();
}

void
RouteGraphPage::set_live (const RouteGraphWindowPtr window)
{
	removeWindow (window);

	save ();
}

void
RouteGraphPage::set_routes ()
{
	clearRouteSelection ();
	refresh ();
}

void
RouteGraphPage::on_rename_page_activate ()
{
	getPageNameEntry () .set_text (getPageName ());
	getRenamePagePopover () .popup ();
}

void
RouteGraphPage::on_page_name_changed ()
{
	getRenamePageButton () .set_sensitive (getPageNameEntry () .get_text () .length ());
}

bool
RouteGraphPage::on_page_name_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			if (getPageNameEntry () .get_text () .length ())
				on_page_name_rename_clicked ();

			return true;
		}
		case GDK_KEY_Escape:
		{
			getRenamePagePopover () .popdown ();
			return true;
		}
		default:
			break;
	}

	return false;
}

void
RouteGraphPage::on_page_name_rename_clicked ()
{
	const auto text     = getPageNameEntry () .get_text ();
	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Rename Route Graph Logic To »%s«"), text .c_str ()));

	getRenamePagePopover () .popdown ();

	undoStep -> addUndoFunction (&RouteGraphPage::setPageName, this, getPageName (), true);
	undoStep -> addRedoFunction (&RouteGraphPage::setPageName, this, text, true);

	setPageName (text);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
RouteGraphPage::on_align_to_grid_activate ()
{
	for (const auto & window : windows)
	{
		auto position = X3D::Vector2d (getPosition (window)) + SNAP_OFFSET;

		position = X3D::round (position / SNAP_DISTANCE) * SNAP_DISTANCE - SNAP_OFFSET;

		setPosition (window, X3D::Vector2i (position));
	}
}

void
RouteGraphPage::on_export_page_activate ()
{
	try
	{
		static const std::map <int32_t, std::string> magick = {
			std::pair (1, "GRAY"),
			std::pair (3, "RGB"),
			std::pair (4, "RGBA"),
		};

		const auto scrollPosition = getScrollPosition ();

		Gtk::OffscreenWindow window;

		window .set_size_request (getFixed () .get_width (), getFixed () .get_height ());

		getWidget () .remove (getOverlay ());

		window .add (getOverlay ());
		window .show ();

		// This must be done, otherwise the pixbuf is empty.
		while (Gtk::Main::events_pending ())
			Gtk::Main::iteration ();

		const auto pixbuf = window .get_pixbuf ();

		window .remove ();

		getWidget () .pack_start (getOverlay (), true, true);

		// Convert pixbuf to magick image

		if (not pixbuf)
			throw std::runtime_error ("RouteGraphPage::on_export_page_activate: pixbuf is NULL.");

		const auto channels = pixbuf -> get_n_channels ();
		const auto width    = pixbuf -> get_width ();
		const auto height   = pixbuf -> get_height ();
		const auto data     = pixbuf -> get_pixels ();

		Magick::Image image (width, height, magick .at (channels), Magick::CharPixel, data);

		// Save magick image

		const auto dialog = addDialog <FileExportImageDialog> ("FileExportImageDialog", false);

		image .quality (100);

		dialog -> save (image, getPageName () + ".png");

		// Set adjustments.

		setScrollPosition (scrollPosition);
	}
	catch (const std::exception & error)
	{
		// No magick
		__LOG__ << error .what () << std::endl;
	}
}

void
RouteGraphPage::on_delete_activate ()
{
	if (not getSelection () .empty ())
	{
		const auto undoStep = getSelection () .size () > 1
		                      ? std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Nodes From Route Graph Logic »%s«"), getPageName () .c_str ()))
		                      : std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Node From Route Graph Logic »%s«"), getPageName () .c_str ()));
	
		undoStep -> addUndoFunction (&RouteGraphPage::queueSave, this);

		for (const auto & window : getSelection ())
		{
			undoStep -> addUndoFunction (&RouteGraphPage::addNode,    this, window -> node, getPosition (window));
			undoStep -> addRedoFunction (&RouteGraphPage::removeNode, this, window -> node);

			removeWindow (window);
		}

		undoStep -> addRedoFunction (&RouteGraphPage::queueSave, this);

		getBrowserWindow () -> addUndoStep (undoStep);
	}

	if (not getRouteSelection () .empty ())
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Route"));
	
		for (const auto & route : RouteArray (getRouteSelection ()))
		{
			X3D::X3DEditor::deleteRoute (X3D::X3DExecutionContextPtr (route -> getExecutionContext ()),
			                             route -> getSourceNode (),
			                             route -> getSourceField (),
			                             route -> getDestinationNode (),
			                             route -> getDestinationField (),
			                             undoStep);
		}
	
		getBrowserWindow () -> addUndoStep (undoStep);
	}

	clearSelection ();
	clearRouteSelection ();

	queueSave ();
}

void
RouteGraphPage::on_select_all_activate ()
{
	setSelection (windows);

	resize ();
}

void
RouteGraphPage::on_deselect_all_activate ()
{
	clearSelection ();

	resize ();
}

void
RouteGraphPage::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                   int x, int y,
                                   const Gtk::SelectionData & selection_data,
                                   guint info,
                                   guint time)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		if (selection_data .get_data_type () == "titania/node-id")
		{
			try
			{
				// Parse node id.
	
				size_t nodeId = 0;
	
				std::istringstream isstream (selection_data .get_data_as_string ());
	
				isstream .imbue (std::locale::classic ());
	
				isstream >> nodeId;
	
				// Add node widget
	
				int xOffset, yOffset;
			
				getViewport () .translate_coordinates (getFixed (), 0, 0, xOffset, yOffset);

				const auto node     = getNode (nodeId);
				const auto position = X3D::Vector2i (x + xOffset, y + yOffset);

				if (routeGraph -> getAddConnectedNodesMenuItem () .get_active ())
				{
					addConnectedNodes (node, position);
				}
				else
				{
					const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Add Node To Route Graph Logic »%s«"), getPageName () .c_str ()));

					undoStep -> addUndoFunction (&RouteGraphPage::queueSave,  this);
					undoStep -> addUndoFunction (&RouteGraphPage::removeNode, this, node);
					undoStep -> addRedoFunction (&RouteGraphPage::addNode,    this, node, position);
					undoStep -> addRedoFunction (&RouteGraphPage::queueSave,  this);

					addNode (node, position);
					queueSave ();

					getBrowserWindow () -> addUndoStep (undoStep);
				}

				context -> drag_finish (true, false, time);
				return;
			}
			catch (const std::exception & error)
			{
				__LOG__ << error .what () << std::endl;
			}
		}
	}

	context -> drag_finish (false, false, time);
}

bool
RouteGraphPage::on_button_press_event (GdkEventButton* event) 
{
	if (button)
		return false;

	const auto pointer = X3D::Vector2d (event -> x, event -> y);

	button = event -> button;

	// Handle buttons

	if (button == 1)
	{
		// Selected node widget and prepare move.

		// Clear connector selection.
	
		clearConnectorSelection ();

		// Handle route set_selection.
	
		if (on_route_button_press_event (event))
		{
			button = 0;
			return false;
		}

		// Prepare move widget.

		clearRouteSelection ();

		startPosition = pointer;
		endPosition   = pointer;

		for (const auto & window : windows)
		{
			const auto & widget = window -> widget;
			const auto   bbox   = X3D::Box2d (getBBox (window));

			if (not bbox .intersects (startPosition))
				continue;

			motionType = MotionType::SELECTION;

			if (getBrowserWindow () -> getKeys () .shift ())
			{
				if (isSelected (window))
					removeSelection ({ window });
				else
					addSelection ({ window });
			}
			else if (not isSelected (window))
				setSelection ({ window });

			// Prepare move

			widget -> setConnectorsSensitive (false);	

			return true;
		}

		motionType = MotionType::RECTANGLE;

		// Clear selection if clicked on grid.

		if (not getBrowserWindow () -> getKeys () .shift ())
			clearSelection ();

		refresh ();

		return false;
	}
	else if (button == 2)
	{
		// Prepare move area.

		startPosition = pointer;
		return false;
	}

	return false;
}

bool
RouteGraphPage::on_route_button_press_event (GdkEventButton* event) 
{
	const auto pointer = X3D::Vector2d (event -> x, event -> y);

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
					const auto   sourcePosition      = X3D::Vector2d (getPosition (sourceWindow) + sourceWidget -> getOutputPosition (sourceField));
					const auto   destinationPosition = X3D::Vector2d (getPosition (destinationWindow) + destinationWidget -> getInputPosition (destinationField));

					// Intersect with arrow

					if (get_arrow (sourcePosition, destinationPosition) .contains (pointer))
					{
						const auto routeptr = X3D::RoutePtr (route);

						clearSelection ();

						if (getBrowserWindow () -> getKeys () .shift ())
						{
							if (isRouteSelected (routeptr))
								removeRouteSelection ({ routeptr });
							else
								addRouteSelection ({ routeptr });
						}
						else if (not isRouteSelected (routeptr))
							setRouteSelection ({ routeptr });

						return true;
					}
				}
				catch (const std::exception & error)
				{ }
			}
		}
	}

	return false;
}

X3D::Triangle2d
RouteGraphPage::get_arrow (const X3D::Vector2d & sourcePosition, const X3D::Vector2d & destinationPosition) const
{
	// Intersect with arrow

	const auto p1 = X3D::Vector2d (0, 0);
	const auto p2 = X3D::Vector2d (0, 14);
	const auto p3 = X3D::Vector2d (10.0 / 9.0 * 14, 7);
	const auto c  = X3D::negate ((p1 + p2 + p3) / 3.0);
	const auto t  = (sourcePosition + destinationPosition) / 2.0;
	const auto d  = destinationPosition - sourcePosition;
	const auto s  = std::sin (1 / d .x () * math::pi <double> * math::pi <double> / 2) * d .y () / 2.0;
	const auto a  = d .x () ? std::atan2 (1, 1 / s) : math::pi <double> / 2;

	X3D::Matrix3d m;

	m .translate (t);
	m .rotate (d .y () >= 0 ? a : a + math::pi <double>);
	m .translate (c);

	return X3D::Triangle2d (p1 * m, p2 * m, p3 * m);
}

bool
RouteGraphPage::on_button_release_event (GdkEventButton* event) 
{
	if (button == 1)
	{
		for (const auto & window : getSelection ())
		{
			const auto & widget = window -> widget;
	
			widget -> setConnectorsSensitive (true);	
		}
	}

	button     = 0;
	motionType = MotionType::DEFAULT;

	refresh ();

	return false;
}

bool
RouteGraphPage::on_motion_notify_event (GdkEventMotion* event) 
{
	const auto pointer = X3D::Vector2d (event -> x, event -> y);

	if (button == 1)
	{
		// Scroll area if pointer is outside window

		const auto x = getScrolledWindow () .get_hscrollbar () -> get_value ();
		const auto y = getScrolledWindow () .get_vscrollbar () -> get_value ();
		const auto w = getScrolledWindow () .get_width ();
		const auto h = getScrolledWindow () .get_height ();

		if (event -> x < x)
			getScrolledWindow () .get_hscrollbar () -> set_value (event -> x);
		else if (event -> x > x + w)
			getScrolledWindow () .get_hscrollbar () -> set_value (event -> x - w);

		if (event -> y < y)
			getScrolledWindow () .get_vscrollbar () -> set_value (event -> y);
		else if (event -> y > y + h)
			getScrolledWindow () .get_vscrollbar () -> set_value (event -> y - h);

		// Handle motion type

		switch (motionType)
		{
			case MotionType::DEFAULT:
				break;
			case MotionType::SELECTION:
			{
				// Move widget

				auto bbox = X3D::Box2d ();
		
				for (const auto & window : selection)
					bbox += X3D::Box2d (getBBox (window));
		
				const auto translation          = pointer - startPosition;
				const auto extents              = bbox .extents ();
				const auto min                  = extents .first;
				const auto translated           = X3D::max (X3D::Vector2d (), min + translation);
				const auto selectionTranslation = X3D::Vector2i (X3D::round (translated - min));
		
				for (const auto & window : selection)
				{
					auto position = getPosition (window);
		
					position += selectionTranslation;
					position  = X3D::max (X3D::Vector2i (), position);
		
					setPosition (window, position);
				}
		
				// Make the grid a litte larger.
				resize ();
		
				startPosition = pointer;

				Glib::signal_idle () .connect_once (sigc::mem_fun (this, &RouteGraphPage::save));

				break;
			}
			case MotionType::RECTANGLE:
			{
				// Rectangular selection

				endPosition = pointer;

				const auto points             = std::vector <X3D::Vector2d> { startPosition, endPosition };
				const auto rectangleSelection = X3D::Box2d (points .begin (), points .end (), X3D::iterator_type ());
				auto       selection          = getBrowserWindow () -> getKeys () .shift () ? getSelection () : RouteGraphWindowArray ();

				for (const auto & window : windows)
				{
					if (X3D::Box2d (getBBox (window)) .intersects (rectangleSelection))
						selection .emplace_back (window);
				}

				if (getBrowserWindow () -> getKeys () .shift ())
				{
					std::stable_sort (selection .begin (), selection .end ());

					selection .erase (std::unique (selection .begin (), selection .end ()), selection .end ());
				}

				setSelection (selection);

				refresh ();
				break;
			}
		}

		return true;
	}
	else if (button == 2)
	{
		// Move area.

		const auto translation = startPosition - pointer;

		setScrollPosition (getScrollPosition () + translation);

		startPosition = pointer + translation;
		return false;
	}

	return false;
}

bool
RouteGraphPage::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto width  = getFixed () .get_width ();
	const auto height = getFixed () .get_height ();

	// Background

	context -> set_source_rgb (0, 0, 0);
	context -> rectangle (0, 0, width, height);
	context -> fill ();

	// Grid with points

	context -> set_source_rgb (GRID_COLOR, GRID_COLOR, GRID_COLOR);

	for (int32_t y = 0; y < height; y += SMALL_GRID_SIZE)
		context -> rectangle (0, y, width, 1);

	for (int32_t x = 0; x < width; x += SMALL_GRID_SIZE)
		context -> rectangle (x, 0, 1, height);

	context -> fill ();

	// Points

	context -> set_source_rgb (GRID_POINT_COLOR, GRID_POINT_COLOR, GRID_POINT_COLOR);

	for (int32_t y = 0; y < height; y += GRID_SIZE)
	{
		for (int32_t x = 0; x < width; x += GRID_SIZE)
			context -> rectangle (x, y, 1, 1);
	}

	context -> fill ();

	// Draw routes.

	on_draw_routes (context);

	// Draw rectangular selection.

	if (motionType == MotionType::RECTANGLE)
	{
		const auto f    = X3D::make_rgba <double> (210, 225, 252, 43);
		const auto s    = X3D::make_rgba <double> (210, 225, 252, 255);
		const auto size = endPosition - startPosition;

		context -> set_source_rgba (f .r (), f .g (), f .b (), f .a ());
		context -> rectangle (startPosition .x (), startPosition .y (), size .x (), size .y ());
		context -> fill ();

		context -> set_line_width (2);
		context -> set_source_rgba (s .r (), s .g (), s .b (), s .a ());
		context -> rectangle (startPosition .x (), startPosition .y (), size .x (), size .y ());
		context -> stroke ();
	}

	return false;
}

void
RouteGraphPage::on_draw_routes (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto front    = X3D::make_rgb <double> (255, 219, 165);
	const auto back     = X3D::make_rgb <double> (255 / 2, 219 / 2, 165 / 2);
	const auto selected = X3D::make_rgb <double> (224, 0, 21);

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
					const auto   sourcePosition      = X3D::Vector2d (getPosition (sourceWindow) + sourceWidget -> getOutputPosition (sourceField));
					const auto   destinationPosition = X3D::Vector2d (getPosition (destinationWindow) + destinationWidget -> getInputPosition (destinationField));

					// Draw sine curved route, dark or light

					const auto wp = (destinationPosition .x () - sourcePosition .x ()) * 0.5;
					const auto x0 = sourcePosition .x ();
					const auto y0 = sourcePosition .y ();
					const auto x1 = sourcePosition .x () + wp;
					const auto y1 = sourcePosition .y ();
					const auto x2 = destinationPosition .x () - wp;
					const auto y2 = destinationPosition .y ();
					const auto x3 = destinationPosition .x ();
					const auto y3 = destinationPosition .y ();
 
					if (isRouteSelected (X3D::RoutePtr (route)))
					{
						context -> set_source_rgb (selected .r (), selected .g (), selected .b ());
					}
					else
					{
						if (wp > 0)
							context -> set_source_rgb (front .r (), front .g (), front .b ());
						else
							context -> set_source_rgb (back .r (), back .g (), back .b ());
					}

					context -> move_to (x0, y0);
					context -> curve_to (x1, y1, x2, y2, x3, y3);
					context -> stroke ();

					// Draw arrow

					const auto arrow = get_arrow (sourcePosition, destinationPosition);

					context -> move_to (arrow .a () .x (), arrow .a () .y ());
					context -> line_to (arrow .b () .x (), arrow .b () .y ());
					context -> line_to (arrow .c () .x (), arrow .c () .y ());
					context -> close_path ();
					context -> fill ();
				}
				catch (const std::exception & error)
				{ }
			}
		}
	}
}

void
RouteGraphPage::on_select_node_clicked (RouteGraphWindow* const window)
{
	const auto iter = std::find_if (windows .begin (), windows .end (), [&] (const RouteGraphWindowPtr & windowPtr) { return windowPtr .get () == window; });

	if (iter == windows .end ())
		return;

	const X3D::MFNode selection = { window -> node };

	setSelection ({ *iter });
	getBrowserWindow () -> getSelection () -> setNodes (selection);
	getBrowserWindow () -> expandNodes (selection);
}

void
RouteGraphPage::on_input_connector_clicked (X3D::X3DFieldDefinition* const connectorField, RouteGraphWindow* const window)
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
RouteGraphPage::on_output_connector_clicked (X3D::X3DFieldDefinition* const connectorField, RouteGraphWindow* const window)
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

void
RouteGraphPage::store ()
{
	X3DRouteGraphPage::store ();
	X3DRouteGraphPageInterface::store ();
}

RouteGraphPage::~RouteGraphPage ()
{
	dispose ();
}

} // puck
} // titania
