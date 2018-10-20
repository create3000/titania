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

#include "Circle2DTool.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

Circle2DTool::Fields::Fields ()
{ }

Circle2DTool::Circle2DTool (X3DBaseNode* const node) :
	            X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	               Circle2D (node -> getExecutionContext ()),
	            X3DBaseTool (node),
	X3DLineGeometryNodeTool (),
	                 fields (),
	            startRadius (0)
{
	addType (X3DConstants::Circle2DTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
Circle2DTool::initialize ()
{
	X3DLineGeometryNodeTool::initialize ();

	getTransformTools () .addInterest (&Circle2DTool::set_transform_tools, this);
}

void
Circle2DTool::set_transform_tools ()
{
	const auto & transformTool = getTransformTools () [0];

	radius () .addInterest (&Circle2DTool::set_radius, this);

	transformTool -> scaleXAxis ()     = false;
	transformTool -> scaleYAxis ()     = false;
	transformTool -> scaleZAxis ()     = false;
	transformTool -> scaleXBackAxis () = false;
	transformTool -> scaleYBackAxis () = false;
	transformTool -> scaleZBackAxis () = false;
	transformTool -> scaleFromEdge ()  = false;

	set_radius ();
}

void
Circle2DTool::set_radius ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> scale () .removeInterest (&Circle2DTool::set_scale, this);
	transformTool -> scale () .addInterest (&Circle2DTool::connectScale, this);

	const float diameter = 2 * radius ();

	transformTool -> scale () = Vector3f (diameter, diameter, 1e-6);
}

void
Circle2DTool::set_scale ()
{
	const auto & transformTool = getTransformTools () [0];

	radius () .removeInterest (&Circle2DTool::set_radius, this);
	radius () .addInterest (&Circle2DTool::connectRadius, this);

	radius () = transformTool -> scale () .getX () / 2;
}

void
Circle2DTool::connectRadius (const SFFloat & field)
{
	field .removeInterest (&Circle2DTool::connectRadius, this);
	field .addInterest (&Circle2DTool::set_radius, this);
}

void
Circle2DTool::connectScale (const SFVec3f & field)
{
	field .removeInterest (&Circle2DTool::connectScale, this);
	field .addInterest (&Circle2DTool::set_scale, this);
}

void
Circle2DTool::beginUndo ()
{
	startRadius = radius ();
}

void
Circle2DTool::endUndo (const UndoStepPtr & undoStep)
{
	if (radius () not_eq startRadius)
	{
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (radius ()), startRadius);
		undoStep -> addUndoFunction (&Circle2DTool::setChanging, X3DPtr <Circle2D> (this), 0, true);

		undoStep -> addRedoFunction (&Circle2DTool::setChanging, X3DPtr <Circle2D> (this), 0, true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (radius ()), radius ());
	}
}

void
Circle2DTool::dispose ()
{
	__LOG__ << std::endl;

	X3DLineGeometryNodeTool::dispose ();
}

Circle2DTool::~Circle2DTool ()
{ }

} // X3D
} // titania
