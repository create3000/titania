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

#ifndef __TITANIA_BROWSER_NOTEBOOK_ROUTE_GRAPH_ROUTE_GRAPH_H__
#define __TITANIA_BROWSER_NOTEBOOK_ROUTE_GRAPH_ROUTE_GRAPH_H__

#include "../../UserInterfaces/X3DRouteGraphInterface.h"

#include "X3DRouteGraph.h"

namespace titania {
namespace puck {

class RouteGraphNode;

class RouteGraph :
	virtual public X3DRouteGraphInterface,
	public X3DRouteGraph
{
public:

	///  @name Construction

	RouteGraph (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id);

	///  @name Destruction

	virtual
	~RouteGraph () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Member types

	using WidgetPtr = std::shared_ptr <RouteGraphNode>;

	struct RouteGraphWindow
	{
		X3D::SFNode   node;
		WidgetPtr     widget;
		X3D::Vector2i position;
	};

	using RouteGraphWindowPtr   = std::shared_ptr <RouteGraphWindow>;
	using RouteGraphWindowArray = std::deque <RouteGraphWindowPtr>;

	///  @name Operations

	X3D::SFNode
	getNode (const size_t id) const
	throw (std::runtime_error);

	void
	addWindow (const X3D::SFNode & node, const X3D::Vector2i & position);

	RouteGraphWindowPtr
	getWindow (const X3D::SFNode & node) const;

	void
	setPosition (const RouteGraphWindowPtr window, const X3D::Vector2i & position);

	X3D::Box2i
	getBBox (const RouteGraphWindowPtr & window) const;

	void
	bringToFront (const RouteGraphWindowPtr window);

	bool
	isSelected (const RouteGraphWindowPtr & window) const;

	void
	addSelection (const RouteGraphWindowArray & windows);

	void
	removeSelection (const RouteGraphWindowArray & windows);

	void
	setSelection (const RouteGraphWindowArray & windows);

	void
	clearSelection ();

	void
	clearConnectorSelection ();

	void
	resizeGrid ();

	void
	refresh ();

	///  @name Event handlers

	virtual
	void
	on_rename_sheet_activate () final override;

	virtual
	void
	on_align_to_grid_activate () final override;

	virtual
	void
	on_export_sheet_activate () final override;

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
	on_input_connector_clicked (X3D::X3DFieldDefinition* const field, RouteGraphWindow* const window);

	void
	on_output_connector_clicked (X3D::X3DFieldDefinition* const field, RouteGraphWindow* const window);

	///  @name Members

	RouteGraphWindowArray       windows;
	RouteGraphWindowArray       selection;
	int32_t                     button;
	X3D::Vector2d               position;
	X3D::Vector2d               startPosition;
	std::string                 sheetName;
	bool                        inputConnectorClicked;
	bool                        outputConnectorClicked;
	X3D::X3DExecutionContextPtr matchingContext;
	X3D::SFNode                 routeNode;
	X3D::X3DFieldDefinition*    routeField;

};

} // puck
} // titania

#endif
