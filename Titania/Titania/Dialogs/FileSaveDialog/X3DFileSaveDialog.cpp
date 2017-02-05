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
#include "../../Configuration/config.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <Titania/X3D/Basic/Traverse.h>
#include <Titania/OS.h>

#include <regex>

namespace titania {
namespace puck {

static constexpr auto ALL_FILES_FILTER = "All Files";
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

static constexpr auto IMAGE_XCF_FILTER  = "Gimp XCF Image (*.xcf)";
static constexpr auto IMAGE_JPEG_FILTER = "JPEG Image (*.jpeg, *.jpg)";
static constexpr auto IMAGE_PDF_FILTER  = "PDF File (*.pdf)";
static constexpr auto IMAGE_PNG_FILTER  = "PNG Image (*.png)";
static constexpr auto IMAGE_PSD_FILTER  = "Photoshop Image (*.psd)";
static constexpr auto IMAGE_TIFF_FILTER = "TIFF Image (*.tiff, *.tif)";
static constexpr auto IMAGE_BMP_FILTER  = "Windows BMP Image (*.bmp)";

X3DFileSaveDialog::X3DFileSaveDialog () :
	X3DFileSaveDialogInterface (get_ui ("Dialogs/FileSaveDialog.glade"))
{
	getFileFilterImage () -> set_name (_ (IMAGES_FILTER));
	getFileFilterAudio () -> set_name (_ (AUDIO_FILTER));
	getFileFilterVideo () -> set_name (_ (VIDEOS_FILTER));

	getFileFilterImageXCF  () -> set_name (_ (IMAGE_XCF_FILTER));
	getFileFilterImageJPEG () -> set_name (_ (IMAGE_JPEG_FILTER));
	getFileFilterImagePDF  () -> set_name (_ (IMAGE_PDF_FILTER));
	getFileFilterImagePNG  () -> set_name (_ (IMAGE_PNG_FILTER));
	getFileFilterImagePSD  () -> set_name (_ (IMAGE_PSD_FILTER));
	getFileFilterImageTIFF () -> set_name (_ (IMAGE_TIFF_FILTER));
	getFileFilterImageBMP  () -> set_name (_ (IMAGE_BMP_FILTER));

	getFileFilterX3DXMLEncoding         () -> set_name (_ (X3D_XML_ENCODING_FILTER));
	getFileFilterX3DClassicVRMLEncoding () -> set_name (_ (X3D_CLASSIC_VRML_ENCODING_FILTER));
	getFileFilterX3DJSONEncoding        () -> set_name (_ (X3D_JSON_ENCODING_FILTER));
	getFileFilterVrmlEncoding           () -> set_name (_ (VRML97_ENCODING_FILTER));

	getFileFilterCompressedX3DXMLEncoding         () -> set_name (_ (COMPRESSED_X3D_XML_ENCODING_FILTER));
	getFileFilterCompressedX3DClassicVRMLEncoding () -> set_name (_ (COMPRESSED_X3D_CLASSIC_VRML_ENCODING_FILTER));
	getFileFilterCompressedVrmlEncoding           () -> set_name (_ (COMPRESSED_VRML97_ENCODING_FILTER));

	getFileFilterAll () -> set_name (_ (ALL_FILES_FILTER));
}

basic::uri
X3DFileSaveDialog::getURL () const
{
	return "file://" + getWindow () .get_file () -> get_path ();
}

bool
X3DFileSaveDialog::run ()
{
	if (getBrowserWindow () -> getConfig () -> getBoolean ("addStandardMetaData"))
	{
		getOutputStyleBox ()    .set_visible (true);
		getOutputStyleButton () .set_active (getConfig () -> getInteger ("outputStyle"));
	}
	else
	{
		getOutputStyleBox ()    .set_visible (false);
		getOutputStyleButton () .set_active (0);
	}

	const auto responseId = getWindow () .run ();

	quit ();

	getConfig () -> setItem ("outputStyle", getOutputStyleButton () .get_active_row_number ());

	if (responseId == Gtk::RESPONSE_OK)
		return true;

	return false;
}

bool
X3DFileSaveDialog::saveRun ()
{
	const auto & worldURL = getCurrentScene () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
	{
		if (getConfig () -> hasItem ("currentFolder"))
		{
			getWindow () .set_current_folder (getConfig () -> getString ("currentFolder"));
			getWindow () .set_current_name (worldURL .basename ());
		}
		else
		{
			getWindow () .set_uri (worldURL .filename () .str ());
			getWindow () .set_current_name (worldURL .basename ());
		}
	}
	else
	{
		if (getConfig () -> hasItem ("currentFolder"))
			getWindow () .set_current_folder (getConfig () -> getString ("currentFolder"));
		else
			getWindow () .set_current_folder (os::home ());

		if (worldURL .basename () .empty ())
			getWindow () .set_current_name (_ ("scene.x3d"));
		else
			getWindow () .set_current_name (worldURL .basename ());
	}

	setX3DFilter (getConfig () -> getString ("filter"));

	const auto response = run ();

	if (getWindow () .get_filter ())
		getConfig () -> setItem ("filter", getWindow () .get_filter () -> get_name ());

	return response;
}

void
X3DFileSaveDialog::saveScene (const bool copy)
{
	if (saveRun ())
		getBrowserWindow () -> save (getURL (), getOutputStyleButton () .get_active_text (), copy);
}

void
X3DFileSaveDialog::setX3DFilter (const std::string & name)
{
	getWindow () .property_filter () .signal_changed () .connect (sigc::mem_fun (this, &X3DFileSaveDialog::on_x3d_filter_changed));

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

void
X3DFileSaveDialog::on_x3d_filter_changed ()
{
	// X3D, VRML97

	if (getWindow () .get_filter () == getFileFilterX3DXMLEncoding ())
		set_suffix (".x3d");

	else if (getWindow () .get_filter () == getFileFilterX3DClassicVRMLEncoding ())
		set_suffix (".x3dv");

	else if (getWindow () .get_filter () == getFileFilterX3DJSONEncoding ())
		set_suffix (".json");

	else if (getWindow () .get_filter () == getFileFilterVrmlEncoding ())
		set_suffix (".wrl");

	// Compressed

	else if (getWindow () .get_filter () == getFileFilterCompressedX3DXMLEncoding ())
		set_suffix (".x3dz");

	else if (getWindow () .get_filter () == getFileFilterCompressedX3DClassicVRMLEncoding ())
		set_suffix (".x3dvz");

	else if (getWindow () .get_filter () == getFileFilterCompressedVrmlEncoding ())
		set_suffix (".wrz");

	// Default

	else
		set_suffix (".x3d");
}

void
X3DFileSaveDialog::setImageFilter (const std::string & name)
{
	getWindow () .property_filter () .signal_changed () .connect (sigc::mem_fun (this, &X3DFileSaveDialog::on_image_filter_changed));

	if (os::program_exists ("gimp"))
		getWindow () .add_filter (getFileFilterImageXCF ());

	getWindow () .add_filter (getFileFilterImageJPEG ());
	getWindow () .add_filter (getFileFilterImagePDF ());
	getWindow () .add_filter (getFileFilterImagePNG ());
	getWindow () .add_filter (getFileFilterImagePSD ());
	getWindow () .add_filter (getFileFilterImageTIFF ());
	getWindow () .add_filter (getFileFilterImageBMP ());

	if (name == _(IMAGE_XCF_FILTER))
		getWindow () .set_filter (getFileFilterImageXCF ());

	else if (name == _(IMAGE_JPEG_FILTER))
		getWindow () .set_filter (getFileFilterImageJPEG ());

	else if (name == _(IMAGE_PDF_FILTER))
		getWindow () .set_filter (getFileFilterImagePDF ());

	else if (name == _(IMAGE_PNG_FILTER))
		getWindow () .set_filter (getFileFilterImagePNG ());

	else if (name == _(IMAGE_PSD_FILTER))
		getWindow () .set_filter (getFileFilterImagePSD ());

	else if (name == _(IMAGE_TIFF_FILTER))
		getWindow () .set_filter (getFileFilterImageTIFF ());

	else if (name == _(IMAGE_BMP_FILTER))
		getWindow () .set_filter (getFileFilterImageBMP ());

	else
		getWindow () .set_filter (getFileFilterImagePNG ());
}

void
X3DFileSaveDialog::on_image_filter_changed ()
{
	if (getWindow () .get_filter () == getFileFilterImageXCF ())
		set_suffix (".xcf");

	else if (getWindow () .get_filter () == getFileFilterImageJPEG ())
		set_suffix (".jpg");

	else if (getWindow () .get_filter () == getFileFilterImagePDF ())
		set_suffix (".pdf");

	else if (getWindow () .get_filter () == getFileFilterImagePNG ())
		set_suffix (".png");

	else if (getWindow () .get_filter () == getFileFilterImagePSD ())
		set_suffix (".psd");

	else if (getWindow () .get_filter () == getFileFilterImageTIFF ())
		set_suffix (".tiff");

	else if (getWindow () .get_filter () == getFileFilterImageBMP ())
		set_suffix (".bmp");
}

void
X3DFileSaveDialog::set_suffix (const std::string & suffix)
{
	basic::uri name (getWindow () .get_current_name ());

	getWindow () .set_current_name (name .basename (false) + suffix);
}

// Export image

void
X3DFileSaveDialog::exportImage ()
{
	const auto worldURL = getCurrentContext () -> getWorldURL ();

	if (getConfig () -> hasItem ("exportFolder"))
		getWindow () .set_current_folder_uri (getConfig () -> getString ("exportFolder"));
	else
		getWindow () .set_current_folder (os::home ());

	getWindow () .set_current_name (worldURL .basename (false) + ".png");

	setImageFilter (getConfig () -> getString ("imageFilter"));

	// Run dialog.

	const auto responseId = getWindow () .run ();

	getConfig () -> setItem ("exportFolder", getWindow () .get_current_folder_uri ());

	if (getWindow () .get_filter ())
		getConfig () -> setItem ("imageFilter", getWindow () .get_filter () -> get_name ());

	quit ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		// Run image options dialog.

		if (imageOptions ())
		{
			// Save image.

			try
			{
				auto filename = basic::uri (Glib::uri_unescape_string (getWindow () .get_filename ()));

				const auto image = getCurrentBrowser () -> getSnapshot (getImageWidthAdjustment () -> get_value (),
				                                                        getImageHeightAdjustment () -> get_value (),
				                                                        getImageAlphaChannelSwitch () .get_active (),
				                                                        getImageAntialiasingAdjustment () -> get_value ());

				image -> quality (getImageCompressionAdjustment () -> get_value ());

				if (filename .suffix () == ".xcf" and os::program_exists ("gimp"))
				{
					std::string pngFilename = "/tmp/titania-XXXXXX.png";
					auto ofstream           = os::mkstemps (pngFilename, 4);

					if (ofstream)
					{
						static const std::regex quotes (R"/(")/");

						filename = std::regex_replace (filename .str (), quotes, "\\\"");

						image -> write (pngFilename);

						os::system ("gimp", "-i", "-b", "(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE \"" + pngFilename + "\" \"" + pngFilename + "\")))"
						            "(drawable (car (gimp-image-get-active-layer image))))"
						            "(gimp-file-save RUN-NONINTERACTIVE image drawable \"" + filename + "\" \"" + filename + "\")"
						            "(gimp-image-delete image)"
						            "(gimp-quit 0))");

					}

					os::unlink (pngFilename);
				}
				else
					image -> write (filename);
			}
			catch (const Magick::Exception & error)
			{
				const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
			
				dialog -> setType (Gtk::MESSAGE_ERROR);
				dialog -> setMessage (_ ("Could not save image!"));
				dialog -> setText (_ ("Tip: check file and folder permissions."));
				dialog -> run ();
			}
			catch (const std::exception & error)
			{
				const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
			
				dialog -> setType (Gtk::MESSAGE_ERROR);
				dialog -> setMessage (_ ("Could not generate image!"));
				dialog -> setText (_ ("Tip: try a smaller image size and/or less antialiasing."));
				dialog -> run ();
			}
		}
	}
}

bool
X3DFileSaveDialog::imageOptions ()
{
	// First configure adjustments.

	const int32_t antialiasing = getCurrentBrowser () -> getMaxSamples ();

	getImageWidthAdjustment ()  -> set_upper (getCurrentBrowser () -> getMaxRenderBufferSize ());
	getImageHeightAdjustment () -> set_upper (getCurrentBrowser () -> getMaxRenderBufferSize ());
	getImageAntialiasingAdjustment () -> set_upper (antialiasing);

	// Restore image options.

	if (getConfig () -> hasItem ("imageWidth"))
		getImageWidthAdjustment () -> set_value (getConfig () -> getInteger ("imageWidth"));

	if (getConfig () -> hasItem ("imageHeight"))
		getImageHeightAdjustment () -> set_value (getConfig () -> getInteger ("imageHeight"));

	getImageAlphaChannelSwitch () .set_active (getConfig () -> getBoolean ("imageAlphaChannel"));

	if (getConfig () -> hasItem ("imageAntialiasing"))
		getImageAntialiasingAdjustment () -> set_value (std::min (getConfig () -> getInteger ("imageAntialiasing"), antialiasing));

	if (getConfig () -> hasItem ("imageCompression"))
		getImageCompressionAdjustment () -> set_value (getConfig () -> getInteger ("imageCompression"));

	getImageAntialiasingBox () .set_sensitive (getCurrentBrowser () -> isExtensionAvailable ("GL_EXT_framebuffer_multisample"));

	// Run image options dialog.

	const auto responseId = getImageOptionsDialog () .run ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		getConfig () -> setItem ("imageWidth",        (int) getImageWidthAdjustment () -> get_value ());
		getConfig () -> setItem ("imageHeight",       (int) getImageHeightAdjustment () -> get_value ());
		getConfig () -> setItem ("imageAlphaChannel", getImageAlphaChannelSwitch () .get_active ());
		getConfig () -> setItem ("imageAntialiasing", (int) getImageAntialiasingAdjustment () -> get_value ());
		getConfig () -> setItem ("imageCompression",  (int) getImageCompressionAdjustment () -> get_value ());
	}

