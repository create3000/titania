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
	      dimension (new MFInt32 ({ 10, 10, 10 })),
	 majorLineEvery (new MFInt32 ({ 5, 5, 5 })),
	majorLineOffset (new MFInt32 ({ 0, 0, 0 })),
	          color (new SFColorRGBA (0.5, 0.5, 0.5, 0.2)),
	      lineColor (new SFColorRGBA (1, 0.7, 0.7, 0.2)),
	 majorLineColor (new SFColorRGBA (1, 0.7, 0.7, 0.4)),
	   snapToCenter (new SFBool (true)),
	   snapDistance (new SFFloat (0.25))
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
			transform -> scale ()       .removeInterest (this, &X3DGridTool::set_scale);
		}
	}

	children = value;

	for (const auto & node : children)
	{
		const X3DPtr <X3DTransformNode> transform (node);

		if (transform)
		{
			transform -> translation () .addInterest (this, &X3DGridTool::set_translation, transform);
			transform -> scale ()       .addInterest (this, &X3DGridTool::set_scale,       transform);
		}
	}
}

void
X3DGridTool::set_translation (const X3DPtr <X3DTransformNode> & master)
{
	try
	{
		if (master -> getActiveTool () not_eq Selection::MOVE_TOOL)
			return;
	
		// The position is transformed to an absolute position and then transformed into the coordinate systwm of the grid
		// for easier snap position calculation.
	
		// Get absolute position.
	
		const auto absoluteMatrix = master -> getCurrentMatrix () * master -> getTransformationMatrix ();
	
		Vector3d position;
	
		if (snapToCenter () and not master -> getKeepCenter ())
			position = Vector3d (master -> center () .getValue ()) * absoluteMatrix;
		else
		{
			// Snap to bbox center.
			const auto bbox = Box3d (master -> X3DGroupingNode::getBBox ()) * absoluteMatrix;
			position = bbox .center ();
		}
	
		// Calculate snap position and apply absolute relative translation.
	
		Matrix4d grid;
		grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());
	
		Matrix4d snap;
		snap .set (getSnapPosition (position * ~grid) * grid - position);
	
		const Matrix4d matrix        = Matrix4d (master -> getMatrix ()) * master -> getTransformationMatrix ();
		const Matrix4d currentMatrix = absoluteMatrix * snap * ~master -> getTransformationMatrix ();
	
		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);
	
		master -> translation () .removeInterest (this, &X3DGridTool::set_translation);
		master -> translation () .addInterest (this, &X3DGridTool::connectTranslation, master);
	
		// Apply translation to translation group.
	
		const Matrix4d differenceMatrix = ~matrix * (absoluteMatrix * snap);
	
		for (const auto & node : children)
		{
			if (node == master)
				continue;
	
			try
			{
				const X3DPtr <X3DTransformNode> transform (node);
	
				if (transform)
				{
					transform -> addAbsoluteMatrix (differenceMatrix, transform -> getKeepCenter ());
	
					if (transform -> translation () .isTainted ())
					{
						transform -> translation () .removeInterest (this, &X3DGridTool::set_translation);
						transform -> translation () .addInterest (this, &X3DGridTool::connectTranslation, transform);
					}
	
					if (transform -> scale () .isTainted ())
					{
						transform -> scale () .removeInterest (this, &X3DGridTool::set_scale);
						transform -> scale () .addInterest (this, &X3DGridTool::connectScale, transform);
					}
				}
			}
			catch (const std::exception &)
			{ }
		}
	}
	catch (const X3DError &)
	{ }
}

void
X3DGridTool::set_scale (const X3DPtr <X3DTransformNode> & master)
{
	try
	{
		// All points are first transformed to grid space, then a snap position is calculated, and then transformed back to absolute space.
	
		const int32_t tool = master -> getActiveTool () - Selection::SCALE_TOOL;
	
		if (tool < 0)
			return;
	
		const Matrix4d currentMatrix = tool < 6 ? getScaleMatrix (master, tool) : getUniformScaleMatrix (master, tool - 6);
		const Matrix4d matrix        = Matrix4d (master -> getMatrix ()) * master -> getTransformationMatrix ();
	
		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);
	
		master -> scale () .removeInterest (this, &X3DGridTool::set_scale);
		master -> scale () .addInterest (this, &X3DGridTool::connectScale, master);
	
		// Apply translation to translation group.
	
		const Matrix4d differenceMatrix = ~matrix * currentMatrix* master -> getTransformationMatrix ();
	
		for (const auto & node : children)
		{
			if (node == master)
				continue;
	
			try
			{
				const X3DPtr <X3DTransformNode> transform (node);
	
				if (transform)
				{
					transform -> addAbsoluteMatrix (differenceMatrix, transform -> getKeepCenter ());
	
					if (transform -> translation () .isTainted ())
					{
						transform -> translation () .removeInterest (this, &X3DGridTool::set_translation);
						transform -> translation () .addInterest (this, &X3DGridTool::connectTranslation, transform);
					}
	
					if (transform -> scale () .isTainted ())
					{
						transform -> scale () .removeInterest (this, &X3DGridTool::set_scale);
						transform -> scale () .addInterest (this, &X3DGridTool::connectScale, transform);
					}
				}
			}
			catch (const std::exception &)
			{ }
		}
	}
	catch (const X3DError &)
	{ }
}

