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

#include "X3DBrowserEditor.h"

#include "../Base/X3DEditorObject.h"
#include "../Browser/BrowserSelection.h"
#include "../Browser/MagicImport.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../Configuration/config.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Browser/BrowserOptions.h>
#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Editing/Undo/UndoStepContainer.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Parser/Filter.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Tools/Grouping/X3DTransformNodeTool.h>

#include <Titania/InputOutput/MultiLineComment.h>
#include <Titania/OS.h>
#include <Titania/String.h>
#include <Titania/Utility/Map.h>

#include <Titania/X3D/Execution/BindableNodeStack.h>

#include <regex>

namespace titania {
namespace puck {

static constexpr double UNDO_TIME = 0.6; // Key press delay time + 0.1???

X3DBrowserEditor::X3DBrowserEditor (const X3D::BrowserPtr & defaultBrowser) :
	 X3DBrowserWidget (defaultBrowser),
	 executionContext (defaultBrowser -> getExecutionContext ()),
	          editing (false),
	        clipboard (defaultBrowser -> getExecutionContext () -> createNode <X3D::Clipboard> ()),
	        selection (new BrowserSelection (getBrowserWindow ())),
	    nudgeUndoStep (),
	         undoTime (0),
	             tool (NONE_TOOL)
{
	addChildObjects (executionContext,
	                 editing,
	                 clipboard);

	clipboard -> target () = "model/x3d+vrml";
}

void
X3DBrowserEditor::initialize ()
{
	X3DBrowserWidget::initialize ();

	getCurrentContext () .addInterest (&X3DBrowserEditor::set_executionContext, this);

	getBrowserWindow () -> getSelection () -> getNodes ()      .addInterest (&X3DBrowserEditor::set_selection, this);
	getBrowserWindow () -> getSelection () -> getGeometries () .addInterest (&X3DBrowserEditor::set_selection, this);

	getClipboard () -> string_changed () .addInterest (&X3DBrowserWindow::set_clipboard, this);
}

void
X3DBrowserEditor::configure ()
{
	// Workspace
	if (not getConfig () -> hasItem ("environment"))
		getConfig () -> setItem ("environment", 1);

	if (getConfig () -> getInteger ("environment") == 1)
		getEditorAction () -> set_active (true);
	else
		getBrowserAction () -> set_active (true);

	// SelectLowest
	if (getConfig () -> hasItem ("selectLowest"))
		getSelectLowestAction () -> set_active (getConfig () -> getBoolean ("selectLowest"));
	else
		getSelectLowestAction () -> set_active (true);

	// FollowPrimarySelection
	if (getConfig () -> hasItem ("followPrimarySelection"))
		getFollowPrimarySelectionAction () -> set_active (getConfig () -> getBoolean ("followPrimarySelection"));
	else
		getFollowPrimarySelectionAction () -> set_active (true);

	// This must be done after.
	X3DBrowserWidget::configure ();
}

void
X3DBrowserEditor::setPage (const NotebookPagePtr & value)
{
	if (getCurrentPage ())
	{
		if (getEditing () and getCurrentBrowser () -> isInitialized ())
			setMetaData ();
	
		getCurrentPage () -> getUndoHistory () .removeInterest (&X3DBrowserEditor::set_undoHistory, this);
		getCurrentBrowser () -> shutdown ()  .removeInterest (&X3DBrowserEditor::set_shutdown,    this);
	}

	X3DBrowserWidget::setPage (value);

	if (getCurrentPage ())
	{
		getCurrentPage () -> getUndoHistory () .addInterest (&X3DBrowserEditor::set_undoHistory, this);
		getCurrentBrowser () -> shutdown ()  .addInterest (&X3DBrowserEditor::set_shutdown,    this);
	
		set_undoHistory ();
	}
}

void
X3DBrowserEditor::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	const X3D::BrowserOptionsPtr browserOptions (new X3D::BrowserOptions (getCurrentBrowser ()));

	browserOptions -> assign (getCurrentBrowser () -> getBrowserOptions ());

	auto scene = dynamic_cast <X3D::X3DScene*> (value .getValue ());

	if (not scene)
		scene = value -> getScene ();

