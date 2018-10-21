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

#include "X3DSnapTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../../Tools/Grids/X3DGridTool.h"
#include "../../Tools/Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

X3DSnapTool::Fields::Fields () :
	position (new SFVec3f ()),
	  normal (new SFVec3f (0, 1, 0))
{ }

X3DSnapTool::X3DSnapTool () :
	     X3DActiveLayerTool (),
	        sigc::trackable (),
	                 fields (),
	                 button (0),
	  buttonPressConnection (),
	buttonReleaseConnection (),
	 motionNotifyConnection ()
{
	addType (X3DConstants::X3DSnapTool);

	position () .setUnit (UnitCategory::LENGTH);
}

void
X3DSnapTool::initialize ()
{
	X3DActiveLayerTool::initialize ();

	requestAsyncLoad ({ get_tool ("SnapTool.x3dv") .str () });
}

void
X3DSnapTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DActiveLayerTool::setExecutionContext (executionContext);
}

void
X3DSnapTool::realize ()
{
	try
	{
		X3DActiveLayerTool::realize ();

		auto & set_position = getToolNode () -> getField <SFVec3f> ("set_position");
		position () .addInterest (set_position);
		set_position = position ();

		auto & set_normal = getToolNode () -> getField <SFVec3f> ("set_normal");
		normal () .addInterest (set_normal);
		set_normal = normal ();

		buttonPressConnection   .disconnect ();
		buttonReleaseConnection .disconnect ();

		buttonPressConnection   = getBrowser () -> signal_button_press_event ()   .connect (sigc::mem_fun (this, &X3DSnapTool::on_button_press_event),   false);
		buttonReleaseConnection = getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (this, &X3DSnapTool::on_button_release_event), false);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
X3DSnapTool::on_button_press_event (GdkEventButton* event)
{
	#ifndef TITANIA_DEBUG
	if (not enabled ())
		return false;
	#endif

	// If not in editing mode return.
	if (not getBrowser () -> getSelectable ())
		return false;

	// Viewer can always be activated by pressing Ctrl+Shift.
	if (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
		return false;

	if (button)
		return false;

	if (event -> button != 2)
		return false;

	if (not getBrowser () -> getSelectable () and not enabled ())
		return false;

	if (not touch (event -> x, event -> y))
	{
		// Disable Snap Object if picked in empty space.
		if (enabled ())
			enabled () = false;

		return false;
	}

	if (not enabled ())
		enabled () = true;

	button = event -> button;

	motionNotifyConnection = getBrowser () -> signal_motion_notify_event () .connect (sigc::mem_fun (this, &X3DSnapTool::on_motion_notify_event), false);

	update ();

	return true;
}

bool
X3DSnapTool::on_button_release_event (GdkEventButton* event)
{
	if (not button)
		return false;

	button = 0;

	motionNotifyConnection  .disconnect ();

	return false;
}

bool
X3DSnapTool::on_motion_notify_event (GdkEventMotion* event)
{
	if (not enabled ())
		return false;

	if (button != 2)
		return false;

	if (not touch (event -> x, event -> y))
		return false;

	update ();

	return false;
}

bool
X3DSnapTool::touch (const double x, const double y) const
{
	getBrowser () -> getToolsPickable () .push (false);
	getBrowser () -> touch (x, getBrowser () -> get_height () - y);
	getBrowser () -> getToolsPickable () .pop ();

	if (getBrowser () -> getHits () .empty ())
		return false;

	if (getGrid (getBrowser () -> getNearestHit ()))
		return true;

	if (getBrowser () -> getNearestHit () -> getLayer () not_eq getBrowser () -> getActiveLayer ())
		return false;

	return true;
}

void
X3DSnapTool::update () 
{
	try
	{
		const auto hit              = getBrowser () -> getNearestHit ();
		const auto pickingMatrix    = getPickingMatrix ();
		const auto invPickingMatrix = inverse (pickingMatrix);
		const auto hitPoint         = hit -> getIntersection () -> getPoint () * invPickingMatrix;
		const auto hitNormal        = normalize (pickingMatrix .mult_matrix_dir (hit -> getIntersection () -> getFaceNormal ()));

		if (getBrowser () -> getShiftKey ())
		{
			position () = snapToVerticesAndCenters (hitPoint, hit, invPickingMatrix);
		}
		else
		{
			const auto gridNode = getGrid (hit);

			if (gridNode)
				position () = gridNode -> getSnapPosition (hitPoint * getModelMatrix ()) * inverse (getModelMatrix ());
			else
				position () = hitPoint;
		}

		normal () = hitNormal;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

Vector3d
X3DSnapTool::snapToVerticesAndCenters (const Vector3d & hitPoint, const HitPtr & hit, const Matrix4d & invPickingMatrix) const
{
	const auto & modelViewMatrix = hit -> getModelViewMatrix ();
	const auto & face            = hit -> getIntersection () -> getFace ();
	auto         points          = std::vector <Vector3d> ();
	
	// Vertices

	for (const auto & vertex : face)
		points .emplace_back (vertex);

	// Center of edges

	for (size_t i = 0, size = face .size (); i < size; ++ i)
		points .emplace_back ((face [i] + face [(i + 1) % size]) / 2.0);

	// Center of face

	points .emplace_back (std::accumulate (face .begin (), face .end (), Vector3d ()) / double (face .size ()));

	// Transform points to model space.

	for (auto & point : points)
		point = point * modelViewMatrix * invPickingMatrix;

	// Find nearest snapping point to hit point.

	const auto snapPoint = std::min_element (points .begin (), points .end (),
	[&hitPoint] (const Vector3d & lhs, const Vector3d & rhs)
	{
		return abs (hitPoint - lhs) < abs (hitPoint - rhs);
	});

	return *snapPoint;
}

X3DPtr <X3DGridTool>
X3DSnapTool::getGrid (const HitPtr & hit) const
{
	for (const auto & node : hit -> getHierarchy ())
	{
		const auto gridNode = X3DPtr <X3DGridTool> (node);

		if (gridNode)
			return gridNode;
	}

	return nullptr;
}

void
X3DSnapTool::dispose ()
{
	notify_callbacks ();

	X3DActiveLayerTool::dispose ();
}

X3DSnapTool::~X3DSnapTool ()
{ }

} // X3D
} // titania
