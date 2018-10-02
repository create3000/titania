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

#include "X3DPanelInterface.h"

#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../BrowserNotebook/PanelMenu/PanelMenu.h"

#include <cassert>

namespace titania {
namespace puck {

X3DPanelInterface::X3DPanelInterface (NotebookPage* const page, const PanelType panelType, const size_t id) :
	X3DUserInterface (),
	            page (page),
	              id (id),
	       panelMenu (new PanelMenu (getBrowserWindow (), page, panelType)),
	           focus (false),
	    focusWidgets ()
{
	assert (page);

	addChildObjects (focus);
}

X3DPanelInterface::X3DPanelInterface () :
	X3DPanelInterface (nullptr, PanelType::BROWSER_PANEL, 0)
{ }

void
X3DPanelInterface::initialize ()
{
	X3DUserInterface::initialize ();

	hasFocus () .addInterest (&X3DPanelInterface::set_focus, this);

	getWidget () .get_style_context () -> add_class ("titania-widget-box");

	getPanelsMenuItem () .set_submenu (panelMenu -> getWidget ());

	for (const auto widget : getWidgets <Gtk::Widget> (getWidget ()))
		addFocusWidget (widget);

	set_focus ();
}

const X3D::SFEnum <PanelType> &
X3DPanelInterface::getPanelType () const
{
	return panelMenu -> getPanelType ();
}

void
X3DPanelInterface::setFocus (const bool value)
{
	if (value == focus)
		return;

	focus = value;
}

void
X3DPanelInterface::addFocusWidget (Gtk::Widget* const widget)
{
	// Be aware that widget must not always be valid, and can only be used as key.

	const auto connection = widget -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DPanelInterface::on_button_press_event), false);

	widget -> add_events (Gdk::BUTTON_PRESS_MASK);

	focusWidgets .emplace (widget, connection);
}

void
X3DPanelInterface::removeFocusWidget (Gtk::Widget* const widget)
{
	const auto iter = focusWidgets .find (widget);

	if (iter == focusWidgets .end ())
		return;

	iter -> second .disconnect ();

	focusWidgets .erase (iter);
}

void
X3DPanelInterface::set_focus ()
{
	if (hasFocus ())
	{
		const auto widgets     = getWidgets <Gtk::Widget> (getWidget ());
		const auto anyHasFocus = std::any_of (widgets .begin (),
		                                      widgets .end (),
		                                      [ ] (Gtk::Widget* const widget)
		                                      { return widget -> has_focus (); });

		getWidget () .get_style_context () -> add_class ("titania-widget-box-selected");

		if (not anyHasFocus)
			getWidget () .grab_focus ();
	}
	else
		getWidget () .get_style_context () -> remove_class ("titania-widget-box-selected");

}

bool
X3DPanelInterface::on_button_press_event (GdkEventButton* event)
{
	if (event -> button == 1)
	{
		for (size_t i = 0, size = page -> getPanels () .size (); i < size; ++ i)
			page -> getPanels () [i] -> setFocus (i == id);
	}

	return false;
}

void
X3DPanelInterface::dispose ()
{
	for (auto & pair : focusWidgets)
		pair .second .disconnect ();

	panelMenu .reset ();

	X3DUserInterface::dispose ();
}

X3DPanelInterface::~X3DPanelInterface ()
{ }

} // puck
} // titania
