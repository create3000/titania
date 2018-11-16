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

#include "SnapTargetTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Editing/X3DEditor.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

const ComponentType SnapTargetTool::component      = ComponentType::TITANIA;
const std::string   SnapTargetTool::typeName       = "SnapTargetTool";
const std::string   SnapTargetTool::containerField = "SnapTool";

SnapTargetTool::Fields::Fields () :
	snapToCenter (new SFBool (true)),
	     snapped (new SFBool ())
{ }

SnapTargetTool::SnapTargetTool (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      X3DSnapTool (),
	           fields (),
	   transformNodes (),
	 activeSnapTarget (this),
	focusInConnection ()
{
	addType (X3DConstants::SnapTargetTool);

	addChildObjects (transformNodes,
	                 activeSnapTarget);

	addField (inputOutput, "metadata",     metadata ());
	addField (inputOutput, "enabled",      enabled ());
	addField (inputOutput, "position",     position ());
	addField (inputOutput, "normal",       normal ());
	addField (inputOutput, "snapToCenter", snapToCenter ());
	addField (outputOnly,  "snapped",      snapped ());
}

X3DBaseNode*
SnapTargetTool::create (X3DExecutionContext* const executionContext) const
{
	return new SnapTargetTool (executionContext);
}

void
SnapTargetTool::initialize ()
{
	X3DSnapTool::initialize ();

	const auto & dependentContext = getBrowser () -> getDependentContext ();

	if (dependentContext)
	{
		const auto & snapTarget = dependentContext -> getSnapTarget ();

		snapTarget -> enabled ()      .addInterest (enabled ());
		snapTarget -> position ()     .addInterest (position ());
		snapTarget -> normal ()       .addInterest (normal ());
		snapTarget -> snapped ()      .addInterest (snapped ());
		snapTarget -> snapToCenter () .addInterest (snapToCenter ());

		enabled ()      .addInterest (snapTarget -> enabled ());
		position ()     .addInterest (snapTarget -> position ());
		normal ()       .addInterest (snapTarget -> normal ());
		snapped ()      .addInterest (snapTarget -> snapped ());
		snapToCenter () .addInterest (snapTarget -> snapToCenter ());

		enabled ()      = snapTarget -> enabled ();
		position ()     = snapTarget -> position ();
		normal ()       = snapTarget -> normal ();
		snapped ()      = snapTarget -> snapped ();
		snapToCenter () = snapTarget -> snapToCenter ();
	}

	getBrowser () -> getTransformTools () .addInterest (&SnapTargetTool::set_transform_tools, this);

	enabled () .addInterest (&SnapTargetTool::set_enabled,         this);
	enabled () .addInterest (&SnapTargetTool::set_transform_tools, this);

	set_enabled ();
	set_transform_tools ();
}

void
SnapTargetTool::realize ()
{
	try
	{
		X3DSnapTool::realize ();

		getToolNode () -> setField <SFString> ("type", "SNAP_TARGET");

		auto & set_snapped = getToolNode () -> getField <SFBool> ("set_snapped");
		snapped () .addInterest (set_snapped);
		set_snapped = snapped ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
SnapTargetTool::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getBrowser () -> getTransformTools () .removeInterest (&SnapTargetTool::set_transform_tools, this);

	X3DSnapTool::setExecutionContext (executionContext);

	getBrowser () -> getTransformTools () .addInterest (&SnapTargetTool::set_transform_tools, this);

	set_enabled ();
	set_transform_tools ();
}

void
SnapTargetTool::set_enabled ()
{
	focusInConnection .disconnect ();

	if (enabled ())
	{
		focusInConnection = getBrowser () -> signal_focus_in_event () .connect (sigc::mem_fun (this, &SnapTargetTool::on_focus_in_event));
	
		if (getBrowser () -> has_focus ())
			on_focus_in_event (nullptr);
	}
}

void
SnapTargetTool::set_transform_tools ()
{
	const auto & value = getBrowser () -> getTransformTools ();

	for (const auto & transformNode : transformNodes)
	{
		try
		{
			transformNode -> translation () .removeInterest (&SnapTargetTool::set_translation, this);
			transformNode -> rotation ()    .removeInterest (&SnapTargetTool::set_rotation,    this);
			transformNode -> scale ()       .removeInterest (&SnapTargetTool::set_scale,       this);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}

	transformNodes = value;

	if (enabled ())
	{
		for (const auto & tool : value)
		{
			try
			{
				tool -> translation () .addInterest (&SnapTargetTool::set_translation, this, tool);
				tool -> rotation ()    .addInterest (&SnapTargetTool::set_rotation,    this, tool);
				tool -> scale ()       .addInterest (&SnapTargetTool::set_scale,       this, tool);
			}
			catch (const Error <DISPOSED> & error)
			{ }
		}
	}
}

void
SnapTargetTool::set_translation (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		// If Shift-key is pressed disable snapping.
		if (not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
			return;

		if (master -> getActiveTool () not_eq ToolType::TRANSLATE)
			return;

		const auto dynamicSnapDistance = getDynamicSnapDistance ();
		const auto absolutePosition    = Vector3d (position () .getValue ()) * getModelMatrix ();
		const auto absoluteMatrix      = master -> getCurrentMatrix () * master -> getModelMatrix ();
		const auto bbox                = master -> X3DGroupingNode::getBBox () * absoluteMatrix;
		const auto center              = (snapToCenter () and not master -> getKeepCenter ()) ? Vector3d (master -> center () .getValue ()) * absoluteMatrix : bbox .center ();
		const auto axes                = bbox .axes ();
		const auto normals             = bbox .normals ();

		// Determine snap translation.

		const auto xCenters = std::vector <Vector3d> ({ center, bbox .center () + axes .x (), bbox .center () - axes .x () });
		const auto xAxes    = std::vector <Vector3d> ({ axes .x (), axes .x (), -axes .x () });
		const auto xNormals = std::vector <Vector3d> ({ normals .x (), normals .x (), -normals .x () });

		const auto yCenters = std::vector <Vector3d> ({ center, bbox .center () + axes .y (), bbox .center () - axes .y () });
		const auto yAxes    = std::vector <Vector3d> ({ axes .y (), axes .y (), -axes .y () });
		const auto yNormals = std::vector <Vector3d> ({ normals .y (), normals .y (), -normals .y () });

		const auto zCenters = std::vector <Vector3d> ({ center, bbox .center () + axes .z (), bbox .center () - axes .z () });
		const auto zAxes    = std::vector <Vector3d> ({ axes .z (), axes .z (), -axes .z () });
		const auto zNormals = std::vector <Vector3d> ({ normals .z (), normals .z (), -normals .z () });

		const auto snapTranslation = getSnapTranslation (absolutePosition, xCenters, xAxes, xNormals, dynamicSnapDistance) +
		                             getSnapTranslation (absolutePosition, yCenters, yAxes, yNormals, dynamicSnapDistance) +
		                             getSnapTranslation (absolutePosition, zCenters, zAxes, zNormals, dynamicSnapDistance);

		snapped () = abs (snapTranslation) > 0.0001;

		if (snapTranslation == Vector3d ())
			return;

		// Snap master.

		const auto snapMatrix    = Matrix4d (snapTranslation);
		const auto currentMatrix = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());

		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);

		// Apply transformation to transformation group.

		master -> translation () .removeInterest (&SnapTargetTool::set_translation, this);
		master -> translation () .addInterest (&SnapTargetTool::connectTranslation, this, master);
	
		setTransformGroup (master, absoluteMatrix * snapMatrix);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
SnapTargetTool::set_rotation (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		// If Shift-key or Ctrl-key is pressed disable snapping.
		if ((not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()) or (getBrowser () -> getControlKey () and not getBrowser () -> getShiftKey ()))
			return;

		if (master -> getActiveTool () not_eq ToolType::ROTATE)
			return;

		const auto absolutePosition = Vector3d (position () .getValue ()) * getModelMatrix ();
		const auto absoluteNormal   = normalize (getModelMatrix () .mult_dir_matrix (Vector3d (normal () .getValue ())));
		const auto matrixBefore     = Matrix4d (master -> getMatrix ()) * master -> getModelMatrix (); // Matrix before transformation
		const auto matrixAfter      = master -> getCurrentMatrix () * master -> getModelMatrix ();     // Matrix after transformation
		const auto absoluteMatrix   = matrixAfter;
		const auto bbox             = master -> X3DGroupingNode::getBBox () * absoluteMatrix;

		// Determine rotation axis and the tho snap axes.

		const auto distances = std::vector <double> ({ dot (normalize (matrixAfter .x_axis ()), normalize (matrixBefore .x_axis ())),
		                                               dot (normalize (matrixAfter .y_axis ()), normalize (matrixBefore .y_axis ())),
		                                               dot (normalize (matrixAfter .z_axis ()), normalize (matrixBefore .z_axis ())) });

		const auto index0 = std::max_element (distances .cbegin (), distances .cend ()) - distances .cbegin (); // Index of rotation axis
		const auto index1 = (index0 + 1) % distances .size ();
		const auto index2 = (index0 + 2) % distances .size ();

		const auto axes = std::vector <Vector3d> ({ matrixAfter .x_axis (), matrixAfter .y_axis (), matrixAfter .z_axis () }); // Rotation axis, equates to grid normal

		const auto axis1 = normalize (axes [index1]); // Snap axis 1
		const auto axis2 = normalize (axes [index2]); // Snap axis 2
		const auto axis0 = normalize (cross (axis1, axis2)); // Rotation plane normal

		// Determine snap vector, from center to the position of this SnapTarget, projected onto plane of rotation axis.
		// If the angle between the normal of this SnapTarget and rotation axis is very small (<10°) the vector from
		// center to the position of this SnapTarget is used.

		const auto center     = Vector3d (master -> center () .getValue ()) * absoluteMatrix;
		const auto useNormal  = std::abs (dot (absoluteNormal, axis0)) < std::cos (radians (10.0));
		const auto snapVector = normalize (cross (cross (axis0, useNormal ? absoluteNormal : absolutePosition - center), axis0));

		// Determine snap point onto plane and axes points onto plane with same distance to center as snap point.

		const auto dynamicSnapDistance = getDynamicSnapDistance () * 2;
		const auto distance            = std::max (math::distance (bbox .center (), center), abs (bbox .size ()));
		const auto snapPoint           = snapVector * distance;
		const auto point1a             = axis1 * distance;
		const auto point1b             = -axis1 * distance;
		const auto point2a             = axis2 * distance;
		const auto point2b             = -axis2 * distance;

		// Determine snap rotation.

		const auto distance1a   = math::distance (snapPoint, point1a);
		const auto distance1b   = math::distance (snapPoint, point1b);
		const auto distance1    = std::min (distance1a, distance1b);
		const auto distance2a   = abs (snapPoint - point2a);
		const auto distance2b   = abs (snapPoint - point2b);
		const auto distance2    = std::min (distance2a, distance2b);
		auto       snapRotation = Rotation4d ();

		if (distance1 < distance2)
		{
			if (distance1 < dynamicSnapDistance)
			{
				snapRotation = Rotation4d (inverse (master -> getModelMatrix ()) .mult_dir_matrix (distance1a < distance1b ? axis1 : -axis1),
		                                 inverse (master -> getModelMatrix ()) .mult_dir_matrix (snapVector));
			}
		}
		else
		{
			if (distance2 < dynamicSnapDistance)
			{
				snapRotation = Rotation4d (inverse (master -> getModelMatrix ()) .mult_dir_matrix (distance2a < distance2b ? axis2 : -axis2),
		                                 inverse (master -> getModelMatrix ()) .mult_dir_matrix (snapVector));
			}
		}

		snapped () = std::abs (snapRotation .angle ()) > 0.0001;

		if (snapRotation == Rotation4d ())
			return;

		// Snap master.

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

		master -> rotation () .removeInterest (&SnapTargetTool::set_rotation, this);
		master -> rotation () .addInterest (&SnapTargetTool::connectRotation, this, master);
	
		setTransformGroup (master, currentMatrix * master -> getModelMatrix ());
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
SnapTargetTool::set_scale (const X3DWeakPtr <X3DTransformNodeTool> & master)
{
	try
	{
		// If Shift-key or Ctrl+Shift-key is pressed disable snapping.
		if ((not getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()) or (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ()))
			return;
	
		const auto tool = int32_t (master -> getActiveTool ()) - int32_t (ToolType::SCALE);

		if (tool < 0)
			return;

		const auto scaleMatrix   = tool < 6 ? getScaleMatrix (master, tool) : getUniformScaleMatrix (master, tool - 6);
		const auto currentMatrix = scaleMatrix .first;

		if (not scaleMatrix .second)
			return;

		if (master -> getKeepCenter ())
			master -> setMatrixKeepCenter (currentMatrix);
		else
			master -> setMatrix (currentMatrix);
	
		// Apply transformation to transformation group.

		master -> scale () .removeInterest (&SnapTargetTool::set_scale, this);
		master -> scale () .addInterest (&SnapTargetTool::connectScale, this, master);
	
		setTransformGroup (master, currentMatrix * master -> getModelMatrix ());
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

bool
SnapTargetTool::on_focus_in_event (GdkEventFocus* event)
{
	const auto & dependentContext = getBrowser () -> getDependentContext ();

	if (dependentContext)
		dependentContext -> getSnapTarget () -> setActiveSnapTarget (this);
	else
		setActiveSnapTarget (this);

	return false;
}

bool
SnapTargetTool::on_button_press_event (GdkEventButton* event)
{
	if (getBrowser () -> getControlKey ())
		return false;

	return X3DSnapTool::on_button_press_event (event);
}

bool
SnapTargetTool::on_button_release_event (GdkEventButton* event)
{
	snapped () = false;

	return X3DSnapTool::on_button_release_event (event);
}

double
SnapTargetTool::getDynamicSnapDistance () const
{
	try
	{
		const auto executionContext    = X3DExecutionContextPtr (activeSnapTarget -> getToolNode ());
		const auto sphereNode          = executionContext -> getNamedNode <Transform> ("Sphere");
		const auto modelMatrix         = X3DEditor::getModelMatrix (executionContext, sphereNode) * activeSnapTarget -> getModelMatrix ();
		const auto bbox                = sphereNode -> getBBox () * modelMatrix;
		const auto dynamicSnapDistance = maximum_norm (bbox .size ()) / 2;

		return dynamicSnapDistance;
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;

		return 0.2;
	}
}

Vector3d
SnapTargetTool::getSnapTranslation (const Vector3d & position,
                                    const std::vector <Vector3d> & centers,
                                    const std::vector <Vector3d> & axes,
                                    const std::vector <Vector3d> & normals,
                                    const double snapDistance) const
{
	// Return first successful snap translation.

	for (size_t i = 0; i < centers .size (); ++ i)
	{
		const auto & center                      = centers [i];
		const auto & axis                        = axes [i];
		const auto & normal                      = normals [i];
		const auto   positionPlane               = Plane3d (position, normal);
		const auto   axisLine                    = Line3d (center, abs (axis) == 0 ? normal : normalize (axis));
		const auto & [intersection, intersected] = positionPlane .intersects (axisLine);

		if (not intersected)
			continue;

		const auto translation = intersection - center;

		if (abs (translation) > snapDistance)
			continue;

		return translation;
	}

	return Vector3d ();
}

std::pair <Matrix4d, bool>
SnapTargetTool::getScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
{
	constexpr double infinity  = std::numeric_limits <double>::infinity ();
	constexpr double MIN_DELTA = 1e-6;
	constexpr double MIN_RATIO = 1e-3;

	const auto dynamicSnapDistance = getDynamicSnapDistance ();
	const auto absolutePosition    = Vector3d (position () .getValue ()) * getModelMatrix ();
	const auto absoluteMatrix      = master -> getCurrentMatrix () * master -> getModelMatrix ();
	const auto bbox                = master -> X3DGroupingNode::getBBox () .aabb () * absoluteMatrix; // Absolute BBox.
	const auto axes                = bbox .axes ();
	const auto normals             = bbox .normals ();

	if (getScaleFromEdge (master))
	{
		// Scale from edge.

		const size_t axis            = tool % 3;
		const double sgn             = tool < 3 ? 1 : -1;
		const auto   aCenters        = std::vector <Vector3d> ({ bbox .center () + axes [axis] * sgn });
		const auto   aAxes           = std::vector <Vector3d> ({ axes [axis] * sgn });
		const auto   aNormals        = std::vector <Vector3d> ({ normals [axis] * sgn });
		const auto   snapTranslation = getSnapTranslation (absolutePosition, aCenters, aAxes, aNormals, dynamicSnapDistance);

		snapped () = abs (snapTranslation) > 0.0001;

		if (snapTranslation == Vector3d ())
			return std::pair (Matrix4d (), false);

		const auto aBefore = 2 * aAxes [0] [axis];
		const auto aAfter  = aAxes [0] [axis] + snapTranslation [axis] + aAxes [0] [axis];
		const auto aDelta  = aAfter - aBefore;
		const auto aRatio  = aAfter / aBefore;

		if (std::abs (aDelta) < MIN_DELTA or std::abs (aRatio) < MIN_RATIO or std::isnan (aRatio) or std::abs (aRatio) == infinity)
			return std::pair (Matrix4d (), false);

		auto snapScale = Vector3d (1, 1, 1);

		snapScale [axis] = aRatio;
		snapScale        = getConnectedAxes (master, axis, snapScale);

		auto snapMatrix = Matrix4d (Vector3d (), Rotation4d (), snapScale);

		snapMatrix *= getOffset (master, bbox, snapMatrix, axes [axis] * sgn);
		snapMatrix  = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());

		return std::pair (snapMatrix, true);
	}
	else
	{
		// Scale one axis in both directions.

		const size_t axis            = tool % 3;
		const double sgn             = tool < 3 ? 1 : -1;
		const auto   aCenters        = std::vector <Vector3d> ({ bbox .center () + axes [axis], bbox .center () - axes [axis] });
		const auto   aAxes           = std::vector <Vector3d> ({ axes [axis], -axes [axis] });
		const auto   aNormals        = std::vector <Vector3d> ({ normals [axis], -normals [axis] });
		const auto   snapTranslation = getSnapTranslation (absolutePosition, aCenters, aAxes, aNormals, dynamicSnapDistance);

		snapped () = abs (snapTranslation) > 0.0001;

		if (snapTranslation == Vector3d ())
			return std::pair (Matrix4d (), false);

		const auto aSnapCenter = std::vector <Vector3d> ({ aCenters [0] + snapTranslation, aCenters [1] + snapTranslation });
		const auto aAxis       = distance (aSnapCenter [0], absolutePosition) < distance (aSnapCenter [1], absolutePosition) ? 0 : 1;
		const auto aBefore     = aAxes [aAxis] [axis];
		const auto aAfter      = aAxes [aAxis] [axis] + snapTranslation [axis];
		const auto aDelta      = aAfter - aBefore;
		const auto aRatio      = aAfter / aBefore;

		if (std::abs (aDelta) < MIN_DELTA or std::abs (aRatio) < MIN_RATIO or std::isnan (aRatio) or std::abs (aRatio) == infinity)
			return std::pair (Matrix4d (), false);

		auto snapScale = Vector3d (1, 1, 1);

		snapScale [axis] = aRatio;
		snapScale        = getConnectedAxes (master, axis, snapScale);

		auto snapMatrix = Matrix4d (Vector3d (), Rotation4d (), snapScale);

		snapMatrix *= getOffset (master, bbox, snapMatrix, axes [axis] * sgn);
		snapMatrix  = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());

		return std::pair (snapMatrix, true);
	}

	return std::pair (Matrix4d (), false);
}

std::pair <Matrix4d, bool>
SnapTargetTool::getUniformScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t tool)
{
	constexpr double infinity = std::numeric_limits <double>::infinity ();

	const auto dynamicSnapDistance = getDynamicSnapDistance ();
	const auto absolutePosition    = Vector3d (position () .getValue ()) * getModelMatrix ();
	const auto absoluteMatrix      = master -> getCurrentMatrix () * master -> getModelMatrix ();
	const auto bbox                = master -> X3DGroupingNode::getBBox () .aabb () * absoluteMatrix; // Absolute BBox.
	const auto center              = bbox .center ();
	const auto points              = bbox .points ();
	const auto normals             = std::vector <Vector3d> ({ Vector3d (1, 0, 0), Vector3d (0, 1, 0), Vector3d (0, 0, 1) });

	if (getScaleFromEdge (master))
	{
		// Uniform scale from corner.

		const auto point            = points [tool];
		const auto pCenters         = std::vector <Vector3d> ({ point });
		const auto pAxes            = std::vector <Vector3d> ({ point - center });
		const auto pNormals         = std::vector <Vector3d> ({ normalize (point - center) });
		const auto pointLine        = Line3d (pCenters [0], pNormals [0]);
		auto       snapTranslations = std::vector <Vector3d> ();

		for (const auto & normal : normals)
		{
			const auto   positionPlane               = Plane3d (absolutePosition, normal);
			const auto & [intersection, intersected] = positionPlane .intersects (pointLine);

			if (not intersected)
				continue;

			snapTranslations .emplace_back (getSnapTranslation (intersection, pCenters, pAxes, pNormals, dynamicSnapDistance));
		}

		snapTranslations .erase (std::remove (snapTranslations .begin (), snapTranslations .end (), Vector3d ()), snapTranslations .end ());

		if (snapTranslations .empty ())
		{
			snapped () = false;
			return std::pair (Matrix4d (), false);
		}

		const auto minTranslation = std::min_element (snapTranslations .cbegin (), snapTranslations .cend (),
		[ ] (const Vector3d & lhs, const Vector3d & rhs)
		{
			return abs (lhs) < abs (rhs);
		});

		const auto snapTranslation = *minTranslation;

		snapped () = abs (snapTranslation) > 0.0001;

		if (snapTranslation == Vector3d ())
			return std::pair (Matrix4d (), false);

		auto before = point - center;
		auto after  = point + snapTranslation - center;

		after  += before;
		before *= 2;
	
		const auto delta = after - before;
		const auto ratio = after / before;

		auto min = infinity;

		for (size_t i = 0; i < 3; ++ i)
		{
			const auto r = std::abs (ratio [i] - 1);

			if (delta [i] and r < std::abs (min - 1))
				min = ratio [i];
		}		
	
		if (min == 0 or min == infinity)
		{
			snapped () = false;
			return std::pair (Matrix4d (), false);
		}

		auto snapMatrix = Matrix4d ();
	
		snapMatrix .scale (Vector3d (min, min, min));
	
		snapMatrix *= getOffset (master, bbox, snapMatrix, points [tool] - bbox .center ());
		snapMatrix  = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());
	
		return std::pair (snapMatrix, true);
	}
	else
	{
	   // Scale from center.

		auto min = infinity;

		for (const auto & point : points)
		{
			const auto pCenters         = std::vector <Vector3d> ({ point });
			const auto pAxes            = std::vector <Vector3d> ({ point - center });
			const auto pNormals         = std::vector <Vector3d> ({ normalize (point - center) });
			const auto pointLine        = Line3d (pCenters [0], pNormals [0]);
			auto       snapTranslations = std::vector <Vector3d> ();
	
			for (const auto & normal : normals)
			{
				const auto   positionPlane               = Plane3d (absolutePosition, normal);
				const auto & [intersection, intersected] = positionPlane .intersects (pointLine);
	
				if (not intersected)
					continue;

				snapTranslations .emplace_back (getSnapTranslation (intersection, pCenters, pAxes, pNormals, dynamicSnapDistance));
			}

			snapTranslations .erase (std::remove (snapTranslations .begin (), snapTranslations .end (), Vector3d ()), snapTranslations .end ());

			const auto minTranslation = std::min_element (snapTranslations .cbegin (), snapTranslations .cend (),
			[ ] (const Vector3d & lhs, const Vector3d & rhs)
			{
				return abs (lhs) < abs (rhs);
			});
	
			const auto snapTranslation = *minTranslation;

			const auto before = point - center;
			const auto after  = point + snapTranslation - center;
			const auto delta  = after - before;
			const auto ratio  = after / before;

			for (size_t i = 0; i < 3; ++ i)
			{
				const auto r = std::abs (ratio [i] - 1);

				if (delta [i] and r < std::abs (min - 1))
					min = ratio [i];
			}
		}

		if (min == 0 or min == infinity)
		{
			snapped () = false;
			return std::pair (Matrix4d (), false);
		}

		snapped () = true;

		auto snapMatrix = Matrix4d ();
	
		snapMatrix .scale (Vector3d (min, min, min));
	
		snapMatrix *= getOffset (master, bbox, snapMatrix, points [tool] - bbox .center ());
		snapMatrix  = absoluteMatrix * snapMatrix * inverse (master -> getModelMatrix ());
	
		return std::pair (snapMatrix, true);
	}

	return std::pair (Matrix4d (), false);
}

Vector3d
SnapTargetTool::getConnectedAxes (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t axis, Vector3d scale) const
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
SnapTargetTool::getOffset (const X3DWeakPtr <X3DTransformNodeTool> & master, const Box3d & bbox, const Matrix4d scaledMatrix, const Vector3d & offset) const
{
	// To keep the bbox center at its point we must compute a translation offset.

	auto distanceFromCenter = bbox .center ();

	if (getScaleFromEdge (master))
		distanceFromCenter -= offset;

	const auto translation = Matrix4d (distanceFromCenter - scaledMatrix .mult_dir_matrix (distanceFromCenter));

	return translation;
}

bool
SnapTargetTool::getScaleFromEdge (const X3DWeakPtr <X3DTransformNodeTool> & master) const
{
	if (master -> scaleFromEdge () and getBrowser () -> getControlKey ())
		return true;

	if (not master -> scaleFromCenter () and master -> scaleFromEdge ())
		return true;

	return false;
}

/// Apply transformation to transformation group.
void
SnapTargetTool::setTransformGroup (const X3DWeakPtr <X3DTransformNodeTool> & master, const Matrix4d & snapMatrix)
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
SnapTargetTool::connectTranslation (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> translation () .removeInterest (&SnapTargetTool::connectTranslation, this);
		tool -> translation () .addInterest (&SnapTargetTool::set_translation, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

void
SnapTargetTool::connectRotation (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> rotation () .removeInterest (&SnapTargetTool::connectRotation, this);
		tool -> rotation () .addInterest (&SnapTargetTool::set_rotation, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

void
SnapTargetTool::connectScale (const X3DWeakPtr <X3DTransformNodeTool> & tool)
{
	try
	{
		tool -> scale () .removeInterest (&SnapTargetTool::connectScale, this);
		tool -> scale () .addInterest (&SnapTargetTool::set_scale, this, tool);
	}
	catch (const Error <DISPOSED> &)
	{ }
}

} // X3D
} // titania
