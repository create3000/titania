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

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "Transform.h"

namespace titania {
namespace X3D {

Transform::Transform (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	 X3DGroupingNode (),                                                    
	          center (),                                                    // SFVec3f    [in,out] center            0 0 0          (-∞,∞)
	        rotation (),                                                    // SFRotation [in,out] rotation          0 0 1 0        [-1,1] or (-∞,∞)
	           scale (1, 1, 1),                                             // SFVec3f    [in,out] scale             1 1 1          (-∞, ∞)
	scaleOrientation (),                                                    // SFRotation [in,out] scaleOrientation  0 0 1 0        [-1,1] or (-∞,∞)
	     translation ()                                                     // SFVec3f    [in,out] translation       0 0 0          (-∞,∞)
{
	setComponent ("Grouping");
	setTypeName ("Transform");

	addField (inputOutput,    "metadata",         metadata);
	addField (inputOutput,    "translation",      translation);
	addField (inputOutput,    "rotation",         rotation);
	addField (inputOutput,    "scale",            scale);
	addField (inputOutput,    "scaleOrientation", scaleOrientation);
	addField (inputOutput,    "center",           center);
	addField (initializeOnly, "bboxSize",         bboxSize);
	addField (initializeOnly, "bboxCenter",       bboxCenter);
	addField (inputOnly,      "addChildren",      addChildren);
	addField (inputOnly,      "removeChildren",   removeChildren);
	addField (inputOutput,    "children",         children);
}

X3DBaseNode*
Transform::create (X3DExecutionContext* const executionContext) const
{
	return new Transform (executionContext);
}

void
Transform::initialize ()
{
	X3DGroupingNode::initialize ();
	eventsProcessed ();
}

Box3f
Transform::getBBox ()
{
	return X3DGroupingNode::getBBox () * matrix;
}

void
Transform::eventsProcessed ()
{
	X3DGroupingNode::eventsProcessed ();

	matrix .set (translation,
	             rotation,
	             scale,
	             scaleOrientation,
	             center);
}

void
Transform::pick ()
{
	//	if (getBrowser () -> getEditMode ())
	//		getBrowser () -> pushSensitiveNode (this);

	glPushMatrix ();

	glMultMatrixf (matrix .data ());

	X3DGroupingNode::pick ();

	glPopMatrix ();

	//	if (getBrowser () -> getEditMode ())
	//		getBrowser () -> popSensitiveNode ();
}

void
Transform::display ()
{
	glPushMatrix ();

	glMultMatrixf (matrix .data ());

	X3DGroupingNode::display ();

	glPopMatrix ();
}

} // X3D
} // titania
