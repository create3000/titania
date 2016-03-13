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

#include "X3DOverlayInterface.h"

namespace titania {
namespace puck {

X3DOverlayInterface::X3DOverlayInterface (const std::string & widgetName) :
	X3DEditorInterface (widgetName),
	          position (),
	           pointer ()
{ }

X3DOverlayInterface::~X3DOverlayInterface ()
{ }

void
X3DOverlayInterface::configure ()
{
	X3DEditorInterface::configure ();

	const auto margin = getConfig () -> get <X3D::Vector2d> ("margin");
	getWidget () .set_margin_left (margin .x ());
	getWidget () .set_margin_top  (margin .y ());
}

void
X3DOverlayInterface::initialize ()
{
	X3DEditorInterface::initialize ();

	getWidget () .signal_map () .connect (sigc::mem_fun (*this, &X3DOverlayInterface::on_map));
}
	           	          
void
X3DOverlayInterface::on_map ()
{
	getWidget () .set_margin_left (X3D::clamp <double> (getWidget () .get_margin_left (), 0, getCurrentBrowser () -> get_width  () - getWidget () .get_width  ()));
	getWidget () .set_margin_top  (X3D::clamp <double> (getWidget () .get_margin_top  (), 0, getCurrentBrowser () -> get_height () - getWidget () .get_height ()));
}

bool
X3DOverlayInterface::on_title_button_press_event (GdkEventButton* event, Gtk::Button &)
{
	int x, y;

	getWidget () .translate_coordinates (getBrowserWindow () -> getWidget (), event -> x, event -> y, x, y);

	position = X3D::Vector2d (getWidget () .get_margin_left (), getWidget () .get_margin_top ());
	pointer  = X3D::Vector2d (x, y);
	return true;
}

bool
X3DOverlayInterface::on_title_button_release_event (GdkEventButton* event, Gtk::Button &)
{
	// Prevent click.
	return true;
}

bool
X3DOverlayInterface::on_title_button_motion_notify_event (GdkEventMotion* event, Gtk::Button &)
{
	int x, y;

	getWidget () .translate_coordinates (getBrowserWindow () -> getWidget (), event -> x, event -> y, x, y);

	const auto margin = position + X3D::Vector2d (x, y) - pointer;

	getWidget () .set_margin_left (X3D::clamp <double> (margin .x (), 0, getCurrentBrowser () -> get_width  () - getWidget () .get_width ()));
	getWidget () .set_margin_top  (X3D::clamp <double> (margin .y (), 0, getCurrentBrowser () -> get_height () - getWidget () .get_height ()));
	return true;
}

void
X3DOverlayInterface::store ()
{
	X3DEditorInterface::store ();

	getConfig () -> set ("margin", X3D::Vector2d (getWidget () .get_margin_left (), getWidget () .get_margin_top ()));
}

} // puck
} // titania
