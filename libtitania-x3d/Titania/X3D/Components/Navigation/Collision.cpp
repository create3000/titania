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

#include "Collision.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   Collision::component      = Component ("Navigation", 2);
const std::string Collision::typeName       = "Collision";
const std::string Collision::containerField = "children";

Collision::Fields::Fields () :
	collideTime (new SFTime ()),
	      proxy (new SFNode ())
{ }

Collision::Collision (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	  X3DSensorNode (),
	         fields (),
	      proxyNode (),
	      showProxy (false)
{
	addType (X3DConstants::Collision);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "enabled",        enabled ());
	addField (outputOnly,     "isActive",       isActive ());
	addField (outputOnly,     "collideTime",    collideTime ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (initializeOnly, "proxy",          proxy ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());

	addField (VRML_V2_0, "collide", "enabled");

	addChildObjects (proxyNode);
}

X3DBaseNode*
Collision::create (X3DExecutionContext* const executionContext) const
{
	return new Collision (executionContext);
}

void
Collision::initialize ()
{
	X3DGroupingNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&Collision::set_live, this);
	isLive () .addInterest (&Collision::set_live, this);

	enabled () .addInterest (&Collision::set_live, this);
	proxy ()   .addInterest (&Collision::set_proxy, this);

	set_live ();
	set_proxy ();
}

void
Collision::setShowProxy (const bool value)
{
	showProxy = value;

	set_cameraObjects ();
}

void
Collision::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive () and enabled ())
		getBrowser () -> addCollision (this);

	else
		getBrowser () -> removeCollision (this);
}

void
Collision::set_active (bool value)
{
	if (value == isActive ())
		return;

	isActive () = value;

	if (isActive ())
		collideTime () = getCurrentTime ();
}

void
Collision::set_proxy ()
{
	proxyNode .set (x3d_cast <X3DChildNode*> (proxy ()));
}

void
Collision::set_cameraObjects ()
{
	if (showProxy)
		setCameraObject (proxyNode and proxyNode -> isCameraObject ());
	else
		X3DGroupingNode::set_cameraObjects ();
}

void
Collision::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::COLLISION:
		{
			if (enabled ())
			{
				renderObject -> getCollisions () .emplace_back (this);

				if (proxyNode)
					proxyNode -> traverse (type, renderObject);

				else
					X3DGroupingNode::traverse (type, renderObject);

				renderObject -> getCollisions () .pop_back ();
			}

			break;
		}
		default:
		{
			if (showProxy)
			{
				if (proxyNode and not isHidden ())
					proxyNode -> traverse (type, renderObject);
			}
			else
				X3DGroupingNode::traverse (type, renderObject);

			break;
		}
	}
}

void
Collision::dispose ()
{
	getBrowser () -> removeCollision (this);

	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
