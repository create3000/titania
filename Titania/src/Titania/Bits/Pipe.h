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

#ifndef __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_PIPE_H__
#define __TITANIA_BROWSER_NOTEBOOK_RENDER_PANEL_PIPE_H__

#include <atomic>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>

#include <sys/wait.h>

namespace titania {
namespace puck {

using PipeCallback = std::function <void (const std::string &)>;

class Pipe
{
public:

	///  @name Member types

	enum StreamType
	{
		DEFAULT,
		STDIN,
		STDOUT,
		STDERR	
	};

	///  @name Construction

	explicit
	Pipe (const PipeCallback & stdout_callback = PipeCallback (),
	      const PipeCallback & stderr_callback = PipeCallback ());

	///  @name Member access

	static
	std::vector <std::string> 
	getEnvironment ();

	bool
	isRunning () const;

	bool
	isOpen () const
	{ return m_is_open; }

	int32_t
	getPid () const
	{ return m_pid; }

	///  @name Operations

	void
	open (const std::vector <std::string> & command);

	void
	open (const std::string & workingDirectory,
	      const std::vector <std::string> & command,
	      const std::vector <std::string> & environment);

	void
	read (const int32_t timeout);

	void
	write (const char* data, const size_t length);

	int32_t
	kill (const int32_t signal = SIGKILL) const;
	
	int32_t
	waitpid (const int32_t options) const;

	bool
	close (const StreamType stream = StreamType::DEFAULT);

	///  @name Destruction

	virtual
	~Pipe ();


private:

	///  @name Operations

	int32_t
	poll (const int32_t fd, const int32_t timeout, const short events);

	static
	bool
	ignoreSigpipe ();

	///  @name Static members

	static constexpr size_t buffer_size = 1024;

	///  @name Members

	const PipeCallback m_stdout_callback;
	const PipeCallback m_stderr_callback;

	pid_t              m_pid;
	int32_t            m_stdin;
	int32_t            m_stdout;
	int32_t            m_stderr;
	std::atomic <bool> m_is_open;

	std::vector <char> m_buffer;

};

} // puck
} // titania

#endif
