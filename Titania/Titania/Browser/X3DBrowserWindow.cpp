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

#include "X3DBrowserWindow.h"

#include "../Browser/BrowserSelection.h"
#include "../Browser/MagicImport.h"
#include "../Browser/ViewpointObserver.h"

#include "../Editors/GeometryEditor/GeometryEditor.h"
#include "../Editors/GridEditor/AngleTool.h"
#include "../Editors/GridEditor/GridTool.h"

#include "../Widgets/Sidebar/Sidebar.h"
#include "../Widgets/Footer/Footer.h"
#include "../Widgets/OutlineEditor/OutlineEditor.h"
#include "../Widgets/OutlineEditor/OutlineTreeModel.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"
#include "../Widgets/ScriptEditor/ScriptEditor.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

X3DBrowserWindow::X3DBrowserWindow (const X3D::BrowserPtr & browser) :
	 X3DBrowserEditor (browser),
	        clipboard (browser -> getExecutionContext () -> createNode <X3D::Clipboard> ()),
	   geometryEditor (new GeometryEditor (this)),
	          sidebar (new Sidebar (this)),
	           footer (new Footer (this)),
	         gridTool (new GridTool (this)),
	        angleTool (new AngleTool (this)),
	viewpointObserver (new ViewpointObserver (this)),
	             keys (),
	     accelerators (true)
{
	addChildObjects (clipboard);

	clipboard -> target () = "model/x3d+vrml";
}

void
X3DBrowserWindow::initialize ()
{
	X3DBrowserEditor::initialize ();

	geometryEditor -> getWidget () .property_reveal_child () .signal_changed () .connect (sigc::mem_fun (this, &X3DBrowserWindow::on_geometry_editor_reveal_child_changed));

	geometryEditor -> reparent (getBrowserOverlay (), getWindow ());
	sidebar        -> reparent (getSidebarBox (),     getWindow ());
	footer         -> reparent (getFooterBox (),      getWindow ());

	clipboard -> string_changed () .addInterest (&X3DBrowserWindow::set_clipboard, this);
}

void
X3DBrowserWindow::isEditor (const bool value)
{
	X3DBrowserEditor::isEditor (value);

	geometryEditor -> getWidget () .set_reveal_child (value and getConfig () -> getBoolean ("geometryEditor"));
}

void
X3DBrowserWindow::setBrowser (const X3D::BrowserPtr & value)
{
	X3DBrowserEditor::setBrowser (value);
}

const std::shared_ptr <OutlineTreeViewEditor> &
X3DBrowserWindow::getOutlineTreeView () const
{
	return sidebar -> getPage <OutlineEditor> ("OutlineEditor") -> getTreeView ();
}

// Menu

void
X3DBrowserWindow::hasAccelerators (const bool value)
{
	accelerators = value;

	for (const auto & child : getMenubar () .get_children ())
	{
		const auto menuItem = dynamic_cast <Gtk::MenuItem*> (child);

		if (menuItem and menuItem -> get_visible ())
		{
			const auto menu = menuItem -> get_submenu ();

			if (menu)
				menu -> set_sensitive (accelerators);
		}
	}
}

bool
X3DBrowserWindow::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	const auto scriptEditor = footer -> getPage <ScriptEditor> ("ScriptEditor");

	if (scriptEditor -> getModified ())
		scriptEditor -> apply (std::make_shared <X3D::UndoStep> (""));

	return X3DBrowserEditor::save (worldURL, outputStyle, copy);
}

void
X3DBrowserWindow::expandNodes (const X3D::MFNode & nodes)
{
	if (getConfig () -> getBoolean ("followPrimarySelection"))
	{
		getCurrentBrowser () -> addEvent ();
		getCurrentBrowser () -> finished () .addInterest (&X3DBrowserWindow::expandNodesImpl, this, nodes);
	}
}

void
X3DBrowserWindow::cutNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	clipboard -> set_string () = X3D::X3DEditor::cutNodes (executionContext, nodes, undoStep);
}

void
X3DBrowserWindow::copyNodes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes)
{
	clipboard -> set_string () = X3D::X3DEditor::copyNodes (executionContext, nodes);
}

void
X3DBrowserWindow::pasteNodes (const X3D::X3DExecutionContextPtr & executionContext, X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto scene = getCurrentBrowser () -> createX3DFromString (clipboard -> string_changed ());

		if (MagicImport (getBrowserWindow ()) .import (executionContext, nodes, scene, undoStep))
			return;

		const auto & activeLayer = getCurrentWorld () -> getActiveLayer ();
		auto &       children    = activeLayer and activeLayer not_eq getCurrentWorld () -> getLayer0 ()
		                           ? activeLayer -> children ()
											: getCurrentContext () -> getRootNodes ();

		undoStep -> addObjects (getCurrentContext (), activeLayer);

		const auto importedNodes = importScene (executionContext,
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

void
X3DBrowserWindow::set_clipboard (const X3D::SFString & string)
{
	getPasteMenuItem () .set_sensitive (not string .empty ());
}

void
X3DBrowserWindow::on_geometry_editor_clicked ()
{
	geometryEditor -> getWidget () .set_reveal_child (not geometryEditor -> getWidget () .get_reveal_child ());
}

void
X3DBrowserWindow::on_geometry_editor_reveal_child_changed ()
{
	getConfig () -> setItem ("geometryEditor", geometryEditor -> getWidget () .get_reveal_child ());
}

void
X3DBrowserWindow::expandNodesImpl (const X3D::MFNode & nodes)
{
	getCurrentBrowser () -> finished () .removeInterest (&X3DBrowserWindow::expandNodesImpl, this);

	const auto &                outlineTreeView = getOutlineTreeView ();
	std::vector <Gtk::TreePath> paths;

	for (const auto & node : nodes)
	{
		outlineTreeView -> expand_to (node);

		for (const auto & iter : outlineTreeView -> get_iters (node))
		{
			paths .emplace_back (outlineTreeView-> get_model () -> get_path (iter));
			break;
		}
	}

	for (const auto & path : paths)
		outlineTreeView -> expand_row (path, false);

	if  (not paths .empty ())
		outlineTreeView -> scroll_to_row (paths .front (), 2 - math::phi <double>);
}

void
X3DBrowserWindow::store ()
{
	X3DBrowserEditor::store ();
}

void
X3DBrowserWindow::dispose ()
{
	geometryEditor    .reset ();
	sidebar           .reset ();
	footer            .reset ();
	gridTool          .reset ();
	angleTool         .reset ();
	viewpointObserver .reset ();

	X3DBrowserEditor::dispose ();
}

X3DBrowserWindow::~X3DBrowserWindow ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
