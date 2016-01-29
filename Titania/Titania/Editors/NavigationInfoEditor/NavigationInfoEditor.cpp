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

#include "NavigationInfoEditor.h"

#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"
#include "../../ComposedWidgets/MFStringWidget.h"

#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

NavigationInfoEditor::NavigationInfoEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DNavigationInfoEditorInterface (get_ui ("Editors/NavigationInfoEditor.glade"), gconf_dir ()),
	                        nodeName (this, getNameEntry (), getRenameButton ()),
	                            type (new MFStringWidget (browserWindow,
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
                 navigationInfoNode (),
                           changing (false)
{
	avatarSize0 .setIndex (0);
	avatarSize1 .setIndex (1);
	avatarSize2 .setIndex (2);

	setup ();
}

void
NavigationInfoEditor::initialize ()
{
	X3DNavigationInfoEditorInterface::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &NavigationInfoEditor::set_selection);

	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
NavigationInfoEditor::set_selection (const X3D::MFNode & selection)
{
	if (navigationInfoNode)
		navigationInfoNode -> isBound () .removeInterest (this, &NavigationInfoEditor::set_bind);

	navigationInfoNode = selection .empty () ? nullptr : selection .back ();
	const auto nodes   = navigationInfoNode ? X3D::MFNode ({ navigationInfoNode }) : X3D::MFNode ();

	nodeName .setNode  (X3D::SFNode (navigationInfoNode));
	type ->           setNodes (nodes);
	avatarSize0      .setNodes (nodes);
	avatarSize1      .setNodes (nodes);
	avatarSize2      .setNodes (nodes);
	speed            .setNodes (nodes);
	headlight        .setNodes (nodes);
	visibilityLimit  .setNodes (nodes);
	transitionType -> setNodes (nodes);
	transitionTime   .setNodes (nodes);

	getRemoveNavigationInfoButton () .set_sensitive (navigationInfoNode);
	getBindToggleButton ()           .set_sensitive (navigationInfoNode);

	if (navigationInfoNode)
		navigationInfoNode -> isBound () .addInterest (this, &NavigationInfoEditor::set_bind);

	set_bind ();
}

void
NavigationInfoEditor::on_new_navigation_info_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Navigation"));
	const X3D::X3DPtr <X3D::X3DBindableNode> node (getBrowserWindow () -> createNode ("NavigationInfo", undoStep));
	node -> set_bind () = true;
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
NavigationInfoEditor::on_remove_navigation_info_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove NavigationInfo"));

	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { nodeName .getNode () }, true, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
NavigationInfoEditor::on_bind_toggled ()
{
	if (changing)
		return;

	if (navigationInfoNode)
		navigationInfoNode -> set_bind () = not navigationInfoNode -> isBound ();
}

void
NavigationInfoEditor::set_bind ()
{
	changing = true;

	if (navigationInfoNode)
	{
		getBindToggleButton () .set_active (navigationInfoNode -> isBound ());
		getBindImage () .set (Gtk::StockID (navigationInfoNode -> isBound () ? "Bound" : "Bind"), Gtk::IconSize (Gtk::ICON_SIZE_BUTTON));
	}
	else
	{
		getBindToggleButton () .set_active (false);
		getBindImage () .set (Gtk::StockID ("Bind"), Gtk::IconSize (Gtk::ICON_SIZE_BUTTON));
	}

	changing = false;
}

void
NavigationInfoEditor::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex"));

	nodeIndex -> setTypes ({ X3D::X3DConstants::NavigationInfo });
}

NavigationInfoEditor::~NavigationInfoEditor ()
{
	dispose ();
}

} // puck
} // titania
