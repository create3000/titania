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

#include "BrowserCellRenderer.h"

namespace titania {
namespace X3D {

static constexpr int MIN_BROWSER_SIZE = 12;

BrowserCellRenderer::BrowserCellRenderer () :
	    Glib::ObjectBase (typeid (BrowserCellRenderer)),
	   Gtk::CellRenderer (),
	          cellHeight (0),
	      index_property (*this, "index", 0),
	transparent_property (*this, "transparent", false),
	   callback_property (*this, "callback", std::function <Browser* ()> ())
{ }

void
BrowserCellRenderer::get_preferred_width_vfunc (Gtk::Widget & widget, int & minimum_width, int & natural_width) const
{
	minimum_width = std::max (cellHeight, MIN_BROWSER_SIZE);
	natural_width = std::max (cellHeight, MIN_BROWSER_SIZE);
}

void
BrowserCellRenderer::get_preferred_height_for_width_vfunc (Gtk::Widget & widget, int width, int & minimum_height, int & natural_height) const
{
	minimum_height = std::max (cellHeight, MIN_BROWSER_SIZE);
	natural_height = std::max (cellHeight, MIN_BROWSER_SIZE);
}

void
BrowserCellRenderer::get_preferred_height_vfunc (Gtk::Widget & widget, int & minimum_height, int & natural_height) const
{
	minimum_height = std::max (cellHeight, MIN_BROWSER_SIZE);
	natural_height = std::max (cellHeight, MIN_BROWSER_SIZE);
}

void
BrowserCellRenderer::get_preferred_width_for_height_vfunc (Gtk::Widget & widget, int height, int & minimum_width, int & natural_width) const
{
	minimum_width = std::max (cellHeight, MIN_BROWSER_SIZE);
	natural_width = std::max (cellHeight, MIN_BROWSER_SIZE);
}

void
BrowserCellRenderer::render_vfunc (const Cairo::RefPtr <Cairo::Context> & context,
                                   Gtk::Widget & widget,
                                   const Gdk::Rectangle & background_area,
                                   const Gdk::Rectangle & cell_area,
                                   Gtk::CellRendererState flags)
{
	try
	{
		// Get variables.

		const auto x        = cell_area .get_x ();
		const auto y        = cell_area .get_y ();
		const auto w        = cell_area .get_width ();
		const auto h        = cell_area .get_height ();
		const auto width    = std::min (w, h);
		const auto height   = std::min (w, h);
		const auto stride   = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, width);
		const auto callback = property_callback () .get_value ();

		// Process callback.

		if (not callback)
			return;

		const auto browser = callback ();

		if (not browser)
			return;

		// Get snapshot image.

		auto image = browser -> getSnapshot (width, height, property_transparent () .get_value (), 8);

		// Determine height and update tree view if needed.

		if (cellHeight not_eq height)
			widget .queue_draw ();

		cellHeight = height;

		// Setup image.

		image .interlaceType (Magick::NoInterlace);
		image .endian (Magick::LSBEndian);
		image .depth (8);
		image .magick ("RGBA");

		// Swap red and blue layers.

		const auto redChannel  = image .separate (Magick::RedChannel);
		const auto blueChannel = image .separate (Magick::BlueChannel);

		image .composite (blueChannel, 0, 0, Magick::CopyRedCompositeOp);
		image .composite (redChannel,  0, 0, Magick::CopyBlueCompositeOp);

		// Create Cairo Surface.

		Magick::Blob blob;

		image .write (&blob);

		const auto surface = Cairo::ImageSurface::create ((uint8_t*) blob .data (), Cairo::FORMAT_ARGB32, width, height, stride);

		// Draw Surface to Context.

		context -> translate ((w - width) / 2.0 + x, (h - height) / 2.0 + y);
		context -> set_source (surface, 0, 0);
		context -> paint ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

} // X3D
} // titania
