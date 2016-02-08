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

#include "CoordinateTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType CoordinateTool::component      = ComponentType::TITANIA;
const std::string   CoordinateTool::typeName       = "CoordinateTool";
const std::string   CoordinateTool::containerField = "coordTool";

CoordinateTool::Fields::Fields () :
	         enabled (new SFBool (true)),
	 modelViewMatrix (new SFMatrix4f ()),
	           color (new SFColorRGBA (0.7, 0.85, 1, 1)),
	     vertexCount (new MFInt32 ()),
	           point (new MFVec3f ()),
	        geometry (new SFNode ()),
	hitPoint_changed (new SFVec3f ()),
	        isActive (new SFBool ()),
	        touchTime (new SFTime ())
{ }

CoordinateTool::CoordinateTool (X3DExecutionContext* const executionContext) :
	          X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DCoordinateNodeTool (),
	               fields (),
	                 show (false)
{
	//addType (X3DConstants::CoordinateTool);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "enabled",          enabled ());
	addField (inputOutput, "modelViewMatrix",  modelViewMatrix ());
	addField (inputOutput, "color",            color ());
	addField (inputOutput, "vertexCount",      vertexCount ());
	addField (inputOutput, "point",            point ());
	addField (inputOutput, "geometry",         geometry ());
	addField (inputOutput, "hitPoint_changed", hitPoint_changed ());
	addField (inputOutput, "isActive",         isActive ());
	addField (inputOutput, "touchTime",        touchTime ());
}

X3DBaseNode*
CoordinateTool::create (X3DExecutionContext* const executionContext) const
{
	return new CoordinateTool (executionContext);
}

void
CoordinateTool::initialize ()
{
	X3DCoordinateNodeTool::initialize ();

	enabled () .addInterest (getInlineNode () -> load ());

	getInlineNode () -> load () = enabled ();
	getInlineNode () -> url ()  = { get_tool ("CoordinateTool.x3dv") .str () };
}

void
CoordinateTool::realize ()
{
	X3DCoordinateNodeTool::realize ();

	try
	{
		getBrowser () -> prepareEvents () .addInterest (this, &CoordinateTool::prepareEvent);

		auto & set_modelViewMatrix = getInlineNode () -> getExportedNode ("TransformMatrix3D") -> getField <SFMatrix4f> ("matrix");
		modelViewMatrix () .addInterest (set_modelViewMatrix);
		set_modelViewMatrix = modelViewMatrix ();

		auto & set_vertexCount = getInlineNode () -> getExportedNode ("EdgesLineSet") -> getField <MFInt32> ("vertexCount");
		vertexCount () .addInterest (set_vertexCount);
		set_vertexCount = vertexCount ();

		auto & set_point = getInlineNode () -> getExportedNode ("EdgesCoord") -> getField <MFVec3f> ("set_point");
		point () .addInterest (set_point);
		set_point = point ();

		auto & set_geometry = getInlineNode () -> getExportedNode ("SelectionShape") -> getField <SFNode> ("geometry");
		geometry () .addInterest (set_geometry);
		set_geometry = geometry ();

		const auto touchSensor = getInlineNode () -> getExportedNode ("TouchSensor");

		touchSensor -> getField <SFVec3f> ("hitPoint_changed") .addInterest (hitPoint_changed ());
		touchSensor -> getField <SFBool>  ("isActive")         .addInterest (isActive ());
		touchSensor -> getField <SFTime>  ("touchTime")        .addInterest (touchTime ());

		color () .addInterest (this, &CoordinateTool::set_color);

		set_color ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
CoordinateTool::prepareEvent ()
{
	try
	{
		getInlineNode () -> getExportedNode ("Switch") -> setField <SFInt32> ("whichChoice", int32_t (show), true);
		show = false;
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
CoordinateTool::set_color ()
{
	try
	{
		const auto material = getInlineNode () -> getExportedNode ("Material");

		material -> setField <SFColor> ("emissiveColor", Color3f (color () .getRed (), color () .getGreen (), color () .getBlue ()));
		material -> setField <SFFloat> ("transparency", 1 - color () .getAlpha ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
CoordinateTool::draw (const ShapeContainer* const container)
{
	if (getCurrentLayer () != getActiveLayer ())
		return;

	show = true;

	const auto mvm = container -> getModelViewMatrix () * getCurrentViewpoint () -> getCameraSpaceMatrix ();

	if (mvm != modelViewMatrix ())
		modelViewMatrix () = mvm;
}

} // X3D
} // titania
