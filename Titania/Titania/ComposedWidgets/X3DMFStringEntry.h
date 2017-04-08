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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DMFSTRING_ENTRY_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DMFSTRING_ENTRY_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

#include <functional>

namespace titania {
namespace puck {

class X3DMFStringEntry :
	public X3DComposedWidget
{
public:

	///  @name Member types

	using FilterFunction = std::function <bool (const std::string &)>;

	///  @name Member access

	void
	setSpacing (const size_t value)
	{ spacing = value; }

	size_t
	getSpacing () const
	{ return spacing; }

	void
	setFilter (const FilterFunction & value)
	{ filter = value; }

	const FilterFunction &
	getFilter () const
	{ return filter; }

	void
	setNodes (const X3D::MFNode & value);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	const X3D::MFString &
	getString ()
	{ return string; }

	///  @name Destruction

	virtual
	~X3DMFStringEntry () override;


protected:

	///  @name Construction

	X3DMFStringEntry (X3DBaseInterface* const editor,
	                  Gtk::Box & box,
	                  Gtk::Button & addButton,
	                  const std::string & name);

	///  @name Operations

	virtual
	Gtk::Widget*
	getAdditionalWidget (Gtk::Entry* const entry)
	{ return nullptr; }

	///  @name Event handlers

	virtual
	void
	set_buffer ();


private:

	///  @name Event handlers

	bool
	on_focus_in_event (GdkEventFocus* focus_event);

	bool
	on_focus_out_event (GdkEventFocus* gdk_event);

	void
	on_add_before_clicked ();

	void
	on_add_clicked (Gtk::Entry* const entry);

	void
	on_remove_clicked (Gtk::Entry* const entry);

	void
	on_insert_text (const Glib::ustring & insert, int* position, Gtk::Entry* const entry);

	void
	on_delete_text (int start_pos, int end_pos, Gtk::Entry* const entry);

	void
	on_changed (Gtk::Entry* const entry);

	void
	on_string_changed ();

	void
	set_field ();

	void
	addWidget (const int32_t index);

	void
	removeWidget (const int32_t index);

	int32_t
	getIndex (Gtk::Entry* const entry) const;

	void
	connect (const X3D::MFString & field);

	///  @name Members

	Gtk::Box &                box;
	Gtk::Button &             addButton;
	X3D::MFNode               nodes;
	const std::string         name;
	size_t                    spacing;
	FilterFunction            filter;
	std::vector <Gtk::Entry*> entrys;
	X3D::MFString             string;
	X3D::SFString             defaultValue;
	X3D::UndoStepPtr          undoStep;
	bool                      changing;
	X3D::SFTime               buffer;

};

} // puck
} // titania

#endif
