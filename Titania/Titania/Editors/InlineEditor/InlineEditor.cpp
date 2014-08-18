/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "InlineEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"

#include "../../ComposedWidgets/MFStringURLWidget.h"

namespace titania {
namespace puck {

InlineEditor::InlineEditor (BrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DInlineEditorInterface (get_ui ("Dialogs/InlineEditor.xml"), gconf_dir ()),
	                nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                    load (browserWindow, getLoadCheckButton (),  "load"),
	                     url (new MFStringURLWidget (getBrowserWindow (),
	                          getURLTreeView (),
	                          getURLCellRendererText (),
	                          getURLAddButton (),
	                          getURLRemoveButton (),
	                          getURLChooserColumn (),
	                          "url")),
	                bboxSize (getBrowserWindow (),
	                          getBBoxSizeXAdjustment (),
	                          getBBoxSizeYAdjustment (),
	                          getBBoxSizeZAdjustment (),
	                          getBBoxSizeBox (),
	                          "bboxSize"),
	              bboxCenter (getBrowserWindow (),
	                          getBBoxCenterXAdjustment (),
	                          getBBoxCenterYAdjustment (),
	                          getBBoxCenterZAdjustment (),
	                          getBBoxCenterBox (),
	                          "bboxCenter"),
	              inlineNode (),
	               loadState (false)
{
	getLoadCheckButton () .signal_clicked () .connect (sigc::mem_fun (this, &InlineEditor::on_load_clicked));
	setup ();
}

void
InlineEditor::initialize ()
{
	X3DInlineEditorInterface::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &InlineEditor::set_selection);

	set_selection (getBrowser () -> getSelection () -> getChildren ());
}

void
InlineEditor::set_selection (const X3D::MFNode & selection)
{
	getConvertMasterSelectionButton () .set_sensitive (not selection .empty ());

	if (inlineNode)
		inlineNode -> checkLoadState () .removeInterest (this, &InlineEditor::set_loadState);

	if (selection .empty ())
		inlineNode = nullptr;
	else
		inlineNode = selection .back ();

	if (inlineNode)
	{
		inlineNode -> checkLoadState () .addInterest (this, &InlineEditor::set_loadState);
	
		loadState = inlineNode -> load ();

		set_loadState (inlineNode -> checkLoadState ());
	}
	else
	{
		loadState = false;
		set_loadState (X3D::NOT_STARTED_STATE);
	}

	const auto nodes = inlineNode ? X3D::MFNode ({ inlineNode }) : X3D::MFNode ();

	getInlineBox () .set_sensitive (inlineNode);

	nodeName   .setNode  (X3D::SFNode (inlineNode));
	load       .setNodes (nodes);
	url ->      setNodes (nodes);
	bboxSize   .setNodes (nodes);
	bboxCenter .setNodes (nodes);
}

void
InlineEditor::on_index_clicked ()
{
	const auto nodeIndex = std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex"));
	nodeIndex -> setTypes ({ X3D::X3DConstants::Inline });
}

void
InlineEditor::on_convert_master_selection_clicked ()
{
	if (getBrowser () -> getSelection () -> getChildren () .empty ())
		return;

	getWindow () .set_sensitive (false);

	const auto & masterSelection  = getBrowser () -> getSelection () -> getChildren () .back ();
	X3D::MFNode  nodes            = { masterSelection };

	const auto fileSaveDialog = std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false));
	const auto undoStep       = std::make_shared <UndoStep> (_ ("Convert Master Selection Into Inline File"));
	basic::uri worldURL;

	if (fileSaveDialog -> exportNodes (nodes, worldURL, undoStep))
	{
		const auto           name       = X3D::get_name_from_uri (worldURL);
		const auto           uniqueName = getExecutionContext () -> getUniqueName (name);
		const X3D::InlinePtr inlineNode = new X3D::Inline (getExecutionContext ());
		const X3D::MFString  url        = { getExecutionContext () -> getWorldURL () .relative_path (worldURL) .str (), worldURL .str () };

		inlineNode -> url () = url;
		inlineNode -> setup ();

		getBrowserWindow () -> updateNamedNode (getExecutionContext (), uniqueName, X3D::SFNode (inlineNode), undoStep);
		getBrowserWindow () -> replaceNodes (masterSelection, X3D::SFNode (inlineNode), undoStep);
		getBrowserWindow () -> getSelection () -> setChildren ({ inlineNode }, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
		getBrowserWindow () -> expandNodes ({ inlineNode });
	}

	getWindow () .set_sensitive (true);
}

void
InlineEditor::on_update_bounding_box_fields_activate ()
{
	__LOG__ << std::endl;
}

void
InlineEditor::on_fold_back_into_scene_clicked ()
{
	const auto        undoStep         = std::make_shared <UndoStep> (_ ("Fold Inline Back Into Scene"));
	const auto        scene            = inlineNode -> getInternalScene ();
	const X3D::SFNode group            = new X3D::Group (getExecutionContext ());
	const auto        name             = X3D::get_name_from_uri (scene -> getWorldURL ());
	const auto        uniqueName       = getExecutionContext () -> getUniqueName (name);
	const auto        importedRoutes   = getBrowserWindow () -> getImportedRoutes (getExecutionContext (), scene);

	const X3D::GroupPtr groupNode (group);

	getBrowserWindow () -> updateNamedNode (getExecutionContext (), uniqueName, group, undoStep);
	getBrowserWindow () -> replaceNodes (X3D::SFNode (inlineNode), group, undoStep);
	getBrowserWindow () -> importScene (scene, groupNode -> children (), undoStep);
	group -> setup ();

	for (const auto & route : importedRoutes)
		getBrowserWindow () -> addRoute (getExecutionContext (), std::get <0> (route), std::get <1> (route), std::get <2> (route), std::get <3> (route), undoStep);

	getBrowserWindow () -> getSelection () -> setChildren ({ group }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
	getBrowserWindow () -> expandNodes ({ group });
}

void
InlineEditor::on_load_clicked ()
{
	if (not inlineNode)
		return;

	if (inlineNode -> load () == loadState)
		return;

	// Inline is loaded and should be unloaded.  Now create undo step for imported nodes.
	if (loadState and load .getUndoStep ())
		getBrowserWindow () -> removeImportedNodes (inlineNode -> getExecutionContext (), { inlineNode }, load .getUndoStep ());				

	loadState = inlineNode -> load ();
}

void
InlineEditor::set_loadState (const X3D::LoadState loadState)
{
	getFoldBackIntoSceneButton () .set_sensitive (false);

	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getLoadStateLabel () .set_text (_ ("NOT STARTED"));
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getLoadStateLabel () .set_text (_ ("IN PROGRESS"));
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getFoldBackIntoSceneButton () .set_sensitive (true);
			getLoadStateLabel () .set_text (_ ("COMPLETE"));
			break;
		}
		case X3D::FAILED_STATE:
		{
			getLoadStateLabel () .set_text (_ ("FAILED"));
			break;
		}
	}
}

InlineEditor::~InlineEditor ()
{
	dispose ();
}

} // puck
} // titania
