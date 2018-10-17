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
#include "../Widgets/Console/Console.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Browser/Core/BrowserOptions.h>
#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataInteger.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/Collision.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Editing/Undo/UndoStepContainer.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Parser/Filter.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Tools/Grouping/X3DTransformNodeTool.h>
#include <Titania/X3D/Tools/SnapTool/SnapTargetTool.h>

#include <Titania/InputOutput/MultiLineComment.h>
#include <Titania/String.h>
#include <Titania/Utility/Map.h>

#include <Titania/X3D/Execution/BindableNodeStack.h>

#include <regex>

namespace titania {
namespace puck {

static constexpr double UNDO_TIME = 0.6; // Key press delay time + 0.1???

X3DBrowserEditor::X3DBrowserEditor (const X3D::BrowserPtr & defaultBrowser) :
	X3DBrowserNotebook (defaultBrowser),
	             scene (),
	  executionContext (defaultBrowser -> getExecutionContext ()),
	           editing (false),
	         selection (new BrowserSelection (getBrowserWindow ())),
	         clipboard (defaultBrowser -> getExecutionContext () -> createNode <X3D::Clipboard> ()),
	     nudgeUndoStep (),
	          undoTime (0),
	              tool (NUDGE_NONE)
{
	addChildObjects (scene,
	                 executionContext,
	                 editing,
	                 clipboard);

	clipboard -> target () = "model/x3d+xml";
}

void
X3DBrowserEditor::initialize ()
{
	X3DBrowserNotebook::initialize ();

	getCurrentContext () .addInterest (&X3DBrowserEditor::set_executionContext, this);

	getBrowserWindow () -> getSelection () -> getSelectGeometry () .addInterest (&X3DBrowserEditor::set_geometries, this);
	getBrowserWindow () -> getSelection () -> getGeometries ()     .addInterest (&X3DBrowserEditor::set_geometries, this);

	getClipboard () -> string_changed () .addInterest (&X3DBrowserWindow::set_clipboard, this);
}

void
X3DBrowserEditor::configure ()
{
	// Workspace
	if (not getConfig () -> hasItem ("environment"))
		getConfig () -> setItem ("environment", 1);

	// SelectLowest
	if (getConfig () -> hasItem ("selectLowest"))
		getSelectLowestAction () -> set_active (getConfig () -> getItem <bool> ("selectLowest"));
	else
		getSelectLowestAction () -> set_active (true);

	// FollowPrimarySelection
	if (getConfig () -> hasItem ("followPrimarySelection"))
		getFollowPrimarySelectionAction () -> set_active (getConfig () -> getItem <bool> ("followPrimarySelection"));
	else
		getFollowPrimarySelectionAction () -> set_active (true);

	// This must be done after.
	X3DBrowserNotebook::configure ();
}

void
X3DBrowserEditor::setPage (const NotebookPagePtr & value)
{
	if (getCurrentPage ())
		getCurrentPage () -> getUndoHistory () -> removeInterest (&X3DBrowserEditor::set_undoHistory, this);

	X3DBrowserNotebook::setPage (value);

	if (getCurrentPage ())
	{
		getCurrentPage () -> getUndoHistory () -> addInterest (&X3DBrowserEditor::set_undoHistory, this);

		set_undoHistory ();
	}
}

void
X3DBrowserEditor::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	const auto & browserOptions = getCurrentBrowser () -> getBrowserOptions ();

	X3DBrowserNotebook::setCurrentContext (value);

