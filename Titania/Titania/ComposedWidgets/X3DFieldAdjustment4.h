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
	~X3DFieldAdjustment4 ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_value_changed (const int);

	void
	set_field ();

	void
	set_buffer ();

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

};

template <class Type>
X3DFieldAdjustment4 <Type>::X3DFieldAdjustment4 (X3DBaseInterface* const editor,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment1,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment2,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment3,
                                                 const Glib::RefPtr <Gtk::Adjustment> & adjustment4,
                                                 Gtk::Widget & widget,
                                                 const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getBrowser ()),
	X3DComposedWidget (editor),
	      adjustments ({ adjustment1, adjustment2, adjustment3, adjustment4 }),
	           widget (widget),
	            nodes (),
	             name (name),
	            index (0),
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer (),
	        normalize (false),
	          uniform (false),
	             unit (X3D::UnitCategory::NONE),
	            lower (),
	            upper ()
{
	addChildren (buffer);

	setup ();

	buffer                 .addInterest (this, &X3DFieldAdjustment4::set_buffer);
	getExecutionContext () .addInterest (this, &X3DFieldAdjustment4::set_field);

	adjustments [0] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment4::on_value_changed), 0));
	adjustments [1] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment4::on_value_changed), 1));
	adjustments [2] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment4::on_value_changed), 2));
	adjustments [3] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DFieldAdjustment4::on_value_changed), 3));
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::setNodes (const X3D::MFNode & value)
{
	if (lower .empty ())
	{
		for (const auto adjustment : adjustments)
		{
			lower .emplace_back (adjustment -> get_lower ());
			upper .emplace_back (adjustment -> get_upper ());
		}
	}
	
	for (const auto & node : nodes)
	{
		try
		{
			node -> getRootContext () -> units_changed () .removeInterest (this, &X3DFieldAdjustment4::set_field);
			node -> getField <Type> (name)                .removeInterest (this, &X3DFieldAdjustment4::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getRootContext () -> units_changed () .addInterest (this, &X3DFieldAdjustment4::set_field);
			node -> getField <Type> (name)                .addInterest (this, &X3DFieldAdjustment4::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::on_value_changed (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <Type> (nodes, name, undoStep);

	const auto scene = getExecutionContext () -> getRootContext ();

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <Type> (name);

			field .removeInterest (this, &X3DFieldAdjustment4::set_field);
			field .addInterest (this, &X3DFieldAdjustment4::connect);

			X3D::Vector4d vector (scene -> toUnit (unit, adjustments [0] -> get_value ()),
			                      scene -> toUnit (unit, adjustments [1] -> get_value ()),
			                      scene -> toUnit (unit, adjustments [2] -> get_value ()),
			                      scene -> toUnit (unit, adjustments [3] -> get_value ()));

			if (normalize)
				vector .normalize ();

			if (uniform)
			{
				changing = true;

				const auto scale  = vector [id] / field .get1Value (id);
				const auto index1 = (id + 1) % 4;
				const auto index2 = (id + 2) % 4;
				const auto index3 = (id + 3) % 4;

				if (field .get1Value (id))
				{
					vector [index1] *= scale;
					vector [index2] *= scale;
					vector [index3] *= scale;
				}
				else
				{
					vector [index1] = vector [id];
					vector [index2] = vector [id];
					vector [index3] = vector [id];
				}

				adjustments [index1] -> set_value (scene -> fromUnit (unit, vector [index1]));
				adjustments [index2] -> set_value (scene -> fromUnit (unit, vector [index2]));
				adjustments [index3] -> set_value (scene -> fromUnit (unit, vector [index3]));

				changing = false;
			}

			field .set1Value (index + 0, vector .x ());
			field .set1Value (index + 1, vector .y ());
			field .set1Value (index + 2, vector .z ());
			field .set1Value (index + 3, vector .w ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <Type> (nodes, name, undoStep);
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

	// Find last ªcreaseAngle´ field.

	const auto scene = getExecutionContext () -> getRootContext ();

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

				adjustments [0] -> set_value (scene -> fromUnit (unit, field .get1Value (index + 0)));
				adjustments [1] -> set_value (scene -> fromUnit (unit, field .get1Value (index + 1)));
				adjustments [2] -> set_value (scene -> fromUnit (unit, field .get1Value (index + 2)));
				adjustments [3] -> set_value (scene -> fromUnit (unit, field .get1Value (index + 3)));

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

		adjustments [0] -> set_value (adjustments [0] -> get_lower () / 2 + adjustments [0] -> get_upper () / 2);
		adjustments [1] -> set_value (adjustments [1] -> get_lower () / 2 + adjustments [1] -> get_upper () / 2);
		adjustments [2] -> set_value (adjustments [2] -> get_lower () / 2 + adjustments [2] -> get_upper () / 2);
		adjustments [3] -> set_value (adjustments [3] -> get_lower () / 2 + adjustments [3] -> get_upper () / 2);
	}

	widget .set_sensitive (hasField);

	changing = false;
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::set_bounds ()
{
	const auto scene = getExecutionContext () -> getRootContext ();

	for (size_t i = 0, size = adjustments .size (); i < size; ++ i)
		adjustments [i] -> set_lower (scene -> fromUnit (unit, lower [i]));

	for (size_t i = 0, size = adjustments .size (); i < size; ++ i)
		adjustments [i] -> set_upper (scene -> fromUnit (unit, upper [i]));
}

template <class Type>
void
X3DFieldAdjustment4 <Type>::connect (const Type & field)
{
	field .removeInterest (this, &X3DFieldAdjustment4::connect);
	field .addInterest (this, &X3DFieldAdjustment4::set_field);
}

} // puck
} // titania

#endif
