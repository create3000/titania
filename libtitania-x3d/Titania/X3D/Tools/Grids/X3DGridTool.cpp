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
#include "../../Components/X_ITE/TouchGroup.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"
#include "../../Tools/SnapTool/SnapTargetTool.h"

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
	       snapping (new SFBool (true)),
	   snapToCenter (new SFBool (true)),
	   snapDistance (new SFDouble (0.25)),
	      collision (new SFBool ()),
	        handles (new SFBool (true)),
	       isActive (new SFBool (true))
{ }

X3DGridTool::X3DGridTool () :
	X3DActiveLayerTool (),
	            fields (),
	    transformNodes (),
	    gridTouchGroup (),
	 handlesTouchGroup ()
{
	addType (X3DConstants::X3DGridTool);

	addChildObjects (transformNodes,
	                 gridTouchGroup,
	                 handlesTouchGroup);

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
X3DGridTool::realize ()
{
	try
	{
		X3DActiveLayerTool::realize ();

		gridTouchGroup    = getInlineNode () -> getExportedNode <TouchGroup> ("GridTouchGroup");
		handlesTouchGroup = getInlineNode () -> getExportedNode <TouchGroup> ("HandlesTouchGroup");
	
		auto & set_translation = getToolNode () -> getField <SFVec3f> ("set_translation");
		translation ()  .addInterest (set_translation);
		set_translation .addInterest (translation ());
		set_translation .addEventObject (translation ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		auto & set_rotation = getToolNode () -> getField <SFRotation> ("set_rotation");
		rotation ()  .addInterest (set_rotation);
		set_rotation .addInterest (rotation ());
		set_rotation .addEventObject (rotation ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		auto & set_scale = getToolNode () -> getField <SFVec3f> ("set_scale");
		scale ()  .addInterest (set_scale);
		set_scale .addInterest (scale ());
		set_scale .addEventObject (scale ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		auto & set_dimension = getToolNode () -> getField <MFInt32> ("set_dimension");
		dimension ()  .addInterest (set_dimension);
		set_dimension .addInterest (dimension ());
		set_dimension .addEventObject (dimension ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		auto & set_majorLineEvery = getToolNode () -> getField <MFInt32> ("set_majorLineEvery");
		majorLineEvery ()  .addInterest (set_majorLineEvery);
		set_majorLineEvery .addInterest (majorLineEvery ());
		set_majorLineEvery .addEventObject (majorLineEvery ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		auto & set_majorLineOffset = getToolNode () -> getField <MFInt32> ("set_majorLineOffset");
		majorLineOffset ()  .addInterest (set_majorLineOffset);
		set_majorLineOffset .addInterest (majorLineOffset ());
		set_majorLineOffset .addEventObject (majorLineOffset ()); // TODO: use normal assign, and remove addEventObject from X3DField.

		collision () .addInterest (getToolNode () -> getField <SFBool> ("collision"));
		getToolNode () -> setField <SFBool> ("collision", collision ());

		handles () .addInterest (getToolNode () -> getField <SFBool> ("handles"));
		getToolNode () -> setField <SFBool> ("handles", handles ());

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
X3DGridTool::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getBrowser () -> getTransformTools () .removeInterest (&X3DGridTool::set_transform_tools, this);

	X3DActiveLayerTool::setExecutionContext (executionContext);

	getBrowser () -> getTransformTools () .addInterest (&X3DGridTool::set_transform_tools, this);

	set_transform_tools (getBrowser () -> getTransformTools ());
}

bool
X3DGridTool::getPickable (X3DRenderObject* const renderObject) const
{
	try
	{
		if (renderObject -> getBrowser () -> getToolsPickable () .top ())
		{
			gridTouchGroup    -> enabled () = false;
			handlesTouchGroup -> enabled () = true;
		}
		else
		{
			gridTouchGroup    -> enabled () = true;
			handlesTouchGroup -> enabled () = false;
		}

		return true;
	}
	catch (const X3DError & error)
	{
		// __LOG__ << error .what () << std::endl;
		return renderObject -> getBrowser () -> getToolsPickable () .top ();
	}
}

///  Returns snap position from @a position in world space.
Vector3d
X3DGridTool::getSnapPosition (const Vector3d & position) const
{
	const auto gridMatrix   = Matrix4d (translation () .getValue (), rotation () .getValue (), scale () .getValue ()) * getModelMatrix ();
	const auto snapPosition = getSnapPosition (position * inverse (gridMatrix), true) * gridMatrix;

	return snapPosition;
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
		catch (const Error <DISPOSED> & error)
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
		if (not snapping ())
			return;

		// If Shift-key is pressed disable snapping.
		if (not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
			return;

		if (master -> getActiveTool () not_eq ToolType::TRANSLATE)
			return;

		if (getBrowser () -> getSnapTarget () -> enabled ())
			return;

		// The position is transformed to an absolute position and then transformed into the coordinate systwm of the grid
		// for easier snapping position calculation.
	
		// Get absolute position.
	
		const auto absoluteMatrix = master -> getCurrentMatrix () * master -> getModelMatrix ();
	
		Vector3d position;
	
		if (snapToCenter () and not master -> getKeepCenter ())
		{
			position = Vector3d (master -> center () .getValue ()) * absoluteMatrix;
		}
		else
		{
			// snapping to bbox center.
			const auto bbox = master -> getSubBBox () * absoluteMatrix;
			position = bbox .center ();
		}

__LOG__ << master -> getModelMatrix () << std::endl;
	
		// Calculate snapping position and apply absolute relative translation.
	
		const auto gridMatrix    = Matrix4d (translation () .getValue (), rotation () .getValue (), scale () .getValue ()) * getModelMatrix ();
		const auto snapMatrix    = Matrix4d (getSnapPosition (position * inverse (gridMatrix), true) * gridMatrix - position);
		const auto currentMatrix = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());

		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);

		// Apply transformation to transformation group.

		master -> translation () .removeInterest (&X3DGridTool::set_translation, this);
		master -> translation () .addInterest (&X3DGridTool::connectTranslation, this, master);
	
		setTransformGroup (master, absoluteMatrix * snapMatrix);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DGridTool::set_rotation (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		if (not snapping ())
			return;

		// If Shift-key or Ctrl-key is pressed disable snapping.
		if ((not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()) or (getBrowser () -> getControlKey () and not getBrowser () -> getShiftKey ()))
			return;

		if (master -> getActiveTool () not_eq ToolType::ROTATE)
			return;

		if (getBrowser () -> getSnapTarget () -> enabled ())
			return;

		const auto matrixBefore = Matrix4d (master -> getMatrix ()) * master -> getModelMatrix (); // Matrix before transformation
		const auto matrixAfter  = master -> getCurrentMatrix () * master -> getModelMatrix ();     // Matrix after transformation

		const auto distances = std::vector <double> ({ dot (normalize (matrixAfter .x_axis ()), normalize (matrixBefore .x_axis ())),
		                                               dot (normalize (matrixAfter .y_axis ()), normalize (matrixBefore .y_axis ())),
		                                               dot (normalize (matrixAfter .z_axis ()), normalize (matrixBefore .z_axis ())) });

		const auto index0 = std::max_element (distances .cbegin (), distances .cend ()) - distances .cbegin (); // Index of rotation axis
		const auto index1 = (index0 + 1) % distances .size ();
		const auto index2 = (index0 + 2) % distances .size ();

		const auto y = std::vector <Vector3d> ({ matrixAfter .x_axis (), matrixAfter .y_axis (), matrixAfter .z_axis () }); // Rotation axis, equates to grid normal
		const auto z = std::vector <Vector3d> ({ matrixAfter .y_axis (), matrixAfter .z_axis (), matrixAfter .y_axis () }); // Axis which snaps, later transformed to grid space

		const auto gridMatrix    = Matrix4d (translation () .getValue (), rotation () .getValue (), scale () .getValue ()) * getModelMatrix ();
		Vector3d   Y             = normalize (cross (y [index1], y [index2])); // Normal of rotation plane
		Vector3d   X             = cross (gridMatrix .y_axis (), Y); // Intersection between both planes
		Vector3d   Z             = cross (X, Y); // Front vector
		Matrix3d   rotationPlane = Matrix3d (X [0], X [1], X [2],   Y [0], Y [1], Y [2],   Z [0], Z [1], Z [2]);
		Rotation4d gridRotation  = Rotation4d (rotation () .getValue ());
		Matrix3d   gridPlane     = gridMatrix .submatrix ();

		// If X or Z are near 0 then Y is collinear to the y-axis.

		if (1 - std::abs (dot (normalize (gridMatrix .y_axis ()), Y)) < 1e-6)
		{
			rotationPlane = Matrix3d ();
			gridRotation  = Rotation4d ();
		}

		const auto vectorToSnap = z [index0];
		const auto vectorOnGrid = normalize (vectorToSnap * inverse (rotationPlane) * gridRotation * inverse (gridPlane)); // Vector inside grid space.
		const auto snapVector   = getSnapPosition (vectorOnGrid, false) * gridPlane * inverse (gridRotation) * rotationPlane;
		const auto snapRotation = Rotation4d (inverse (master -> getModelMatrix ()) .mult_dir_matrix (vectorToSnap),
                                            inverse (master -> getModelMatrix ()) .mult_dir_matrix (snapVector));

		const auto currentMatrix = Matrix4d (master -> translation ()      .getValue (),
		                                     master -> rotation ()         .getValue () * snapRotation,
		                                     master -> scale ()            .getValue (),
		                                     master -> scaleOrientation () .getValue (),
		                                     master -> center ()           .getValue ());

		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);
	
		// Apply transformation to transformation group.

		master -> rotation () .removeInterest (&X3DGridTool::set_rotation, this);
		master -> rotation () .addInterest (&X3DGridTool::connectRotation, this, master);
	
		setTransformGroup (master, currentMatrix * master -> getModelMatrix ());
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DGridTool::set_scale (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		if (not snapping ())
			return;

		// If Shift-key or Ctrl+Shift-key is pressed disable snapping.
		if ((not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()) or (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()))
			return;

		if (getBrowser () -> getSnapTarget () -> enabled ())
			return;
	
		const auto tool = int32_t (master -> getActiveTool ()) - int32_t (ToolType::SCALE);

		if (tool < 0)
			return;

		// All points are first transformed to grid space, then a snapping position is calculated, and then transformed back to absolute space.
	
		const auto currentMatrix = tool < 6 ? getScaleMatrix (master, tool) : getUniformScaleMatrix (master, tool - 6);
	
		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);
	
		// Apply transformation to transformation group.

		master -> scale () .removeInterest (&X3DGridTool::set_scale, this);
		master -> scale () .addInterest (&X3DGridTool::connectScale, this, master);
	
		setTransformGroup (master, currentMatrix * master -> getModelMatrix ());
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

Matrix4d
X3DGridTool::getScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
{
	// All points are first transformed to grid space, then a snapping position is calculated, and then transformed back to absolute space.

	constexpr double infinity  = std::numeric_limits <double>::infinity ();
	constexpr double MIN_DELTA = 1e-6;
	constexpr double MIN_RATIO = 1e-3;

	// Get absolute position.

	const auto currentMatrix  = master -> getCurrentMatrix ();
	const auto absoluteMatrix = currentMatrix * master -> getModelMatrix ();
	const auto geometry       = master -> getSubBBox (); // BBox of the geometry.
	const auto shape          = geometry .aabb ();                     // AABB BBox
	const auto bbox           = shape * absoluteMatrix;                // Absolute OBB of AABB
	const auto position       = bbox .center ();                       // Absolute position

	// Calculate snapping scale for one axis. The ratio is calculated in transforms sub space.

	const auto gridMatrix = Matrix4d (translation () .getValue (), rotation () .getValue (), scale () .getValue ()) * getModelMatrix ();

	const size_t axis         = tool % 3;
	const double sgn          = tool < 3 ? 1 : -1;
	const auto   direction    = bbox .axes () [axis] * sgn;
	const auto   point        = direction + position;
	const auto   snapPosition = getSnapPosition (point * inverse (gridMatrix), normalize (inverse (gridMatrix) .mult_dir_matrix (direction))) * gridMatrix;
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

	// We must procced with the original current matrix and a snapping scale of [1 1 1], for correct grouped event handling.

	if (std::abs (delta) < MIN_DELTA or std::abs (ratio) < MIN_RATIO or std::isnan (ratio) or std::abs (ratio) == infinity)
		return currentMatrix;

	auto snapScale = Vector3d (1, 1, 1);

	snapScale [axis] = ratio;
	snapScale        = getConnectedAxes (master, axis, snapScale);

	auto snapMatrix = Matrix4d ();

	snapMatrix .scale (snapScale);

	snapMatrix *= getOffset (master, shape, snapMatrix, shape .axes () [axis] * sgn);
	snapMatrix *= currentMatrix;

	return snapMatrix;
}

Matrix4d
X3DGridTool::getUniformScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
{
	// All points are first transformed to grid space, then a snapping position is calculated, and then transformed back to absolute space.

	constexpr double infinity = std::numeric_limits <double>::infinity ();

	// Get absolute position.

	const auto currentMatrix  = master -> getCurrentMatrix ();
	const auto absoluteMatrix = currentMatrix * master -> getModelMatrix ();
	const auto bbox           = master -> getSubBBox () .aabb () * absoluteMatrix; // Absolute BBox
	const auto position       = bbox .center (); // Absolute position

	// Calculate snapping scale and apply absolute relative translation.

	const auto gridMatrix = Matrix4d (translation () .getValue (), rotation () .getValue (), scale () .getValue ());
	const auto points     = bbox .points ();
	double     min        = infinity;

	if (getScaleFromEdge (master))
	{
		// Uniform scale from corner.

		const auto point  = points [tool];
		auto       before = point - position;
		auto       after  = getSnapPosition (point * inverse (gridMatrix), normalize (inverse (gridMatrix) .mult_dir_matrix (before))) * gridMatrix - position;
		
		after  += before;
		before *= 2;
	
		const auto delta = after - before;
		const auto ratio = after / before;

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
			const auto after  = getSnapPosition (point * inverse (gridMatrix), normalize (inverse (gridMatrix) .mult_dir_matrix (before))) * gridMatrix - position;
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

	// We must procced with the original current matrix and a snapping scale of [1 1 1], for correct grouped event handling.

	if (min == 0 or min == infinity)
		return currentMatrix;

	auto snapMatrix = Matrix4d ();

	snapMatrix .scale (Vector3d (min, min, min));

	snapMatrix *= getOffset (master, bbox, snapMatrix, points [tool] - bbox .center ());
	snapMatrix  = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());

	return snapMatrix;
}

Vector3d
X3DGridTool::getConnectedAxes (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t axis, Vector3d scale) const
{
	for (const auto & connectedAxis : basic::make_const_range (master -> connectedAxes ()))
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

			if (rhs == axis)
				scale [lhs] = scale [rhs];
		}
		catch (const std::out_of_range &)
		{ }
	}

	return scale;
}

Matrix4d
X3DGridTool::getOffset (const X3DWeakPtr <X3DTransformNodeTool> & master, const Box3d & bbox, const Matrix4d scaledMatrix, const Vector3d & offset) const
{
	// To keep the bbox center at its point we must compute a translation offset.

	auto distanceFromCenter = bbox .center ();

	if (getScaleFromEdge (master))
		distanceFromCenter -= offset;

	const auto translation = Matrix4d (distanceFromCenter - scaledMatrix .mult_dir_matrix (distanceFromCenter));

	return translation;
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

/// Apply transformation to transformation group.
void
X3DGridTool::setTransformGroup (const X3DWeakPtr <X3DTransformNodeTool> & master, const Matrix4d & snapMatrix)
{
	if (not master -> grouping ())
		return;

	if (not master -> grouped ())
		return;

	const auto groupMatrix      = master -> getGroupMatrix () * master -> getModelMatrix ();
	const auto differenceMatrix = inverse (groupMatrix) * snapMatrix;

	for (const auto & tool : getBrowser () -> getTransformTools ())
	{
		try
		{
			if (tool == master)
				continue;

			if (not tool -> grouping ())
				continue;

			if (not tool -> grouped ())
				continue;

			tool -> addAbsoluteMatrix (differenceMatrix, tool -> getKeepCenter ());
		}
		catch (const std::exception &)
		{ }
	}
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

X3DGridTool::~X3DGridTool ()
{ }

} // X3D
} // titania
