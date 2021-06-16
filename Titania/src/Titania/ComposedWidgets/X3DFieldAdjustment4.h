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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT4_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DFIELD_ADJUSTMENT4_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

template <class Type>
class X3DFieldAdjustment4 :
	public X3DComposedWidget
{
public:

	///  @name Construction

	X3DFieldAdjustment4 (X3DBaseInterface* const,
	                     const Glib::RefPtr <Gtk::Adjustment> &,
	                     const Glib::RefPtr <Gtk::Adjustment> &,
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
			set_buffer ();
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
	{
		uniform = value;

		undoStep .reset ();
	}

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
	~X3DFieldAdjustment4 () final override
	{ dispose (); }


private:

	///  @name Event handlers

	void
	set_scene ();

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
	X3D::Vector4d
	get_value (const X3D::X3DField <ValueType> &) const;

	template <class ValueType>
	X3D::Vector4d
	get_value (X3D::X3DArrayField <ValueType> &);

	void
	set_bounds ();

	void
	connect (const Type &);

	///  @name Members

	const std::vector <Glib::RefPtr <Gtk::Adjustment>>  adjustments;
	Gtk::Widget &                                       widget;
	X3D::X3DScenePtr                                    scene;
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
X3DFieldAdjustment4 <Type>::X3DFieldAdjustment4 (X3DBaseInterface* const editor,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment1,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment2,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment3,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment4,
                                                 Gtk::Widget & widget,
                                                 const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getMasterBrowser ()),
	X3DComposedWidget (editor),
	      adjustments ({ adjustment1, adjustment2, adjustment3, adjustment4 }),
	           widget (widget),
	            scene (),
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
	addChildObjects (scene, nodes, buffer);

	setup ();

	buffer               .addInterest (&X3DFieldAdjustment4::set_buffer, this);
	getCurrentContext () .addInterest (&X3DFieldAdjustment4::set_field, this);

	adjustments [0] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DFieldAdjustment4::on_value_changed), 0));
	adjustments [1] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DFieldAdjustment4::on_value_changed), 1));
	adjustments [2] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DFieldAdjustment4::on_value_changed), 2));
	adjustments [3] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DFieldAdjustment4::on_value_changed), 3));

	getCurrentScene () .addInterest (&X3DFieldAdjustment4 <Type>::set_scene, this);

	set_scene ();
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::set_scene ()
{
	// Connect units.

	if (scene)
		scene -> units_changed () .removeInterest (&X3DFieldAdjustment4 <Type>::set_field, this);

	scene = getCurrentScene ();

	scene -> units_changed () .addInterest (&X3DFieldAdjustment4 <Type>::set_field, this);
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::setNodes (const X3D::MFNode & value)
{
	// Connect field.

	for (const auto & node : nodes)
	{
		try
		{
			node -> template getField <Type> (name) .removeInterest (&X3DFieldAdjustment4::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> template getField <Type> (name) .addInterest (&X3DFieldAdjustment4::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::on_value_changed (const int index0)
{
	if (nodes .empty ())
		return;

	if (changing)
		return;

	if (index0 not_eq input)
		undoStep .reset ();

	input = index0;

	addUndoFunction <Type> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> template getField <Type> (name);

			field .removeInterest (&X3DFieldAdjustment4::set_field, this);
			field .addInterest (&X3DFieldAdjustment4::connect, this);

			X3D::Vector4d vector (getCurrentScene () -> fromUnit (unit, adjustments [0] -> get_value ()),
			                      getCurrentScene () -> fromUnit (unit, adjustments [1] -> get_value ()),
			                      getCurrentScene () -> fromUnit (unit, adjustments [2] -> get_value ()),
			                      getCurrentScene () -> fromUnit (unit, adjustments [3] -> get_value ()));

			if (normalize)
				vector .normalize ();

			if (uniform)
			{
				changing = true;

				const auto current = get_value (field);
				const auto scale   = vector [index0] / current [index0];
				const auto index1  = (index0 + 1) % 4;
				const auto index2  = (index0 + 2) % 4;
				const auto index3  = (index0 + 3) % 4;

				if (field .get1Value (index0))
				{
					vector [index1] = current [index1] * scale;
					vector [index2] = current [index2] * scale;
					vector [index3] = current [index3] * scale;
				}
				else
				{
					vector [index1] = vector [index0];
					vector [index2] = vector [index0];
					vector [index3] = vector [index0];
				}

				adjustments [index1] -> set_value (getCurrentScene () -> toUnit (unit, vector [index1]));
				adjustments [index2] -> set_value (getCurrentScene () -> toUnit (unit, vector [index2]));
				adjustments [index3] -> set_value (getCurrentScene () -> toUnit (unit, vector [index3]));

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
X3DFieldAdjustment4 <Type>::set_value (X3D::X3DField <ValueType> & field, const Value & value) const
{
	field = value;
}

template <class Type>
template <class ValueType, class Value>
void
X3DFieldAdjustment4 <Type>::set_value (X3D::X3DArrayField <ValueType> & field, const Value & value) const
{
	field .set1Value (index, value);
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::set_field ()
{
	buffer .addEvent ();
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last ?creaseAngle? field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto &     field = node -> template getField <Type> (name);
			const auto index = get_index (field);

			unit = field .getUnit ();

			if (index >= 0)
			{
				const auto value = get_value (field);

				set_bounds ();

				adjustments [0] -> set_value (getCurrentScene () -> toUnit (unit, value [0]));
				adjustments [1] -> set_value (getCurrentScene () -> toUnit (unit, value [1]));
				adjustments [2] -> set_value (getCurrentScene () -> toUnit (unit, value [2]));
				adjustments [3] -> set_value (getCurrentScene () -> toUnit (unit, value [3]));
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

		adjustments [0] -> set_value (getCurrentScene () -> toUnit (unit, empty [0]));
		adjustments [1] -> set_value (getCurrentScene () -> toUnit (unit, empty [1]));
		adjustments [2] -> set_value (getCurrentScene () -> toUnit (unit, empty [2]));
		adjustments [3] -> set_value (getCurrentScene () -> toUnit (unit, empty [3]));
	}

	widget .set_sensitive (hasField);

	changing = false;
}

template <class Type>
template <class ValueType>
int32_t
X3DFieldAdjustment4 <Type>::get_index (X3D::X3DArrayField <ValueType> & field) const
{
	if (index < (int32_t) field .size ())
		return index;

	return -1;
}

template <class Type>
template <class ValueType>
X3D::Vector4d
X3DFieldAdjustment4 <Type>::get_value (const X3D::X3DField <ValueType> & field) const
{
	return X3D::Vector4d (field .getValue ());
}

template <class Type>
template <class ValueType>
X3D::Vector4d
X3DFieldAdjustment4 <Type>::get_value (X3D::X3DArrayField <ValueType> & field)
{
	if (index >= 0 and index < field .size ())
		return X3D::Vector4d (field .get1Value (index));

	return X3D::Vector4d ();
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::set_bounds ()
{
	if (lower .empty ())
	{
		for (const auto & adjustment : adjustments)
		{
			lower .emplace_back (adjustment -> get_lower ());
			upper .emplace_back (adjustment -> get_upper ());
			empty .emplace_back (adjustment -> get_value ());
		}
	}

	for (size_t i = 0, size = adjustments .size (); i < size; ++ i)
	{
		adjustments [i] -> set_lower (getCurrentScene () -> toUnit (unit, lower [i]));
		adjustments [i] -> set_upper (getCurrentScene () -> toUnit (unit, upper [i]));
	}
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::connect (const Type & field)
{
	field .removeInterest (&X3DFieldAdjustment4::connect, this);
	field .addInterest (&X3DFieldAdjustment4::set_field, this);
}

} // puck
} // titania

#endif
