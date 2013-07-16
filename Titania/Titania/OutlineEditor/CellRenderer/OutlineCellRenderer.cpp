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

#include "OutlineCellRenderer.h"

#

namespace titania {
namespace puck {

OutlineCellRenderer::OutlineCellRenderer () :
	 Glib::ObjectBase (typeid (OutlineCellRenderer)),
	Gtk::CellRenderer (),
	    data_property (*this, "tree-data", nullptr)
{
	//property_mode () = Gtk::CELL_RENDERER_MODE_EDITABLE;
	property_xpad () = 2;
	property_ypad () = 2;
}

void
OutlineCellRenderer::get_size_vfunc (Gtk::Widget & widget,
                                     const Gdk::Rectangle* cell_area,
                                     int* x_offset, int* y_offset,
                                     int* width, int* height) const
{
	// Compute text width
	Glib::RefPtr <Pango::Layout> layout_ptr = widget .create_pango_layout (property_data () .get_value () -> object () -> getName ());
	Pango::Rectangle             rect       = layout_ptr -> get_pixel_logical_extents ();

	*width  = property_xpad () * 4 + rect .get_width ();
	*height = property_ypad () * 4 + rect .get_height ();

	__LOG__ << width << " : " << height << std::endl;
}

void
OutlineCellRenderer::render_vfunc (const Cairo::RefPtr <Cairo::Context> & context,
                                   Gtk::Widget & widget,
                                   const Gdk::Rectangle & background_area,
                                   const Gdk::Rectangle & cell_area,
                                   Gtk::CellRendererState flags)
{
//	// Get cell state
//	Gtk::StateType state;
//	Gtk::StateType text_state;
//
//	if ((flags & Gtk::CELL_RENDERER_SELECTED) not_eq 0)
//	{
//		state      = Gtk::STATE_SELECTED;
//		text_state = (widget.has_focus ()) ? Gtk::STATE_SELECTED : Gtk::STATE_ACTIVE;
//	}
//	else
//	{
//		state      = Gtk::STATE_NORMAL;
//		text_state = (widget.is_sensitive ()) ? Gtk::STATE_NORMAL : Gtk::STATE_INSENSITIVE;
//	}

//	context -> set_source_rgb (1, 1, 0);
//	context -> paint ();
	
	context -> set_source_rgb (0, 0, 0);
	context -> move_to (0, 0);
	context -> line_to (13, 13);
   context -> stroke ();

//	// Draw color text
//	
//	Pango::FontDescription font;
//
//	Glib::RefPtr <Pango::Layout> layout = Pango::Layout::create (context);
//
//	font .set_family ("monospace");
//	font .set_weight (Pango::WEIGHT_BOLD);
//	layout -> set_font_description (font);
//
//	context -> move_to (0, 5);
//	context -> set_source_rgb (0, 0, 0);
//
//	layout->set_text("text");
//	layout->update_from_cairo_context(context);  //gets cairo cursor position
//	layout->add_to_cairo_context(context);       //adds text to cairos stack of stuff to be drawn
//	context->stroke();                                //tells Cairo to render it's stack
//
//	int text_width;
//	int text_height;
//
//	//get the text dimensions (it updates the variables -- by reference)
//	layout -> get_pixel_size (text_width, text_height);
//	
//	__LOG__ << text_width << " : " << text_height << std::endl;
//	{
//		Cairo::RefPtr<Cairo::ToyFontFace> font = Cairo::ToyFontFace::create ("sans", Cairo::FONT_SLANT_ITALIC, Cairo::FONT_WEIGHT_BOLD);
//		context -> set_font_face (font);
//		context -> set_font_size (12);
//
//		context -> set_source_rgb (0, 0, 0);
//		context -> show_text ("text");
//
//		context -> move_to (0, 5);
//		context -> line_to (1, 5);
//      context -> stroke ();
//	}
}

} // puck
} // titania
