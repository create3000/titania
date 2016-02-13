/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "LOD.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Tools/Navigation/LODTool.h"

namespace titania {
namespace X3D {

static constexpr size_t  FRAMES         = 180; // Number of frames after wich a level change takes in affect.
static constexpr double  FRAME_RATE_MIN = 20;  // Lowest level of detail.
static constexpr double  FRAME_RATE_MAX = 55;  // Highest level of detail.

const ComponentType LOD::component      = ComponentType::NAVIGATION;
const std::string   LOD::typeName       = "LOD";
const std::string   LOD::containerField = "children";

LOD::Fields::Fields () :
	forceTransitions (new SFBool ()),
	          center (new SFVec3f ()),
	           range (new MFFloat ()),
	   level_changed (new SFInt32 (-1))
{ }

LOD::LOD (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	 X3DGroupingNode (),
	          fields (),
	       frameRate (60),
        changeLevel (true),
	keepCurrentLevel (false),
	       childNode ()
{
	addType (X3DConstants::LOD);

	addField (inputOutput,    "metadata",         metadata ());
	addField (initializeOnly, "forceTransitions", forceTransitions ());
	addField (initializeOnly, "center",           center ());
	addField (initializeOnly, "range",            range ());
	addField (outputOnly,     "level_changed",    level_changed ());
	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (inputOnly,      "addChildren",      addChildren ());
	addField (inputOnly,      "removeChildren",   removeChildren ());
	addField (inputOutput,    "children",         children ());

	addField (VRML_V2_0, "level", "children");

	X3DBaseNode::addChildren (childNode);

	center () .setUnit (UnitCategory::LENGTH);
	range ()  .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
LOD::create (X3DExecutionContext* const executionContext) const
{
	return new LOD (executionContext);
}

Box3d
LOD::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		const auto boundedObject = x3d_cast <X3DBoundedObject*> (childNode);

		if (boundedObject)
			return boundedObject -> getBBox ();
		
		return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
LOD::setKeepCurrentLevel (const bool value)
{
	keepCurrentLevel = value;

	getBrowser () -> addEvent ();
}

size_t
LOD::getLevel (const TraverseType type) const
{
	if (range () .empty ())
	{
		frameRate = ((FRAMES - 1) * frameRate + getBrowser () -> getCurrentFrameRate ()) / FRAMES;

		const auto size = children () .size ();

		if (size < 2)
			return 0;

		if (size == 2)
			return frameRate > FRAME_RATE_MAX;

		const auto   n        = size - 1;
		const double fraction = std::max ((frameRate - FRAME_RATE_MIN) / (FRAME_RATE_MAX - FRAME_RATE_MIN), 0.0);

		return std::min <size_t> (std::ceil (fraction * (n - 1)), n);
	}

	const double distance = getDistance (type);
	const auto   iter     = std::upper_bound (range () .cbegin (), range () .cend (), distance);

	return iter - range () .cbegin ();
}

double
LOD::getDistance (const TraverseType type) const
{
	Matrix4d modelViewMatrix = getModelViewMatrix (type);

	modelViewMatrix .translate (center () .getValue ());

	return math::abs (modelViewMatrix .origin ());
}

void
LOD::set_cameraObjects ()
{
	setCameraObject (childNode and childNode -> isCameraObject ());
}

void
LOD::traverse (const TraverseType type)
{
	int32_t level = 0;

	if (keepCurrentLevel)
		level = level_changed ();
	
	else
	{
		level = getLevel (type);

		if (forceTransitions ())
		{
			if (level > level_changed ())
				level = level_changed () + 1;
			
			else if (level < level_changed ())
				level = level_changed () - 1;
		}
	}

	if (type == TraverseType::DISPLAY)
	{
		if (level not_eq level_changed ())
		{
			level_changed () = level;

			set_child (level);
		}
		else if (keepCurrentLevel)
			set_child (level);
	}

	try
	{
		childNode -> traverse (type);
	}
	catch (const X3DError &)
	{ }
}

void
LOD::set_child (const int32_t level)
{
	if (level >= 0 and level < (int32_t) children () .size ())
		childNode .set (x3d_cast <X3DChildNode*> (children () [level]));
	
	else
		childNode .set (nullptr);
			
	set_cameraObjects ();
}

void
LOD::addTool ()
{
	X3DGroupingNode::addTool (new LODTool (this));
}

} // X3D
} // titania
