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

#include "BrowserSelection.h"

#include "X3DBrowserWindow.h"

namespace titania {
namespace puck {

BrowserSelection::BrowserSelection (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	         enabled (getBrowser () -> getSelection () -> isEnabled ()),  
	            mode (getBrowser () -> getSelection () -> getMode ()),
	    selectLowest (getBrowser () -> getSelection () -> getSelectLowest ()),
	            over (),
	          active (),
	       touchTime (),
	        children (),
	         browser (getBrowser ())
{
	X3DBaseInterface::addChildren (over, active, touchTime, children, browser);

	getBrowser () .addInterest (this, &BrowserSelection::set_browser);

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

	browser = getBrowser ();

	{
		const auto & selection = browser -> getSelection ();

		selection -> isEnabled (enabled);
		selection -> setMode (mode);
		selection -> setSelectLowest (selectLowest);

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
BrowserSelection::addChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep) const
{
	const auto & selection = getBrowser () -> getSelection ();

	//undoStep -> addUndoFunction (&X3D::X3DBrowser::update, getBrowser ());

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::addChildren, selection, nodes);

	selection -> addChildren (nodes);

	//undoStep -> addRedoFunction (&X3D::X3DBrowser::update, getBrowser ());
	//getBrowser () -> update ();
}

void
BrowserSelection::removeChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep) const
{
	const auto & selection = getBrowser () -> getSelection ();

	//undoStep -> addUndoFunction (&X3D::X3DBrowser::update, getBrowser ());

	undoStep -> addUndoFunction (&X3D::Selection::setChildren,    selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::removeChildren, selection, nodes);

	selection -> removeChildren (nodes);

	//undoStep -> addRedoFunction (&X3D::X3DBrowser::update, getBrowser ());
	//getBrowser () -> update ();
}

void
BrowserSelection::setChildren (const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep) const
{
	const auto & selection = getBrowser () -> getSelection ();

	//undoStep -> addUndoFunction (&X3D::X3DBrowser::update, getBrowser ());

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::setChildren, selection, nodes);

	selection -> setChildren (nodes);

	//undoStep -> addRedoFunction (&X3D::X3DBrowser::update, getBrowser ());
	//getBrowser () -> update ();
}

void
BrowserSelection::redoRestoreSelection (const X3D::UndoStepPtr & undoStep) const
{
	undoStep -> addRedoFunction (&X3D::Selection::setChildren,
	                             getBrowser () -> getSelection (),
	                             getBrowser () -> getSelection () -> getChildren ());

	//undoStep -> addRedoFunction (&X3D::X3DBrowser::update, getBrowser ());
}

void
BrowserSelection::undoRestoreSelection (const X3D::UndoStepPtr & undoStep) const
{
	//undoStep -> addUndoFunction (&X3D::X3DBrowser::update, getBrowser ());

	undoStep -> addUndoFunction (&X3D::Selection::setChildren,
	                             getBrowser () -> getSelection (),
	                             getBrowser () -> getSelection () -> getChildren ());

	//getBrowser () -> update ();
}

void
BrowserSelection::clear (const X3D::UndoStepPtr & undoStep) const
{
	const auto & selection = getBrowser () -> getSelection ();

	//undoStep -> addUndoFunction (&X3D::X3DBrowser::update, getBrowser ());

	undoStep -> addUndoFunction (&X3D::Selection::setChildren, selection, selection -> getChildren ());
	undoStep -> addRedoFunction (&X3D::Selection::clear, selection);

	selection -> clear ();

	//undoStep -> addRedoFunction (&X3D::X3DBrowser::update, getBrowser ());
	//getBrowser () -> update ();
}

BrowserSelection::~BrowserSelection ()
{
	dispose ();
}

} // puck
} // titania
