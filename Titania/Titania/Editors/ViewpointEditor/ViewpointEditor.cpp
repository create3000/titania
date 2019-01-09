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

#include "ViewpointEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>

namespace titania {
namespace puck {

ViewpointEditor::ViewpointEditor (X3DBrowserWindow* const browserWindow) :
	           X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DViewpointEditorInterface (get_ui ("Editors/ViewpointEditor.glade")),
	         X3DViewpointEditor (),
	    X3DOrthoViewpointEditor (),
	      X3DGeoViewpointEditor (),
	                    browser (getCurrentBrowser ()),
	              viewpointList (new ViewpointList (browserWindow)),
	                   nodeName (this, getViewpointNameEntry (), getViewpointRenameButton ()),
	                description (this, getViewpointDescriptionTextView (), "description"),
	                       jump (this, getViewpointJumpCheckButton (), "jump"),
	          retainUserOffsets (this, getViewpointRetainUserOffsetsCheckButton (), "retainUserOffsets"),
	              viewpointNode ()
{
	addChildObjects (browser, viewpointNode);

	setup ();
}

void
ViewpointEditor::initialize ()
{
	X3DViewpointEditorInterface::initialize ();
	X3DViewpointEditor::initialize ();
	X3DOrthoViewpointEditor::initialize ();
	X3DGeoViewpointEditor::initialize ();

	viewpointList -> getSelection () .addInterest (&ViewpointEditor::set_viewpoint, this);

	viewpointList -> setEditing (true);
	viewpointList -> getHeaderNotebook () .set_current_page (1);
	viewpointList -> getHeaderBar () .set_visible (false);
	viewpointList -> getTreeView () .set_headers_visible (true);
	viewpointList -> reparent (getViewpointListBox (), getWindow ());

	set_viewpoint (nullptr);
}

void
ViewpointEditor::configure ()
{
	X3DViewpointEditorInterface::configure ();
	X3DViewpointEditor::configure ();
	X3DOrthoViewpointEditor::configure ();
	X3DGeoViewpointEditor::configure ();

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));
}

void
ViewpointEditor::set_viewpoint (const X3D::X3DPtr <X3D::X3DViewpointNode> & value)
{
	if (viewpointNode)
		viewpointNode -> isLockedToCamera () .removeInterest (&ViewpointEditor::set_lock_to_camera, this);

	viewpointNode = value;

	if (viewpointNode)
		viewpointNode -> isLockedToCamera () .addInterest (&ViewpointEditor::set_lock_to_camera, this);

	const bool inScene = (viewpointNode and viewpointNode -> getScene () == getCurrentScene () and not inPrototypeInstance ());

	setViewpoint (viewpointNode, inScene);
	setOrthoViewpoint (viewpointNode, inScene);
	setGeoViewpoint (viewpointNode, inScene);

	const auto viewpointNodes = viewpointNode ? X3D::MFNode ({ viewpointNode }) : X3D::MFNode ();

	getViewpointActionBox ()    .set_sensitive (not inPrototypeInstance ());
	getRemoveViewpointButton () .set_sensitive (inScene);
	getViewpointBox ()          .set_sensitive (inScene);
	getLockToCameraButton ()    .set_active (viewpointNode ? viewpointNode -> isLockedToCamera () : false);

	nodeName          .setNode  (viewpointNode);
	description       .setNodes (viewpointNodes);
	retainUserOffsets .setNodes (viewpointNodes);
	jump              .setNodes (viewpointNodes);
}

void
ViewpointEditor::on_new_viewpoint_popup_clicked ()
{
	getNewViewpointPopover () .popup ();
}

void
ViewpointEditor::on_remove_viewpoint_clicked ()
{
	if (not getCurrentBrowser () -> getActiveLayer ())
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove %s"), viewpointNode-> getTypeName () .c_str ()));
	const auto executionContext = X3D::X3DExecutionContextPtr (viewpointNode -> getExecutionContext ());

	X3D::X3DEditor::removeNodesFromScene (executionContext, { viewpointNode }, true, undoStep);

	getBrowserWindow () -> getSelection () -> removeNodes ({ viewpointNode }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	set_viewpoint (nullptr);
}

void
ViewpointEditor::on_update_viewpoint_clicked ()
{
	if (not getCurrentBrowser () -> getActiveLayer ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Update Active Viewpoint"));

	X3D::X3DEditor::updateViewpoint (viewpointNode, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointEditor::on_lock_to_camera_toggled ()
{
	if (not viewpointNode)
		return;

	if (not getCurrentBrowser () -> getActiveLayer ())
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
		X3D::X3DEditor::updateViewpoint (viewpointNode, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ViewpointEditor::set_lock_to_camera ()
{
	getLockToCameraButton () .set_active (viewpointNode -> isLockedToCamera ());
}

void
ViewpointEditor::store ()
{
	getConfig () -> setItem ("paned", getPaned () .get_position ());

	X3DViewpointEditorInterface::store ();
}

ViewpointEditor::~ViewpointEditor ()
{
	dispose ();
}

} // puck
} // titania