	getCurrentBrowser () -> getBrowserOptions () -> assign (browserOptions, true);
}

void
X3DBrowserEditor::set_executionContext ()
{
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
X3DBrowserEditor::set_geometries ()
{
	const auto & selection = getSelection () -> getNodes ();

	if (getSelectionContext (selection))
		set_selection (selection);
	else
		set_selection ({ });
}

void
X3DBrowserEditor::set_selection (const X3D::MFNode & selection_)
{
	X3DBrowserNotebook::set_selection (selection_);

	// Must use unfiltered selection:

	const auto selection  = getSelection () -> getNodes ();
	const auto geometries = getSelection () -> getGeometries ();

	for (const auto & node : getSelection () -> getSelectGeometry () ? geometries : selection)
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
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::Collision:
				{
					const auto collisionNode = dynamic_cast <X3D::Collision*> (node .getValue ());
			
					if (selection .back () == collisionNode -> proxy ())
					{
						collisionNode -> setShowProxy (true);
						break;
					}

					const auto iter = std::find (collisionNode -> children () .begin (),
					                             collisionNode -> children () .end (),
					                             selection .back ());
					
					if (iter == collisionNode -> children () .end ())
						break;

					collisionNode -> setShowProxy (false);
					break;
				}
				case X3D::X3DConstants::Switch:
				{
					const auto switchNode = dynamic_cast <X3D::Switch*> (node .getValue ());
			
					const auto iter = std::find (switchNode -> children () .begin (),
					                             switchNode -> children () .end (),
					                             selection .back ());
			
					if (iter == switchNode -> children () .end ())
						break;

					switchNode -> setPrivateChoice (iter - switchNode -> children () .begin ());
					break;
				}
				default:
					continue;
			}

			break;
		}
	}
}

// File operations

void
X3DBrowserEditor::blank ()
{
	append (get_page ("about/new.x3dv"));
	getBrowserNotebook () .set_current_page (getPages () .size () - 1);
}

void
X3DBrowserEditor::load (const basic::uri & URL)
{
	if (getCurrentPage () -> isSaved ())
		X3DBrowserNotebook::load (URL);
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
			const auto scene         = getCurrentBrowser () -> createX3DFromURL ({ worldURL .str () });
			const auto importedNodes = X3D::X3DEditor::importScene (getCurrentContext (), scene, undoStep);

			// Bind bindables
			magicImport .process (getCurrentContext (), importedNodes, scene, undoStep);

			nodes .append (std::move (importedNodes));
		}
		catch (const X3D::X3DError & error)
		{
			getCurrentBrowser () -> getConsole () -> error (error .what ());
		}
	}

	// If more than one nodes create a parent Transform node.

	if (nodes .size () > 1)
	{
		const auto transformNode = getCurrentContext () -> createNode <X3D::Transform> ();

		transformNode -> children () = std::move (nodes);

		nodes .emplace_back (transformNode);
	}

	// Add nodes to root nodes or layer children.

	if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
	{
		for (const auto & node : nodes)
			X3D::X3DEditor::pushBackIntoArray (layerSet -> getActiveLayer (), layerSet -> getActiveLayer () -> children (), node, undoStep);
	}
	else
	{
		for (const auto & node : nodes)
			X3D::X3DEditor::pushBackIntoArray (getCurrentContext (), getCurrentContext () -> getRootNodes (), node, undoStep);
	}

	// If Snap Target is enabled place nodes at it.

	const auto & snapTarget = getCurrentBrowser () -> getSnapTarget ();

	if (snapTarget -> enabled ())
	{
		const auto targetPosition = X3D::Vector3d (snapTarget -> position () .getValue ());
		const auto targetNormal   = X3D::Vector3d (snapTarget -> normal () .getValue ());
	
		X3D::X3DEditor::moveNodesCenterToTarget (executionContext, nodes, targetPosition, targetNormal, X3D::Vector3d (), X3D::Vector3d (), true, undoStep);
	}

	return nodes;
}

bool
X3DBrowserEditor::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	// Save world
	const bool saved = X3DBrowserNotebook::save (getCurrentScene (), worldURL, outputStyle, copy);

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
		X3DBrowserNotebook::reload ();
}

void
X3DBrowserEditor::close (const NotebookPagePtr page)
{
	getWidget () .grab_focus ();

	if (page -> isSaved ())
		X3DBrowserNotebook::close (page);
}

