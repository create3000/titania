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

#include "ExternalTool.h"

#include "../../Bits/File.h"
#include "../../Bits/Pipe.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Widgets/Console/Console.h"

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

ExternalTool::ExternalTool (X3DBrowserWindow* const browserWindow,
                            const std::string & name,
                            const std::string & saveType,
                            const std::string & inputType,
                            const std::string & inputEncoding,
                            const std::string & outputType,
                            const std::string & command) :
	X3D::X3DInterruptibleThread (),
	             X3D::X3DInput  (),
	            sigc::trackable (),
	              browserWindow (browserWindow),
	                       name (name),
	                   saveType (saveType),
	                  inputType (inputType),
	              inputEncoding (inputEncoding),
	                 outputType (outputType),
	                    command (command),
	                     thread (),
	                      mutex (),
	                     stdout (),
	                     stderr (),
	                      queue (),
	           stdoutDispatcher (),
	           stderrDispatcher (),
	             doneDispatcher ()
{
	stdoutDispatcher .connect (sigc::mem_fun (this, &ExternalTool::on_stdout));
	stderrDispatcher .connect (sigc::mem_fun (this, &ExternalTool::on_stderr));
	doneDispatcher   .connect (sigc::mem_fun (this, &ExternalTool::on_done));
}

void
ExternalTool::start ()
{
	if (outputType == "DISPLAY_IN_CONSOLE")
		browserWindow -> getCurrentBrowser () -> getConsole () -> log ("\nRunning tool »", name, "«.\n");

	saveScenes ();

	thread = std::thread (&ExternalTool::run,
	                      this,
	                      Glib::get_home_dir (),
	                      command,
	                      getEnvironment (),
	                      getInput (),
	                      outputType not_eq "NOTHING");
}

