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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "X3DBrowserEditor.h"

#include "../Base/X3DEditorObject.h"
#include "../Browser/BrowserUserData.h"
#include "../Browser/MagicImport.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"
#include "../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"

#include <Titania/X3D/Browser/BrowserOptions.h>
#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Parser/Filter.h>

#include <Titania/InputOutput/MultiLineComment.h>
#include <Titania/OS.h>
#include <Titania/String.h>
#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

static constexpr double UNDO_TIME = 0.6; // Key press delay time + 0.1???

X3DBrowserEditor::X3DBrowserEditor (const X3D::BrowserPtr & browser) :
	X3DBrowserWidget (browser),
	  X3D::X3DEditor (),
	         enabled (false),
	       selection (new BrowserSelection (getBrowserWindow ())),
	    undoMatrices (),
	        undoStep (),
	        undoTime (0),
	            tool (NONE_TOOL)
{
	addChildren (enabled);
}

void
X3DBrowserEditor::initialize ()
{
	X3DBrowserWidget::initialize ();

	getCurrentContext () .addInterest (this, &X3DBrowserEditor::set_executionContext);
	getBrowserWindow () -> getSelection () -> isActive () .addInterest (this, &X3DBrowserEditor::set_selection_active);
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DBrowserEditor::set_selection);
}

void
X3DBrowserEditor::restoreSession ()
{
	// Workspace
	if (not getConfig () .hasItem ("environment"))
		getConfig () .setItem ("environment", 1);

	if (getConfig () .getInteger ("environment") == 1)
		getEditorAction () -> set_active (true);
	else
		getBrowserAction () -> set_active (true);

	// SelectLowest
	if (getConfig () .hasItem ("selectLowest"))
		getSelectLowestAction () -> set_active (getConfig () .getBoolean ("selectLowest"));

	// FollowPrimarySelection
	if (getConfig () .hasItem ("followPrimarySelection"))
		getFollowPrimarySelectionAction () -> set_active (getConfig () .getBoolean ("followPrimarySelection"));

	// This must be done after.
	X3DBrowserWidget::restoreSession ();
}

void
X3DBrowserEditor::setBrowser (const X3D::BrowserPtr & value)
{
	getCurrentBrowser () -> shutdown ()    .removeInterest (this, &X3DBrowserEditor::set_shutdown);
	getUndoHistory (getCurrentBrowser ())  .removeInterest (this, &X3DBrowserEditor::set_undoHistory);

	X3DBrowserWidget::setBrowser (value);

	getCurrentBrowser () -> shutdown ()    .addInterest (this, &X3DBrowserEditor::set_shutdown);
	getUndoHistory (getCurrentBrowser ())  .addInterest (this, &X3DBrowserEditor::set_undoHistory);

	set_undoHistory ();
}

void
X3DBrowserEditor::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	const X3D::BrowserOptionsPtr browserOptions = new X3D::BrowserOptions (getCurrentBrowser ());

	browserOptions -> assign (getCurrentBrowser () -> getBrowserOptions ());

	auto scene = dynamic_cast <X3D::X3DScene*> (value .getValue ());

	if (not scene)
		scene = value -> getScene ();

	if (scene == getCurrentScene ())
	{
		// Shutdown is immediately processed.
		getCurrentBrowser () -> shutdown () .removeInterest (this, &X3DBrowserEditor::set_shutdown);
		getCurrentBrowser () -> shutdown () .addInterest (this, &X3DBrowserEditor::connectShutdown);

		X3DBrowserWidget::setCurrentContext (value);
	}
	else
	{
		if (isSaved (getCurrentBrowser ()))
		{
			// Shutdown is immediately processed.
			getCurrentBrowser () -> shutdown () .removeInterest (this, &X3DBrowserEditor::set_shutdown);
			getCurrentBrowser () -> shutdown () .addInterest (this, &X3DBrowserEditor::connectShutdown);

			getUserData (getCurrentBrowser ()) -> dispose ();
			isModified (getCurrentBrowser (), false);

			X3DBrowserWidget::setCurrentContext (value);
		}
		else
			return;                    // Do nothing.

	}

	if (isEditor ())
		getCurrentBrowser () -> getBrowserOptions () -> assign (browserOptions, true);
}

