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
	         enabled (getCurrentBrowser () -> getSelection () -> isEnabled ()),  
	  selectMultiple (getCurrentBrowser () -> getSelection () -> getSelectMultiple ()),
	    selectLowest (getCurrentBrowser () -> getSelection () -> getSelectLowest ()),
	  selectGeometry (getCurrentBrowser () -> getSelection () -> getSelectGeometry ()),
	            over (),
	          active (),
	       touchTime (),
	           nodes (),
	         browser (getCurrentBrowser ())
{
	addChildObjects (over, active, touchTime, nodes, browser);

	getCurrentBrowser () .addInterest (&BrowserSelection::set_browser, this);
	getCurrentContext () .addInterest (&BrowserSelection::set_execution_context, this);

	nodes .addInterest (&BrowserSelection::set_nodes, this);

	setup ();
}

void
BrowserSelection::set_browser ()
{
	{
		const auto & selection = browser -> getSelection ();

		selection -> isOver ()        .removeInterest (over);
		selection -> isActive ()      .removeInterest (active);
		selection -> getPickedTime () .removeInterest (touchTime);
		selection -> getNodes ()      .removeInterest (nodes);
	}

	browser = getCurrentBrowser ();

	{
		const auto & selection = browser -> getSelection ();

		selection -> isEnabled         (enabled);
		selection -> setSelectMultiple (selectMultiple);
		selection -> setSelectLowest   (selectLowest);
		selection -> setSelectGeometry (selectGeometry);

		selection -> isOver ()        .addInterest (over);
		selection -> isActive ()      .addInterest (active);
		selection -> getPickedTime () .addInterest (touchTime);
		selection -> getNodes ()      .addInterest (nodes);

		nodes = selection -> getNodes ();
	}
}

void
BrowserSelection::set_execution_context ()
{
	try
	{
		const auto worldInfo = getBrowserWindow () -> createWorldInfo ();
		const auto previous  = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/previous");
		const auto current   = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

		for (const auto & node : current)
			node -> getUserData <UserData> () -> selected |= PREVIOUSLY_SELECTED;
	
		for (const auto & node : current)
			node -> getUserData <UserData> () -> selected |= SELECTED;

		setNodes (current);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
BrowserSelection::set_nodes ()
{
	const auto worldInfo = getBrowserWindow () -> createWorldInfo ();
	const auto previous  = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/previous");
	const auto current   = worldInfo -> getMetaData <X3D::MFNode> ("/Titania/Selection/nodes");

	if (nodes == current)
		return;

	for (const auto & node : previous)
		node -> getUserData <UserData> () -> selected &= ~PREVIOUSLY_SELECTED;

	for (const auto & node : current)
	{
		node -> getUserData <UserData> () -> selected &= ~SELECTED;
		node -> getUserData <UserData> () -> selected |= PREVIOUSLY_SELECTED;
	}

	for (const auto & node : nodes)
		node -> getUserData <UserData> () -> selected |= SELECTED;

	worldInfo -> setMetaData ("/Titania/Selection/previous", current);
	worldInfo -> setMetaData ("/Titania/Selection/nodes",    nodes);

	// Remove old meta data: remove this call later, inserted at 18th Feb. 2017.
	worldInfo -> removeMetaData ("/Titania/Selection/children");
}

X3D::MFNode
BrowserSelection::getPreviousNodes () const
{
	return getBrowserWindow () -> getWorldInfo () -> getMetaData <X3D::MFNode> ("/Titania/Selection/previous");
}

void
BrowserSelection::isEnabled (const bool value)
{
	enabled = value;
	browser -> getSelection () -> isEnabled (value);
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

	selection -> addNodes (value);
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value)
{
	const auto & selection = browser-> getSelection ();

	selection -> removeNodes (value);
}

void
BrowserSelection::clearNodes ()
{
	const auto & selection = browser-> getSelection ();

	selection -> clearNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value)
{
	const auto & selection = browser-> getSelection ();

	selection -> setNodes (value);
}

void
BrowserSelection::addNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::addNodes, selection, value);

	selection -> addNodes (value);
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes,    selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::removeNodes, selection, value);

	selection -> removeNodes (value);
}

void
BrowserSelection::clearNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::clearNodes, selection);

	selection -> clearNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = browser-> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, value);

	selection -> setNodes (value);
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
