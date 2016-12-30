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

#ifndef __TITANIA_EDITORS_APPEARANCE_EDITOR_BROWSER_CELL_RENDERER_H__
#define __TITANIA_EDITORS_APPEARANCE_EDITOR_BROWSER_CELL_RENDERER_H__

#include <gtkmm.h>
#include <Titania/X3D/Browser/Browser.h>

namespace titania {
namespace X3D {

class BrowserCellRenderer :
	public Gtk::CellRenderer
{
public:

	///  @name Construction

	BrowserCellRenderer (X3D::Browser* const browser);
	
	///  @name Properties

	Glib::Property <uint32_t> &
	property_index ()
	{ return index_property; }

	const Glib::Property <uint32_t> &
	property_index () const
	{ return index_property; }

	Glib::Property <bool> &
	property_transparent ()
	{ return transparent_property; }

	const Glib::Property <bool> &
	property_transparent () const
	{ return transparent_property; }

	Glib::Property <std::function <X3D::Browser* ()>> &
	property_callback ()
	{ return callback_property; }

	const Glib::Property <std::function <X3D::Browser* ()>> &
	property_callback () const
	{ return callback_property; }

	///  @name Destruction

	virtual
	~BrowserCellRenderer ()
	{ }


private:

	virtual
	void
	get_preferred_width_vfunc (Gtk::Widget & widget, int & minimum_width, int & natural_width) const final override;

	virtual
	void
	get_preferred_height_for_width_vfunc (Gtk::Widget & widget, int width, int & minimum_height, int & natural_height) const final override;

	virtual
	void
	get_preferred_height_vfunc (Gtk::Widget & widget, int & minimum_height, int & natural_height) const final override;

	virtual
	void
	get_preferred_width_for_height_vfunc (Gtk::Widget & widget, int height, int & minimum_width, int & natural_width) const final override;

	virtual
	void
	render_vfunc (const Cairo::RefPtr <Cairo::Context> & context,
	              Gtk::Widget & widget,
	              const Gdk::Rectangle & background_area,
	              const Gdk::Rectangle & cell_area,
	              Gtk::CellRendererState flags) final override;

	///  @name Members

	int                                               cellHeight;
	Glib::Property <uint32_t>                         index_property;
	Glib::Property <bool>                             transparent_property;
	Glib::Property <std::function <X3D::Browser* ()>> callback_property;

};

} // X3D
} // titania

#endif
