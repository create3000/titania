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

#include "X3DComposedGeometryNodeTool.h"

#include "../Rendering/CoordinateTool.h"

#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Editing/Editor.h"

namespace titania {
namespace X3D {

X3DComposedGeometryNodeTool::X3DComposedGeometryNodeTool () :
	X3DComposedGeometryNode (),
	    X3DGeometryNodeTool ()
{
	addType (X3DConstants::X3DComposedGeometryNodeTool);
}

void
X3DComposedGeometryNodeTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DComposedGeometryNodeTool::set_loadState);

	set_loadState ();
}

void
X3DComposedGeometryNodeTool::set_loadState ()
{
	try
	{
		const auto & inlineNode          = getCoordinateTool () -> getInlineNode ();
		const auto   activeEdgesGeometry = inlineNode -> getExportedNode <IndexedLineSet> ("ActiveEdgesGeometry");

		coord () .addInterest (activeEdgesGeometry -> coord ());

		activeEdgesGeometry -> coord () = coord ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DComposedGeometryNodeTool::undoSetColorColor (const UndoStepPtr & undoStep)
{
	if (not getColor ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (color ()), nullptr);
	   return;
	}

	Editor () .undoSetColorColor (getColor (), undoStep);
}

void
X3DComposedGeometryNodeTool::undoSetTexCoordPoint (const UndoStepPtr & undoStep)
{
	if (not getTexCoord ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (texCoord ()), nullptr);
	   return;
	}

	Editor () .undoSetTexCoordPoint (getTexCoord (), undoStep);
}

void
X3DComposedGeometryNodeTool::undoSetNormalVector (const UndoStepPtr & undoStep)
{
	if (not getNormal ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (normal ()), nullptr);
	   return;
	}

	Editor () .undoSetNormalVector (getNormal (), undoStep);
}

void
X3DComposedGeometryNodeTool::undoSetCoordPoint (const UndoStepPtr & undoStep)
{
	if (not getCoord ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (coord ()), nullptr);
	   return;
	}

	Editor () .undoSetCoordPoint (getCoord (), undoStep);
}

void
X3DComposedGeometryNodeTool::redoSetColorColor (const UndoStepPtr & undoStep)
{
	if (not getColor ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (color ()), nullptr);
	   return;
	}

	Editor () .redoSetColorColor (getColor (), undoStep);
}

void
X3DComposedGeometryNodeTool::redoSetTexCoordPoint (const UndoStepPtr & undoStep)
{
	if (not getTexCoord ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (texCoord ()), nullptr);
	   return;
	}

	Editor () .redoSetTexCoordPoint (getTexCoord (), undoStep);
}

void
X3DComposedGeometryNodeTool::redoSetNormalVector (const UndoStepPtr & undoStep)
{
	if (not getNormal ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (normal ()), nullptr);
	   return;
	}

	Editor () .redoSetNormalVector (getNormal (), undoStep);
}

void
X3DComposedGeometryNodeTool::redoSetCoordPoint (const UndoStepPtr & undoStep)
{
	if (not getCoord ())
	{
		undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
		undoStep -> addUndoFunction (&SFNode::setValue, std::ref (coord ()), nullptr);
	   return;
	}

	Editor () .redoSetCoordPoint (getCoord (), undoStep);
}

} // X3D
} // titania
