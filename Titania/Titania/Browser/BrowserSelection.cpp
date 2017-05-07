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

#include "BrowserSelection.h"

#include "UserData.h"
#include "X3DBrowserWindow.h"

#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

// nodes triggers two event when setNodes or ... is called because removeTool is called in X3DBrowser::getSelection.

BrowserSelection::BrowserSelection (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	         enabled (getCurrentBrowser () -> getSelection () -> getEnabled ()),  
	  selectMultiple (getCurrentBrowser () -> getSelection () -> getSelectMultiple ()),
	    selectLowest (getCurrentBrowser () -> getSelection () -> getSelectLowest ()),
	  selectGeometry (getCurrentBrowser () -> getSelection () -> getSelectGeometry ()),
	       touchTime (),
	           nodes (),
	   geometryNodes (),
	       hierarchy (),
	         browser (getCurrentBrowser ())
{
	addChildObjects (enabled,
	                 selectMultiple,
	                 selectLowest,
	                 selectGeometry,
	                 touchTime,
	                 nodes,
	                 geometryNodes,
	                 hierarchy,
	                 browser);

	getCurrentBrowser () .addInterest (&BrowserSelection::set_browser, this);
	getCurrentContext () .addInterest (&BrowserSelection::set_execution_context, this);

	nodes .addInterest (&BrowserSelection::set_nodes, this);

	setup ();
}

void
BrowserSelection::connectNodes ()
{
	nodes .removeInterest (&BrowserSelection::connectNodes, this);
	nodes .addInterest (&BrowserSelection::set_nodes, this);
}

void
BrowserSelection::set_browser ()
{
	{
		const auto & selection = browser -> getSelection ();

		selection -> getEnabled ()        .removeInterest (enabled);
		selection -> getSelectMultiple () .removeInterest (selectMultiple);
		selection -> getSelectLowest ()   .removeInterest (selectLowest);
		selection -> getSelectGeometry () .removeInterest (selectGeometry);

		selection -> getTouchTime ()  .removeInterest (touchTime);
		selection -> getGeometries () .removeInterest (geometryNodes);
		selection -> getNodes ()      .removeInterest (nodes);
		selection -> getHierarchy ()  .removeInterest (hierarchy);
	}

	browser = getCurrentBrowser ();

	{
		const auto & selection = browser -> getSelection ();

		selection -> setEnabled        (enabled);
		selection -> setSelectMultiple (selectMultiple);
		selection -> setSelectLowest   (selectLowest);
		selection -> setSelectGeometry (selectGeometry);

		selection -> getEnabled ()        .addInterest (enabled);
		selection -> getSelectMultiple () .addInterest (selectMultiple);
		selection -> getSelectLowest ()   .addInterest (selectLowest);
		selection -> getSelectGeometry () .addInterest (selectGeometry);

		selection -> getTouchTime ()  .addInterest (touchTime);
		selection -> getNodes ()      .addInterest (nodes);
		selection -> getGeometries () .addInterest (geometryNodes);
		selection -> getHierarchy ()  .addInterest (hierarchy);

		nodes         = selection -> getNodes ();
		geometryNodes = selection -> getGeometries ();
		hierarchy     = selection -> getHierarchy ();
	}
}

void
BrowserSelection::set_execution_context ()
{
	try
	{
		const auto worldInfo = getBrowserWindow () -> createWorldInfo ();
		const auto current   = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

		// Set clone bits.

		for (const auto & node : current)
		{
			if (node)
				node -> getUserData <UserData> () -> cloneCount .set (CLONE_SELECTED);
		}

		// Set selection.

		// Note: setNodes triggers a set_nodes event, which we do not need if scene changes.
		nodes .removeInterest (&BrowserSelection::set_nodes, this);
		nodes .addInterest (&BrowserSelection::connectNodes, this);

		setNodes (current);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
BrowserSelection::set_nodes (const X3D::MFNode & nodes)
{
	const auto worldInfo = getBrowserWindow () -> createWorldInfo ();
	const auto current   = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

	if (nodes == current)
		return;

	// Set and clear clone bits.

	for (const auto & node : current)
	{
		if (node)
			node -> getUserData <UserData> () -> cloneCount .reset (CLONE_SELECTED);
	}

	for (const auto & node : nodes)
	{
		if (node)
			node -> getUserData <UserData> () -> cloneCount .set (CLONE_SELECTED);
	}

	// Set meta data.

	worldInfo -> setMetaData ("/Titania/Selection/nodes", nodes);

	// Remove old meta data: remove this call later, inserted at 18th Feb. 2017.
	worldInfo -> removeMetaData ("/Titania/Selection/previous");
	worldInfo -> removeMetaData ("/Titania/Selection/children");

	getBrowserWindow () -> setModified (browser, true);
}

void
BrowserSelection::setEnabled (const bool value)
{
	enabled = value;
	browser -> getSelection () -> setEnabled (value);
}

void
BrowserSelection::setSelectMultiple (const bool value)
{
	selectMultiple = value;
	browser -> getSelection () -> setSelectMultiple (value);
}

void
BrowserSelection::setSelectLowest (const bool value)
{
	selectLowest = value;
	browser -> getSelection () -> setSelectLowest (value);
}

void
BrowserSelection::setSelectGeometry (const bool value)
{
	selectGeometry = value;
	browser -> getSelection () -> setSelectGeometry (value);
}

bool
BrowserSelection::isSelected (const X3D::SFNode & node) const
{
	return browser -> getSelection () -> isSelected (node);
}

void
BrowserSelection::addNodes (const X3D::MFNode & value)
{
	const auto & selection = browser -> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> addNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value)
{
	const auto & selection = browser-> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> removeNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::clearNodes ()
{
	const auto & selection = browser-> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> clearNodes ();

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::setNodes (const X3D::MFNode & value)
{
	const auto & selection = browser-> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> setNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

X3D::MFNode
BrowserSelection::getParents () const
{
	const auto & selection = browser-> getSelection ();

	return selection -> getParents ();
}

X3D::MFNode
BrowserSelection::getChildren () const
{
	const auto & selection = browser-> getSelection ();

	return selection -> getChildren ();
}

void
BrowserSelection::addNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::addNodes, selection, value);

	selection -> setSelectGeometry (false);
	selection -> addNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes,    selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::removeNodes, selection, value);

	selection -> setSelectGeometry (false);
	selection -> removeNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::clearNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::clearNodes, selection);

	selection -> setSelectGeometry (false);
	selection -> clearNodes ();

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::setNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, value);

	selection -> setSelectGeometry (false);
	selection -> setNodes (value);

	// We must immediately call set_nodes to to remove from meta data to lower clone count.
	set_nodes (selection -> getNodes ());
}

void
BrowserSelection::undoRestoreNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
}

void
BrowserSelection::redoRestoreNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
}

BrowserSelection::~BrowserSelection ()
{
	dispose ();
}

} // puck
} // titania
