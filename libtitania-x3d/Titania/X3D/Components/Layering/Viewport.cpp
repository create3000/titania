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

#include "Viewport.h"

#include "../../Browser/X3DBrowser.h"

#include <iostream>

namespace titania {
namespace X3D {

const std::string Viewport::componentName  = "Layering";
const std::string Viewport::typeName       = "Viewport";
const std::string Viewport::containerField = "viewport";

Viewport::Fields::Fields () :
	clipBoundary (new MFFloat ({ 0, 1, 0, 1 }))
{ }

Viewport::Viewport (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DViewportNode (),
	    X3DRenderer (),
	         fields ()
{
	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "clipBoundary",   clipBoundary ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
Viewport::create (X3DExecutionContext* const executionContext) const
{
	return new Viewport (executionContext);
}

void
Viewport::initialize ()
{
	X3DViewportNode::initialize ();
	X3DRenderer::initialize ();
}

float
Viewport::getLeft ()
{
	return clipBoundary () .size () > 0 ? clipBoundary () [0] : 0.0f;
}

float
Viewport::getRight ()
{
	return clipBoundary () .size () > 1 ? clipBoundary () [1] : 1.0f;
}

float
Viewport::getBottom ()
{
	return clipBoundary () .size () > 2 ? clipBoundary () [2] : 0.0f;
}

float
Viewport::getTop ()
{
	return clipBoundary () .size () > 3 ? clipBoundary () [3] : 1.0f;
}

void
Viewport::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::PICKING:
		{
			push ();

			for (const auto & child : children ())
				child -> traverse (type);

			pop ();

			break;
		}
		case TraverseType::CAMERA:
		{
			for (const auto & child : children ())
				child -> traverse (type);

			break;
		}
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		case TraverseType::COLLECT:
		{
			push ();
			render (type);
			pop ();

			break;
		}
	}
}

void
Viewport::collect (const TraverseType type)
{
	for (const auto & child : children ())
		child -> traverse (type);
}

void
Viewport::push ()
{
	viewport = Viewport4i (); // x, y, width, heigth

	float left   = getLeft ();
	float right  = getRight ();
	float bottom = getBottom ();
	float top    = getTop ();

	float width  = right - left;
	float height = top - bottom;

	glViewport (left * viewport [2],
	            bottom * viewport [3],
	            width * viewport [2],
	            height * viewport [3]);
}

void
Viewport::pop ()
{
	glViewport (viewport [0],
	            viewport [1],
	            viewport [2],
	            viewport [3]);
}

void
Viewport::dispose ()
{
	X3DViewportNode::dispose ();
	X3DRenderer::dispose ();
}

} // X3D
} // titania
