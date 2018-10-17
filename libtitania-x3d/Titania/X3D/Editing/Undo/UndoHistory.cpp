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

#include "UndoHistory.h"

namespace titania {
namespace X3D {

UndoHistory::UndoHistory () :
	X3D::X3DOutput (),
	      undoList (),
	      redoList (),
	    savedIndex (-1)
{ }

bool
UndoHistory::getModified () const
{
	return int32_t (undoList .size ()) - 1 not_eq savedIndex;
}

void
UndoHistory::setSaved ()
{
	savedIndex = int32_t (undoList .size ()) - 1;
}

std::string
UndoHistory::getUndoDescription () const
{
	return _ ("Undo ") + undoList .back () -> getDescription ();
}

std::string
UndoHistory::getRedoDescription () const
{
	return _ ("Redo ") + redoList .front () -> getDescription ();
}

void
UndoHistory::addUndoStep (const UndoStepPtr & undoStep, const time_type time)
{
	if (undoStep -> getUndoFunctions () .empty ())
		return;

	if (int32_t (undoList .size ()) - 1 < savedIndex)
		savedIndex = -1;

	undoStep -> setTime (time);

	if (not undoList .empty () and time == undoList .back () -> getTime ())
	{
		if (undoList .back () -> getDescription () .empty ())
			undoList .back () -> setDescription (undoStep -> getDescription ());

		undoList .back () -> addUndoFunction (&UndoStep::undo, undoStep);
		undoList .back () -> addRedoFunction (&UndoStep::redo, undoStep);
	}
	else
		undoList .emplace_back (undoStep);

	redoList .clear ();

	processInterests ();
}

void
UndoHistory::removeUndoStep ()
{
	if (undoList .empty ())
		return;

	undoList .pop_back ();

	processInterests ();
}

const std::shared_ptr <UndoStep> &
UndoHistory::getUndoStep () const
{
	static const std::shared_ptr <UndoStep> empty;

	if (undoList .empty ())
		return empty;

	return undoList .back ();
}

bool
UndoHistory::hasUndo () const
{
	return not undoList .empty ();
}

bool
UndoHistory::hasRedo () const
{
	return not redoList .empty ();
}

void
UndoHistory::undo ()
{
	if (undoList .empty ())
		return;

	const auto undoStep = undoList .back ();

	undoStep -> undo ();

	undoList .pop_back ();
	redoList .emplace_front (undoStep);

	processInterests ();
}

void
UndoHistory::redo ()
{
	if (redoList .empty ())
		return;

	const auto undoStep = redoList .front ();

	undoStep -> redo ();

	undoList .emplace_back (undoStep);
	redoList .pop_front ();

	processInterests ();
}

void
UndoHistory::clear ()
{
	undoList .clear ();
	redoList .clear ();

	setSaved ();

	processInterests ();
}

void
UndoHistory::dispose ()
{
	undoList .clear ();
	redoList .clear ();

	X3D::X3DOutput::dispose ();
}

UndoHistory::~UndoHistory ()
{ }

} // X3D
} // titania
