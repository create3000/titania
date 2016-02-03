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

#ifndef __TITANIA_EDITORS_LODEDITOR_LODRANGE_WIDGET_H__
#define __TITANIA_EDITORS_LODEDITOR_LODRANGE_WIDGET_H__

#include "../../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class LODRangeWidget :
	public X3DComposedWidget
{
public:

	///  @name Construction

	LODRangeWidget (X3DBaseInterface* const,
	                const Glib::RefPtr <Gtk::Adjustment> &,
	                const Glib::RefPtr <Gtk::Adjustment> &,
	                Gtk::SpinButton &,
	                Gtk::SpinButton &,
	                Gtk::CheckButton &,
	                Gtk::Widget &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~LODRangeWidget ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_max_toggled (const int);

	void
	on_value_changed (const int);

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const X3D::MFFloat &);

	///  @name Members

	const Glib::RefPtr <Gtk::Adjustment> adjustment1;
	const Glib::RefPtr <Gtk::Adjustment> adjustment2;
	Gtk::SpinButton &                    spinButton1;
	Gtk::SpinButton &                    spinButton2;
	Gtk::CheckButton &                   maxButton;
	Gtk::Widget &                        widget;
	X3D::MFNode                          nodes;
	X3D::UndoStepPtr                          undoStep;
	int                                  input;
	bool                                 changing;
	X3D::SFTime                          buffer;
	bool                                 normalize;

};

inline
LODRangeWidget::LODRangeWidget (X3DBaseInterface* const editor,
                                const Glib::RefPtr <Gtk::Adjustment> & adjustment1,
                                const Glib::RefPtr <Gtk::Adjustment> & adjustment2,
                                Gtk::SpinButton & spinButton1,
                                Gtk::SpinButton & spinButton2,
                                Gtk::CheckButton & maxButton,
                                Gtk::Widget & widget) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	      adjustment1 (adjustment1),
	      adjustment2 (adjustment2),
	      spinButton1 (spinButton1),
	      spinButton2 (spinButton2),
	        maxButton (maxButton),
	           widget (widget),
	            nodes (),
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer (),
	        normalize (false)
{
	addChildren (buffer);
	buffer .addInterest (this, &LODRangeWidget::set_buffer);

	maxButton .signal_toggled () .connect (sigc::bind (sigc::mem_fun (*this, &LODRangeWidget::on_max_toggled), 2));
	adjustment1 -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &LODRangeWidget::on_value_changed), 0));
	adjustment2 -> signal_value_changed () .connect (sigc::bind (sigc::mem_fun (*this, &LODRangeWidget::on_value_changed), 1));

	spinButton1 .set_editable (false);
	setup ();
}

inline
void
LODRangeWidget::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> ("range")         .removeInterest (this, &LODRangeWidget::set_field);
			node -> getField <X3D::SFInt32> ("level_changed") .removeInterest (this, &LODRangeWidget::set_field);
			node -> getField <X3D::MFNode>  ("children")      .removeInterest (this, &LODRangeWidget::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> ("range")         .addInterest (this, &LODRangeWidget::set_field);
			node -> getField <X3D::SFInt32> ("level_changed") .addInterest (this, &LODRangeWidget::set_field);
			node -> getField <X3D::MFNode>  ("children")      .addInterest (this, &LODRangeWidget::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
LODRangeWidget::on_max_toggled (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::MFFloat> (nodes, "range", undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & range = node -> getField <X3D::MFFloat> ("range");
			auto & level = node -> getField <X3D::SFInt32> ("level_changed");

			range .removeInterest (this, &LODRangeWidget::set_field);
			range .addInterest (this, &LODRangeWidget::connect);

			if (maxButton .get_active ())
			{
				range .resize (level);
				spinButton2 .set_visible (false);
			}
			else
			{
				range .resize (level + 1, range .get1Value (level ? level - 1 : 0));
				adjustment2 -> set_value (range .get1Value (level ? level - 1 : 0));
				spinButton2 .set_visible (true);
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFFloat> (nodes, "range", undoStep);
}

inline
void
LODRangeWidget::on_value_changed (const int id)
{
	if (changing)
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::MFFloat> (nodes, "range", undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & range    = node -> getField <X3D::MFFloat> ("range");
			auto & level    = node -> getField <X3D::SFInt32> ("level_changed");
			auto & children = node -> getField <X3D::MFNode> ("children");

			range .removeInterest (this, &LODRangeWidget::set_field);
			range .addInterest (this, &LODRangeWidget::connect);

			if (level < (int32_t) children .size ())
			{
				if (level == 0)
					range .set1Value (0, adjustment2 -> get_value ());

				else
				{
					range .set1Value (level - 1, adjustment1 -> get_value ());
					range .set1Value (level,     adjustment2 -> get_value ());
				}
			}
			else
				range .set1Value (children .size () - 1, adjustment1 -> get_value ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFFloat> (nodes, "range", undoStep);
}

inline
void
LODRangeWidget::set_field ()
{
	buffer .addEvent ();
}

inline
void
LODRangeWidget::set_buffer ()
{
	changing = true;

	// Find last ªcreaseAngle´ field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto & range    = node -> getField <X3D::MFFloat> ("range");
			auto & level    = node -> getField <X3D::SFInt32> ("level_changed");
			auto & children = node -> getField <X3D::MFNode> ("children");

			const int32_t size = std::min (children .size (), range .size ());

			if (level < size)
			{
				if (level == 0)
				{
					adjustment1 -> set_value (0);
					adjustment2 -> set_value (range .get1Value (0));

					spinButton1 .set_editable (false);
					spinButton2 .set_editable (true);
				}
				else
				{
					adjustment1 -> set_value (range .get1Value (level - 1));
					adjustment2 -> set_value (range .get1Value (level));

					spinButton1 .set_editable (true);
					spinButton2 .set_editable (true);
				}

				spinButton2 .set_visible (true);
			}
			else
			{
				adjustment1 -> set_value (size ? range .get1Value (size - 1) : 0);
				spinButton1 .set_editable (true);
				spinButton2 .set_visible (false);
			}

			maxButton .set_sensitive (level < size + 1);
			maxButton .set_active (level >= (int32_t) range .size ());

			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
	{
		adjustment1 -> set_value (adjustment1 -> get_lower ());
		adjustment2 -> set_value (adjustment2 -> get_lower ());
	}

	widget .set_sensitive (hasField);

	changing = false;
}

inline
void
LODRangeWidget::connect (const X3D::MFFloat & field)
{
	field .removeInterest (this, &LODRangeWidget::connect);
	field .addInterest (this, &LODRangeWidget::set_field);
}

} // puck
} // titania

#endif
