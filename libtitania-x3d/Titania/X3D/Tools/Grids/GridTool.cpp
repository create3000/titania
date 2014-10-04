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

#include "GridTool.h"

#include "../../Bits/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../../Components/Grouping/Transform.h"
#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

constexpr bool  snapToCenter = true;
constexpr float snapDistance = 0.2;

const ComponentType GridTool::component      = ComponentType::TITANIA;
const std::string   GridTool::typeName       = "GridTool";
const std::string   GridTool::containerField = "grid";

GridTool::Fields::Fields () :
	      dimension (new MFInt32 ({ 10, 10, 10 })),
	 majorLineEvery (new MFInt32 ({ 5, 5, 5 })),
	majorLineOffset (new MFInt32 ({ 0, 0, 0 }))
{ }

GridTool::GridTool (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGridTool (),
	     fields (),
	  selection (),
	   children ()
{
	addType (X3DConstants::GridTool);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "translation",     translation ());
	addField (inputOutput, "rotation",        rotation ());
	addField (inputOutput, "scale",           scale ());
	addField (inputOutput, "dimension",       dimension ());
	addField (inputOutput, "majorLineEvery",  majorLineEvery ());
	addField (inputOutput, "majorLineOffset", majorLineOffset ());
	addField (inputOutput, "color",           color ());
	addField (inputOutput, "lineColor",       lineColor ());
	addField (inputOutput, "majorLineColor",  majorLineColor ());

	addChildren (selection, children);
}

X3DBaseNode*
GridTool::create (X3DExecutionContext* const executionContext) const
{
	return new GridTool (executionContext);
}

void
GridTool::initialize ()
{
	X3DGridTool::initialize ();

	requestAsyncLoad ({ get_tool ("GridTool.x3dv") .str () });

	set_selection (getExecutionContext () -> getBrowser () -> getSelection ());
}

void
GridTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)

{
	X3DGridTool::setExecutionContext (executionContext);

	set_selection (getBrowser () -> getSelection ());
}

void
GridTool::realize ()
{
	X3DGridTool::realize ();

	try
	{
		auto & set_dimension = getToolNode () -> getField <MFInt32> ("set_dimension");
		dimension ()  .addInterest (set_dimension);
		set_dimension .addInterest (dimension ());
		set_dimension .addEvent (dimension ());

		auto & set_majorLineEvery = getToolNode () -> getField <MFInt32> ("set_majorLineEvery");
		majorLineEvery ()  .addInterest (set_majorLineEvery);
		set_majorLineEvery .addInterest (majorLineEvery ());
		set_majorLineEvery .addEvent (majorLineEvery ());

		auto & set_majorLineOffset = getToolNode () -> getField <MFInt32> ("set_majorLineOffset");
		majorLineOffset ()  .addInterest (set_majorLineOffset);
		set_majorLineOffset .addInterest (majorLineOffset ());
		set_majorLineOffset .addEvent (majorLineOffset ());
	}
	catch (const X3DError & error)
	{ }
}

void
GridTool::set_selection (const SelectionPtr & value)
{
	if (selection)
		selection -> getChildren () .removeInterest (this, &GridTool::set_children);

	selection = value;
	selection -> getChildren () .addInterest (this, &GridTool::set_children);

	set_children (selection -> getChildren ());
}

void
GridTool::set_children (const MFNode & value)
{
	for (const auto & node : children)
	{
		const X3DPtr <X3DTransformNode> transform (node);

		if (transform)
		{
			transform -> translation () .removeInterest (this, &GridTool::set_translation);
		}
	}

	children = value;

	for (const auto & node : children)
	{
		const X3DPtr <X3DTransformNode> transform (node);

		if (transform)
		{
			transform -> translation () .addInterest (this, &GridTool::set_translation, transform);
		}
	}
}

void
GridTool::set_translation (const X3DPtr <X3DTransformNode> & transform)
{
	if (getBrowser () -> getSelection () -> activeTool_changed () not_eq Selection::MOVE_TOOL)
		return;

	for (const auto & node : children)
	{
		const auto transform = dynamic_cast <X3DTransformNode*> (node .getValue ());

		if (transform)
		{
			transform -> translation () .removeInterest (this, &GridTool::set_translation);
			transform -> translation () .addInterest (this, &GridTool::connectTranslation, transform);
		}
	}

	try
	{
		// Get absolute position.

		const auto matrix = transform -> getCurrentMatrix () * transform -> getTransformationMatrix ();

		Vector3d position;

		if (snapToCenter)
			position = Vector3d (transform -> center () .getValue ()) * matrix;
		else
		{
			const auto bbox = Box3d (transform -> X3DGroupingNode::getBBox ()) * matrix;
			position = bbox .center ();
		}

		// Calculate snap position and apply relative translation.

		Matrix4d snap;
		snap .set (getTranslation (position) - position);

		transform -> setMatrix (matrix * snap * ~transform -> getTransformationMatrix ());
	}
	catch (const std::exception &)
	{
		// Catch NOT_SUPPORTED from getTransformationMatrix,
		// Catch std::domain_error from matrix inverse.
	}
}

Vector3d
GridTool::getTranslation (const Vector3d & position)
{
	auto translation = position * ~Rotation4d (rotation () .getValue ());

	if (scale () .getX ())
	{
		const auto x = getTranslation (0, translation);

		if (std::abs (x - translation .x ()) < std::abs (scale () .getX () * snapDistance))
			translation .x (x);
	}

	if (scale () .getY ())
	{
		const auto y = getTranslation (1, translation);

		if (std::abs (y - translation .y ()) < std::abs (scale () .getY () * snapDistance))
			translation .y (y);
	}

	if (scale () .getZ ())
	{
		const auto z = getTranslation (2, translation);

		if (std::abs (z - translation .z ()) < std::abs (scale () .getZ () * snapDistance))
			translation .z (z);
	}

	return translation * Rotation4d (rotation () .getValue ());
}

double
GridTool::getTranslation (const size_t axis, const Vector3d & position)
{
	const auto o  = dimension () .get1Value (axis) % 2 * 0.5; // Add a half scale if dimension is odd.
	const auto p  = std::round (position [axis] / scale () .get1Value (axis)) * scale () .get1Value (axis);
	const auto p1 = p - o * scale () .get1Value (axis) + translation () .get1Value (axis);
	const auto p2 = p + o * scale () .get1Value (axis) + translation () .get1Value (axis);

	return std::abs (p1 - position [axis]) < std::abs (p2 - position [axis]) ? p1 : p2;
}

void
GridTool::connectTranslation (const X3DPtr <X3DTransformNode> & transform)
{
	transform -> translation () .removeInterest (this, &GridTool::connectTranslation);
	transform -> translation () .addInterest (this, &GridTool::set_translation, transform);
}

} // X3D
} // titania
