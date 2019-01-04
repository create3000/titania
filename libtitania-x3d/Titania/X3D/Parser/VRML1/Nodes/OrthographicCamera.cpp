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

#include "OrthographicCamera.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Navigation/OrthoViewpoint.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const std::string OrthographicCamera::componentName  = "Titania";
const std::string OrthographicCamera::typeName       = "OrthographicCamera";
const std::string OrthographicCamera::containerField = "children";

OrthographicCamera::Fields::Fields () :
	     position (new X3D::SFVec3f (0, 0, 1)),
	  orientation (new X3D::SFRotation (0, 0, 1, 0)),
	focalDistance (new X3D::SFFloat (5)),
	       height (new X3D::SFFloat (2)),
	 nearDistance (new X3D::SFFloat (1)),
	  farDistance (new X3D::SFFloat (10))
{ }

OrthographicCamera::OrthographicCamera (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1OrthographicCamera);

	addField (initializeOnly, "position",      position ());
	addField (initializeOnly, "orientation",   orientation ());
	addField (initializeOnly, "focalDistance", focalDistance ());
	addField (initializeOnly, "height",        height ());
	addField (initializeOnly, "nearDistance",  nearDistance ());
	addField (initializeOnly, "farDistance",   farDistance ());
	addField (initializeOnly, "children",      children ());
}

X3D::X3DBaseNode*
OrthographicCamera::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new OrthographicCamera (executionContext);
}

void
OrthographicCamera::convert (Converter* const converter)
{
	if (use (converter))
		return;

	// Create nodes.

	const auto viewpointNode = converter -> scene -> createNode <X3D::OrthoViewpoint> ();

	// Set name.

	if (not getName () .empty ())
		converter -> scene -> updateNamedNode (getName (), viewpointNode);

	// Assign values.

	viewpointNode -> position ()    = position ();
	viewpointNode -> orientation () = orientation ();
	viewpointNode -> fieldOfView () = { -height () / 2, -height () / 2, height () / 2, height () / 2 };

	if (converter -> transforms .empty ())
		converter -> scene -> getRootNodes () .emplace_back (viewpointNode);
	else
		converter -> groups .back () -> children () .emplace_back (viewpointNode);
}

OrthographicCamera::~OrthographicCamera ()
{ }

} // VRML1
} // X3D
} // titania
