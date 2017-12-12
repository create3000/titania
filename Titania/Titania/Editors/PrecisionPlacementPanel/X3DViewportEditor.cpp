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

#include "X3DViewportEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Layering/Viewport.h>

namespace titania {
namespace puck {

X3DViewportEditor::X3DViewportEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                      clipBoundary0 (this, getViewportClipBoundaryLeftAdjustment (),   getViewportClipBoundaryLeftSpinButton (),   "clipBoundary"),
	                      clipBoundary1 (this, getViewportClipBoundaryRightAdjustment (),  getViewportClipBoundaryRightSpinButton (),  "clipBoundary"),
	                      clipBoundary2 (this, getViewportClipBoundaryBottomAdjustment (), getViewportClipBoundaryBottomSpinButton (), "clipBoundary"),
	                      clipBoundary3 (this, getViewportClipBoundaryTopAdjustment (),    getViewportClipBoundaryTopSpinButton (),    "clipBoundary"),
	                            parents (),
	                     viewportBuffer (),
	                       viewportNode (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (parents, viewportBuffer, viewportNode);

	viewportBuffer .addInterest (&X3DViewportEditor::set_node, this);

	getViewportClipBoundaryLeftAdjustment ()   -> set_step_increment (1e-4);
	getViewportClipBoundaryRightAdjustment ()  -> set_step_increment (1e-4);
	getViewportClipBoundaryBottomAdjustment () -> set_step_increment (1e-4);
	getViewportClipBoundaryTopAdjustment ()    -> set_step_increment (1e-4);

	getViewportClipBoundaryLeftSpinButton ()   .property_climb_rate () = 1e-3;
	getViewportClipBoundaryRightSpinButton ()  .property_climb_rate () = 1e-3;
	getViewportClipBoundaryBottomSpinButton () .property_climb_rate () = 1e-3;
	getViewportClipBoundaryTopSpinButton ()    .property_climb_rate () = 1e-3;

	clipBoundary0 .setIndex (0);
	clipBoundary1 .setIndex (1);
	clipBoundary2 .setIndex (2);
	clipBoundary3 .setIndex (3);
}

void
X3DViewportEditor::initialize ()
{ }

void
X3DViewportEditor::set_selection (const X3D::MFNode & selection)
{
	set_viewport ();
}

void
X3DViewportEditor::on_viewport_toggled ()
{
	if (changing)
		return;

	getViewportCheckButton () .set_inconsistent (false);
	getViewportBox ()         .set_sensitive (getViewportCheckButton () .get_active ());

	// Set field.

	const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	const auto newViewportNode  = viewportNode ? viewportNode : executionContext -> createNode <X3D::Viewport> ();

	addUndoFunction <X3D::SFNode> (parents, "viewport", undoStep);

	for (const auto & parent : parents)
	{
		try
		{
			auto & field = parent -> getField <X3D::SFNode> ("viewport");

			if (getViewportCheckButton () .get_active ())
				X3D::X3DEditor::replaceNode (executionContext, parent, field, newViewportNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, parent, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (parents, "viewport", undoStep);
}

void
X3DViewportEditor::set_viewport ()
{
	viewportBuffer .addEvent ();
}

void
X3DViewportEditor::set_node ()
{
	changing = true;

	undoStep .reset ();

	// Remove selection

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("viewport") .removeInterest (&X3DViewportEditor::set_viewport, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	parents .clear ();

	// Find Emitter in selection

	const auto & selection    = getBrowserWindow () -> getSelection () -> getNodes ();
	const auto   pair         = getNode <X3D::Viewport> (selection, "viewport");
	const bool   inconsistent = pair .second == -1;

	viewportNode = selection .empty () ? nullptr : selection .back ();

	if (pair .second not_eq -2)
	{
		parents     = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DLayerNode, X3D::X3DConstants::LayoutGroup });
		viewportNode = pair .first;
	}

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("viewport") .addInterest (&X3DViewportEditor::set_viewport, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getViewportExpander ()    .set_visible (viewportNode or not parents .empty ());
	getCreateViewportBox ()   .set_visible (not parents .empty ());
	getViewportCheckButton () .set_active (viewportNode and not inconsistent);
	getViewportCheckButton () .set_inconsistent (inconsistent);
	getViewportBox ()         .set_sensitive (viewportNode and not inconsistent);

	// Widgets

	if (viewportNode)
	{
		// Normalize clipBoundary

		viewportNode -> clipBoundary () = {
			viewportNode -> getLeft (), 
			viewportNode -> getRight (), 
			viewportNode -> getBottom (), 
			viewportNode -> getTop ()
		};
	}

	const auto viewportNodes = viewportNode ? X3D::MFNode ({ viewportNode }) : X3D::MFNode ();

	clipBoundary0 .setNodes (viewportNodes);
	clipBoundary1 .setNodes (viewportNodes);
	clipBoundary2 .setNodes (viewportNodes);
	clipBoundary3 .setNodes (viewportNodes);

	changing = false;
}

X3DViewportEditor::~X3DViewportEditor ()
{ }

} // puck
} // titania
