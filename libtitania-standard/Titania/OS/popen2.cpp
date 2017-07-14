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

#include "popen2.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mutex>

namespace titania {
namespace os {

static constexpr size_t READ  = 0;
static constexpr size_t WRITE = 1;

pid_t
popen2 (const char* const command, int* const stdin, int* const stdout)
{
	return popen3 (command, stdin, stdout, nullptr);
}

pid_t
popen3 (const char* const command, int* const stdin, int* const stdout, int* const stderr)
{
	int input [2], output [2], error [2];

	if (pipe (output) not_eq 0 or pipe (input) not_eq 0 or pipe (error) not_eq 0)
		return -1;

	const pid_t pid = fork ();

	if (pid < 0)
		return pid;

	if (pid == 0)
	{
		// Child process

		close (output [WRITE]);
		dup2 (output [READ], STDIN_FILENO);

		close (input [READ]);
		dup2 (input [WRITE], STDOUT_FILENO);
		
		close (error [READ]);
		dup2 (error [WRITE], STDERR_FILENO);

		execl ("/bin/sh", "sh", "-c", command, nullptr);
		perror ("execl");
		exit (1);
	}

	fcntl (output [WRITE], F_SETFD, FD_CLOEXEC);
	fcntl (input  [READ],  F_SETFD, FD_CLOEXEC);
	fcntl (error  [READ],  F_SETFD, FD_CLOEXEC);

	close (output [READ]);

	if (stdin)
		*stdin = output [WRITE];
	else
		close (output [WRITE]);

	close (input [WRITE]);

	if (stdout)
		*stdout = input [READ];
	else
		close (input [READ]);

	close (error [WRITE]);

	if (stderr)
		*stderr = error [READ];
	else
		close (error [READ]);

	return pid;
}

} // os
} // titania
