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

#include <Titania/String.h>

#include <glibmm.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <vector>

#include <Titania/LOG.h>

namespace titania {
namespace puck {

Pipe::Pipe (const PipeCallback & stdout_callback, const PipeCallback & stderr_callback) :
	m_stdout_callback (stdout_callback),
	m_stderr_callback (stderr_callback),
	            m_pid (0),
	          m_stdin (0),
	         m_stdout (0),
	         m_stderr (0),
	        m_is_open (false),
	         m_buffer (buffer_size) 
{
	const bool e = ignoreSigpipe ();

	if (e)
		__LOG__ << e << std::endl;
}

bool
Pipe::ignoreSigpipe ()
{
	// Ignore SIGPIPE.

	struct sigaction act;
	memset (&act, 0, sizeof (act));
	
	act .sa_handler = SIG_IGN;
	act .sa_flags   = SA_RESTART;

	return sigaction (SIGPIPE, &act, NULL);
}

std::vector <std::string> 
Pipe::getEnvironment ()
{
	std::vector <std::string> environment;

	for (const auto & key : Glib::listenv ())
		environment .emplace_back (key + "=" + Glib::getenv (key));

	return environment;
}

bool
Pipe::isRunning () const
{
	if (waitpid (WNOHANG))
		return false;

	if (kill (0))
		return false;

	return true;
}

void
Pipe::open (const std::vector <std::string> & command)
{
	open (Glib::get_home_dir (), command, getEnvironment ());
}

void
Pipe::open (const std::string & workingDirectory,
            const std::vector <std::string> & command,
            const std::vector <std::string> & environment)
{
	try
	{
		__LOG__ << basic::join (command, " ") << std::endl;

		if (m_is_open)
			close ();			

		// FD_CLOEXEC   This flag specifies that the file descriptor should be closed when an exec function is invoked.

		Glib::spawn_async_with_pipes (workingDirectory,
		                              command,
		                              environment,
		                              Glib::SPAWN_SEARCH_PATH | Glib::SPAWN_SEARCH_PATH_FROM_ENVP | Glib::SPAWN_CLOEXEC_PIPES | Glib::SPAWN_DO_NOT_REAP_CHILD,
		                              Glib::SlotSpawnChildSetup (),
		                              &m_pid,
		                              &m_stdin,
		                              &m_stdout,
		                              &m_stderr);
	
		m_is_open = true;
	}
	catch (const Glib::SpawnError & error)
	{
		throw std::runtime_error (error .what ());
	}
}

/***
 * Returns a value greater than zero on success. Zero on timeout and -1 if an error occured.
 */
int32_t
Pipe::poll (const int32_t fd, const int32_t timeout, const short events)
{
	struct pollfd p = { .fd = fd, .events = events };

	return ::poll (&p, 1, timeout);
}

void
Pipe::read (const int32_t timeout)
{
	while (poll (m_stdout, timeout, POLLIN) > 0)
	{
		const auto bytesRead = ::read (m_stdout, m_buffer .data (), sizeof (char) * buffer_size);

		if (bytesRead > 0)
		{
			if (m_stdout_callback)
				m_stdout_callback (std::string (m_buffer .data (), bytesRead));

			else
				std::cout .write (m_buffer .data (), bytesRead);
		}
		else
			break;
	}

	while (poll (m_stderr, timeout, POLLIN) > 0)
	{
		const auto bytesRead = ::read (m_stderr, m_buffer .data (), sizeof (char) * buffer_size);

		if (bytesRead > 0)
		{
			if (m_stderr_callback)
				m_stderr_callback (std::string (m_buffer .data (), bytesRead));

			else
				std::clog .write (m_buffer .data (), bytesRead);
		}
		else
			break;
	}
}

void
Pipe::write (const char* data, const size_t length)
{
	if (not m_is_open)
		throw std::runtime_error ("Write to closed pipe.");

	if (isRunning () and poll (m_stdin, 5, POLLOUT) >= 0)
	{
		const int32_t bytes        = length;
		const int32_t bytesWritten = ::write (m_stdin, data, bytes);

		if (bytesWritten not_eq bytes)
		{
			if (bytesWritten < 0)
				throw std::runtime_error ("Write to pipe failed: " + std::string (strerror (errno)) + ".");
			else
				throw std::runtime_error ("Write to pipe failed: " + std::to_string (bytes) + " bytes received, " + std::to_string (bytesWritten) + " bytes witen.");
		}
	}
	else
	{
		close ();

		throw std::runtime_error ("Broken pipe: write to pipe with no readers.");
	}
}

int32_t
Pipe::kill (const int32_t signal) const
{
	return ::kill (m_pid, signal);
}

int32_t
Pipe::waitpid (const int32_t options) const
{
	int32_t status = 0;

	::waitpid (m_pid, &status, options);

	return status;
}

bool
Pipe::close (const StreamType stream)
{
	switch (stream)
	{
		case STDIN:
		{
			::close (m_stdin);
			return false;
		}
		case STDOUT:
		{
			::close (m_stdout);
			return false;
		}
		case STDERR:
		{
			::close (m_stdout);
			return false;
		}
		case DEFAULT:
		{
			if (not m_is_open)
				return false;
		
			m_is_open = false;
		
			::close (m_stdin);
		
			do
			{
				read (50);
			}
			while (isRunning ());
		
			::close (m_stdout);
			::close (m_stderr);

			const auto status = waitpid (0);
		
			Glib::spawn_close_pid (m_pid);

			if (status)
				return false;

			return true;
		}
	}

	return false;
}

Pipe::~Pipe ()
{
	close ();
}

} // puck
} // titania