	if (scene == getCurrentScene ())
	{
		// Shutdown is immediately processed.
		getCurrentBrowser () -> shutdown () .removeInterest (&X3DBrowserEditor::set_shutdown, this);
		getCurrentBrowser () -> shutdown () .addInterest (&X3DBrowserEditor::connectShutdown, this);

		X3DBrowserWidget::setCurrentContext (value);
	}
	else
	{
		if (getCurrentPage () -> isSaved ())
		{
			// Shutdown is immediately processed.
			getCurrentBrowser () -> shutdown () .removeInterest (&X3DBrowserEditor::set_shutdown, this);
			getCurrentBrowser () -> shutdown () .addInterest (&X3DBrowserEditor::connectShutdown, this);

			getCurrentPage () -> reset ();

			X3DBrowserWidget::setCurrentContext (value);
		}
		else
		{
		   // Do nothing.
			return;
		}
	}

	if (getEditing ())
		getCurrentBrowser () -> getBrowserOptions () -> assign (browserOptions, true);
}

void
X3DBrowserEditor::set_shutdown ()
{
	if (getCurrentPage () -> isSaved ())
	{
		getCurrentPage () -> reset ();
	}
	else
		// Cancel shutdown, there will be no further shutdown now.
		X3DBrowserWidget::setCurrentContext (getCurrentContext ());
}

void
X3DBrowserEditor::connectShutdown ()
{
	getCurrentBrowser () -> shutdown () .removeInterest (&X3DBrowserEditor::connectShutdown, this);
	getCurrentBrowser () -> shutdown () .addInterest (&X3DBrowserEditor::set_shutdown, this);
}

void
X3DBrowserEditor::set_executionContext ()
{
	if (not getEditing ())
		return;

	// Restore Viewpoint, and NavigationInfo from meta data.

	getMetaData ();

	// Restore context or save context path in History.

	const auto masterScene = getCurrentContext () -> getMasterScene ();

	if (getCurrentContext () == masterScene and masterScene not_eq executionContext -> getMasterScene ())
	{
		// Restore context from path.

		const auto contextPath = getHistory () -> getContextPath (getCurrentContext () -> getWorldURL ());
		const auto subContext  = getContextFromPath (getCurrentContext (), contextPath);

		if (subContext not_eq getCurrentContext ())
		{
			setCurrentContext (subContext);
		}
	}
	else
	{
		// Save context path in History.

		const auto contextPath = getPathFromContext (getCurrentContext ());

		getHistory () -> setContextPath (masterScene -> getWorldURL (), contextPath);
	}

	executionContext = getCurrentContext ();
}

void
X3DBrowserEditor::set_selection (const X3D::MFNode & selection)
{
	X3DBrowserWidget::set_selection (selection);

	for (const auto & node : getSelection () -> getSelectGeometry () ? getSelection () -> getGeometries () : selection)
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::X3DNodeTool:
				{
					node -> getField <X3D::UndoStepContainerPtr> ("undo_changed") .addInterest (&X3DBrowserEditor::set_tool_undo, this);
					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	if (selection .empty ())
		return;

	// If the parent node of the selection leader is a Switch, set whichChoice to the index of the leader.

	for (const auto & node : X3D::X3DEditor::getParentNodes (selection .back ()))
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
X3DBrowserEditor::setEditing (const bool value)
{
	editing = value;
	getConfig () -> setItem ("environment", value ? 1 : 0);
}

void
X3DBrowserEditor::setMetaData ()
{
	const auto worldInfo = createWorldInfo (getCurrentScene ());

	//

	const auto & world    = getCurrentWorld ();
	const auto & layerSet = world -> getLayerSet ();

	if (layerSet not_eq world -> getDefaultLayerSet ())
		worldInfo -> setMetaData ("/Titania/LayerSet/activeLayer", layerSet -> privateActiveLayer ());
	else
		worldInfo -> removeMetaData ("/Titania/LayerSet/activeLayer");

	//

	static const std::map <X3D::X3DConstants::NodeType, std::string> types = {
		std::make_pair (X3D::X3DConstants::ExamineViewer,      "EXAMINE"),
		std::make_pair (X3D::X3DConstants::WalkViewer,         "WALK"),
		std::make_pair (X3D::X3DConstants::FlyViewer,          "FLY"),
		std::make_pair (X3D::X3DConstants::PlaneViewer,        "PLANE_create3000.de"),
		std::make_pair (X3D::X3DConstants::NoneViewer,         "NONE"),
		std::make_pair (X3D::X3DConstants::LookAtViewer,       "LOOKAT"),
		std::make_pair (X3D::X3DConstants::LassoSelection,     "EXAMINE"),
		std::make_pair (X3D::X3DConstants::RectangleSelection, "EXAMINE"),
		std::make_pair (X3D::X3DConstants::LightSaber,         "EXAMINE"),
	};

	const auto type = types .find (getCurrentBrowser () -> getCurrentViewer ());

	worldInfo -> setMetaData ("/Titania/NavigationInfo/type", type not_eq types .end () ? type -> second : "EXAMINE");

	//

	const auto & activeLayer = getCurrentWorld () -> getActiveLayer ();

	if (activeLayer)
	{
		const auto viewpoint = activeLayer -> getViewpoint ();

		worldInfo -> setMetaData ("/Titania/Viewpoint/position",         viewpoint -> getUserPosition ());
		worldInfo -> setMetaData ("/Titania/Viewpoint/orientation",      viewpoint -> getUserOrientation ());
		worldInfo -> setMetaData ("/Titania/Viewpoint/centerOfRotation", viewpoint -> getUserCenterOfRotation ());
	}
}

void
X3DBrowserEditor::getMetaData ()
{
	try
	{
		const auto   worldInfo = getWorldInfo (getCurrentScene ());
		const auto & world     = getCurrentWorld ();
		const auto & layerSet  = world -> getLayerSet ();

		//

		if (layerSet not_eq world -> getDefaultLayerSet ())
			layerSet -> privateActiveLayer () = worldInfo -> getMetaData <int32_t> ("/Titania/LayerSet/activeLayer", -1);

		//

		try
		{
			static const std::map <std::string, X3D::X3DConstants::NodeType> viewerTypes = {
				std::make_pair ("EXAMINE",             X3D::X3DConstants::ExamineViewer),
				std::make_pair ("WALK",                X3D::X3DConstants::WalkViewer),
				std::make_pair ("FLY",                 X3D::X3DConstants::FlyViewer),
				std::make_pair ("PLANE",               X3D::X3DConstants::PlaneViewer),
				std::make_pair ("PLANE_create3000.de", X3D::X3DConstants::PlaneViewer),
				std::make_pair ("NONE",                X3D::X3DConstants::NoneViewer),
				std::make_pair ("LOOKAT",              X3D::X3DConstants::LookAtViewer),
			};

			setViewer (viewerTypes .at (worldInfo -> getMetaData <std::string> ("/Titania/NavigationInfo/type")));
		}
		catch (const std::exception & error)
		{
			getCurrentBrowser () -> setPrivateViewer (X3D::X3DConstants::DefaultViewer);
		}

		//

		const auto activeLayer = layerSet -> getActiveLayer ();
		const auto viewpoint   = activeLayer -> getViewpoint ();

		const auto position         = worldInfo -> getMetaData <X3D::Vector3d>   ("/Titania/Viewpoint/position",         viewpoint -> getUserPosition ());
		const auto orientation      = worldInfo -> getMetaData <X3D::Rotation4d> ("/Titania/Viewpoint/orientation",      viewpoint -> getUserOrientation ());
		const auto centerOfRotation = worldInfo -> getMetaData <X3D::Vector3d>   ("/Titania/Viewpoint/centerOfRotation", viewpoint -> getUserCenterOfRotation ());

		viewpoint -> setUserPosition         (position);
		viewpoint -> setUserOrientation      (orientation);
		viewpoint -> setUserCenterOfRotation (centerOfRotation);
	}
	catch (const std::exception & error)
	{ }
}

void
X3DBrowserEditor::setViewer (const X3D::X3DConstants::NodeType viewer)
{
	if (getEditing () and not getHandButton () .get_active ())
		getCurrentBrowser () -> setPrivateViewer (viewer);
	else
	{
		getCurrentBrowser () -> setPrivateViewer (X3D::X3DConstants::DefaultViewer);
		getCurrentBrowser () -> setViewerType (viewer);
	}
}

// File operations

void
X3DBrowserEditor::blank ()
{
	if (getEditing ())
	{
		append (get_page ("about/new.x3dv"));
		getBrowserNotebook () .set_current_page (getPages () .size () - 1);
	}
	else
		openRecent ();
}

void
X3DBrowserEditor::open (const basic::uri & URL)
{
	if (getEditing ())
		X3DBrowserWidget::open (URL);

	else
		load (URL);
}

void
X3DBrowserEditor::load (const basic::uri & URL)
{
	if (getCurrentPage () -> isSaved ())
		X3DBrowserWidget::load (URL);
}

X3D::MFNode
X3DBrowserEditor::import (const std::vector <basic::uri> & url, const X3D::UndoStepPtr & undoStep)
{
	// Import into scene graph

	MagicImport magicImport (getBrowserWindow ());

	X3D::MFNode  nodes;
	auto         selection = getSelection () -> getNodes ();
	const auto & layerSet  = getCurrentWorld () -> getLayerSet ();

	for (const auto & worldURL : url)
	{
		try
		{
			const auto scene = getCurrentBrowser () -> createX3DFromURL ({ worldURL .str () });

			if (magicImport .import (getCurrentContext (), selection, scene, undoStep))
				return selection;

			X3D::MFNode importedNodes;

			if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
			{
				importedNodes = X3D::X3DEditor::importScene (getCurrentContext (),
				                                             layerSet -> getActiveLayer (),
				                                             layerSet -> getActiveLayer () -> children (),
				                                             scene,
				                                             undoStep);
			}
			else
			{
				importedNodes = X3D::X3DEditor::importScene (getCurrentContext (),
				                                             getCurrentContext (),
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
X3DBrowserEditor::importAsInline (const std::vector <basic::uri> & url, const X3D::UndoStepPtr & undoStep)
{
	// Import As Inline

	for (const auto & worldURL : url)
	{
		const auto relativePath = getCurrentContext () -> getWorldURL () .relative_path (worldURL);

		std::string string;

		string += "DEF " + X3D::GetNameFromURI (worldURL) + " Transform {";
		string += "  children Inline {";
		string += "    url [";
		string += "      \"" + relativePath + "\"";
		string += "      \"" + worldURL + "\"";
		string += "    ]";
		string += "  }";
		string += "}";

		const auto scene = getCurrentBrowser () -> createX3DFromString (string);

		return X3D::X3DEditor::importScene (getCurrentContext (), getCurrentContext (), getCurrentContext () -> getRootNodes (), scene, undoStep);
	}

	return X3D::MFNode ();
}

bool
X3DBrowserEditor::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	setMetaData ();

	// Save world
	const bool saved = X3DBrowserWidget::save (worldURL, outputStyle, copy);

	if (saved)
	{
		if (not copy)
			getCurrentPage () -> setModified (false);

		return true;
	}

	return false;
}

void
X3DBrowserEditor::reload ()
{
	if (getCurrentPage () -> isSaved ())
		X3DBrowserWidget::reload ();
}

void
X3DBrowserEditor::close (const NotebookPagePtr page)
{
	getWidget () .grab_focus ();

	if (page -> isSaved ())
		X3DBrowserWidget::close (page);
}

bool
X3DBrowserEditor::quit ()
{
	getWidget () .grab_focus ();

	const auto pages = getRecentPages ();

	if (not pages .empty ())
	{
		for (const auto & page : pages)
		{
			if (page -> isSaved ())
				continue;

			for (const auto & page : getPages ())
				page -> setSaveConfirmed (false);

			// Cancel quit.
			return true;
		}
	}

	return X3DBrowserWidget::quit ();
}

void
X3DBrowserEditor::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	getCurrentPage () -> getUndoHistory () .addUndoStep (undoStep);
}

void
X3DBrowserEditor::removeUndoStep ()
{
	getCurrentPage () -> getUndoHistory () .removeUndoStep ();
}

const X3D::UndoStepPtr &
X3DBrowserEditor::getUndoStep () const
{
	return getCurrentPage () -> getUndoHistory () .getUndoStep ();
}

void
X3DBrowserEditor::undo ()
{
	// Focus out tree view cells.
//	getCurrentBrowser () -> grab_focus ();

	getCurrentPage () -> getUndoHistory () .undo ();
}

void
X3DBrowserEditor::redo ()
{
	// Focus out tree view cells.
//	getCurrentBrowser () -> grab_focus ();

	getCurrentPage () -> getUndoHistory ()  .redo ();
}

void
X3DBrowserEditor::set_undoHistory ()
{
	const auto & undoHistory = getCurrentPage () -> getUndoHistory () ;

	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem ()        .set_label (undoHistory .getUndoDescription ());
		getBrowserUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()          .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem ()        .set_sensitive (true);
		getBrowserUndoMenuItem () .set_sensitive (true);
		getUndoButton ()          .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem ()        .set_label (_ ("Undo"));
		getBrowserUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()          .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem ()        .set_sensitive (false);
		getBrowserUndoMenuItem () .set_sensitive (false);
		getUndoButton ()          .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem ()        .set_label (undoHistory .getRedoDescription ());
		getBrowserRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()          .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem ()        .set_sensitive (true);
		getBrowserRedoMenuItem () .set_sensitive (true);
		getRedoButton ()          .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem ()        .set_label (_ ("Redo"));
		getBrowserRedoMenuItem () .set_label (_ ("Redo"));
		getRedoButton ()          .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem ()        .set_sensitive (false);
		getBrowserRedoMenuItem () .set_sensitive (false);
		getRedoButton ()          .set_sensitive (false);
	}
}

void
X3DBrowserEditor::set_tool_undo (const X3D::UndoStepContainerPtr & container)
{
	const auto & undoStep = container -> getUndoStep ();

	getSelection () -> undoRestoreNodes (undoStep);
	getSelection () -> redoRestoreNodes (undoStep);

	addUndoStep (undoStep);
}

void
X3DBrowserEditor::set_clipboard (const X3D::SFString & string)
{
	getPasteMenuItem ()        .set_sensitive (not string .empty ());
	getBrowserPasteMenuItem () .set_sensitive (not string .empty ());
}

void
X3DBrowserEditor::cutNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	getClipboard () -> set_string () = X3D::X3DEditor::cutNodes (executionContext, nodes, undoStep);
}

void
X3DBrowserEditor::copyNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes)
{
	getClipboard () -> set_string () = X3D::X3DEditor::copyNodes (executionContext, nodes);
}

void
X3DBrowserEditor::pasteNodes (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto scene = getCurrentBrowser () -> createX3DFromString (getClipboard () -> string_changed ());

		if (MagicImport (getBrowserWindow ()) .import (executionContext, nodes, scene, undoStep))
			return;

		const auto & activeLayer = getCurrentWorld () -> getActiveLayer ();
		auto &       children    = activeLayer and activeLayer not_eq getCurrentWorld () -> getLayer0 ()
		                           ? activeLayer -> children ()
											: getCurrentContext () -> getRootNodes ();

		undoStep -> addObjects (getCurrentContext (), activeLayer);

		const auto importedNodes = X3D::X3DEditor::importScene (executionContext,
		                                                        executionContext,
		                                                        executionContext == getCurrentContext ()
		                                                        ? children
															                 : executionContext -> getRootNodes (),
		                                                        scene,
		                                                        undoStep);

		getSelection () -> setNodes (importedNodes, undoStep);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

/***
 *  Create node and add node to current layer root nodes.
 */
X3D::SFNode
X3DBrowserEditor::createNode (const std::string & typeName, const X3D::UndoStepPtr & undoStep)
{
	const auto node = getCurrentContext () -> createNode (typeName);

	X3D::X3DEditor::addNodesToActiveLayer (getCurrentWorld (), { node }, undoStep);
	X3D::X3DEditor::requestUpdateInstances (getCurrentContext (), undoStep);

	getSelection () -> setNodes ({ node }, undoStep);

	return node;
}

/***
 *  Completely removes @a nodes from @a executionContext.
 */
void
X3DBrowserEditor::removeNodesFromScene (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const bool removeFromSceneGraph, const X3D::UndoStepPtr & undoStep) const
{
	getSelection () -> removeNodes (nodes, undoStep);

	X3D::X3DEditor::removeNodesFromScene (executionContext, nodes, removeFromSceneGraph, undoStep);
}

// Undo functions

void
X3DBrowserEditor::translateSelection (const X3D::Vector3f & offset, const bool alongFrontPlane, const ToolType currentTool)
{
	static const std::vector <const char*> undoText = {
		"Nudge Left",
		"Nudge Right",
		"Nudge Up",
		"Nudge Down",
		"Nudge Front",
		"Nudge Back"
	};

	for (const auto & node : basic::make_reverse_range (getSelection () -> getNodes ()))
	{
		X3D::X3DPtr <X3D::X3DTransformNodeTool> first (node);

		if (first)
		{
			if (currentTool not_eq tool or X3D::SFTime::now () - undoTime > UNDO_TIME or nudgeUndoStep not_eq getBrowserWindow () -> getUndoStep ())
				nudgeUndoStep = std::make_shared <X3D::UndoStep> (_ (undoText [currentTool - NUDGE_LEFT]));

			tool     = currentTool;
			undoTime = X3D::SFTime::now ();

			getSelection () -> redoRestoreNodes (nudgeUndoStep);

			// Translate first Transform

			X3D::Matrix4d matrix;
			matrix .set (offset);

			nudgeUndoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, first, first -> getMatrix (), first -> center () .getValue ());
			first -> addAbsoluteMatrix (matrix, first -> getKeepCenter ());
			nudgeUndoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, first, first -> getMatrix (), first -> center () .getValue ());

			// Translate other Transforms

			for (const auto & node : basic::make_reverse_range (getSelection () -> getNodes ()))
			{
				X3D::X3DPtr <X3D::X3DTransformNodeTool> transform (node);

				if (not transform)
					continue;

				if (transform == first)
					continue;

				nudgeUndoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center () .getValue ());
				transform -> addAbsoluteMatrix (matrix, transform -> getKeepCenter ());
				nudgeUndoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center () .getValue ());

				// Prototype support

				X3D::X3DEditor::requestUpdateInstances (transform, nudgeUndoStep);
			}

			// 

			getSelection () -> undoRestoreNodes (nudgeUndoStep);

			if (nudgeUndoStep not_eq getBrowserWindow () -> getUndoStep ())
			{
				addUndoStep (nudgeUndoStep);
			}

			break;
		}
	}
}

