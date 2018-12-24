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

#include "SFColorButton.h"

#include "../Bits/Cairo.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>

namespace titania {
namespace puck {

SFColorButton::SFColorButton (X3DBaseInterface* const editor,
                              Gtk::Button & colorButton,
                              const Glib::RefPtr <Gtk::Adjustment> & valueAdjustment,
                              Gtk::Widget & widget,
                              const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	      colorButton (colorButton),
	  valueAdjustment (valueAdjustment),
	           widget (widget),
	      drawingArea (),
	           dialog (),
	             menu (),
	            nodes (),
	             name (name),
	         undoStep (),
	            input (-1),
	         changing (false),
	           buffer (),
	              hsv (),
	        clipboard (new X3D::Clipboard (getMasterBrowser () -> getExecutionContext ()))
{
	addChildObjects (nodes, buffer, clipboard);

	// Buffer

	buffer .addInterest (&SFColorButton::set_buffer, this);

	// Button

	colorButton .signal_button_press_event () .connect (sigc::mem_fun (this, &SFColorButton::on_button_press_event));
	colorButton .signal_clicked ()            .connect (sigc::mem_fun (this, &SFColorButton::on_clicked));
	colorButton .add (drawingArea);

	// Drawing Area

	drawingArea .signal_draw () .connect (sigc::mem_fun (this, &SFColorButton::on_draw));
	drawingArea .show ();

	// Value Adjustment

	valueAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &SFColorButton::on_value_changed));

	// Dialog

	dialog .set_transient_for (getBrowserWindow () -> getWindow ());
	dialog .set_title (refineName (name));
	dialog .get_color_selection () -> signal_color_changed () .connect (sigc::mem_fun (this, &SFColorButton::on_color_changed));
	dialog .get_color_selection () -> set_has_opacity_control (false);
	dialog .get_color_selection () -> set_has_palette (true);

	dialog .property_ok_button ()     .get_value () -> hide ();
	dialog .property_cancel_button () .get_value () -> hide ();

	// Menu

	const auto copyMenuItem  = Gtk::manage (new Gtk::ImageMenuItem (Gtk::StockID ("gtk-copy")));
	const auto pasteMenuItem = Gtk::manage (new Gtk::ImageMenuItem (Gtk::StockID ("gtk-paste")));

	copyMenuItem  -> signal_activate () .connect (sigc::mem_fun (this, &SFColorButton::on_copy));
	pasteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &SFColorButton::on_paste));

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
SFColorButton::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFColor> (name) .removeInterest (&SFColorButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFColor> (name) .addInterest (&SFColorButton::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
SFColorButton::on_color_changed ()
{
	drawingArea .queue_draw ();

	if (changing)
		return;

	const auto rgba  = dialog .get_color_selection () -> get_current_rgba ();
	const auto color = X3D::Color3f (rgba .get_red (), rgba .get_green (), rgba .get_blue ());

	hsv = color .hsv ();

	set_color (0, color);

	changing = true;
	valueAdjustment -> set_value (hsv [2]);
	changing = false;
}

void
SFColorButton::on_value_changed ()
{
	drawingArea .queue_draw ();

	if (changing)
		return;

	hsv [2] = valueAdjustment -> get_value ();

	const auto color = make_hsv (hsv);

	set_color (1, color);

	changing = true;
	dialog .get_color_selection () -> set_current_rgba (to_rgba (color));
	changing = false;
}

void
SFColorButton::set_color (const int id, const X3D::Color3f & color)
{
	if (nodes .empty ())
		return;

	if (id not_eq input)
		undoStep .reset ();

	input = id;

	addUndoFunction <X3D::SFColor> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFColor> (name);

			field .removeInterest (&SFColorButton::set_field, this);
			field .addInterest (&SFColorButton::connect, this);

			field = color;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFColor> (nodes, name, undoStep);
}

void
SFColorButton::set_field ()
{
	buffer .addEvent ();
}

void
SFColorButton::set_buffer ()
{
	undoStep .reset ();

	changing = true;

	// Find last field.

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::SFColor> (name);
			const auto   rgba  = to_rgba (field);

			hsv = field .getHSV ();

			dialog .get_color_selection () -> set_current_rgba (rgba);
			dialog .get_color_selection () -> set_previous_rgba (rgba);
			valueAdjustment -> set_value (hsv [2]);

			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
	{
		dialog .get_color_selection () -> set_current_rgba (Gdk::RGBA ());
		valueAdjustment -> set_value (0);
	}

	widget .set_sensitive (hasField);

	if (nodes .empty ())
		dialog .set_visible (false);

	changing = false;
}

void
SFColorButton::connect (const X3D::SFColor & field)
{
	field .removeInterest (&SFColorButton::connect, this);
	field .addInterest (&SFColorButton::set_field, this);
}

bool
SFColorButton::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	draw_checker_board (context,
	                    8, 8,
	                    X3D::Color4f (0.6, 0.6, 0.6, 1),
	                    X3D::Color4f (0.4, 0.4, 0.4, 1),
	                    0, 0,
	                    drawingArea .get_width (), drawingArea .get_height ());

	const auto color = dialog .get_color_selection () -> get_current_rgba ();

	context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
	context -> rectangle (0, 0, drawingArea .get_width (), drawingArea .get_height ());
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
SFColorButton::on_clicked ()
{
	dialog .present ();
}

Gdk::RGBA
SFColorButton::to_rgba (const X3D::Color3f & value)
{
	Gdk::RGBA color;

	color .set_rgba (value .r (), value .g (), value .b (), 1);
	return color;
}

bool
SFColorButton::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 3:
		{
			menu .popup (event -> button, event -> time);
			return true;
		}
		default:
			break;
	}

	return false;
}

void
SFColorButton::on_copy ()
{
	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::SFColor> (name);
			clipboard -> set_string () = field .toString ();
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

void
SFColorButton::on_paste ()
{
	X3D::Color3f     value;
	X3D::SFColorRGBA color;

	if (color .fromString (clipboard -> string_changed ()))
		value = X3D::Color3f (color .getRed (), color .getGreen (), color .getBlue ());
	else
	{
		X3D::SFColor color;
	   
		if (color .fromString (clipboard -> string_changed ()))
			value = color;
      else
		   return;
	}

	undoStep .reset ();

	addUndoFunction <X3D::SFColor> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			node -> setField <X3D::SFColor> (name, value);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFColor> (nodes, name, undoStep);
}

SFColorButton::~SFColorButton ()
{
	colorButton .remove ();
	dispose ();
}

} // puck
} // titania