Matrix4d
X3DGridTool::getScaleMatrix (const X3DPtr <X3DTransformNode> & master, const size_t tool)
{
	// All points are first transformed to grid space, then a snap position is calculated, and then transformed back to absolute space.

	constexpr double infinity = std::numeric_limits <double>::infinity ();
	constexpr double eps      = 1e-6;

	// Get absolute position.

	const auto currentMatrix  = master -> getCurrentMatrix ();
	const auto absoluteMatrix = currentMatrix * master -> getTransformationMatrix ();
	const auto geometry       = Box3d (master -> X3DGroupingNode::getBBox ());  // BBox of the geometry.
	const auto shape          = Box3d (geometry .size (), geometry .center ()); // AABB BBox
	const auto bbox           = shape * absoluteMatrix;                         // Absolute OBB of AABB
	const auto position       = bbox .center ();                                // Absolute position

	// Calculate snap scale for one axis. The ratio is calculated in transforms sub space.

	Matrix4d grid;
	grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());

	const size_t axis         = tool % 3;
	const double sgn          = tool < 3 ? 1 : -1;
	const auto   direction    = bbox .axes () [axis] * sgn;
	const auto   point        = direction + position;
	const auto   snapPosition = getSnapPosition (point * ~grid, normalize ((~grid) .mult_dir_matrix (direction))) * grid;
	auto         after        = (snapPosition * ~absoluteMatrix - shape .center ()) [axis];
	auto         before       = shape .axes () [axis] [axis] * sgn;

	if (getBrowser () -> hasControlKey ()) // Scale from corner.
	{
		after  += before;
		before *= 2;
	}

	const auto delta = after - before;
	auto       ratio = after / before;

	//	__LOG__ << std::endl;
	//	__LOG__ << tool << std::endl;
	//	__LOG__ << before << std::endl;
	//	__LOG__ << after << std::endl;
	//	__LOG__ << delta << std::endl;
	//	__LOG__ << ratio << std::endl;

	// We must procced with the original current matrix and a snap scale of [1 1 1], for correct grouped event handling.

	if (std::abs (delta) < eps or std::abs (ratio) < 1e-3 or std::isnan (ratio) or std::abs (ratio) == infinity)
		return currentMatrix;

	Vector3d scale (1, 1, 1);
	scale [axis] = ratio;

	Matrix4d snap;
	snap .scale (scale);

	snap *= getOffset (shape, snap, shape .axes () [axis] * sgn);

	return snap * currentMatrix;
}

Matrix4d
X3DGridTool::getUniformScaleMatrix (const X3DPtr <X3DTransformNode> & master, const size_t tool)
{
	// All points are first transformed to grid space, then a snap position is calculated, and then transformed back to absolute space.

	constexpr double infinity = std::numeric_limits <double>::infinity ();

	// Get absolute position.

	const auto currentMatrix  = master -> getCurrentMatrix ();
	const auto absoluteMatrix = currentMatrix * master -> getTransformationMatrix ();
	const auto geometry       = Box3d (master -> X3DGroupingNode::getBBox ());  // BBox of the geometry.
	const auto shape          = Box3d (geometry .size (), geometry .center ()); // AABB BBox
	const auto bbox           = shape * absoluteMatrix;                         // Absolute OBB of AABB
	const auto position       = bbox .center ();                                // Absolute position

	// Calculate snap scale and apply absolute relative translation.

	Matrix4d grid;
	grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());

	const auto points = bbox .points ();
	double     min    = infinity;

	if (getBrowser () -> hasControlKey ())
	{
		// Uniform scale from corner.

		const auto point  = points [tool];
		auto before = point - position;
		auto after  = getSnapPosition (point * ~grid, normalize ((~grid) .mult_dir_matrix (before))) * grid - position;
		
		after  += before;
		before *= 2;
	
		const auto delta  = after - before;
		const auto ratio  = after / before;

		for (size_t i = 0; i < 3; ++ i)
		{
			const auto r = std::abs (ratio [i] - 1);

			if (delta [i] and r < std::abs (min - 1))
				min = ratio [i];
		}		
	}
	else
	{
		for (const auto & point : points)
		{
			const auto before = point - position;
			const auto after  = getSnapPosition (point * ~grid, normalize ((~grid) .mult_dir_matrix (before))) * grid - position;
			const auto delta  = after - before;
			const auto ratio  = after / before;

			for (size_t i = 0; i < 3; ++ i)
			{
				const auto r = std::abs (ratio [i] - 1);

				if (delta [i] and r < std::abs (min - 1))
					min = ratio [i];
			}
		}
	}

	// We must procced with the original current matrix and a snap scale of [1 1 1], for correct grouped event handling.

	if (min == 0 or min == infinity)
		return currentMatrix;

	Matrix4d snap;
	snap .scale (Vector3d (min, min, min));

	snap *= getOffset (bbox, snap, points [tool] - bbox .center ());

	return absoluteMatrix * snap * ~master -> getTransformationMatrix ();
}

Matrix4d
X3DGridTool::getOffset (const Box3d & bbox, const Matrix4d scaledMatrix, const Vector3d & offset) const
{
	// To keep the bbox center at its point we must compute a translation offset.

	Vector3d distanceFromCenter = bbox .center ();

	if (getBrowser () -> hasControlKey ()) // Scale from corner.
		distanceFromCenter -= offset;

	Matrix4d translation;
	translation .set (distanceFromCenter - scaledMatrix .mult_dir_matrix (distanceFromCenter));

	return translation;
}

void
X3DGridTool::connectTranslation (const X3DPtr <X3DTransformNode> & transform)
{
	transform -> translation () .removeInterest (this, &X3DGridTool::connectTranslation);
	transform -> translation () .addInterest (this, &X3DGridTool::set_translation, transform);
}

void
X3DGridTool::connectScale (const X3DPtr <X3DTransformNode> & transform)
{
	transform -> scale () .removeInterest (this, &X3DGridTool::connectScale);
	transform -> scale () .addInterest (this, &X3DGridTool::set_scale, transform);
}

X3DGridTool::~X3DGridTool ()
{ }

} // X3D
} // titania
