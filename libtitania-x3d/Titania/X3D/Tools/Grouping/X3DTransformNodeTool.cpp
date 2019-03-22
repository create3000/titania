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

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/Tools/TransformToolOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/X_ITE/TouchGroup.h"
#include "../../Editing/Undo/UndoStepContainer.h"
#include "../../Editing/X3DEditor.h"
#include "../../Prototype/X3DProtoDeclarationNode.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Routing/Router.h"

#include "../ToolColors.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

X3DTransformNodeTool::Fields::Fields () :
	       grouping (new SFBool (true)),
	          tools (new MFString ({ "TRANSLATE", "ROTATE", "SCALE" })),
	     scaleXAxis (new SFBool (true)),
	     scaleYAxis (new SFBool (true)),
	     scaleZAxis (new SFBool (true)),
	 scaleXBackAxis (new SFBool (true)),
	 scaleYBackAxis (new SFBool (true)),
	 scaleZBackAxis (new SFBool (true)),
	   scaleUniform (new SFBool (true)),
	  scaleFromEdge (new SFBool (true)),
	scaleFromCenter (new SFBool (true)),
	  connectedAxes (new MFString ()),
	    displayBBox (new SFBool (true)),
	  displayCenter (new SFBool (true)),
	     isPickable (new SFBool (true)),
	          color (new SFColor (ToolColors::GREEN)),
	        grouped (new SFBool (false)),
	       isActive (new SFBool ()),
	      touchTime (new SFTime ())
{ }

X3DTransformNodeTool::X3DTransformNodeTool () :
	              X3DTransformNode (),
	           X3DGroupingNodeTool (ToolColors::GREEN),
	X3DTransformMatrix3DObjectTool (),
	             handlesTouchGroup (),
	                availableTools (),
	                   modelMatrix (),
	                   groupMatrix (),
	                    undoMatrix (),
	                      changing (false)
{
	addType (X3DConstants::X3DTransformNodeTool);

	addField (inputOutput, "grouping",        grouping ());
	addField (inputOutput, "tools",           tools ());
	addField (inputOutput, "scaleXAxis",      scaleXAxis ());
	addField (inputOutput, "scaleYAxis",      scaleYAxis ());
	addField (inputOutput, "scaleZAxis",      scaleZAxis ());
	addField (inputOutput, "scaleXBackAxis",  scaleXBackAxis ());
	addField (inputOutput, "scaleYBackAxis",  scaleYBackAxis ());
	addField (inputOutput, "scaleZBackAxis",  scaleZBackAxis ());
	addField (inputOutput, "scaleUniform",    scaleUniform ());
	addField (inputOutput, "scaleFromEdge",   scaleFromEdge ());
	addField (inputOutput, "scaleFromCenter", scaleFromCenter ());
	addField (inputOutput, "connectedAxes",   connectedAxes ());
	addField (inputOutput, "displayBBox",     displayBBox ());
	addField (inputOutput, "displayCenter",   displayCenter ());
	addField (inputOutput, "isPickable",      isPickable ());
	addField (inputOutput, "color",           color ());
	addField (outputOnly,  "grouped",         grouped ());
	addField (outputOnly,  "isActive",        isActive ());
	addField (outputOnly,  "touchTime",       touchTime ());

	addChildObjects (handlesTouchGroup);

	setCameraObject (true);
}

void
X3DTransformNodeTool::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getBrowser () -> removeTransformTool (this);

	X3DGroupingNodeTool::setExecutionContext (executionContext);

	getBrowser () -> addTransformTool (this);
}

void
X3DTransformNodeTool::initialize ()
{
	if (getExecutionContext () -> isType ({ X3DConstants::X3DPrototypeInstance }))
	{
		X3DGroupingNodeTool::initialize ();
		X3DTransformMatrix3DObjectTool::initialize ();

		setLinetype (LineType::SOLID);
	}
	else
	{
		X3DChildNodeTool::initialize ();
		X3DTransformMatrix3DObjectTool::initialize ();

		grouped ()  .addInterest (&X3DTransformNodeTool::set_grouped, this);
		tools ()    .addInterest (&X3DTransformNodeTool::set_tools,   this);
		isActive () .addInterest (&X3DTransformNodeTool::set_active,  this);
	
		getBrowser () -> addTransformTool (this);
	
		requestAsyncLoad ({ get_tool ("TransformTool.x3dv") .str () });
	
		set_tools ();
	}
}

