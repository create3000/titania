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

#ifndef __TITANIA_X3D_BROWSER_DEVICES_X3DPOINTING_DEVICE_H__
#define __TITANIA_X3D_BROWSER_DEVICES_X3DPOINTING_DEVICE_H__

#include "../X3DWidget.h"

#include <sigc++/sigc++.h>
#include <gdk/gdk.h>

namespace titania {
namespace X3D {

class X3DBrowserSurface;

class X3DPointingDevice :
	virtual public sigc::trackable
{
protected:

	///  @name Construction

	X3DPointingDevice (X3DBrowserSurface* const);

	void
	connect ();

	void
	disconnect ();

	///  @name Options

	virtual
	bool
	trackSensors ()
	{ return true; }

	///  @name Event Toolrs

	virtual
	void
	motionNotifyEvent (const bool)
	{ }

	virtual
	bool
	buttonPressEvent (const bool, const int)
	{ return false; }

	virtual
	void
	buttonReleaseEvent (const bool, const int)
	{ }
	
	virtual
	void
	leaveNotifyEvent ()
	{ }


private:

	///  @name Event Toolrs

	bool
	on_motion_notify_event (GdkEventMotion*);

	void
	set_motion (const double, const double);

	void
	set_verify_motion (const double, const double);

	bool
	on_button_press_event (GdkEventButton*);

	bool
	on_button_release_event (GdkEventButton*);

	bool
	on_leave_notify_event (GdkEventCrossing*);

	///  @name Operations

	bool
	pick (const double, const double);

	bool
	haveSensor ();

	///  @name Members
	
	X3DBrowserSurface* const browser;

	sigc::connection button_press_conncection;
	sigc::connection button_release_conncection;
	sigc::connection motion_notify_conncection;
	sigc::connection leave_notify_conncection;

	size_t button;
	bool   isOver;

};

} // X3D
} // titania

#endif