bool
X3DBrowserEditor::quit ()
{
	getWidget () .grab_focus ();

	const auto pages = getRecentPages ();

	for (const auto & page : pages)
	{
		if (page -> isSaved ())
			continue;

		for (const auto & page : getPages ())
			page -> setSaveConfirmed (false);

		// Cancel quit.
		return true;
	}

	return X3DBrowserNotebook::quit ();
}

void
X3DBrowserEditor::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	getCurrentPage () -> getUndoHistory () -> addUndoStep (undoStep, getCurrentBrowser () -> getCurrentTime ());
}

void
X3DBrowserEditor::removeUndoStep ()
{
	getCurrentPage () -> getUndoHistory () -> removeUndoStep ();
}

const X3D::UndoStepPtr &
X3DBrowserEditor::getUndoStep () const
{
	return getCurrentPage () -> getUndoHistory () -> getUndoStep ();
}

void
X3DBrowserEditor::undo ()
{
	// Focus out tree view cells.
//	getCurrentBrowser () -> grab_focus ();

	getCurrentPage () -> getUndoHistory () -> undo ();
}

void
X3DBrowserEditor::redo ()
{
	// Focus out tree view cells.
//	getCurrentBrowser () -> grab_focus ();

	getCurrentPage () -> getUndoHistory ()  -> redo ();
}

void
X3DBrowserEditor::set_undoHistory ()
{
	const auto & undoHistory = getCurrentPage () -> getUndoHistory ();

	if (undoHistory -> hasUndo ())
	{
		getUndoMenuItem ()        .set_label (undoHistory -> getUndoDescription ());
		getBrowserUndoMenuItem () .set_label (undoHistory -> getUndoDescription ());
		getUndoButton ()          .set_tooltip_text (undoHistory -> getUndoDescription ());
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

	if (undoHistory -> hasRedo ())
	{
		getRedoMenuItem ()        .set_label (undoHistory -> getRedoDescription ());
		getBrowserRedoMenuItem () .set_label (undoHistory -> getRedoDescription ());
		getRedoButton ()          .set_tooltip_text (undoHistory -> getRedoDescription ());
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

		const auto   importedNodes = X3D::X3DEditor::importScene (executionContext, scene, undoStep);
		const auto & layerSet      = getCurrentWorld () -> getLayerSet ();

		undoStep -> addObjects (executionContext, layerSet);

		if (executionContext == getCurrentContext () and layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
		{
			for (const auto & node : importedNodes)
				X3D::X3DEditor::pushBackIntoArray (layerSet -> getActiveLayer (), layerSet -> getActiveLayer () -> children (), node, undoStep);
		}
		else
		{
			for (const auto & node : importedNodes)
				X3D::X3DEditor::pushBackIntoArray (executionContext, executionContext -> getRootNodes (), node, undoStep);
		}

		getSelection () -> setNodes (importedNodes, undoStep);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserEditor::translateSelection (const X3D::Vector3f & offset, const bool alongFrontPlane, const ToolType currentTool)
{
	static const std::vector <std::string> undoText = {
		_ ("Nudge Left"),
		_ ("Nudge Right"),
		_ ("Nudge Up"),
		_ ("Nudge Down"),
		_ ("Nudge Front"),
		_ ("Nudge Back"),
	};

	for (const auto & node : basic::make_reverse_range (getSelection () -> getNodes ()))
	{
		X3D::X3DPtr <X3D::X3DTransformNodeTool> first (node);

		if (first)
		{
			if (currentTool not_eq tool or X3D::SFTime::now () - undoTime > UNDO_TIME or nudgeUndoStep not_eq getBrowserWindow () -> getUndoStep ())
				nudgeUndoStep = std::make_shared <X3D::UndoStep> (undoText [currentTool - NUDGE_LEFT]);

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

void
X3DBrowserEditor::dispose ()
{
	selection     .reset ();
	nudgeUndoStep .reset ();

	X3DBrowserNotebook::dispose ();
}

X3DBrowserEditor::~X3DBrowserEditor ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
