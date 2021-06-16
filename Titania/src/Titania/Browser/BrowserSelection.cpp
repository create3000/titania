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

#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"

#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

// nodes triggers two event when setNodes or ... is called because removeTool is called in X3DBrowser::getSelection.

BrowserSelection::BrowserSelection (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	         enabled (getCurrentBrowser () -> getSelectable ()),
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

	getCurrentBrowser () .addInterest (&BrowserSelection::set_browser,           this);
	getCurrentContext () .addInterest (&BrowserSelection::set_execution_context, this);

	nodes .addInterest (&BrowserSelection::set_nodes, this);

	setup ();
}

void
BrowserSelection::set_browser ()
{
	{
		const auto & selection = browser -> getSelection ();

		browser -> getSelectable ()       .removeInterest (enabled);
		selection -> getSelectMultiple () .removeInterest (selectMultiple);
		selection -> getSelectLowest ()   .removeInterest (selectLowest);
		selection -> getSelectGeometry () .removeInterest (selectGeometry);

		selection -> getTouchTime ()  .removeInterest (touchTime);
		selection -> getHierarchy ()  .removeInterest (hierarchy);

		selection -> getNodes ()      .removeInterest (&BrowserSelection::set_selection, this);
		selection -> getGeometries () .removeInterest (&BrowserSelection::set_selection, this);
	}

	browser = getCurrentBrowser ();

	{
		const auto & selection = browser -> getSelection ();

		browser -> getSelectable ()       .addInterest (enabled);
		selection -> getSelectMultiple () .addInterest (selectMultiple);
		selection -> getSelectLowest ()   .addInterest (selectLowest);
		selection -> getSelectGeometry () .addInterest (selectGeometry);

		selection -> getTouchTime () .addInterest (touchTime);
		selection -> getHierarchy () .addInterest (hierarchy);

		selection -> getNodes ()      .addInterest (&BrowserSelection::set_selection, this);
		selection -> getGeometries () .addInterest (&BrowserSelection::set_selection, this);

		browser -> setSelectable       (enabled);
		selection -> setSelectMultiple (selectMultiple);
		selection -> setSelectLowest   (selectLowest);
		selection -> setSelectGeometry (selectGeometry);

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
		const auto worldInfo      = getBrowserWindow () -> createWorldInfo (getCurrentScene ());
		const auto selectGeometry = worldInfo -> getMetaData <X3D::SFBool> ("/Titania/Selection/selectGeometry");
		const auto current        = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

		// Set selection.

		setNodes (current);
		setSelectGeometry (selectGeometry);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
BrowserSelection::set_nodes (const X3D::MFNode & nodes)
{
	const auto worldInfo = createWorldInfo (getCurrentScene ());
	const auto current   = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

	if (nodes == current)
		return;

	// Set meta data.

	worldInfo -> setMetaData ("/Titania/Selection/nodes", nodes);

	// Remove old meta data: remove this call later, inserted at 18th Feb. 2017.
	worldInfo -> removeMetaData ("/Titania/Selection/previous");
	worldInfo -> removeMetaData ("/Titania/Selection/children");

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
BrowserSelection::set_selection ()
{
	assignNodes ();
}

void
BrowserSelection::setEnabled (const bool value)
{
	browser -> setSelectable (value);
	enabled .set (value);
}

void
BrowserSelection::setSelectMultiple (const bool value)
{
	browser -> getSelection () -> setSelectMultiple (value);
	selectMultiple .set (value);
}

void
BrowserSelection::setSelectLowest (const bool value)
{
	browser -> getSelection () -> setSelectLowest (value);
	selectLowest .set (value);
}

void
BrowserSelection::setSelectGeometry (const bool value)
{
	const auto & selection = browser -> getSelection ();

	createWorldInfo (getCurrentScene ()) -> setMetaData ("/Titania/Selection/selectGeometry", value);

	selection -> setSelectGeometry (value);
	selectGeometry .set (value);

	assignNodes ();
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
	selection -> addNodes (filterSelection (value));

	assignNodes ();
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value)
{
	const auto & selection = browser -> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> removeNodes (value);

	assignNodes ();
}

void
BrowserSelection::clearNodes ()
{
	const auto & selection = browser -> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> clearNodes ();

	assignNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value)
{
	const auto & selection = browser -> getSelection ();

	selection -> setSelectGeometry (false);
	selection -> setNodes (filterSelection (value));

	assignNodes ();
}

X3D::MFNode
BrowserSelection::getParents () const
{
	const auto & selection = browser -> getSelection ();

	return selection -> getParents ();
}

X3D::MFNode
BrowserSelection::getChildren () const
{
	const auto & selection = browser -> getSelection ();

	return selection -> getChildren ();
}

void
BrowserSelection::addNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();
	const auto   filtered  = filterSelection (value);

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::addNodes, selection, filtered);

	selection -> setSelectGeometry (false);
	selection -> addNodes (filtered);

	assignNodes ();
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::removeNodes, selection, value);

	selection -> setSelectGeometry (false);
	selection -> removeNodes (value);

	assignNodes ();
}

void
BrowserSelection::clearNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::clearNodes, selection);

	selection -> setSelectGeometry (false);
	selection -> clearNodes ();

	assignNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();
	const auto   filtered  = filterSelection (value);

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, selection -> getSelectGeometry ());
	undoStep -> addRedoFunction (&X3D::Selection::setSelectGeometry, selection, false);
	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, filtered);

	selection -> setSelectGeometry (false);
	selection -> setNodes (filtered);

	assignNodes ();
}

void
BrowserSelection::undoRestoreNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
}

void
BrowserSelection::redoRestoreNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser -> getSelection ();

	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
}

X3D::MFNode
BrowserSelection::filterSelection (X3D::MFNode value)
{
	try
	{
		const auto worldInfo = getWorldInfo (getCurrentScene ());

		value .erase (std::remove (value .begin (), value .end (), worldInfo), value .end ());

		return value;
	}
	catch (const X3D::X3DError &)
	{
		return value;
	}
}

void
BrowserSelection::assignNodes ()
{
	const auto & selection = browser -> getSelection ();

	if (selection -> getNodes () not_eq nodes)
	{
		nodes = selection -> getNodes ();

		set_nodes (nodes);
	}

	if (selection -> getGeometries () not_eq geometryNodes)
		geometryNodes = selection -> getGeometries ();
}

BrowserSelection::~BrowserSelection ()
{
	dispose ();
}

} // puck
} // titania
