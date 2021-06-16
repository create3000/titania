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

#include "X3DOrthoViewpointEditor.h"

#include "../../Browser/BrowserSelection.h"

#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

X3DOrthoViewpointEditor::X3DOrthoViewpointEditor () :
	X3DViewpointEditorInterface (),
	                   position (this,
	                             getOrthoViewpointPositionXAdjustment (),
	                             getOrthoViewpointPositionYAdjustment (),
	                             getOrthoViewpointPositionZAdjustment (),
	                             getOrthoViewpointPositionBox (),
	                             "position"),
	            orientationTool (this, getOthoViewpointOrientationToolBox (), "orientation"),
	                orientation (this,
	                             getOrthoViewpointOrientationXAdjustment (),
	                             getOrthoViewpointOrientationYAdjustment (),
	                             getOrthoViewpointOrientationZAdjustment (),
	                             getOrthoViewpointOrientationAAdjustment (),
	                             getOrthoViewpointOrientationBox (),
	                             "orientation"),
	           orientationEuler (this,
	                             getOrthoViewpointOrientationEulerXAdjustment (),
	                             getOrthoViewpointOrientationEulerYAdjustment (),
	                             getOrthoViewpointOrientationEulerZAdjustment (),
	                             getOrthoViewpointOrientationEulerBox (),
	                             "orientation"),
	           centerOfRotation (this,
	                             getOrthoViewpointCenterOfRotationXAdjustment (),
	                             getOrthoViewpointCenterOfRotationYAdjustment (),
	                             getOrthoViewpointCenterOfRotationZAdjustment (),
	                             getOrthoViewpointCenterOfRotationBox (),
	                             "centerOfRotation"),
	               fieldOfView0 (this, getOrthoViewpointFieldOfViewMinXAdjustment (), getOrthoViewpointFieldOfViewSpinButton0 (), "fieldOfView"),
	               fieldOfView1 (this, getOrthoViewpointFieldOfViewMinYAdjustment (), getOrthoViewpointFieldOfViewSpinButton1 (), "fieldOfView"),
	               fieldOfView2 (this, getOrthoViewpointFieldOfViewMaxXAdjustment (), getOrthoViewpointFieldOfViewSpinButton2 (), "fieldOfView"),
	               fieldOfView3 (this, getOrthoViewpointFieldOfViewMaxYAdjustment (), getOrthoViewpointFieldOfViewSpinButton3 (), "fieldOfView")
{
	fieldOfView0 .setIndex (0);
	fieldOfView1 .setIndex (1);
	fieldOfView2 .setIndex (2);
	fieldOfView3 .setIndex (3);
}

void
X3DOrthoViewpointEditor::configure ()
{
	switch (getConfig () -> getItem <int32_t> ("orthoViewpointOrientationType"))
	{
		case 1:
			getOrthoViewpointOrientationEulerButton () .set_active (true);
			break;
		default:
			getOrthoViewpointOrientationAxisAngleButton () .set_active (true);
			break;
	}
}

void
X3DOrthoViewpointEditor::setOrthoViewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & viewpointNode, const bool inScene)
{
	const X3D::X3DPtr <X3D::OrthoViewpoint> orthoViewpoint (viewpointNode);

	if (orthoViewpoint and orthoViewpoint -> fieldOfView () .size () < 4)
	{
		orthoViewpoint -> fieldOfView () = {
			orthoViewpoint -> getMinimumX (),
			orthoViewpoint -> getMinimumY (),
			orthoViewpoint -> getMaximumX (),
			orthoViewpoint -> getMaximumY ()
		};
	}

	getOrthoViewpointBox ()      .set_sensitive (inScene);
	getOrthoViewpointExpander () .set_visible (orthoViewpoint);

	const auto orthoViewpoints = orthoViewpoint ? X3D::MFNode ({ orthoViewpoint }) : X3D::MFNode ();

	position         .setNodes (orthoViewpoints);
	orientationTool  .setNodes (orthoViewpoints);
	orientation      .setNodes (orthoViewpoints);
	orientationEuler .setNodes (orthoViewpoints);
	centerOfRotation .setNodes (orthoViewpoints);
	fieldOfView0     .setNodes (orthoViewpoints);
	fieldOfView1     .setNodes (orthoViewpoints);
	fieldOfView2     .setNodes (orthoViewpoints);
	fieldOfView3     .setNodes (orthoViewpoints);

}

void
X3DOrthoViewpointEditor::on_new_ortho_viewpoint_clicked ()
{
	getNewViewpointPopover () .popdown ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New OrthoViewpoint"));
	const auto node     = X3D::X3DPtr <X3D::OrthoViewpoint> (X3D::X3DEditor::createNode (getCurrentWorld (), getCurrentContext (), "OrthoViewpoint", undoStep));

	node -> set_bind () = true;

	try
	{
		const auto & activeViewpoint   = getCurrentWorld () -> getActiveLayer () -> getViewpoint ();
		const auto & cameraSpaceMatrix = activeViewpoint -> getCameraSpaceMatrix ();

		X3D::Vector3f   position;
		X3D::Rotation4d orientation;

		cameraSpaceMatrix .get (position, orientation);
		node -> setPosition (position);
		node -> setOrientation (orientation);
		node -> setCenterOfRotation (activeViewpoint -> getUserCenterOfRotation () * activeViewpoint -> getModelMatrix ());

		const X3D::X3DPtr <X3D::OrthoViewpoint> orthoViewpoint (activeViewpoint);

		if (orthoViewpoint)
			node -> fieldOfView () = orthoViewpoint -> fieldOfView ();
	}
	catch (const X3D::X3DError &)
	{ }	

	getBrowserWindow () -> getSelection () -> setNodes ({ node }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	getViewpointList () -> setSelection (X3D::X3DPtr <X3D::X3DViewpointNode> (node), true);
}

void
X3DOrthoViewpointEditor::on_ortho_viewpoint_orientation_type_clicked ()
{
	getOrthoViewpointOrientationPopover () .popup ();
}

void
X3DOrthoViewpointEditor::on_ortho_viewpoint_orientation_axis_angle_toggled ()
{
	getOrthoViewpointOrientationPopover () .popdown ();

	if (not getOrthoViewpointOrientationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getOrthoViewpointOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getOrthoViewpointOrientationBox () .set_visible (true);
	getOrthoViewpointOrientationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("orthoViewpointOrientationType", 0);
}

void
X3DOrthoViewpointEditor::on_ortho_viewpoint_orientation_euler_toggled ()
{
	getOrthoViewpointOrientationPopover () .popdown ();

	if (not getOrthoViewpointOrientationEulerButton () .get_active ())
		return;

	for (const auto widget : getOrthoViewpointOrientationNotebook () .get_children ())
		widget -> set_visible (false);

	getOrthoViewpointOrientationEulerBox () .set_visible (true);
	getOrthoViewpointOrientationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("orthoViewpointOrientationType", 1);
}

X3DOrthoViewpointEditor::~X3DOrthoViewpointEditor ()
{ }

} // puck
} // titania
