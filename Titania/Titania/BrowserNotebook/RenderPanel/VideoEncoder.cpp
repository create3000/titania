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

#include <Titania/OS.h>

#include <sys/wait.h>

namespace titania {
namespace puck {

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

void
VideoEncoder::open ()
throw (std::runtime_error)
{
	__LOG__ << command_with_args << std::endl;

	if (opened)
		close ();

	os::unlink (filename .path ());

	if ((pid = os::popen3 (command_with_args, &stdin, &stdout, &stderr)) <= 0)
		throw std::runtime_error ("Couldn't open command '" +  command_with_args+ "'.");

	opened = true;
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

} // puck
} // titania