	getImageOptionsDialog () .hide ();

	return responseId == Gtk::RESPONSE_OK;
}

// Export nodes

bool
X3DFileSaveDialog::exportNodes (const X3D::MFNode & nodes, basic::uri & worldURL, const X3D::UndoStepPtr & undoStep)
{
	if (getConfig () -> hasItem ("exportFolder"))
		getWindow () .set_current_folder_uri (getConfig () -> getString ("exportFolder"));

	bool response = saveRun ();

	if (response)
	{
		getConfig () -> setItem ("exportFolder", getWindow () .get_current_folder_uri ());

		if (not exportNodes (nodes, getURL (), getOutputStyleButton () .get_active_text (), undoStep))
			response = false;

		worldURL = getURL ();
	}

	return response;
}

bool
X3DFileSaveDialog::exportNodes (const X3D::MFNode & nodes, const basic::uri & worldURL, const std::string & outputStyle, const X3D::UndoStepPtr & undoStep)
{
	using namespace std::placeholders;

	// Temporarily change url's in protos

	const auto protoUndoStep = std::make_shared <X3D::UndoStep> ("Traverse");

	X3D::traverse (getCurrentContext (),
	               std::bind (&X3DBrowserWidget::transform, getCurrentContext () -> getWorldURL (), worldURL, protoUndoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS);

	// Change url's in nodes

	X3D::traverse (const_cast <X3D::MFNode &> (nodes),
	               std::bind (&X3DBrowserWidget::transform, getCurrentContext () -> getWorldURL (), worldURL, undoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS |
	               X3D::TRAVERSE_ROOT_NODES);

	// Export nodes to stream

	std::ostringstream osstream;

	getBrowserWindow () -> exportNodes (getCurrentContext (), osstream, nodes, false);

	// Undo url change in protos

	protoUndoStep -> undo ();

	// Save scene

	basic::ifilestream stream (osstream .str ());

	const auto scene = getCurrentBrowser () -> createX3DFromStream (worldURL, stream);

	return getBrowserWindow () -> save (scene, worldURL, outputStyle, false);
}

X3DFileSaveDialog::~X3DFileSaveDialog ()
{ }

} // puck
} // titania
