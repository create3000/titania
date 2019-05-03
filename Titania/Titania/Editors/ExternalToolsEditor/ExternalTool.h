/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_EDITORS_EXTERNAL_TOOLS_EDITOR_EXTERNAL_TOOL_H__
#define __TITANIA_EDITORS_EXTERNAL_TOOLS_EDITOR_EXTERNAL_TOOL_H__

#include <Titania/X3D/Thread/X3DInterruptibleThread.h>

#include <giomm.h>
#include <thread>
#include <mutex>

namespace titania {
namespace puck {

class Pipe;
class X3DBrowserWindow;

class ExternalTool :
	public X3D::X3DInterruptibleThread,
	public X3D::X3DInput,
	public sigc::trackable
{
public:

	///  @name Construction

	ExternalTool (X3DBrowserWindow* const browserWindow,
	              const std::string & name,
	              const std::string & saveType,
	              const std::string & inputType,
	              const std::string & inputEncoding,
	              const std::string & outputType,
	              const std::string & command);

	///  @name Operations

	void
	start ();

	void
	stop ();

	///  @name Signals

	Glib::Dispatcher &
	signal_done ()
	{ return doneDispatcher; }

	const Glib::Dispatcher &
	signal_done () const
	{ return doneDispatcher; }

	///  @name Destruction

	virtual
	~ExternalTool () final override;


private:

	///  @name Member types

	void
	run (const std::string & workingDirectory,
	     const std::string & command,
	     const std::vector <std::string> & environment,
	     const std::vector <std::string> & shortEnvironment,
	     const std::string & input,
	     const bool stdout);

	void
	on_stdout_async (const std::string & string);

	void
	on_stderr_async (const std::string & string);

	void
	on_stdout ();

	void
	on_stderr ();

	void
	on_done ();

	void
	saveScenes ();

	std::vector <std::string>
	getEnvironment (const bool defaultValues) const;

	std::string
	getInput () const;

	void
	processOutput (const std::string & stdout);

	///  @name Static members

	///  @name Members

	X3DBrowserWindow* const browserWindow;
	const std::string       name;
	const std::string       saveType;
	const std::string       inputType;
	const std::string       inputEncoding;
	const std::string       outputType;
	const std::string       command;

	std::thread              thread;
	std::mutex               mutex;
	std::string              stdout;
	std::string              stderr;
	std::deque <std::string> queue;

	Glib::Dispatcher stdoutDispatcher;
	Glib::Dispatcher stderrDispatcher;
	Glib::Dispatcher doneDispatcher;

};

} // puck
} // titania

#endif