void
X3DTransformNodeTool::realize ()
{
	try
	{
		getNode <X3DTransformNode> () -> addInterest (&X3DTransformNodeTool::eventsProcessed, this);

		handlesTouchGroup = getInlineNode () -> getExportedNode <TouchGroup> ("HandlesTouchGroup");

		getBrowser ()  -> getTransformToolOptions () -> toolMode ()  .addInterest (getToolNode () -> getField ("toolMode"));
		getBrowser ()  -> getTransformToolOptions () -> snapAngle () .addInterest (getToolNode () -> getField ("snapAngle"));
		getBrowser ()  -> getControlKey () .addInterest (getToolNode () -> getField ("controlKey"));
		getBrowser ()  -> getShiftKey ()   .addInterest (getToolNode () -> getField ("shiftKey"));
		getBrowser ()  -> getAltKey ()     .addInterest (getToolNode () -> getField ("altKey"));
		getToolNode () -> getField ("isActive") -> addInterest (isActive ());
		getToolNode () -> getField ("touchTime") -> addInterest (touchTime ());

		getToolNode () -> setField <SFInt32>  ("toolMode",   getBrowser () -> getTransformToolOptions () -> toolMode ());
		getToolNode () -> setField <SFDouble> ("snapAngle",  getBrowser () -> getTransformToolOptions () -> snapAngle ());
		getToolNode () -> setField <SFBool>   ("controlKey", getBrowser () -> getControlKey ());
		getToolNode () -> setField <SFBool>   ("shiftKey",   getBrowser () -> getShiftKey ());
		getToolNode () -> setField <SFBool>   ("altKey",     getBrowser () -> getAltKey ());
		getToolNode () -> setField <SFNode>   ("transform",  getNode <X3DTransformNode> ());

		tools ()           .addInterest (getToolNode () -> getField <MFString> ("tools"));
		scaleXAxis ()      .addInterest (getToolNode () -> getField <SFBool>   ("scaleXAxis"));
		scaleYAxis ()      .addInterest (getToolNode () -> getField <SFBool>   ("scaleYAxis"));
		scaleZAxis ()      .addInterest (getToolNode () -> getField <SFBool>   ("scaleZAxis"));
		scaleXBackAxis ()  .addInterest (getToolNode () -> getField <SFBool>   ("scaleXBackAxis"));
		scaleYBackAxis ()  .addInterest (getToolNode () -> getField <SFBool>   ("scaleYBackAxis"));
		scaleZBackAxis ()  .addInterest (getToolNode () -> getField <SFBool>   ("scaleZBackAxis"));
		scaleUniform ()    .addInterest (getToolNode () -> getField <SFBool>   ("scaleUniform"));
		scaleFromEdge ()   .addInterest (getToolNode () -> getField <SFBool>   ("scaleFromEdge"));
		scaleFromCenter () .addInterest (getToolNode () -> getField <SFBool>   ("scaleFromCenter"));
		connectedAxes ()   .addInterest (getToolNode () -> getField <MFString> ("connectedAxes"));
		color ()           .addInterest (getToolNode () -> getField <SFColor>  ("color"));
		displayBBox ()     .addInterest (getToolNode () -> getField <SFBool>   ("displayBBox"));
		displayCenter ()   .addInterest (getToolNode () -> getField <SFBool>   ("displayCenter"));

		getToolNode () -> setField <MFString> ("tools",           tools ());
		getToolNode () -> setField <SFBool>   ("scaleXAxis",      scaleXAxis ());
		getToolNode () -> setField <SFBool>   ("scaleYAxis",      scaleYAxis ());
		getToolNode () -> setField <SFBool>   ("scaleZAxis",      scaleZAxis ());
		getToolNode () -> setField <SFBool>   ("scaleXBackAxis",  scaleXBackAxis ());
		getToolNode () -> setField <SFBool>   ("scaleYBackAxis",  scaleYBackAxis ());
		getToolNode () -> setField <SFBool>   ("scaleZBackAxis",  scaleZBackAxis ());
		getToolNode () -> setField <SFBool>   ("scaleUniform",    scaleUniform ());
		getToolNode () -> setField <SFBool>   ("scaleFromEdge",   scaleFromEdge ());
		getToolNode () -> setField <SFBool>   ("scaleFromCenter", scaleFromCenter ());
		getToolNode () -> setField <MFString> ("connectedAxes",   connectedAxes ());
		getToolNode () -> setField <SFColor>  ("color",           color ());
		getToolNode () -> setField <SFBool>   ("displayBBox",     displayBBox ());
		getToolNode () -> setField <SFBool>   ("displayCenter",   displayCenter ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
X3DTransformNodeTool::getPickable (X3DRenderObject* const renderObject) const
{
	try
	{
		if (not isPickable ())
			return false;

		if (renderObject -> getBrowser () -> getToolsPickable () .top ())
			handlesTouchGroup -> enabled () = true;
		else
			handlesTouchGroup -> enabled () = false;

		return true;
	}
	catch (const X3DError & error)
	{
		// __LOG__ << error .what () << std::endl;
		return renderObject -> getBrowser () -> getToolsPickable () .top ();
	}
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
{
	try
	{
		Vector3d t, s;
		Rotation4d r, so;

		auto relativeMatrix = modelMatrix * absoluteMatrix * inverse (modelMatrix);

		// Connected Axes handling
		if (not connectedAxes () .empty ())
		{
			relativeMatrix .get (t, r, s, so);

			for (const auto & connectedAxis : basic::make_const_reverse_range (connectedAxes ()))
			{
				try
				{
					static const std::map <String::value_type, size_t> axes = {
						std::pair ('x', 0),
						std::pair ('y', 1),
						std::pair ('z', 2),
					};
		
					const auto lhs = axes .at (std::tolower (connectedAxis .at (0)));
					const auto rhs = axes .at (std::tolower (connectedAxis .at (1)));

					s [lhs] = s [rhs];
				}
				catch (const std::out_of_range &)
				{ }
			}
	
			relativeMatrix .set (t, r, s, so);
		}

		//

		auto matrix = (grouped () ? groupMatrix : getMatrix ()) * relativeMatrix;

		matrix .get (t, r, s, so);

		if (not availableTools .count (ToolType::TRANSLATE))
			t = Vector3d ();

		if (not availableTools .count (ToolType::ROTATE))
			r = Rotation4d ();

		if (not availableTools .count (ToolType::SCALE))
			s = Vector3d ();

		matrix .set (t, r, s, so);

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
X3DTransformNodeTool::beginUndo ()
{
	undoMatrix = std::pair (getMatrix (), center () .getValue ());
}

void
X3DTransformNodeTool::endUndo (const UndoStepPtr & undoStep)
{
	const auto & startMatrix = undoMatrix .first;
	const auto & startCenter = undoMatrix .second;

	if (startMatrix not_eq getMatrix () or startCenter not_eq center ())
	{
		undoStep -> addUndoFunction (&X3DTransformNode::setMatrixWithCenter,
		                             X3DPtr <X3DTransformNode> (this),
		                             startMatrix,
		                             startCenter);

		undoStep -> addRedoFunction (&X3DTransformNode::setMatrixWithCenter,
		                             X3DPtr <X3DTransformNode> (this),
		                             getMatrix (),
		                             center () .getValue ());
	}
}

void
X3DTransformNodeTool::set_grouped ()
{
	groupMatrix = getMatrix ();
}

void
X3DTransformNodeTool::set_tools ()
{
	static const std::map <std::string, ToolType> toolTypes = {
		std::pair ("TRANSLATE", ToolType::TRANSLATE),
		std::pair ("ROTATE",    ToolType::ROTATE),
		std::pair ("SCALE",     ToolType::SCALE),
	};

	availableTools .clear ();

	for (const auto & tool : basic::make_const_range (tools ()))
	{
		try
		{
			availableTools .emplace (toolTypes .at (tool));
		}
		catch (const std::out_of_range &)
		{ }
	}
}

void
X3DTransformNodeTool::set_active ()
{
	const auto & selection = getBrowser () -> getSelection ();

	// Activate transformation group.

	for (const auto & tool : getBrowser () -> getTransformTools ())
		tool -> grouped () = isActive ();

	if (isActive ())
	{
		// Begin undo.

		for (const auto & node : selection -> getSelectGeometry () ? selection -> getGeometries () : selection -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> beginUndo ();
		}
	}
	else
	{
		// End undo.

		std::string description;

      switch (getActiveTool ())
		{
			case ToolType::DEFAULT:
				description = basic::sprintf (_ ("Edit %s"), getTypeName () .c_str ());
				break;
			case ToolType::TRANSLATE:
				description = basic::sprintf (_ ("Edit Field %s »translation«"), getTypeName () .c_str ());
				break;
			case ToolType::ROTATE:
				description = basic::sprintf (_ ("Edit Field %s »rotation«"), getTypeName () .c_str ());
				break;
			case ToolType::SCALE:
			default:
				description = basic::sprintf (_ ("Edit Field %s »scale«"), getTypeName () .c_str ());
				break;
		}

		const auto undoStep = std::make_shared <UndoStep> (description);

		for (const auto & node : selection -> getSelectGeometry () ? selection -> getGeometries () : selection -> getNodes ())
		{
			const X3DPtr <X3DNodeTool> tool (node);

			if (tool)
				tool -> endUndo (undoStep);
		}

		if (not undoStep -> isEmpty ())
		{
			// Prototype support
		
			X3DEditor::requestUpdateInstances (SFNode (getNode <X3DTransformNode> ()), undoStep);

			// Send undo step.

			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
		}
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

		if (not grouping ())
			return;

		if (not grouped ())
			return;

		const auto differenceMatrix = inverse (groupMatrix * modelMatrix) * getMatrix () * modelMatrix;

		for (const auto & tool : getBrowser () -> getTransformTools ())
		{
			try
			{
				if (tool == this)
					continue;

				if (not tool -> grouping ())
					continue;

				if (not tool -> grouped ())
					continue;

				tool -> addAbsoluteMatrix (differenceMatrix, tool -> getKeepCenter ());
			}
			catch (const X3DError &)
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

		const auto bbox = getNode <X3DTransformNode> () -> getSubBBox ();

		getToolNode () -> setField <SFMatrix4f> ("cameraSpaceMatrix", renderObject -> getCameraSpaceMatrix () .get (), true);
		getToolNode () -> setField <SFMatrix4f> ("modelViewMatrix",   renderObject -> getModelViewMatrix   () .get (), true);
		getToolNode () -> setField <SFVec3f>    ("bboxSize",          bbox .size (),   true);
		getToolNode () -> setField <SFVec3f>    ("bboxCenter",        bbox .center (), true);

		getBrowser () -> getRouter () -> processEvents ();
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
	if (getExecutionContext () -> isType ({ X3DConstants::X3DPrototypeInstance }))
	{
		X3DGroupingNodeTool::traverse (type, renderObject);
	}
	else
	{
		getNode <X3DTransformNode> () -> traverse (type, renderObject);
	
		// Remember matrices
	
		if (type == TraverseType::CAMERA)
		{
			modelMatrix = renderObject -> getModelViewMatrix () .get ();
		}
	
		// Tool
	
		renderObject -> getModelViewMatrix () .push ();
		renderObject -> getModelViewMatrix () .mult_left (getMatrix ());
	
		switch (type)
		{
			case TraverseType::DISPLAY:
			{
				// Last chance to process events
				reshape (renderObject);
				break;
			}
			default:
				break;
		}

		X3DToolObject::traverse (type, renderObject);
	
		renderObject -> getModelViewMatrix () .pop ();
	}
}

void
X3DTransformNodeTool::dispose ()
{
	__LOG__ << std::endl;

	getBrowser () -> removeTransformTool (this);

	X3DTransformMatrix3DObjectTool::dispose ();
	X3DGroupingNodeTool::dispose ();
}

X3DTransformNodeTool::~X3DTransformNodeTool ()
{ }

} // X3D
} // titania
