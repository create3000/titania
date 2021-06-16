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

#include "FileExportImageDialog.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <regex>

namespace titania {
namespace puck {

static constexpr auto IMAGE_JPEG_FILTER = "JPEG Image (*.jpeg, *.jpg)";
static constexpr auto IMAGE_PNG_FILTER  = "PNG Image (*.png)";
static constexpr auto IMAGE_BMP_FILTER  = "Windows BMP Image (*.bmp)";

const std::set <std::string> FileExportImageDialog::knownFileTypes = {
	".jpeg",
	".jpg",
	".png",
	".bmp",
};

FileExportImageDialog::FileExportImageDialog (X3DBrowserWindow* const browserWindow) :
	     X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DBaseFileSaveDialog ()
{
	setName ("FileExportImageDialog");

	getWindow () .set_title (_ ("Export Image …"));

	getFileFilterImageJPEG () -> set_name (_ (IMAGE_JPEG_FILTER));
	getFileFilterImagePNG  () -> set_name (_ (IMAGE_PNG_FILTER));
	getFileFilterImageBMP  () -> set_name (_ (IMAGE_BMP_FILTER));

	setTitleBar (getImageOptionsDialog (), getImageOptionsHeaderBar ());

	// Image options dialog

	getImageOptionsDialog () .set_transient_for (getBrowserWindow () -> getWindow ());

	setup ();
}

void
FileExportImageDialog::setFileFilter (const std::string & name)
{
	getWindow () .add_filter (getFileFilterImageJPEG ());
	getWindow () .add_filter (getFileFilterImagePNG ());
	getWindow () .add_filter (getFileFilterImageBMP ());

	if (name == _(IMAGE_JPEG_FILTER))
		getWindow () .set_filter (getFileFilterImageJPEG ());

	else if (name == _(IMAGE_PNG_FILTER))
		getWindow () .set_filter (getFileFilterImagePNG ());

	else if (name == _(IMAGE_BMP_FILTER))
		getWindow () .set_filter (getFileFilterImageBMP ());

	else
		getWindow () .set_filter (getFileFilterImagePNG ());
}

std::string
FileExportImageDialog::getExtension () const
{
	if (getWindow () .get_filter () == getFileFilterImageJPEG ())
		return ".jpg";

	else if (getWindow () .get_filter () == getFileFilterImagePNG ())
		return ".png";

	else if (getWindow () .get_filter () == getFileFilterImageBMP ())
		return ".bmp";

	// Default

	return ".png";
}

// Export image

bool
FileExportImageDialog::run ()
{
	// Run image options dialog.

	if (not options ())
		return false;

	// Save image.

	auto image = getCurrentBrowser () -> getSnapshot (getImageWidthAdjustment () -> get_value (),
	                                                  getImageHeightAdjustment () -> get_value (),
	                                                  getImageRenderBackgroundSwitch () .get_active (),
	                                                  getImageAlphaChannelSwitch () .get_active (),
	                                                  getImageAntialiasingAdjustment () -> get_value ());

	return save (image);
}

bool
FileExportImageDialog::save (const Glib::RefPtr <Gdk::Pixbuf> & image, const std::string & basename)
{
	const auto worldURL = getCurrentContext () -> getWorldURL ();

	if (getConfig () -> hasItem ("currentFolder"))
		getWindow () .set_current_folder (getConfig () -> getItem <std::string> ("currentFolder"));
	else
		getWindow () .set_current_folder (Glib::get_home_dir ());

	if (basename .empty ())
		getWindow () .set_current_name (worldURL .stem () + ".png");
	else
		getWindow () .set_current_name (basename);

	// Run dialog.

	const auto success = X3DBaseFileSaveDialog::run ();

	getConfig () -> setItem ("currentFolder", getWindow () .get_current_folder ());

	if (not success)
		return false;

	// Save image.

	try
	{
		const auto url       = getUrl ();
		const auto extension = url .extension ();
		auto       format    = extension .empty () ? "png" : extension .substr (1);

		if (format == "jpg")
			format = "jpeg";

		image -> save (url .path (), format);

		return true;
	}
	catch (const Glib::Error & error)
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");

		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Could not generate image!"));
		dialog -> setText (error .what ());
		dialog -> run ();
		return false;
	}
	catch (const std::exception & error)
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");

		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Could not generate image!"));
		dialog -> setText (_ ("Tip: try a smaller image size and/or less antialiasing."));
		dialog -> run ();
		return false;
	}
}

bool
FileExportImageDialog::options ()
{
	// First configure adjustments.

	const int32_t antialiasing = getCurrentBrowser () -> getMaxSamples ();

	getImageWidthAdjustment ()  -> set_upper (getCurrentBrowser () -> getMaxRenderBufferSize ());
	getImageHeightAdjustment () -> set_upper (getCurrentBrowser () -> getMaxRenderBufferSize ());
	getImageAntialiasingAdjustment () -> set_upper (antialiasing);

	// Restore image options.

	if (getConfig () -> hasItem ("imageWidth"))
		getImageWidthAdjustment () -> set_value (getConfig () -> getItem <int32_t> ("imageWidth"));

	if (getConfig () -> hasItem ("imageHeight"))
		getImageHeightAdjustment () -> set_value (getConfig () -> getItem <int32_t> ("imageHeight"));

	getImageRenderBackgroundSwitch () .set_active (getConfig () -> getItem <bool> ("imageRenderBackground", true));
	getImageAlphaChannelSwitch ()     .set_active (getConfig () -> getItem <bool> ("imageAlphaChannel"));

	if (getConfig () -> hasItem ("imageAntialiasing"))
		getImageAntialiasingAdjustment () -> set_value (std::min (getConfig () -> getItem <int32_t> ("imageAntialiasing"), antialiasing));

	if (getConfig () -> hasItem ("imageCompression"))
		getImageCompressionAdjustment () -> set_value (getConfig () -> getItem <int32_t> ("imageCompression"));

	getImageAntialiasingBox () .set_sensitive (getCurrentBrowser () -> getExtension ("GL_EXT_framebuffer_multisample"));

	// Run image options dialog.

	const auto responseId = getImageOptionsDialog () .run ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		getConfig () -> setItem ("imageWidth",            (int) getImageWidthAdjustment () -> get_value ());
		getConfig () -> setItem ("imageHeight",           (int) getImageHeightAdjustment () -> get_value ());
		getConfig () -> setItem ("imageRenderBackground", getImageRenderBackgroundSwitch () .get_active ());
		getConfig () -> setItem ("imageAlphaChannel",     getImageAlphaChannelSwitch () .get_active ());
		getConfig () -> setItem ("imageAntialiasing",     (int) getImageAntialiasingAdjustment () -> get_value ());
		getConfig () -> setItem ("imageCompression",      (int) getImageCompressionAdjustment () -> get_value ());
	}

	getImageOptionsDialog () .hide ();

	return responseId == Gtk::RESPONSE_OK;
}

FileExportImageDialog::~FileExportImageDialog ()
{
	dispose ();
}

} // puck
} // titania
