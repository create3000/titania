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

#include "Notification.h"

#include "../Bits/config.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

const std::string Notification::componentName  = "Browser";
const std::string Notification::typeName       = "Notification";
const std::string Notification::containerField = "notification";

Notification::Fields::Fields () :
	string (new SFString ())
{ }

Notification::Notification (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (),
	      world ()
{
	addField (inputOutput, "string", string ());
	
	addChildren (world);
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

	if (glXGetCurrentContext ())
	{
		X3DSFNode <Scene> scene;

		try
		{
			scene = getBrowser () -> createX3DFromURL ({ get_tool ("Notification.wrl") .str () });

			try
			{
				auto notification = scene -> getNamedNode ("Notification");

				SFBool & field = *static_cast <SFBool*> (notification -> getField ("isActive"));

				field .addInterest (this, &Notification::set_active);
			}
			catch (const X3DError &)
			{ }
		}
		catch (const X3DError & error)
		{
			std::clog << error .what () << std::endl;

			scene = getBrowser () -> createScene ();
			scene -> setup ();
		}

		world = new World (scene);
		world -> setup ();

		string () .addInterest (this, &Notification::set_string);
	}
}
	
void
Notification::set_string ()
{
	try
	{
		auto notification = world -> getExecutionContext () -> getNamedNode ("Notification");

		try
		{
			SFString & field = *static_cast <SFString*> (notification -> getField ("set_string"));

			field = string ();
		}
		catch (const X3DError &)
		{ }
	}
	catch (const X3DError &)
	{
		// catch error from getNamedNode
	}
}

void
Notification::set_active (const bool & value)
{
	if (value)
	{
		getBrowser () -> displayed () .addInterest (this, &Notification::display);
	}
	else
	{
		getBrowser () -> displayed () .removeInterest (this, &Notification::display);
	}
}

void
Notification::display ()
{
	PolygonMode polygonMode (GL_FILL);

	world -> traverse (TraverseType::COLLECT);
}

void
Notification::dispose ()
{
	world .dispose ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania
