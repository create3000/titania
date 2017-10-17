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

#include "FileImportDialog.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/OS.h>

namespace titania {
namespace puck {

namespace ImportType {

static constexpr int EXTERN_PROTOS = 0;
static constexpr int PROTOS        = 1;
static constexpr int SCENE         = 2;
static constexpr int INLINE        = 3;

}

static constexpr auto ALL_FILES_FILTER = "All Files";
static constexpr auto X3D_FILTER       = "All X3D Files";
static constexpr auto IMAGES_FILTER    = "All Images";
static constexpr auto AUDIO_FILTER     = "All Audio";
static constexpr auto VIDEOS_FILTER    = "All Videos";

static constexpr auto X3D_XML_ENCODING_FILTER                     = "X3D XML Encoding (*.x3d)";
static constexpr auto X3D_CLASSIC_VRML_ENCODING_FILTER            = "X3D Classic VRML Encoding (*.x3dv)";
static constexpr auto VRML97_ENCODING_FILTER                      = "VRML97 Encoding (*.wrl)";
static constexpr auto X3D_JSON_ENCODING_FILTER                    = "X3D JSON Encoding (*.json)";
static constexpr auto COMPRESSED_X3D_XML_ENCODING_FILTER          = "Compressed X3D XML Encoding (*.x3dz)";
static constexpr auto COMPRESSED_X3D_CLASSIC_VRML_ENCODING_FILTER = "Compressed X3D Classic VRML Encoding (*.x3dvz)";
static constexpr auto COMPRESSED_VRML97_ENCODING_FILTER           = "Compressed VRML97 Encoding (*.wrz)";

static constexpr auto AUTODESK_3DS_MAX_FILE_FILTER = "Autodesk 3DS Max File (*.3ds)";
static constexpr auto WAVEFRONT_OBJ_FILE_FILTER    = "Wavefront OBJ File (*.obj)";

FileImportDialog::FileImportDialog (X3DBrowserWindow* const browserWindow) :
	            X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DFileImportDialogInterface (get_ui ("Dialogs/FileImportDialog.glade"))
{
	// Dialog

	setTitleBar (getImportDialog (), getImportDialogHeaderBar ());

	// Set filter name

	getFileFilterAll   () -> set_name (_ (ALL_FILES_FILTER));
	getFileFilterX3D   () -> set_name (_ (X3D_FILTER));
	getFileFilterImage () -> set_name (_ (IMAGES_FILTER));
	getFileFilterAudio () -> set_name (_ (AUDIO_FILTER));
	getFileFilterVideo () -> set_name (_ (VIDEOS_FILTER));

	getFileFilterX3DXMLEncoding         () -> set_name (_ (X3D_XML_ENCODING_FILTER));
	getFileFilterX3DClassicVRMLEncoding () -> set_name (_ (X3D_CLASSIC_VRML_ENCODING_FILTER));
	getFileFilterX3DJSONEncoding        () -> set_name (_ (X3D_JSON_ENCODING_FILTER));
	getFileFilterVrmlEncoding           () -> set_name (_ (VRML97_ENCODING_FILTER));

	getFileFilterCompressedX3DXMLEncoding         () -> set_name (_ (COMPRESSED_X3D_XML_ENCODING_FILTER));
	getFileFilterCompressedX3DClassicVRMLEncoding () -> set_name (_ (COMPRESSED_X3D_CLASSIC_VRML_ENCODING_FILTER));
	getFileFilterCompressedVrmlEncoding           () -> set_name (_ (COMPRESSED_VRML97_ENCODING_FILTER));

	getFileFilterAutodesk3DSMax ()  -> set_name (_ (AUTODESK_3DS_MAX_FILE_FILTER));
	getFileFilterWavefrontOBJ ()    -> set_name (_ (WAVEFRONT_OBJ_FILE_FILTER));

	// Add filter

	getWindow () .add_filter (getFileFilterX3D ());
	getWindow () .add_filter (getFileFilterImage ());
	getWindow () .add_filter (getFileFilterAudio ());
	getWindow () .add_filter (getFileFilterVideo ());

	getWindow () .add_filter (getFileFilterX3DXMLEncoding ());
	getWindow () .add_filter (getFileFilterX3DClassicVRMLEncoding ());
	//getWindow () .add_filter (getFileFilterX3DJSONEncoding ());
	getWindow () .add_filter (getFileFilterVrmlEncoding ());

	getWindow () .add_filter (getFileFilterCompressedX3DXMLEncoding ());
	getWindow () .add_filter (getFileFilterCompressedX3DClassicVRMLEncoding ());
	getWindow () .add_filter (getFileFilterCompressedVrmlEncoding ());

	getWindow () .add_filter (getFileFilterAutodesk3DSMax ());
	getWindow () .add_filter (getFileFilterWavefrontOBJ ());

	// Set uri

	const auto worldURL = getCurrentScene () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
		getWindow () .set_uri (worldURL .filename () .str ());

	else
		getWindow () .set_filename (Glib::get_home_dir () + "/" + _ ("scene.x3d"));

	// Import dialog

	getImportDialog () .set_transient_for (getBrowserWindow () -> getWindow ());

	setup ();
}

basic::uri
FileImportDialog::getUrl () const
{
	return "file://" + getWindow () .get_file () -> get_path ();
}

void
FileImportDialog::setFilter (const std::string & name)
{
	if (name == _("Images"))
		getWindow () .set_filter (getFileFilterImage ());
	else if (name == _("Audio"))
		getWindow () .set_filter (getFileFilterAudio ());
	else if (name == _("Videos"))
		getWindow () .set_filter (getFileFilterVideo ());
	else
		getWindow () .set_filter (getFileFilterX3D ());
}

void
FileImportDialog::run ()
{
	if (getConfig () -> hasItem ("currentFolder"))
		getWindow () .set_current_folder_uri (getConfig () -> getItem <std::string> ("currentFolder"));
	else
		getWindow () .set_current_folder (Glib::get_home_dir ());
	
	setFilter (getConfig () -> getItem <std::string> ("filter"));
		
	const auto responseId = getWindow () .run ();

	getConfig () -> setItem ("currentFolder", getWindow () .get_current_folder_uri ());

	if (getWindow () .get_filter ())
		getConfig () -> setItem ("filter", getWindow () .get_filter () -> get_name ());

	if (responseId == Gtk::RESPONSE_OK)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import Scene"));
		const auto nodes    = getBrowserWindow () -> import ({ getUrl () }, undoStep);

		getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}

	quit ();
}

FileImportDialog::~FileImportDialog ()
{
	dispose ();
}

} // puck
} // titania