void
X3DBrowserEditor::set_shutdown ()
{
	if (isSaved (getCurrentBrowser ()))
	{
		getUserData (getCurrentBrowser ()) -> dispose ();
		isModified (getCurrentBrowser (), false);
	}
	else
		X3DBrowserWidget::setCurrentContext (getCurrentContext ());  // Cancel shutdown, there will be no further shutdown now.

}

void
X3DBrowserEditor::connectShutdown ()
{
	getCurrentBrowser () -> shutdown () .removeInterest (this, &X3DBrowserEditor::connectShutdown);
	getCurrentBrowser () -> shutdown () .addInterest (this, &X3DBrowserEditor::set_shutdown);
}

void
X3DBrowserEditor::set_executionContext ()
{
	if (not (isEditor () and getArrowButton () .get_active ()))
		return;

	try
	{
		static const std::map <std::string, X3D::X3DConstants::NodeType> viewerTypes = {
			std::make_pair ("EXAMINE",             X3D::X3DConstants::ExamineViewer),
			std::make_pair ("WALK",                X3D::X3DConstants::WalkViewer),
			std::make_pair ("FLY",                 X3D::X3DConstants::FlyViewer),
			std::make_pair ("PLANE",               X3D::X3DConstants::PlaneViewer),
			std::make_pair ("PLANE_create3000.de", X3D::X3DConstants::PlaneViewer),
			std::make_pair ("NONE",                X3D::X3DConstants::NoneViewer),
			std::make_pair ("LOOKAT",              X3D::X3DConstants::LookAtViewer)
		};

		const auto   worldInfo   = getWorldInfo ();
		const auto   metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/NavigationInfo");
		const auto & type        = metadataSet -> getValue <X3D::MetadataString> ("type") -> value ();

		setViewer (viewerTypes .at (type .at (0)));
	}
	catch (const std::exception & error)
	{
		setViewer (X3D::X3DConstants::ExamineViewer);
	}

	try
	{
		const auto worldInfo   = getWorldInfo ();
		const auto metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/Viewpoint");

		auto & p = metadataSet -> getValue <X3D::MetadataDouble> ("position") -> value ();
		auto & o = metadataSet -> getValue <X3D::MetadataDouble> ("orientation") -> value ();
		auto & c = metadataSet -> getValue <X3D::MetadataDouble> ("centerOfRotation") -> value ();

		const auto viewpoint = getCurrentWorld () -> getActiveLayer () -> getViewpoint ();

		viewpoint -> setUserPosition         (X3D::Vector3f (p .get1Value (0), p .get1Value (1), p .get1Value (2)));
		viewpoint -> setUserOrientation      (X3D::Rotation4f (o .get1Value (0), o .get1Value (1), o .get1Value (2), o .get1Value (3)));
		viewpoint -> setUserCenterOfRotation (X3D::Vector3f (c .get1Value (0), c .get1Value (1), c .get1Value (2)));

		viewpoint -> set_bind () = true;
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
X3DBrowserEditor::setViewer (const X3D::X3DConstants::NodeType viewer)
{
	if (isEditor () and getArrowButton () .get_active ())
		getCurrentBrowser () -> setPrivateViewer (viewer);
	else
		getCurrentBrowser () -> setViewer (viewer);
}

void
X3DBrowserEditor::set_selection_active (const bool value)
{
	if (value)
	{
		for (const auto & child : getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
				undoMatrices [transform] = std::make_pair (transform -> getMatrix (), transform -> center () .getValue ());
		}
	}
	else
	{
		bool changed = false;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Edit Transform"));

		getSelection () -> redoRestoreSelection (undoStep);

		for (const auto & child : getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
			{
				try
				{
					const X3D::Matrix4f matrix = undoMatrices .at (transform) .first;
					const X3D::Vector3f center = undoMatrices .at (transform) .second;

					if (matrix not_eq transform -> getMatrix () or center not_eq transform -> center ())
					{
						changed = true;

						undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             matrix,
						                             center);

						undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             transform -> getMatrix (),
						                             transform -> center ());
					}
				}
				catch (const std::out_of_range &)
				{ }
			}
		}

		undoMatrices .clear ();

		getSelection () -> undoRestoreSelection (undoStep);

		if (changed)
			addUndoStep (undoStep);
	}
}

