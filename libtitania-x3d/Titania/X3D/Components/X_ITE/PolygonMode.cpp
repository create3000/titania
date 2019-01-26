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

#include "PolygonMode.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/PolygonModeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   PolygonMode::component      = Component ("Titania", 1);
const std::string PolygonMode::typeName       = "PolygonMode";
const std::string PolygonMode::containerField = "children";

PolygonMode::Fields::Fields () :
	  type (new SFString ("FILL"))
{ }

PolygonMode::PolygonMode (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	polygonModeType (GL_FILL)
{
	addType (X3DConstants::PolygonMode);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "type",           type ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
PolygonMode::create (X3DExecutionContext* const executionContext) const
{
	return new PolygonMode (executionContext);
}

void
PolygonMode::initialize ()
{
	X3DGroupingNode::initialize ();

	type () .addInterest (&PolygonMode::set_type, this);

	set_type ();
}

void
PolygonMode::set_type ()
{
	if (type () == "POINT")
		polygonModeType = GL_POINT;

	else if (type () == "LINE")
		polygonModeType = GL_LINE;

	else
		polygonModeType = GL_FILL;
}

void
PolygonMode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::DISPLAY:
		{
			renderObject -> getLocalObjects () .emplace_back (new PolygonModeContainer (polygonModeType));

			X3DGroupingNode::traverse (type, renderObject);

			renderObject -> getLocalObjects () .pop_back ();
			break;
		}
		default:
		{
			X3DGroupingNode::traverse (type, renderObject);
			break;
		}
	}
}

} // X3D
} // titania
