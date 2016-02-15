/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "ViewpointList.h"

#include "../../Base/AdjustmentObject.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Execution/BindableNodeList.h>
#include <Titania/X3D/Parser/RegEx.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int INDEX       = 0;
static constexpr int NAME        = 1;
static constexpr int DESCRIPTION = 2;
static constexpr int TYPE_NAME   = 3;
static constexpr int WEIGHT      = 4;
static constexpr int STYLE       = 5;
static constexpr int BIND        = 6;

};

namespace Weight {

static constexpr int NORMAL = 400;
static constexpr int BOLD   = 700;

};

ViewpointList::ViewpointList (X3DBrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DViewpointListInterface (get_ui ("ViewpointList.glade"), gconf_dir ()),
	                  browser (getCurrentBrowser ()),
	              activeLayer (),
	                   editor (false),
	              hadjustment (new AdjustmentObject ()),
	              vadjustment (new AdjustmentObject ())
{
	addChildren (browser, activeLayer);

	setup ();
}

void
ViewpointList::initialize ()
{
	X3DViewpointListInterface::initialize ();

	getNameCellRenderer ()        -> property_weight_set () = true;
	getDescriptionCellRenderer () -> property_weight_set () = true;
	getTypeNameCellRenderer ()    -> property_weight_set () = true;

	getNameCellRenderer ()        -> property_style_set () = true;
	getDescriptionCellRenderer () -> property_style_set () = true;
	getTypeNameCellRenderer ()    -> property_style_set () = true;

	getCurrentBrowser () .addInterest (this, &ViewpointList::set_browser);

	set_browser (getCurrentBrowser ());
}

void
ViewpointList::isEditor (const bool value)
{
	editor = value;

	getNameColumn ()     -> set_visible (editor);
	getTypeNameColumn () -> set_visible (editor);
	getPadColumn ()      -> set_visible (editor);
	getBindColumn ()     -> set_visible (editor);
}

const X3D::ViewpointStackPtr &
ViewpointList::getStack ()
{
	return activeLayer -> getViewpointStack ();
}

const X3D::ViewpointListPtr &
ViewpointList::getList () const
{
	return activeLayer -> getViewpoints ();
}

void
ViewpointList::set_browser (const X3D::BrowserPtr & value)
{
	browser -> getActiveLayer () .removeInterest (this, &ViewpointList::set_activeLayer);

	browser = value;

	browser -> getActiveLayer () .addInterest (this, &ViewpointList::set_activeLayer);

	set_activeLayer ();
}

void
ViewpointList::set_activeLayer ()
{
	if (activeLayer)
	{
		getList ()  -> removeInterest (this, &ViewpointList::set_viewpoints);
		getStack () -> removeInterest (this, &ViewpointList::set_currentViewpoint);
	}

	activeLayer = browser -> getActiveLayer ();

	if (activeLayer)
	{
		getList ()  -> addInterest (this, &ViewpointList::set_viewpoints);
		getStack () -> addInterest (this, &ViewpointList::set_currentViewpoint);

		set_viewpoints ();
		set_currentViewpoint ();
	}
	else
		getListStore () -> clear ();
}

void
ViewpointList::set_viewpoints ()
{
	// Clear

	hadjustment -> preserve (getTreeView () .get_hadjustment ());
	vadjustment -> preserve (getTreeView () .get_vadjustment ());

	getTreeView () .unset_model ();
	getListStore () -> clear ();

	// Fill the TreeView's model

	const auto & list = getList ();

	if (not list -> empty ())
	{
		for (size_t i = 0, size = list -> size (); i < size; ++ i)
		{
		   const auto & viewpoint = list -> at (i);

			if (not editor and viewpoint -> description () .empty ())
			   continue;

			auto name = viewpoint -> getName ();

			X3D::RegEx::LastNumber_ .Replace ("", &name);

			const auto row = getListStore () -> append ();
			row -> set_value (Columns::INDEX,       i);
			row -> set_value (Columns::NAME       , name);
			row -> set_value (Columns::DESCRIPTION, i ? viewpoint -> description () .str () : _ ("Default Viewpoint"));
			row -> set_value (Columns::TYPE_NAME,   viewpoint -> getTypeName ());

			if (viewpoint -> isBound ())
			{
				row -> set_value (Columns::BIND,   std::string ("Bound"));
				row -> set_value (Columns::WEIGHT, Weight::BOLD);
				row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
			}
			else
			{
				row -> set_value (Columns::BIND,   std::string ("Bind"));
				row -> set_value (Columns::WEIGHT, Weight::NORMAL);
				row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
			}
		}
	}

	getTreeView () .set_model (getListStore ());
	getTreeView () .set_search_column (Columns::DESCRIPTION);

	processInterests ();
}

void
ViewpointList::set_currentViewpoint ()
{
	// Update list store

	const auto & list = getList ();
	const auto   rows = getListStore () -> children ();

	if (editor)
	{
		for (size_t i = 0, size = list -> size (); i < size; ++ i)
		{
		   const auto & node = list -> at (i);
		   const auto & row  = rows [i];

			if (node -> isBound ())
			{
				row -> set_value (Columns::BIND,   std::string ("Bound"));
				row -> set_value (Columns::WEIGHT, Weight::BOLD);
				row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
			}
			else
			{
				row -> set_value (Columns::BIND,   std::string ("Bind"));
				row -> set_value (Columns::WEIGHT, Weight::NORMAL);
				row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
			}

			getListStore () -> row_changed (getListStore () -> get_path (row), row);
		}
	}
	else
	{
		for (size_t i = 0, size = list -> size (); i < size; ++ i)
		{
		   const auto & node = list -> at (i);

			if (node -> description () .empty ())
			   continue;

			if (node -> isBound ())
			{
				rows [i] -> set_value (Columns::WEIGHT, Weight::BOLD);
				rows [i] -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
			}
			else
			{
				rows [i] -> set_value (Columns::WEIGHT, Weight::NORMAL);
				rows [i] -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
			}

			getListStore () -> row_changed (getListStore () -> get_path (rows [i]), rows [i]);
		}
	}
}

void
ViewpointList::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	guint index;

	getListStore () -> get_iter (path) -> get_value (Columns::INDEX, index);

	const auto node = getList () -> at (index);

	if (node -> isBound ())
		node -> transitionStart (node);

	else
		node -> set_bind () = true;
}

bool
ViewpointList::on_button_release_event (GdkEventButton* event)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	const auto row = getListStore () -> get_iter (path);

	if (getListStore () -> iter_is_valid (row))
	{
		if (column == getBindColumn () .operator -> ())
		{
			on_bind_toggled (path);
			return true;
		}
	}

	return false;
}

void
ViewpointList::on_bind_toggled (const Gtk::TreePath & path)
{
	// Get Node

	const auto node = getList () -> at (path [0]);

	if (node -> isBound ())
	   return;

	// Change Bind State Of Node

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Bind State Of Node"));

	node -> set_bind () = true;

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointList::set_adjustment (const Glib::RefPtr <Gtk::Adjustment> & adjustment, const double value)
{
	adjustment -> set_value (value);
}

ViewpointList::~ViewpointList ()
{
	dispose ();
}

} // puck
} // titania
