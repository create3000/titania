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

namespace titania {
namespace puck {

NavigationInfoEditor::NavigationInfoEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DNavigationInfoEditorInterface (get_ui ("Editors/NavigationInfoEditor.xml"), gconf_dir ()),
	                            type (new MFStringWidget (browserWindow,
	                                  getTypeTreeView (),
	                                  getTypeCellRendererText (),
	                                  getTypeAddButton (),
	                                  getTypeRemoveButton (),
	                                  "type",
	                                  "EXAMINE")),
	                      avatarSize (getBrowserWindow (),
	                                  getCollisionRadiusAdjustment (),
	                                  getHeightAdjustment (),
	                                  getStepHeightAdjustment (),
	                                  getAvatarSizeBox (),
	                                  "avatarSize"),
	                           speed (browserWindow, getSpeedAdjustment (), getSpeedSpinButton (), "speed"),
	                       headlight (browserWindow, getHeadlightCheckButton (),  "headlight"),
	                 visibilityLimit (browserWindow, getVisibilityLimitAdjustment (), getVisibilityLimitSpinButton (), "visibilityLimit"),
	                  transitionType (new MFStringWidget (browserWindow,
	                                  getTransitionTypeTreeView (),
	                                  getTransitionTypeCellRendererText (),
	                                  getTransitionTypeAddButton (),
	                                  getTransitionTypeRemoveButton (),
	                                  "transitionType",
	                                  "LINEAR")),
	                  transitionTime (browserWindow, getTransitionTimeAdjustment (), getTransitionTimeSpinButton (), "transitionTime")
{
	setup ();
}

void
NavigationInfoEditor::initialize ()
{
	X3DNavigationInfoEditorInterface::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &NavigationInfoEditor::set_selection);

	set_selection ();
}

void
NavigationInfoEditor::set_selection ()
{
	const auto nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::NavigationInfo });

	type ->           setNodes (nodes);
	avatarSize       .setNodes (nodes);
	speed            .setNodes (nodes);
	headlight        .setNodes (nodes);
	visibilityLimit  .setNodes (nodes);
	transitionType -> setNodes (nodes);
	transitionTime   .setNodes (nodes);
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
