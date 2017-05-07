/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
#include "X3DFileSaveDialogInterface.h"

namespace titania {
namespace puck {

void
X3DFileSaveDialogInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DFileSaveDialogInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DFileSaveDialogInterface::create ()
{
	// Get objects.
	m_FileFilterAll                              = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAll"));
	m_FileFilterAudio                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAudio"));
	m_FileFilterCompressedVrmlEncoding           = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedVrmlEncoding"));
	m_FileFilterCompressedX3DClassicVRMLEncoding = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedX3DClassicVRMLEncoding"));
	m_FileFilterCompressedX3DXMLEncoding         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterCompressedX3DXMLEncoding"));
	m_FileFilterImage                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImage"));
	m_FileFilterImageBMP                         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImageBMP"));
	m_FileFilterImageJPEG                        = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImageJPEG"));
	m_FileFilterImagePDF                         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImagePDF"));
	m_FileFilterImagePNG                         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImagePNG"));
	m_FileFilterImagePSD                         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImagePSD"));
	m_FileFilterImageTIFF                        = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImageTIFF"));
	m_FileFilterImageXCF                         = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImageXCF"));
	m_FileFilterVideo                            = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterVideo"));
	m_FileFilterVrmlEncoding                     = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterVrmlEncoding"));
	m_FileFilterX3DClassicVRMLEncoding           = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DClassicVRMLEncoding"));
	m_FileFilterX3DJSONEncoding                  = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DJSONEncoding"));
	m_FileFilterX3DXMLEncoding                   = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3DXMLEncoding"));
	m_ImageAntialiasingAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ImageAntialiasingAdjustment"));
	m_ImageCompressionAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ImageCompressionAdjustment"));
	m_ImageHeightAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ImageHeightAdjustment"));
	m_ImageWidthAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ImageWidthAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("OkButton", m_OkButton);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("OutputStyleBox", m_OutputStyleBox);
	m_builder -> get_widget ("OutputStyleButton", m_OutputStyleButton);
	m_builder -> get_widget ("ImageOptionsDialog", m_ImageOptionsDialog);
	m_builder -> get_widget ("ImageAlphaChannelSwitch", m_ImageAlphaChannelSwitch);
	m_builder -> get_widget ("ImageAntialiasingBox", m_ImageAntialiasingBox);

	// Connect object Gtk::FileChooserDialog with id 'Window'.
	m_Window -> signal_response () .connect (sigc::mem_fun (this, &X3DFileSaveDialogInterface::on_response), false);

	// Call construct handler of base class.
	construct ();
}

X3DFileSaveDialogInterface::~X3DFileSaveDialogInterface ()
{
	delete m_Window;
	delete m_ImageOptionsDialog;
}

} // puck
} // titania
