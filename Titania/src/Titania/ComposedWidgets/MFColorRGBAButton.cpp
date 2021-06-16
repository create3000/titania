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

#include "MFColorRGBAButton.h"

#include "../Bits/Cairo.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>

namespace titania {
namespace puck {

MFColorRGBAButton::MFColorRGBAButton (X3DBaseInterface* const editor,
                                      Gtk::Button & colorButton,
                                      const Glib::RefPtr <Gtk::Adjustment> & valueAdjustment,
                                      Gtk::Widget & widget,
                                      Gtk::Button & addColorButton,
                                      Gtk::Button & removeColorButton,
                                      Gtk::ScrolledWindow & colorsScrolledWindow,
                                      const std::string & name) :
	    X3DBaseInterface (editor -> getBrowserWindow (), editor -> getMasterBrowser ()),
	   X3DComposedWidget (editor),
          index_changed (),
	         colorButton (colorButton),
	     valueAdjustment (valueAdjustment),
	              widget (widget),
	      addColorButton (addColorButton),
	   removeColorButton (removeColorButton),
	colorsScrolledWindow (colorsScrolledWindow),
	         drawingArea (),
	              dialog (),
	   colorsDrawingArea (),
	                menu (),
	               nodes (),
	                node (),
	                name (name),
	               index (0),
                   hide (false),
	            undoStep (),
	               input (-1),
	            changing (false),
	              buffer (),
	                hsva (),
	          colorsSize (32),
	           colorsGap (2),
	        colorsBorder (2, 2, 2, 2),
	           clipboard (new X3D::Clipboard (getMasterBrowser () -> getExecutionContext ()))
{
	addChildObjects (node, nodes, buffer, clipboard);

	// Buffer

	buffer .addInterest (&MFColorRGBAButton::set_buffer, this);

	// Widget

	widget .signal_style_updated () .connect (sigc::mem_fun (colorsDrawingArea, &Gtk::Widget::queue_draw));

	// Button

	colorButton .signal_button_press_event () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_button_press_event));
	colorButton .signal_clicked ()            .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_clicked));
	colorButton .add (drawingArea);

	// Drawing Area

	drawingArea .signal_draw () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_draw));
	drawingArea .show ();

	// Value Adjustment

	valueAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_value_changed));

	// Dialog

	dialog .set_title (refineName (name));
	dialog .set_transient_for (editor -> getBrowserWindow () -> getWindow ());
	dialog .get_color_selection () -> signal_color_changed () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_color_changed));
	dialog .get_color_selection () -> set_has_opacity_control (true);
	dialog .get_color_selection () -> set_has_palette (true);

	dialog .property_ok_button () .get_value () -> hide ();
	dialog .property_cancel_button () .get_value () -> hide ();

	// Colors Add/Remove Buttons
	
	addColorButton    .signal_clicked () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_add_color_clicked));
	removeColorButton .signal_clicked () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_remove_color_clicked));

	// Colors Drawing Area

	colorsDrawingArea .add_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::KEY_PRESS_MASK);
	colorsDrawingArea .set_can_focus (true);

	colorsDrawingArea .signal_configure_event ()      .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_colors_configure_event));
	colorsDrawingArea .signal_key_press_event ()      .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_colors_key_press_event));
	colorsDrawingArea .signal_button_release_event () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_colors_button_release_event));
	colorsDrawingArea .signal_draw ()                 .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_colors_draw));
	colorsDrawingArea .show ();

	colorsScrolledWindow .add (colorsDrawingArea);

	// Menu

	const auto copyMenuItem  = Gtk::manage (new Gtk::ImageMenuItem (Gtk::StockID ("gtk-copy")));
	const auto pasteMenuItem = Gtk::manage (new Gtk::ImageMenuItem (Gtk::StockID ("gtk-paste")));

	copyMenuItem  -> signal_activate () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_copy));
	pasteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &MFColorRGBAButton::on_paste));

	copyMenuItem  -> set_always_show_image (true);
	pasteMenuItem -> set_always_show_image (true);
	pasteMenuItem -> set_sensitive (false);

	menu .append (*copyMenuItem);
	menu .append (*pasteMenuItem);
	menu .show_all ();

	clipboard -> string_changed () .addInterest (&Gtk::ImageMenuItem::set_sensitive, pasteMenuItem, true);
	clipboard -> target () = "titania-x3d-color";
	clipboard -> setup ();

	// Setup

	setup ();
}

