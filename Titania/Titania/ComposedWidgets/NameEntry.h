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

#ifndef __TITANIA_COMPOSED_WIDGETS_NAME_ENTRY_H__
#define __TITANIA_COMPOSED_WIDGETS_NAME_ENTRY_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class NameEntry :
	public X3DComposedWidget
{
public:

	///  @name Construction

	NameEntry (X3DBaseInterface* const,
	           Gtk::Entry &,
	           Gtk::Button &);

	///  @name Member access

	void
	setNode (const X3D::SFNode &);

	const X3D::SFNode &
	getNode ()
	{ return node; }

	const X3D::SFTime &
	name_changed ()
	{ return nameOutput; }

	///  @name Destruction

	virtual
	~NameEntry () final override
	{ dispose (); }


private:

	///  @name Event handlers

	void
	on_insert_text (const Glib::ustring &, int*);

	void
	on_delete_text (int, int);

	bool
	on_key_press_event (GdkEventKey*);

	void
	on_clicked ();

	void
	set_name ();

	///  @name Members

	Gtk::Entry &  entry;
	Gtk::Button & button;
	X3D::SFNode   node;
	X3D::SFTime   nameOutput;

};

} // puck
} // titania

#endif
