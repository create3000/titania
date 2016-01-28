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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

template <class Type>
class X3DFieldAdjustment :
	public X3DComposedWidget
{
public:

	///  @name Construction

	X3DFieldAdjustment (X3DBaseInterface* const,
	                    const Glib::RefPtr <Gtk::Adjustment> &,
	                    Gtk::Widget &,
	                    const std::string &);

	///  @name Member access

	void
	setIndex (const int value)
	{
		index = value;
		
		if (not nodes .empty ())
			set_field ();
	}

	int
	getIndex () const
	{ return index; }

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~X3DFieldAdjustment ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_value_changed ();

	template <class ValueType, class Value>
	void
	set_value (X3D::X3DField <ValueType> &, const Value &) const;
	
	template <class ValueType, class Value>
	void
	set_value (X3D::X3DArrayField <ValueType> &, const Value &) const;

	void
	set_field ();

	void
	set_buffer ();

	template <class ValueType>
	double
	get_value (const X3D::X3DField <ValueType> &) const;
	
	template <class ValueType>
	double
	get_value (X3D::X3DArrayField <ValueType> &);

	void
	set_bounds ();

	void
	connect (const Type &);

	///  @name Members

	const Glib::RefPtr <Gtk::Adjustment> adjustment;
	Gtk::Widget &                        widget;
	X3D::MFNode                          nodes;
	const std::string                    name;
	X3D::UndoStepPtr                     undoStep;
	bool                                 changing;
	X3D::SFTime                          buffer;
	X3D::UnitCategory                    unit;
	double                               lower;
	double                               upper;
	int32_t                              index;

};

template <class Type>
X3DFieldAdjustment <Type>::X3DFieldAdjustment (X3DBaseInterface* const editor,
                                               const Glib::RefPtr <Gtk::Adjustment> & adjustment,
                                               Gtk::Widget & widget,
                                               const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	       adjustment (adjustment),
	           widget (widget),
	            nodes (),
	             name (name),
	         undoStep (),
	         changing (false),
	           buffer (),
	             unit (X3D::UnitCategory::NONE),
	            lower (0),
	            upper (0),
               index (0)
{
	addChildren (buffer);

	setup ();

	buffer               .addInterest (this, &X3DFieldAdjustment::set_buffer);
	getCurrentContext () .addInterest (this, &X3DFieldAdjustment::set_field);
	   
	adjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DFieldAdjustment::on_value_changed));
}

template <class Type>
void
X3DFieldAdjustment <Type>::setNodes (const X3D::MFNode & value)
{
	if (lower == upper)
	{
		lower = adjustment -> get_lower ();
		upper = adjustment -> get_upper ();
	}

	for (const auto & node : nodes)
	{
		try
		{
			node -> getScene () -> units_changed () .removeInterest (this, &X3DFieldAdjustment::set_field);
			node -> getField <Type> (name)          .removeInterest (this, &X3DFieldAdjustment::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getScene () -> units_changed () .addInterest (this, &X3DFieldAdjustment::set_field);
			node -> getField <Type> (name)          .addInterest (this, &X3DFieldAdjustment::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type>
void
X3DFieldAdjustment <Type>::on_value_changed ()
{
	if (changing)
		return;

	addUndoFunction <Type> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <Type> (name);

			field .removeInterest (this, &X3DFieldAdjustment::set_field);
			field .addInterest (this, &X3DFieldAdjustment::connect);

			set_value (field, getCurrentScene () -> toBaseUnit (unit, adjustment -> get_value ()));
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <Type> (nodes, name, undoStep);
}

template <class Type>
template <class ValueType, class Value>
void
X3DFieldAdjustment <Type>::set_value (X3D::X3DField <ValueType> & field, const Value & value) const
{
	field = value;
}

template <class Type>
template <class ValueType, class Value>
void
X3DFieldAdjustment <Type>::set_value (X3D::X3DArrayField <ValueType> & field, const Value & value) const
{
	field .set1Value (index, value);
}

template <class Type>
void
X3DFieldAdjustment <Type>::set_field ()
{
	buffer .addEvent ();
}

template <class Type>
void
X3DFieldAdjustment <Type>::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last field.

	bool hasField = false;

	if (index >= 0)
	{
		for (const auto & node : basic::make_reverse_range (nodes))
		{
			try
			{
			   auto & field = node -> getField <Type> (name);
	
				unit = field .getUnit ();
	
				set_bounds ();
	
				adjustment -> set_value (getCurrentScene () -> fromBaseUnit (unit, get_value (field)));

				hasField = true;
				break;
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}

	if (not hasField)
	{
		unit = X3D::UnitCategory::NONE;

		set_bounds ();

		adjustment -> set_value (adjustment -> get_lower ());
	}

	widget .set_sensitive (hasField);

	changing = false;
}

template <class Type>
template <class ValueType>
double
X3DFieldAdjustment <Type>::get_value (const X3D::X3DField <ValueType> & field) const
{
	return field;
}

template <class Type>
template <class ValueType>
double
X3DFieldAdjustment <Type>::get_value (X3D::X3DArrayField <ValueType> & field)
{
	if (index >= (int32_t) field .size ())
		index = field .size () - 1;

	return field .get1Value (index);
}

template <class Type>
void
X3DFieldAdjustment <Type>::set_bounds ()
{
	adjustment -> set_lower (getCurrentScene () -> fromBaseUnit (unit, lower));
	adjustment -> set_upper (getCurrentScene () -> fromBaseUnit (unit, upper));
}

template <class Type>
void
X3DFieldAdjustment <Type>::connect (const Type & field)
{
	field .removeInterest (this, &X3DFieldAdjustment::connect);
	field .addInterest (this, &X3DFieldAdjustment::set_field);
}

} // puck
} // titania

#endif
