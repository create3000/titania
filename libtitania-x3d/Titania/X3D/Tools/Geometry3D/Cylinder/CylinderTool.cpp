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

#include "CylinderTool.h"

#include "../../../Execution/X3DExecutionContext.h"

#include "../../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

CylinderTool::Fields::Fields ()
{ }

CylinderTool::CylinderTool (X3DBaseNode* const node) :
	        X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	           Cylinder (node -> getExecutionContext ()),
	        X3DBaseTool (node),
	X3DGeometryNodeTool (),
	             fields (),
	        startHeight (0),
	        startRadius (0)
{
	addType (X3DConstants::CylinderTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
CylinderTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getTransformTools () .addInterest (&CylinderTool::set_transform_tools, this);
}

void
CylinderTool::set_transform_tools ()
{
	const auto & transformTool = getTransformTools () [0];

	height () .addInterest (&CylinderTool::set_height, this);
	radius () .addInterest (&CylinderTool::set_radius, this);

	transformTool -> scaleUniform ()  = false;
	transformTool -> scaleFromEdge () = false;
	transformTool -> connectedAxes () = { "XZ", "ZX" };

	set_height ();
	set_radius ();
}

void
CylinderTool::set_height ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> scale () .removeInterest (&CylinderTool::set_scale, this);
	transformTool -> scale () .addInterest (&CylinderTool::connectScale, this);

	transformTool -> scale () .setY (height ());
}

void
CylinderTool::set_radius ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> scale () .removeInterest (&CylinderTool::set_scale, this);
	transformTool -> scale () .addInterest (&CylinderTool::connectScale, this);

	const float  diameter = 2 * radius ();

	transformTool -> scale () .setX (diameter);
	transformTool -> scale () .setZ (diameter);
}

void
CylinderTool::set_scale ()
{
	const auto & transformTool = getTransformTools () [0];

	height () .removeInterest (&CylinderTool::set_height, this);
	height () .addInterest (&CylinderTool::connectHeight, this);

	radius () .removeInterest (&CylinderTool::set_radius, this);
	radius () .addInterest (&CylinderTool::connectRadius, this);

	height () = transformTool -> scale () .getY ();
	radius () = transformTool -> scale () .getX () / 2;
}

void
CylinderTool::connectHeight (const SFFloat & field)
{
	field .removeInterest (&CylinderTool::connectHeight, this);
	field .addInterest (&CylinderTool::set_height, this);
}

void
CylinderTool::connectRadius (const SFFloat & field)
{
	field .removeInterest (&CylinderTool::connectRadius, this);
	field .addInterest (&CylinderTool::set_radius, this);
}

void
CylinderTool::connectScale (const SFVec3f & field)
{
	field .removeInterest (&CylinderTool::connectScale, this);
	field .addInterest (&CylinderTool::set_scale, this);
}

void
CylinderTool::beginUndo ()
{
	startHeight = height ();
	startRadius = radius ();
}

void
CylinderTool::endUndo (const UndoStepPtr & undoStep)
{
	if (height () not_eq startHeight or
	    radius () not_eq startRadius)
	{
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (radius ()), startRadius);
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (height ()), startHeight);
		undoStep -> addUndoFunction (&CylinderTool::setChanging, X3DPtr <Cylinder> (this), 0, true);

		undoStep -> addRedoFunction (&CylinderTool::setChanging, X3DPtr <Cylinder> (this), 0, true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (height ()), height ());
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (radius ()), radius ());
	}
}

void
CylinderTool::dispose ()
{
	__LOG__ << std::endl;

	X3DGeometryNodeTool::dispose ();
}

CylinderTool::~CylinderTool ()
{ }

} // X3D
} // titania
