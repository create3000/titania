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

#include "X3DRevealerInterface.h"

#include "../Browser/X3DBrowserWindow.h"

namespace titania {
namespace puck {

X3DRevealerInterface::X3DRevealerInterface () :
	X3DUserInterface (),
	 X3DEditorObject (),
	        position (),
	         pointer ()
{ }

void
X3DRevealerInterface::setup ()
{
	X3DUserInterface::setup ();
	X3DEditorObject::setup ();

	getWidget () .property_reveal_child ()   .signal_changed () .connect (sigc::mem_fun (this, &X3DRevealerInterface::on_reveal_child));
	getWidget () .property_child_revealed () .signal_changed () .connect (sigc::mem_fun (this, &X3DRevealerInterface::on_child_revealed));

	getWidget () .signal_map ()           .connect (sigc::mem_fun (this, &X3DRevealerInterface::on_map));
	getWidget () .signal_size_allocate () .connect (sigc::mem_fun (this, &X3DRevealerInterface::on_size_allocate));
}

void
X3DRevealerInterface::initialize ()
{
	X3DUserInterface::initialize ();
	X3DEditorObject::initialize ();
}

void
X3DRevealerInterface::configure ()
{
	X3DUserInterface::configure ();

	const auto boxWidth       = getBrowserWindow () -> getBrowserOverlay () .get_width ();
	const auto boxHeight      = getBrowserWindow () -> getBrowserOverlay () .get_height ();
	const auto defaultMarigin = X3D::Vector2d (boxWidth / 2, boxHeight / 2);
	const auto margin         = getConfig () -> getItem <X3D::Vector2d> ("margin", defaultMarigin);

	getWidget () .set_margin_left (margin .x ());
	getWidget () .set_margin_top  (margin .y ());
}

void
X3DRevealerInterface::on_map ()
{ }

void
X3DRevealerInterface::on_size_allocate (Gtk::Allocation & allocation)
{
	const auto boxWidth  = getBrowserWindow () -> getBrowserOverlay () .get_width ();
	const auto boxHeight = getBrowserWindow () -> getBrowserOverlay () .get_height ();

	getWidget () .set_margin_left (std::clamp <double> (getWidget () .get_margin_left (), 0, boxWidth  - getWidget () .get_width  ()));
	getWidget () .set_margin_top  (std::clamp <double> (getWidget () .get_margin_top  (), 0, boxHeight - getWidget () .get_height ()));
}

void
X3DRevealerInterface::on_reveal_child ()
{
	if (getWidget () .get_reveal_child ())
		getWidget () .set_visible (true);
}

void
X3DRevealerInterface::on_child_revealed ()
{
	if (not getWidget () .get_reveal_child ())
		getWidget () .set_visible (false);
}

bool
X3DRevealerInterface::on_title_button_press_event (GdkEventButton* event, Gtk::Button &)
{
	int x, y;

	getWidget () .translate_coordinates (getBrowserWindow () -> getWidget (), event -> x, event -> y, x, y);

	position = X3D::Vector2d (getWidget () .get_margin_left (), getWidget () .get_margin_top ());
	pointer  = X3D::Vector2d (x, y);
	return true;
}

bool
X3DRevealerInterface::on_title_button_release_event (GdkEventButton* event, Gtk::Button &)
{
	// Prevent click.
	return true;
}

bool
X3DRevealerInterface::on_title_button_motion_notify_event (GdkEventMotion* event, Gtk::Button &)
{
	int x, y;

	getWidget () .translate_coordinates (getBrowserWindow () -> getWidget (), event -> x, event -> y, x, y);

	const auto boxWidth  = getBrowserWindow () -> getBrowserOverlay () .get_width ();
	const auto boxHeight = getBrowserWindow () -> getBrowserOverlay () .get_height ();
	const auto margin    = position + X3D::Vector2d (x, y) - pointer;

	getWidget () .set_margin_left (std::clamp <double> (margin .x (), 0, boxWidth  - getWidget () .get_width ()));
	getWidget () .set_margin_top  (std::clamp <double> (margin .y (), 0, boxHeight - getWidget () .get_height ()));
	return true;
}

void
X3DRevealerInterface::store ()
{
	getConfig () -> setItem ("margin", X3D::Vector2d (getWidget () .get_margin_left (), getWidget () .get_margin_top ()));

	X3DUserInterface::store ();
}

///  @name Destruction

void
X3DRevealerInterface::dispose ()
{
	X3DEditorObject::dispose ();
	X3DUserInterface::dispose ();
}

X3DRevealerInterface::~X3DRevealerInterface ()
{ }

} // puck
} // titania
