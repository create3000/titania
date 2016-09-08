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

#include "SFStringEntry.h"

namespace titania {
namespace puck {

SFStringEntry::SFStringEntry (X3DBaseInterface* const editor,
                              Gtk::Entry & entry,
                              const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	            entry (entry),
	            nodes (),
	             name (name),
	         undoStep (),
	         changing (false),
	           buffer ()
{
	addChildren (nodes, buffer);

	buffer .addInterest (this, &SFStringEntry::set_buffer);

	entry .signal_changed () .connect (sigc::mem_fun (*this, &SFStringEntry::on_changed));
	setup ();
}

void
SFStringEntry::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFString> (name) .removeInterest (this, &SFStringEntry::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFString> (name) .addInterest (this, &SFStringEntry::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
SFStringEntry::on_changed ()
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	addUndoFunction <X3D::SFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFString> (name);

			field .removeInterest (this, &SFStringEntry::set_field);
			field .addInterest (this, &SFStringEntry::connect);

			field = entry .get_text ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFString> (nodes, name, undoStep);
}

void
SFStringEntry::set_field ()
{
	buffer .addEvent ();
}

void
SFStringEntry::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last field.

	const auto pair = getArray <X3D::SFString> (nodes, name);

	if (pair .second >= 0)
		entry .set_text (pair .first);
	else
	{
		entry .set_text ("");
		entry .set_placeholder_text (pair .first);
	}

	entry .set_sensitive (pair .second not_eq -2);

	changing = false;
}

void
SFStringEntry::connect (const X3D::SFString & field)
{
	field .removeInterest (this, &SFStringEntry::connect);
	field .addInterest (this, &SFStringEntry::set_field);
}

} // puck
} // titania
