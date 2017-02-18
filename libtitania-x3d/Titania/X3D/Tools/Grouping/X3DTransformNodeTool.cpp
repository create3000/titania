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

#include "X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

X3DTransformNodeTool::Fields::Fields () :
	     bbox (new SFBool (true)),
	 isActive (new SFBool ()),
	touchTime (new SFTime ())
{ }

X3DTransformNodeTool::X3DTransformNodeTool () :
	            X3DTransformNode (),
	X3DTransformMatrix3DNodeTool (ToolColors::GREEN),
	        transformationMatrix (),
	                      matrix (),
	                    changing (false)
{
	addType (X3DConstants::X3DTransformNodeTool);

	addField (inputOutput, "bbox",      bbox ());
	addField (outputOnly,  "isActive",  isActive ());
	addField (outputOnly,  "touchTime", touchTime ());

	setCameraObject (true);
}

void
X3DTransformNodeTool::initialize ()
{
	X3DBaseTool::initialize ();

	requestAsyncLoad ({ get_tool ("TransformTool.x3dv") .str () });
}

void
X3DTransformNodeTool::realize ()
{
	getNode <X3DTransformNode> () -> addInterest (&X3DTransformNodeTool::eventsProcessed, this);

	try
	{
		getBrowser ()  -> getTransformToolOptions () -> toolMode ()  .addInterest (getToolNode () -> getField ("toolMode"));
		getBrowser ()  -> getTransformToolOptions () -> snapAngle () .addInterest (getToolNode () -> getField ("snapAngle"));
		getBrowser ()  -> getControlKey () .addInterest (getToolNode () -> getField ("controlKey"));
		getBrowser ()  -> getShiftKey ()   .addInterest (getToolNode () -> getField ("shiftKey"));
		getBrowser ()  -> getAltKey ()     .addInterest (getToolNode () -> getField ("altKey"));
		getToolNode () -> getField ("isActive") -> addInterest (getBrowser () -> getSelection () -> isActive ());
		getToolNode () -> getField ("isActive") -> addInterest (isActive ());
		getToolNode () -> getField ("touchTime") -> addInterest (touchTime ());

		getToolNode () -> setField <SFInt32>  ("toolMode",   getBrowser () -> getTransformToolOptions () -> toolMode ());
		getToolNode () -> setField <SFDouble> ("snapAngle",  getBrowser () -> getTransformToolOptions () -> snapAngle ());
		getToolNode () -> setField <SFBool>   ("controlKey", getBrowser () -> getControlKey ());
		getToolNode () -> setField <SFBool>   ("shiftKey",   getBrowser () -> getShiftKey ());
		getToolNode () -> setField <SFBool>   ("altKey",     getBrowser () -> getAltKey ());
		getToolNode () -> setField <SFNode>   ("transform",  getNode <X3DTransformNode> ());

		bbox () .addInterest (getToolNode () -> getField <SFBool> ("bbox"));

		getToolNode () -> setField <SFBool> ("bbox", bbox ());
	}
	catch (const X3DError & error)
	{ }
}

bool
X3DTransformNodeTool::getKeepCenter () const
{
	try
	{
		return getToolNode () -> getField <SFBool> ("keepCenter_changed");
	}
	catch (const X3DError & error)
	{
		return false;
	}
}

// Functions for grouping X3DTransformNodeTools together

void
X3DTransformNodeTool::setMatrix (const Matrix4d & matrix)
{
	changing = true;

	getNode <X3DTransformNode> () -> setMatrix (matrix);
}

void
X3DTransformNodeTool::setMatrixWithCenter (const Matrix4d & matrix, const Vector3d & center)
{
	changing = true;

	getNode <X3DTransformNode> () -> setMatrixWithCenter (matrix, center);
}

void
X3DTransformNodeTool::setMatrixKeepCenter (const Matrix4d & matrix)
{
	changing = true;

	getNode <X3DTransformNode> () -> setMatrixKeepCenter (matrix);
}

// This function must be virtual, as it is not guarenteed that a X3DPtr has a X3DTransformNodeTool inside.
void
X3DTransformNodeTool::addAbsoluteMatrix (const Matrix4d & absoluteMatrix, const bool keepCenter)
throw (Error <NOT_SUPPORTED>)
{
	try
	{
		const auto matrix = getMatrix () * transformationMatrix * absoluteMatrix * inverse (transformationMatrix);

		if (keepCenter)
			setMatrixKeepCenter (matrix);
		else
			setMatrix (matrix);
	}
	catch (const std::domain_error &)
	{
		// Catch matrix inverse.
	}
}

void
X3DTransformNodeTool::eventsProcessed ()
{
	try
	{
		if (changing)
		{
			changing = false;
			return;
		}

		const auto differenceMatrix = inverse (matrix * transformationMatrix) * getMatrix () * transformationMatrix;

		for (const auto & node : getBrowser () -> getSelection () -> getNodes ())
		{
			try
			{
				if (node == this)
					continue;
	
				const auto transform = dynamic_cast <X3DTransformNode*> (node .getValue ());
	
				if (transform)
					transform -> addAbsoluteMatrix (differenceMatrix, transform -> getKeepCenter ());
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
	catch (const std::domain_error &)
	{
		// Catch matrix inverse.
	}
}

// Traverse

void
X3DTransformNodeTool::reshape (X3DRenderObject* const renderObject)
{
	try
	{
		getBrowser () -> endUpdateForFrame ();

		const auto bbox = getNode <X3DTransformNode> () -> X3DGroupingNode::getBBox ();

		getToolNode () -> setField <SFMatrix4f> ("cameraSpaceMatrix", renderObject -> getCameraSpaceMatrix () .get (), true);
		getToolNode () -> setField <SFMatrix4f> ("modelViewMatrix",   renderObject -> getModelViewMatrix   () .get (), true);
		getToolNode () -> setField <SFVec3f>    ("bboxSize",          bbox .size (),   true);
		getToolNode () -> setField <SFVec3f>    ("bboxCenter",        bbox .center (), true);

		getBrowser () -> processEvents ();
		getBrowser () -> beginUpdateForFrame ();
	}
	catch (const X3DError & error)
	{
		getBrowser () -> beginUpdateForFrame ();
	}
}

void
X3DTransformNodeTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DTransformNode> () -> traverse (type, renderObject);

	// Remember matrices

	if (type == TraverseType::CAMERA)
	{
		transformationMatrix = renderObject -> getModelViewMatrix () .get ();
		matrix               = getMatrix ();
	}

	// Tool

	renderObject -> getModelViewMatrix () .push ();
	renderObject -> getModelViewMatrix () .mult_left (getMatrix ());

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape (renderObject);

	X3DToolObject::traverse (type, renderObject);

	renderObject -> getModelViewMatrix () .pop ();
}

} // X3D
} // titania
