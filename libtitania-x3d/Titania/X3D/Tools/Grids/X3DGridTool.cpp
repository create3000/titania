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

#include "X3DGridTool.h"

#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/X3DTransformNode.h"

namespace titania {
namespace X3D {

X3DGridTool::Fields::Fields () :
	   translation (new SFVec3f ()),
	      rotation (new SFRotation ()),
	         scale (new SFVec3f (1, 1, 1)),
	         color (new SFColorRGBA (0.5, 0.5, 0.5, 0.2)),
	     lineColor (new SFColorRGBA (1, 0.7, 0.7, 0.2)),
	majorLineColor (new SFColorRGBA (1, 0.7, 0.7, 0.4)),
	  snapToCenter (new SFBool (true)),
	  snapDistance (new SFFloat (0.2))
{ }

X3DGridTool::X3DGridTool () :
	X3DActiveLayerTool (),
	            fields (),
	         selection (),
	          children ()
{
	addType (X3DConstants::X3DGridTool);

	addChildren (selection, children);
}

void
X3DGridTool::initialize ()
{
	X3DActiveLayerTool::initialize ();

	set_selection (getExecutionContext () -> getBrowser () -> getSelection ());
}

void
X3DGridTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)

{
	X3DActiveLayerTool::setExecutionContext (executionContext);

	set_selection (getBrowser () -> getSelection ());
}

void
X3DGridTool::realize ()
{
	X3DActiveLayerTool::realize ();

	try
	{
		auto & set_translation = getToolNode () -> getField <SFVec3f> ("set_translation");
		translation ()  .addInterest (set_translation);
		set_translation .addInterest (translation ());
		set_translation .addEvent (translation ());

		auto & set_rotation = getToolNode () -> getField <SFRotation> ("set_rotation");
		rotation ()  .addInterest (set_rotation);
		set_rotation .addInterest (rotation ());		
		set_rotation .addEvent (rotation ());

		auto & set_scale = getToolNode () -> getField <SFVec3f> ("set_scale");
		scale ()  .addInterest (set_scale);
		set_scale .addInterest (scale ());
		set_scale .addEvent (scale ());

		color ()          .addInterest (this, &X3DGridTool::set_color);
		lineColor ()      .addInterest (this, &X3DGridTool::set_lineColor);
		majorLineColor () .addInterest (this, &X3DGridTool::set_majorLineColor);

		set_color ();
		set_lineColor ();
		set_majorLineColor ();
	}
	catch (const X3DError & error)
	{ }
}

void
X3DGridTool::set_color ()
{
	try
	{
		auto & set_color        = getToolNode () -> getField <SFColor> ("set_color");
		auto & set_transparency = getToolNode () -> getField <SFFloat> ("set_transparency");

		set_color        = Color3f (color () .getRed (), color () .getGreen (), color () .getBlue ());
		set_transparency = 1 - color () .getAlpha ();
	}
	catch (const X3DError & error)
	{ }

}

void
X3DGridTool::set_lineColor ()
{
	try
	{
		auto & set_color        = getToolNode () -> getField <SFColor> ("set_lineColor");
		auto & set_transparency = getToolNode () -> getField <SFFloat> ("set_lineTransparency");

		set_color        = Color3f (lineColor () .getRed (), lineColor () .getGreen (), lineColor () .getBlue ());
		set_transparency = 1 - lineColor () .getAlpha ();
	}
	catch (const X3DError & error)
	{ }

}

void
X3DGridTool::set_majorLineColor ()
{
	try
	{
		auto & set_color        = getToolNode () -> getField <SFColor> ("set_majorLineColor");
		auto & set_transparency = getToolNode () -> getField <SFFloat> ("set_majorLineTransparency");

		set_color        = Color3f (majorLineColor () .getRed (), majorLineColor () .getGreen (), majorLineColor () .getBlue ());
		set_transparency = 1 - majorLineColor () .getAlpha ();
	}
	catch (const X3DError & error)
	{ }

}

// Snaping

void
X3DGridTool::set_selection (const SelectionPtr & value)
{
	if (selection)
		selection -> getChildren () .removeInterest (this, &X3DGridTool::set_children);

	selection = value;
	selection -> getChildren () .addInterest (this, &X3DGridTool::set_children);

	set_children (selection -> getChildren ());
}

void
X3DGridTool::set_children (const MFNode & value)
{
	for (const auto & node : children)
	{
		const X3DPtr <X3DTransformNode> transform (node);

		if (transform)
		{
			transform -> translation () .removeInterest (this, &X3DGridTool::set_translation);
		}
	}

	children = value;

	for (const auto & node : children)
	{
		const X3DPtr <X3DTransformNode> transform (node);

		if (transform)
		{
			transform -> translation () .addInterest (this, &X3DGridTool::set_translation, transform);
		}
	}
}

void
X3DGridTool::set_translation (const X3DPtr <X3DTransformNode> & leader)
{
	if (getBrowser () -> getSelection () -> activeTool_changed () not_eq Selection::MOVE_TOOL)
		return;

	try
	{
		// Get absolute position.

		const auto absoluteMatrix = leader -> getCurrentMatrix () * leader -> getTransformationMatrix ();

		Vector3d position;

		if (snapToCenter ())
			position = Vector3d (leader -> center () .getValue ()) * absoluteMatrix;
		else
		{
			const auto bbox = Box3d (leader -> X3DGroupingNode::getBBox ()) * absoluteMatrix;
			position = bbox .center ();
		}

		// Calculate snap position and apply absolute relative translation.
		
		Matrix4d grid;
		grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());

		Matrix4d snap;
		snap .set (getSnapPosition (position * ~grid) * grid - position);

		const Matrix4d matrix        = Matrix4d (leader -> getMatrix ()) * leader -> getTransformationMatrix ();
		const Matrix4d currentMatrix = absoluteMatrix * snap * ~leader -> getTransformationMatrix ();
		leader -> setMatrix (currentMatrix);

		leader -> translation () .removeInterest (this, &X3DGridTool::set_translation);
		leader -> translation () .addInterest (this, &X3DGridTool::connectTranslation, leader);

		// Apply translation to translation group.

		const Matrix4d differenceMatrix = ~matrix * (absoluteMatrix * snap);

		for (const auto & node : children)
		{
			if (node == leader)
				continue;

			try
			{
				const X3DPtr <X3DTransformNode> transform (node);

				if (transform)
				{
					const Matrix4d absoluteMatrix = transform -> getCurrentMatrix () * transform -> getTransformationMatrix ();

					transform -> setMatrix (absoluteMatrix * differenceMatrix * ~transform -> getTransformationMatrix ());
		
					transform -> translation () .removeInterest (this, &X3DGridTool::set_translation);
					transform -> translation () .addInterest (this, &X3DGridTool::connectTranslation, transform);
				}
			}
			catch (const std::domain_error &)
			{ }
		}
	}
	catch (const std::exception &)
	{
		// Catch NOT_SUPPORTED from getTransformationMatrix,
		// Catch std::domain_error from matrix inverse.
	}
}

void
X3DGridTool::connectTranslation (const X3DPtr <X3DTransformNode> & transform)
{
	transform -> translation () .removeInterest (this, &X3DGridTool::connectTranslation);
	transform -> translation () .addInterest (this, &X3DGridTool::set_translation, transform);
}

X3DGridTool::~X3DGridTool ()
{ }

} // X3D
} // titania
