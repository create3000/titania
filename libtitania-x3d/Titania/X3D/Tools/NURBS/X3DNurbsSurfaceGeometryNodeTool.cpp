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

#include "X3DNurbsSurfaceGeometryNodeTool.h"

#include "../../Components/Grouping/Switch.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Components/Rendering/PointSet.h"
#include "../Rendering/CoordinateTool.h"

namespace titania {
namespace X3D {

X3DNurbsSurfaceGeometryNodeTool::X3DNurbsSurfaceGeometryNodeTool () :
	  X3DNurbsSurfaceGeometryNode (),
	X3DParametricGeometryNodeTool (),
	            controlPointLines ()
{
	addType (X3DConstants::X3DNurbsSurfaceGeometryNodeTool);

	addChildObjects (controlPointLines);
}

void
X3DNurbsSurfaceGeometryNodeTool::initialize ()
{
	X3DParametricGeometryNodeTool::initialize ();

	const auto & inlineNode = getCoordinateTool () -> getInlineNode ();

	inlineNode -> checkLoadState () .addInterest (&X3DNurbsSurfaceGeometryNodeTool::set_load_state, this);

	controlPoint () .addInterest (&X3DNurbsSurfaceGeometryNodeTool::set_controlPoint, this);

	getNode <X3DNurbsSurfaceGeometryNode> () -> addInterest (&X3DNurbsSurfaceGeometryNodeTool::eventProcessed, this);

	set_load_state ();
}

void
X3DNurbsSurfaceGeometryNodeTool::set_load_state ()
{
	try
	{
		const auto & inlineNode = getCoordinateTool () -> getInlineNode ();
	
		if (inlineNode -> checkLoadState () not_eq COMPLETE_STATE)
			return;
	
		const auto controlPointSwitch = inlineNode -> getExportedNode <Switch> ("ControlPointSwitch");

		controlPointLines = inlineNode -> getExportedNode <IndexedLineSet> ("ControlPointLines");

		controlPointSwitch -> whichChoice () = true;

		set_controlPoint ();
		eventProcessed ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DNurbsSurfaceGeometryNodeTool::set_controlPoint ()
{
	try
	{
		const auto & inlineNode      = getCoordinateTool () -> getInlineNode ();
		const auto   controlPointSet = inlineNode -> getExportedNode <PointSet> ("ControlPointSet");

		controlPointSet -> coord () = controlPoint ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DNurbsSurfaceGeometryNodeTool::eventProcessed ()
{
	try
	{
		auto & coordIndex = controlPointLines -> coordIndex ();

		coordIndex .clear ();

		const auto & controlPointNode = getControlPoint ();

		if (uOrder () < 2)
			return;
	
		if (vOrder () < 2)
			return;
	
		if (uDimension () < uOrder ())
			return;
	
		if (vDimension () < vOrder ())
			return;
	
		if (not controlPointNode)
			return;
	
		if (controlPointNode -> getSize () not_eq size_t (uDimension () * vDimension ()))
			return;

		for (size_t v = 0, size = vDimension (); v < size; ++ v)
		{
			for (size_t u = 0, size = uDimension () - 1; u < size; ++ u)
			{
				const auto i1 = v * uDimension () + u;
				const auto i2 = v * uDimension () + u + 1;

				coordIndex .emplace_back (i1);
				coordIndex .emplace_back (i2);
				coordIndex .emplace_back (-1);
			}
		}

		for (size_t u = 0, size = uDimension (); u < size; ++ u)
		{
			for (size_t v = 0, size = vDimension () - 1; v < size; ++ v)
			{
				const auto i1 = v * uDimension () + u;
				const auto i2 = (v + 1) * uDimension () + u;

				coordIndex .emplace_back (i1);
				coordIndex .emplace_back (i2);
				coordIndex .emplace_back (-1);
			}
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DNurbsSurfaceGeometryNodeTool::dispose ()
{
	X3DParametricGeometryNodeTool::dispose ();
}

X3DNurbsSurfaceGeometryNodeTool::~X3DNurbsSurfaceGeometryNodeTool ()
{ }

} // X3D
} // titania
