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
	     snapped (new SFBool ()),
	snapToCenter (new SFBool (true))
{ }

SnapTargetTool::SnapTargetTool (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DSnapTool (),
	          fields (),
	  transformNodes (),
	activeSnapTarget (this)
{
	addType (X3DConstants::SnapTargetTool);

	addChildObjects (transformNodes,
	                 activeSnapTarget);

	addField (inputOutput, "metadata",     metadata ());
	addField (inputOutput, "enabled",      enabled ());
	addField (inputOutput, "position",     position ());
	addField (inputOutput, "normal",       normal ());
	addField (inputOutput, "snapped",      snapped ());
	addField (inputOutput, "snapToCenter", snapToCenter ());
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

	getBrowser () -> getTransformTools () .addInterest (&SnapTargetTool::set_transform_tools, this);
	enabled () .addInterest (&SnapTargetTool::set_enabled, this);

	set_transform_tools (getBrowser () -> getTransformTools ());
}

void
SnapTargetTool::realize ()
{
	try
	{
		X3DSnapTool::realize ();

		getBrowser () -> signal_focus_in_event () .connect (sigc::mem_fun (this, &SnapTargetTool::on_focus_in_event));

		getToolNode () -> setField <SFString> ("type", "SNAP_TARGET");

		auto & set_snapped = getToolNode () -> getField <SFBool> ("set_snapped");
		snapped () .addInterest (set_snapped);
		set_snapped = snapped ();

		if (getBrowser () -> has_focus ())
			on_focus_in_event (nullptr);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
SnapTargetTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> getTransformTools () .removeInterest (&SnapTargetTool::set_transform_tools, this);

	X3DSnapTool::setExecutionContext (executionContext);

	getBrowser () -> getTransformTools () .addInterest (&SnapTargetTool::set_transform_tools, this);

	set_transform_tools (getBrowser () -> getTransformTools ());
}

void
SnapTargetTool::set_enabled ()
{
	set_transform_tools (getBrowser () -> getTransformTools ());
}

void
SnapTargetTool::set_transform_tools (const X3DWeakPtrArray <X3DTransformNodeTool> & value)
{
	for (const auto & transformNode : transformNodes)
	{
		try
		{
			transformNode -> translation () .removeInterest (&SnapTargetTool::set_translation, this);
		}
		catch (const Error <DISPOSED> &)
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
		const auto bbox                = Box3d (master -> X3DGroupingNode::getBBox ()) * absoluteMatrix;
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

		const auto translation = getSnapTranslation (absolutePosition, xCenters, xAxes, xNormals, dynamicSnapDistance) +
		                         getSnapTranslation (absolutePosition, yCenters, yAxes, yNormals, dynamicSnapDistance) +
		                         getSnapTranslation (absolutePosition, zCenters, zAxes, zNormals, dynamicSnapDistance);

		snapped () = abs (translation) > 0.0001;

		if (translation == Vector3d ())
			return;

		// Snap master.

		const auto snapMatrix    = Matrix4d (translation);
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
		__LOG__ << error .what () << std::endl;
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
		const auto & center        = centers [i];
		const auto & axis          = axes [i];
		const auto & normal        = normals [i];
		const auto   positionPlane = Plane3d (position, normal);
		const auto   axisLine      = Line3d (center, normalize (axis));
		const auto   intersection  = positionPlane .intersects (axisLine);

		if (not intersection .second)
			continue;

		const auto translation = intersection .first - center;

		if (abs (translation) > snapDistance)
			continue;

		return translation;
	}

	return Vector3d ();
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

} // X3D
} // titania
