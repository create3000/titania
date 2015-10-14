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

#ifndef __TITANIA_COMPOSED_WIDGETS_SFROTATION_ADJUSTMENT_H__
#define __TITANIA_COMPOSED_WIDGETS_SFROTATION_ADJUSTMENT_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class SFRotationAdjustment :
	public X3DComposedWidget
{
public:

	///  @name Construction

	SFRotationAdjustment (X3DBaseInterface* const,
	                      const Glib::RefPtr <Gtk::Adjustment> &,
	                      const Glib::RefPtr <Gtk::Adjustment> &,
	                      const Glib::RefPtr <Gtk::Adjustment> &,
	                      const Glib::RefPtr <Gtk::Adjustment> &,
	                      Gtk::Widget &,
	                      const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~SFRotationAdjustment ()
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
	connect (const X3D::SFRotation &);

	///  @name Members

	const std::vector <Glib::RefPtr <Gtk::Adjustment>>  adjustments;
	Gtk::Widget &                                       widget;
	X3D::MFNode                                         nodes;
	const std::string                                   name;
	X3D::UndoStepPtr                                         undoStep;
	int                                                 input;
	bool                                                changing;
	X3D::SFTime                                         buffer;

};

inline
SFRotationAdjustment::SFRotationAdjustment (X3DBaseInterface* const editor,
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
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer ()
{
	addChildren (buffer);
	buffer .addInterest (this, &SFRotationAdjustment::set_buffer);

	adjustments [0] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &SFRotationAdjustment::on_value_changed), 0));
	adjustments [1] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &SFRotationAdjustment::on_value_changed), 1));
	adjustments [2] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &SFRotationAdjustment::on_value_changed), 2));
	adjustments [3] -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &SFRotationAdjustment::on_value_changed), 3));

	setup ();
}

inline
void
SFRotationAdjustment::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .removeInterest (this, &SFRotationAdjustment::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .addInterest (this, &SFRotationAdjustment::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
SFRotationAdjustment::on_value_changed (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::SFRotation> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFRotation> (name);

			field .removeInterest (this, &SFRotationAdjustment::set_field);
			field .addInterest (this, &SFRotationAdjustment::connect);

			X3D::Rotation4f rotation (adjustments [0] -> get_value (),
			                          adjustments [1] -> get_value (),
			                          adjustments [2] -> get_value (),
			                          adjustments [3] -> get_value ());

			field = rotation;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFRotation> (nodes, name, undoStep);
}

inline
void
SFRotationAdjustment::set_field ()
{
	buffer .addEvent ();
}

inline
void
SFRotationAdjustment::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last ªcreaseAngle´ field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto & field = node -> getField <X3D::SFRotation> (name);

			adjustments [0] -> set_value (field .getX ());
			adjustments [1] -> set_value (field .getY ());
			adjustments [2] -> set_value (field .getZ ());
			adjustments [3] -> set_value (field .getAngle ());

			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
	{
		adjustments [0] -> set_value (adjustments [0] -> get_lower () / 2 + adjustments [0] -> get_upper () / 2);
		adjustments [1] -> set_value (adjustments [1] -> get_lower () / 2 + adjustments [1] -> get_upper () / 2);
		adjustments [2] -> set_value (adjustments [2] -> get_lower () / 2 + adjustments [2] -> get_upper () / 2);
		adjustments [3] -> set_value (adjustments [3] -> get_lower () / 2 + adjustments [3] -> get_upper () / 2);
	}

	widget .set_sensitive (hasField);

	changing = false;
}

inline
void
SFRotationAdjustment::connect (const X3D::SFRotation & field)
{
	field .removeInterest (this, &SFRotationAdjustment::connect);
	field .addInterest (this, &SFRotationAdjustment::set_field);
}

} // puck
} // titania

#endif
