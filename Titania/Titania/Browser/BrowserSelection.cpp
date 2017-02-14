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
	        children (),
	         browser (getCurrentBrowser ())
{
	addChildObjects (over, active, touchTime, children, browser);

	getCurrentBrowser () .addInterest (&BrowserSelection::set_browser, this);
	getChildren ()       .addInterest (&BrowserSelection::set_children, this);

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
		selection -> getChildren ()   .removeInterest (children);
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
		selection -> getChildren ()   .addInterest (children);

		children = selection -> getChildren ();
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
BrowserSelection::addChildren (const X3D::MFNode & nodes)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> addChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::removeChildren (const X3D::MFNode & nodes)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> removeChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::setChildren (const X3D::MFNode & nodes)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> setChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::clear ()
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	selection -> clear ();

	children = selection -> getChildren ();
}

void
BrowserSelection::addChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::addChildren, selection, nodes);

	selection -> addChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::removeChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setChildren,    selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::removeChildren, selection, nodes);

	selection -> removeChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::setChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::setChildren, selection, nodes);

	selection -> setChildren (nodes);

	children = selection -> getChildren ();
}

void
BrowserSelection::undoRestoreSelection (const X3D::UndoStepPtr & undoStep)
{
	undoStep -> addUndoFunction (&X3D::Selection::setChildren,
	                             getCurrentBrowser () -> getSelection (),
	                             getCurrentBrowser () -> getSelection () -> getChildren ());
}

void
BrowserSelection::redoRestoreSelection (const X3D::UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&X3D::Selection::setChildren,
	                             getCurrentBrowser () -> getSelection (),
	                             getCurrentBrowser () -> getSelection () -> getChildren ());
}

void
BrowserSelection::clear (const X3D::UndoStepPtr & undoStep)
{
	const auto & selection = getCurrentBrowser () -> getSelection ();

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::clear, selection);

	selection -> clear ();

	children = selection -> getChildren ();
}

X3D::MFNode
BrowserSelection::getPrevious () const
{
	try
	{
		const auto worldInfo   = getWorldInfo ();
		const auto metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/Selection");
		const auto children    = metadataSet -> getValue <X3D::MetadataSet> ("previous");

		return children -> value ();
	}
	catch (const std::exception & error)
	{
		return X3D::MFNode ();
	}
}

void
BrowserSelection::set_children ()
{
	const auto worldInfo = getBrowserWindow () -> createWorldInfo ();

	if (children .empty ())
		worldInfo -> removeMetaData ("/Titania/Selection/children");

	else
	{
		const auto metadataSet = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/Selection");
		const auto previous    = metadataSet -> createValue <X3D::MetadataSet> ("previous");
		const auto children    = metadataSet -> createValue <X3D::MetadataSet> ("children");

		if (children -> value () == getChildren ())
			return;

		previous -> isPrivate (true);
		previous -> value () = children -> value ();

		children -> isPrivate (true);
		children -> value () = getChildren ();
	}
}

BrowserSelection::~BrowserSelection ()
{
	dispose ();
}

} // puck
} // titania
