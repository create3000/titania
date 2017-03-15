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

#include "X3DNodeTool.h"

#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Editing/Undo/UndoStepContainer.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

X3DNodeTool::Fields::Fields () :
	undo_changed (new UndoStepContainerPtr ())
{ }

X3DNodeTool::X3DNodeTool () :
         X3DNode (),
	  X3DBaseTool (),
          fields (),
	transformTool ()
{
	addType (X3DConstants::X3DNodeTool);

	addField (outputOnly , "undo_changed", undo_changed ());

	addChildObjects (transformTool);
}

void
X3DNodeTool::setTransformTool (const X3DWeakPtr <X3DTransformNode> & value)
{
	value -> addTool ();

	transformTool = X3DWeakPtr <X3DTransformNodeTool> (value);

	transformTool -> isActive () .addInterest (&X3DNodeTool::set_active, this);
}

void
X3DNodeTool::setChanging (const X3DPtr <X3D::X3DNode> & node, const bool value)
{
	try
	{
		const auto   tool          = X3DPtr <X3D::X3DNodeTool> (node);
		const auto & transformTool = tool -> getTransformTool ();

		transformTool -> setChanging (value);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DNodeTool::set_active (const bool active)
{
	const auto & selection = getBrowser () -> getSelection ();

	if (active)
	{
		for (const auto & node : selection -> getSelectGeometry () ? selection -> getGeometries () : selection -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> beginUndo ();
		}
	}
	else
	{
		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Edit %s"), getTypeName () .c_str ()));

		for (const auto & node : selection -> getSelectGeometry () ? selection -> getGeometries () : selection -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> endUndo (undoStep);
		}

		if (not undoStep -> isEmpty ())
			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
	}
}

X3DNodeTool::~X3DNodeTool ()
{ }

} // X3D
} // titania
