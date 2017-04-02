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

#include "MFStringEntry.h"

namespace titania {
namespace puck {

MFStringEntry::MFStringEntry (X3DBaseInterface* const editor,
                              Gtk::Box & box,
                              Gtk::Button & addButton,
                              const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	              box (box),
	        addButton (addButton),
	            nodes (),
	             name (name),
	          spacing (2),
	           filter ([ ] (const std::string &) { return true; }),
	           entrys (),
	            index (-1),
	           string (),
	     defaultValue (),
	         undoStep (),
	         changing (false),
	           buffer ()
{
	addChildObjects (nodes, buffer);

	buffer .addInterest (&MFStringEntry::set_buffer, this);

	addButton .signal_clicked () .connect (sigc::mem_fun (*this, &MFStringEntry::on_add_before_clicked));

	setup ();
}

void
MFStringEntry::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .removeInterest (&MFStringEntry::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFString> (name) .addInterest (&MFStringEntry::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
MFStringEntry::on_add_before_clicked ()
{
	// Add value.

	addWidget (0, defaultValue);

	// Set node value.

	undoStep .reset ();

	on_string_changed ();
}

void
MFStringEntry::on_add_clicked (Gtk::Entry* const entry)
{
	// Add value.

	addWidget (getIndex (entry) + 1, defaultValue);

	// Set node value.

	undoStep .reset ();

	on_string_changed ();
}

void
MFStringEntry::on_remove_clicked (Gtk::Entry* const entry)
{
	// Remove Entry.

	removeWidget (getIndex (entry));

	// Set node value.

	undoStep .reset ();

	on_string_changed ();
}

void
MFStringEntry::on_insert_text (const Glib::ustring & insert, int* position, Gtk::Entry* const entry)
{
	const auto text = entry -> get_text () .insert (*position, insert);

	if (not filter (text))
		entry -> signal_insert_text () .emission_stop ();
}

void
MFStringEntry::on_delete_text (int start_pos, int end_pos, Gtk::Entry* const entry)
{
	const auto text = entry -> get_text () .erase (start_pos, end_pos - start_pos);

	if (text .length () and not filter (text))
		entry -> signal_delete_text () .emission_stop ();
}

void
MFStringEntry::on_changed (Gtk::Entry* const entry)
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	// Get Entry index, reset undo step if needed, and set value.

	const auto currentIndex = getIndex (entry);

	if (currentIndex not_eq index)
		undoStep .reset ();

	index = currentIndex;

	string .set1Value (index, entry -> get_text ());

	on_string_changed ();
}

void
MFStringEntry::on_string_changed ()
{
	// Set value.

	addUndoFunction <X3D::MFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFString> (name);

			field .removeInterest (&MFStringEntry::set_field, this);
			field .addInterest (&MFStringEntry::connect, this);

			field = string;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFString> (nodes, name, undoStep);
}

void
MFStringEntry::set_field ()
{
	buffer .addEvent ();
}

void
MFStringEntry::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last field.

	const auto pair   = getArray <X3D::MFString> (nodes, name);
	const auto string = pair .first;

	if (pair .second > -2)
	{
		// Set Entrys.

		size_t i = 0;

		entrys .resize (std::min (entrys .size (), string .size ()));

		for (const size_t size = entrys .size (); i < size; ++ i)
		{
			entrys [i] -> set_text (string [i]);
		}

		// Add widgets.

		for (const size_t size = string .size (); i < size; ++ i)
			addWidget (i, string [i]);

		// Remove widgets.

		const auto children = box .get_children ();

		for (const size_t size = children .size (); i < size; ++ i)
			box .remove (*children [i]);
	}
	else
	{
		// Clear box
	}

	box       .set_sensitive (pair .second not_eq -2);
	addButton .set_sensitive (pair .second not_eq -2);

	changing = false;
}

void
MFStringEntry::addWidget (const int32_t index, const X3D::SFString & value)
{
	const auto parent = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, spacing));
	const auto entry  = Gtk::manage (new Gtk::Entry ());
	const auto add    = Gtk::manage (new Gtk::Button ());
	const auto remove = Gtk::manage (new Gtk::Button ());

	string .insert (string .begin () + index, value);
	entrys .insert (entrys .begin () + index, entry);

	entry -> signal_insert_text () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringEntry::on_insert_text), entry), false);
	entry -> signal_delete_text () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringEntry::on_delete_text), entry), false);
	entry -> signal_changed ()     .connect (sigc::bind (sigc::mem_fun (*this, &MFStringEntry::on_changed),     entry));

	add    -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringEntry::on_add_clicked),    entry));
	remove -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (*this, &MFStringEntry::on_remove_clicked), entry));

	parent -> set_hexpand_set (true);
	entry  -> set_hexpand_set (true);
	entry  -> set_text (value);
	add    -> set_image_from_icon_name ("gtk-add",    Gtk::ICON_SIZE_MENU);
	remove -> set_image_from_icon_name ("gtk-remove", Gtk::ICON_SIZE_MENU);

	parent -> pack_start (*entry,  true,  true);
	parent -> pack_start (*add,    false, true);
	parent -> pack_start (*remove, false, true);

	box .pack_start (*parent, false, true);
	box .reorder_child (*parent, index);

	parent -> show_all ();
}

void
MFStringEntry::removeWidget (const int32_t index)
{
	entrys .erase (entrys .begin () + index);
	string .erase (string .begin () + index);

	box .remove (*box .get_children () [index]);
}

int32_t
MFStringEntry::getIndex (Gtk::Entry* const entry) const
{
	return std::find (entrys .begin (), entrys .end (), entry) - entrys .begin (); 
}

void
MFStringEntry::connect (const X3D::MFString & field)
{
	field .removeInterest (&MFStringEntry::connect, this);
	field .addInterest (&MFStringEntry::set_field, this);
}

} // puck
} // titania
