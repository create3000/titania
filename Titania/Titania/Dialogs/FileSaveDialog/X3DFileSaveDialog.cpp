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

#include "X3DFileSaveDialog.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>
#include <Titania/X3D/Editing/X3DEditor.h>

namespace titania {
namespace puck {

static constexpr auto ALL_FILES_FILTER = "All Files";
static constexpr auto IMAGES_FILTER    = "All Images";
static constexpr auto AUDIO_FILTER     = "All Audio";
static constexpr auto VIDEOS_FILTER    = "All Videos";

static constexpr auto X3D_XML_ENCODING_FILTER                     = "X3D XML Encoding (*.x3d)";
static constexpr auto X3D_CLASSIC_VRML_ENCODING_FILTER            = "X3D Classic VRML Encoding (*.x3dv)";
static constexpr auto VRML97_ENCODING_FILTER                      = "VRML97 Encoding (*.wrl)";
static constexpr auto X3D_JSON_ENCODING_FILTER                    = "X3D JSON Encoding (*.x3dj)";
static constexpr auto COMPRESSED_X3D_XML_ENCODING_FILTER          = "Compressed X3D XML Encoding (*.x3dz)";
static constexpr auto COMPRESSED_X3D_CLASSIC_VRML_ENCODING_FILTER = "Compressed X3D Classic VRML Encoding (*.x3dvz)";
static constexpr auto COMPRESSED_VRML97_ENCODING_FILTER           = "Compressed VRML97 Encoding (*.wrz)";

X3DFileSaveDialog::X3DFileSaveDialog () :
	X3DBaseFileSaveDialog ()
{
	getFileFilterAll () -> set_name (_ (ALL_FILES_FILTER));

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
}

bool
X3DFileSaveDialog::save (const bool copy)
{
	const auto success = run (getCurrentScene () -> getWorldURL ());

	if (success)
		getBrowserWindow () -> save (getUrl (), getOutputStyleButton () .get_active_text (), copy);

	return success;
}

bool
X3DFileSaveDialog::run (const basic::uri & url)
{
	if (not url .empty () and url .is_local ())
	{
		if (getConfig () -> hasItem ("currentFolder"))
		{
			getWindow () .set_current_folder (getConfig () -> getItem <std::string> ("currentFolder"));
			getWindow () .set_current_name (url .basename ());
		}
		else
		{
			getWindow () .set_uri (url .filename () .escape () .str ());
			getWindow () .set_current_name (url .basename ());
		}
	}
	else
	{
		if (getConfig () -> hasItem ("currentFolder"))
			getWindow () .set_current_folder (getConfig () -> getItem <std::string> ("currentFolder"));
		else
			getWindow () .set_current_folder (Glib::get_home_dir ());

		if (url .basename () .empty ())
			getWindow () .set_current_name (_ ("scene.x3d"));
		else
			getWindow () .set_current_name (url .basename ());
	}

	if (getBrowserWindow () -> getConfig () -> getItem <bool> ("addStandardMetaData"))
	{
		getOutputStyleBox ()    .set_visible (true);
		getOutputStyleButton () .set_active (getConfig () -> getItem <int32_t> ("outputStyle"));
	}
	else
	{
		getOutputStyleBox ()    .set_visible (false);
		getOutputStyleButton () .set_active (0);
	}

	setFileFilter (getConfig () -> getItem <std::string> ("fileFilter"));

	const auto success = X3DBaseFileSaveDialog::run ();

	getConfig () -> setItem ("outputStyle", getOutputStyleButton () .get_active_row_number ());
	getConfig () -> setItem ("currentFolder", getWindow () .get_current_folder ());

	return success;
}

void
X3DFileSaveDialog::setFileFilter (const std::string & name)
{
	getWindow () .add_filter (getFileFilterX3DXMLEncoding ());
	getWindow () .add_filter (getFileFilterX3DClassicVRMLEncoding ());
	getWindow () .add_filter (getFileFilterX3DJSONEncoding ());
	getWindow () .add_filter (getFileFilterVrmlEncoding ());

	getWindow () .add_filter (getFileFilterCompressedX3DXMLEncoding ());
	getWindow () .add_filter (getFileFilterCompressedX3DClassicVRMLEncoding ());
	getWindow () .add_filter (getFileFilterCompressedVrmlEncoding ());

	// X3D, VRML97

	if (name == _(X3D_XML_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterX3DXMLEncoding ());

	else if (name == _(X3D_CLASSIC_VRML_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterX3DClassicVRMLEncoding ());

	else if (name == _(X3D_JSON_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterX3DJSONEncoding ());

	else if (name == _(VRML97_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterVrmlEncoding ());

	// Compressed

	else if (name == _(COMPRESSED_X3D_XML_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterCompressedX3DXMLEncoding ());

	else if (name == _(COMPRESSED_X3D_CLASSIC_VRML_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterCompressedX3DClassicVRMLEncoding ());

	else if (name == _(COMPRESSED_VRML97_ENCODING_FILTER))
		getWindow () .set_filter (getFileFilterCompressedVrmlEncoding ());

	// Default

	else
		getWindow () .set_filter (getFileFilterX3DXMLEncoding ());
}

std::string
X3DFileSaveDialog::getExtension () const
{
	// X3D, VRML97

	if (getWindow () .get_filter () == getFileFilterX3DXMLEncoding ())
		return ".x3d";

	else if (getWindow () .get_filter () == getFileFilterX3DClassicVRMLEncoding ())
		return ".x3dv";

	else if (getWindow () .get_filter () == getFileFilterX3DJSONEncoding ())
		return ".x3dj";

	else if (getWindow () .get_filter () == getFileFilterVrmlEncoding ())
		return ".wrl";

	// Compressed

	else if (getWindow () .get_filter () == getFileFilterCompressedX3DXMLEncoding ())
		return ".x3dz";

	else if (getWindow () .get_filter () == getFileFilterCompressedX3DClassicVRMLEncoding ())
		return ".x3dvz";

	else if (getWindow () .get_filter () == getFileFilterCompressedVrmlEncoding ())
		return ".wrz";

	// Default

	return ".x3d";
}

const std::set <std::string> &
X3DFileSaveDialog::getKnownFileTypes () const
{
	return X3D::FileGenerator::getKnownFileTypes ();
}

bool
X3DFileSaveDialog::exportNodes (const X3D::MFNode & nodes, const basic::uri & worldURL, const std::string & outputStyle, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		using namespace std::placeholders;
	
		// Temporarily change url's in protos
	
		const auto protoUndoStep = std::make_shared <X3D::UndoStep> ("Traverse");
	
		X3D::traverse (getCurrentContext (),
		               std::bind (&X3D::X3DEditor::transform, getCurrentContext () -> getWorldURL (), worldURL, protoUndoStep, _1),
		               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
		               X3D::TRAVERSE_PROTO_DECLARATIONS |
		               X3D::TRAVERSE_PROTO_DECLARATION_BODY);
	
		// Change url's in nodes
	
		X3D::traverse (const_cast <X3D::MFNode &> (nodes),
		               std::bind (&X3D::X3DEditor::transform, getCurrentContext () -> getWorldURL (), worldURL, undoStep, _1),
		               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
		               X3D::TRAVERSE_PROTO_DECLARATIONS |
		               X3D::TRAVERSE_PROTO_DECLARATION_BODY |
		               X3D::TRAVERSE_ROOT_NODES);
	
		// Export nodes to string
	
		const auto string = X3D::X3DEditor::exportNodes (getCurrentContext (), nodes, "XML", false);
	
		// Undo url change in protos
	
		protoUndoStep -> undo ();
	
		// Save scene
	
		const auto browser = X3D::createBrowser ();
	
		browser -> setLoadUrlObjects (false);
	
		const auto scene = browser -> createX3DFromString (string);
	
		scene -> setWorldURL (worldURL);
	
		return getBrowserWindow () -> save (scene, worldURL, outputStyle, false);
	}
	catch (const X3D::X3DError & error)
	{
		getCurrentBrowser () -> getConsole () -> error (error .what ());
		return false;
	}
}

X3DFileSaveDialog::~X3DFileSaveDialog ()
{ }

} // puck
} // titania
