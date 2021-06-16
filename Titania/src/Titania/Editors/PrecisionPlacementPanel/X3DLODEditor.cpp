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

#include "X3DLODEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Navigation/LOD.h>

namespace titania {
namespace puck {

X3DLODEditor::X3DLODEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                           nodeName (this, getLODNameEntry (), getLODRenameButton ()),
	                   forceTransitions (this, getLODForceTransitionsCheckButton (),  "forceTransitions"),
	                             center (this,
	                                     getLODCenterXAdjustment (),
	                                     getLODCenterYAdjustment (),
	                                     getLODCenterZAdjustment (),
	                                     getLODCenterBox (),
	                                     "center"),
		  	            			   range (this, "Range", getRangeGradientBox ()),
	                        singleRange (this, getLODRangeAdjustment (), getLODRangeSpinButton (), "range"),
	                      level_changed (this,
	                                     getLODLevelAdjustment (),
	                                     getLODLevelSpinButton (),
	                                     "level_changed"),
	                                lod ()
{
	addChildObjects (lod);	                                	                               
}

void
X3DLODEditor::initialize ()
{
	range .signal_index_changed () .connect (sigc::mem_fun (this, &X3DLODEditor::on_range_index_changed)); 
}

void
X3DLODEditor::set_selection (const X3D::MFNode & selection)
{
	lod = X3D::X3DPtr <X3D::LOD> (selection .empty () ? nullptr : selection .back ());

	const auto lods = lod ? X3D::MFNode ({ lod }) : X3D::MFNode ();

	nodeName         .setNode  (lod);
	forceTransitions .setNodes (lods);
	center           .setNodes (lods);
	range            .setNodes (lods);
	singleRange      .setNodes (lods);
	level_changed    .setNodes (lods);

	getLODExpander ()                    .set_visible (lod);
	getLODKeepCurrentLevelCheckButton () .set_sensitive (lod);
	getLODKeepCurrentLevelCheckButton () .set_inconsistent (not lod);

	if (lod)
	{
		range       .setIndex (lod -> range () .empty () ? -1 : 0);
		singleRange .setIndex (lod -> range () .empty () ? -1 : 0);
	}
}

void
X3DLODEditor::on_lod_move_center_button_clicked ()
{
	using setValue = void (X3D::SFVec3f::*) (const X3D::SFVec3f::internal_type &);

	if (getBrowserWindow () -> getSelection () -> getNodes () .empty ())
		return;

	const X3D::X3DPtr <X3D::LOD> lod (getBrowserWindow () -> getSelection () -> getNodes () .back ());

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Center Of LOD To BBox Center"));

	undoStep -> addObjects (lod);
	undoStep -> addUndoFunction ((setValue) &X3D::SFVec3f::setValue, std::ref (lod -> center ()), lod -> center ());
	
	lod -> center () = lod -> getBBox () .center ();

	undoStep -> addRedoFunction ((setValue) &X3D::SFVec3f::setValue, std::ref (lod -> center ()), lod -> center ());

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DLODEditor::on_range_index_changed ()
{
	singleRange .setIndex (range .getIndex ());
}

void
X3DLODEditor::on_lod_keep_current_level_toggled ()
{
	lod -> setKeepCurrentLevel (getLODKeepCurrentLevelCheckButton () .get_active ());
	getLODLevelSpinButton () .set_editable (getLODKeepCurrentLevelCheckButton () .get_active ());
}

X3DLODEditor::~X3DLODEditor ()
{ }

} // puck
} // titania
