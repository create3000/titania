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

#include "Pipe.h"

#include <Titania/OS.h>
#include <Titania/String.h>

#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <poll.h>
#include <vector>

#include <sys/socket.h>

#include <Titania/LOG.h>

namespace titania {
namespace puck {

const size_t Pipe::bufferSize = 1024;

Pipe::Pipe (const PipeCallback & stdoutCallback, const PipeCallback & stderrCallback) :
	stdoutCallback (stdoutCallback),
	stderrCallback (stderrCallback),
	           pid (0),
	         stdin (0),
	        stdout (0),
	        stderr (0),
	        opened (false),
	        buffer (bufferSize) 
{ }

void
Pipe::open (const std::string & command)
throw (std::runtime_error)
{
	__LOG__ << command << std::endl;

	if (opened)
		close ();

	if ((pid = os::popen3 (command, &stdin, &stdout, &stderr)) <= 0)
		throw std::runtime_error ("Couldn't open command '" +  command+ "'.");

	read (5);

	opened = true;
}

/***
 * Returns a value greater than zero on success. Zero on timeout and -1 if an error occured.
 */
int32_t
Pipe::wait (const int32_t fd, const int32_t timeout)
{
	struct pollfd p = { .fd = fd, .events = POLLIN };

	return ::poll (&p, 1, timeout);
}

void
Pipe::read (const int32_t timeout)
{
	int32_t bytesRead = 0;

	while (timeout == 0 or wait (stdout, timeout) > 0)
	{
		if ((bytesRead = ::read (stdout, buffer .data (), sizeof (char) * bufferSize)) > 0)
		{
			if (stdoutCallback)
				stdoutCallback (std::string (buffer .data (), bytesRead));
			else
				std::cout .write (buffer .data (), bytesRead);
		}
		else if (timeout == 0)
			break;
	}

	while (timeout == 0 or wait (stderr, timeout) > 0)
	{
		if ((bytesRead = ::read (stderr, buffer .data (), sizeof (char) * bufferSize)) > 0)
		{
			if (stderrCallback)
				stderrCallback (std::string (buffer .data (), bytesRead));
			else
				std::clog .write (buffer .data (), bytesRead);
		}
		else if (timeout == 0)
			break;
	}
}

void
Pipe::write (const char* data, const size_t length)
throw (std::runtime_error)
{
	const int32_t bytes        = length;
	const int32_t bytesWritten = ::write (stdin, data, bytes);

	read (5);

	if (bytesWritten not_eq bytes)
	{
		if (bytesWritten < 0)
			throw std::runtime_error ("Write to pipe failed: " + std::string (strerror (errno)) + ".");
		else
			throw std::runtime_error ("Write to pipe failed: " + std::to_string (bytes) + " bytes received, " + std::to_string (bytesWritten) + " bytes witen.");
	}
}

bool
Pipe::close ()
{
	if (not opened)
		return false;

	opened = false;

	::close (stdin);

	read (0);

	::close (stdout);
	::close (stderr);

	int32_t status = 0;

	::waitpid (pid, &status, 0);

	if (status)
		return false;

	return true;
}

Pipe::~Pipe ()
{
	close ();
}

} // puck
} // titania
