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

#ifndef __TITANIA_COMPOSED_WIDGETS_SFCOLOR_RGBABUTTON_H__
#define __TITANIA_COMPOSED_WIDGETS_SFCOLOR_RGBABUTTON_H__

#include "../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

inline
void
draw_checker_board (const Cairo::RefPtr <Cairo::Context> & context,
                    const double x, const double y,
                    const double width, const double height,
                    const double w, const double h,
                    const X3D::Color4f & color1,
                    const X3D::Color4f & color2)
{
	const size_t columns = std::ceil (width / w);
	const size_t rows    = std::ceil (height / h);

	for (size_t r = 0; r < rows; ++ r)
	{
		for (size_t c = 0; c < columns; ++ c)
		{
			if (r % 2 - c % 2)
				context -> set_source_rgba (color2 .r (), color2 .g (), color2 .b (), color2 .a ());
			else
				context -> set_source_rgba (color1 .r (), color1 .g (), color1 .b (), color1 .a ());

			const auto sx = x + c * w;
			const auto sy = y + r * h;

			context -> rectangle (sx, sy, w, h);
			context -> fill ();
		}
	}
}

class SFColorRGBAButton :
	public X3DEditorObject
{
public:

	///  @name Construction

	SFColorRGBAButton (BrowserWindow* const,
	                   Gtk::Button &,
	                   const Glib::RefPtr <Gtk::Adjustment> &,
	                   Gtk::Widget &,
	                   const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	///  @name Destruction

	virtual
	~SFColorRGBAButton ()
	{ dispose (); }


private:

	///  @name Event handler

	void
	on_color_changed ();

	void
	on_value_changed ();

	void
	set_color (const X3D::Color4f &);

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const X3D::SFColorRGBA &);

	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> &);

	void
	on_clicked ();

	Gdk::RGBA
	to_rgba (const X3D::Color4f &);

	///  @name Members

	Gtk::Button &                        colorButton;
	const Glib::RefPtr <Gtk::Adjustment> valueAdjustment;
	Gtk::Widget &                        widget;
	Gtk::DrawingArea                     drawingArea;
	Gtk::ColorSelectionDialog            dialog;
	X3D::MFNode                          nodes;
	const std::string                    name;
	UndoStepPtr                          undoStep;
	bool                                 changing;
	X3D::SFTime                          buffer;
	float                                hsva [4];

};

inline
SFColorRGBAButton::SFColorRGBAButton (BrowserWindow* const browserWindow,
                                      Gtk::Button & colorButton,
                                      const Glib::RefPtr <Gtk::Adjustment> & valueAdjustment,
                                      Gtk::Widget & widget,
                                      const std::string & name) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DEditorObject (),
	     colorButton (colorButton),
	 valueAdjustment (valueAdjustment),
	          widget (widget),
	     drawingArea (),
	          dialog (),
	           nodes (),
	            name (name),
	        undoStep (),
	        changing (false),
	          buffer ()
{
	// Buffer

	addChildren (buffer);
	buffer .addInterest (this, &SFColorRGBAButton::set_buffer);

	// Button

	colorButton .signal_clicked () .connect (sigc::mem_fun (*this, &SFColorRGBAButton::on_clicked));
	colorButton .add (drawingArea);

	// Drawing Area

	drawingArea .signal_draw () .connect (sigc::mem_fun (*this, &SFColorRGBAButton::on_draw));
	drawingArea .show ();

	// Value Adjustment

	valueAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &SFColorRGBAButton::on_value_changed));

	// Dialog

	dialog .get_color_selection () -> signal_color_changed () .connect (sigc::mem_fun (*this, &SFColorRGBAButton::on_color_changed));
	dialog .get_color_selection () -> set_has_opacity_control (true);
	dialog .get_color_selection () -> set_has_palette (true);

	dialog .property_ok_button () .get_value () -> hide ();
	dialog .property_cancel_button () .get_value () -> hide ();

	// Setup

	setup ();
}