// Misc

void
X3DBrowserEditor::editSourceCode (const X3D::SFNode & node)
{
	const auto  cdata    = node -> getSourceText ();
	std::string filename = "/tmp/titania-XXXXXX.js";
	auto        ostream  = os::mkstemps (filename, 3);

	if (not cdata or not ostream)
		return;

	// Output file.

	for (const auto & string : *cdata)
	{
		ostream
			<< "<![CDATA["
			<< X3D::EscapeSourceText (string)
			<< "]]>" << std::endl
			<< std::endl
			<< std::endl;
	}

	static const std::regex CommentEnd (R"/((\*/))/");

	const auto name = std::regex_replace (node -> getName (), CommentEnd, "");

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

	fileMonitor -> signal_changed () .connect (sigc::bind (sigc::mem_fun (this, &X3DBrowserEditor::on_source_code_changed), node));
	getCurrentPage () -> addFileMonitor (file, fileMonitor);

	try
	{
		getCurrentBrowser () -> println ("Trying to start gnome-text-editor ...");

		Gio::AppInfo::create_from_commandline (os::realpath ("/usr/bin/gnome-text-editor"), "", Gio::APP_INFO_CREATE_NONE) -> launch (file);
	}
	catch (...)
	{ }
}

void
X3DBrowserEditor::on_source_code_changed (const Glib::RefPtr <Gio::File> & file,
                                          const Glib::RefPtr <Gio::File> &,
                                          Gio::FileMonitorEvent event,
                                          const X3D::SFNode & node)
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

	X3D::MFString* const cdata = node -> getSourceText ();

	if (string not_eq *cdata)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), cdata -> getName () .c_str ()));

		undoStep -> addObjects (node);

		undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
		undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, string);
		cdata -> setValue (string);

		// Prototype support

		X3D::X3DEditor::requestUpdateInstances (node, undoStep);

		// Add undo step

		addUndoStep (undoStep);
	}

	getCurrentBrowser () -> println (X3D::SFTime (X3D::SFTime::now ()) .toUTCString (), ": ", basic::sprintf (_ ("Script »%s« saved."), node -> getName () .c_str ()));
}

void
X3DBrowserEditor::dispose ()
{
	selection     .reset ();
	nudgeUndoStep .reset ();

	X3DBrowserWidget::dispose ();
}

X3DBrowserEditor::~X3DBrowserEditor ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
