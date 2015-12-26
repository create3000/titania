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

#include "X3DViewpointEditor.h"

#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

X3DViewpointEditor::X3DViewpointEditor () :
	X3DViewpointEditorInterface (),
	                   position (this,
	                             getPerspectiveViewpointPositionXAdjustment (),
	                             getPerspectiveViewpointPositionYAdjustment (),
	                             getPerspectiveViewpointPositionZAdjustment (),
	                             getPerspectiveViewpointPositionBox (),
	                             "position"),
	                orientation (this,
	                             getPerspectiveViewpointOrientationXAdjustment (),
	                             getPerspectiveViewpointOrientationYAdjustment (),
	                             getPerspectiveViewpointOrientationZAdjustment (),
	                             getPerspectiveViewpointOrientationAAdjustment (),
	                             getPerspectiveViewpointOrientationBox (),
	                             "orientation"),
	            orientationTool (new RotationTool (this, getPerspectiveViewpointOrientationToolBox (), "orientation")),
	           centerOfRotation (this,
	                             getPerspectiveViewpointCenterOfRotationXAdjustment (),
	                             getPerspectiveViewpointCenterOfRotationYAdjustment (),
	                             getPerspectiveViewpointCenterOfRotationZAdjustment (),
	                             getPerspectiveViewpointCenterOfRotationBox (),
	                             "centerOfRotation"),
	                fieldOfView (this,
	                             getPerspectiveViewpointFieldOfViewAdjustment (),
	                             getPerspectiveViewpointFieldOfViewBox (),
	                             "fieldOfView")
{
	getPerspectiveViewpointFieldOfViewAdjustment () -> set_upper (M_PI); // getCurrentContext () -> fromRadiant (M_PI);
}

void
X3DViewpointEditor::setViewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & viewpointNode, const bool inScene)
{
	const X3D::X3DPtr <X3D::Viewpoint> viewpoint (viewpointNode);

	getPerspectiveViewpointBox () .set_sensitive (inScene);
	getPerspectiveViewpointExpander () .set_visible (viewpoint);

	const auto viewpoints = viewpoint ? X3D::MFNode ({ viewpoint }) : X3D::MFNode ();

	position         .setNodes (viewpoints);
	orientation      .setNodes (viewpoints);
	centerOfRotation .setNodes (viewpoints);
	fieldOfView      .setNodes (viewpoints);

	orientationTool -> setNodes (viewpoints);
}

void
X3DViewpointEditor::on_new_viewpoint_activated ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Viewpoint"));
	const X3D::X3DPtr <X3D::Viewpoint> node (getBrowserWindow () -> createNode ("Viewpoint", undoStep));
	node -> set_bind () = true;

	try
	{
		const auto & activeViewpoint   = getCurrentWorld () -> getActiveLayer () -> getViewpoint ();
		const auto & cameraSpaceMatrix = activeViewpoint -> getCameraSpaceMatrix ();

		X3D::Vector3f   position;
		X3D::Rotation4f orientation;

		cameraSpaceMatrix .get (position, orientation);
		node -> setPosition (position);
		node -> setOrientation (orientation);
		node -> setCenterOfRotation (activeViewpoint -> getUserCenterOfRotation () * activeViewpoint -> getTransformationMatrix ());

		const X3D::X3DPtr <X3D::Viewpoint> viewpoint (activeViewpoint);

		if (viewpoint)
			node -> fieldOfView () = viewpoint -> fieldOfView ();
	}
	catch (const X3D::X3DError &)
	{ }	

	getBrowserWindow () -> addUndoStep (undoStep);
}

X3DViewpointEditor::~X3DViewpointEditor ()
{ }

} // puck
} // titania
