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

#include "X3DIndexedFaceSetTool.h"

#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Rendering/CoordinateTool.h"

namespace titania {
namespace X3D {

const int32_t X3DIndexedFaceSetTool::ToolNumber::NONE   = 0;
const int32_t X3DIndexedFaceSetTool::ToolNumber::SELECT = 1;
const int32_t X3DIndexedFaceSetTool::ToolNumber::CUT    = 2;
const int32_t X3DIndexedFaceSetTool::ToolNumber::SCULP  = 3;

X3DIndexedFaceSetTool::Fields::Fields () :
	    toolType (new SFString ("NONE")),
	    isActive (new SFBool ()),
	   touchTime (new SFTime ()),
	undo_changed (new UndoStepContainerPtr ())
{ }

X3DIndexedFaceSetTool::X3DIndexedFaceSetTool () :
	                  IndexedFaceSet (getExecutionContext ()),
	     X3DComposedGeometryNodeTool (),
	                          fields ()
{
	addType (X3DConstants::X3DIndexedFaceSetTool);
}

void
X3DIndexedFaceSetTool::initialize ()
{
	X3DComposedGeometryNodeTool::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (&X3DIndexedFaceSetTool::set_loadState, this);
	toolType () .addInterest (&X3DIndexedFaceSetTool::set_toolType, this);

	set_toolType ();
}

void
X3DIndexedFaceSetTool::set_loadState ()
{
	try
	{
		const auto & inlineNode  = getCoordinateTool () -> getInlineNode ();
		const auto   touchSensor = inlineNode -> getExportedNode <TouchSensor> ("TouchSensor");

		touchSensor -> isActive ()  .addInterest (isActive ());
		touchSensor -> touchTime () .addInterest (touchTime ());

		set_toolType ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetTool::set_toolType ()
{
	try
	{
		if (toolType () == "NONE")
		{
			const auto & inlineNode = getCoordinateTool () -> getInlineNode ();
			const auto   toolSwich  = inlineNode -> getExportedNode <Switch> ("ToolSwitch");

			toolSwich -> whichChoice () = 0;
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetTool::rewriteArray (const std::map <int32_t, int32_t> & map, std::vector <int32_t> & array) const
{
	for (auto & value : array)
	{
		try
		{
			value = map .at (value);
		}
		catch (const std::out_of_range &)
		{ }
	}
}

void
X3DIndexedFaceSetTool::undoSetColorIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (colorIndex ()), colorIndex ());
}

void
X3DIndexedFaceSetTool::redoSetColorIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (colorIndex ()), colorIndex ());
}

void
X3DIndexedFaceSetTool::undoSetTexCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (texCoordIndex ()), texCoordIndex ());
}

void
X3DIndexedFaceSetTool::redoSetTexCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (texCoordIndex ()), texCoordIndex ());
}

void
X3DIndexedFaceSetTool::undoSetNormalIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (normalIndex ()), normalIndex ());
}

void
X3DIndexedFaceSetTool::redoSetNormalIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (normalIndex ()), normalIndex ());
}

void
X3DIndexedFaceSetTool::undoSetCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (coordIndex ()), coordIndex ());
}

void
X3DIndexedFaceSetTool::redoSetCoordIndex (const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (SFNode (getNode <IndexedFaceSet> ()));
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (coordIndex ()), coordIndex ());
}

void
X3DIndexedFaceSetTool::dispose ()
{
	__LOG__ << std::endl;

	X3DComposedGeometryNodeTool::dispose ();
}

X3DIndexedFaceSetTool::~X3DIndexedFaceSetTool ()
{ }

} // X3D
} // titania
