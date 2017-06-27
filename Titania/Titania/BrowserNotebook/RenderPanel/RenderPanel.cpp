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

#include "RenderPanel.h"

#include "RenderThread.h"
#include "VideoEncoder.h"
#include "../NotebookPage/NotebookPage.h"

#include "../../Bits/String.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../ComposedWidgets/TexturePreview.h"
#include "../../Dialogs/FileSaveDialog/FileSaveVideoDialog.h"

#include <Titania/X3D/Components/Texturing/ImageTexture.h>

namespace titania {
namespace puck {

RenderPanel::RenderPanel (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRenderPanelInterface (get_ui ("Panels/RenderPanel.glade"), page, PanelType::RENDER_PANEL),
	                preview (new TexturePreview (this,
                            getPreviewBox (),
                            getTextureFormatLabel (),
                            getTextureLoadStateLabel ())),
	                texture (preview -> getLocalBrowser () -> createNode <X3D::ImageTexture> ()),
	           renderThread (),
	           videoEncoder (),
	               filename ()
{
	setTitleBar (getPropertiesDialog (), getPropertiesHeaderBar ());
	getPropertiesDialog () .set_transient_for (getBrowserWindow () -> getWindow ());

	const auto textureProperties = preview -> getLocalBrowser ()-> createNode <X3D::TextureProperties> ();

	textureProperties -> generateMipMaps ()     = false;
	textureProperties -> minificationFilter ()  = "NEAREST_PIXEL";
	textureProperties -> magnificationFilter () = "NEAREST_PIXEL";
	textureProperties -> boundaryModeS ()       = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeT ()       = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeR ()       = "CLAMP_TO_EDGE";
	textureProperties -> textureCompression ()  = "DEFAULT";

	texture -> textureProperties () = textureProperties;

	preview -> setTexture (texture);

	setup ();
}

void
RenderPanel::initialize ()
{
	X3DRenderPanelInterface::initialize ();

	preview -> getLocalBrowser () -> signal_focus_out_event () .connect (sigc::mem_fun ((X3DPanelInterface*) this, &X3DPanelInterface::on_focus_out_event));
	preview -> getLocalBrowser () -> signal_focus_in_event ()  .connect (sigc::mem_fun ((X3DPanelInterface*) this, &X3DPanelInterface::on_focus_in_event));
}

bool
RenderPanel::getPropertiesDialogResponse ()
{
	const auto &  browser      = getPage () -> getMainBrowser ();
	const int32_t antialiasing = browser -> getMaxSamples ();

	filename = browser -> getWorldURL () .parent () + browser -> getWorldURL () .basename (false) + ".mp4";

	getWidthAdjustment ()  -> set_upper (browser -> getMaxRenderBufferSize ());
	getHeightAdjustment () -> set_upper (browser -> getMaxRenderBufferSize ());
	getAntialiasingAdjustment () -> set_upper (antialiasing);

	getFileLabel () .set_text (filename .basename ());

	getDurationAdjustment ()     -> set_value (1800);
	getFrameRateAdjustment ()    -> set_value (30);
	getWidthAdjustment ()        -> set_value (768);
	getHeightAdjustment ()       -> set_value (576);
	getAntialiasingAdjustment () -> set_value (std::min (4, antialiasing));

	const auto responseId = getPropertiesDialog () .run ();

	getPropertiesDialog () .hide ();

	if (responseId not_eq Gtk::RESPONSE_OK)
		return false;

	return true;
}

void
RenderPanel::setRendering (const bool value)
{
	if (value)
	{
		if (not getPropertiesDialogResponse ())
			return;

		const auto   worldURL      = getPage () -> getMainBrowser () -> getWorldURL ();
	   const size_t frames        = getDurationAdjustment () -> get_value ();
		const size_t frameRate     = getFrameRateAdjustment () -> get_value ();
		const size_t width         = getWidthAdjustment () -> get_value ();
		const size_t height        = getHeightAdjustment () -> get_value ();
		const size_t antialiasing  = getAntialiasingAdjustment () -> get_value ();
		const size_t fixedPipeline = getPage () -> getMainBrowser () -> getFixedPipeline ();

		getRecordButton () .set_stock_id (Gtk::StockID ("gtk-media-stop"));
		set_frame (0);

		videoEncoder = std::make_unique <VideoEncoder> (filename, frameRate);

		try
		{
			renderThread = std::make_unique <RenderThread> (worldURL, frames, frameRate, width, height, antialiasing, fixedPipeline);
			renderThread -> signal_frame_changed () .connect (sigc::mem_fun (this, &RenderPanel::on_frame_changed));

			videoEncoder = std::make_unique <VideoEncoder> (filename, frameRate);
			videoEncoder -> open ();
		}
		catch (const std::exception & error)
		{
			__LOG__ << error .what () << std::endl;
			setRendering (false);
		}
	}
	else
	{
		getRecordButton () .set_stock_id (Gtk::StockID ("gtk-media-record"));

		videoEncoder .reset ();
		renderThread .reset ();
	}
}

bool
RenderPanel::getRendering () const
{
	return bool (renderThread);
}

void
RenderPanel::on_properties_file_chooser_button_clicked ()
{
	const auto dialog = std::dynamic_pointer_cast <FileSaveVideoDialog> (createDialog ("FileSaveVideoDialog"));

	dialog -> setUrl (filename);

	if (not dialog -> run ())
		return;

	filename = dialog -> getUrl ();

	getFileLabel () .set_text (filename .basename ());
}

void
RenderPanel::on_properties_time_changed ()
{
	getTimeLabel () .set_text (strfframes (getDurationAdjustment () -> get_value (), getFrameRateAdjustment () -> get_value ()));
}

void
RenderPanel::on_record_clicked ()
{
	setRendering (not getRendering ());
}

void
RenderPanel::on_frame_changed ()
{
	const auto frame     = renderThread -> getFrame ();
	const auto lastFrame = frame + 1 == renderThread -> getFrames ();

	set_frame (frame);

	if (texture -> checkLoadState () not_eq X3D::IN_PROGRESS_STATE)
		texture -> setUrl (renderThread -> getCurrentImage ());

	if (not videoEncoder -> write (renderThread -> getCurrentImage ()) or lastFrame)
		setRendering (false);
}

void
RenderPanel::set_frame (const size_t value)
{
	getFrameAdjustment () -> set_lower (value);
	getFrameAdjustment () -> set_upper (value);
	getFrameAdjustment () -> set_value (value);
}

RenderPanel::~RenderPanel ()
{
	setRendering (false);

	dispose ();
}

} // puck
} // titania
