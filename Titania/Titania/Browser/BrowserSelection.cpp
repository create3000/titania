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

#include "X3DBrowserWindow.h"

#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

BrowserSelection::BrowserSelection (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	         enabled (getCurrentBrowser () -> getSelection () -> isEnabled ()),  
	            mode (getCurrentBrowser () -> getSelection () -> getMode ()),
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
	getNodes ()          .addInterest (&BrowserSelection::set_nodes, this);

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

		selection -> isEnabled (enabled);
		selection -> setMode (mode);
		selection -> setSelectLowest (selectLowest);
		selection -> setSelectGeometry (selectGeometry);

		selection -> isOver ()        .addInterest (over);
		selection -> isActive ()      .addInterest (active);
		selection -> getPickedTime () .addInterest (touchTime);
		selection -> getNodes ()      .addInterest (nodes);

		nodes = selection -> getNodes ();
	}
}

void
BrowserSelection::isEnabled (const bool value)
{
	enabled = value;
	browser -> getSelection () -> isEnabled (value);
}

void
BrowserSelection::setMode (const X3D::Selection::SelectionType value)
{
	mode = value;
	browser -> getSelection () -> setMode (value);
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

void
BrowserSelection::addNodes (const X3D::MFNode & value)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> addNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> removeNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::clearNodes ()
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> clearNodes ();

	nodes = selection -> getNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> setNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::addNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::addNodes, selection, value);

	selection -> addNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::removeNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes,    selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::removeNodes, selection, value);

	selection -> removeNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::clearNodes (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::clearNodes, selection);

	selection -> clearNodes ();

	nodes = selection -> getNodes ();
}

void
BrowserSelection::setNodes (const X3D::MFNode & value, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setNodes, selection, selection -> getNodes ());
	undoStep -> addRedoFunction (&X3D::Selection::setNodes, selection, value);

	selection -> setNodes (value);

	nodes = selection -> getNodes ();
}

void
BrowserSelection::undoRestoreSelection (const X3D::UndoStepPtr & undoStep)
{
	undoStep -> addUndoFunction (&X3D::Selection::setNodes,
	                             getCurrentBrowser () -> getSelection (),
	                             getCurrentBrowser () -> getSelection () -> getNodes ());
}

void
BrowserSelection::redoRestoreSelection (const X3D::UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&X3D::Selection::setNodes,
	                             getCurrentBrowser () -> getSelection (),
	                             getCurrentBrowser () -> getSelection () -> getNodes ());
}

const X3D::MFNode &
BrowserSelection::getPrevious () const
{
	try
	{
		return getBrowserWindow () -> getWorldInfo () -> getMetaData <X3D::MFNode> ("/Titania/Selection/previous", false);
	}
	catch (const std::exception & error)
	{
		static const X3D::MFNode empty;

		return empty;
	}
}

void
BrowserSelection::set_nodes ()
{
	if (getNodes () .empty ())
		return;

	const auto worldInfo   = getBrowserWindow () -> createWorldInfo ();
	const auto metadataSet = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/Selection");
	const auto previous    = metadataSet -> createValue <X3D::MetadataSet> ("previous");
	const auto nodes       = metadataSet -> createValue <X3D::MetadataSet> ("children");

	if (nodes -> value () == getNodes ())
		return;

	previous -> isPrivate (true);
	previous -> value () = nodes -> value ();

	nodes -> isPrivate (true);
	nodes -> value () = getNodes ();
}

BrowserSelection::~BrowserSelection ()
{
	dispose ();
}

} // puck
} // titania
