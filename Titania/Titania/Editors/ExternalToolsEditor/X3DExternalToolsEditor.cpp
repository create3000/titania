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

#include "X3DExternalToolsEditor.h"

#include "../../Bits/Pipe.h"
#include "../../Browser/BrowserSelection.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"

#include <Titania/String.h>

#include <regex>

namespace titania {
namespace puck {

class X3DExternalToolsEditor::Columns {
public:

	static constexpr size_t ID            = 0;
	static constexpr size_t NAME          = 1;
	static constexpr size_t MASK          = 2;
	static constexpr size_t KEY           = 3;
	static constexpr size_t SAVE_TYPE     = 4;
	static constexpr size_t INPUT_TYPE    = 5;
	static constexpr size_t OUTPUT_TYPE   = 6;
	static constexpr size_t APPLICABILITY = 7;
	static constexpr size_t INPUT_FORMAT  = 8;

};

std::string X3DExternalToolsEditor::stdout = "";

X3DExternalToolsEditor::X3DExternalToolsEditor () :
	X3DExternalToolsEditorInterface ()
{ }

Glib::RefPtr <Gio::File>
X3DExternalToolsEditor::getToolsFolder ()
{
	const auto folder = Gio::File::create_for_path (config_dir ("Tools"));

	if (not folder -> query_exists ())
		folder -> make_directory_with_parents ();

	return folder;
}

std::string
X3DExternalToolsEditor::createTool ()
{
	const auto  folder   = getToolsFolder ();
	const auto  templ    = folder -> get_child ("XXXXXX.txt");
	std::string filename = templ -> get_path ();

	::close (Glib::mkstemp (filename));

	const auto id   = filename .substr (filename .size () - 10, 6);
	const auto file = Gio::File::create_for_path (filename);

	::chmod (file -> get_path () .c_str (), 0711);
	//fs::permissions (file -> get_path (), fs::perms::owner_excec, fs::perm_options::add); // C++17

	return id;
}

void
X3DExternalToolsEditor::removeTool (const Gtk::TreeIter & iter)
{
	const auto id     = getId (iter);
	const auto folder = getToolsFolder ();
	const auto file   = folder -> get_child (id + ".txt");
	
	try
	{
		file -> remove ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	for (const auto & child : iter -> children ())
		removeTool (child);
}

void
X3DExternalToolsEditor::setId (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::ID, value);
}

std::string
X3DExternalToolsEditor::getId (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::ID, value);

	return value;
}

void
X3DExternalToolsEditor::setName (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::NAME, value);
}

std::string
X3DExternalToolsEditor::getName (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::NAME, value);

	return value;
}

void
X3DExternalToolsEditor::setText (const std::string & id, const std::string & text) const
{
	const auto folder   = getToolsFolder ();
	const auto file     = folder -> get_child (id + ".txt");
	auto       ofstream = std::ofstream (file -> get_path ());

	ofstream << text;
}

std::string
X3DExternalToolsEditor::getText (const std::string & id)
{
	const auto folder = getToolsFolder ();
	const auto file   = folder -> get_child (id + ".txt");

	return Glib::file_get_contents (file -> get_path ());
}

void
X3DExternalToolsEditor::setSaveType (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::SAVE_TYPE, value);
}

std::string
X3DExternalToolsEditor::getSaveType (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::SAVE_TYPE, value);

	return value;
}

void
X3DExternalToolsEditor::setInputType (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::INPUT_TYPE, value);
}

std::string
X3DExternalToolsEditor::getInputType (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::INPUT_TYPE, value);

	return value;
}

void
X3DExternalToolsEditor::setInputFormat (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::INPUT_FORMAT, value);
}

std::string
X3DExternalToolsEditor::getInputFormat (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::INPUT_FORMAT, value);

	return value;
}

void
X3DExternalToolsEditor::setOutputType (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::OUTPUT_TYPE, value);
}

std::string
X3DExternalToolsEditor::getOutputType (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::OUTPUT_TYPE, value);

	return value;
}

void
X3DExternalToolsEditor::setApplicabilityType (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::APPLICABILITY, value);
}

std::string
X3DExternalToolsEditor::getApplicabilityType (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::APPLICABILITY, value);

	return value;
}

std::string
X3DExternalToolsEditor::getContentType (const std::string & data)
{
	bool        result_uncertain;
	std::string contentType = Gio::content_type_guess ("", (guchar*) &data [0], data .size (), result_uncertain);

	return contentType;
}

