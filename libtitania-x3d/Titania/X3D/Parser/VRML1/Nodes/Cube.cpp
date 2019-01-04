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

#include "Cube.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Geometry3D/Box.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Shape/Appearance.h"
#include "../../../Components/Shape/Shape.h"
#include "../../../Components/Shape/X3DMaterialNode.h"
#include "../../../Components/Texturing/X3DTextureNode.h"
#include "../../../Components/Texturing/X3DTextureTransformNode.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"
#include "ShapeHints.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const std::string Cube::componentName  = "Titania";
const std::string Cube::typeName       = "Cube";
const std::string Cube::containerField = "children";

Cube::Fields::Fields () :
	 width (new X3D::SFFloat (2)),
	height (new X3D::SFFloat (2)),
	 depth (new X3D::SFFloat (2))
{ }

Cube::Cube (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1Cube);

	addField (initializeOnly, "width",    width ());
	addField (initializeOnly, "height",   height ());
	addField (initializeOnly, "depth",    depth ());
	addField (initializeOnly, "children", children ());
}

X3D::X3DBaseNode*
Cube::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new Cube (executionContext);
}

void
Cube::convert (Converter* const converter)
{
	if (use (converter))
		return;

	// Create nodes.

	const auto shapeNode      = converter -> scene -> createNode <X3D::Shape> ();
	const auto appearanceNode = converter -> scene -> createNode <X3D::Appearance> ();
	const auto geometryNode   = converter -> scene -> createNode <X3D::Box> ();

	// Set name.

	if (not getName () .empty ())
		converter -> scene -> updateNamedNode (getName (), shapeNode);

	// Assign values.

	shapeNode -> appearance () = appearanceNode;
	shapeNode -> geometry ()   = geometryNode;

	appearanceNode -> material ()         = converter -> materials         .back ();
	appearanceNode -> texture ()          = converter -> textures          .back ();
	appearanceNode -> textureTransform () = converter -> textureTransforms .back ();

	if (not converter -> shapeHints .empty ())
		geometryNode -> solid () = converter -> shapeHints .back () -> getSolid ();

	geometryNode -> size () .setX (width ());
	geometryNode -> size () .setY (height ());
	geometryNode -> size () .setZ (depth ());

	if (converter -> transforms .empty ())
		converter -> scene -> getRootNodes () .emplace_back (shapeNode);
	else
		converter -> groups .back () -> children () .emplace_back (shapeNode);
}

Cube::~Cube ()
{ }

} // VRML1
} // X3D
} // titania
