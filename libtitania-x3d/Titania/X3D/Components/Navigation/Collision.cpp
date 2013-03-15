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

#include "Collision.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

Collision::Collision (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGroupingNode (),                                                    
	  X3DSensorNode (),                                                    
	    collideTime (),                                                    // SFTime [out] collideTime
	          proxy (),                                                    // SFNode [ ]   proxy        NULL        [X3DChildNode]
	         _proxy (NULL)                                                 
{
	setComponent ("Navigation");
	setTypeName ("Collision");

	addField (inputOutput,    "metadata",       metadata);
	addField (inputOutput,    "enabled",        enabled);
	addField (outputOnly,     "isActive",       isActive);
	addField (outputOnly,     "collideTime",    collideTime);
	addField (initializeOnly, "bboxSize",       bboxSize);
	addField (initializeOnly, "bboxCenter",     bboxCenter);
	addField (initializeOnly, "proxy",          proxy);
	addField (inputOnly,      "addChildren",    addChildren);
	addField (inputOnly,      "removeChildren", removeChildren);
	addField (inputOutput,    "children",       children);
	addFieldAlias ("collide", "enabled");
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

	proxy .addInterest (this, &Collision::set_proxy);

	set_proxy ();
}

void
Collision::set_active (bool value)
{
	if (isActive not_eq value)
	{
		isActive = value;

		if (isActive)
			collideTime = getCurrentTime ();
	}
}

void
Collision::set_proxy ()
{
	_proxy = x3d_cast <X3DChildNode*> (proxy .getValue ());
}

void
Collision::traverse (TraverseType type)
{
	switch (type)
	{
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		{
			if (enabled)
			{
				if (_proxy)
					_proxy -> traverse (type);

				else
				{
					getCurrentLayer () -> getCollisions () .emplace_back (this);

					X3DGroupingNode::traverse (type);

					getCurrentLayer () -> getCollisions () .pop_back ();
				}
			}

			break;
		}
		default:
		{
			X3DGroupingNode::traverse (type);
			break;
		}
	}
}

} // X3D
} // titania
