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

#include "Arc2DTool.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

Arc2DTool::Fields::Fields ()
{ }

Arc2DTool::Arc2DTool (X3DBaseNode* const node) :
	            X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                  Arc2D (node -> getExecutionContext ()),
	            X3DBaseTool (node),
	X3DLineGeometryNodeTool (),
	                 fields ()
{
	addType (X3DConstants::Arc2DTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
Arc2DTool::initialize ()
{
	X3DLineGeometryNodeTool::initialize ();

	getTransformTool () .addInterest (&Arc2DTool::set_transform_tool, this);
}

void
Arc2DTool::set_transform_tool ()
{
	radius () .addInterest (&Arc2DTool::set_radius, this);

	getTransformTool () -> scaleXAxis ()     = false;
	getTransformTool () -> scaleYAxis ()     = false;
	getTransformTool () -> scaleZAxis ()     = false;
	getTransformTool () -> scaleXBackAxis () = false;
	getTransformTool () -> scaleYBackAxis () = false;
	getTransformTool () -> scaleZBackAxis () = false;
	getTransformTool () -> scaleFromEdge ()  = false;

	set_radius ();
}

void
Arc2DTool::set_radius ()
{
	getTransformTool () -> scale () .removeInterest (&Arc2DTool::set_scale, this);
	getTransformTool () -> scale () .addInterest (&Arc2DTool::connectScale, this);

	const float diameter = 2 * radius ();

	getTransformTool () -> scale () = Vector3f (diameter, diameter, 1e-6);
}

void
Arc2DTool::set_scale ()
{
	radius () .removeInterest (&Arc2DTool::set_radius, this);
	radius () .addInterest (&Arc2DTool::connectRadius, this);

	radius () = getTransformTool () -> scale () .getX () / 2;
}

void
Arc2DTool::connectRadius (const SFFloat & field)
{
	field .removeInterest (&Arc2DTool::connectRadius, this);
	field .addInterest (&Arc2DTool::set_radius, this);
}

void
Arc2DTool::connectScale (const SFVec3f & field)
{
	field .removeInterest (&Arc2DTool::connectScale, this);
	field .addInterest (&Arc2DTool::set_scale, this);
}

void
Arc2DTool::beginUndo ()
{
	startRadius = radius ();
}

void
Arc2DTool::endUndo (const UndoStepPtr & undoStep)
{
	if (radius () not_eq startRadius)
	{
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (radius ()), startRadius);
		undoStep -> addUndoFunction (&Arc2DTool::setChanging, X3DPtr <Arc2D> (this), true);

		undoStep -> addRedoFunction (&Arc2DTool::setChanging, X3DPtr <Arc2D> (this), true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (radius ()), radius ());
	}
}

void
Arc2DTool::dispose ()
{
	__LOG__ << std::endl;

	X3DLineGeometryNodeTool::dispose ();
}

Arc2DTool::~Arc2DTool ()
{ }

} // X3D
} // titania
