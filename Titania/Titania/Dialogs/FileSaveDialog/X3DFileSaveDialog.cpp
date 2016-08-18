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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

namespace titania {
namespace puck {

X3DFileSaveDialog::X3DFileSaveDialog () :
	X3DFileSaveDialogInterface (get_ui ("Dialogs/FileSaveDialog.glade"))
{
	getFileFilterX3D   () -> set_name (_ ("X3D"));
	getFileFilterImage () -> set_name (_ ("Images"));
	getFileFilterAudio () -> set_name (_ ("Audio"));
	getFileFilterVideo () -> set_name (_ ("Videos"));
	getFileFilterAll   () -> set_name (_ ("All Files"));

	const auto & worldURL = getCurrentScene () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
		getWindow () .set_uri (worldURL .filename () .str ());

	else
	{
		if (worldURL .basename () .empty ())
		{
			getWindow () .set_current_folder (os::home ());
			getWindow () .set_current_name (_ ("scene.x3d"));
		}
		else
			getWindow () .set_current_name (worldURL .basename ());
	}
}

basic::uri
X3DFileSaveDialog::getURL () const
{
	return "file://" + getWindow () .get_file () -> get_path ();
}

bool
X3DFileSaveDialog::run ()
{
	getCompressFileBox () .set_visible (true);
	getCompressFileButton () .set_active (getCurrentScene () -> isCompressed ());
	const auto responseId = getWindow () .run ();

	quit ();

	if (responseId == Gtk::RESPONSE_OK)
		return true;

	return false;
}

void
X3DFileSaveDialog::saveScene (const bool copy)
{
	getWindow () .add_filter (getFileFilterX3D ());
	getWindow () .set_filter (getFileFilterX3D ());

	if (run ())
		getBrowserWindow () -> save (getURL (), getCompressFileButton () .get_active (), copy);
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
	getWindow () .add_filter (getFileFilterImage ());
	getWindow () .set_filter (getFileFilterImage ());

	// Run dialog.

	const auto responseId = getWindow () .run ();

	getConfig () -> setItem ("exportFolder", getWindow () .get_current_folder_uri ());
	quit ();

	if (responseId == Gtk::RESPONSE_OK)
	{

		// Run image options dialog.

		if (imageOptions ())
		{
			// Save image.

			try
			{
				const auto image = getCurrentBrowser () -> getSnapshot (getImageWidthAdjustment () -> get_value (),
				                                                        getImageHeightAdjustment () -> get_value (),
				                                                        getImageAlphaChannelSwitch () .get_active (),
				                                                        getImageAntialiasingAdjustment () -> get_value ());

				image -> quality (getImageCompressionAdjustment () -> get_value ());
				image -> write (Glib::uri_unescape_string (getWindow () .get_filename ()));
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

	getImageAntialiasingBox () .set_sensitive (getCurrentBrowser () -> hasExtension ("GL_EXT_framebuffer_multisample"));

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
X3DFileSaveDialog::exportNodes (X3D::MFNode & nodes, basic::uri & worldURL, const X3D::UndoStepPtr & undoStep)
{
	if (getConfig () -> hasItem ("exportFolder"))
		getWindow () .set_current_folder_uri (getConfig () -> getString ("exportFolder"));

	getWindow () .add_filter (getFileFilterX3D ());
	getWindow () .set_filter (getFileFilterX3D ());

	getCompressFileBox ()    .set_visible (true);
	getCompressFileButton () .set_active (getCurrentScene () -> isCompressed ());

	auto responseId = getWindow () .run ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		getConfig () -> setItem ("exportFolder", getWindow () .get_current_folder_uri ());

		if (not exportNodes (nodes, getURL (), getCompressFileButton () .get_active (), undoStep))
			responseId = Gtk::RESPONSE_CANCEL;

		worldURL = getURL ();
	}

	quit ();

	return responseId == Gtk::RESPONSE_OK;
}

bool
X3DFileSaveDialog::exportNodes (X3D::MFNode & nodes, const basic::uri & worldURL, const bool compressed, const X3D::UndoStepPtr & undoStep)
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

	X3D::traverse (nodes,
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

	return getBrowserWindow () -> save (scene, worldURL, compressed, false);
}

X3DFileSaveDialog::~X3DFileSaveDialog ()
{ }

} // puck
} // titania