void
X3DBrowserEditor::set_selection (const X3D::MFNode & selection)
{
	if (selection .empty ())
		return;

	// If the parent node of the selection leader is a Switch, set whichChoice to the index of the leader.

	for (const auto & node : getParentNodes (selection .back ()))
	{
		const auto switchNode = dynamic_cast <X3D::Switch*> (node .getValue ());

		if (not switchNode)
			continue;

		const auto iter = std::find (switchNode -> children () .begin (),
		                             switchNode -> children () .end (),
		                             selection .back ());

		if (iter == switchNode -> children () .end ())
			continue;

		switchNode -> setWhichChoice (iter - switchNode -> children () .begin ());
	}
}

void
X3DBrowserEditor::isEditor (const bool value)
{
	enabled = value;
	getConfig () .setItem ("environment", value ? 1 : 0);
}

bool
X3DBrowserEditor::isSaved (const X3D::BrowserPtr & browser)
{
	const auto userData = getUserData (browser);

	if (userData -> saveConfirmed)
		return true;

	if (isModified (browser))
	{
		const auto pageNumber = getBrowserNotebook () .page_num (*browser);

		if (pageNumber < 0)
			return true;

		getBrowserNotebook () .set_current_page (pageNumber);

		const auto responseId = std::dynamic_pointer_cast <FileSaveWarningDialog> (addDialog ("FileSaveWarningDialog", false)) -> run ();

		switch (responseId)
		{
			case Gtk::RESPONSE_OK:
			{
				on_save_activated ();
				userData -> saveConfirmed = not isModified (browser);
				return userData -> saveConfirmed;
			}
			case Gtk::RESPONSE_CANCEL:
			case Gtk::RESPONSE_DELETE_EVENT:
			{
				return false;
			}
			default:
			{
				userData -> saveConfirmed = true;
				return true;
			}
		}
	}

	return true;
}

void
X3DBrowserEditor::isModified (const X3D::BrowserPtr & browser, const bool value)
{
	const auto userData = getUserData (browser);

	userData -> modified      = value;
	userData -> saveConfirmed = false;

	if (not value)
		getUndoHistory (browser) .save ();

	setTitle ();
}

bool
X3DBrowserEditor::isModified (const X3D::BrowserPtr & browser) const
{
	return getUndoHistory (browser) .isModified () or getUserData (browser) -> modified;
}

// File operations

void
X3DBrowserEditor::blank ()
{
	if (isEditor ())
	{
		append (X3D::createBrowser (getMasterBrowser ()), "");
		getBrowserNotebook () .set_current_page (getBrowsers () .size () - 1);
	}
	else
		openRecent ();
}

void
X3DBrowserEditor::open (const basic::uri & URL)
{
	if (isEditor ())
		X3DBrowserWidget::open (URL);

	else
		load (getCurrentBrowser (), URL);
}

void
X3DBrowserEditor::load (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	if (isSaved (browser))
		X3DBrowserWidget::load (browser, URL);
}

