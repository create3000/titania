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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DFIELD_TOGGLE_BUTTON_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DFIELD_TOGGLE_BUTTON_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

template <class Type, class ToggleButtonType = Gtk::ToggleButton>
class X3DFieldToggleButton :
	public X3DComposedWidget
{
public:

	///  @name Construction

	X3DFieldToggleButton (X3DBaseInterface* const,
	                      ToggleButtonType &,
	                      const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	const X3D::UndoStepPtr &
	getOwnUndoStep () const
	{ return undoStep; }

	///  @name Destruction

	virtual
	~X3DFieldToggleButton () final override
	{ dispose (); }


private:

	///  @name Event handlers

	void
	on_toggled ();

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const Type &);

	void
	set_inconsistent (const bool);

	///  @name Members

	ToggleButtonType & toggleButton;
	X3D::MFNode        nodes;
	const std::string  name;
	X3D::UndoStepPtr   undoStep;
	bool               changing;
	X3D::SFTime        buffer;

};

template <class Type, class ToggleButtonType>
X3DFieldToggleButton <Type, ToggleButtonType>::X3DFieldToggleButton (X3DBaseInterface* const editor,
                                                   ToggleButtonType & toggleButton,
                                                   const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getMasterBrowser ()),
	X3DComposedWidget (editor),
	     toggleButton (toggleButton),
	            nodes (),
	             name (name),
	         undoStep (),
	         changing (false),
	           buffer ()
{
	addChildObjects (nodes, buffer);

	buffer .addInterest (&X3DFieldToggleButton::set_buffer, this);

	toggleButton .signal_toggled () .connect (sigc::mem_fun (this, &X3DFieldToggleButton::on_toggled));

	setup ();
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <Type> (name) .removeInterest (&X3DFieldToggleButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <Type> (name) .addInterest (&X3DFieldToggleButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::on_toggled ()
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	set_inconsistent (false);

	addUndoFunction <Type> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <Type> (name);

			field .removeInterest (&X3DFieldToggleButton::set_field, this);
			field .addInterest (&X3DFieldToggleButton::connect, this);

			field = toggleButton .get_active ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <Type> (nodes, name, undoStep);
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::set_field ()
{
	buffer .addEvent ();
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	const int  active   = getBoolean (nodes, name);
	const bool hasField = (active not_eq -2);

	toggleButton .set_sensitive (hasField);
	toggleButton .set_active (active > 0);
	set_inconsistent (active < 0);

	changing = false;
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::connect (const Type & field)
{
	field .removeInterest (&X3DFieldToggleButton::connect, this);
	field .addInterest (&X3DFieldToggleButton::set_field, this);
}

template <class Type, class ToggleButtonType>
void
X3DFieldToggleButton <Type, ToggleButtonType>::set_inconsistent (const bool value)
{
	toggleButton .set_inconsistent (value);
}

template <>
inline
void
X3DFieldToggleButton <X3D::SFBool, Gtk::ToggleToolButton>::set_inconsistent (const bool)
{
}

template <>
inline
void
X3DFieldToggleButton <X3D::SFBool, Gtk::RadioToolButton>::set_inconsistent (const bool)
{
}

} // puck
} // titania

#endif
