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

#include "StringSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string StringSensor::componentName  = "KeyDeviceSensor";
const std::string StringSensor::typeName       = "StringSensor";
const std::string StringSensor::containerField = "children";

StringSensor::Fields::Fields () :
	deletionAllowed (new SFBool (true)),
	    enteredText (new SFString ()),
	      finalText (new SFString ())
{ }

StringSensor::StringSensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DKeyDeviceSensorNode (),
	                fields ()
{
	addType (X3DConstants::StringSensor);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "enabled",         enabled ());
	addField (inputOutput, "deletionAllowed", deletionAllowed ());
	addField (outputOnly,  "enteredText",     enteredText ());
	addField (outputOnly,  "finalText",       finalText ());
	addField (outputOnly,  "isActive",        isActive ());
}

X3DBaseNode*
StringSensor::create (X3DExecutionContext* const executionContext) const
{
	return new StringSensor (executionContext);
}

void
StringSensor::setActionKeyPressEvent (const int32_t keyval)
{
	switch (keyval)
	{
		case GDK_KEY_BackSpace:
		{
			if (isActive ())
			{
				if (deletionAllowed ())
				{
					if (enteredText () .length ())
						enteredText () = enteredText () .getValue () .substr (0, enteredText () .length () - 1);
				}
			}

			break;
		}
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			finalText () = enteredText ();

			enteredText () .set ("");

			if (isActive ())
				isActive () = false;

			break;
		}
		case GDK_KEY_Escape:
		{
			enteredText () .set ("");

			if (isActive ())
				isActive () = false;

			break;
		}
		case GDK_KEY_Tab:
		{
			break;
		}
	}
}

void
StringSensor::setKeyPressEvent (const String & key)
{
	if (not isActive ())
	{
		isActive ()    = true;
		enteredText () = "";
	}

	enteredText () = enteredText () + key;
}

} // X3D
} // titania