X3D::MFNode
X3DBrowserEditor::import (const std::vector <basic::uri> & uris, const X3D::UndoStepPtr & undoStep)
{
	// Import into scene graph

	MagicImport magicImport (getBrowserWindow ());

	X3D::MFNode  nodes;
	auto         selection = getSelection () -> getChildren ();
	const auto & layerSet  = getCurrentWorld () -> getLayerSet ();

	for (const auto & worldURL : uris)
	{
		try
		{
			const auto scene = getCurrentBrowser () -> createX3DFromURL ({ worldURL .str () });

			if (magicImport .import (getCurrentContext (), selection, scene, undoStep))
				return selection;

			X3D::MFNode importedNodes;

			if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
			{
				importedNodes = importScene (getCurrentContext (),
				                             X3D::SFNode (layerSet -> getActiveLayer ()),
				                             layerSet -> getActiveLayer () -> children (),
				                             scene,
				                             undoStep);
			}
			else
			{
				importedNodes = importScene (getCurrentContext (),
				                             X3D::SFNode (getCurrentContext ()),
				                             getCurrentContext () -> getRootNodes (),
				                             scene,
				                             undoStep);
			}

			magicImport .process (getCurrentContext (), importedNodes, scene, undoStep);

			nodes .append (std::move (importedNodes));
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	return nodes;
}

X3D::MFNode
X3DBrowserEditor::importAsInline (const std::vector <basic::uri> & uris, const X3D::UndoStepPtr & undoStep)
{
	// Import As Inline

	for (const auto & worldURL : uris)
	{
		const auto relativePath = getCurrentContext () -> getWorldURL () .relative_path (worldURL);

		std::string string;

		string += "DEF " + X3D::get_name_from_uri (worldURL) + " Transform {";
		string += "  children Inline {";
		string += "    url [";
		string += "      \"" + relativePath + "\"";
		string += "      \"" + worldURL + "\"";
		string += "    ]";
		string += "  }";
		string += "}";

		const auto scene = getCurrentBrowser () -> createX3DFromString (string);

		return importScene (getCurrentContext (), X3D::SFNode (getCurrentContext ()), getCurrentContext () -> getRootNodes (), scene, undoStep);
	}

	return X3D::MFNode ();
}

bool
X3DBrowserEditor::save (const basic::uri & worldURL, const bool compressed, const bool copy)
{
	if (true)
	{
		static const std::map <X3D::X3DConstants::NodeType, std::string> types = {
			std::make_pair (X3D::X3DConstants::ExamineViewer, "EXAMINE"),
			std::make_pair (X3D::X3DConstants::WalkViewer,    "WALK"),
			std::make_pair (X3D::X3DConstants::FlyViewer,     "FLY"),
			std::make_pair (X3D::X3DConstants::PlaneViewer,   "PLANE"),
			std::make_pair (X3D::X3DConstants::NoneViewer,    "NONE"),
			std::make_pair (X3D::X3DConstants::LookAtViewer,  "LOOKAT")
		};

		const auto worldInfo   = createWorldInfo ();
		const auto metadataSet = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/NavigationInfo");
		const auto type        = types .find (getCurrentBrowser () -> getCurrentViewer ());

		metadataSet -> createValue <X3D::MetadataString> ("type") -> value () = { type not_eq types .end () ? type -> second : "EXAMINE" };
	}

	try
	{
		const auto   worldInfo        = createWorldInfo ();
		const auto   metadataSet      = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/Viewpoint");
		const auto & activeLayer      = getCurrentWorld () -> getActiveLayer ();
		const auto   viewpoint        = activeLayer -> getViewpoint ();
		const auto   position         = viewpoint -> getUserPosition ();
		const auto   orientation      = viewpoint -> getUserOrientation ();
		const auto   centerOfRotation = viewpoint -> getUserCenterOfRotation ();

		metadataSet -> createValue <X3D::MetadataDouble> ("position")         -> value () = { position         .x (), position         .y (), position         .z () };
		metadataSet -> createValue <X3D::MetadataDouble> ("orientation")      -> value () = { orientation      .x (), orientation      .y (), orientation      .z (), orientation .angle () };
		metadataSet -> createValue <X3D::MetadataDouble> ("centerOfRotation") -> value () = { centerOfRotation .x (), centerOfRotation .y (), centerOfRotation .z () };
	}
	catch (const X3D::X3DError &)
	{ }

	if (X3DBrowserWidget::save (worldURL, compressed, copy))
	{
		if (not copy)
			isModified (getCurrentBrowser (), false);

		return true;
	}

	return false;
}

void
X3DBrowserEditor::reload ()
{
	if (isSaved (getCurrentBrowser ()))
		X3DBrowserWidget::reload ();
}

void
X3DBrowserEditor::close (const X3D::BrowserPtr & browser)
{
	getWidget () .grab_focus ();

	if (isSaved (browser))
		X3DBrowserWidget::close (browser);
}

bool
X3DBrowserEditor::quit ()
{
	getWidget () .grab_focus ();

	for (const auto & browser : getBrowsers ())
	{
		if (isSaved (browser))
			continue;

		for (const auto & browser : getBrowsers ())
			getUserData (browser) -> saveConfirmed = false;

		// Cancel quit.
		return true;
	}

	return X3DBrowserWidget::quit ();
}

// Undo/Redo operations

X3D::UndoHistory &
X3DBrowserEditor::getUndoHistory (const X3D::BrowserPtr & browser)
{
	return getUserData (browser) -> undoHistory;
}

const X3D::UndoHistory &
X3DBrowserEditor::getUndoHistory (const X3D::BrowserPtr & browser) const
{
	return getUserData (browser) -> undoHistory;
}

void
X3DBrowserEditor::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	getUndoHistory (getCurrentBrowser ()) .addUndoStep (undoStep);
}

void
X3DBrowserEditor::addUndoStep (const X3D::BrowserPtr & browser, const X3D::UndoStepPtr & undoStep)
{
	getUndoHistory (browser) .addUndoStep (undoStep);
}

void
X3DBrowserEditor::undo ()
{
	getCurrentBrowser () -> grab_focus ();

	getUndoHistory (getCurrentBrowser ()) .undo ();
}

void
X3DBrowserEditor::redo ()
{
	getCurrentBrowser () -> grab_focus ();

	getUndoHistory (getCurrentBrowser ()) .redo ();
}

void
X3DBrowserEditor::set_undoHistory ()
{
	const auto & undoHistory = getUndoHistory (getCurrentBrowser ());

	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		getUndoButton ()   .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		getRedoButton ()   .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		getRedoButton ()   .set_sensitive (false);
	}

	setTitle ();
}

