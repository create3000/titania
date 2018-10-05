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

#include "IconFactory.h"

#include "../Browser/Image.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"

#include <Titania/X3D/InputOutput/FileLoader.h>

namespace titania {
namespace puck {

IconFactory::IconFactory (X3DBrowserWindow* const browserWindow) :
	browserWindow (browserWindow),
	  iconFactory (Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (browserWindow -> getBuilder () -> get_object ("IconFactory")))
{ }

void
IconFactory::createIcon (const std::string & name, const std::string & document)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	if (not document .empty ())
	{
		try
		{
			const Image icon (document);

			iconSet = Gtk::IconSet::create (icon .getIcon ());
		}
		catch (const std::exception & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
}

void
IconFactory::createIcon (const std::string & name, Magick::Image && image)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	try
	{
		const Image icon (std::move (image));

		iconSet = Gtk::IconSet::create (icon .getIcon ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
}

std::string
IconFactory::getIcon (const basic::uri & worldURL, const Gtk::IconSize & iconSize)
{
	std::string image;
	auto        iconSet = Gtk::IconSet::lookup_default (Gtk::StockID (worldURL .filename () .str ()));

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	if (iconSet)
	{
		try
		{
			const auto pixbuf = iconSet -> render_icon_pixbuf (getBrowserWindow () -> getWidget () .get_style_context (), iconSize);

			if (pixbuf)
			{
				gchar* buffer;
				gsize  bufferSize;

				pixbuf -> save_to_buffer (buffer, bufferSize);

				image = std::string (buffer, bufferSize);

				g_free (buffer);
			}
		}
		catch (const Glib::Exception &)
		{ }
	}

	return image;
}

IconFactory::~IconFactory ()
{ }

} // puck
} // titania