void
MFColorRGBAButton::setIndex (const int32_t value)
{
	index = value;

	set_field ();

	index_changed .emit ();
}

void
MFColorRGBAButton::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColorRGBA> (name) .removeInterest (&MFColorRGBAButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColorRGBA> (name) .addInterest (&MFColorRGBAButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
MFColorRGBAButton::on_color_changed ()
{
	drawingArea       .queue_draw ();
	colorsDrawingArea .queue_draw ();

	if (changing)
		return;

	const auto rgba  = dialog .get_color_selection () -> get_current_rgba ();
	const auto color = X3D::Color4f (rgba .get_red (), rgba .get_green (), rgba .get_blue (), rgba .get_alpha ());

	hsva = color .hsva ();

	set_color (0, color);

	changing = true;
	valueAdjustment -> set_value (hsva [2]);
	changing = false;
}

void
MFColorRGBAButton::on_value_changed ()
{
	drawingArea       .queue_draw ();
	colorsDrawingArea .queue_draw ();

	if (changing)
		return;

	hsva [2] = valueAdjustment -> get_value ();

	const auto color = make_hsva (hsva);

	set_color (1, color);

	changing = true;
	dialog .get_color_selection () -> set_current_rgba (to_rgba (color));
	changing = false;
}

void
MFColorRGBAButton::set_color (const int id, const X3D::Color4f & color)
{
	if (nodes .empty ())
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFColorRGBA> (name);

			field .removeInterest (&MFColorRGBAButton::set_field, this);
			field .addInterest (&MFColorRGBAButton::connect, this);

			field .set1Value (index, color);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);
}

void
MFColorRGBAButton::set_field ()
{
	buffer .addEvent ();
}

void
MFColorRGBAButton::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last field.

	bool hasField = false;
	bool isEmpty  = false;

	this -> node = nullptr;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto & field = node -> getField <X3D::MFColorRGBA> (name);
	
			if (field .empty () or index < 0)
			{
				index        = -1;
				hasField     = true;
				isEmpty      = field .empty ();
				this -> node = node;
				break;
			}

			if (index >= int32_t (field .size ()))
				index = field .size () - 1;

			const auto & value = field .get1Value (index);
			const auto   rgba  = to_rgba (value);

			hsva = value .hsva ();

			dialog .get_color_selection () -> set_current_rgba (rgba);
			dialog .get_color_selection () -> set_previous_rgba (rgba);
			valueAdjustment -> set_value (hsva [2]);

			this -> node = node;
			hasField     = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField or isEmpty)
	{
		dialog .get_color_selection () -> set_current_rgba (Gdk::RGBA ());
		valueAdjustment -> set_value (0);
	}

	widget               .set_sensitive (hasField and index >= 0);
	colorsScrolledWindow .set_sensitive (hasField and not isEmpty);
	removeColorButton    .set_sensitive (index >= 0);
	on_colors_configure_event (nullptr);

	if (hide)
		widget .set_visible (hasField);

	if (nodes .empty ())
		dialog .set_visible (false);

	changing = false;
}

void
MFColorRGBAButton::connect (const X3D::MFColorRGBA & field)
{
	field .removeInterest (&MFColorRGBAButton::connect, this);
	field .addInterest (&MFColorRGBAButton::set_field, this);
}

bool
MFColorRGBAButton::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto color    = dialog .get_color_selection () -> get_current_rgba ();
	const int  width1_2 = drawingArea .get_width () / 2;

	draw_checker_board (context,
	                    8, 8,
	                    X3D::Color4f (0.6, 0.6, 0.6, 1),
	                    X3D::Color4f (0.4, 0.4, 0.4, 1),
	                    0, 0,
	                    width1_2, drawingArea .get_height ());

	context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
	context -> rectangle (0, 0, width1_2, drawingArea .get_height ());
	context -> fill ();

	context -> set_source_rgb (color .get_red (), color .get_green (), color .get_blue ());
	context -> rectangle (width1_2, 0, drawingArea .get_width () - width1_2, drawingArea .get_height ());
	context -> fill ();

	if (colorButton .get_style_context () -> get_state () & Gtk::STATE_FLAG_INSENSITIVE)
	{
		draw_checker_board (context,
		                    1, 1,
		                    X3D::Color4f (0.8, 0.8, 0.8, 0.5),
		                    X3D::Color4f (0.2, 0.2, 0.2, 0.5),
		                    0, 0,
		                    drawingArea .get_width (), drawingArea .get_height ());
	}

	return true;
}