void
X3DBrowserEditor::cutNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	Gtk::Clipboard::get () -> set_text (X3D::X3DEditor::cutNodes (executionContext, nodes, undoStep));
}

void
X3DBrowserEditor::copyNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes)
{
	Gtk::Clipboard::get () -> set_text (X3D::X3DEditor::copyNodes (executionContext, nodes));
}

void
X3DBrowserEditor::pasteNodes (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

		if (not clipboard -> wait_is_text_available ())
			return;

		basic::ifilestream text (clipboard -> wait_for_text ());

		text .imbue (std::locale::classic ());

		std::string header;

		if (not X3D::Grammar::Comment (text, header))
			return;

		std::string encoding, specificationVersion, characterEncoding, comment;

		if (not X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
			return;

		std::string whiteSpaces;

		X3D::Grammar::WhiteSpaces (text, whiteSpaces);

		std::string worldURL;

		if (not X3D::Grammar::Comment (text, worldURL))
			return;

		const auto scene = getCurrentBrowser () -> createX3DFromStream (worldURL, text);

		if (MagicImport (getBrowserWindow ()) .import (executionContext, nodes, scene, undoStep))
			return;

		const auto & activeLayer = getCurrentWorld () -> getActiveLayer ();
		auto &       children    = activeLayer and activeLayer not_eq getCurrentWorld () -> getLayer0 ()
		                           ? activeLayer -> children ()
											: getCurrentContext () -> getRootNodes ();
	
		undoStep -> addObjects (getCurrentContext (), activeLayer);

		const auto importedNodes = importScene (executionContext,
                                              X3D::SFNode (executionContext),
                                              executionContext == getCurrentContext ()
                                              ? children
                                              : executionContext -> getRootNodes (),
                                              scene,
                                              undoStep);

		getSelection () -> setChildren (importedNodes, undoStep);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserEditor::updatePasteStatus ()
{
	getPasteMenuItem () .set_sensitive (getPasteStatus ());
}

bool
X3DBrowserEditor::getPasteStatus () const
{
	const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

	if (clipboard -> wait_is_text_available ())
	{
		std::istringstream text (clipboard -> wait_for_text ());

		text .imbue (std::locale::classic ());

		std::string header;

		if (X3D::Grammar::Comment (text, header))
		{
			std::string encoding, specificationVersion, characterEncoding, comment;

			if (X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
				return true;
		}
	}

	return false;
}

// Edit operations


/***
 *  Add node to current layer root nodes.
 */
void
X3DBrowserEditor::addNode (const X3D::SFNode & node, const X3D::UndoStepPtr & undoStep)
{
	const auto & activeLayer = getCurrentWorld () -> getActiveLayer ();
	auto &       children    = activeLayer and activeLayer not_eq getCurrentWorld () -> getLayer0 ()
	                           ? activeLayer -> children ()
										: getCurrentContext () -> getRootNodes ();

	undoStep -> addObjects (getCurrentContext (), activeLayer);
	children .emplace_back (node);
	getBrowserWindow () -> getSelection () -> setChildren ({ node }, undoStep);

	const auto removeUndoStep = std::make_shared <X3D::UndoStep> ("");

	removeNodesFromScene (getCurrentContext (), { node }, true, removeUndoStep);
	undoStep -> addUndoFunction (&X3D::UndoStep::redo, removeUndoStep);
	removeUndoStep -> undo ();
	undoStep -> addRedoFunction (&X3D::UndoStep::undo, removeUndoStep);
}

/***
 *  Create node and add node to current layer root nodes.
 */
X3D::SFNode
X3DBrowserEditor::createNode (const std::string & typeName, const X3D::UndoStepPtr & undoStep)
{
	const auto node = getCurrentContext () -> createNode (typeName);
	getCurrentContext () -> addUninitializedNode (node);
	getCurrentContext () -> realize ();

	addNode (node, undoStep);

	return node;
}

/***
 *  Completely removes @a nodes from @a executionContext.
 */
void
X3DBrowserEditor::removeNodesFromScene (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const bool removeFromSceneGraph, const X3D::UndoStepPtr & undoStep) const
{
	getSelection () -> removeChildren (nodes, undoStep);

	X3D::X3DEditor::removeNodesFromScene (executionContext, nodes, removeFromSceneGraph, undoStep);
}

// Undo functions

void
X3DBrowserEditor::translateSelection (const X3D::Vector3f & offset, const bool alongFrontPlane, const ToolType currentTool)
{
	using setValue = void (X3D::SFVec3f::*) (const X3D::Vector3f &);

	static const std::vector <const char*> undoText = {
		"Nudge Left",
		"Nudge Right",
		"Nudge Up",
		"Nudge Down",
		"Nudge Front",
		"Nudge Back"
	};

	for (const auto & node : basic::make_reverse_range (getSelection () -> getChildren ()))
	{
		X3D::X3DTransformNodePtr transform (node);

		if (transform)
		{
			if (currentTool not_eq tool or chrono::now () - undoTime > UNDO_TIME or undoStep not_eq getBrowserWindow () -> getUndoStep ())
				undoStep = std::make_shared <X3D::UndoStep> (_ (undoText [currentTool - NUDGE_LEFT]));

			tool     = currentTool;
			undoTime = chrono::now ();

			getSelection () -> redoRestoreSelection (undoStep);

			if (transform -> getKeepCenter ())
			{
				X3D::Matrix4d matrix;
				matrix .set (offset);
				matrix = transform -> getCurrentMatrix () * matrix;

				undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center ());
				undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixKeepCenter, transform, matrix);
				transform -> setMatrixKeepCenter (matrix);

				// If we use setMatrixKeepCenter we must do the group translation by ourself.

				matrix .set (offset);
				matrix *= transform -> getTransformationMatrix ();

				bool first = true;

				for (const auto & node : basic::make_reverse_range (getSelection () -> getChildren ()))
				{
					X3D::X3DTransformNodePtr transform (node);

					if (not transform)
						continue;

					if (first)
					{
						first = false;
						continue;
					}

					undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center ());
					transform -> addAbsoluteMatrix (matrix, transform -> getKeepCenter ());
					undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getCurrentMatrix (), transform -> center ());
				}
			}
			else
			{
				undoStep -> addObjects (node);
				undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation ());
				undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation () + offset);
				transform -> translation () += offset;
			}

			getSelection () -> undoRestoreSelection (undoStep);

			if (undoStep not_eq getBrowserWindow () -> getUndoStep ())
				addUndoStep (undoStep);

			break;
		}
	}
}

