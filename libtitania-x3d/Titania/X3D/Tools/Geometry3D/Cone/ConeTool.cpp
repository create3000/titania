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

#include "ConeTool.h"

#include "../../../Execution/X3DExecutionContext.h"

#include "../../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

ConeTool::Fields::Fields ()
{ }

ConeTool::ConeTool (X3DBaseNode* const node) :
	        X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	               Cone (node -> getExecutionContext ()),
	        X3DBaseTool (node),
	X3DGeometryNodeTool (),
	             fields (),
	        startHeight (0),
	  startBottomRadius (0)
{
	addType (X3DConstants::ConeTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
ConeTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getTransformTool () .addInterest (&ConeTool::set_transform_tool, this);
}

void
ConeTool::set_transform_tool ()
{
	height ()       .addInterest (&ConeTool::set_height, this);
	bottomRadius () .addInterest (&ConeTool::set_bottomRadius, this);

	getTransformTool () -> scaleUniform ()  = false;
	getTransformTool () -> scaleFromEdge () = false;
	getTransformTool () -> connectedAxes () = { "XZ", "ZX" };

	set_height ();
	set_bottomRadius ();
}

void
ConeTool::set_height ()
{
	getTransformTool () -> scale () .removeInterest (&ConeTool::set_scale, this);
	getTransformTool () -> scale () .addInterest (&ConeTool::connectScale, this);

	getTransformTool () -> scale () .setY (height ());
}

void
ConeTool::set_bottomRadius ()
{
	const float diameter = 2 * bottomRadius ();

	getTransformTool () -> scale () .removeInterest (&ConeTool::set_scale, this);
	getTransformTool () -> scale () .addInterest (&ConeTool::connectScale, this);

	getTransformTool () -> scale () .setX (diameter);
	getTransformTool () -> scale () .setZ (diameter);
}

void
ConeTool::set_scale ()
{
	height () .removeInterest (&ConeTool::set_height, this);
	height () .addInterest (&ConeTool::connectHeight, this);

	bottomRadius () .removeInterest (&ConeTool::set_bottomRadius, this);
	bottomRadius () .addInterest (&ConeTool::connectBottomRadius, this);

	height ()       = getTransformTool () -> scale () .getY ();
	bottomRadius () = getTransformTool () -> scale () .getX () / 2;
}

void
ConeTool::connectHeight (const SFFloat & field)
{
	field .removeInterest (&ConeTool::connectHeight, this);
	field .addInterest (&ConeTool::set_height, this);
}

void
ConeTool::connectBottomRadius (const SFFloat & field)
{
	field .removeInterest (&ConeTool::connectBottomRadius, this);
	field .addInterest (&ConeTool::set_bottomRadius, this);
}

void
ConeTool::connectScale (const SFVec3f & field)
{
	field .removeInterest (&ConeTool::connectScale, this);
	field .addInterest (&ConeTool::set_scale, this);
}

void
ConeTool::beginUndo ()
{
	startHeight       = height ();
	startBottomRadius = bottomRadius ();
}

void
ConeTool::endUndo (const UndoStepPtr & undoStep)
{
	if (height ()       not_eq startHeight or
	    bottomRadius () not_eq startBottomRadius)
	{
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (bottomRadius ()), startBottomRadius);
		undoStep -> addUndoFunction (&SFFloat::setValue, std::ref (height ()), startHeight);
		undoStep -> addUndoFunction (&ConeTool::setChanging, X3DPtr <Cone> (this), true);

		undoStep -> addRedoFunction (&ConeTool::setChanging, X3DPtr <Cone> (this), true);
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (height ()), height ());
		undoStep -> addRedoFunction (&SFFloat::setValue, std::ref (bottomRadius ()), bottomRadius ());
	}
}

void
ConeTool::dispose ()
{
	__LOG__ << std::endl;

	X3DGeometryNodeTool::dispose ();
}

ConeTool::~ConeTool ()
{ }

} // X3D
} // titania
