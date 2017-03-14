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

#include "X3DViewpointNodeTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"

#include "../../Editing/Undo/UndoStepContainer.h"

#include "../Grouping/TransformTool.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

X3DViewpointNodeTool::X3DViewpointNodeTool () :
	   X3DViewpointNode (),
	X3DBindableNodeTool (),
	   X3DBoundedObject (),
	      startPosition (),
	   startOrientation ()
{
	addType (X3DConstants::X3DViewpointNodeTool);

	addChildObjects (bboxSize (), bboxCenter ());
}

void
X3DViewpointNodeTool::initialize ()
{
	X3DBindableNodeTool::initialize ();
	X3DBoundedObject::initialize ();

	requestAsyncLoad ({ get_tool ("ViewpointTool.x3dv") .str () });
}

void
X3DViewpointNodeTool::realize ()
{
	try
	{
		const auto transformTool = getInlineNode () -> getExportedNode <Transform> ("Transform");

		transformTool -> addTool ();
		transformTool -> setField <MFString> ("tools",         MFString ({ "MOVE", "ROTATE" }));
		transformTool -> setField <SFBool>   ("displayCenter", false);

		transformTool -> getField <SFBool> ("isActive") .addInterest (&X3DViewpointNodeTool::set_active, this);

		getToolNode () -> setField <SFNode> ("viewpoint", getNode <X3DViewpointNode> ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DViewpointNodeTool::set_active (const bool active)
{
	if (active)
	{
		for (const auto & node : getBrowser () -> getSelection () -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> beginUndo ();
		}
	}
	else
	{
		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Edit %s"), getTypeName () .c_str ()));

		for (const auto & node : getBrowser () -> getSelection () -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> endUndo (undoStep);
		}

		if (not undoStep -> isEmpty ())
			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
	}
}

Box3d
X3DViewpointNodeTool::getBBox () const
{
	if (getBrowser () -> getDisplayTools () .top ())
		return getInlineNode () -> getBBox ();

	return Box3d ();
}

void
X3DViewpointNodeTool::beginUndo ()
{
	startPosition    = getPosition ();
	startOrientation = getOrientation ();
}

void
X3DViewpointNodeTool::endUndo (const UndoStepPtr & undoStep)
{
	if (getPosition ()    not_eq startPosition or
	    getOrientation () not_eq startOrientation)
	{
		undoStep -> addUndoFunction (&X3DViewpointNode::setPosition,     X3DPtr <X3D::X3DViewpointNode> (this), startPosition);
		undoStep -> addUndoFunction (&X3DViewpointNode::setOrientation,  X3DPtr <X3D::X3DViewpointNode> (this), startOrientation);
		undoStep -> addUndoFunction (&X3DViewpointNodeTool::setChanging, X3DPtr <X3D::X3DViewpointNode> (this), true);

		undoStep -> addRedoFunction (&X3DViewpointNodeTool::setChanging, X3DPtr <X3D::X3DViewpointNode> (this), true);
		undoStep -> addRedoFunction (&X3DViewpointNode::setPosition,     X3DPtr <X3D::X3DViewpointNode> (this), getPosition ());
		undoStep -> addRedoFunction (&X3DViewpointNode::setOrientation,  X3DPtr <X3D::X3DViewpointNode> (this), getOrientation ());
	}
}

void
X3DViewpointNodeTool::setChanging (const X3DPtr <X3D::X3DViewpointNode> & viewpoint, const bool value)
{
	try
	{
		const X3DPtr <X3D::X3DViewpointNodeTool> viewpointTool (viewpoint);

		const auto transformTool = viewpointTool -> getInlineNode () -> getExportedNode <TransformTool> ("Transform");

		transformTool -> setChanging (value);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DViewpointNodeTool::addTool ()
{
	try
	{
		const auto transformTool = getInlineNode () -> getExportedNode <Transform> ("Transform");

		transformTool -> setField <SFBool> ("enabled", true);

		getToolNode () -> setField <SFBool> ("selected", true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DViewpointNodeTool::removeTool (const bool really)
{
	if (really)
		X3DBindableNodeTool::removeTool ();

	else
	{
		try
		{
			const auto transformTool = getInlineNode () -> getExportedNode <Transform> ("Transform");

			transformTool -> setField <SFBool> ("enabled", false);

			getToolNode () -> setField <SFBool> ("selected", false);
		}
		catch (const X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
}

void
X3DViewpointNodeTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DViewpointNode> () -> traverse (type, renderObject);

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	// Tool

	X3DToolObject::traverse (type, renderObject);
}

void
X3DViewpointNodeTool::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DBindableNodeTool::dispose ();

	removeChildObjects (bboxSize (), bboxCenter ());
}

} // X3D
} // titania