// Misc

void
X3DBrowserEditor::editCDATA (const X3D::SFNode & node)
{
	X3D::MFString* const cdata          = node -> getCDATA ();
	std::string          filename       = "/tmp/titania-XXXXXX.js";
	const int            fileDescriptor = mkstemps (&filename [0], 3);

	if (not cdata or fileDescriptor == -1)
		return;

	// Output file.

	std::ofstream ostream (filename);

	for (const auto & string : *cdata)
	{
		ostream
			<< "<![CDATA["
			<< X3D::escape_cdata (string)
			<< "]]>" << std::endl
			<< std::endl
			<< std::endl;
	}

	std::string name = node -> getName ();
	pcrecpp::RE (R"/((\*/))/") .GlobalReplace ("", &name);

	ostream
		<< "/**" << std::endl
		<< " * " << node -> getTypeName () << " " << name << std::endl
		<< " * " << _ ("This file is automaticaly generated to edit CDATA fields. Each SFString value is enclosed inside a CDATA") << std::endl
		<< " * " << _ ("section.  A CDATA section starts with \"<![CDATA[\" and ends with \"]]>\".") << std::endl
		<< " * " << std::endl
		<< " * " << _ ("If this is a Script node, a inline script must be enclosed in \"<![CDATA[ecmascript: your program here ]]>\".") << std::endl
		<< " * " << _ ("If this is a shader, it must be enclosed in \"<![CDATA[data:text/plain, your shader here ]]>\".  Just save") << std::endl
		<< " * " << _ ("this file to apply changes.") << std::endl
		<< " **/" << std::endl;

	// Launch Gnome TextEditor.

	Glib::RefPtr <Gio::File>        file        = Gio::File::create_for_path (filename);
	Glib::RefPtr <Gio::FileMonitor> fileMonitor = file -> monitor_file ();

	fileMonitor -> signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DBrowserEditor::on_cdata_changed), node));
	getUserData (getCurrentBrowser ()) -> fileMonitors .emplace (file, fileMonitor);

	try
	{
		getCurrentBrowser () -> println ("Trying to start gnome-text-editor ...");

		Gio::AppInfo::create_from_commandline (os::realpath ("/usr/bin/gnome-text-editor"), "", Gio::APP_INFO_CREATE_NONE) -> launch (file);
	}
	catch (...)
	{ }

	::close (fileDescriptor);
}

