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

#include "X3DFileImportDialogInterface.h"

namespace titania {
namespace puck {

void
X3DFileImportDialogInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DFileImportDialogInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DFileImportDialogInterface::create ()
{
	// Get objects.
	m_FileFilterAll                              = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAll"));
	m_FileFilterAudio                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAudio"));
	m_FileFilterAutodesk3DSMax                   = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAutodesk3DSMax"));
	m_FileFilterCompressedVrmlEncoding           = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedVrmlEncoding"));
	m_FileFilterCompressedX3DClassicVRMLEncoding = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedX3DClassicVRMLEncoding"));
	m_FileFilterCompressedX3DXMLEncoding         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedX3DXMLEncoding"));
	m_FileFilterFonts                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterFonts"));
	m_FileFilterImage                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImage"));
	m_FileFilterVideo                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterVideo"));
	m_FileFilterVrmlEncoding                     = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterVrmlEncoding"));
	m_FileFilterWavefrontOBJ                     = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterWavefrontOBJ"));
	m_FileFilterX3D                              = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3D"));
	m_FileFilterX3DClassicVRMLEncoding           = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DClassicVRMLEncoding"));
	m_FileFilterX3DJSONEncoding                  = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DJSONEncoding"));
	m_FileFilterX3DXMLEncoding                   = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DXMLEncoding"));

	// Get widgets.
	m_builder -> get_widget ("ImportImage1", m_ImportImage1);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("ImportDialog", m_ImportDialog);
	m_builder -> get_widget ("ImportDialogHeaderBar", m_ImportDialogHeaderBar);
	m_builder -> get_widget ("ImportSceneButton", m_ImportSceneButton);
	m_builder -> get_widget ("ImportAsInlineButton", m_ImportAsInlineButton);
	m_builder -> get_widget ("ImportExternProtosButton", m_ImportExternProtosButton);
	m_builder -> get_widget ("ImportProtosButton", m_ImportProtosButton);
}

X3DFileImportDialogInterface::~X3DFileImportDialogInterface ()
{
	delete m_Window;
	delete m_ImportDialog;
}

} // puck
} // titania
