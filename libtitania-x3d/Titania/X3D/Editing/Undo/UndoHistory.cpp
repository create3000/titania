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

#include "UndoHistory.h"

namespace titania {
namespace X3D {

UndoHistory::UndoHistory () :
	X3D::X3DOutput (),
	          list (),
	      redoList (),
	         index (-1),
	     redoIndex (-1),
	    savedIndex (-1)
{ }

std::string
UndoHistory::getUndoDescription () const
{
	return _ ("Undo ") + list [index] -> getDescription ();
}

std::string
UndoHistory::getRedoDescription () const
{
	return _ ("Redo ") + list [index + 1] -> getDescription ();
}

void
UndoHistory::addUndoStep (const UndoStepPtr & undoStep)
{
	if (undoStep -> getUndoFunctions () .empty ())
		return;

	if (index < savedIndex)
		savedIndex = -2;

	const size_t size = index + 1;

	redoIndex = index;
	redoList .assign (list .begin () + size, list .end ());

	list .resize (size);
	list .emplace_back (undoStep);

	++ index;

	processInterests ();
}

void
UndoHistory::removeUndoStep ()
{
	if (index < 0)
		return;

	-- index;

	list .pop_back ();

	if (redoIndex == index)
	{
		for (const auto & undoStep : redoList)
			list .emplace_back (undoStep);
	}

	processInterests ();
}

const std::shared_ptr <UndoStep> &
UndoHistory::getUndoStep () const
{
	static const std::shared_ptr <UndoStep> empty;

	if (list .empty () or index < 0)
		return empty;

	return list [index];
}

bool
UndoHistory::hasUndo () const
{
	return index >= 0;
}

bool
UndoHistory::hasRedo () const
{
	return index + 1 < (int32_t) list .size ();
}

void
UndoHistory::undo ()
{
	if (index >= 0)
	{
		list [index] -> undo ();

		-- index;

		processInterests ();
	}
}

void
UndoHistory::redo ()
{
	if (index + 1 < (int32_t) list .size ())
	{
		++ index;

		list [index] -> redo ();

		processInterests ();
	}
}

void
UndoHistory::clear ()
{
	list .clear ();

	index = -1;

	save ();

	processInterests ();
}

} // X3D
} // titania
