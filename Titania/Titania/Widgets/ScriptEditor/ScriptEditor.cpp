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

#include "ScriptEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Widgets/Console/Console.h"
#include "../../Editors/NodeIndex/NodeIndex.h"
#include "ScriptEditorDatabase.h"

#include <Titania/X3D/Components/Shaders/ShaderPart.h>
#include <Titania/X3D/Components/Shaders/ShaderProgram.h>
#include <Titania/X3D/Components/Scripting/Script.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

ScriptEditor::ScriptEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	           X3DScriptEditor (),
	       X3DScriptNodeEditor (),
	           X3DShaderEditor (),
	     X3DScriptEditorSearch (),
	X3DScriptEditorPreferences (),
	                      page (browserWindow -> getCurrentPage ()),
	                  modified (false),
	                 nodeIndex (new NodeIndex (browserWindow)),
	                   console (new Console (browserWindow)),
	                  nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                 nodeTypes ({ X3D::X3DConstants::Script,
	                              X3D::X3DConstants::ShaderPart,
	                              X3D::X3DConstants::ShaderProgram }),
	                      node (),
	                     index (0)
{
	addChildObjects (node);

	// Apply Button

	getApplyButton () .add_accelerator ("clicked", getAccelGroup (), GDK_KEY_S, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	// Text Buffer

	getTextBuffer () -> create_mark ("scroll", getTextBuffer () -> end (), true);

	// Title Bar

	setTitleBar (getPreferencesDialog (), getPreferencesHeaderBar ());

	setup ();
}

void
ScriptEditor::initialize ()
{
	X3DScriptEditor::initialize ();
	X3DScriptNodeEditor::initialize ();
	X3DShaderEditor::initialize ();
	X3DScriptEditorPreferences::initialize ();

	// Text view

	getTextBuffer () -> get_undo_manager () -> signal_can_undo_changed () .connect (sigc::mem_fun (this, &ScriptEditor::on_can_undo_changed));
	getTextBuffer () -> get_undo_manager () -> signal_can_redo_changed () .connect (sigc::mem_fun (this, &ScriptEditor::on_can_redo_changed));

	getTextView () .signal_focus_in_event ()  .connect (sigc::mem_fun (this, &ScriptEditor::on_focus_in_event));
	getTextView () .signal_focus_out_event () .connect (sigc::mem_fun (this, &ScriptEditor::on_focus_out_event));

	// Node index

	nodeIndex -> getNode () .addInterest (&ScriptEditor::set_node, this);

	// Node index

	nodeIndex -> setName (getName () + "." + nodeIndex -> getName ());
	nodeIndex -> setShowWidget (true);
	nodeIndex -> setDisplayProtoNodes (true);
	nodeIndex -> setTypes ({ X3D::X3DConstants::Script,
	                         X3D::X3DConstants::ShaderPart,
	                         X3D::X3DConstants::ShaderProgram });
	nodeIndex -> reparent (getNodeIndexBox (), getWindow ());

	// Widgets

	console -> reparent (getConsoleBox (), getWindow ());

	// Observe context change.

	page -> getExecutionContext () .addInterest (&ScriptEditor::set_executionContext, this);

	set_executionContext ();

	// Initialize after all.

	X3DScriptEditorSearch::initialize ();
}

void
ScriptEditor::configure ()
{
	X3DScriptEditor::configure ();
	X3DScriptEditorSearch::configure ();
	X3DScriptEditorPreferences::configure ();

	// Config

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));

	if (getConfig () -> hasItem ("sidePaned"))
		getSidePaned () .set_position (getConfig () -> getItem <int32_t> ("sidePaned"));
}

