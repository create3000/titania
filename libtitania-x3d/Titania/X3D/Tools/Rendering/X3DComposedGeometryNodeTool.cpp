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

#include "X3DComposedGeometryNodeTool.h"

#include "../Rendering/CoordinateTool.h"

#include "../../Components/Geospatial/GeoCoordinate.h"
#include "../../Components/NURBS/CoordinateDouble.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"

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
X3DComposedGeometryNodeTool::addCoordUndoFunction (const UndoStepPtr & undoStep) const
{
	switch (getCoord () -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			X3DPtr <Coordinate> coordinate (getCoord ());

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::CoordinateDouble:
		{
			X3DPtr <CoordinateDouble> coordinate (getCoord ());

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::GeoCoordinate:
		{
			X3DPtr <GeoCoordinate> coordinate (getCoord ());

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		default:
			break;
	}
}

void
X3DComposedGeometryNodeTool::addCoordRedoFunction (const UndoStepPtr & undoStep) const
{
	switch (getCoord () -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			X3DPtr <Coordinate> coordinate (getCoord ());

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::CoordinateDouble:
		{
			X3DPtr <CoordinateDouble> coordinate (getCoord ());

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::GeoCoordinate:
		{
			X3DPtr <GeoCoordinate> coordinate (getCoord ());

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		default:
			break;
	}
}

} // X3D
} // titania
