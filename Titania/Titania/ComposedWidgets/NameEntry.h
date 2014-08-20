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

#ifndef __TITANIA_COMPOSED_WIDGETS_NAME_ENTRY_H__
#define __TITANIA_COMPOSED_WIDGETS_NAME_ENTRY_H__

#include "../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

class NameEntry :
	public X3DEditorObject
{
public:

	///  @name Construction

	NameEntry (BrowserWindow* const browserWindow,
	           Gtk::Entry &,
	           Gtk::Button &);

	///  @name Member access

	void
	setNode (const X3D::SFNode &);

	const X3D::SFNode &
	getNode ()
	{ return node; }

	///  @name Destruction

	virtual
	~NameEntry ()
	{ dispose (); }


private:

	///  @name Event handler

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

	static
	void
	updateNamedNode (BrowserWindow* const, const std::string &, const X3D::SFNode &);

	///  @name Members

	Gtk::Entry &  entry;
	Gtk::Button & button;
	X3D::SFNode   node;

};

inline
NameEntry::NameEntry (BrowserWindow* const browserWindow,
                      Gtk::Entry & entry,
                      Gtk::Button & button) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DEditorObject (),
	           entry (entry),
	          button (button),
	            node ()
{
	entry  .signal_insert_text ()     .connect (sigc::mem_fun (*this, &NameEntry::on_insert_text), false);
	entry  .signal_delete_text ()     .connect (sigc::mem_fun (*this, &NameEntry::on_delete_text), false);
	entry  .signal_key_press_event () .connect (sigc::mem_fun (*this, &NameEntry::on_key_press_event), false);
	button .signal_clicked ()         .connect (sigc::mem_fun (*this, &NameEntry::on_clicked));

	entry  .set_sensitive (false);
	button .set_sensitive (false);

	setup ();
}

inline
void
NameEntry::setNode (const X3D::SFNode & value)
{
	if (node)
		node -> name_changed () .removeInterest (this, &NameEntry::set_name);

	node = value;

	if (node)
	{
		node -> name_changed () .addInterest (this, &NameEntry::set_name);
		set_name ();
	}
	else
		entry .set_text ("");

	entry  .set_sensitive (node);
	button .set_sensitive (node);
}

inline
void
NameEntry::on_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (entry, text, *position);
}

inline
void
NameEntry::on_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (entry, start_pos, end_pos);
}

inline
bool
NameEntry::on_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
	{
		on_clicked ();
		return true;
	}

	return false;
}

inline
void
NameEntry::on_clicked ()
{
	const std::string name = entry .get_text ();

	if (name not_eq node -> getName ())
	{
		const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Rename %s"), node -> getTypeName () .c_str ()));

		undoStep -> addUndoFunction (&NameEntry::updateNamedNode,
		                             getBrowserWindow (),
		                             node -> getName (),
		                             node);

		undoStep -> addRedoFunction (&NameEntry::updateNamedNode,
		                             getBrowserWindow (),
		                             name,
		                             node);

		updateNamedNode (getBrowserWindow (), name, node);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
}

inline
void
NameEntry::set_name ()
{
	entry .set_text (node -> getName ());
}

inline
void
NameEntry::updateNamedNode (BrowserWindow* const browserWindow, const std::string & name, const X3D::SFNode & node)
{
	node -> getExecutionContext () -> removeNamedNode (node -> getName ());

	if (not name .empty ())
		node -> getExecutionContext () -> updateNamedNode (node -> getExecutionContext () -> getUniqueName (name), node);
}

} // puck
} // titania

#endif
