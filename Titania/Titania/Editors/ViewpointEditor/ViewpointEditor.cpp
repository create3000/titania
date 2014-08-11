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

#include "ViewpointEditor.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Widgets/ViewpointList/ViewpointList.h"

namespace titania {
namespace puck {

ViewpointEditor::ViewpointEditor (BrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DViewpointEditorInterface (get_ui ("Dialogs/ViewpointEditor.xml"), gconf_dir ()),
	         X3DViewpointEditor (),
	    X3DOrthoViewpointEditor (),
	      X3DGeoViewpointEditor (),
	              viewpointList (new ViewpointList (browserWindow)),
	                   nodeName (getBrowserWindow (), getViewpointNameEntry (), getViewpointRenameButton ()),
	                description (getBrowserWindow (), getViewpointDescriptionTextView (), "description"),
	                       jump (getBrowserWindow (), getViewpointJumpCheckButton (), "jump"),
	          retainUserOffsets (getBrowserWindow (), getViewpointRetainUserOffsetsCheckButton (), "retainUserOffsets")
{
	setup ();

	viewpointList -> setUserViewpoints (false);
	viewpointList -> getTreeView () .set_headers_visible (true);
	viewpointList -> reparent (getViewpointListBox (), getWindow ());

	// In Scene
}

void
ViewpointEditor::initialize ()
{
	X3DViewpointEditorInterface::initialize ();
	X3DViewpointEditor::initialize ();
	X3DOrthoViewpointEditor::initialize ();
	X3DGeoViewpointEditor::initialize ();

	getBrowser () -> getActiveViewpointEvent () .addInterest (this, &ViewpointEditor::set_active_viewpoint);

	set_active_viewpoint ();
}

void
ViewpointEditor::set_active_viewpoint ()
{
	const auto activeLayer = getBrowser () -> getActiveLayer ();

	bool haveViewpoint = false;
	bool inScene       = false;

	if (activeLayer)
	{
		haveViewpoint = (activeLayer -> getViewpointStack () -> bottom () not_eq activeLayer -> getViewpoint ());
		inScene       = (activeLayer -> getViewpointStack () -> top () -> getExecutionContext () == getExecutionContext () and not inPrototypeInstance ());
	}

	const X3D::X3DPtr <X3D::X3DViewpointNode> viewpointNode (haveViewpoint ? activeLayer -> getViewpointStack () -> top () : nullptr);

	setViewpoint (viewpointNode);
	setOrthoViewpoint (viewpointNode);
	setGeoViewpoint (viewpointNode);

	const auto viewpointNodes = haveViewpoint ? X3D::MFNode ({ activeLayer -> getViewpointStack () -> top () }) : X3D::MFNode ();

	getViewpointBox () .set_sensitive (haveViewpoint and inScene);

	nodeName          .setNode  (X3D::SFNode (viewpointNode));
	description       .setNodes (viewpointNodes);
	retainUserOffsets .setNodes (viewpointNodes);
	jump              .setNodes (viewpointNodes);
}

void
ViewpointEditor::on_update_viewpoint_clicked ()
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	const X3D::X3DViewpointNodePtr viewpoint = getBrowser () -> getActiveLayer () -> getViewpoint ();

	const auto undoStep = std::make_shared <UndoStep> (_ ("Update Active Viewpoint"));

	undoStep -> addVariables (viewpoint);
	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (viewpoint -> set_bind ()), true);
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::transitionStart, viewpoint, viewpoint);

	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setPosition, viewpoint, viewpoint -> getPosition ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setPosition, viewpoint, viewpoint -> getUserPosition ());
	viewpoint -> setPosition (viewpoint -> getUserPosition ());

	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setOrientation, viewpoint, viewpoint -> getOrientation ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setOrientation, viewpoint, viewpoint -> getUserOrientation ());
	viewpoint -> setOrientation (viewpoint -> getUserOrientation ());

	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setCenterOfRotation, viewpoint, viewpoint -> getCenterOfRotation ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setCenterOfRotation, viewpoint, viewpoint -> getUserCenterOfRotation ());
	viewpoint -> setCenterOfRotation (viewpoint -> getUserCenterOfRotation ());

	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::resetUserOffsets, viewpoint);
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::resetUserOffsets, viewpoint);
	viewpoint -> resetUserOffsets ();

	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::transitionStart, viewpoint, viewpoint);
	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (viewpoint -> set_bind ()), true);

	getBrowserWindow () -> addUndoStep (undoStep);
}

ViewpointEditor::~ViewpointEditor ()
{
	dispose ();
}

} // puck
} // titania
