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

#include "X3DGridTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

X3DGridTool::Fields::Fields () :
	        enabled (new SFBool (true)),
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
	   snapDistance (new SFDouble (0.25)),
	       isActive (new SFBool (true))
{ }

X3DGridTool::X3DGridTool () :
	X3DActiveLayerTool (),
	            fields (),
	    transformNodes ()
{
	addType (X3DConstants::X3DGridTool);

	addChildObjects (transformNodes);

	translation ()  .setUnit (UnitCategory::LENGTH);
	snapDistance () .setUnit (UnitCategory::LENGTH);
}

void
X3DGridTool::initialize ()
{
	X3DActiveLayerTool::initialize ();

	getBrowser () -> getTransformTools () .addInterest (&X3DGridTool::set_transform_tools, this);

	set_transform_tools (getBrowser () -> getTransformTools ());
}

void
X3DGridTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> getTransformTools () .removeInterest (&X3DGridTool::set_transform_tools, this);

	X3DActiveLayerTool::setExecutionContext (executionContext);

	getBrowser () -> getTransformTools () .addInterest (&X3DGridTool::set_transform_tools, this);

	set_transform_tools (getBrowser () -> getTransformTools ());
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

		getToolNode () -> getField <SFBool> ("isActive") .addInterest (isActive ());

		color ()          .addInterest (&X3DGridTool::set_color, this);
		lineColor ()      .addInterest (&X3DGridTool::set_lineColor, this);
		majorLineColor () .addInterest (&X3DGridTool::set_majorLineColor, this);

		set_color ();
		set_lineColor ();
		set_majorLineColor ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
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
X3DGridTool::set_transform_tools (const X3DWeakPtrArray <X3DTransformNodeTool> & value)
{
	for (const auto & transformNode : transformNodes)
	{
		try
		{
			transformNode -> translation () .removeInterest (&X3DGridTool::set_translation, this);
			transformNode -> rotation ()    .removeInterest (&X3DGridTool::set_rotation,    this);
			transformNode -> scale ()       .removeInterest (&X3DGridTool::set_scale,       this);
		}
		catch (const Error <DISPOSED> &)
		{ }
	}

	transformNodes = value;

	for (const auto & tool : value)
	{
		try
		{
			tool -> translation () .addInterest (&X3DGridTool::set_translation, this, tool);
			tool -> rotation ()    .addInterest (&X3DGridTool::set_rotation,    this, tool);
			tool -> scale ()       .addInterest (&X3DGridTool::set_scale,       this, tool);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DGridTool::set_translation (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		if (not enabled ())
			return;

		if (not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
			return;

		if (master -> getActiveTool () not_eq ToolType::MOVE)
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
		grid *= getTool () -> getTransformationMatrix ();

		Matrix4d snap;
		snap .set (getSnapPosition (position * inverse (grid)) * grid - position);
	
		const auto matrix        = master -> getMatrix () * master -> getTransformationMatrix ();
		const auto currentMatrix = absoluteMatrix * snap * inverse (master -> getTransformationMatrix ());
	
		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);

		//getBrowser () -> setDescription (master -> getDescription ());

		master -> translation () .removeInterest (&X3DGridTool::set_translation, this);
		master -> translation () .addInterest (&X3DGridTool::connectTranslation, this, master);
	
		// Apply translation to translation group.
	
		if (master -> grouping ())
		{
			const auto differenceMatrix = inverse (matrix) * (absoluteMatrix * snap);
		
			for (const auto & tool : getBrowser () -> getTransformTools ())
			{
				try
				{
					if (tool == master)
						continue;
	
					if (not tool -> grouping ())
						continue;
			
					tool -> addAbsoluteMatrix (differenceMatrix, tool -> getKeepCenter ());
	
					if (tool -> translation () .getTainted ())
					{
						tool -> translation () .removeInterest (&X3DGridTool::set_translation, this);
						tool -> translation () .addInterest (&X3DGridTool::connectTranslation, this, tool);
					}
				}
				catch (const std::exception &)
				{ }
			}
		}
	}
	catch (const std::exception &)
	{ }
}

void
X3DGridTool::set_rotation (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		if (not enabled ())
			return;

		if (not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
			return;

		if (master -> getActiveTool () not_eq ToolType::ROTATE)
			return;

//__LOG__ << std::endl;

		const auto matrixBefore = Matrix4d (master -> getMatrix ()) * master -> getTransformationMatrix (); // Matrix before transformation
		const auto matrixAfter  = master -> getCurrentMatrix ()     * master -> getTransformationMatrix (); // Matrix after transformation

		std::vector <double> distances = { dot (normalize (matrixAfter .x ()), normalize (matrixBefore .x ())),
		                                   dot (normalize (matrixAfter .y ()), normalize (matrixBefore .y ())),
		                                   dot (normalize (matrixAfter .z ()), normalize (matrixBefore .z ())) };

		const auto index0 = std::max_element (distances .begin (), distances .end ()) - distances .begin (); // Index of rotation axis

		const std::vector <Vector3d> y = { matrixAfter .x (), matrixAfter .y (), matrixAfter .z () }; // Rotation axis, equates to grid normal
		const std::vector <Vector3d> z = { matrixAfter .y (), matrixAfter .z (), matrixAfter .y () }; // Vector to snap, later transformed to grid space

		Matrix4d grid;
		grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());
		grid *= getTool () -> getTransformationMatrix ();

		const auto index1 = (index0 + 1) % y .size ();
		const auto index2 = (index0 + 2) % y .size ();

		Vector3d   Y             = normalize (cross (y [index1], y [index2]));
		Vector3d   X             = cross (grid .y (), Y); // Intersection between both planes
		Vector3d   Z             = cross (X, Y); // Front vector
		Matrix3d   rotationPlane = Matrix3d (X [0], X [1], X [2],   Y [0], Y [1], Y [2],   Z [0], Z [1], Z [2]);
		Rotation4d gridRotation  = Rotation4d (rotation () .getValue ());
		Matrix3d   gridPlane     = grid;

//__LOG__ << abs (X) << std::endl;
//__LOG__ << abs (Y) << std::endl;
//__LOG__ << abs (Z) << std::endl;
//__LOG__ << 1 - std::abs (dot (normalize (grid .y ()), Y)) << std::endl;

		// If X or Z are near 0 then Y is collinear to the y-axis.

		if (1 - std::abs (dot (normalize (grid .y ()), Y)) < 1e-6)
		{
			rotationPlane = Matrix3d ();
			gridRotation  = Rotation4d ();
		}

//__LOG__ << std::endl;
//__LOG__ << X << std::endl;
//__LOG__ << Y << std::endl;
//__LOG__ << Z << std::endl;

		Vector3d vectorToSnap  = z [index0];
		Vector3d vectorOnGrid  = normalize (vectorToSnap * inverse (rotationPlane) * gridRotation * inverse (gridPlane)); // Vector inside grid space.

		const auto snapVector   = getSnapPosition (vectorOnGrid, false) * gridPlane * inverse (gridRotation) * rotationPlane;
		const auto snapRotation = Rotation4d (inverse (master -> getTransformationMatrix ()) .mult_dir_matrix (vectorToSnap),
                                            inverse (master -> getTransformationMatrix ()) .mult_dir_matrix (snapVector));

		Matrix4d currentMatrix;
		currentMatrix .set (master -> translation ()      .getValue (),
		                    master -> rotation ()         .getValue () * snapRotation,
		                    master -> scale ()            .getValue (),
		                    master -> scaleOrientation () .getValue (),
		                    master -> center ()           .getValue ());

//__LOG__ << std::endl;
//__LOG__ << gridPlane << std::endl;
//__LOG__ << vectorToSnap << std::endl;
//__LOG__ << normalize (vectorToSnap * inverse (rotationPlane)) << std::endl;
//__LOG__ << vectorOnGrid << std::endl;
//__LOG__ << getSnapPosition (vectorOnGrid) << std::endl;
//__LOG__ << snapVector << std::endl;

		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);

		//getBrowser () -> setDescription (master -> getDescription ());
	
		master -> rotation () .removeInterest (&X3DGridTool::set_rotation, this);
		master -> rotation () .addInterest (&X3DGridTool::connectRotation, this, master);
	
		// Apply translation to translation group.
	
		if (master -> grouping ())
		{
			const auto differenceMatrix = inverse (matrixBefore) * currentMatrix * master -> getTransformationMatrix ();
		
			for (const auto & tool : getBrowser () -> getTransformTools ())
			{
				try
				{
					if (tool == master)
						continue;
	
					if (not tool -> grouping ())
						continue;
		
					tool -> addAbsoluteMatrix (differenceMatrix, tool -> getKeepCenter ());
	
					if (tool -> translation () .getTainted ())
					{
						tool -> translation () .removeInterest (&X3DGridTool::set_translation, this);
						tool -> translation () .addInterest (&X3DGridTool::connectTranslation, this, tool);
					}
				}
				catch (const std::exception &)
				{ }
			}
		}
	}
	catch (const std::exception &)
	{ }
}

