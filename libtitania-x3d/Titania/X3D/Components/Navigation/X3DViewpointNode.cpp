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

#include "X3DViewpointNode.h"

#include "../../Browser/Browser.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/NavigationInfo.h"

#include <limits>

namespace titania {
namespace X3D {

X3DViewpointNode::X3DViewpointNode (bool addToList) :
	  X3DBindableNode (),     
	      description (),     // SFString   [in,out] description       ""
	      orientation (),     // SFRotation [in,out] orientation       0 0 1 0        [-1,1],(-∞,∞)
	 centerOfRotation (),     // SFVec3f    [in,out] centerOfRotation  0 0 0          (-∞,∞)
	             jump (true), // SFBool     [in,out] jump              TRUE
	retainUserOffsets (),     // SFBool     [ ]      retainUserOffsets
	addToList (addToList)
{
	addNodeType (X3DConstants::X3DViewpointNode);

	setChildren (translation,
	             rotation);
}

void
X3DViewpointNode::initialize ()
{
	X3DBindableNode::initialize ();

	if (addToList)
		getScene () -> addViewpoint (this);
}

void
X3DViewpointNode::setTransformationMatrix (const Matrix4f & value)
{
	transformationMatrix        = value;
	inverseTransformationMatrix = ~value;
}

void
X3DViewpointNode::bindToLayer (X3DLayerNode* const layer)
{
	std::clog << "Trying to bind X3DViewpoint '" << description << "': " << std::flush;

	layer -> viewpointStack .push (this);

	std::clog << (layer -> viewpointStack .top () == this ? "success." : "rejected.") << std::endl;
}

void
X3DViewpointNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> viewpointStack .pop (this);
}

void
X3DViewpointNode::display ()
{ }

void
X3DViewpointNode::draw ()
{
	NavigationInfo* navigationInfo = getCurrentLayer () -> getNavigationInfo ();

	float sizeZ           = navigationInfo -> avatarSize .size () ? navigationInfo -> avatarSize .front () : 0.25;
	float visibilityLimit = navigationInfo -> visibilityLimit ? navigationInfo -> visibilityLimit : 100000;
	
	if (sizeZ == 0)
		sizeZ = std::numeric_limits <float> ::epsilon () * 100;

	reshape (sizeZ * 0.5f, visibilityLimit);

	glLoadMatrixf (getInverseTransformationMatrix () .data ());
}

void
X3DViewpointNode::dispose ()
{
	X3DBindableNode::dispose ();

	getScene () -> removeViewpoint (this);
}

} // X3D
} // titania
