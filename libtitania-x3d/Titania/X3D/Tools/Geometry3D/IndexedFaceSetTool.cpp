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

#include "IndexedFaceSetTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Editing/FaceSelection.h"

#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/Rendering/Coordinate.h"

namespace titania {
namespace X3D {

IndexedFaceSetTool::IndexedFaceSetTool (IndexedFaceSet* const node) :
	                                 X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                X3DBaseTool <IndexedFaceSet> (node),
	X3DComposedGeometryNodeTool <IndexedFaceSet> (),
	                                   selection (new FaceSelection ())
{
	addType (X3DConstants::IndexedFaceSetTool);
}

void
IndexedFaceSetTool::initialize ()
{
	X3DComposedGeometryNodeTool <IndexedFaceSet>::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &IndexedFaceSetTool::set_loadState);
	getCoord () .addInterest (this, &IndexedFaceSetTool::set_coord);

	selection -> setGeometry (getNode ());

	set_loadState ();
	set_coord ();
}

void
IndexedFaceSetTool::set_loadState ()
{
	try
	{
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");

		touchSensor -> hitPoint_changed () .addInterest (this, &IndexedFaceSetTool::set_hitPoint);
		touchSensor -> touchTime () .addInterest (this, &IndexedFaceSetTool::set_touchTime);
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_coord ()
{
	selection -> setCoord (getCoord ());
}

void
IndexedFaceSetTool::set_hitPoint (const X3D::Vector3f & hitPoint)
{
	__LOG__ << std::endl;

	try
	{
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");

		// Determine face and faces

		selection -> setHitPoint (hitPoint, touchSensor -> hitTriangle_changed ());

		if (selection -> isEmpty ())
			return;

		// Set selected point

		set_selection (getCoord () -> get1Point (selection -> getIndices () [0]));

//		if (touchSensor -> isActive () and (keys .shift () or keys .control ()))
//		{
//			rightPaintSelecion = true;
//			set_right_touchTime ();
//		}
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_touchTime ()
{
	__LOG__ << std::endl;
}

void
IndexedFaceSetTool::set_selection (const X3D::Vector3f & point)
{
	__LOG__ << point << std::endl;

	try
	{
		const auto coord    = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("ActivePointCoord");
		const auto vertices = selection -> getVertices (selection -> getFace () .first);

		if (vertices .size () < 3)
			return;

//		size_t p = 0;
//
//		for (const auto & i : vertices)
//			coord -> point () .set1Value (p ++, getCoord () -> get1Point (coordIndex () [i]));

		coord -> point () .set1Value (0, point);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
IndexedFaceSetTool::dispose ()
{
	selection .reset  ();

	X3DComposedGeometryNodeTool <IndexedFaceSet>::dispose ();
}

} // X3D
} // titania
