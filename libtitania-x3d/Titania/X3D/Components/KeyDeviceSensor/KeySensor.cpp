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

#include "KeySensor.h"

#include "../../Execution/X3DExecutionContext.h"

#include <gdk/gdk.h>

namespace titania {
namespace X3D {

const ComponentType KeySensor::component      = ComponentType::KEY_DEVICE_SENSOR;
const std::string   KeySensor::typeName       = "KeySensor";
const std::string   KeySensor::containerField = "children";

static constexpr int32_t X3D_KEY_F1  = 1;
static constexpr int32_t X3D_KEY_F2  = 2;
static constexpr int32_t X3D_KEY_F3  = 3;
static constexpr int32_t X3D_KEY_F4  = 4;
static constexpr int32_t X3D_KEY_F5  = 5;
static constexpr int32_t X3D_KEY_F6  = 6;
static constexpr int32_t X3D_KEY_F7  = 7;
static constexpr int32_t X3D_KEY_F8  = 8;
static constexpr int32_t X3D_KEY_F9  = 9;
static constexpr int32_t X3D_KEY_F10 = 10;
static constexpr int32_t X3D_KEY_F11 = 11;
static constexpr int32_t X3D_KEY_F12 = 12;

static constexpr int32_t X3D_KEY_HOME  = 13;
static constexpr int32_t X3D_KEY_END   = 14;
static constexpr int32_t X3D_KEY_PGUP  = 15;
static constexpr int32_t X3D_KEY_PGDN  = 16;
static constexpr int32_t X3D_KEY_UP    = 17;
static constexpr int32_t X3D_KEY_DOWN  = 18;
static constexpr int32_t X3D_KEY_LEFT  = 19;
static constexpr int32_t X3D_KEY_RIGHT = 20;

static constexpr int32_t Control_R = 1;
static constexpr int32_t Control_L = 2;
static constexpr int32_t Shift_R   = 1;
static constexpr int32_t Shift_L   = 2;

KeySensor::Fields::Fields () :
	      controlKey (new SFBool ()),
	        shiftKey (new SFBool ()),
	          altKey (new SFBool ()),
	  actionKeyPress (new SFInt32 ()),
	actionKeyRelease (new SFInt32 ()),
	        keyPress (new SFString ()),
	      keyRelease (new SFString ())
{ }

KeySensor::KeySensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DKeyDeviceSensorNode (),
	                fields (),
	             shiftKeys (),
	           controlKeys ()
{
	addType (X3DConstants::KeySensor);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "enabled",          enabled ());
	addField (outputOnly,  "controlKey",       controlKey ());
	addField (outputOnly,  "shiftKey",         shiftKey ());
	addField (outputOnly,  "altKey",           altKey ());
	addField (outputOnly,  "actionKeyPress",   actionKeyPress ());
	addField (outputOnly,  "actionKeyRelease", actionKeyRelease ());
	addField (outputOnly,  "keyPress",         keyPress ());
	addField (outputOnly,  "keyRelease",       keyRelease ());
	addField (outputOnly,  "isActive",         isActive ());
}

X3DBaseNode*
KeySensor::create (X3DExecutionContext* const executionContext) const
{
	return new KeySensor (executionContext);
}

void
KeySensor::setActionKeyPressEvent (const int32_t keyval)
{
	if (not isActive ())
		isActive () = true;

	switch (keyval)
	{
		// Special keys

		case GDK_KEY_Control_L:
		{
			controlKeys |= Control_L;

			if (not controlKey ())
				controlKey () = true;

			break;
		}
		case GDK_KEY_Control_R:
		{
			controlKeys |= Control_R;

			if (not controlKey ())
				controlKey () = true;

			break;
		}
		case GDK_KEY_Shift_L:
		{
			shiftKeys |= Shift_L;

			if (not shiftKey ())
				shiftKey () = true;

			break;
		}
		case GDK_KEY_Shift_R:
		{
			shiftKeys |= Shift_R;

			if (not shiftKey ())
				shiftKey () = true;

			break;
		}
		case GDK_KEY_Meta_L:
		case GDK_KEY_Alt_L:
		{
			altKey () = true;
			break;
		}

		// Action keys

		case GDK_KEY_F1:
			actionKeyPress () = X3D_KEY_F1;
			break;

		case GDK_KEY_F2:
			actionKeyPress () = X3D_KEY_F2;
			break;

		case GDK_KEY_F3:
			actionKeyPress () = X3D_KEY_F3;
			break;

		case GDK_KEY_F4:
			actionKeyPress () = X3D_KEY_F4;
			break;

		case GDK_KEY_F5:
			actionKeyPress () = X3D_KEY_F5;
			break;

		case GDK_KEY_F6:
			actionKeyPress () = X3D_KEY_F6;
			break;

		case GDK_KEY_F7:
			actionKeyPress () = X3D_KEY_F7;
			break;

		case GDK_KEY_F8:
			actionKeyPress () = X3D_KEY_F8;
			break;

		case GDK_KEY_F9:
			actionKeyPress () = X3D_KEY_F9;
			break;

		case GDK_KEY_F10:
			actionKeyPress () = X3D_KEY_F10;
			break;

		case GDK_KEY_F11:
			actionKeyPress () = X3D_KEY_F11;
			break;

		case GDK_KEY_F12:
			actionKeyPress () = X3D_KEY_F12;
			break;

		case GDK_KEY_Home:
		case GDK_KEY_KP_Home:
			actionKeyPress () = X3D_KEY_HOME;
			break;

		case GDK_KEY_End:
		case GDK_KEY_KP_End:
			actionKeyPress () = X3D_KEY_END;
			break;

		case GDK_KEY_Page_Up:
		case GDK_KEY_KP_Page_Up:
			actionKeyPress () = X3D_KEY_PGUP;
			break;

		case GDK_KEY_Page_Down:
		case GDK_KEY_KP_Page_Down:
			actionKeyPress () = X3D_KEY_PGDN;
			break;

		case GDK_KEY_Up:
		case GDK_KEY_KP_Up:
			actionKeyPress () = X3D_KEY_UP;
			break;

		case GDK_KEY_Down:
		case GDK_KEY_KP_Down:
			actionKeyPress () = X3D_KEY_DOWN;
			break;

		case GDK_KEY_Left:
		case GDK_KEY_KP_Left:
			actionKeyPress () = X3D_KEY_LEFT;
			break;

		case GDK_KEY_Right:
		case GDK_KEY_KP_Right:
			actionKeyPress () = X3D_KEY_RIGHT;
			break;

		// Keys

		case GDK_KEY_KP_Enter:
			keyPress () = String (1, gdk_keyval_to_unicode (GDK_KEY_Return));
			break;

		default:
		{
			const auto character = gdk_keyval_to_unicode (keyval);

			if (character)
				keyPress () = String (1, character);

			break;
		}
	}
}

void
KeySensor::setActionKeyReleaseEvent (const int32_t keyval)
{
	switch (keyval)
	{
		// Special keys

		case GDK_KEY_Control_L:
		{
			controlKeys &= ~Control_L;

			if (not controlKeys and controlKey ())
				controlKey () = false;

			break;
		}
		case GDK_KEY_Control_R:
		{
			controlKeys &= ~Control_R;

			if (not controlKeys and controlKey ())
				controlKey () = false;

			break;
		}
		case GDK_KEY_Shift_L:
		{
			shiftKeys &= ~Shift_L;

			if (not shiftKeys and shiftKey ())
				shiftKey () = false;

			break;
		}
		case GDK_KEY_Shift_R:
		{
			shiftKeys &= ~Shift_R;

			if (not shiftKeys and shiftKey ())
				shiftKey () = false;

			break;
		}
		case GDK_KEY_Meta_L:
		case GDK_KEY_Alt_L:
		{
			altKey () = false;
			break;
		}

		// Action keys

		case GDK_KEY_F1:
			actionKeyRelease () = X3D_KEY_F1;
			break;

		case GDK_KEY_F2:
			actionKeyRelease () = X3D_KEY_F2;
			break;

		case GDK_KEY_F3:
			actionKeyRelease () = X3D_KEY_F3;
			break;

		case GDK_KEY_F4:
			actionKeyRelease () = X3D_KEY_F4;
			break;

		case GDK_KEY_F5:
			actionKeyRelease () = X3D_KEY_F5;
			break;

		case GDK_KEY_F6:
			actionKeyRelease () = X3D_KEY_F6;
			break;

		case GDK_KEY_F7:
			actionKeyRelease () = X3D_KEY_F7;
			break;

		case GDK_KEY_F8:
			actionKeyRelease () = X3D_KEY_F8;
			break;

		case GDK_KEY_F9:
			actionKeyRelease () = X3D_KEY_F9;
			break;

		case GDK_KEY_F10:
			actionKeyRelease () = X3D_KEY_F10;
			break;

		case GDK_KEY_F11:
			actionKeyRelease () = X3D_KEY_F11;
			break;

		case GDK_KEY_F12:
			actionKeyRelease () = X3D_KEY_F12;
			break;

		case GDK_KEY_Home:
		case GDK_KEY_KP_Home:
			actionKeyRelease () = X3D_KEY_HOME;
			break;

		case GDK_KEY_End:
		case GDK_KEY_KP_End:
			actionKeyRelease () = X3D_KEY_END;
			break;

		case GDK_KEY_Page_Up:
		case GDK_KEY_KP_Page_Up:
			actionKeyRelease () = X3D_KEY_PGUP;
			break;

		case GDK_KEY_Page_Down:
		case GDK_KEY_KP_Page_Down:
			actionKeyRelease () = X3D_KEY_PGDN;
			break;

		case GDK_KEY_Up:
		case GDK_KEY_KP_Up:
			actionKeyRelease () = X3D_KEY_UP;
			break;

		case GDK_KEY_Down:
		case GDK_KEY_KP_Down:
			actionKeyRelease () = X3D_KEY_DOWN;
			break;

		case GDK_KEY_Left:
		case GDK_KEY_KP_Left:
			actionKeyRelease () = X3D_KEY_LEFT;
			break;

		case GDK_KEY_Right:
		case GDK_KEY_KP_Right:
			actionKeyRelease () = X3D_KEY_RIGHT;
			break;

		// Keys

		case GDK_KEY_KP_Enter:
			keyRelease () = String (1, gdk_keyval_to_unicode (GDK_KEY_Return));
			break;

		default:
		{
			const auto character = gdk_keyval_to_unicode (keyval);

			if (character)
				keyRelease () = String (1, character);

			break;
		}
	}

	if (isActive ())
		isActive () = false;
}

void
KeySensor::setKeyPressEvent (const String & key)
{
	isActive () = true;
	keyPress () = key;
}

void
KeySensor::setKeyReleaseEvent (const String & key)
{
	keyRelease () = key;
	isActive ()   = false;
}

void
KeySensor::setKeyReleaseEvent ()
{
	shiftKeys   = 0;
	controlKeys = 0;

	if (shiftKey ())
		shiftKey () = false;

	if (controlKey ())
		controlKey () = false;

	if (altKey ())
		altKey () = false;
}

} // X3D
} // titania
