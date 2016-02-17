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

#include "SoundEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"
#include "../../ComposedWidgets/NormalTool.h"

#include <Titania/X3D/Components/Sound/Sound.h>

namespace titania {
namespace puck {

SoundEditor::SoundEditor (X3DBrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	 X3DSoundEditorInterface (get_ui ("Editors/SoundEditor.glade"), gconf_dir ()),
	X3DSoundSourceNodeEditor (),
	                nodeName (this, getSoundNameEntry (), getSoundRenameButton ()),
	               intensity (this, getSoundIntensityAdjustment (), getSoundIntensityBox (), "intensity"),
	              spatialize (this, getSoundSpatializeCheckButton (),  "spatialize"),
	                location (this,
	                          getSoundLocationXAdjustment (),
	                          getSoundLocationYAdjustment (),
	                          getSoundLocationZAdjustment (),
	                          getSoundLocationBox (),
	                          "location"),
	               direction (this,
	                          getSoundDirectionXAdjustment (),
	                          getSoundDirectionYAdjustment (),
	                          getSoundDirectionZAdjustment (),
	                          getSoundDirectionBox (),
	                          "direction"),
	           directionTool (new NormalTool (this, getSoundNormalToolBox (), "direction")),
	                 minBack (this, getSoundMinBackAdjustment (),  getSoundMinBackSpinButton (),  "minBack"),
	                minFront (this, getSoundMinFrontAdjustment (), getSoundMinFrontSpinButton (), "minFront"),
	                 maxBack (this, getSoundMaxBackAdjustment (),  getSoundMaxBackSpinButton (),  "maxBack"),
	                maxFront (this, getSoundMaxFrontAdjustment (), getSoundMaxFrontSpinButton (), "maxFront"),
	                priority (this, getSoundPriorityAdjustment (), getSoundPriorityBox (),        "priority")
{
	setup ();
}

void
SoundEditor::configure ()
{
	X3DSoundEditorInterface::configure ();

	getSoundNotebook () .set_current_page (getConfig () -> getInteger ("currentPage"));
}

void
SoundEditor::initialize ()
{
	X3DSoundEditorInterface::initialize ();
	X3DSoundSourceNodeEditor::initialize ();

	direction .setNormalize (true);
}

void
SoundEditor::set_selection (const X3D::MFNode & selection)
{
	X3DSoundEditorInterface::set_selection (selection);
	X3DSoundSourceNodeEditor::set_selection (selection);

	const X3D::X3DPtr <X3D::Sound> soundNode (selection .empty () ? nullptr : selection .back ());
	const auto nodes = soundNode ? X3D::MFNode ({ soundNode }) : X3D::MFNode ();

	getSoundBox () .set_sensitive (soundNode);

	nodeName   .setNode (X3D::SFNode (soundNode));
	intensity  .setNodes (nodes);
	spatialize .setNodes (nodes);
	location   .setNodes (nodes);
	direction  .setNodes (nodes);
	directionTool -> setNodes (nodes);
	minBack    .setNodes (nodes);
	minFront   .setNodes (nodes);
	maxBack    .setNodes (nodes);
	maxFront   .setNodes (nodes);
	priority   .setNodes (nodes);

	getRemoveSoundButton () .set_sensitive (soundNode);
}

void
SoundEditor::on_new_sound_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Sound"));
	getBrowserWindow () -> createNode ("Sound", undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
SoundEditor::on_remove_sound_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Sound"));

	getBrowserWindow () -> removeNodesFromScene (getCurrentContext (), { nodeName .getNode () }, true, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
SoundEditor::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex"));
	nodeIndex -> setTypes ({ X3D::X3DConstants::Sound });
}

void
SoundEditor::store ()
{
	getConfig () -> setItem ("currentPage", getSoundNotebook () .get_current_page ());

	X3DSoundEditorInterface::store ();
}

SoundEditor::~SoundEditor ()
{
	dispose ();
}

} // puck
} // titania
