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

#include <sys/wait.h>
#include <vector>

#include <Titania/LOG.h>

namespace titania {
namespace puck {

const size_t Pipe::bufferSize = 1024;

Pipe::Pipe (const bool use_stdin, const bool use_stdout, const bool use_stderr) :
	   use_stdin (use_stdin),
	  use_stdout (use_stdout),
	  use_stderr (use_stderr),
	         pid (0),
	    fd_stdin (0),
	   fd_stdout (0),
	   fd_stderr (0),
	      opened (false),
	stdoutBuffer (),
	stderrBuffer (),
	      buffer (bufferSize) 
{ }

std::string
Pipe::stdout ()
{
	return std::move (stdoutBuffer);
}

std::string
Pipe::stderr ()
{
	return std::move (stderrBuffer);
}

void
Pipe::open (const std::string & command)
throw (std::runtime_error)
{
	__LOG__ << command << std::endl;

	if (opened)
		close ();

	if ((pid = os::popen3 (command, &fd_stdin, &fd_stdout, &fd_stderr)) <= 0)
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
	fd_set fdread;

	FD_ZERO (&fdread);
	FD_SET (fd, &fdread);

	struct timeval tv;

	tv .tv_sec  = timeout / 1000;
	tv .tv_usec = (timeout % 1000) * 1000;

	return select (fd + 1, &fdread, nullptr, nullptr, &tv);
}

void
Pipe::read (const int32_t timeout)
{
	int32_t bytesRead = 0;

	while (timeout == 0 or wait (fd_stdout, timeout) > 0)
	{
		if ((bytesRead = ::read (fd_stdout, buffer .data (), sizeof (char) * bufferSize)) > 0)
		{
			if (use_stdout)
				stdoutBuffer .append (buffer .data (), bytesRead);
			else
				std::cout .write (buffer .data (), bytesRead);
		}
		else if (timeout == 0)
			break;
	}

	while (timeout == 0 or wait (fd_stderr, timeout) > 0)
	{
		if ((bytesRead = ::read (fd_stderr, buffer .data (), sizeof (char) * bufferSize)) > 0)
		{
			if (use_stderr)
				stderrBuffer .append (buffer .data (), bytesRead);
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
	const int32_t bytesWritten = ::write (fd_stdin, data, bytes);

	read (5);

	if (bytesWritten not_eq bytes)
		throw std::runtime_error ("Write to pipe faile: " + std::to_string (bytes) + " bytes received, " + std::to_string (bytesWritten) + " bytes witen.");
}

bool
Pipe::close ()
{
	if (not opened)
		return false;

	opened = false;

	::close (fd_stdin);

	read (0);

	::close (fd_stdout);
	::close (fd_stderr);

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