void
X3DGridTool::set_scale (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		if (not enabled ())
			return;

		if ((not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()) or (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()))
			return;
	
		const auto tool = int32_t (master -> getActiveTool ()) - int32_t (ToolType::SCALE);

		if (tool < 0)
			return;

		// All points are first transformed to grid space, then a snap position is calculated, and then transformed back to absolute space.
	
		const auto currentMatrix = tool < 6 ? getScaleMatrix (master, tool) : getUniformScaleMatrix (master, tool - 6);
		const auto matrix        = Matrix4d (master -> getMatrix ()) * master -> getTransformationMatrix ();
	
		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);

		//getBrowser () -> setDescription (master -> getDescription ());
	
		master -> scale () .removeInterest (&X3DGridTool::set_scale, this);
		master -> scale () .addInterest (&X3DGridTool::connectScale, this, master);
	
		// Apply translation to translation group.

		if (master -> grouping ())
		{
			const auto differenceMatrix = inverse (matrix) * currentMatrix * master -> getTransformationMatrix ();
		
			for (const auto & tool : getBrowser () -> getTransformTools ())
			{
				try
				{
					if (tool == master)
						continue;
	
					if (not tool -> grouping ())
						continue;
	
					tool -> addAbsoluteMatrix (differenceMatrix, tool -> getKeepCenter ());
	
					if (tool -> scale () .getTainted ())
					{
						tool -> scale () .removeInterest (&X3DGridTool::set_scale, this);
						tool -> scale () .addInterest (&X3DGridTool::connectScale, this, tool);
					}
				}
				catch (const std::exception &)
				{ }
			}
		}
	}
	catch (const std::exception &)
	{ }
}

