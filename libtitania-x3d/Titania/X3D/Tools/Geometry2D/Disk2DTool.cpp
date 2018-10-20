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

#include "Disk2DTool.h"

#include "../../Components/Grouping/Transform.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Grouping/X3DTransformNodeTool.h"
#include "../Rendering/CoordinateTool.h"
#include "../ToolColors.h"

namespace titania {
namespace X3D {

Disk2DTool::Fields::Fields ()
{ }

Disk2DTool::Disk2DTool (X3DBaseNode* const node) :
	            X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                 Disk2D (node -> getExecutionContext ()),
	            X3DBaseTool (node),
	    X3DGeometryNodeTool (),
	X3DLineGeometryNodeTool (),
	                 fields (),
	       startInnerRadius (0),
	       startOuterRadius (0)
{
	addType (X3DConstants::Disk2DTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
Disk2DTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (&Disk2DTool::set_loadState, this);

	getTransformTools () .addInterest (&Disk2DTool::set_transform_tools, this);
}

void
Disk2DTool::set_loadState ()
{
	// TransformTool 2

	setTransformTool (1, getCoordinateTool () -> getInlineNode () -> getExportedNode <Transform> ("TransformTool2"));
}

void
Disk2DTool::set_transform_tools ()
{
	if (getTransformTools () .size () not_eq 2)
		return;

	const auto & transformTool0 = getTransformTools () [0];
	const auto & transformTool1 = getTransformTools () [1];

	transformTool0 -> color () = ToolColors::BLUE;

	transformTool1 -> grouping ()      = false;
	transformTool1 -> tools ()         = { "SCALE" };
	transformTool1 -> displayCenter () = false;
	transformTool1 -> displayBBox ()   = false;
	transformTool1 -> color ()         = ToolColors::DARK_BLUE;

	// Fields

	innerRadius () .addInterest (&Disk2DTool::set_innerRadius, this);
	outerRadius () .addInterest (&Disk2DTool::set_outerRadius, this);

	transformTool0 -> scaleXAxis ()     = false;
	transformTool0 -> scaleYAxis ()     = false;
	transformTool0 -> scaleZAxis ()     = false;
	transformTool0 -> scaleXBackAxis () = false;
	transformTool0 -> scaleYBackAxis () = false;
	transformTool0 -> scaleZBackAxis () = false;
	transformTool0 -> scaleFromEdge ()  = false;

	transformTool1 -> scaleXAxis ()     = false;
	transformTool1 -> scaleYAxis ()     = false;
	transformTool1 -> scaleZAxis ()     = false;
	transformTool1 -> scaleXBackAxis () = false;
	transformTool1 -> scaleYBackAxis () = false;
	transformTool1 -> scaleZBackAxis () = false;
	transformTool1 -> scaleFromEdge ()  = false;

	set_innerRadius ();
	set_outerRadius ();
}

void
Disk2DTool::set_innerRadius ()
{
	const auto & transformTool0 = getTransformTools () [0];

	transformTool0 -> scale () .removeInterest (&Disk2DTool::set_scale1, this);
	transformTool0 -> scale () .addInterest (&Disk2DTool::connectScale1, this);

	const float diameter = 2 * innerRadius ();

	transformTool0 -> scale () = Vector3f (diameter, diameter, 1e-6);
}

void
Disk2DTool::set_outerRadius ()
{
	const auto & transformTool1 = getTransformTools () [1];

	transformTool1 -> scale () .removeInterest (&Disk2DTool::set_scale2, this);
	transformTool1 -> scale () .addInterest (&Disk2DTool::connectScale2, this);

	const float diameter = 2 * outerRadius ();

	transformTool1 -> scale () = Vector3f (diameter, diameter, 1e-6);
}

void
Disk2DTool::set_scale1 ()
{
	const auto & transformTool0 = getTransformTools () [0];

	innerRadius () .removeInterest (&Disk2DTool::set_innerRadius, this);
	innerRadius () .addInterest (&Disk2DTool::connectInnerRadius, this);

	innerRadius () = transformTool0 -> scale () .getX () / 2;
}

void
Disk2DTool::set_scale2 ()
{
	const auto & transformTool1 = getTransformTools () [1];

	outerRadius () .removeInterest (&Disk2DTool::set_outerRadius, this);
	outerRadius () .addInterest (&Disk2DTool::connectOuterRadius, this);

	outerRadius () = transformTool1 -> scale () .getX () / 2;
}

void
Disk2DTool::connectInnerRadius (const SFFloat & field)
{
	field .removeInterest (&Disk2DTool::connectInnerRadius, this);
	field .addInterest (&Disk2DTool::set_innerRadius, this);
}

void
Disk2DTool::connectOuterRadius (const SFFloat & field)
{
	field .removeInterest (&Disk2DTool::connectOuterRadius, this);
	field .addInterest (&Disk2DTool::set_outerRadius, this);
}

void
Disk2DTool::connectScale1 (const SFVec3f & field)
{
	field .removeInterest (&Disk2DTool::connectScale1, this);
	field .addInterest (&Disk2DTool::set_scale1, this);
}

void
Disk2DTool::connectScale2 (const SFVec3f & field)
{
	field .removeInterest (&Disk2DTool::connectScale2, this);
	field .addInterest (&Disk2DTool::set_scale2, this);
}

void
Disk2DTool::beginUndo ()
{
	startInnerRadius = innerRadius ();
	startOuterRadius = outerRadius ();
}

void
Disk2DTool::endUndo (const UndoStepPtr & undoStep)
{
	if (innerRadius () not_eq startInnerRadius or
	    outerRadius () not_eq startOuterRadius)
	{
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (outerRadius ()), startOuterRadius);
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (innerRadius ()), startInnerRadius);
		undoStep -> addUndoFunction (&Disk2DTool::setChanging, X3DPtr <Disk2D> (this), 0, true);
		undoStep -> addUndoFunction (&Disk2DTool::setChanging, X3DPtr <Disk2D> (this), 1, true);

		undoStep -> addRedoFunction (&Disk2DTool::setChanging, X3DPtr <Disk2D> (this), 1, true);
		undoStep -> addRedoFunction (&Disk2DTool::setChanging, X3DPtr <Disk2D> (this), 0, true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (innerRadius ()), innerRadius ());
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (outerRadius ()), outerRadius ());
	}
}

void
Disk2DTool::dispose ()
{
	__LOG__ << std::endl;

	X3DGeometryNodeTool::dispose ();
}

Disk2DTool::~Disk2DTool ()
{ }

} // X3D
} // titania