void
ScriptEditor::restore ()
{
	try
	{
	   if (not node)
			return;

		ScriptEditorDatabase database;

		const auto item = database .getItem (node -> getExecutionContext () -> getWorldURL () .filename (), getPathFromNode (node));

		getTextView () .get_hadjustment () -> set_value (std::get <1> (item));
		getTextView () .get_vadjustment () -> set_value (std::get <2> (item));
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
ScriptEditor::getModified () const
{
	if (not node)
		return false;

	if (modified)
		return true;

	const auto   sourceText = node -> getSourceText ();
	const auto   text       = getTextBuffer () -> get_text ();
	const auto & current    = sourceText -> get1Value (index);

	if (text not_eq current)
		return true;

	return false;
}

void
ScriptEditor::set_executionContext ()
{
	try
	{
		ScriptEditorDatabase database;

		const auto item     = database .getItem (page -> getExecutionContext () -> getWorldURL () .filename ());
		const auto nodePath = std::get <0> (item);
		const auto node     = getNodeFromPath (nodePath);

		if (nodeTypes .count (node -> getType () .back ()))
		{
			set_node (node);
			nodeIndex -> setSelection (node);
			return;
		}
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
   
   // Fallback if the node has no name or was not found.
	set_node (nullptr);
}

void
ScriptEditor::set_node (const X3D::SFNode & value)
{
	if (value == node)
		return;

	if (node)
	{
		if (getModified ())
			on_apply_clicked ();

		const auto sourceText = node -> getSourceText ();

		node  -> isLive () .removeInterest (&ScriptEditor::set_live, this);
		sourceText -> removeInterest (&ScriptEditor::set_sourceText, this);
		sourceText -> removeInterest (&ScriptEditor::connectSourceText, this);

		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);

		if (urlObject)
			urlObject -> checkLoadState () .removeInterest (&ScriptEditor::set_loadState, this);

		save ();

		getTextBuffer () -> begin_not_undoable_action ();
		getTextBuffer () -> set_text ("");
		getTextBuffer () -> end_not_undoable_action ();
	}

	X3DScriptNodeEditor::set_node (value);
	X3DShaderEditor::set_node (value);

	node = value;
	nodeName .setNode (node);

	getScriptEditorBox () .set_sensitive (node);
	getToolbar ()         .set_sensitive (node);

	if (node)
	{
		const auto sourceText = node -> getSourceText ();

		// Realized protos.
		node -> getExecutionContext () -> realize ();

		node  -> isLive () .addInterest (&ScriptEditor::set_live, this);
		sourceText -> addInterest (&ScriptEditor::set_sourceText, this);

		set_sourceText ();

		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);

		if (urlObject)
		{
			urlObject -> checkLoadState () .addInterest (&ScriptEditor::set_loadState, this);
			set_loadState (urlObject -> checkLoadState ());
		}
	}
	else
	{
		set_loadState (X3D::NOT_STARTED_STATE);
	}

	setModified (false);
}

void
ScriptEditor::set_live ()
{
	if (node -> isLive ())
		return;

	modified = false;

	set_node (nullptr);
}

bool
ScriptEditor::on_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> setAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	return false;
}

