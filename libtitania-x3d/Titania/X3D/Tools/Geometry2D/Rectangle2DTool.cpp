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

#include "Rectangle2DTool.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

Rectangle2DTool::Fields::Fields ()
{ }

Rectangle2DTool::Rectangle2DTool (X3DBaseNode* const node) :
	        X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	        Rectangle2D (node -> getExecutionContext ()),
	        X3DBaseTool (node),
	X3DGeometryNodeTool (),
	             fields (),
	          startSize ()
{
	addType (X3DConstants::Rectangle2DTool);

	addField (inputOutput, "toolType",   toolType ());
	addField (inputOutput, "normalTool", normalTool ());
	addField (inputOutput, "coordTool",  coordTool ());

	toolType () = "TRANSFORM";
}

void
Rectangle2DTool::initialize ()
{
	X3DGeometryNodeTool::initialize ();

	getTransformTools () .addInterest (&Rectangle2DTool::set_transform_tools, this);
}

void
Rectangle2DTool::set_transform_tools ()
{
	const auto & transformTool = getTransformTools () [0];

	size () .addInterest (&Rectangle2DTool::set_size, this);

	transformTool -> scaleZAxis ()     = false;
	transformTool -> scaleZBackAxis () = false;
	transformTool -> scaleFromEdge ()  = false;

	set_size ();
}

void
Rectangle2DTool::set_size ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> scale () .removeInterest (&Rectangle2DTool::set_scale, this);
	transformTool -> scale () .addInterest (&Rectangle2DTool::connectScale, this);

	transformTool -> scale () = Vector3f (size () .getX (), size () .getY (), 1e-6);
}

void
Rectangle2DTool::set_scale ()
{
	const auto & transformTool = getTransformTools () [0];

	size () .removeInterest (&Rectangle2DTool::set_size, this);
	size () .addInterest (&Rectangle2DTool::connectSize, this);

	size () = Vector2f (transformTool -> scale () .getX (), transformTool -> scale () .getY ());
}

void
Rectangle2DTool::connectSize (const SFVec2f & field)
{
	field .removeInterest (&Rectangle2DTool::connectSize, this);
	field .addInterest (&Rectangle2DTool::set_size, this);
}

void
Rectangle2DTool::connectScale (const SFVec3f & field)
{
	field .removeInterest (&Rectangle2DTool::connectScale, this);
	field .addInterest (&Rectangle2DTool::set_scale, this);
}

void
Rectangle2DTool::beginUndo ()
{
	startSize = size ();
}

void
Rectangle2DTool::endUndo (const UndoStepPtr & undoStep)
{
	if (size () not_eq startSize)
	{
		undoStep -> addUndoFunction (&SFVec2f::setValue, std::ref (size ()), startSize);
		undoStep -> addUndoFunction (&Rectangle2DTool::setChanging, X3DPtr <Rectangle2D> (this), 0, true);

		undoStep -> addRedoFunction (&Rectangle2DTool::setChanging, X3DPtr <Rectangle2D> (this), 0, true);
		undoStep -> addRedoFunction (&SFVec2f::setValue, std::ref (size ()), size ());
	}
}

void
Rectangle2DTool::dispose ()
{
	__LOG__ << std::endl;

	X3DGeometryNodeTool::dispose ();
}

Rectangle2DTool::~Rectangle2DTool ()
{ }

} // X3D
} // titania
