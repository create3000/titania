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

#include "IndexedFaceSet.h"

#include "../../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Rendering/X3DCoordinateNode.h"
#include "../../../Components/Rendering/X3DNormalNode.h"
#include "../../../Components/Shape/Appearance.h"
#include "../../../Components/Shape/Shape.h"
#include "../../../Components/Shape/X3DMaterialNode.h"
#include "../../../Components/Texturing/X3DTextureNode.h"
#include "../../../Components/Texturing/X3DTextureCoordinateNode.h"
#include "../../../Components/Texturing/X3DTextureTransformNode.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"
#include "NormalBinding.h"
#include "ShapeHints.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const ComponentType IndexedFaceSet::component      = ComponentType::TITANIA;
const std::string   IndexedFaceSet::typeName       = "IndexedFaceSet";
const std::string   IndexedFaceSet::containerField = "children";

IndexedFaceSet::Fields::Fields () :
	       coordIndex (new X3D::MFInt32 ({ 0 })),
	    materialIndex (new X3D::MFInt32 ({ -1 })),
	      normalIndex (new X3D::MFInt32 ({ -1 })),
	textureCoordIndex (new X3D::MFInt32 ({ -1 }))
{ }

IndexedFaceSet::IndexedFaceSet (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	addField (initializeOnly, "coordIndex",        coordIndex ());
	addField (initializeOnly, "materialIndex",     materialIndex ());
	addField (initializeOnly, "normalIndex",       normalIndex ());
	addField (initializeOnly, "textureCoordIndex", textureCoordIndex ());
	addField (initializeOnly, "children",          children ());
}

X3D::X3DBaseNode*
IndexedFaceSet::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new IndexedFaceSet (executionContext);
}

void
IndexedFaceSet::convert (Converter* const converter)
{
	if (use (converter))
		return;

	// Create nodes.

	const auto shape      = converter -> scene -> createNode <X3D::Shape> ();
	const auto appearance = converter -> scene -> createNode <X3D::Appearance> ();
	const auto geometry   = converter -> scene -> createNode <X3D::IndexedFaceSet> ();

	// Set name.

	if (not getName () .empty ())
		converter -> scene -> updateNamedNode (getName (), shape);

	// Assign values.

	shape -> appearance () = appearance;
	shape -> geometry ()   = geometry;

	appearance -> material ()         = converter -> materials         .back ();
	appearance -> texture ()          = converter -> textures          .back ();
	appearance -> textureTransform () = converter -> textureTransforms .back ();

	if (not converter -> shapeHints .empty ())
	{
		geometry -> ccw ()         = converter -> shapeHints .back () -> getCCW ();
		geometry -> solid ()       = converter -> shapeHints .back () -> getSolid ();
		geometry -> convex ()      = converter -> shapeHints .back () -> getConvex ();
		geometry -> creaseAngle () = converter -> shapeHints .back () -> getCreaseAngle ();
	}

	if (not converter -> normalBindings .empty ())
	{
		if (converter -> normalBindings .back () -> getValue () == "OVERALL")
		{
			for (const auto & index : coordIndex ())
				geometry -> normalIndex () .emplace_back (index < 0 ? -1 : 0);
		}
		else if (converter -> normalBindings .back () -> getValue () == "PER_FACE")
		{
			geometry -> normalPerVertex () = false;
		}
		else if (converter -> normalBindings .back () -> getValue () == "PER_FACE_INDEXED")
		{
			geometry -> normalPerVertex () = false;
			geometry -> normalIndex ()     = normalIndex ();
		}
		else if (converter -> normalBindings .back () -> getValue () == "PER_VERTEX")
			;
		else //if (converter -> normalBindings .back () -> getValue () == "PER_VERTEX_INDEXED")
		{
			geometry -> normalIndex () = normalIndex ();
		}
	}
	else
		geometry -> normalIndex () = normalIndex ();

	geometry -> texCoordIndex () = textureCoordIndex ();
	geometry -> coordIndex ()    = coordIndex ();

	geometry -> texCoord () = converter -> texCoords .back ();
	geometry -> normal ()   = converter -> normals   .back ();
	geometry -> coord ()    = converter -> coords    .back ();

	if (converter -> transforms .empty ())
		converter -> scene -> getRootNodes () .emplace_back (shape);
	else
		converter -> transforms .back () -> children () .emplace_back (shape);
}

IndexedFaceSet::~IndexedFaceSet ()
{ }

} // VRML1
} // X3D
} // titania
