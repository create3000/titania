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

class RouteGraphPage;

using RouteGraphPagePtr = std::shared_ptr <RouteGraphPage>;

class RouteGraph :
	virtual public X3DRouteGraphInterface,
	public X3DRouteGraph
{
public:

	///  @name Construction

	RouteGraph (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~RouteGraph () final override;


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

	///  @name operations

	void
	setCurrentPage (const size_t pageNumber, const bool modify = true);

	RouteGraphPagePtr
	getCurrentPage () const
	{ return currentPage; }

	RouteGraphPagePtr
	createPage ();

	RouteGraphPagePtr
	appendPage (const std::string & pageName);

	void
	closePage (const RouteGraphPagePtr page);

	void
	savePages ();

	///  @name Event handlers

	void
	set_scene ();

	virtual
	void
	on_new_page_activate () final override;

	virtual
	void
	on_rename_page_activate () final override;

	virtual
	void
	on_align_to_grid_activate () final override;

	virtual
	void
	on_export_page_activate () final override;

	virtual
	void
	on_delete_activate () final override;

	virtual
	void
	on_select_all_activate () final override;

	virtual
	void
	on_deselect_all_activate () final override;

	virtual
	void
	on_close_page_activate () final override;


	virtual
	void
	on_new_page_clicked () final override;

	virtual
	void
	on_rename_page_clicked () final override;

	virtual
	void
	on_delete_clicked () final override;

	virtual
	void
	on_close_page_clicked () final override;

	virtual
	void
	on_switch_page (Gtk::Widget*, guint pageNumber) final override;

	virtual
	void
	on_page_reordered (Gtk::Widget* widget, guint pageNumber) final override;

	///  @name Members

	std::vector <RouteGraphPagePtr> pages;
	RouteGraphPagePtr               currentPage;
	bool                            changing;

};

} // puck
} // titania

#endif
