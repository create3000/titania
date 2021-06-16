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

#include "NavigationInfoEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../ComposedWidgets/MFStringWidget.h"

namespace titania {
namespace puck {

NavigationInfoEditor::NavigationInfoEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DNavigationInfoEditorInterface (get_ui ("Editors/NavigationInfoEditor.glade")),
	              navigationInfoList (new NavigationInfoList (browserWindow)),
	                        nodeName (this, getNameEntry (), getRenameButton ()),
	                            type (new MFStringWidget (this,
	                                  getTypeTreeView (),
	                                  getTypeCellRendererCombo (),
	                                  getTypeAddButton (),
	                                  getTypeRemoveButton (),
	                                  "type",
	                                  "EXAMINE")),
	                     avatarSize0 (this, getCollisionRadiusAdjustment (), getAvatarSizeSpinButton0 (), "avatarSize"),
	                     avatarSize1 (this, getHeightAdjustment (), getAvatarSizeSpinButton1 (), "avatarSize"),
	                     avatarSize2 (this, getStepHeightAdjustment (), getAvatarSizeSpinButton2 (), "avatarSize"),
	                           speed (this, getSpeedAdjustment (), getSpeedSpinButton (), "speed"),
	                       headlight (this, getHeadlightCheckButton (),  "headlight"),
	                 visibilityLimit (this, getVisibilityLimitAdjustment (), getVisibilityLimitSpinButton (), "visibilityLimit"),
	                  transitionType (new MFStringWidget (this,
	                                  getTransitionTypeTreeView (),
	                                  getTransitionTypeCellRendererCombo (),
	                                  getTransitionTypeAddButton (),
	                                  getTransitionTypeRemoveButton (),
	                                  "transitionType",
	                                  "LINEAR")),
	                  transitionTime (this, getTransitionTimeAdjustment (), getTransitionTimeSpinButton (), "transitionTime"),
                 navigationInfoNode ()
{
	addChildObjects (navigationInfoNode);

	setup ();
}

void
NavigationInfoEditor::initialize ()
{
	X3DNavigationInfoEditorInterface::initialize ();

	navigationInfoList -> getSelection () .addInterest (&NavigationInfoEditor::set_navigationInfo, this);

	navigationInfoList -> setEditing (true);
	navigationInfoList -> getHeaderNotebook () .set_current_page (1);
	navigationInfoList -> getHeaderBar () .set_visible (false);
	navigationInfoList -> getTreeView () .set_headers_visible (true);
	navigationInfoList -> reparent (getNavigationInfoListBox (), getWindow ());

	avatarSize0 .setIndex (0);
	avatarSize1 .setIndex (1);
	avatarSize2 .setIndex (2);

	set_navigationInfo (nullptr);
}

void
NavigationInfoEditor::configure ()
{
	X3DNavigationInfoEditorInterface::configure ();

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));
}

void
NavigationInfoEditor::set_navigationInfo (const X3D::X3DPtr <X3D::NavigationInfo> & value)
{
	navigationInfoNode = value;

	const bool inScene = (navigationInfoNode and navigationInfoNode -> getScene () == getCurrentScene () and not inPrototypeInstance ());
	const auto nodes   = navigationInfoNode ? X3D::MFNode ({ navigationInfoNode }) : X3D::MFNode ();

	getRemoveNavigationInfoButton () .set_sensitive (inScene and navigationInfoNode);
	getNavigationInfoBox ()          .set_sensitive (inScene);

	nodeName .setNode  (navigationInfoNode);
	type ->           setNodes (nodes);
	avatarSize0      .setNodes (nodes);
	avatarSize1      .setNodes (nodes);
	avatarSize2      .setNodes (nodes);
	speed            .setNodes (nodes);
	headlight        .setNodes (nodes);
	visibilityLimit  .setNodes (nodes);
	transitionType -> setNodes (nodes);
	transitionTime   .setNodes (nodes);
}

void
NavigationInfoEditor::on_new_navigation_info_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Navigation"));
	const auto node     = X3D::X3DPtr <X3D::NavigationInfo> (X3D::X3DEditor::createNode (getCurrentWorld (), getCurrentContext (), "NavigationInfo", undoStep));

	node -> set_bind () = true;

	getBrowserWindow () -> getSelection () -> setNodes ({ node }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	navigationInfoList -> setSelection (node, true);
}

void
NavigationInfoEditor::on_remove_navigation_info_clicked ()
{
	const auto   undoStep           = std::make_shared <X3D::UndoStep> (_ ("Remove NavigationInfo"));
	const auto & navigationInfoNode = nodeName .getNode ();
	const auto   executionContext   = X3D::X3DExecutionContextPtr (navigationInfoNode -> getExecutionContext ());

	X3D::X3DEditor::removeNodesFromScene (executionContext, { navigationInfoNode }, true, undoStep);

	getBrowserWindow () -> getSelection () -> removeNodes ({ navigationInfoNode }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	set_navigationInfo (nullptr);
}

void
NavigationInfoEditor::store ()
{
	getConfig () -> setItem ("paned", getPaned () .get_position ());

	X3DNavigationInfoEditorInterface::store ();
}

NavigationInfoEditor::~NavigationInfoEditor ()
{
	dispose ();
}

} // puck
} // titania
