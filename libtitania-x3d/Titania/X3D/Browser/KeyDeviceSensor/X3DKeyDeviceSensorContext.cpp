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

#include "X3DKeyDeviceSensorContext.h"

#include "../X3DBrowser.h"
#include "../KeyDeviceSensor/KeyDevice.h"
#include "../../Components/KeyDeviceSensor/X3DKeyDeviceSensorNode.h"

namespace titania {
namespace X3D {

X3DKeyDeviceSensorContext::X3DKeyDeviceSensorContext () :
	              X3DBaseNode (),
	                keyDevice (new KeyDevice (getBrowser ())),
	     keyDeviceSensorNodes (),
	               controlKey (),
	                 shiftKey (),
	                   altKey (),
	       internalControlKey (false),
	         internalShiftKey (false),
	           internalAltKey (false),
	             externalKeys ()
{
	addChildObjects (keyDevice,
	                 keyDeviceSensorNodes,
	                 controlKey,
	                 shiftKey,
	                 altKey);
}

void
X3DKeyDeviceSensorContext::initialize ()
{
	keyDevice -> setup ();
}

void
X3DKeyDeviceSensorContext::addKeyDeviceSensor (X3DKeyDeviceSensorNode* const keyDeviceSensorNode)
{
	keyDeviceSensorNodes .emplace_back (keyDeviceSensorNode);
}

void
X3DKeyDeviceSensorContext::removeKeyDeviceSensor (X3DKeyDeviceSensorNode* const keyDeviceSensorNode)
{
	keyDeviceSensorNodes .remove (X3DWeakPtr <X3DKeyDeviceSensorNode> (keyDeviceSensorNode));
}

void
X3DKeyDeviceSensorContext::setControlKey (const bool value)
{
	internalControlKey = value;

	if ((internalControlKey or externalKeys .control ()) not_eq controlKey)
		controlKey = internalControlKey or externalKeys .control ();
}

void
X3DKeyDeviceSensorContext::setShiftKey (const bool value)
{
	internalShiftKey = value;

	if ((internalShiftKey or externalKeys .shift ()) not_eq shiftKey)
		shiftKey = internalShiftKey or externalKeys .shift ();
}

void
X3DKeyDeviceSensorContext::setAltKey (const bool value)
{
	internalAltKey = value;

	if ((internalAltKey or externalKeys .alt ()) not_eq altKey)
		altKey = internalAltKey or externalKeys .alt ();
}

bool
X3DKeyDeviceSensorContext::on_external_focus_out_event (GdkEventFocus* event)
{
	externalKeys .clear ();

	on_external_key_event ();

	return false;
}

bool
X3DKeyDeviceSensorContext::on_external_key_press_event (GdkEventKey* event)
{
	externalKeys .press (event);

	on_external_key_event ();

	return false;
}

bool
X3DKeyDeviceSensorContext::on_external_key_release_event (GdkEventKey* event)
{
	externalKeys .release (event);

	on_external_key_event ();

	return false;
}

void
X3DKeyDeviceSensorContext::on_external_key_event ()
{
	if ((internalControlKey or externalKeys .control ()) not_eq controlKey)
		controlKey = internalControlKey or externalKeys .control ();
	
	if ((internalShiftKey or externalKeys .shift ()) not_eq shiftKey)
		shiftKey = internalShiftKey or externalKeys .shift ();
	
	if ((internalAltKey or externalKeys .alt ()) not_eq altKey)
		altKey = internalAltKey or externalKeys .alt ();
}

X3DKeyDeviceSensorContext::~X3DKeyDeviceSensorContext ()
{ }

} // X3D
} // titania