void
ExternalTool::run (const std::string & workingDirectory,
                   const std::string & command,
                   const std::vector <std::string> & environment,
                   const std::string & input,
                   const bool processOutput)
{
	using namespace std::placeholders;

	const auto stdoutCallback = std::bind (&ExternalTool::on_stdout_async, this, _1);
	const auto stderrCallback = std::bind (&ExternalTool::on_stderr_async, this, _1);

	// Process input.

	Pipe pipe (processOutput ? stdoutCallback : PipeCallback (), stderrCallback);

	try
	{
		const auto flatpak_spawn = Glib::find_program_in_path ("flatpak-spawn");

		std::vector <std::string> commandWithArgs;

		if (not flatpak_spawn .empty ())
		{
			commandWithArgs .emplace_back ("flatpak-spawn");
			commandWithArgs .emplace_back ("--host");

			for (const auto & variable : environment)
				commandWithArgs .emplace_back ("--env=" + variable);
		}

		commandWithArgs .emplace_back (command);

		pipe .open (workingDirectory, commandWithArgs, environment);
		pipe .read (100);
		pipe .write (input .data (), input .size ());
		pipe .close (Pipe::STDIN);

		do
		{
			checkForInterrupt ();
			pipe .read (100);
		}
		while (pipe .isRunning ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;

		pipe .kill ();
	}

	doneDispatcher .emit ();
}

void
ExternalTool::on_stdout_async (const std::string & string)
{
	checkForInterrupt ();

	std::lock_guard <std::mutex> lock (mutex);

	// Queue begin
	auto first = 0;
	auto last  = string .find ('\0', first);

	while (last not_eq std::string::npos)
	{
		queue .emplace_back (stdout + string .substr (first, last - first));

		stdout .clear ();

		first = last + 1;
		last  = string .find ('\0', first);
	}
	// Queue end

	stdout .append (string .substr (first));

	stdoutDispatcher .emit ();
}

void
ExternalTool::on_stderr_async (const std::string & string)
{
	checkForInterrupt ();

	std::lock_guard <std::mutex> lock (mutex);

	stderr .append (string);

	stderrDispatcher .emit ();
}

void
ExternalTool::on_stdout ()
{
	std::lock_guard <std::mutex> lock (mutex);

	// Queue begin
	while (queue .size ())
	{
		processOutput (queue .front ());
		queue .pop_front ();
	}
	// Queue end

	if (outputType == "DISPLAY_IN_CONSOLE")
	{
		processOutput (stdout);
		stdout .clear ();
	}
}

void
ExternalTool::on_stderr ()
{
	std::lock_guard <std::mutex> lock (mutex);

	browserWindow -> getCurrentBrowser () -> getConsole () -> error (stderr);

	stderr .clear ();
}

void
ExternalTool::on_done ()
{
	std::lock_guard <std::mutex> lock (mutex);

	processOutput (stdout);

	stdout .clear ();

	if (outputType == "DISPLAY_IN_CONSOLE")
		browserWindow -> getCurrentBrowser () -> getConsole () -> log ("Tool »", name, "« finished.\n");
}

void
ExternalTool::saveScenes ()
{
	if (saveType == "NOTHING")
		;
	else if (saveType == "CURRENT_SCENE")
	{
		if (browserWindow -> getCurrentPage () -> getModified ())
			browserWindow -> on_save_activated ();
	}
	else if (saveType == "ALL_SCENES")
	{
		const auto currentPage = browserWindow -> getCurrentPage () -> getPageNumber ();
		const auto pages       = browserWindow -> getPages ();

		for (const auto & page : pages)
		{
			if (not page -> getModified ())
				continue;

			browserWindow -> getBrowserNotebook () .set_current_page (page -> getPageNumber ());
			browserWindow -> on_save_activated ();
		}

		browserWindow -> getBrowserNotebook () .set_current_page (currentPage);
	}
}

std::string
ExternalTool::getInput () const
{
	if (inputType == "CURRENT_SCENE")
	{
		return X3D::X3DEditor::exportScene ( browserWindow -> getCurrentScene (), inputEncoding);
	}
	else if (inputType == "SELECTION")
	{
		const auto & selection        = browserWindow -> getSelection () -> getNodes ();
		const auto & executionContext = browserWindow -> getCurrentContext ();

		return X3D::X3DEditor::exportNodes (executionContext, selection, inputEncoding, false);
	}

	return "";
}

std::vector <std::string>
ExternalTool::getEnvironment () const
{
	std::vector <std::string> environment = Pipe::getEnvironment ();

	Configuration projectsEditor ("Sidebar.FilesEditor.ProjectsEditor");

	const auto projects = projectsEditor .getItem <X3D::MFString> ("projects");
	const auto file     = Gio::File::create_for_uri (browserWindow -> getCurrentContext () -> getWorldURL ());

	if (file -> has_parent ())
	{
		const auto folder = file -> get_parent ();

		environment .emplace_back ("TITANIA_CURRENT_FOLDER="     + folder -> get_path ());
		environment .emplace_back ("TITANIA_CURRENT_FOLDER_URI=" + folder -> get_uri ());
		environment .emplace_back ("TITANIA_CURRENT_FILE="       + file   -> get_path ());
		environment .emplace_back ("TITANIA_CURRENT_FILE_URI="   + file   -> get_uri ());

		for (const auto & projectPath : projects)
		{
			const auto project = Gio::File::create_for_path (projectPath);

			if (File::isSubfolder (folder, project))
			{
				environment .emplace_back ("TITANIA_CURRENT_PROJECT="     + project -> get_path ());
				environment .emplace_back ("TITANIA_CURRENT_PROJECT_URI=" + project -> get_uri ());
			}
		}
	}

	return environment;
}

void
ExternalTool::processOutput (const std::string & stdout)
{
	try
	{
		// Process output.

		if (stdout .empty ())
			return;

		if (outputType == "NOTHING")
			;
		else if (outputType == "DISPLAY_IN_CONSOLE")
		{
			browserWindow -> getCurrentBrowser () -> getConsole () -> print (stdout);
		}
		else if (outputType == "CREATE_NEW_SCENE")
		{
			browserWindow -> open ("data:" + File::getContentType (stdout) .first + "," + stdout);
		}
		else if (outputType == "REPLACE_CURRENT_SCENE")
		{
			browserWindow -> load ("data:" + File::getContentType (stdout) .first + "," + stdout);
		}
		else if (outputType == "APPEND_TO_CURRENT_SCENE")
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Append Output From Tool »%s« To Current Scene", name .c_str ())));
			const auto nodes    = browserWindow -> import ({ "data:" + File::getContentType (stdout) .first + "," + stdout }, undoStep);

			if (not nodes .empty ())
			{
				X3D::X3DEditor::detachFromGroup (browserWindow -> getCurrentContext (), nodes, true, undoStep);

				browserWindow -> getSelection () -> setNodes (nodes, undoStep);
				browserWindow -> addUndoStep (undoStep);
			}
		}
		else if (outputType == "APPEND_TO_CURRENT_LAYER")
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Append Output From Tool »%s« To Current Layer", name .c_str ())));
			const auto nodes    = browserWindow -> import ({ "data:" + File::getContentType (stdout) .first + "," + stdout }, undoStep);

			if (not nodes .empty ())
			{
				browserWindow -> getSelection () -> setNodes (nodes, undoStep);
				browserWindow -> addUndoStep (undoStep);
			}
		}
		else if (outputType == "REPLACE_SELECTION")
		{
			const auto & selection = browserWindow -> getSelection () -> getNodes ();

			if (not selection .empty ())
			{
				const auto executionContext = X3D::X3DExecutionContextPtr (browserWindow -> getSelectionContext (selection));

				if (executionContext)
				{
					const auto undoStep     = std::make_shared <X3D::UndoStep> (_ (basic::sprintf (_ ("Assign Output From Tool »%s« To Selection"), name .c_str ())));
					const auto newSelection = X3D::X3DEditor::replaceNodes (executionContext, stdout, selection, true, undoStep);

					browserWindow -> getSelection () -> setNodes (newSelection, undoStep);
					browserWindow -> addUndoStep (undoStep);
				}
				else
				{
					browserWindow -> getCurrentBrowser () -> getConsole () -> warn ("Can't assing output of tool »", name, "« to nodes of multiple contexts.\n");
				}
			}
			else
			{
				browserWindow -> getCurrentBrowser () -> getConsole () -> warn ("No selection found to process output of tool »", name, "«.\n");
			}
		}
	}
	catch (const std::exception & error)
	{
		browserWindow -> getCurrentBrowser () -> getConsole () -> error ("Couldn't process output of tool »", name, "«.\n", error .what (), "\nOutput >>", stdout, "<<\n");
	}
}

void
ExternalTool::stop ()
{
	X3D::X3DInterruptibleThread::stop ();

	if (thread .joinable ())
		thread .join ();
}

ExternalTool::~ExternalTool ()
{
	stop ();
}

} // puck
} // titania
