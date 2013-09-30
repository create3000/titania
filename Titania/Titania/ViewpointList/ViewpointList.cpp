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

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

namespace Columns {

constexpr int Index       = 0;
constexpr int Description = 1;
constexpr int Weight      = 2;

};

namespace Weight {

constexpr int Normal = 400;
constexpr int Bold   = 700;

};

ViewpointList::ViewpointList (BrowserWindow* const browserWindow) :
	         X3D::X3DBaseNode (browserWindow -> getBrowser (), browserWindow -> getExecutionContext ()),
	         X3DBaseInterface (browserWindow),
	X3DViewpointListInterface (get_ui ("ViewpointList.ui"), gconf_dir ()),
	              activeLayer ()
{ }

void
ViewpointList::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("ViewpointList"));
}

void
ViewpointList::initialize ()
{
	X3DViewpointListInterface::initialize ();

	getCellRendererDescription () -> property_weight_set () = true;

	getBrowser () -> getActiveLayer () .addInterest (this, &ViewpointList::set_activeLayer);

	set_activeLayer ();
}

const X3D::X3DSFNode <X3D::ViewpointStack> &
ViewpointList::getViewpointStack ()
{
	return activeLayer -> getViewpointStack ();
}

const X3D::X3DSFNode <X3D::ViewpointList> &
ViewpointList::getViewpoints () const
{
	return activeLayer -> getViewpoints ();
}

X3D::UserViewpointList
ViewpointList::getUserViewpoints ()
{
	return activeLayer -> getUserViewpoints ();
}

void
ViewpointList::set_activeLayer ()
{
	if (activeLayer)
	{
		getViewpoints ()     -> bindTime () .removeInterest (this, &ViewpointList::set_viewpoints);
		getViewpointStack () -> bindTime () .removeInterest (this, &ViewpointList::set_currentViewpoint);
	}

	activeLayer = getBrowser () -> getActiveLayer ();

	if (activeLayer)
	{
		getViewpoints ()     -> bindTime () .addInterest (this, &ViewpointList::set_viewpoints);
		getViewpointStack () -> bindTime () .addInterest (this, &ViewpointList::set_currentViewpoint);

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
	getListStore () -> clear ();

	guint index = 0;

	// Fill the TreeView's model
	for (const auto & viewpoint : *getViewpoints ())
	{
		if (viewpoint -> description () .length ())
		{
			auto row = getListStore () -> append ();
			row -> set_value (Columns::Index,       index);
			row -> set_value (Columns::Description, Glib::Markup::escape_text (viewpoint -> description ()));
			row -> set_value (Columns::Weight,      viewpoint -> isBound () .getValue () ? Weight::Bold : Weight::Normal);

			getListStore () -> row_changed (getListStore () -> get_path (row), row);
		}

		++ index;
	}
}

void
ViewpointList::set_currentViewpoint ()
{
	// Update list store

	auto userViewpoints = getUserViewpoints ();
	auto rows           = getListStore () -> children ();

	for (size_t i = 0, size = rows .size (); i < size; ++ i)
	{
		rows [i] -> set_value (Columns::Weight, userViewpoints [i] -> isBound () ? Weight::Bold : Weight::Normal);

		getListStore () -> row_changed (getListStore () -> get_path (rows [i]), rows [i]);
	}
}

void
ViewpointList::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	guint index;

	getListStore () -> get_iter (path) -> get_value (Columns::Index, index);

	auto viewpoint = getViewpoints () -> at (index);

	if (viewpoint -> isBound ())
		viewpoint -> transitionStart (viewpoint);

	else
		viewpoint -> set_bind () = true;

}

ViewpointList::~ViewpointList ()
{ }

} // puck
} // titania
