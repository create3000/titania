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

#include "X3DTextViewEditable.h"

#include <Titania/LOG.h>

namespace titania {
namespace puck {

X3DTextViewEditable::X3DTextViewEditable (const bool multiline) :
	        Gtk::CellEditable (),
	                 Gtk::Box (),
	editing_canceled_property (*this, "editing-canceled", false),
	           scrolledWindow (),
	                 textview (),
	                multiline (multiline),
	                validated (false),
	           handleFocusOut (true)
{
	get_style_context () -> add_class ("titania-textview-editable");
	get_style_context () -> add_class ("entry");

	scrolledWindow .set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	textview       .set_editable (true);

	pack_start (scrolledWindow, true, true);
	scrolledWindow .add (textview);

	show_all ();

	textview .signal_button_press_event () .connect (sigc::mem_fun (this, &X3DTextViewEditable::on_textview_button_press_event_before), false);
	textview .signal_button_press_event () .connect (sigc::mem_fun (this, &X3DTextViewEditable::on_textview_button_press_event_after));
	textview .signal_focus_out_event ()    .connect (sigc::mem_fun (this, &X3DTextViewEditable::on_textview_focus_out_event));
	textview .signal_key_press_event ()    .connect (sigc::mem_fun (this, &X3DTextViewEditable::on_textview_key_press_event), false);
}

void
X3DTextViewEditable::start_editing_vfunc (GdkEvent* event)
{
	property_editing_canceled () = false;
	validated                    = false;
}

void
X3DTextViewEditable::set_padding (const int top, const int right, const int bottom, const int left)
{
	textview .set_margin_top (top);
	textview .set_margin_right (right);
	textview .set_margin_bottom (bottom);
	textview .set_margin_left (left);
}

void
X3DTextViewEditable::on_grab_focus ()
{
	textview .grab_focus ();

	const auto buffer = textview .get_buffer ();

	buffer -> place_cursor (buffer -> begin ());
}

bool
X3DTextViewEditable::on_textview_button_press_event_before (GdkEventButton* event)
{
	if (event -> button == 3)
		handleFocusOut = false;

	return false;
}

bool
X3DTextViewEditable::on_textview_button_press_event_after (GdkEventButton* event)
{
	return true;
}

bool
X3DTextViewEditable::on_textview_focus_out_event (GdkEventFocus* event)
{
	if (handleFocusOut)
		editing_done ();

	handleFocusOut = true;

	return false;
}

bool
X3DTextViewEditable::on_textview_key_press_event (GdkEventKey* event)
{
	if (multiline)
	{
		if (event -> state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK) and (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter))
		{
			editing_done ();
			return true;
		}
	}
	else
	{
		if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
		{
			editing_done ();
			return true;
		}
	}

	if (event -> keyval == GDK_KEY_Escape)
	{
		editing_canceled ();
		return true;
	}

	return false;
}

void
X3DTextViewEditable::editing_canceled ()
{
	property_editing_canceled () = true;
	editing_done ();
	remove_widget ();
}

X3DTextViewEditable::~X3DTextViewEditable ()
{ }

} // puck
} // titania
