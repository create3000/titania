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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT2_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT2_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

template <class Type>
class X3DFieldAdjustment2 :
	public X3DComposedWidget
{
public:

	///  @name Construction

	X3DFieldAdjustment2 (X3DBaseInterface* const,
	                     const Glib::RefPtr <Gtk::Adjustment> &,
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
	setNormalize (const bool value)
	{ normalize = value; }

	bool
	getNormalize () const
	{ return normalize; }

	void
	setUniform (const bool value)
	{ uniform = value; }

	bool
	getUniform () const
	{ return uniform; }

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~X3DFieldAdjustment2 ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_value_changed (const int);

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
	int32_t
	get_index (const X3D::X3DField <ValueType> & field) const
	{ return 0; }
	
	template <class ValueType>
	int32_t
	get_index (X3D::X3DArrayField <ValueType> & field) const;

	template <class ValueType>
	X3D::Vector2d
	get_value (const X3D::X3DField <ValueType> &) const;
	
	template <class ValueType>
	X3D::Vector2d
	get_value (X3D::X3DArrayField <ValueType> &);

	void
	set_bounds ();

	void
	connect (const Type &);

	///  @name Members

	const std::vector <Glib::RefPtr <Gtk::Adjustment>>  adjustments;
	Gtk::Widget &                                       widget;
	X3D::MFNode                                         nodes;
	const std::string                                   name;
	int                                                 index;
	X3D::UndoStepPtr                                    undoStep;
	int                                                 input;
	bool                                                changing;
	X3D::SFTime                                         buffer;
	bool                                                normalize;
	bool                                                uniform;
	X3D::UnitCategory                                   unit;
	std::vector <double>                                lower;
	std::vector <double>                                upper;
	std::vector <double>                                empty;

};

template <class Type>
X3DFieldAdjustment2 <Type>::X3DFieldAdjustment2 (X3DBaseInterface* const editor,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment1,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment2,
                                                 Gtk::Widget & widget,
                                                 const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	      adjustments ({ adjustment1, adjustment2 }),
	           widget (widget),
	            nodes (),
	             name (name),
	            index (-1),
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer (),
	        normalize (false),
	          uniform (false),
	             unit (X3D::UnitCategory::NONE),
	            lower (),
	            upper (),
               empty ()
{
	addChildren (buffer);

	setup ();

	buffer               .addInterest (this, &X3DFieldAdjustment2::set_buffer);
	getCurrentContext () .addInterest (this, &X3DFieldAdjustment2::set_field);

	adjustments [0] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment2::on_value_changed), 0));
	adjustments [1] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment2::on_value_changed), 1));
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::setNodes (const X3D::MFNode & value)
{
	if (lower .empty ())
	{
		for (const auto adjustment : adjustments)
		{
			lower .emplace_back (adjustment -> get_lower ());
			upper .emplace_back (adjustment -> get_upper ());
			empty .emplace_back (adjustment -> get_value ());
		}
	}
	
	for (const auto & node : nodes)
	{
		try
		{
			node -> getScene () -> units_changed () .removeInterest (this, &X3DFieldAdjustment2::set_field);
			node -> getField <Type> (name)          .removeInterest (this, &X3DFieldAdjustment2::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getScene () -> units_changed () .addInterest (this, &X3DFieldAdjustment2::set_field);
			node -> getField <Type> (name)          .addInterest (this, &X3DFieldAdjustment2::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::on_value_changed (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <Type> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <Type> (name);

			field .removeInterest (this, &X3DFieldAdjustment2::set_field);
			field .addInterest (this, &X3DFieldAdjustment2::connect);

			X3D::Vector2d vector (getCurrentScene () -> toBaseUnit (unit, adjustments [0] -> get_value ()),
			                      getCurrentScene () -> toBaseUnit (unit, adjustments [1] -> get_value ()));

			if (normalize)
				vector .normalize ();

			if (uniform)
			{
				changing = true;

				const auto scale  = vector [id] / field .get1Value (id);
				const auto index1 = (id + 1) % 2;

				if (field .get1Value (id))
					vector [index1] *= scale;
				else
					vector [index1] = vector [id];

				adjustments [index1] -> set_value (getCurrentScene () -> fromBaseUnit (unit, vector [index1]));

				changing = false;
			}

			set_value (field, vector);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <Type> (nodes, name, undoStep);
}

template <class Type>
template <class ValueType, class Value>
void
X3DFieldAdjustment2 <Type>::set_value (X3D::X3DField <ValueType> & field, const Value & value) const
{
	field = value;
}

template <class Type>
template <class ValueType, class Value>
void
X3DFieldAdjustment2 <Type>::set_value (X3D::X3DArrayField <ValueType> & field, const Value & value) const
{
	field .set1Value (index, value);
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::set_field ()
{
	buffer .addEvent ();
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last ªcreaseAngle´ field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto &     field = node -> getField <Type> (name);
			const auto value = get_value (field);

			unit  = field .getUnit ();
			index = get_index (field);

			if (index >= 0)
			{
				set_bounds ();

				adjustments [0] -> set_value (getCurrentScene () -> fromBaseUnit (unit, value [0]));
				adjustments [1] -> set_value (getCurrentScene () -> fromBaseUnit (unit, value [1]));
			}

			hasField = (index >= 0);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
	{
		unit = X3D::UnitCategory::NONE;

		set_bounds ();

		adjustments [0] -> set_value (getCurrentScene () -> fromBaseUnit (unit, empty [0]));
		adjustments [1] -> set_value (getCurrentScene () -> fromBaseUnit (unit, empty [1]));
	}

	widget .set_sensitive (hasField);

	changing = false;
}

template <class Type>
template <class ValueType>
int32_t
X3DFieldAdjustment2 <Type>::get_index (X3D::X3DArrayField <ValueType> & field) const
{
	if (field .empty ())
		return -1;

	return std::min <int32_t> (index, field .size () - 1);
}

template <class Type>
template <class ValueType>
X3D::Vector2d
X3DFieldAdjustment2 <Type>::get_value (const X3D::X3DField <ValueType> & field) const
{
	return X3D::Vector2d (field .getValue ());
}

template <class Type>
template <class ValueType>
X3D::Vector2d
X3DFieldAdjustment2 <Type>::get_value (X3D::X3DArrayField <ValueType> & field)
{
	if (index >= (int32_t) field .size ())
		index = field .size () - 1;

	return X3D::Vector2d (field .get1Value (index));
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::set_bounds ()
{
	for (size_t i = 0, size = adjustments .size (); i < size; ++ i)
	{
		adjustments [i] -> set_lower (getCurrentScene () -> fromBaseUnit (unit, lower [i]));
		adjustments [i] -> set_upper (getCurrentScene () -> fromBaseUnit (unit, upper [i]));
	}
}

template <class Type>
void
X3DFieldAdjustment2 <Type>::connect (const Type & field)
{
	field .removeInterest (this, &X3DFieldAdjustment2::connect);
	field .addInterest (this, &X3DFieldAdjustment2::set_field);
}

} // puck
} // titania

#endif
