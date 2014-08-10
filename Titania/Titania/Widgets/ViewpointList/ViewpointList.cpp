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

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

namespace Columns {

static constexpr int INDEX       = 0;
static constexpr int NAME        = 1;
static constexpr int DESCRIPTION = 2;
static constexpr int TYPE_NAME   = 3;
static constexpr int WEIGHT      = 4;
static constexpr int STYLE       = 5;

};

namespace Weight {

static constexpr int NORMAL = 400;
static constexpr int BOLD   = 700;

};

ViewpointList::ViewpointList (BrowserWindow* const browserWindow, const bool label) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DViewpointListInterface (get_ui ("ViewpointList.xml"), gconf_dir ()),
	                    label (label),
	           userViewpoints (true),
	              activeLayer ()
{
	setup ();
}

void
ViewpointList::on_map ()
{
	if (label)
		getBrowserWindow () -> getSideBarLabel () .set_text (_ ("Viewpoint List"));
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

	getBrowser () -> getActiveLayer () .addInterest (this, &ViewpointList::set_activeLayer);

	set_activeLayer ();
}

void
ViewpointList::setUserViewpoints (const bool value)
{
	userViewpoints = value;

	getNameColumn ()     -> set_visible (not userViewpoints);
	getTypeNameColumn () -> set_visible (not userViewpoints);
}

const X3D::ViewpointStackPtr &
ViewpointList::getViewpointStack ()
{
	return activeLayer -> getViewpointStack ();
}

const X3D::ViewpointListPtr &
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
		getViewpoints ()     -> removeInterest (this, &ViewpointList::set_viewpoints);
		getViewpointStack () -> removeInterest (this, &ViewpointList::set_currentViewpoint);
	}

	activeLayer = getBrowser () -> getActiveLayer ();

	if (activeLayer)
	{
		getViewpoints ()     -> addInterest (this, &ViewpointList::set_viewpoints);
		getViewpointStack () -> addInterest (this, &ViewpointList::set_currentViewpoint);

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

	// Fill the TreeView's model

	guint index = 1;

	for (const auto & viewpoint : std::make_pair (getViewpoints () -> begin () + 1, getViewpoints () -> end ()))
	{
		if (userViewpoints and not viewpoint -> description () .length ())
			continue;

		auto name = viewpoint -> getName ();

		X3D::RegEx::LastNumber_ .Replace ("", &name);

		const auto row = getListStore () -> append ();
		row -> set_value (Columns::INDEX,       index);
		row -> set_value (Columns::NAME       , name);
		row -> set_value (Columns::DESCRIPTION, viewpoint -> description () .str ());
		row -> set_value (Columns::TYPE_NAME,   viewpoint -> getTypeName ());

		if (viewpoint -> isBound ())
		{
			row -> set_value (Columns::WEIGHT, Weight::BOLD);
			row -> set_value (Columns::STYLE,  Pango::STYLE_ITALIC);
		}
		else
		{
			row -> set_value (Columns::WEIGHT, Weight::NORMAL);
			row -> set_value (Columns::STYLE,  Pango::STYLE_NORMAL);
		}

		++ index;
	}
}

void
ViewpointList::set_currentViewpoint ()
{
	// Update list store

	if (userViewpoints)
	{
		const auto viewpoints = getUserViewpoints ();
		const auto rows       = getListStore () -> children ();

		for (size_t i = 0, size = rows .size (); i < size; ++ i)
		{
			if (viewpoints [i] -> isBound ())
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
	else
	{
		const auto & viewpoints = *getViewpoints ();
		const auto   rows       = getListStore () -> children ();

		for (size_t i = 0, size = rows .size (); i < size; ++ i)
		{
			if (viewpoints [i + 1] -> isBound ())
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

	const auto viewpoint = getViewpoints () -> at (index);

	if (viewpoint -> isBound ())
		viewpoint -> transitionStart (viewpoint);

	else
		viewpoint -> set_bind () = true;
}

ViewpointList::~ViewpointList ()
{
	dispose ();
}

} // puck
} // titania
