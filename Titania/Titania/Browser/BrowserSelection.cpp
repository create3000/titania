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

#include "BrowserWindow.h"

namespace titania {
namespace puck {

BrowserSelection::BrowserSelection (BrowserWindow* const browserWindow) :
	      X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3D::X3DPointingDevice (browserWindow -> getBrowser ()),
	               enabled (false),
	              hasMoved (false)
{ }

void
BrowserSelection::setEnabled (bool value)
{
	enabled = value;

	if (enabled)
	{
		connect ();

		getBrowser () -> setPicking (false);
	}
	else
	{
		disconnect ();

		getBrowser () -> setPicking (true);
	}
}

void
BrowserSelection::addChildren (const X3D::MFNode & nodes, const UndoStepPtr & undoStep) const
{
	auto selection = getBrowser () -> getSelection ();

	undoStep -> addUndoFunction (std::mem_fn (&X3D::Selection::setChildren), selection, selection -> getChildren ());
	undoStep -> addRedoFunction (std::mem_fn (&X3D::Selection::addChildren), selection, nodes);

	selection -> addChildren (nodes);
}

void
BrowserSelection::removeChildren (const X3D::MFNode & nodes, const UndoStepPtr & undoStep) const
{
	auto selection = getBrowser () -> getSelection ();

	undoStep -> addUndoFunction (std::mem_fn (&X3D::Selection::setChildren),    selection, selection -> getChildren ());
	undoStep -> addRedoFunction (std::mem_fn (&X3D::Selection::removeChildren), selection, nodes);

	selection -> removeChildren (nodes);
}

void
BrowserSelection::setChildren (const X3D::MFNode & nodes, const UndoStepPtr & undoStep) const
{
	auto selection = getBrowser () -> getSelection ();

	undoStep -> addUndoFunction (std::mem_fn (&X3D::Selection::setChildren), selection, selection -> getChildren ());
	undoStep -> addRedoFunction (std::mem_fn (&X3D::Selection::setChildren), selection, nodes);

	selection -> setChildren (nodes);
}

void
BrowserSelection::clear (const UndoStepPtr & undoStep) const
{
	auto selection = getBrowser () -> getSelection ();

	undoStep -> addUndoFunction (std::mem_fn (&X3D::Selection::setChildren), selection, selection -> getChildren ());
	undoStep -> addRedoFunction (std::mem_fn (&X3D::Selection::clear),       selection);

	selection -> clear ();
}

void
BrowserSelection::motionNotifyEvent (bool)
{
	hasMoved = true;
}

bool
BrowserSelection::buttonPressEvent (bool)
{
	hasMoved = false;

	return false;
}

void
BrowserSelection::buttonReleaseEvent (bool picked)
{
	if (hasMoved)
		return;

	if (picked)
	{
		auto hit       = getBrowser () -> getHits () .front ();
		auto hierarchy = std::move (X3D::find (getBrowser () -> getExecutionContext () -> getRootNodes (), hit -> shape, false));

		if (not hierarchy .empty ())
		{
			X3D::SFNode node (hierarchy .front ());

			if (getBrowserWindow () -> getConfig () .getBoolean ("selectLowest"))
			{
				for (const auto & object : basic::reverse_adapter (hierarchy))
				{
					X3D::SFNode lowest (object);

					if (not lowest)
						continue;

					if (lowest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
						continue;

					if (dynamic_cast <X3D::Transform*> (lowest .getValue ()))
					{
						node = lowest;
						break;
					}
				}
			}

			getBrowser () -> getSelection () -> setChildren ({ node });
			getBrowser () -> update ();
		}
	}
	else
	{
		getBrowser () -> getSelection () -> clear ();
		getBrowser () -> update ();
	}
}

bool
BrowserSelection::trackSensors ()
{
	return not (getBrowserWindow () -> getKeys () .shift () or getBrowserWindow () -> getKeys () .control ());
}

BrowserSelection::~BrowserSelection ()
{ }

} // puck
} // titania
