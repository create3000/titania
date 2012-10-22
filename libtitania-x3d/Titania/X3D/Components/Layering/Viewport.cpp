/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "Viewport.h"

#include "../../Browser/Browser.h"

#include <iostream>

namespace titania {
namespace X3D {

Viewport::Viewport (X3DExecutionContext* const executionContext) :
	   X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DViewportNode (),                                 
	    X3DRenderer (),                                 
	   clipBoundary ({ 0, 1, 0, 1 })                    // MFFloat [in,out] clipBoundary  0 1 0 1        [0,1]
{
	setComponent ("Layering");
	setTypeName ("Viewport");

	appendField (inputOutput,    "metadata",       metadata);
	appendField (inputOutput,    "clipBoundary",   clipBoundary);
	appendField (initializeOnly, "bboxSize",       bboxSize);
	appendField (initializeOnly, "bboxCenter",     bboxCenter);
	appendField (inputOnly,      "addChildren",    addChildren);
	appendField (inputOnly,      "removeChildren", removeChildren);
	appendField (inputOutput,    "children",       children);
}

X3DBasicNode*
Viewport::create (X3DExecutionContext* const executionContext) const
{
	return new Viewport (executionContext);
}

void
Viewport::initialize ()
{
	X3DViewportNode::initialize ();
}

void
Viewport::intersect ()
{
	enable ();
	X3DViewportNode::intersect ();
	disable ();
}

void
Viewport::display ()
{
	enable ();

	X3DRenderer::display ();

	disable ();
}

void
Viewport::enable ()
{
	MFFloat cB = clipBoundary .size () < 4 ? MFFloat ({ 0, 1, 0, 1 }) : clipBoundary;

	glGetIntegerv (GL_VIEWPORT, viewport); // x, y, width, heigth

	glViewport (cB [0] * viewport [2],
	            cB [2] * viewport [3],
	            cB [1] * viewport [2],
	            cB [3] * viewport [3]);
}

void
Viewport::disable ()
{
	glViewport (viewport [0],
	            viewport [1],
	            viewport [2],
	            viewport [3]);
}

} // X3D
} // titania
