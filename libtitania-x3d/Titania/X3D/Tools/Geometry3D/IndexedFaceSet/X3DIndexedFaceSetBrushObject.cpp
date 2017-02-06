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

#include "X3DIndexedFaceSetBrushObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"

namespace titania {
namespace X3D {

X3DIndexedFaceSetBrushObject::Fields::Fields () :
	 brush (new SFNode ())
{ }

X3DIndexedFaceSetBrushObject::X3DIndexedFaceSetBrushObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetSelectionObject (),
	                          fields (),
	                      toolSwitch (),
	                     touchSensor (),
	                     brushSwitch (),
	                  brushTransform (),
	                       brushNode ()
{
	//addType (X3DConstants::X3DIndexedFaceSetBrushObject);

	addChildObjects (toolSwitch,
	                 touchSensor,
	                 brushSwitch,
	                 brushTransform,
	                 brushNode);
}

void
X3DIndexedFaceSetBrushObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (&X3DIndexedFaceSetBrushObject::set_loadState, this);

	toolType () .addInterest (&X3DIndexedFaceSetBrushObject::set_toolType, this);
	brush ()    .addInterest (&X3DIndexedFaceSetBrushObject::set_brush, this);

	set_loadState ();
	set_brush ();
}

void
X3DIndexedFaceSetBrushObject::set_loadState ()
{
	try
	{
		const auto & inlineNode = getCoordinateTool () -> getInlineNode ();

		toolSwitch     = inlineNode -> getExportedNode <Switch> ("ToolSwitch");
		touchSensor    = inlineNode -> getExportedNode <TouchSensor> ("BrushTouchSensor");
		brushSwitch    = inlineNode -> getExportedNode <Switch> ("BrushSwitch");
		brushTransform = inlineNode -> getExportedNode <Transform> ("Brush");

		touchSensor -> isOver ()           .addInterest (&X3DIndexedFaceSetBrushObject::set_touch_sensor_over, this);
		touchSensor -> hitPoint_changed () .addInterest (&X3DIndexedFaceSetBrushObject::set_touch_sensor_hitPoint, this);

		set_toolType ();
		set_brush_radius ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetBrushObject::set_toolType ()
{
	try
	{
		static const std::set <std::string> brushes = { "SCULP", "SCULP_SMOOTH", "SCULP_UNDO" };

		if (brushes .count (toolType ()))
			toolSwitch -> whichChoice () = getToolNumber ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetBrushObject::set_brush ()
{
	try
	{
		if (brushNode)
		{
			brushNode -> getField <SFDouble> ("radius") .removeInterest (&X3DIndexedFaceSetBrushObject::set_brush_radius, this);
			brushNode -> getField <SFDouble> ("scale")  .removeInterest (&X3DIndexedFaceSetBrushObject::set_brush_radius, this);
		}
	}
	catch (const X3DError & error)
	{ }

	brushNode = brush ();

	try
	{
		if (brushNode)
		{
			brushNode -> getField <SFDouble> ("radius") .addInterest (&X3DIndexedFaceSetBrushObject::set_brush_radius, this);
			brushNode -> getField <SFDouble> ("scale")  .addInterest (&X3DIndexedFaceSetBrushObject::set_brush_radius, this);
		}

		set_brush_radius ();
	}
	catch (const X3DError & error)
	{ }
}

void
X3DIndexedFaceSetBrushObject::set_brush_radius ()
{
	try
	{
		const double radius = brushNode -> getField <SFDouble> ("radius");
		const double scale  = brushNode -> getField <SFDouble> ("scale");

		brushTransform -> scale () = Vector3d (radius, radius, radius) * scale;
	}
	catch (const X3DError & error)
	{ }
}

void
X3DIndexedFaceSetBrushObject::set_touch_sensor_over ()
{
	brushSwitch -> whichChoice () = touchSensor -> isOver ();
}

void
X3DIndexedFaceSetBrushObject::set_touch_sensor_hitPoint ()
{
	brushTransform -> translation () = touchSensor -> hitPoint_changed ();
	brushTransform -> rotation ()    = Rotation4d (Vector3d (0, 0, 1), Vector3d (touchSensor -> hitNormal_changed () .getValue ()));
}

X3DIndexedFaceSetBrushObject::~X3DIndexedFaceSetBrushObject ()
{ }

} // X3D
} // titania
