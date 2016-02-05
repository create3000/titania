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

#include "X3DViewportEditor.h"

#include "../../ComposedWidgets/RotationTool.h"

namespace titania {
namespace puck {

X3DViewportEditor::X3DViewportEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                              nodes (),
	                     viewportBuffer (),
	                           viewport (),
	                           undoStep (),
	                           changing (false),
	                      clipBoundary0 (this, getViewportClipBoundaryLeftAdjustment (),   getViewportClipBoundarySpinButton0 (), "clipBoundary"),
	                      clipBoundary1 (this, getViewportClipBoundaryRightAdjustment (),  getViewportClipBoundarySpinButton1 (), "clipBoundary"),
	                      clipBoundary2 (this, getViewportClipBoundaryBottomAdjustment (), getViewportClipBoundarySpinButton2 (), "clipBoundary"),
	                      clipBoundary3 (this, getViewportClipBoundaryTopAdjustment (),    getViewportClipBoundarySpinButton3 (), "clipBoundary")
{
	addChildren (viewportBuffer);
	viewportBuffer .addInterest (this, &X3DViewportEditor::set_node);

	getViewportClipBoundaryLeftAdjustment ()   -> set_step_increment (0.001);
	getViewportClipBoundaryRightAdjustment ()  -> set_step_increment (0.001);
	getViewportClipBoundaryBottomAdjustment () -> set_step_increment (0.001);
	getViewportClipBoundaryTopAdjustment ()    -> set_step_increment (0.001);

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

	addUndoFunction <X3D::SFNode> (nodes, "viewport", undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFNode> ("viewport");

			field .removeInterest (this, &X3DViewportEditor::set_viewport);
			field .addInterest (this, &X3DViewportEditor::connectViewport);

			if (getViewportCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (getCurrentContext (), node, field, X3D::SFNode (viewport), undoStep);
			else
				getBrowserWindow () -> replaceNode (getCurrentContext (), node, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (nodes, "viewport", undoStep);
}

void
X3DViewportEditor::set_viewport ()
{
	viewportBuffer .addEvent ();
}

void
X3DViewportEditor::set_node ()
{
	undoStep .reset ();

	// Remove selection

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("viewport") .removeInterest (this, &X3DViewportEditor::set_viewport);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes .clear ();

	// Find Viewport in selection

	const auto & selection = getBrowserWindow () -> getSelection () -> getChildren ();

	viewport = selection .empty () ? nullptr : selection .back ();

	try
	{
		if (not selection .empty ())
		{
			auto & field = selection .back () -> getField <X3D::SFNode> ("viewport");

			field .addInterest (this, &X3DViewportEditor::set_viewport);

			nodes    = { selection .back () };
			viewport = field;

			changing = true;

			getCreateViewportBox () .set_visible (true);
			getViewportCheckButton () .set_active (viewport);
			getViewportBox () .set_sensitive (viewport);

			changing = false;

			if (not viewport)
			{
				viewport = new X3D::Viewport (getCurrentContext ());
				viewport -> setup ();
			}
		}
	}
	catch (const X3D::X3DError &)
	{ }

	getViewportExpander () .set_visible (viewport);
	getCreateViewportBox () .set_visible (not nodes .empty ());

	const X3D::MFNode viewports (viewport ? X3D::MFNode ({ viewport }) : X3D::MFNode ());

	if (viewport)
	{
		viewport -> clipBoundary () = {
			viewport -> getLeft (), 
			viewport -> getRight (), 
			viewport -> getBottom (), 
			viewport -> getTop ()
		};
	}

	clipBoundary0 .setNodes (viewports);
	clipBoundary1 .setNodes (viewports);
	clipBoundary2 .setNodes (viewports);
	clipBoundary3 .setNodes (viewports);
}

void
X3DViewportEditor::connectViewport (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DViewportEditor::connectViewport);
	field .addInterest (this, &X3DViewportEditor::set_viewport);
}

X3DViewportEditor::~X3DViewportEditor ()
{ }

} // puck
} // titania