Matrix4d
X3DGridTool::getScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
{
	// All points are first transformed to grid space, then a snap position is calculated, and then transformed back to absolute space.

	constexpr double infinity = std::numeric_limits <double>::infinity ();
	constexpr double eps      = 1e-6;

	// Get absolute position.

	const auto currentMatrix  = master -> getCurrentMatrix ();
	const auto absoluteMatrix = currentMatrix * master -> getTransformationMatrix ();
	const auto geometry       = master -> X3DGroupingNode::getBBox ();          // BBox of the geometry.
	const auto shape          = Box3d (geometry .size (), geometry .center ()); // AABB BBox
	const auto bbox           = shape * absoluteMatrix;                         // Absolute OBB of AABB
	const auto position       = bbox .center ();                                // Absolute position

	// Calculate snap scale for one axis. The ratio is calculated in transforms sub space.

	Matrix4d grid;
	grid .set (translation () .getValue (), rotation () .getValue (), scale () .getValue ());
	grid *= getTool () -> getTransformationMatrix ();

	const size_t axis         = tool % 3;
	const double sgn          = tool < 3 ? 1 : -1;
	const auto   direction    = bbox .axes () [axis] * sgn;
	const auto   point        = direction + position;
	const auto   snapPosition = getSnapPosition (point * inverse (grid), normalize (inverse (grid) .mult_dir_matrix (direction))) * grid;
	auto         after        = (snapPosition * inverse (absoluteMatrix) - shape .center ()) [axis];
	auto         before       = shape .axes () [axis] [axis] * sgn;

	if (getScaleFromEdge (master))
	{
		// Scale from corner.
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

	for (const auto & connectedAxis : master -> connectedAxes ())
	{
		try
		{
			static const std::map <String::value_type, size_t> axes = {
				std::make_pair ('x', 0),
				std::make_pair ('y', 1),
				std::make_pair ('z', 2),
			};

			const auto lhs = axes .at (std::tolower (connectedAxis .getValue () .at (0)));
			const auto rhs = axes .at (std::tolower (connectedAxis .getValue () .at (1)));

			if (rhs == axis)
				scale [lhs] = scale [rhs];
		}
		catch (const std::out_of_range &)
		{ }
	}

	Matrix4d snap;
	snap .scale (scale);

	snap *= getOffset (master, shape, snap, shape .axes () [axis] * sgn);

	return snap * currentMatrix;
}

Matrix4d
X3DGridTool::getUniformScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
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

	if (getScaleFromEdge (master))
	{
		// Uniform scale from corner.

		const auto point  = points [tool];
		auto before = point - position;
		auto after  = getSnapPosition (point * inverse (grid), normalize (inverse (grid) .mult_dir_matrix (before))) * grid - position;
		
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
	   // Scale from center.

		for (const auto & point : points)
		{
			const auto before = point - position;
			const auto after  = getSnapPosition (point * inverse (grid), normalize (inverse (grid) .mult_dir_matrix (before))) * grid - position;
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

	snap *= getOffset (master, bbox, snap, points [tool] - bbox .center ());

	return absoluteMatrix * snap * inverse (master -> getTransformationMatrix ());
}

Matrix4d
X3DGridTool::getOffset (const X3DWeakPtr <X3DTransformNodeTool> & master, const Box3d & bbox, const Matrix4d scaledMatrix, const Vector3d & offset) const
{
	// To keep the bbox center at its point we must compute a translation offset.

	Vector3d distanceFromCenter = bbox .center ();

	if (getScaleFromEdge (master))
		distanceFromCenter -= offset;

	Matrix4d translation;
	translation .set (distanceFromCenter - scaledMatrix .mult_dir_matrix (distanceFromCenter));

	return translation;
}

void
X3DGridTool::connectTranslation (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> translation () .removeInterest (&X3DGridTool::connectTranslation, this);
		tool -> translation () .addInterest (&X3DGridTool::set_translation, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

void
X3DGridTool::connectRotation (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> rotation () .removeInterest (&X3DGridTool::connectRotation, this);
		tool -> rotation () .addInterest (&X3DGridTool::set_rotation, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

void
X3DGridTool::connectScale (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> scale () .removeInterest (&X3DGridTool::connectScale, this);
		tool -> scale () .addInterest (&X3DGridTool::set_scale, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

bool
X3DGridTool::getScaleFromEdge (const X3DWeakPtr <X3DTransformNodeTool> & master) const
{
	if (master -> scaleFromEdge () and getBrowser () -> getControlKey ())
		return true;

	if (not master -> scaleFromCenter () and master -> scaleFromEdge ())
		return true;

	return false;
}

X3DGridTool::~X3DGridTool ()
{ }

} // X3D
} // titania
