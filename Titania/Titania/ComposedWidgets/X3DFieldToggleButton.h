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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DFIELD_TOGGLE_BUTTON_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DFIELD_TOGGLE_BUTTON_H__

#include "../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

template <class Type>
class X3DFieldToggleButton :
	public X3DEditorObject
{
public:

	///  @name Construction

	X3DFieldToggleButton (BrowserWindow* const,
	                      Gtk::ToggleButton &,
	                      const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	///  @name Destruction
	
	virtual
	~X3DFieldToggleButton ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_toggled ();

	void
	set_field ();

	void
	connect (const Type &);

	///  @name Members

	Gtk::ToggleButton & toggleButton;
	X3D::MFNode         nodes;
	const std::string   name;
	UndoStepPtr         undoStep;
	bool                changing;

};

template <class Type>
X3DFieldToggleButton <Type>::X3DFieldToggleButton (BrowserWindow* const browserWindow,
                                                   Gtk::ToggleButton & toggleButton,
                                                   const std::string & name) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DEditorObject (),
	    toggleButton (toggleButton),
	           nodes (),
	            name (name),
	        undoStep (),
	        changing (false)
{
	toggleButton .signal_toggled () .connect (sigc::mem_fun (*this, &X3DFieldToggleButton::on_toggled));
	setup ();
}

template <class Type>
void
X3DFieldToggleButton <Type>::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <Type> (name) .removeInterest (this, &X3DFieldToggleButton::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <Type> (name) .addInterest (this, &X3DFieldToggleButton::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type>
void
X3DFieldToggleButton <Type>::on_toggled ()
{
	if (changing)
		return;

	toggleButton .set_inconsistent (false);

	addUndoFunction <Type> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <Type> (name);

			field .removeInterest (this, &X3DFieldToggleButton::set_field);
			field .addInterest (this, &X3DFieldToggleButton::connect);

			field = toggleButton .get_active ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <Type> (nodes, name, undoStep);
}

template <class Type>
void
X3DFieldToggleButton <Type>::set_field ()
{
	changing = true;

	const int  active   = getBoolean (nodes, name);
	const bool hasField = (active not_eq -2);

	toggleButton .set_sensitive (hasField);
	toggleButton .set_active (active > 0);
	toggleButton .set_inconsistent (active < 0);

	changing = false;
}

template <class Type>
void
X3DFieldToggleButton <Type>::connect (const Type & field)
{
	field .removeInterest (this, &X3DFieldToggleButton::connect);
	field .addInterest (this, &X3DFieldToggleButton::set_field);
}

} // puck
} // titania

#endif