void
MFColorRGBAButton::on_clicked ()
{
	dialog .present ();
}

Gdk::RGBA
MFColorRGBAButton::to_rgba (const X3D::Color4f & value)
{
	Gdk::RGBA color;

	color .set_rgba (value .r (), value .g (), value .b (), value .a ());
	return color;
}

void
MFColorRGBAButton::on_add_color_clicked ()
{
	undoStep .reset ();

	addUndoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFColorRGBA> (name);

			if (index >= 0 and index < int32_t (field .size ()))
				field .emplace_back (field .get1Value (index));
			else
				field .emplace_back (1, 1, 1, 1);

			setIndex (field .size () - 1);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);
}

void
MFColorRGBAButton::on_remove_color_clicked ()
{
	undoStep .reset ();

	addUndoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFColorRGBA> (name);

			if (index >= 0 and index < int32_t (field .size ()))
				field .erase (field .begin () + index);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);
}

bool
MFColorRGBAButton::on_colors_configure_event (GdkEventConfigure* const)
{
	colorsDrawingArea .queue_draw ();

	if (not node)
	{
		colorsDrawingArea .set_size_request (-1, (colorsSize + colorsGap) - colorsGap + colorsBorder [2] + colorsBorder [3]);
		return false;
	}

	try
	{
		const auto & field   = node -> getField <X3D::MFColorRGBA> (name);
		const auto   width   = colorsDrawingArea .get_width ();
		const double columns = getColumns (width, colorsSize, colorsGap, colorsBorder);
		const int    height  = int (field .size () / columns + 1) * (colorsSize + colorsGap) - colorsGap + colorsBorder [2] + colorsBorder [3];

		colorsDrawingArea .set_size_request (-1, height);
	}
	catch (const X3D::X3DError &)
	{ }

	return false;
}

bool
MFColorRGBAButton::on_colors_key_press_event (GdkEventKey* event)
{
	if (not node)
		return false;

	try
	{
		const auto &  field   = node -> getField <X3D::MFColorRGBA> (name);
		const auto    width   = colorsDrawingArea .get_width ();
		const int32_t columns = getColumns (width, colorsSize, colorsGap, colorsBorder);

		if (field .empty ())
		{
			setIndex (-1);
			return true;
		}

		if (index < 0)
		{
			setIndex (field .empty () ? -1 : 0);
			return true;
		}

		switch (event -> keyval)
		{
			case GDK_KEY_Up:
			{
				if (index < columns)
				{
					// First row.
					const int32_t column = index % columns;
					const int32_t rest   = field .size () % columns;

					if (column)
					{
						if (column - 1 < rest)
							setIndex (field .size () - (rest - column + 1));
						else
							setIndex (field .size () - rest - (columns - column + 1));
					}
					else
					{
						// First column.
						if (rest < (int32_t) field .size ())
							setIndex (field .size () - rest - 1);
						else
							setIndex (field .size () - 1);
					}
				}
				else
					setIndex (index - columns);

				return true;
			}
			case GDK_KEY_Down:
			{
				const auto column = index % columns;

				if (index + columns < (int32_t) field .size ())
					setIndex (index + columns);
				else
				{
					if (column + 1 < std::min <int32_t> (columns, field .size ()))
						setIndex (column + 1);
					else
						setIndex (0);
				}

				return true;
			}
			case GDK_KEY_Left:
			{
				if (index > 0)
					setIndex (index - 1);

				else
					setIndex (field .size () - 1);

				return true;
			}
			case GDK_KEY_Right:
			{
				if (index + 1 < (int32_t) field .size ())
					setIndex (index + 1);

				else
					setIndex (0);

				return true;
			}
			default:
				break;
		}
	}
	catch (const X3D::X3DError &)
	{ }

	return false;
}

