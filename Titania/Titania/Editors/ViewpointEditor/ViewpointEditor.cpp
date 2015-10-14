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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Widgets/ViewpointList/ViewpointList.h"

#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>

namespace titania {
namespace puck {

ViewpointEditor::ViewpointEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DViewpointEditorInterface (get_ui ("Editors/ViewpointEditor.xml"), gconf_dir ()),
	         X3DViewpointEditor (),
	    X3DOrthoViewpointEditor (),
	      X3DGeoViewpointEditor (),
	                    browser (getBrowser ()),
	              viewpointList (new ViewpointList (browserWindow)),
	                   nodeName (this, getViewpointNameEntry (), getViewpointRenameButton ()),
	                description (this, getViewpointDescriptionTextView (), "description"),
	                       jump (this, getViewpointJumpCheckButton (), "jump"),
	          retainUserOffsets (this, getViewpointRetainUserOffsetsCheckButton (), "retainUserOffsets"),
	              viewpointNode ()
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

	getBrowser () .addInterest (this, &ViewpointEditor::set_browser);

	set_browser (getBrowser ());
}

void
ViewpointEditor::set_browser (const X3D::BrowserPtr & value)
{
	browser -> getActiveViewpointEvent () .removeInterest (this, &ViewpointEditor::set_active_viewpoint);

	browser = value;

	browser -> getActiveViewpointEvent () .addInterest (this, &ViewpointEditor::set_active_viewpoint);

	// We shouldn't store the viewpoint in X3DPtr better would be a X3DWeakPtr inbeteen we use:

	set_active_viewpoint ();
}

void
ViewpointEditor::set_active_viewpoint ()
{
	const auto activeLayer = getBrowser () -> getActiveLayer ();

	if (activeLayer and activeLayer -> getViewpointStack () -> bottom () not_eq activeLayer -> getViewpoint ())
		set_viewpoint (activeLayer -> getViewpointStack () -> top ());

	else
		set_viewpoint (nullptr);
}

void
ViewpointEditor::set_viewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & value)
{
	if (viewpointNode)
		viewpointNode -> isLockedToCamera () .removeInterest (this, &ViewpointEditor::set_lock_to_camera);

	viewpointNode = value;

	if (viewpointNode)
		viewpointNode -> isLockedToCamera () .addInterest (this, &ViewpointEditor::set_lock_to_camera);

	const bool inScene = (viewpointNode and viewpointNode -> getExecutionContext () == getExecutionContext () and not inPrototypeInstance ());

	setViewpoint (viewpointNode, inScene);
	setOrthoViewpoint (viewpointNode, inScene);
	setGeoViewpoint (viewpointNode, inScene);

	const auto viewpointNodes = viewpointNode ? X3D::MFNode ({ viewpointNode }) : X3D::MFNode ();

	getViewpointActionBox () .set_sensitive (inScene);
	getViewpointBox ()       .set_sensitive (inScene);
	getLockToCameraButton () .set_active (viewpointNode ? viewpointNode -> isLockedToCamera () : false);

	nodeName          .setNode  (X3D::SFNode (viewpointNode));
	description       .setNodes (viewpointNodes);
	retainUserOffsets .setNodes (viewpointNodes);
	jump              .setNodes (viewpointNodes);
}

void
ViewpointEditor::on_remove_viewpoint_clicked ()
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove %s"), viewpointNode-> getTypeName () .c_str ()));

	getBrowserWindow () -> removeNodesFromScene (getExecutionContext (), { viewpointNode }, true, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointEditor::on_update_viewpoint_clicked ()
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Update Active Viewpoint"));

	update (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointEditor::on_lock_to_camera_toggled ()
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	if (getLockToCameraButton () .get_active ())
		getLockToCameraImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	else
		getLockToCameraImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (viewpointNode -> isLockedToCamera () == getLockToCameraButton () .get_active ())
		return;

	// Do it.

	using isLockedToCamera = void (X3D::X3DViewpointNode::*) (const bool);

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Lock Viewpoint To Camera"));

	undoStep -> addObjects (viewpointNode);
	undoStep -> addUndoFunction ((isLockedToCamera) &X3D::X3DViewpointNode::isLockedToCamera, viewpointNode, viewpointNode -> isLockedToCamera ());
	undoStep -> addRedoFunction ((isLockedToCamera) &X3D::X3DViewpointNode::isLockedToCamera, viewpointNode, getLockToCameraButton () .get_active ());
	viewpointNode -> isLockedToCamera (getLockToCameraButton () .get_active ());

	if (getLockToCameraButton () .get_active ())
		update (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointEditor::set_lock_to_camera ()
{
	getLockToCameraButton () .set_active (viewpointNode -> isLockedToCamera ());
}

void
ViewpointEditor::update (const X3D::UndoStepPtr & undoStep)
{
	undoStep -> addObjects (viewpointNode);
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::transitionStart,     viewpointNode, viewpointNode);
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::resetUserOffsets,    viewpointNode);
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setCenterOfRotation, viewpointNode, viewpointNode -> getCenterOfRotation ());
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setOrientation,      viewpointNode, viewpointNode -> getOrientation ());
	undoStep -> addUndoFunction (&X3D::X3DViewpointNode::setPosition,         viewpointNode, viewpointNode -> getPosition ());
	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (viewpointNode -> set_bind ()), true);

	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (viewpointNode -> set_bind ()), true);
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setPosition,         viewpointNode, viewpointNode -> getUserPosition ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setOrientation,      viewpointNode, viewpointNode -> getUserOrientation ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::setCenterOfRotation, viewpointNode, viewpointNode -> getUserCenterOfRotation ());
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::resetUserOffsets,    viewpointNode);
	undoStep -> addRedoFunction (&X3D::X3DViewpointNode::transitionStart,     viewpointNode, viewpointNode);

	viewpointNode -> setPosition (viewpointNode -> getUserPosition ());
	viewpointNode -> setOrientation (viewpointNode -> getUserOrientation ());
	viewpointNode -> setCenterOfRotation (viewpointNode -> getUserCenterOfRotation ());
	viewpointNode -> resetUserOffsets ();
}

ViewpointEditor::~ViewpointEditor ()
{
	dispose ();
}

} // puck
} // titania