inline
void
SFColorRGBAButton::setNodes (const X3D::MFNode & value)
{
	undoStep .reset ();

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFColorRGBA> (name) .removeInterest (this, &SFColorRGBAButton::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFColorRGBA> (name) .addInterest (this, &SFColorRGBAButton::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
SFColorRGBAButton::on_color_changed ()
{
	drawingArea .queue_draw ();

	if (changing)
		return;

	const auto rgba  = dialog .get_color_selection () -> get_current_rgba ();
	const auto color = X3D::Color4f (rgba .get_red (), rgba .get_green (), rgba .get_blue (), rgba .get_alpha ());

	color .get_hsv (hsva [0], hsva [1], hsva [2]);
	hsva [3] = color .a ();

	set_color (color);

	changing = true;
	valueAdjustment -> set_value (hsva [2]);
	changing = false;
}

inline
void
SFColorRGBAButton::on_value_changed ()
{
	drawingArea .queue_draw ();

	if (changing)
		return;

	hsva [2] = valueAdjustment -> get_value ();

	X3D::Color4f color;
	color .set_hsv (hsva [0], hsva [1], hsva [2]);
	color .a (hsva [3]);

	set_color (color);

	changing = true;
	dialog .get_color_selection () -> set_current_rgba (to_rgba (color));
	changing = false;
}

inline
void
SFColorRGBAButton::set_color (const X3D::Color4f & color)
{
	addUndoFunction <X3D::SFColorRGBA> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFColorRGBA> (name);

			field .removeInterest (this, &SFColorRGBAButton::set_field);
			field .addInterest (this, &SFColorRGBAButton::connect);

			field = color;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFColorRGBA> (nodes, name, undoStep);
}

inline
void
SFColorRGBAButton::set_field ()
{
	buffer .addEvent ();
}

inline
void
SFColorRGBAButton::set_buffer ()
{
	changing = true;

	// Find last field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::SFColorRGBA> (name);
			const auto   rgba  = to_rgba (field);

			field .getHSV (hsva [0], hsva [1], hsva [2]);
			hsva [3] = field .getA ();

			dialog .get_color_selection () -> set_current_rgba (rgba);
			dialog .get_color_selection () -> set_previous_rgba (rgba);
			valueAdjustment -> set_value (hsva [2]);

			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
		dialog .get_color_selection () -> set_current_rgba (Gdk::RGBA ());

	widget .set_sensitive (hasField);

	changing = false;
}

inline
void
SFColorRGBAButton::connect (const X3D::SFColorRGBA & field)
{
	field .removeInterest (this, &SFColorRGBAButton::connect);
	field .addInterest (this, &SFColorRGBAButton::set_field);
}

inline
bool
SFColorRGBAButton::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	draw_checker_board (context,
	                    0, 0, drawingArea .get_width (), drawingArea .get_height (),
	                    10, 10,
	                    X3D::Color4f (0.8, 0.8, 0.8, 1),
	                    X3D::Color4f (0.2, 0.2, 0.2, 1));

	const auto color = dialog .get_color_selection () -> get_current_rgba ();

	context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
	context -> rectangle (0, 0, drawingArea .get_width (), drawingArea .get_height ());
	context -> fill ();

	if (colorButton .get_style_context () -> get_state () & Gtk::STATE_FLAG_INSENSITIVE)
	{
		draw_checker_board (context,
		                    0, 0, drawingArea .get_width (), drawingArea .get_height (),
		                    1, 1,
		                    X3D::Color4f (0.8, 0.8, 0.8, 0.5),
		                    X3D::Color4f (0.2, 0.2, 0.2, 0.5));
	}

	return true;
}

inline
void
SFColorRGBAButton::on_clicked ()
{
	dialog .present ();
}

inline
Gdk::RGBA
SFColorRGBAButton::to_rgba (const X3D::Color4f & value)
{
	Gdk::RGBA color;

	color .set_rgba (value .r (), value .g (), value .b (), value .a ());
	return color;
}

} // puck
} // titania

#endif
