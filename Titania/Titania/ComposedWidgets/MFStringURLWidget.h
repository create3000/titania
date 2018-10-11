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

#ifndef __TITANIA_COMPOSED_WIDGETS_MFSTRING_URLWIDGET_H__
#define __TITANIA_COMPOSED_WIDGETS_MFSTRING_URLWIDGET_H__

#include "../ComposedWidgets/X3DMFStringWidget.h"

#include "../Base/X3DUserInterface.h"

namespace titania {
namespace puck {

class FileOpenDialog;

class MFStringURLWidget :
	public X3DMFStringWidget
{
public:

	///  @name Construction

	MFStringURLWidget (X3DUserInterface* const,
	                   Gtk::TreeView &,
	                   const Glib::RefPtr <Gtk::CellRendererText> &,
	                   Gtk::Button &,
	                   Gtk::Button &,
	                   Gtk::Button &,
	                   const Glib::RefPtr <Gtk::TreeViewColumn> &,
	                   const std::string &);

	///  @name Member access

	void
	setFragment (const std::string & value)
	{ fragment = value; }

	const std::string &
	getFragment () const
	{ return fragment; }

	///  @name Destruction

	virtual
	~MFStringURLWidget () final override;


private:

	///  @name Event handlers

	virtual
	void
	on_add_clicked () final override;

	void
	on_reload_clicked ();

	bool
	on_button_release_event (GdkEventButton*);

	virtual
	void
	set_buffer () final override;

	virtual
	void
	set_string (const X3D::MFString &) final override;

	///  @name Members

	X3DUserInterface* const                  userInterface;
	Gtk::Button &                            reloadButton;
	const Glib::RefPtr <Gtk::TreeViewColumn> URLChooserColumn;
	std::unique_ptr <FileOpenDialog>         fileOpenDialog;
	std::string                              fragment;

};

} // puck
} // titania

#endif