void
X3DBrowserEditor::on_cdata_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event, const X3D::SFNode & node)
{
	io::multi_line_comment comment ("/*", "*/");
	io::sequence           whitespaces ("\r\n \t,");
	io::inverse_string     cdata_start ("<![CDATA[");
	io::inverse_string     contents ("]]>");

	if (event not_eq Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
		return;

	// Parse file.

	X3D::MFString string;
	std::string   ws;

	std::ifstream istream (file -> get_path ());

	// Parse CDATA sections.

	while (istream)
	{
		whitespaces (istream, ws);
		comment (istream, ws);

		if (cdata_start (istream, ws))
		{
			std::string value;

			contents (istream, value);

			string .emplace_back (std::move (value));
		}
		else
			break;
	}

	// Set value.

	X3D::MFString* const cdata = node -> getCDATA ();

	if (string not_eq *cdata)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), cdata -> getName () .c_str ()));

		undoStep -> addObjects (node);

		undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
		undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, string);
		cdata -> setValue (string);

		addUndoStep (undoStep);
	}

	getCurrentBrowser () -> println (X3D::SFTime (chrono::now ()) .toUTCString (), ": ", basic::sprintf (_ ("Script »%s« saved."), node -> getName () .c_str ()));
}

X3DBrowserEditor::~X3DBrowserEditor ()
{ }

} // puck
} // titania
