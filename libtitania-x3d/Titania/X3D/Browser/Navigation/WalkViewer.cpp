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

#include "WalkViewer.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   WalkViewer::component      = Component ("Titania", 1);
const std::string WalkViewer::typeName       = "WalkViewer";
const std::string WalkViewer::containerField = "viewer";

WalkViewer::WalkViewer (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFlyViewer ()
{
	addType (X3DConstants::WalkViewer);

	addField (outputOnly, "isActive",   isActive ());
	addField (outputOnly, "scrollTime", scrollTime ());
}

X3DBaseNode*
WalkViewer::create (X3DExecutionContext* const executionContext) const
{
	return new WalkViewer (executionContext);
}

void
WalkViewer::initialize ()
{
	X3DFlyViewer::initialize ();

	getBrowser () -> addCollision (this);
}

Vector3d
WalkViewer::getTranslationOffset (const Vector3d & velocity)
{
	static constexpr Vector3d yAxis (0, 1, 0);

	const auto & viewpoint = getActiveViewpoint ();
	const auto   upVector  = viewpoint -> getUpVector ();

	const auto userOrientation = viewpoint -> getUserOrientation ();
	const auto orientation     = userOrientation * Rotation4d (yAxis * userOrientation, upVector);

	return velocity * orientation;
}

Vector3d
WalkViewer::constrainPanDirection (const Vector3d & direction)
{
	if (direction .y () < 0)
		return Vector3d (direction .x (), 0, direction .z ());

	return direction;
}

void
WalkViewer::dispose ()
{
	getBrowser () -> removeCollision (this);
	
	X3DFlyViewer::dispose ();
}

} // X3D
} // titania
