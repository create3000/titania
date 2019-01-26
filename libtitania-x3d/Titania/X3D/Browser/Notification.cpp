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

#include "Notification.h"

#include "../Browser/Networking/config.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Browser/X3DBrowser.h"
#include "../InputOutput/FileLoader.h"

namespace titania {
namespace X3D {

const Component   Notification::component      = Component ("Titania", 1);
const std::string Notification::typeName       = "Notification";
const std::string Notification::containerField = "notification";

Notification::Fields::Fields () :
	string (new SFString ())
{ }

Notification::Notification (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (),
	      scene ()
{
	addType (X3DConstants::Notification);

	addField (inputOutput, "string", string ());
	
	addChildObjects (scene);
}

X3DBaseNode*
Notification::create (X3DExecutionContext* const executionContext) const
{
	return new Notification (executionContext);
}

void
Notification::initialize ()
{
	X3DBaseNode::initialize ();

	string () .addInterest (&Notification::set_string, this);
}

void
Notification::set_string ()
{
	try
	{
		string () .removeInterest (&Notification::set_string, this);
	
		if (scene)
			return;

		scene = FileLoader (getBrowser () -> getPrivateScene ()) .createX3DFromURL ({ get_tool ("Notification.x3dv") .str () });

		const auto notification = scene -> getNamedNode ("Notification");

		// string
		
		SFString & set_string = notification -> getField <SFString> ("set_string");

		string () .addInterest (set_string);

		set_string = string ();

		// isActive

		SFBool & isActive = notification -> getField <SFBool> ("isActive");

		isActive .addInterest (&Notification::set_active, this);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;

		scene = getBrowser () -> createScene ();
	}
}

void
Notification::set_active (const bool value)
{
	try
	{
		const auto   notificationLayer = scene -> getNamedNode ("Layer");
		const auto & headUpDisplay     = getBrowser () -> getHeadUpDisplay ();

		if (value)
		{
			headUpDisplay -> order () .emplace_back (headUpDisplay -> layers () .size () + 1);
			headUpDisplay -> layers () .emplace_back (notificationLayer);
		}
		else
		{
			for (const auto index : headUpDisplay -> layers () .indices_of (notificationLayer))
				headUpDisplay -> order () .remove (index + 1);
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Notification::dispose ()
{
	X3DBaseNode::dispose ();
}

} // X3D
} // titania
