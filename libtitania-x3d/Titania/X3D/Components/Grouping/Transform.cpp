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

#include "Transform.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Handles/TransformHandle.h"

namespace titania {
namespace X3D {

Transform::Fields::Fields () :
	     translation (new SFVec3f ()),
	        rotation (new SFRotation ()),
	           scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	          center (new SFVec3f ())
{ }

Transform::Transform (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	         fields (),
	         matrix (),
	         handle ()
{
	setComponent ("Grouping");
	setTypeName ("Transform");

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOutput,    "translation",      translation ());
	addField (inputOutput,    "rotation",         rotation ());
	addField (inputOutput,    "scale",            scale ());
	addField (inputOutput,    "scaleOrientation", scaleOrientation ());
	addField (inputOutput,    "center",           center ());
	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (inputOnly,      "addChildren",      addChildren ());
	addField (inputOnly,      "removeChildren",   removeChildren ());
	addField (inputOutput,    "children",         children ());

	addChildren (handle);
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
Transform::setMatrix (const Matrix4f & value)
{
	Vector3f   t, s;
	Rotation4f r, so;

	value .get (t, r, s, so);

	translation ()      = t;
	rotation ()         = r;
	scale ()            = s;
	scaleOrientation () = so;
}

void
Transform::addHandle ()
{
	X3DGroupingNode::addHandle (new TransformHandle (this, getExecutionContext ()));
}

void
Transform::eventsProcessed ()
{
	X3DGroupingNode::eventsProcessed ();

	matrix .set (translation (),
	             rotation (),
	             scale (),
	             scaleOrientation (),
	             center ());
}

void
Transform::traverse (const TraverseType type)
{
	glPushMatrix ();

	glMultMatrixf (matrix .data ());

	X3DGroupingNode::traverse (type);

	glPopMatrix ();
}

void
Transform::dispose ()
{
	handle .dispose ();

	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
