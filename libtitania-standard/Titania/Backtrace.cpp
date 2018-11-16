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

#include "Backtrace.h"

#include <csignal>
#include <execinfo.h>
#include <iostream>

namespace titania {

// The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.

static signal_handler_index signal_handlers;

/*
 * backtrace handling
 */

static
void
backtrace_print (size_t size, int sig)
{
	// http://man7.org/linux/man-pages/man7/signal.7.html
	static const std::map <int32_t, std::pair <std::string, std::string>> sigs = {
		std::pair (SIGHUP,  std::pair ("SIGHUP",  "Hangup detected on controlling terminal or death of controlling process")),
		std::pair (SIGILL,  std::pair ("SIGILL",  "Illegal Instruction")),
		std::pair (SIGFPE,  std::pair ("SIGFPE",  "Floating-point exception")),
		std::pair (SIGKILL, std::pair ("SIGKILL", "Kill signal")),
		std::pair (SIGABRT, std::pair ("SIGABRT", "Abort signal from abort")),
		std::pair (SIGSEGV, std::pair ("SIGSEGV", "Invalid memory reference")),
		std::pair (SIGPIPE, std::pair ("SIGPIPE", "Broken pipe: write to pipe with no readers; see pipe")),
		std::pair (SIGTERM, std::pair ("SIGTERM", "Termination signal")),
	};

	const auto iter = sigs .find (sig);

	std::clog
		<< std::string (80, '#') << std::endl
		<< "#" << std::endl
		<< "# Backtrace" << std::endl
		<< "#" << std::endl
		<< "# Error: signal " << sig << " " << (iter not_eq sigs .end () ? iter -> second .first : "") << std::endl
		<< "#" << std::endl
		<< "# " << (iter not_eq sigs .end () ? iter -> second .second : "") << std::endl
		<< "#" << std::endl
		<< std::string (80, '#') << std::endl
		<< backtrace_symbols (size)
		<< std::endl;
}

static
void
handle_signal (int sig)
{
	const auto handlers = signal_handlers .find (sig);

	if (handlers not_eq signal_handlers .end ())
	{
		if (not handlers -> second .empty ())
		{
			for (const auto & handler : handlers -> second)
				handler ();
		}
	}
}

static
void
backtrace_signal_handler (int sig)
{
	handle_signal (sig);

	// print out all the frames to stderr
	backtrace_print (1000, sig);
	::exit (255);
}

static
void
backtrace_terminate_handler ()
{
	// print out all the frames to stderr
	backtrace_print (1000, SIGABRT);
	::exit (1);
}

void
install_backtrace ()
{
	// install our handler
	std::signal (SIGSEGV, &backtrace_signal_handler);
	std::signal (SIGABRT, &backtrace_signal_handler);
	std::signal (SIGPIPE, &backtrace_signal_handler);

	std::set_terminate (&backtrace_terminate_handler);
}

void
backtrace (size_t size)
{
	std::clog
		<< std::endl
		<< "======= Backtrace: =========" << std::endl
		<< backtrace_symbols (size) << std::endl
		<< std::endl;
}

std::string
backtrace_symbols (size_t size)
{
	void* array [size];

	// get void*'s for all entries on the stack
	size = ::backtrace (array, size);

	// Get all the frames.
	const auto symbols = ::backtrace_symbols (array, size);

	std::string string;

	for (size_t i = 0; i < size; ++ i)
	{
		string += symbols [i];
		string += "\n";
	}

	::free (symbols);

	return string;
}

/*
 * signal handling
 */

signal_handler_id
add_signal_handler (int sig, const signal_callback & callback)
{
	std::signal (sig, &backtrace_signal_handler);

	signal_handlers [sig] .emplace_back (callback);

	return -- signal_handlers [sig] .end ();
}

void
remove_signal_handler (int sig, const signal_handler_id id)
{
	signal_handlers [sig] .erase (id);
}

} // titania
