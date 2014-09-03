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

#ifndef __TITANIA_COMPOSED_WIDGETS_SFSTRING_ENTRY_H__
#define __TITANIA_COMPOSED_WIDGETS_SFSTRING_ENTRY_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class SFStringEntry :
	public X3DComposedWidget
{
public:

	///  @name Construction

	SFStringEntry (X3DBrowserWindow* const,
	               Gtk::Entry &,
	               const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~SFStringEntry ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_changed ();

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const X3D::SFString &);

	///  @name Members

	Gtk::Entry &      entry;
	X3D::MFNode       nodes;
	const std::string name;
	UndoStepPtr       undoStep;
	bool              changing;
	X3D::SFTime       buffer;

};

inline
SFStringEntry::SFStringEntry (X3DBrowserWindow* const browserWindow,
                              Gtk::Entry & entry,
                              const std::string & name) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DComposedWidget (),
	           entry (entry),
	           nodes (),
	            name (name),
	        undoStep (),
	        changing (false),
	          buffer ()
{
	addChildren (buffer);
	buffer .addInterest (this, &SFStringEntry::set_buffer);

	entry .signal_changed () .connect (sigc::mem_fun (*this, &SFStringEntry::on_changed));
	setup ();
}

inline
void
SFStringEntry::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

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

inline
void
SFStringEntry::on_changed ()
{
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

inline
void
SFStringEntry::set_field ()
{
	buffer .addEvent ();
}

inline
void
SFStringEntry::set_buffer ()
{
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

inline
void
SFStringEntry::connect (const X3D::SFString & field)
{
	field .removeInterest (this, &SFStringEntry::connect);
	field .addInterest (this, &SFStringEntry::set_field);
}

} // puck
} // titania

#endif