void
X3DExternalToolsEditor::setLanguage (const std::string & text)
{
	try
	{
		const auto contextType = getContentType (text);
		const auto language    = Gsv::LanguageManager::get_default () -> guess_language ("", contextType);

		getSourceView () .get_source_buffer () -> set_language (language);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DExternalToolsEditor::restoreTree ()
{
	try
	{
		const auto browser   = X3D::createBrowser ();
		const auto scene     = browser -> createX3DFromString (Glib::file_get_contents (config_dir ("tools.x3d")));
		const auto worldInfo = scene -> getNamedNode <X3D::WorldInfo> ("Configuration");
		auto       expandeds = std::vector <Gtk::TreePath> ();
	
		restoreTree (worldInfo, "/Tools/Tree/children", Gtk::TreeIter (), expandeds);
	
		for (const auto & path : expandeds)
			getTreeView () .expand_row (path, false);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DExternalToolsEditor::restoreTree (const X3D::X3DPtr <X3D::WorldInfo> & worldInfo,
                                     const std::string & key,
                                     const Gtk::TreeIter & parent,
                                     std::vector <Gtk::TreePath> & expandeds)
{
	const auto children = worldInfo -> getMetaData <X3D::MFNode> (key);

	for (size_t i = 0, size = children .size (); i < size; ++ i)
	{
		const auto k                 = key + "/" + basic::to_string (i, std::locale::classic ());
		auto       path              = Gtk::TreePath (worldInfo -> getMetaData <std::string> (k + "/path"));
		const auto expanded          = worldInfo -> getMetaData <bool> (k + "/expanded");
		const auto id                = worldInfo -> getMetaData <std::string> (k + "/id");
		const auto name              = worldInfo -> getMetaData <std::string> (k + "/name");
		const auto saveType          = worldInfo -> getMetaData <std::string> (k + "/saveType");
		const auto inputType         = worldInfo -> getMetaData <std::string> (k + "/inputType");
		const auto inputFormat       = worldInfo -> getMetaData <std::string> (k + "/inputFormat");
		const auto outputType        = worldInfo -> getMetaData <std::string> (k + "/outputType");
		const auto applicabilityType = worldInfo -> getMetaData <std::string> (k + "/applicabilityType");
		const auto iter              = getTreeStore () -> iter_is_valid (parent) ? getTreeStore () -> append (parent -> children ()) : getTreeStore () -> append ();

		if (id .empty ())
			continue;

		if (expanded)
			expandeds .emplace_back (path);

		setId                (iter, id);
		setName              (iter, name);
		setSaveType          (iter, saveType);
		setInputType         (iter, inputType);
		setInputFormat       (iter, inputFormat);
		setOutputType        (iter, outputType);
		setApplicabilityType (iter, applicabilityType);

		restoreTree (worldInfo, k + "/children", iter, expandeds);
	}
}

void
X3DExternalToolsEditor::saveTree ()
{
	const auto browser   = X3D::createBrowser ();
	const auto scene     = browser -> getExecutionContext ();
	const auto worldInfo = scene -> createNode <X3D::WorldInfo> ();
	
	scene -> addNamedNode ("Configuration", worldInfo);
	scene -> getRootNodes () .emplace_back (worldInfo);

	saveTree (getTreeStore () -> children (), worldInfo);

	std::ofstream ofstream (config_dir ("tools.x3d"));

	browser -> getExecutionContext () -> toXMLStream (ofstream);
}

void
X3DExternalToolsEditor::saveTree (const Gtk::TreeNodeChildren & children, const X3D::X3DPtr <X3D::WorldInfo> & worldInfo) const
{
	static const std::regex colon (R"/(:)/");

	for (const auto & child : children)
	{
		const auto path = getTreeStore () -> get_path (child);
		const auto key  = "/Tools/Tree/children/" + std::regex_replace (path .to_string () .raw (), colon, "/children/");

		worldInfo -> setMetaData <std::string> (key + "/path",              path .to_string ());
		worldInfo -> setMetaData <bool>        (key + "/expanded",          getTreeView () .row_expanded (path));
		worldInfo -> setMetaData <std::string> (key + "/id",                getId (child));
		worldInfo -> setMetaData <std::string> (key + "/name",              getName (child));
		worldInfo -> setMetaData <std::string> (key + "/saveType",          getSaveType (child));
		worldInfo -> setMetaData <std::string> (key + "/inputType",         getInputType (child));
		worldInfo -> setMetaData <std::string> (key + "/inputFormat",       getInputFormat (child));
		worldInfo -> setMetaData <std::string> (key + "/outputType",        getOutputType (child));
		worldInfo -> setMetaData <std::string> (key + "/applicabilityType", getApplicabilityType (child));

		saveTree (child -> children (), worldInfo);
	}
}

void
X3DExternalToolsEditor::createMenu (X3DBrowserWindow* const browserWindow, Gtk::MenuItem & menuItem)
{
	try
	{
		const auto menu = menuItem .get_submenu ();
	
		for (const auto widget : menu -> get_children ())
		{
			if (widget -> get_name () not_eq "X3DExternalToolsEditor")
				continue;
	
			menu -> remove (*widget);
		}
	
		const auto browser   = X3D::createBrowser ();
		const auto scene     = browser -> createX3DFromString (Glib::file_get_contents (config_dir ("tools.x3d")));
		const auto worldInfo = scene -> getNamedNode <X3D::WorldInfo> ("Configuration");
	
		createMenu (browserWindow, worldInfo, "/Tools/Tree/children", &menuItem, menu);
	
		menuItem .show_all ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DExternalToolsEditor::createMenu (X3DBrowserWindow* const browserWindow,
                                    const X3D::X3DPtr <X3D::WorldInfo> & worldInfo,
                                    const std::string & key,
                                    Gtk::MenuItem* const menuItem,
	                                 Gtk::Menu* menu)
{
	const auto children = worldInfo -> getMetaData <X3D::MFNode> (key);

	if (children .empty ())
		return;

	if (not menu)
	{
		menu = Gtk::manage (new Gtk::Menu ());
		menuItem -> set_submenu (*menu);
	}

	const auto & worldUrl       = browserWindow -> getCurrentPage () -> getWorldURL ();
	const auto & selection      = browserWindow -> getSelection () -> getNodes ();
	const auto   separatorRegex = std::regex  (R"(^-*$)");
	auto         separatorMatch = std::smatch ();

	for (size_t i = 0, size = children .size (); i < size; ++ i)
	{
		const auto k                 = key + "/" + basic::to_string (i, std::locale::classic ());
		const auto id                = worldInfo -> getMetaData <std::string> (k + "/id");
		const auto name              = worldInfo -> getMetaData <std::string> (k + "/name");
		const auto inputType         = worldInfo -> getMetaData <std::string> (k + "/inputType");
		const auto outputType        = worldInfo -> getMetaData <std::string> (k + "/outputType");
		const auto applicabilityType = worldInfo -> getMetaData <std::string> (k + "/applicabilityType");

		if (id .empty ())
			continue;

		if (selection .empty ())
		{
			if (inputType == "SELECTION")
				continue;

			if (outputType == "REPLACE_SELECTION")
				continue;
		}

		if (worldUrl .empty ())
		{
			if (applicabilityType == "ALL_SCENES_EXCEPT_UNTITLED_ONES")
				continue;

			if (applicabilityType == "LOCAL_FILES_ONLY")
				continue;

			if (applicabilityType == "REMOTE_FILES_ONLY")
				continue;
		}
		else
		{
			if (applicabilityType == "UNTITLED_SCENS_ONLY")
				continue;

			if (worldUrl .is_local ())
			{
				if (applicabilityType == "REMOTE_FILES_ONLY")
					continue;
			}
			else
			{
				if (applicabilityType == "LOCAL_FILES_ONLY")
					continue;
			}
		}

		const auto separator = std::regex_match (name, separatorMatch, separatorRegex);
		const auto menuItem  = Gtk::manage (separator ? new Gtk::SeparatorMenuItem () : new Gtk::MenuItem (name));

		menuItem -> signal_activate () .connect (sigc::bind (sigc::ptr_fun (&X3DExternalToolsEditor::launchTool), browserWindow, k));
		menuItem -> set_name ("X3DExternalToolsEditor");

		menu -> append (*menuItem);

		if (separator)
			continue;

		createMenu (browserWindow, worldInfo, k + "/children", menuItem, nullptr);
	}
}

void
X3DExternalToolsEditor::launchTool (X3DBrowserWindow* const browserWindow, const std::string & k)
{
	try
	{
		using namespace std::placeholders;

		const auto browser        = X3D::createBrowser ();
		const auto scene          = browser -> createX3DFromString (Glib::file_get_contents (config_dir ("tools.x3d")));
		const auto worldInfo      = scene -> getNamedNode <X3D::WorldInfo> ("Configuration");
		const auto id             = worldInfo -> getMetaData <std::string> (k + "/id");
		const auto name           = worldInfo -> getMetaData <std::string> (k + "/name");
		const auto inputType      = worldInfo -> getMetaData <std::string> (k + "/inputType");
		const auto inputFormat    = worldInfo -> getMetaData <std::string> (k + "/inputFormat");
		const auto outputType     = worldInfo -> getMetaData <std::string> (k + "/outputType");
		const auto folder         = getToolsFolder ();
		const auto file           = folder -> get_child (id + ".txt");
		const auto command        = std::vector <std::string> ({ file -> get_path () });
		const auto stdoutCallback = std::bind (&X3DExternalToolsEditor::on_stdout, browserWindow, getConsoleAction (outputType),  _1);
		const auto stderrCallback = std::bind (&X3DExternalToolsEditor::on_stderr, browserWindow, _1);

		Pipe pipe (stdoutCallback, stderrCallback);

		pipe .open (Glib::get_home_dir (), command, { });

		// Process input.

		if (inputType == "CURRENT_SCENE")
		{
			const auto & scene = browserWindow -> getCurrentScene ();

			// Write scene to stdin of tool.

			std::string input;

			if (inputFormat == "VRML")
				input = scene -> toString ();
			else if (inputFormat == "JSON")
				input = scene -> toJSONString ();
			else
				input = scene -> toXMLString ();

			pipe .write (input .data (), input .size ());
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

			std::string input;

			if (inputFormat == "VRML")
				input = scene -> toString ();
			else if (inputFormat == "JSON")
				input = scene -> toJSONString ();
			else
				input = scene -> toXMLString ();

			pipe .write (input .data (), input .size ());
		}

		pipe .close ();

		// Process output.

		if (outputType == "CREATE_NEW_SCENE")
		{
			browserWindow -> open ("data:" + getContentType (stdout) + "," + stdout);
		}
		else if (outputType == "REPLACE_CURRENT_SCENE")
		{
			browserWindow -> load ("data:" + getContentType (stdout) + "," + stdout);
		}
		else if (outputType == "APPEND_TO_CURRENT_SCENE")
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Append Output From Tool »%s« To Current Scene", name .c_str ())));
			const auto nodes    = browserWindow -> import ({ "data:" + getContentType (stdout) + "," + stdout }, undoStep);

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
			const auto nodes    = browserWindow -> import ({ "data:" + getContentType (stdout) + "," + stdout }, undoStep);

			if (not nodes .empty ())
			{
				browserWindow -> getSelection () -> setNodes (nodes, undoStep);
				browserWindow -> addUndoStep (undoStep);
			}
		}
		else if (outputType == "REPLACE_SELECTION")
		{
			const auto   undoStep         = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Replace Selection By Output From Tool »%s«", name .c_str ())));
			const auto & selection        = browserWindow -> getSelection () -> getNodes ();
			const auto   executionContext = X3D::X3DExecutionContextPtr (selection .back () -> getExecutionContext ());
			const auto   scene            = browserWindow -> getCurrentBrowser () -> createX3DFromString (stdout);
			const auto   nodes            = X3D::X3DEditor::importScene (executionContext, executionContext, executionContext -> getRootNodes (), scene, undoStep);

			if (not nodes .empty ())
			{
				X3D::X3DEditor::createClone (executionContext, nodes .front (), selection, undoStep);
 
				if (nodes .size () > 1)
					X3D::X3DEditor::removeNodesFromScene (executionContext, X3D::MFNode (nodes .begin () + 1, nodes .end ()), false, undoStep);

				executionContext -> getRootNodes () .resize (executionContext -> getRootNodes () .size () - nodes .size ());

				browserWindow -> getSelection () -> setNodes ({ nodes .front () }, undoStep);
				browserWindow -> addUndoStep (undoStep);
			}
		}
	}
	catch (const std::exception & error)
	{
		browserWindow -> println ("Couldn't execute tool.");
		browserWindow -> println (error .what ());
	}

	stdout .clear ();
}

void
X3DExternalToolsEditor::on_stdout (X3DBrowserWindow* const browserWindow, const ConsoleAction action, const std::string & string)
{
	switch (action)
	{
		case ConsoleAction::NOTHING:
			return;
		case ConsoleAction::STDOUT:
			stdout .append (string);
			return;
		case ConsoleAction::PRINT:
			browserWindow -> print (string);
			return;
	}
}

void
X3DExternalToolsEditor::on_stderr (X3DBrowserWindow* const browserWindow, const std::string & string)
{
	browserWindow -> print (string);
}

X3DExternalToolsEditor::ConsoleAction
X3DExternalToolsEditor::getConsoleAction (const std::string & outputType)
{
	 if (outputType == "DISPLAY_IN_CONSOLE")
		return ConsoleAction::PRINT;

	 if (outputType == "NOTHING")
		return ConsoleAction::NOTHING;

	return ConsoleAction::STDOUT;
}

X3DExternalToolsEditor::~X3DExternalToolsEditor ()
{ }

} // puck
} // titania