bool
ScriptEditor::on_focus_out_event (GdkEventFocus*)
{
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

void
ScriptEditor::on_new_clicked ()
{
	getNewScriptPopover () .popup ();
}

void
ScriptEditor::on_apply_clicked ()
{
	/*
    * We don't save the whole scene here, as the script could contain infinitive loop,
	 * and then the scene couldn't be opened again.
	 */

	if (not node)
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Script"));

	apply (undoStep);

	 if (node -> isLive ())
		 getBrowserWindow () -> addUndoStep (undoStep);
}

void
ScriptEditor::apply (const X3D::UndoStepPtr & undoStep)
{
	const auto sourceText = node -> getSourceText ();
	const auto text       = getTextBuffer () -> get_text ();

	sourceText -> removeInterest (&ScriptEditor::set_sourceText, this);
	sourceText -> addInterest (&ScriptEditor::connectSourceText, this);

	undoStep -> addObjects (node);

	undoStep -> addUndoFunction (&X3D::MFString::setValue, sourceText, *sourceText);
	sourceText -> set1Value (index, text);
	undoStep -> addRedoFunction (&X3D::MFString::setValue, sourceText, *sourceText);

	X3D::X3DEditor::requestUpdateInstances (node, undoStep);

	page -> getMainBrowser () -> getConsole () -> log (basic::sprintf (_ ("%s : %s »%s« is build.\n"), X3D::SFTime (X3D::SFTime::now ()) .toUTCString () .c_str (), node -> getTypeName () .c_str (), node -> getName () .c_str ()));

	setModified (false);
}

void
ScriptEditor::on_undo_clicked ()
{
	getTextBuffer () -> undo ();
}

void
ScriptEditor::on_redo_clicked ()
{
	getTextBuffer () -> redo ();
}

void
ScriptEditor::on_can_undo_changed ()
{
	getUndoButton () .set_sensitive (getTextBuffer () -> can_undo ());

	if (getTextBuffer () -> can_undo ())
	{
		if (node)
		{
			setModified (true);
			page -> setModified (true);
		}
	}
}

void
ScriptEditor::on_can_redo_changed ()
{
	getRedoButton () .set_sensitive (getTextBuffer () -> can_redo ());
}

void
ScriptEditor::set_sourceText ()
{
	if (getTextBuffer () -> end () .get_offset ())
		save ();

	getTextBuffer () -> begin_not_undoable_action ();

	const auto sourceText = node -> getSourceText ();

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::Script:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "application/javascript"));

				if (index >= sourceText -> size () or sourceText -> get1Value (index) .empty ())
					getTextBuffer () -> set_text ("ecmascript:\n");
				else
					getTextBuffer () -> set_text (sourceText -> get1Value (index));

				break;
			}
			case X3D::X3DConstants::ShaderPart:
			case X3D::X3DConstants::ShaderProgram:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "text/x-c"));

				if (index >= sourceText -> size () or sourceText -> get1Value (index) .empty ())
					getTextBuffer () -> set_text ("data:text/plain,\n");
				else
					getTextBuffer () -> set_text (sourceText -> get1Value (index));

				break;
			}
			default:
				continue;
		}

		break;
	}

	getTextBuffer () -> end_not_undoable_action ();

	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &ScriptEditor::restore), Glib::PRIORITY_HIGH + 20);
}

void
ScriptEditor::connectSourceText (const X3D::MFString & field)
{
	field .removeInterest (&ScriptEditor::connectSourceText, this);
	field .addInterest (&ScriptEditor::set_sourceText, this);
}

void
ScriptEditor::set_loadState (const X3D::LoadState loadState)
{
	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getApplyButton () .set_stock_id (Gtk::StockID ("gtk-stop"));
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getApplyButton () .set_stock_id (Gtk::StockID ("gtk-refresh"));
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getApplyButton () .set_stock_id (Gtk::StockID ("gtk-yes"));
			break;
		}
		case X3D::FAILED_STATE:
		{
			getApplyButton () .set_stock_id (Gtk::StockID ("gtk-no"));
			break;
		}
	}
}

void
ScriptEditor::save ()
{
	try
	{
		if (node)
		{
			ScriptEditorDatabase database;

			database .setItem (node -> getExecutionContext () -> getWorldURL () .filename (),
			                   getPathFromNode (node),
			                   getTextView () .get_hadjustment () -> get_value (),
			                   getTextView () .get_vadjustment () -> get_value ());
		}
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

X3D::SFNode
ScriptEditor::getNodeFromPath (const std::string & nodePath)
{
	X3D::X3DExecutionContext* executionContext = page -> getScene ();
	auto path                                  = std::deque <std::string> ();

	basic::split (std::back_inserter (path), nodePath, ".");

	for (int32_t i = 0, size = path .size () - 1; i < size; ++ i)
		executionContext = executionContext -> getProtoDeclaration (path [i]);

	return executionContext -> getNamedNode (path .empty () ? nodePath : path .back ());
}

std::string
ScriptEditor::getPathFromNode (const X3D::SFNode & node) const
{
	const auto path = getNodePath (node);

	return basic::join (path .begin (), path .end (), ".");
}

void
ScriptEditor::store ()
{
	save ();

	getConfig () -> setItem <int32_t> ("paned",      getPaned ()     .get_position ());
	getConfig () -> setItem <int32_t> ("sidePaned",  getSidePaned () .get_position ());

	X3DScriptEditorPreferences::store ();
	X3DScriptEditorSearch::store ();
	X3DScriptEditor::store ();
}

/*
 * Destruction
 */

ScriptEditor::~ScriptEditor ()
{
	dispose ();
}

} // puck
} // titania
