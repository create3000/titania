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

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

ExternalTool::ExternalTool (X3DBrowserWindow* const browserWindow,
                            const std::string & id,
                            const std::string & name,
                            const std::string & inputType,
                            const std::string & inputEncoding,
                            const std::string & outputType,
                            const Glib::RefPtr <Gio::File> & command) :
	X3D::X3DInterruptibleThread (),
	             X3D::X3DInput  (),
	            sigc::trackable (),
	              browserWindow (browserWindow),
	                         id (id),
	                       name (name),
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
		browserWindow -> println ("Run tool »" + name + "«.");

	thread = std::thread (&ExternalTool::run,
	                      this,
	                      Glib::get_home_dir (),
	                      command -> get_path (),
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
		pipe .open (workingDirectory, { command }, environment);
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

	auto first = 0;
	auto last  = string .find ('\0', first);

	while (last not_eq std::string::npos)
	{
		queue .emplace_back (stdout + string .substr (first, last - first));

		stdout .clear ();

		first = last + 1;
		last  = string .find ('\0', first);
	}

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

	while (queue .size ())
	{
		processOutput (queue .front ());
		queue .pop_front ();
	}

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

	browserWindow -> print (stderr);

	stderr .clear ();
}

void
ExternalTool::on_done ()
{
	std::lock_guard <std::mutex> lock (mutex);

	processOutput (stdout);

	stdout .clear ();

	if (outputType == "DISPLAY_IN_CONSOLE")
		browserWindow -> println ("Tool »" + name + "« finished.");
}

std::vector <std::string> 
ExternalTool::getEnvironment () const
{
	std::vector <std::string> environment = Pipe::getEnvironment ();

	Configuration projectsEditor ("Sidebar.FilesEditor.ProjectsView");

	const auto projects = projectsEditor .getItem <X3D::MFString> ("projects");
	const auto file     = Gio::File::create_for_uri (browserWindow -> getCurrentContext () -> getWorldURL ());
	const auto folder   = file -> get_parent ();

	environment .emplace_back ("TITANIA_CURRENT_FOLDER=" + folder -> get_uri ());
	environment .emplace_back ("TITANIA_CURRENT_FILE="   + file -> get_uri ());

	for (const auto & projectPath : projects)
	{
		const auto project = Gio::File::create_for_path (projectPath);

		if (File::isSubfolder (folder, project))
			environment .emplace_back ("TITANIA_CURRENT_PROJECT=" + project -> get_uri ());
	}

	return environment;
}

std::string
ExternalTool::getInput () const
{
	if (inputType == "CURRENT_SCENE")
	{
		const auto & scene = browserWindow -> getCurrentScene ();

		// Write scene to stdin of tool.

		if (inputEncoding == "VRML")
			return scene -> toString ();
		else if (inputEncoding == "JSON")
			return scene -> toJSONString ();
		else
			return scene -> toXMLString ();
	}
	else if (inputType == "SELECTION")
	{
		const auto & selection        = browserWindow -> getSelection () -> getNodes ();
		const auto   executionContext = X3D::X3DExecutionContextPtr (selection .back () -> getExecutionContext ());

		// Export nodes to stream

		std::ostringstream osstream;

		X3D::X3DEditor::exportNodes (osstream, executionContext, selection, false);

		basic::ifilestream stream (osstream .str ());
	
		const auto scene = browserWindow -> getCurrentBrowser () -> createX3DFromStream (executionContext -> getWorldURL (), stream);
	
		scene -> addMetaData ("titania-add-metadata", "true");

		// Write scene to stdin of tool.

		if (inputEncoding == "VRML")
			return scene -> toString ();
		else if (inputEncoding == "JSON")
			return scene -> toJSONString ();
		else
			return scene -> toXMLString ();
	}

	return "";
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
			browserWindow -> print (stdout);
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
			const auto   undoStep  = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Replace Selection By Output From Tool »%s«", name .c_str ())));
			const auto & selection = browserWindow -> getSelection () -> getNodes ();

			if (not selection .empty ())
			{
				const auto   executionContext = X3D::X3DExecutionContextPtr (selection .back () -> getExecutionContext ());
				const auto   scene            = browserWindow -> getCurrentBrowser () -> createX3DFromString (stdout);
				const auto   nodes            = X3D::X3DEditor::importScene (executionContext, scene, undoStep);
	
				if (not nodes .empty ())
				{
					X3D::X3DEditor::createClone (executionContext, nodes .front (), selection, undoStep);
	 
					if (nodes .size () > 1)
						X3D::X3DEditor::removeNodesFromScene (executionContext, X3D::MFNode (nodes .begin () + 1, nodes .end ()), false, undoStep);
	
					browserWindow -> getSelection () -> setNodes ({ nodes .front () }, undoStep);
					browserWindow -> addUndoStep (undoStep);
				}
			}
			else
			{
				// Display message.
				browserWindow -> println ("No selection found to process output of tool »" + name + "«.");
			}
		}
	}
	catch (const std::exception & error)
	{
		browserWindow -> println ("Couldn't process output of tool »" + name + "«.");
		browserWindow -> println (error .what ());
		browserWindow -> println ("Output >>" + stdout + "<<");
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
