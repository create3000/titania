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

#include "X3DEditorObject.h"

namespace titania {
namespace puck {

void
X3DEditorObject::setup ()
{
	getBrowser () .addInterest (this, &X3DEditorObject::set_browser);
}

void
X3DEditorObject::set_browser (const X3D::BrowserPtr & value)
{
	fields -> setExecutionContext (value);
}

/***********************************************************************************************************************
 *
 *  Validate Id
 *
 **********************************************************************************************************************/

void
X3DEditorObject::validateIdOnInsert (Gtk::Entry & entry, const Glib::ustring & insert, int position)
{
	const std::string text = entry .get_text () .insert (position, insert);

	if (not validateId (text))
		entry .signal_insert_text () .emission_stop ();
}

void
X3DEditorObject::validateIdOnDelete (Gtk::Entry & entry, int start_pos, int end_pos)
{
	const std::string text = entry .get_text () .erase (start_pos, end_pos - start_pos);

	if (text .length () and not validateId (text))
		entry .signal_delete_text () .emission_stop ();
}

bool
X3DEditorObject::validateId (const std::string & text) const
{
	return X3D::RegEx::Id .FullMatch (text) and text not_eq "NULL";
}

/***********************************************************************************************************************
 *
 *  Refine name
 *
 **********************************************************************************************************************/

Glib::ustring
X3DEditorObject::refineName (const Glib::ustring & name)
{
	Glib::ustring result;

	if (not name .empty ())
	{
		result += name .substr (0, 1) .uppercase ();

		for (const auto c : std::make_pair (++ name .begin (), name .end ()))
		{
			const Glib::ustring s (1, c);

			if (s .uppercase () == s)
				result += " ";

			result += s;
		}
	}

	return result;
}

} // puck
} // titania
