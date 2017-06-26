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
#include "../NotebookPage/NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../ComposedWidgets/TexturePreview.h"

#include <Titania/X3D/Components/Texturing/ImageTexture.h>
#include <Titania/OS.h>

#include <sys/wait.h>

namespace titania {
namespace puck {

class VideoEncoder
{
public:

	VideoEncoder (const basic::uri & filename);

	bool
	open ();

	bool
	write (Magick::Image & image);

	bool
	close ();

	~VideoEncoder ();


private:

	const basic::uri filename;
	std::string      command_with_args;
	pid_t            pid;
	int32_t          stdin;
	int32_t          stdout;
	int32_t          stderr;
	bool             opened;

};

VideoEncoder::VideoEncoder (const basic::uri & filename) :
	         filename (filename),
	command_with_args (os::escape_argument ("ffmpeg")),
	              pid (0),
	            stdin (0),
	           stdout (0),
	           stderr (0),
	           opened (false)
{
	os::join_command (command_with_args, "-f", "image2pipe", "-vcodec", "png", "-r", "30", "-i", "-", "-vcodec", "png", "-q:v", "0", filename .path ());
}

bool
VideoEncoder::open ()
{
	__LOG__ << command_with_args << std::endl;

	if (opened)
		close ();

	os::unlink (filename .path ());

	if ((pid = os::popen3 (command_with_args, &stdin, &stdout, &stderr)) <= 0)
		return false;

	opened = true;
	return true;
}

bool
VideoEncoder::write (Magick::Image & image)
{
	Magick::Blob blob;

	image .magick ("PNG");
	image .write (&blob);

	const int32_t bytes        = blob .length ();
	const int32_t bytesWritten = ::write (stdin, static_cast <const char*> (blob .data ()), bytes);

	__LOG__ << pid << " : " << stdin << " : " << bytesWritten << " : " << bytes << std::endl;

	if (bytesWritten not_eq bytes)
		return false;

	return true;
}

bool
VideoEncoder::close ()
{
	if (not opened)
		return false;

	opened = false;

	{
		constexpr size_t BUFFER_SIZE = 1024;
	
		size_t             bytesRead = 0;
		std::vector <char> buffer (BUFFER_SIZE);
		std::string        output;
	
		::close (stdin);

		while ((bytesRead = read (stdout, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
			output .append (buffer .data (), bytesRead);

		__LOG__ << output << std::endl;

		::close (stdout);

		while ((bytesRead = read (stderr, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
			std::clog .write (buffer .data (), bytesRead);

		::close (stderr);
	}

	stdin  = 0;
	stdout = 0;
	stderr = 0;

	int32_t status = 0;

	::waitpid (pid, &status, 0);

	if (status)
		return false;

	return true;
}

VideoEncoder::~VideoEncoder ()
{
	close ();
}

//static
//std::string
//golden_pipe (const std::string & program, const std::string & input)
////throw (Error <URL_UNAVAILABLE>)
//{
//	constexpr size_t BUFFER_SIZE = 1024;
//
//	int32_t pid       = 0;
//	int32_t status    = 0;
//	int32_t stdin     = 0;
//	int32_t stdout    = 0;
//	int32_t stderr    = 0;
//	size_t  bytesRead = 0;
//
//	std::vector <char> buffer (BUFFER_SIZE);
//
//	// Open pipe.
//
//	if ((pid = os::popen3 (program, &stdin, &stdout, &stderr)) <= 0)
//		throw Error <URL_UNAVAILABLE> ("Couldn't open program '" + program + "'.");
//
//	// Write to pipe.
//
//	if (write (stdin, input .c_str (), input .size ()) not_eq (int32_t) input .size ())
//		throw Error <URL_UNAVAILABLE> ("Write to pipe failed.");
//
//	close (stdin);
//
//	// Read from pipe.
//
//	std::string output;
//
//	while ((bytesRead = read (stdout, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
//		output .append (buffer .data (), bytesRead);
//
//	close (stdout);
//
//	// Read error from pipe.
//
//	while ((bytesRead = read (stderr, buffer .data (), sizeof (char) * BUFFER_SIZE)) > 0)
//		std::clog .write (buffer .data (), bytesRead);
//
//	close (stderr);
//
//	// Read from pipe.
//
//	waitpid (pid, &status, 0);
//
//	if (status)
//		throw Error <INVALID_X3D> ("Exit status :" + basic::to_string (status, std::locale::classic ()));
//
//	return output;
//}

RenderPanel::RenderPanel (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	       X3DBaseInterface (browserWindow, page -> getMainBrowser ()),
	X3DRenderPanelInterface (get_ui ("Panels/RenderPanel.glade"), page, PanelType::RENDER_PANEL),
	                preview (new TexturePreview (this,
                            getPreviewBox (),
                            getTextureFormatLabel (),
                            getTextureLoadStateLabel ())),
	                texture (preview -> getLocalBrowser () -> createNode <X3D::ImageTexture> ()),
	           renderThread (),
	           videoEncoder ()
{
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
RenderPanel::on_record_clicked ()
{
	auto         url             = getPage () -> getMainBrowser () -> getWorldURL ();
   const size_t frames          = 1800;
	const size_t framesPerSecond = 30;
	const size_t width           = 768;
	const size_t height          = 576;
	const size_t antialiasing    = 4;

	set_frame (0);

	videoEncoder .reset (new VideoEncoder (url + ".mp4"));

	if (videoEncoder -> open ())
	{
		try
		{
			renderThread .reset (new RenderThread ({ url .str () }, frames, framesPerSecond, width, height, antialiasing));
			renderThread -> signal_frame_changed () .connect (sigc::mem_fun (this, &RenderPanel::on_frame_changed));
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
	else
	{
		__LOG__ << "Couldn't open output video file." << std::endl;
	}
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
	{
		videoEncoder -> close ();
		renderThread -> stop ();
	}
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
	renderThread .reset ();
	videoEncoder .reset ();

	dispose ();
}

} // puck
} // titania
