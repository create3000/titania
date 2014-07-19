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

#include "X3DEditorObject.h"

namespace titania {
namespace puck {

X3DEditorObject::X3DEditorObject () :
	X3DBaseInterface (),
	    currentField (),
	          fields (new X3D::FieldSet (getBrowser ())),
	        undoSize (0)
{ }

void
X3DEditorObject::on_number_insert_text (const Glib::ustring & insert, int* position, Gtk::Entry & entry)
{
	const std::string text = entry .get_text () .insert (*position, insert);

	if (not validateNumber (text))
		entry .signal_insert_text () .emission_stop ();
}

void
X3DEditorObject::on_number_delete_text (int start_pos, int end_pos, Gtk::Entry & entry)
{
	const std::string text = entry .get_text () .erase (start_pos, end_pos - start_pos);

	if (text .length () and not validateNumber (text))
		entry .signal_delete_text () .emission_stop ();
}

bool
X3DEditorObject::validateNumber (const std::string & text) const
{
	std::istringstream istream (text);

	double value = 0;

	if (istream >> value)
		return istream .eof ();

	return false;
}

double
X3DEditorObject::toDouble (const std::string & string)
{
	std::istringstream istream (string);

	double value = 0;

	istream >> value;

	return value;
}

X3DEditorObject::~X3DEditorObject ()
{ }

} // puck
} // titania
