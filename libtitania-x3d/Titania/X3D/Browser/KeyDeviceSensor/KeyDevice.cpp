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

#include "KeyDevice.h"

#include "../Browser.h"
#include "../Selection.h"

#include "../../Components/KeyDeviceSensor/X3DKeyDeviceSensorNode.h"

namespace titania {
namespace X3D {

KeyDevice::KeyDevice (Browser* const browser) :
	      X3DBrowserObject (browser),
	        imContextPress (gtk_im_context_simple_new ()),
	      imContextRelease (gtk_im_context_simple_new ()),
	              keyPress (),
	            keyRelease (),
	  key_press_connection (),
	key_release_connection (),
	                  keys ()
{ }

void
KeyDevice::initialize ()
{
	X3DBrowserObject::initialize ();

	getBrowser () -> signal_focus_out_event   () .connect (sigc::mem_fun (*this, &KeyDevice::on_focus_out_event));
	getBrowser () -> signal_key_press_event   () .connect (sigc::mem_fun (*this, &KeyDevice::on_action_key_press_event));
	getBrowser () -> signal_key_release_event () .connect (sigc::mem_fun (*this, &KeyDevice::on_action_key_release_event));

	getBrowser () -> keyDeviceSensorNodeEvent () .addInterest (this, &KeyDevice::set_keyDeviceSensorNodeEvent);

	g_signal_connect (imContextPress,   "commit", G_CALLBACK (&KeyDevice::on_commit), &this -> keyPress);
	g_signal_connect (imContextRelease, "commit", G_CALLBACK (&KeyDevice::on_commit), &this -> keyRelease);
}

void
KeyDevice::set_keyDeviceSensorNodeEvent ()
{
	gtk_im_context_reset (imContextPress);
	gtk_im_context_reset (imContextRelease);

	key_press_connection   .disconnect ();
	key_release_connection .disconnect ();

	if (getBrowser () -> getKeyDeviceSensorNode ())
	{
		key_press_connection   = getBrowser () -> signal_key_press_event   () .connect (sigc::mem_fun (*this, &KeyDevice::on_key_press_event));
		key_release_connection = getBrowser () -> signal_key_release_event () .connect (sigc::mem_fun (*this, &KeyDevice::on_key_release_event));
	}
}

bool
KeyDevice::on_focus_out_event (GdkEventFocus*)
{
	keys .clear ();

	if (getBrowser () -> hasControlKey ())
		getBrowser () -> hasControlKey (false);

	if (getBrowser () -> hasShiftKey ())
		getBrowser () -> hasShiftKey (false);

	if (getBrowser () -> hasAltKey ())
		getBrowser () -> hasAltKey (false);

	if (getBrowser () -> getKeyDeviceSensorNode ())
		getBrowser () -> getKeyDeviceSensorNode () -> setKeyReleaseEvent ();

	return false;
}

bool
KeyDevice::on_action_key_press_event (GdkEventKey* event)
{
	keys .press (event);

	if (keys .control () not_eq getBrowser () -> hasControlKey ())
		getBrowser () -> hasControlKey (keys .control ());

	if (keys .shift () not_eq getBrowser () -> hasShiftKey ())
		getBrowser () -> hasShiftKey (keys .shift ());

	if (keys .alt () not_eq getBrowser () -> hasAltKey ())
		getBrowser () -> hasAltKey (keys .alt ());

	return false;
}

bool
KeyDevice::on_action_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	if (keys .control () not_eq getBrowser () -> hasControlKey ())
		getBrowser () -> hasControlKey (keys .control ());

	if (keys .shift () not_eq getBrowser () -> hasShiftKey ())
		getBrowser () -> hasShiftKey (keys .shift ());

	if (keys .alt () not_eq getBrowser () -> hasAltKey ())
		getBrowser () -> hasAltKey (keys .alt ());
	
	return false;
}

bool
KeyDevice::on_key_press_event (GdkEventKey* event)
{
	if (gtk_im_context_filter_keypress (imContextPress, event))
	{
		if (not keyPress .empty ())
		{
			getBrowser () -> getKeyDeviceSensorNode () -> setKeyPressEvent (keyPress);

			keyPress .clear ();
			return false;
		}
	}

	getBrowser () -> getKeyDeviceSensorNode () -> setActionKeyPressEvent (event -> keyval);
	return not getBrowser () -> getSelection () -> isEnabled ();
}

bool
KeyDevice::on_key_release_event (GdkEventKey* event)
{
	event -> type = GDK_KEY_PRESS;

	if (gtk_im_context_filter_keypress (imContextRelease, event))
	{
		if (not keyRelease .empty ())
		{
			getBrowser () -> getKeyDeviceSensorNode () -> setKeyReleaseEvent (keyRelease);

			keyRelease .clear ();
			return false;
		}
	}

	getBrowser () -> getKeyDeviceSensorNode () -> setActionKeyReleaseEvent (event -> keyval);
	return not getBrowser () -> getSelection () -> isEnabled ();
}

void
KeyDevice::on_commit (GtkIMContext*, gchar* string, gpointer user_data)
{
	String* const key = static_cast <String*> (user_data);

	*key = string;
}

KeyDevice::~KeyDevice ()
{
	g_object_unref (imContextPress);
	g_object_unref (imContextRelease);
}

} // X3D
} // titania
