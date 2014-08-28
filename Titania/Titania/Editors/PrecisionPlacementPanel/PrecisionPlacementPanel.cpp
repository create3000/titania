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

#include "PrecisionPlacementPanel.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

namespace titania {
namespace puck {

PrecisionPlacementPanel::PrecisionPlacementPanel (BrowserWindow* const browserWindow) :
	                   X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DPrecisionPlacementPanelInterface (get_ui ("Dialogs/PrecisionPlacementPanel.xml"), gconf_dir ()),
	                 X3DTransformEditor (),
	                           nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                           bboxSize (getBrowserWindow (),
	                                     getBBoxSizeXAdjustment (),
	                                     getBBoxSizeYAdjustment (),
	                                     getBBoxSizeZAdjustment (),
	                                     getBBoxSizeBox (),
	                                     "bboxSize"),
	                         bboxCenter (getBrowserWindow (),
	                                     getBBoxCenterXAdjustment (),
	                                     getBBoxCenterYAdjustment (),
	                                     getBBoxCenterZAdjustment (),
	                                     getBBoxCenterBox (),
	                                     "bboxCenter"),
	                      boundedObject ()
{
	setup ();
}

void
PrecisionPlacementPanel::initialize ()
{
	X3DPrecisionPlacementPanelInterface::initialize ();
	X3DTransformEditor::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &PrecisionPlacementPanel::set_selection);

	set_selection (getBrowser () -> getSelection () -> getChildren ());
}

void
PrecisionPlacementPanel::set_selection (const X3D::MFNode & selection)
{
//	boundedObject = selection .empty () ? nullptr : selection .back ();
//
//	const X3D::MFNode boundedObjects = boundedObject ? X3D::MFNode ({ boundedObject }) : X3D::MFNode ();
//
//	nodeName   .setNode  (X3D::SFNode (boundedObject));
//	bboxSize   .setNodes (boundedObjects);
//	bboxCenter .setNodes (boundedObjects);
//
//	getFillBoundingBoxFieldsButton () .set_sensitive (boundedObject);
//	getWindow () .resize (getWindow () .get_width (), 1);
}

void
PrecisionPlacementPanel::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex"));

	nodeIndex -> setTypes ({ X3D::X3DConstants::X3DBoundedObject });
}

void
PrecisionPlacementPanel::on_fill_bounding_box_fields_clicked ()
{
	using setValue = void (X3D::SFVec3f::*) (const X3D::Vector3f &);

	boundedObject -> bboxSize ()   = X3D::Vector3f (-1, -1, -1);
	boundedObject -> bboxCenter () = X3D::Vector3f ();

	const auto bbox       = boundedObject -> getBBox ();
	const auto bboxSize   = bbox .size ();
	const auto bboxCenter = bbox .center ();
	const auto undoStep   = std::make_shared <UndoStep> (_ ("Fill Bounding Box Fields"));

	undoStep -> addObjects (boundedObject);
	undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxSize ()), boundedObject -> bboxSize ());
	undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxSize ()), bboxSize);
	boundedObject -> bboxSize () = bboxSize;

	undoStep -> addObjects (boundedObject);
	undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxCenter ()), boundedObject -> bboxCenter ());
	undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (boundedObject -> bboxCenter ()), bboxCenter);
	boundedObject -> bboxCenter () = bboxCenter;
}

PrecisionPlacementPanel::~PrecisionPlacementPanel ()
{
	dispose ();
}

} // puck
} // titania
