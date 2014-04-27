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

#include "TransformTool.h"

#include "../ToolColors.h"

#include "../../Bits/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/PolygonModeContainer.h"

namespace titania {
namespace X3D {

TransformTool::TransformTool (Transform* const node) :
	                    X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	        X3DBaseTool <Transform> (node),
	X3DGroupingNodeTool <Transform> (ToolColors::GREEN),
	                   parentMatrix (),
	                         matrix (),
	                 interestEvents (node -> isTainted ())
{ }

void
TransformTool::initialize ()
{
	X3DBaseTool <Transform>::initialize ();

	requestAsyncLoad ({ get_tool ("TransformTool.x3dv") .str () });
}

void
TransformTool::realize ()
{
	getNode () -> addInterest (this, &TransformTool::eventsProcessed);

	try
	{
		getToolNode () -> getField ("isActive") -> addInterest (getBrowser () -> getSelection () -> isActive ());

		getToolNode () -> setField <SFNode> ("transform", getNode ());
	}
	catch (const X3DError & error)
	{ }
}

// Functions for grouping TransformTools together

void
TransformTool::addAbsoluteMatrix (const Matrix4d & absoluteMatrix)
{
	++ interestEvents;

	getNode () -> setMatrix (Matrix4d (getMatrix ()) * parentMatrix * absoluteMatrix * ~parentMatrix);
}

void
TransformTool::setMatrix (const Matrix4d & matrix)
{
	++ interestEvents;

	getNode () -> setMatrix (matrix);
}

void
TransformTool::setMatrixWithCenter (const Matrix4d & matrix, const Vector3f & center)
{
	++ interestEvents;

	getNode () -> setMatrixWithCenter (matrix, center);
}

void
TransformTool::eventsProcessed ()
{
	if (interestEvents)
		-- interestEvents;

	else
	{
		const auto differenceMatrix = ~(matrix * parentMatrix) * Matrix4d (getMatrix ()) * parentMatrix;

		for (const auto & node : getBrowser () -> getSelection () -> getChildren ())
		{
			if (node == this)
				continue;

			const auto tool = dynamic_cast <TransformTool*> (node .getValue ());

			if (tool)
				tool -> addAbsoluteMatrix (differenceMatrix);
		}
	}
}

// Traverse

void
TransformTool::reshape ()
{
	try
	{
		const auto bbox = getNode () -> X3DGroupingNode::getBBox ();

		getToolNode () -> setField <SFMatrix4f> ("cameraSpaceMatrix", getCameraSpaceMatrix (),       true);
		getToolNode () -> setField <SFMatrix4f> ("modelViewMatrix",   getModelViewMatrix () .get (), true);
		getToolNode () -> setField <SFVec3f>    ("bboxSize",          bbox .size (),                 true);
		getToolNode () -> setField <SFVec3f>    ("bboxCenter",        bbox .center (),               true);

		getBrowser () -> processEvents ();
	}
	catch (const X3DError & error)
	{ }
}

void
TransformTool::traverse (const TraverseType type)
{
	getNode () -> traverse (type);

	// Remember matrices

	if (type == TraverseType::CAMERA)
	{
		parentMatrix = getModelViewMatrix () .get ();
		matrix       = getMatrix ();
	}

	// Tool

	getCurrentLayer () -> getLocalObjects () .emplace_back (new PolygonModeContainer (GL_FILL));

	getModelViewMatrix () .push ();
	getModelViewMatrix () .mult_left (getMatrix ());

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	X3DToolObject::traverse (type);

	getModelViewMatrix () .pop ();

	getCurrentLayer () -> getLocalObjects () .pop_back ();
}

} // X3D
} // titania
