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

#ifndef __TITANIA_COMPOSED_WIDGETS_SFSTRING_COMBO_BOX_TEXT_H__
#define __TITANIA_COMPOSED_WIDGETS_SFSTRING_COMBO_BOX_TEXT_H__

#include "../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

class SFStringComboBoxText :
	public X3DEditorObject
{
public:

	///  @name Construction

	SFStringComboBoxText (BrowserWindow* const,
	                      Gtk::ComboBoxText &,
	                      const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction
	
	virtual
	~SFStringComboBoxText ()
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

	Gtk::ComboBoxText & comboBoxText;
	X3D::MFNode         nodes;
	const std::string   name;
	UndoStepPtr         undoStep;
	bool                changing;
	X3D::SFTime         buffer;

};

inline
SFStringComboBoxText::SFStringComboBoxText (BrowserWindow* const browserWindow,
                                            Gtk::ComboBoxText & comboBoxText,
                                            const std::string & name) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DEditorObject (),
	    comboBoxText (comboBoxText),
	           nodes (),
	            name (name),
	        undoStep (),
	        changing (false),
	          buffer ()
{
	addChildren (buffer);
	buffer .addInterest (this, &SFStringComboBoxText::set_buffer);

	comboBoxText .signal_changed () .connect (sigc::mem_fun (*this, &SFStringComboBoxText::on_changed));
	setup ();
}

inline
void
SFStringComboBoxText::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFString> (name) .removeInterest (this, &SFStringComboBoxText::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFString> (name) .addInterest (this, &SFStringComboBoxText::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
SFStringComboBoxText::on_changed ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFString> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFString> (name);

			field .removeInterest (this, &SFStringComboBoxText::set_field);
			field .addInterest (this, &SFStringComboBoxText::connect);

			field = comboBoxText .get_active_text ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFString> (nodes, name, undoStep);
}

inline
void
SFStringComboBoxText::set_field ()
{
	buffer .addEvent ();
}

inline
void
SFStringComboBoxText::set_buffer ()
{
	changing = true;

	const auto pair = getString (nodes, name);

	if (pair .second > 0)
		comboBoxText .set_active_text (pair .first);
	else
		comboBoxText .set_active (-1);

	comboBoxText .set_sensitive (pair .second not_eq -2);

	changing = false;
}

inline
void
SFStringComboBoxText::connect (const X3D::SFString & field)
{
	field .removeInterest (this, &SFStringComboBoxText::connect);
	field .addInterest (this, &SFStringComboBoxText::set_field);
}

} // puck
} // titania

#endif
