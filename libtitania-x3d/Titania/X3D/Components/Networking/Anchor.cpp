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

#include "Anchor.h"

#include "../PointingDeviceSensor/TouchSensor.h"
#include "../../Browser/Notification.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   Anchor::component      = Component ("Networking", 2);
const std::string Anchor::typeName       = "Anchor";
const std::string Anchor::containerField = "children";

Anchor::Fields::Fields () :
	description (new SFString ()),
	  parameter (new MFString ())
{ }

Anchor::Anchor (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	   X3DUrlObject (),
	         fields (),
	touchSensorNode (new TouchSensor (executionContext))
{
	addType (X3DConstants::Anchor);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "description",    description ());
	addField (inputOutput,    "url",            url ());
	addField (inputOutput,    "parameter",      parameter ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
	
	addChildObjects (touchSensorNode);
}

X3DBaseNode*
Anchor::create (X3DExecutionContext* const executionContext) const
{
	return new Anchor (executionContext);
}

void
Anchor::initialize ()
{
	X3DGroupingNode::initialize ();
	X3DUrlObject::initialize ();
	
	touchSensorNode -> touchTime () .addInterest (&Anchor::requestImmediateLoad, this);
	
	description () .addInterest (touchSensorNode -> description ());

	touchSensorNode -> description () = description ();
	touchSensorNode -> setup ();
}

void
Anchor::setExecutionContext (X3DExecutionContext* const executionContext)
{
	touchSensorNode -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DGroupingNode::setExecutionContext (executionContext);
}

void
Anchor::requestImmediateLoad ()
{
	try
	{
		setLoadState (IN_PROGRESS_STATE, false);

		FileLoader (getExecutionContext ()) .loadURL (url (), parameter ());

		setLoadState (COMPLETE_STATE, false);
	}
	catch (const X3DError &)
	{
		setLoadState (FAILED_STATE, false);
	}
}

void
Anchor::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			renderObject -> getBrowser () -> getSensors () .emplace_back ();
			touchSensorNode -> push (renderObject);

			X3DGroupingNode::traverse (type, renderObject);

			renderObject -> getBrowser () -> getSensors () .pop_back ();
			break;
		}
		default:
		{
			X3DGroupingNode::traverse (type, renderObject);
			break;
		}
	}
}

void
Anchor::dispose ()
{
	X3DUrlObject::dispose ();
	X3DGroupingNode::dispose ();
}

Anchor::~Anchor ()
{ }

} // X3D
} // titania