bool
MFColorRGBAButton::on_colors_button_release_event (GdkEventButton* event)
{
	try
	{
		if (not node)
			return true;

		const auto & field   = node -> getField <X3D::MFColorRGBA> (name);
		const auto   width   = colorsDrawingArea .get_width ();
		const auto   columns = getColumns (width, colorsSize, colorsGap, colorsBorder);
		const size_t column  = (event -> x - colorsBorder [0] + colorsGap / 2.0) / (colorsSize + colorsGap);
		const size_t row     = (event -> y - colorsBorder [3] + colorsGap / 2.0) / (colorsSize + colorsGap);
		const size_t index   = columns * row + column;

		if (index < field .size ())
			setIndex (index);

		colorsDrawingArea .grab_focus ();
	}
	catch (const X3D::X3DError &)
	{ }

	return true;
}

bool
MFColorRGBAButton::on_colors_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	if (not node)
		return true;

	try
	{
		const auto & field   = node -> getField <X3D::MFColorRGBA> (name);
		const auto   width   = colorsDrawingArea .get_width ();
		const auto   columns = getColumns (width, colorsSize, colorsGap, colorsBorder);

		for (int32_t i = 0, size = field .size (); i < size; ++ i)
		{
			const auto &  color  = field [i];
			const int32_t column = i % columns;
			const int32_t row    = i / columns;
			const double  x      = colorsBorder [0] + column * colorsSize + column * colorsGap;
			const double  y      = colorsBorder [3] + row * colorsSize + row * colorsGap;
			const double  w      = std::floor (colorsSize / 2);

			draw_checker_board (context,
			                    2, 2,
			                    X3D::Color4f (0.6, 0.6, 0.6, 1),
			                    X3D::Color4f (0.4, 0.4, 0.4, 1),
			                    x, y,
			                    colorsSize - w, colorsSize);

			context -> set_source_rgba (color .r (), color .g (), color .b (), color .a ());
			context -> rectangle (x, y, colorsSize - w, colorsSize);
			context -> fill ();

			context -> set_source_rgb (color .r (), color .g (), color .b ());
			context -> rectangle (x + w, y, colorsSize - w, colorsSize);
			context -> fill ();

			if (i == getIndex ())
			{
				const auto color = widget .get_style_context () -> get_color (Gtk::STATE_FLAG_NORMAL);

				context -> set_line_width (2);
				context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
				context -> rectangle (x - 1, y - 1, colorsSize + 2, colorsSize + 2);
				context -> stroke ();
			}		
		}
	}
	catch (const X3D::X3DError &)
	{ }

	return true;
}

size_t
MFColorRGBAButton::getColumns (const double width, const double size, const double gap, const X3D::Vector4i & border)
{
	return ((width / gap + 1 - border [0] + border [1]) * gap) / (size + gap);
}

bool
MFColorRGBAButton::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 3:
		{
			if (index >= 0)
				menu .popup (event -> button, event -> time);

			return true;
		}
		default:
			break;
	}

	return false;
}

void
MFColorRGBAButton::on_copy ()
{
	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			auto & field = node -> getField <X3D::MFColorRGBA> (name);
			clipboard -> set_string () = X3D::SFColorRGBA (field .get1Value (index)) .toString ();
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

void
MFColorRGBAButton::on_paste ()
{
	X3D::Color4f     value;
	X3D::SFColorRGBA color;

	if (color .fromString (clipboard -> string_changed ()))
		value = color;
	else
	{
		X3D::SFColor color;

		if (color .fromString (clipboard -> string_changed ()))
		   value = X3D::Color4f (color .getRed (), color .getGreen (), color .getBlue (), 1);
	   else
	      return;
	}

	undoStep .reset ();

	addUndoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColorRGBA> (name) .set1Value (index, value);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFColorRGBA> (nodes, name, undoStep);
}

MFColorRGBAButton::~MFColorRGBAButton ()
{
	colorButton .remove ();
	dispose ();
}

} // puck
} // titania
