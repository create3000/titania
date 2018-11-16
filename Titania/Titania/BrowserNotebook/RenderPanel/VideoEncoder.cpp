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

#include "VideoEncoder.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

using namespace std::placeholders;

/***
 *  Codecs PNG, H.264, Xvid
 */

VideoEncoder::VideoEncoder (const basic::uri & filename,
                            const std::string & codec,
                            const size_t frameRate,
                            const size_t duration,
                            size_t width,
                            size_t height) :
	        filename (filename),
	         command (),
	            pipe (std::bind (&VideoEncoder::on_stdout, this, _1), std::bind (&VideoEncoder::on_stderr, this, _1)),
	          stdout (),
	          stderr (),
	stdoutDispatcher (),
	stderrDispatcher (),
	           mutex ()
{
	static const std::map <std::string, std::string> codecs = {
		std::pair ("PNG",   "png"),
		std::pair ("H.264", "libx264"),
		std::pair ("Xvid",  "libxvid"),
	};

	if (basic::toupper (codec, std::locale::classic ()) == "WEB")
	{
		// WEBM for web
		//ffmpeg -i original.mp4 -c:v libvpx -preset slow -s 1024x576 -qmin 0 -qmax 50 -an -b:v 400K -pass 1 homepage.webm

		// MP4 for web
		//ffmpeg -i original.mp4 -c:v libx264 -preset slow -s 1024x576 -an -b:v 370K homepage.mp4

		width  += width  % 2;
		height += height % 2;

		const auto size = basic::to_string (width, std::locale::classic ()) + "x" + basic::to_string (height, std::locale::classic ());

		command = { "ffmpeg",
		            "-y", // Overwrite output files without asking.
		            "-f", "image2pipe",
		            "-vcodec", "png",
		            "-r", basic::to_string (frameRate, std::locale::classic ()),
		            "-i", "-",
		            "-c:v", "libx264",
		            "-preset", "slow",
		            "-s", size,
		            "-an",
		            "-b:v", "370K",
		            "-r", basic::to_string (frameRate, std::locale::classic ()),
		            "-frames:v", basic::to_string (duration, std::locale::classic ()),
		            filename .path () };
	}
	else
	{
		const auto iter   = codecs .find (codec);
		const auto vcodec = iter not_eq codecs .end () ? iter -> second : "png";
	
		command = { "ffmpeg",
		            "-y", // Overwrite output files without asking.
		            "-f", "image2pipe",
		            "-vcodec", "png",
		            "-r", basic::to_string (frameRate, std::locale::classic ()),
		            "-i", "-",
		            "-vcodec", vcodec,
		            "-q:v", "0",
		            "-r", basic::to_string (frameRate, std::locale::classic ()),
		            "-frames:v", basic::to_string (duration, std::locale::classic ()),
		            filename .path () };
	}
}

std::string
VideoEncoder::getStdout ()
{
	std::lock_guard <std::mutex> lock (mutex);

	return std::move (stdout);
}

std::string
VideoEncoder::getStderr ()
{
	std::lock_guard <std::mutex> lock (mutex);

	return std::move (stderr);
}

void
VideoEncoder::on_stdout (const std::string & string)
{
	std::lock_guard <std::mutex> lock (mutex);

	stdout .insert (stdout .end (), string .begin (), string .end ());

	stdoutDispatcher .emit ();
}

void
VideoEncoder::on_stderr (const std::string & string)
{
	std::lock_guard <std::mutex> lock (mutex);

	stderr .insert (stderr .end (), string .begin (), string .end ());

	stderrDispatcher .emit ();
}

void
VideoEncoder::open ()
{
	pipe .open (command);
	pipe .read (50);
}

void
VideoEncoder::write (Magick::Image & image)
{
	Magick::Blob blob;

	image .magick ("PNG");
	image .write (&blob);

	pipe .read (50);
	pipe .write (static_cast <const char*> (blob .data ()), blob .length ());
}

void
VideoEncoder::kill ()
{
	pipe .kill ();
}

bool
VideoEncoder::close ()
{
	return pipe .close ();
}

VideoEncoder::~VideoEncoder ()
{
	kill ();
	close ();
}

} // puck
} // titania
