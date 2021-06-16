/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_NOTEBOOK_ROUTE_GRAPH_ROUTE_GRAPH_PAGE_H__
#define __TITANIA_BROWSER_NOTEBOOK_ROUTE_GRAPH_ROUTE_GRAPH_PAGE_H__

#include "../../UserInterfaces/X3DRouteGraphPageInterface.h"

#include "X3DRouteGraphPage.h"

namespace titania {
namespace puck {

class RouteGraph;
class RouteGraphNode;

class RouteGraphPage :
	virtual public X3DRouteGraphPageInterface,
	public X3DRouteGraphPage
{
public:

	///  @name Construction

	RouteGraphPage (X3DBrowserWindow* const browserWindow,
	                RouteGraph* const routeGraph,
	                const std::string & pageName);

	///  @name Member access

	int32_t
	getPageNumber () const;

	void
	setPageName (const std::string & value, const bool modify = true);

	const std::string &
	getPageName () const
	{ return pageName; }

	std::string
	getMetaKey () const;

	///  @name Operations

	void
	open ();

	void
	save ();

	///  @name Event handlers

	void
	on_rename_page_activate ();

	void
	on_align_to_grid_activate ();

	void
	on_export_page_activate ();

	void
	on_delete_activate ();

	void
	on_select_all_activate ();

	void
	on_deselect_all_activate ();

	///  @name Destruction

	virtual
	~RouteGraphPage () final override;


protected:

	///  @name Construction

	virtual
	void
	configure () final override;

	virtual
	void
	initialize () final override;

	virtual
	void
	store () final override;


private:

	///  @name Member types

	using WidgetPtr = std::shared_ptr <RouteGraphNode>;

	struct RouteGraphWindow
	{
		X3D::SFNode node;
		WidgetPtr   widget;
	};

	using RouteGraphWindowPtr   = std::shared_ptr <RouteGraphWindow>;
	using RouteGraphWindowArray = std::deque <RouteGraphWindowPtr>;
	using RouteArray            = std::deque <X3D::RoutePtr>;

	enum class MotionType
	{
		DEFAULT,
		SELECTION,
		RECTANGLE
	};

	///  @name Operations

	void
	queueSave ();

	X3D::SFNode
	getNode (const size_t id) const;

	void
	addConnectedNodes (const X3D::SFNode & node, const X3D::Vector2i & position);

	void
	getConnectedNodes (X3D::X3DExecutionContext* const executionContext,
	                   const X3D::SFNode & node,
	                   const int32_t index,
	                   std::map <int32_t, X3D::MFNode> & columns,
	                   std::set <X3D::SFNode> & nodes) const;

	void
	on_place_nodes (const std::map <int32_t, X3D::MFNode> & columns, const X3D::Vector2i & position);

	RouteGraphWindowPtr
	addNode (const X3D::SFNode & node, const X3D::Vector2i & position);

	void
	removeNode (const X3D::SFNode & node);

	void
	removeWindow (const RouteGraphWindowPtr window);

	RouteGraphWindowPtr
	getWindow (const X3D::SFNode & node) const;

	void
	setPosition (const RouteGraphWindowPtr window, const X3D::Vector2i & position);

	X3D::Vector2i
	getPosition (const RouteGraphWindowPtr window) const;

	X3D::Box2i
	getBBox (const RouteGraphWindowPtr & window) const;

	void
	setScrollPosition (const X3D::Vector2d & scrollPosition, const bool modify = true);
	
	X3D::Vector2d
	getScrollPosition () const;

	void
	bringToFront (const RouteGraphWindowPtr window);

	bool
	isSelected (const RouteGraphWindowPtr & window) const;

	const RouteGraphWindowArray &
	getSelection () const
	{ return selection; }

	void
	addSelection (const RouteGraphWindowArray & windows);

	void
	removeSelection (const RouteGraphWindowArray & windows);

	void
	setSelection (const RouteGraphWindowArray & windows);

	void
	clearSelection ();

	bool
	isRouteSelected (const X3D::RoutePtr & route) const;

	const RouteArray &
	getRouteSelection () const
	{ return routeSelection; }

	void
	addRouteSelection (const RouteArray & routes);
	
	void
	setRouteSelection (const RouteArray & routes);
	
	void
	removeRouteSelection (const RouteArray & routes);
	
	void
	clearRouteSelection ();

	void
	clearConnectorSelection ();

	void
	resize ();

	void
	refresh ();

	///  @name Event handlers

	void
	set_live (const RouteGraphWindowPtr window);

	void
	set_routes ();
	
	virtual
	void
	on_page_name_changed () final override;

	virtual
	bool
	on_page_name_key_press_event (GdkEventKey* event) final override;

	virtual
	void
	on_page_name_rename_clicked () final override;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
	                       int x, int y,
	                       const Gtk::SelectionData & selection_data,
	                       guint info,
	                       guint time) final override;

	virtual
	bool
	on_button_press_event (GdkEventButton* event) final override;

	bool
	on_route_button_press_event (GdkEventButton* event);

	X3D::Triangle2d
	get_arrow (const X3D::Vector2d & sourcePosition, const X3D::Vector2d & destinationPosition) const;

	virtual
	bool
	on_button_release_event (GdkEventButton* event) final override;
	
	virtual
	bool
	on_motion_notify_event (GdkEventMotion* event) final override;

	virtual
	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> & context) final override;

	void
	on_draw_routes (const Cairo::RefPtr <Cairo::Context> & context);

	void
	on_select_node_clicked (RouteGraphWindow* const window);

	void
	on_input_connector_clicked (X3D::X3DFieldDefinition* const field, RouteGraphWindow* const window);

	void
	on_output_connector_clicked (X3D::X3DFieldDefinition* const field, RouteGraphWindow* const window);

	///  @name Members

	RouteGraph* const           routeGraph;
	RouteGraphWindowArray       windows;
	RouteGraphWindowArray       selection;
	RouteArray                  routeSelection;
	int32_t                     button;
	X3D::Vector2d               position;
	X3D::Vector2d               startPosition;
	X3D::Vector2d               endPosition;
	MotionType                  motionType;
	std::string                 pageName;
	bool                        inputConnectorClicked;
	bool                        outputConnectorClicked;
	X3D::X3DExecutionContextPtr matchingContext;
	X3D::SFNode                 routeNode;
	X3D::X3DFieldDefinition*    routeField;

};

} // puck
} // titania

#endif
