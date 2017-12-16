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

#include "X3DDialogInterface.h"

#include "../Browser/X3DBrowserWindow.h"

namespace titania {
namespace puck {

X3DDialogInterface::X3DDialogInterface () :
	X3DUserInterface (),
	       expanders ()
{ }

void
X3DDialogInterface::setup ()
{
	X3DUserInterface::setup ();

	if (&getWindow () not_eq &getBrowserWindow () -> getWindow ())
		getWindow () .set_transient_for (getBrowserWindow () -> getWindow ());
}

void
X3DDialogInterface::initialize ()
{
	X3DUserInterface::initialize ();

	restoreExpanders (getWidget ());
	setGridLabels (getWidget ());
}

void
X3DDialogInterface::restoreExpanders (Gtk::Widget & widget)
{
	expanders = getWidgets <Gtk::Expander> (widget);

	for (auto & expander : expanders)
	{
		if (expander -> get_name () .empty ())
			continue;

		if (not getConfig () -> hasItem (expander -> get_name ()))
			continue;

		expander -> set_expanded (getConfig () -> getItem <bool> (expander -> get_name ()));

		expander -> property_expanded () .signal_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DDialogInterface::on_expander_expanded), expander));
	}
}

void
X3DDialogInterface::storeExpanders ()
{
	for (auto & expander : expanders)
	{
		if (not expander -> get_name () .empty ())
			getConfig () -> setItem (expander -> get_name (), expander -> get_expanded ());
	}
}

void
X3DDialogInterface::on_expander_expanded (Gtk::Expander* const expander)
{
	expander -> get_child () -> set_visible (expander -> get_expanded ());
}

void
X3DDialogInterface::setGridLabels (Gtk::Widget & widget)
{
	std::vector <Gtk::Label*> labels;

	getLabels (&widget, labels);

	int width = 0;

	for (auto & label : labels)
	{
		const auto layout = label -> create_pango_layout (label -> get_text ());
		
		int w = 0;
		int h = 0;

		layout -> get_pixel_size (w, h);

		width = std::max (width, w);
	}

	for (auto & label : labels)
	{
		float xalign = 0;
		float yalign = 0;

		label -> set_size_request (width, -1);
		label -> get_alignment (xalign, yalign);
		label -> set_alignment (1, yalign);
	}
}

void
X3DDialogInterface::getLabels (Gtk::Widget* const widget, std::vector <Gtk::Label*> & labels)
{
	if (not widget)
		return;

	const auto grid = dynamic_cast <Gtk::Grid*> (widget);

	if (grid)
	{
		for (auto & child : grid -> get_children ())
		{
			const auto label = dynamic_cast <Gtk::Label*> (child);

			if (label)
				labels .emplace_back (label);
		}

		return;
	}

	const auto container = dynamic_cast <Gtk::Container*> (widget);

	if (container)
	{
		for (auto & child : container -> get_children ())
			getLabels (child, labels);

		return;
	}
}

void
X3DDialogInterface::store ()
{
	storeExpanders ();

	X3DUserInterface::store ();
}

void
X3DDialogInterface::dispose ()
{
	X3DUserInterface::dispose ();
}

X3DDialogInterface::~X3DDialogInterface ()
{ }

} // puck
} // titania
