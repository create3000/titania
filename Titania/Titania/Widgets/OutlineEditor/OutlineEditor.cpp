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

#include "OutlineEditor.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include "OutlineEditorDatabase.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Parser/Filter.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

OutlineEditor::OutlineEditor (X3DBrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DOutlineEditorInterface (get_ui ("Widgets/OutlineEditor.glade")),
	                 treeView (new OutlineTreeViewEditor (browserWindow, getCurrentContext (), this)),
	               sceneGroup (),
	               sceneIndex (),
	                   scenes (),
	                 nodeName (this, getNameEntry (), getRenameButton ()),
	                 nodePath (),
	                fieldPath (),
	                 realized (false)
{
	nodeName .name_changed () .addInterest (&OutlineEditor::set_name, this);

	setup ();
}

void
OutlineEditor::on_map ()
{
	getBrowserWindow () -> worldURL_changed () .addInterest (&OutlineEditor::set_scenes_menu, this);

	set_scenes_menu ();
	treeView -> queue_draw ();
}

void
OutlineEditor::on_unmap ()
{
	getBrowserWindow () -> worldURL_changed () .removeInterest (&OutlineEditor::set_scenes_menu, this);
}

void
OutlineEditor::initialize ()
{
	X3DOutlineEditorInterface::initialize ();

	getScrolledWindow () .add (*treeView);
	treeView -> show ();

	getShowExternProtosMenuItem ()         .set_active (getConfig () -> getItem <bool> ("showExternProtos",         true));
	getShowPrototypesMenuItem ()           .set_active (getConfig () -> getItem <bool> ("showPrototypes",           true));
	getShowImportedNodesMenuItem ()        .set_active (getConfig () -> getItem <bool> ("showImportedNodes",        true));
	getShowExportedNodesMenuItem ()        .set_active (getConfig () -> getItem <bool> ("showExportedNodes",        true));
	getExpandExternProtosMenuItem ()       .set_active (getConfig () -> getItem <bool> ("expandExternProtos",       true));
	getExpandPrototypeInstancesMenuItem () .set_active (getConfig () -> getItem <bool> ("expandPrototypeInstances", true));
	getExpandInlineNodesMenuItem ()        .set_active (getConfig () -> getItem <bool> ("expandInlineNodes",        true));

	getColorizeTreeViewMenuItem () .set_active (getConfig () -> getItem <bool> ("colorizeTreeView", true));
	getUseLocaleMenuItem ()        .set_active (getConfig () -> getItem <bool> ("useLocale",        true));

	getCurrentScene ()   .addInterest (&OutlineEditor::set_scene, this);
	getCurrentContext () .addInterest (&OutlineEditor::set_executionContext, this);

	set_scene ();
	set_executionContext ();

	realized = true;
}

void
OutlineEditor::set_scene ()
{
	// Arrow buttons

	getPreviousSceneButton () .set_sensitive (false);
	getNextSceneButton ()     .set_sensitive (false);
}

void
OutlineEditor::set_executionContext ()
{
	const auto menuItem = addSceneMenuItem (getCurrentContext ());
	menuItem .first -> set_active (true);

	getSceneLabel () .set_markup (getSceneLabelText (getCurrentContext ()));
	getSceneMenuButton () .set_tooltip_text (getCurrentContext () -> getWorldURL () .escape (" ") .str ());

	// Scene menu

	getSceneImage ()          .set_sensitive (not inPrototypeInstance ());
	getPreviousSceneButton () .set_sensitive (menuItem .second not_eq 0);
	getNextSceneButton ()     .set_sensitive (menuItem .second not_eq scenes .size () - 1);

	// Tree view

	saveExpanded ();

	treeView -> set_execution_context (getCurrentContext ());

	restoreExpanded ();
}

// Pointing Device

bool
OutlineEditor::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 3:
		{
			selectNode (event -> x, event -> y);
			selectField (event -> x, event -> y);

			getContextMenu () .popup (event -> button, event -> time);
			return true;
		}
		default:
			break;
	}

	return false;
}

// Edit Menu Item

void
OutlineEditor::on_set_as_current_scene_activate ()
{
	const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			const auto & node     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   instance = dynamic_cast <X3D::X3DPrototypeInstance*> (node .getValue ());

			if (instance)
				setCurrentContext (X3D::X3DExecutionContextPtr (instance));

			else
			{
				const auto inlineNode = dynamic_cast <X3D::Inline*> (node .getValue ());

				if (inlineNode)
					setCurrentContext (X3D::X3DExecutionContextPtr (inlineNode -> getInternalScene ()));

				else
					setCurrentContext (X3D::X3DExecutionContextPtr (node -> getExecutionContext ()));
			}

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & node        = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (node .getValue ());

			try
			{
				if (externProto -> checkLoadState () == X3D::NOT_STARTED_STATE)
					externProto -> requestImmediateLoad ();

				if (externProto -> checkLoadState () == X3D::COMPLETE_STATE)
					setCurrentContext (X3D::X3DExecutionContextPtr (externProto -> getInternalScene ()));
			}
			catch (const X3D::X3DError &)
			{ }

			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & node      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   prototype = dynamic_cast <X3D::ProtoDeclaration*> (node .getValue ());

			prototype -> realize ();

			setCurrentContext (X3D::X3DExecutionContextPtr (prototype));
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto & node         = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (node .getValue ());
				const auto   exportedNode = importedNode -> getExportedNode ();

				setCurrentContext (X3D::X3DExecutionContextPtr (exportedNode -> getExecutionContext ()));
			}
			catch (const X3D::X3DError &)
			{ }

			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_scene_activate (Gtk::RadioMenuItem* const menuItem, const size_t index)
{
	if (menuItem -> get_active ())
	{
		const auto & executionContext = scenes [index] .first;

		if (executionContext not_eq getCurrentContext ())
			setCurrentContext (executionContext);
	}
}

void
OutlineEditor::on_previous_scene_clicked ()
{
	const auto iter = sceneIndex .find (treeView -> get_execution_context ());

	if (iter == sceneIndex .end ())
		return;

	if (iter -> second > 0)
		setCurrentContext (scenes [iter -> second - 1] .first);
}

void
OutlineEditor::on_next_scene_clicked ()
{
	const auto iter = sceneIndex .find (treeView -> get_execution_context ());

	if (iter == sceneIndex .end ())
		return;

	if (iter -> second + 1 < scenes .size ())
		setCurrentContext (scenes [iter -> second + 1] .first);
}

void
OutlineEditor::set_scenes_menu ()
{
	if (scenes .empty ())
		return;

	for (const auto & scene : scenes)
	{
	   const auto & currentScene = scene .first;
	   auto menuItem             = scene .second;

		menuItem -> set_label (getSceneMenuLabelText (currentScene, false));

		if (menuItem -> get_active ())
		{
			getSceneLabel () .set_markup (getSceneLabelText (currentScene));
			getSceneMenuButton () .set_tooltip_text (currentScene  -> getWorldURL () .escape (" ") .str ());
		}
	}
}

std::pair <Gtk::RadioMenuItem*, size_t>
OutlineEditor::addSceneMenuItem (const X3D::X3DExecutionContextPtr & currentScene)
{
	// Restore scene menu.

	X3D::X3DExecutionContext*                executionContext = currentScene;
	std::deque <X3D::X3DExecutionContextPtr> executionContexts;

	while (executionContext not_eq executionContext -> getExecutionContext ())
	{
		executionContexts .emplace_front (executionContext);
		executionContext = executionContext -> getExecutionContext ();
	}

	executionContexts .emplace_front (executionContext);

	size_t i = 0;

	for (size_t size = std::min (scenes .size (), executionContexts .size ()); i < size; ++ i)
	{
	   if (scenes [i] .first not_eq executionContexts [i])
	      break;
	}

	const size_t index = i;

	if (index == executionContexts .size ())
	   return std::pair (scenes [index - 1] .second, index - 1);

	if (i < scenes .size ())
	{
		for (size_t size = scenes .size (); i < size; ++ i)
		{
			sceneIndex .erase (scenes [i] .first);
			getSceneMenu () .remove (*scenes [i] .second);
		}

		scenes .resize (index);
	}

	std::pair <Gtk::RadioMenuItem*, size_t> menuPair;

	for (size_t i = index, size = executionContexts .size (); i < size; ++ i)
	{
	   const auto executionContext = executionContexts [i];
		const auto basename         = executionContext -> getWorldURL () .basename ();

		// Add menu item.

		const auto label    = getSceneMenuLabelText (executionContext, false);
		const auto menuItem = Gtk::manage (new Gtk::RadioMenuItem (sceneGroup, label));
		menuItem -> set_active (true);
		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineEditor::on_scene_activate), menuItem, scenes .size ()));
		menuItem -> show ();

		sceneIndex .emplace (executionContext, i);
		scenes .emplace_back (executionContext, menuItem);

		getSceneMenu () .append (*menuItem);

		menuPair = std::pair (menuItem, i);
	}

	return menuPair;
}

std::string
OutlineEditor::getSceneLabelText (const X3D::X3DExecutionContextPtr & scene) const
{
	const auto basename = scene -> getWorldURL () .escape (" ") .basename ();
	const auto child    = getSceneMenuLabelText (scene, true);

	return "<i><b>" + std::string (_ ("Current Scene")) + "</b> »" + Glib::Markup::escape_text (basename) + "«</i>" +
	       "<i>" + (scene -> isType ({ X3D::X3DConstants::X3DScene }) ? "" : " " + child) + "</i>";
}

std::string
OutlineEditor::getSceneMenuLabelText (const X3D::X3DExecutionContextPtr & scene, const bool markup) const
{
	if (markup)
	{
		const auto basename = scene -> getWorldURL () .escape (" ") .basename ();

		return scene -> isType ({ X3D::X3DConstants::ProtoDeclaration, X3D::X3DConstants::X3DPrototypeInstance })
		       ? "<b>" + scene -> getTypeName () + "</b> " + scene -> getName ()
		       : scene -> getTypeName () + " »" + basename + "«";
	}
	else
	{
		const auto basename = scene -> getWorldURL () .escape (" ") .basename ();

		return scene -> isType ({ X3D::X3DConstants::ProtoDeclaration, X3D::X3DConstants::X3DPrototypeInstance })
		       ? scene -> getTypeName () + " " + scene -> getName ()
		       : scene -> getTypeName () + " »" + basename + "«";
	}
}

void
OutlineEditor::on_rename_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::X3DBaseNode:
			break;
		default:
			return;
	}

	// Update node name composed widget.

	nodeName .setNode (*static_cast <X3D::SFNode*> (treeView -> get_object (iter)));

	// Display popover.

	Gdk::Rectangle rectangle;

	treeView -> get_cell_area (nodePath, *treeView -> getColumn (), rectangle);

	getRenamePopover () .set_pointing_to (rectangle);
	getRenamePopover () .popup ();
}

void
OutlineEditor::set_name ()
{
	getRenamePopover () .popdown ();
}

void
OutlineEditor::on_cut_activate ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cut Node"));

	on_copy_activate ();
	remove (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_copy_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::X3DBaseNode:
			break;
		default:
			return;
	}

	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto   executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());

	getBrowserWindow () -> copyNodes (executionContext, { node });
}

void
OutlineEditor::on_paste_activate ()
{
	auto executionContext = X3D::X3DExecutionContextPtr (treeView -> get_model () -> get_execution_context ());

	if (not nodePath .empty ())
	{
		const auto iter = treeView -> get_model () -> get_iter (nodePath);

		switch (treeView -> get_data_type (iter))
		{
			case OutlineIterType::ProtoDeclaration:
			case OutlineIterType::X3DExecutionContext:
			{
				executionContext = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				break;
			}
			case OutlineIterType::X3DBaseNode:
			{
				const auto & node = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				executionContext = node -> getExecutionContext ();
				break;
			}
			default:
				return;
		}
	}

	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Paste Node"));
	auto       magicNodes = X3D::MFNode ();

	getBrowserWindow () -> pasteNodes (executionContext, magicNodes, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_reload_activated ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & node      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   protoNode = dynamic_cast <X3D::ExternProtoDeclaration*> (node .getValue ());

			protoNode -> setup ();
			protoNode -> url () .addEvent ();
			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_update_instances_activated ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & node      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   protoNode = dynamic_cast <X3D::X3DProtoDeclarationNode*> (node .getValue ());

			X3D::X3DEditor::requestUpdateInstances (X3D::X3DProtoDeclarationNodePtr (protoNode), std::make_shared <X3D::UndoStep> ());
		}
		default:
			break;
	}
}

void
OutlineEditor::OutlineEditor::on_create_instance_activate ()
{
	if (nodePath .empty ())
		return;

	const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & node     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Instance %s"), node -> getName () .c_str ()));
			const auto   instance = X3D::X3DEditor::createProto (getCurrentWorld (), treeView -> get_execution_context (), node -> getName (), undoStep);

			getBrowserWindow () -> getSelection () -> setNodes ({ instance }, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_add_reference_activate (const X3D::SFNode & node, const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	const auto field     = fieldPtr .getValue ();
	const auto reference = referencePtr .getValue ();
	const auto undoStep  = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Reference To %s »%s«"), node -> getTypeName () .c_str (), reference -> getName () .c_str ()));

	undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	X3D::X3DEditor::addReference (node, field, reference, undoStep);

	undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	treeView -> queue_draw ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_remove_reference_activate (const X3D::SFNode & node, const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	const auto field     = fieldPtr .getValue ();
	const auto reference = referencePtr .getValue ();
	const auto undoStep  = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Reference To %s »%s«"), node -> getTypeName () .c_str (), reference -> getName () .c_str ()));

	undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	X3D::X3DEditor::removeReference (node, field, reference, undoStep);

	undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	treeView -> queue_draw ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_remove_activate ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Node"));

	remove (undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::remove (const X3D::UndoStepPtr & undoStep)
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			break;
		default:
			return;
	}

	const auto executionContext = getParentContext (nodePath);

	getBrowserWindow () -> getSelection () -> clearNodes (undoStep);

	if (nodePath .size () == 1 or treeView -> get_data_type (iter -> parent ()) == OutlineIterType::X3DExecutionContext)
	{
		// Root node

		auto &     rootNodes = executionContext -> getRootNodes ();
		const auto index     = treeView -> get_index (iter);

		X3D::X3DEditor::removeNode (executionContext, executionContext, rootNodes, index, undoStep);
	}
	else if (nodePath .size () > 2)
	{
		// Child node

		const auto fieldIter  = iter -> parent ();
		const auto parentIter = fieldIter -> parent ();

		if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
			return;

		switch (treeView -> get_data_type (parentIter))
		{
			case OutlineIterType::ProtoDeclaration:
			case OutlineIterType::X3DBaseNode:
			case OutlineIterType::ExportedNode:
				break;
			default:
				return;
		}

		const auto field        = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));
		auto &     parent       = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
		const auto exportedNode = X3D::X3DPtr <X3D::ExportedNode> (parent);

		if (exportedNode)
			parent = exportedNode -> getLocalNode  ();

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto & sfnode = *static_cast <X3D::SFNode*> (field);

				X3D::X3DEditor::removeNode (executionContext, parent, sfnode, undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &     mfnode = *static_cast <X3D::MFNode*> (field);
				const auto index  = treeView -> get_index (iter);

				X3D::X3DEditor::removeNode (executionContext, parent, mfnode, index, undoStep);
				break;
			}
			default:
				break;
		}
	}
}

void
OutlineEditor::on_create_clone_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto   undoStep         = std::make_shared <X3D::UndoStep> (_ ("Create Clone"));
	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto & executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());
	auto &       rootNodes        = executionContext -> getRootNodes ();

	if (nodePath .size () == 1 or treeView -> get_data_type (iter -> parent ()) == OutlineIterType::X3DExecutionContext)
	{
		// Root node

		const auto index = treeView -> get_index (iter);

		X3D::X3DEditor::insertIntoArray (executionContext, rootNodes, index + 1, node, undoStep);
	}
	else if (nodePath .size () > 2)
	{
		// Child node

		const auto fieldIter = iter -> parent ();

		if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
			return;

		const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				X3D::X3DEditor::pushBackIntoArray (executionContext, rootNodes, node, undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &     mfnode = *static_cast <X3D::MFNode*> (field);
				const auto index  = treeView -> get_index (iter);

				X3D::X3DEditor::insertIntoArray (executionContext, mfnode, index + 1, node, undoStep);
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_unlink_clone_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto   undoStep         = std::make_shared <X3D::UndoStep> (_ ("Unlink Clone"));
	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto & executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());

	if (nodePath .size () == 1 or treeView -> get_data_type (iter -> parent ()) == OutlineIterType::X3DExecutionContext)
	{
		// Root node

		auto &     rootNodes = executionContext -> getRootNodes ();
		const auto index     = treeView -> get_index (iter);
		const auto copy      = X3D::SFNode (rootNodes [index] -> copy (X3D::FLAT_COPY));

		X3D::X3DEditor::replaceNode (executionContext, executionContext, rootNodes, index, copy, undoStep);
	}
	else if (nodePath .size () > 2)
	{
		// Child node

		const auto fieldIter  = iter -> parent ();
		const auto parentIter = fieldIter -> parent ();

		if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
			return;

		switch (treeView -> get_data_type (parentIter))
		{
			case OutlineIterType::ProtoDeclaration:
			case OutlineIterType::X3DBaseNode:
			case OutlineIterType::ExportedNode:
				break;
			default:
				return;
		}

		const auto field        = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));
		auto &     parent       = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
		const auto exportedNode = X3D::X3DPtr <X3D::ExportedNode> (parent);

		if (exportedNode)
			parent = exportedNode -> getLocalNode ();

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto &     sfnode = *static_cast <X3D::SFNode*> (field);
				const auto copy   = X3D::SFNode (sfnode -> copy (X3D::FLAT_COPY));

				X3D::X3DEditor::replaceNode (executionContext, parent, sfnode, copy, undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &     mfnode = *static_cast <X3D::MFNode*> (field);
				const auto index  = treeView -> get_index (iter);
				const auto copy   = X3D::SFNode (mfnode [index] -> copy (X3D::FLAT_COPY));

				X3D::X3DEditor::replaceNode (executionContext, parent, mfnode, index, copy, undoStep);
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_detach_from_group_activated ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
			break;
		default:
			return;
	}

	const auto   undoStep         = std::make_shared <X3D::UndoStep> (_ ("Detach Node From Group"));
	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto & executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());
	auto         root             = X3D::SFNode (executionContext);

	// Determine parent field and parent node.

	X3D::X3DFieldDefinition* field  = nullptr;
	X3D::SFNode*             parent = nullptr;

	if (nodePath .size () == 1)
	{
		field  = &executionContext -> getRootNodes ();
		parent = &root;
	}
	else
	{
		const auto fieldIter  = iter -> parent ();
		const auto parentIter = fieldIter -> parent ();

		switch (treeView -> get_data_type (parentIter))
		{
			case OutlineIterType::ProtoDeclaration:
			case OutlineIterType::X3DBaseNode:
				break;
			default:
				return;
		}

		switch (treeView -> get_data_type (fieldIter))
		{
			case OutlineIterType::X3DField:
			{
				field  = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));
				parent = static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
				break;
			}
			case OutlineIterType::X3DExecutionContext:
			{
				field  = &executionContext -> getRootNodes ();
				parent = &root;
				break;
			}
			default:
				return;
		}
	}

	// Adjust transformation matrix.

	const X3D::X3DPtr <X3D::X3DTransformNode> transform (node);

	if (transform)
	{
		const auto modelMatrix = X3D::X3DEditor::getModelMatrix (executionContext, node);

		X3D::X3DEditor::setMatrix (transform, transform -> getMatrix () * modelMatrix, undoStep);
	}

	// Detach.

	const auto layer     = getLayer (iter -> parent ());
	const auto rootNode  = layer ? X3D::SFNode (layer) : X3D::SFNode (executionContext);
	auto &     rootField = layer ? layer -> children () : executionContext -> getRootNodes ();

	// First insert at root nodes end.
	X3D::X3DEditor::insertIntoArray (rootNode, rootField, rootField .size (), node, undoStep);

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
		{
			auto & sfnode = static_cast <X3D::SFNode &> (*field);

			X3D::X3DEditor::setValue (*parent, sfnode, X3D::SFNode (), undoStep);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			auto &     mfnode = static_cast <X3D::MFNode &> (*field);
			const auto index  = treeView -> get_index (iter);

			X3D::X3DEditor::eraseFromArray (*parent, mfnode, index, undoStep);
			break;
		}
		default:
			break;
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

X3D::X3DPtr <X3D::X3DLayerNode>
OutlineEditor::getLayer (Gtk::TreeIter iter) const
{
	X3D::X3DPtr <X3D::X3DLayerNode> layer;

	while (iter)
	{
		if (treeView -> get_data_type (iter) == OutlineIterType::X3DBaseNode)
		{
			layer = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

			if (layer)
				return layer;
		}

		iter = iter -> parent ();
	}

	return nullptr;
}

void
OutlineEditor::on_create_parent_transform_activate ()
{
	on_create_parent ("Transform");
}

void
OutlineEditor::on_create_parent_group_activate ()
{
	on_create_parent ("Group");
}

void
OutlineEditor::on_create_parent_static_group_activate ()
{
	on_create_parent ("StaticGroup");
}

void
OutlineEditor::on_create_parent_switch_activate ()
{
	on_create_parent ("Switch");
}

void
OutlineEditor::on_create_parent_billboard_activate ()
{
	on_create_parent ("Billboard");
}

void
OutlineEditor::on_create_parent_collision_activate ()
{
	on_create_parent ("Collision");
}

void
OutlineEditor::on_create_parent_lod_activate ()
{
	on_create_parent ("LOD");
}

void
OutlineEditor::on_create_parent_viewpoint_group_activated ()
{
	on_create_parent ("ViewpointGroup");
}

void
OutlineEditor::on_create_parent_anchor_activate ()
{
	on_create_parent ("Anchor");
}

void
OutlineEditor::on_create_parent_layout_layer_activate ()
{
	on_create_parent ("LayoutLayer");
}

void
OutlineEditor::on_create_parent_screen_group_activate ()
{
	on_create_parent ("ScreenGroup");
}

void
OutlineEditor::on_create_parent_layout_group_activate ()
{
	on_create_parent ("LayoutGroup");
}

void
OutlineEditor::on_create_parent_geo_transform_activate ()
{
	on_create_parent ("GeoTransform");
}

void
OutlineEditor::on_create_parent_geo_location_activate ()
{
	on_create_parent ("GeoLocation");
}

void
OutlineEditor::on_create_parent_cad_part_activate ()
{
	on_create_parent ("CADPart");
}

void
OutlineEditor::on_create_parent_cad_assembly_activate ()
{
	on_create_parent ("CADAssembly");
}

void
OutlineEditor::on_create_parent_cad_layer_activate ()
{
	on_create_parent ("CADLayer");
}

void
OutlineEditor::on_create_parent_layer_set_activate ()
{
	on_create_parent ("LayerSet", "layers");
}

void
OutlineEditor::on_create_parent_layer_activate ()
{
	on_create_parent ("Layer");
}

void
OutlineEditor::on_create_parent_viewport_activate ()
{
	on_create_parent ("Viewport");
}

void
OutlineEditor::on_create_parent_pickable_group_activate ()
{
	on_create_parent ("PickableGroup");
}

void
OutlineEditor::on_create_parent_collidable_shape_activate ()
{
	on_create_parent ("CollidableShape", "shape", X3D::X3DConstants::SFNode);
}

void
OutlineEditor::on_create_parent_collidable_offset_activate ()
{
	on_create_parent ("CollidableOffset", "collidable", X3D::X3DConstants::SFNode);
}

X3D::SFNode
OutlineEditor::on_create_parent (const std::string & typeName, const std::string & fieldName, const X3D::FieldType fieldType)
{
	if (nodePath .empty ())
		return nullptr;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return nullptr;

	const auto   undoStep         = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Parent Node %s"), typeName .c_str ()));
	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto & executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());
	const auto   group            = X3D::SFNode (executionContext -> createNode (typeName));

	if (nodePath .size () == 1 or treeView -> get_data_type (iter -> parent ()) == OutlineIterType::X3DExecutionContext)
	{
		// Root node

		auto &     rootNodes = executionContext -> getRootNodes ();
		const auto index     = treeView -> get_index (iter);
		const auto child     = rootNodes [index];

		if (fieldType == X3D::X3DConstants::SFNode)
			X3D::X3DEditor::replaceNode (executionContext, group, group -> getField <X3D::SFNode> (fieldName), child, undoStep);
		else
			X3D::X3DEditor::pushBackIntoArray (group, group -> getField <X3D::MFNode> (fieldName), child, undoStep);

		X3D::X3DEditor::replaceNode (executionContext, executionContext, rootNodes, index, group, undoStep);
	}
	else
	{
		// Child node

		auto path = nodePath;

		if (not path .up ())
			return nullptr;

		const auto fieldIter = treeView -> get_model () -> get_iter (path);

		if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
			return nullptr;

		const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));

		if (not path .up ())
			return nullptr;

		const auto parentIter = treeView -> get_model () -> get_iter (path);

		if (treeView -> get_data_type (parentIter) not_eq OutlineIterType::X3DBaseNode)
			return nullptr;

		const auto & parent = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));

		//

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto & child = *static_cast <X3D::SFNode*> (field);

				if (fieldType == X3D::X3DConstants::SFNode)
					X3D::X3DEditor::replaceNode (executionContext, group, group -> getField <X3D::SFNode> (fieldName), child, undoStep);
				else
					X3D::X3DEditor::pushBackIntoArray (group, group -> getField <X3D::MFNode> (fieldName), child, undoStep);

				X3D::X3DEditor::replaceNode (executionContext, parent, child, group, undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &       mfnode   = *static_cast <X3D::MFNode*> (field);
				const auto   index    = treeView -> get_index (iter);
				const auto & child    = mfnode [index];

				if (fieldType == X3D::X3DConstants::SFNode)
					X3D::X3DEditor::replaceNode (executionContext, group, group -> getField <X3D::SFNode> (fieldName), child, undoStep);
				else
					X3D::X3DEditor::pushBackIntoArray (group, group -> getField <X3D::MFNode> (fieldName), child, undoStep);

				X3D::X3DEditor::replaceNode (executionContext, parent, mfnode, index, group, undoStep);
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> getSelection () -> setNodes ({ group });
	getBrowserWindow () -> addUndoStep (undoStep);
	getBrowserWindow () -> expandNodes ({ group });

	return group;
}

void
OutlineEditor::on_remove_parent_activate ()
{
	if (nodePath .size () < 3)
	   return;

	// Child node

	const auto iter       = treeView -> get_model () -> get_iter (nodePath);
	const auto fieldIter  = iter -> parent ();
	const auto parentIter = fieldIter -> parent ();

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
		return;

	if (treeView -> get_data_type (parentIter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
	const auto   executionContext = X3D::X3DExecutionContextPtr (node -> getExecutionContext ());
	const auto & parentNode       = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
	const auto   field            = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));
	const auto   undoStep         = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Parent Node %s"), X3D::GetDescription (parentNode) .c_str ()));

	getBrowserWindow () -> getSelection () -> clearNodes (undoStep);

	if (nodePath .size () == 3 or (nodePath .size () > 3 and treeView -> get_data_type (parentIter -> parent ()) == OutlineIterType::X3DExecutionContext))
	{
	   // Root node

		auto &     rootNodes = executionContext -> getRootNodes ();
		const auto index     = treeView -> get_index (parentIter);

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto & sfnode = *static_cast <X3D::SFNode*> (field);

				applyTransformation (sfnode, rootNodes [index], undoStep);

				X3D::X3DEditor::insertIntoArray (executionContext, rootNodes, index, sfnode, undoStep);
				X3D::X3DEditor::removeNode (executionContext, executionContext, rootNodes, index + 1, undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto & mfnode = *static_cast <X3D::MFNode*> (field);

				for (const auto & node : mfnode)
					applyTransformation (node, rootNodes [index], undoStep);

				X3D::X3DEditor::insertIntoArray (executionContext, rootNodes, index, mfnode .cbegin (), mfnode .cend (), undoStep);
				X3D::X3DEditor::removeNode (executionContext, executionContext, rootNodes, index + mfnode .size (), undoStep);
				break;
			}
			default:
			   break;
		}
	}
	else if (nodePath .size () > 4)
	{
		const auto secondFieldIter  = parentIter -> parent ();
		const auto secondParentIter = secondFieldIter -> parent ();

		if (treeView -> get_data_type (secondFieldIter) not_eq OutlineIterType::X3DField)
			return;

		if (treeView -> get_data_type (secondParentIter) not_eq OutlineIterType::X3DBaseNode)
			return;

		const auto   secondField  = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (secondFieldIter));
		const auto & secondParent = *static_cast <X3D::SFNode*> (treeView -> get_object (secondParentIter));

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto & sfnode = *static_cast <X3D::SFNode*> (field);

				switch (secondField -> getType ())
				{
					case X3D::X3DConstants::SFNode:
					{
						applyTransformation (sfnode, *static_cast <X3D::SFNode*> (secondField), undoStep);

						X3D::X3DEditor::replaceNode (executionContext, secondParent, *static_cast <X3D::SFNode*> (secondField), sfnode, undoStep);
						break;
					}
					case X3D::X3DConstants::MFNode:
					{
						auto &     mfnode = *static_cast <X3D::MFNode*> (secondField);
						const auto index  = treeView -> get_index (parentIter);

						applyTransformation (sfnode, mfnode [index], undoStep);

						X3D::X3DEditor::insertIntoArray (secondParent, mfnode, index, sfnode, undoStep);
						X3D::X3DEditor::removeNode (executionContext, secondParent, mfnode, index + 1, undoStep);
						break;
					}
					default:
						break;
				}

				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &     mfnode = *static_cast <X3D::MFNode*> (field);
				const auto index  = treeView -> get_index (iter);

				switch (secondField -> getType ())
				{
					case X3D::X3DConstants::SFNode:
					{
						applyTransformation (mfnode [index], *static_cast <X3D::SFNode*> (secondField), undoStep);

						X3D::X3DEditor::replaceNode (executionContext, secondParent, *static_cast <X3D::SFNode*> (secondField), mfnode [index], undoStep);
						break;
					}
					case X3D::X3DConstants::MFNode:
					{
						auto &     secondmfnode = *static_cast <X3D::MFNode*> (secondField);
						const auto secondIndex  = treeView -> get_index (parentIter);

						for (const auto & node : mfnode)
							applyTransformation (node, secondmfnode [secondIndex], undoStep);

						X3D::X3DEditor::insertIntoArray (secondParent, secondmfnode, secondIndex, mfnode .cbegin (), mfnode .cend (), undoStep);
						X3D::X3DEditor::removeNode (executionContext, secondParent, secondmfnode, secondIndex + mfnode .size (), undoStep);
						break;
					}
					default:
						break;
				}

				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::applyTransformation (const X3D::SFNode & node, const X3D::SFNode & parent, const X3D::UndoStepPtr & undoStep)
{
	const auto transform       = X3D::X3DPtr <X3D::X3DTransformNode> (node);
	const auto parentTransform = X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> (parent);

	if (transform and parentTransform)
	{
		const auto matrix = transform -> getMatrix () * parentTransform -> getMatrix ();

		undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrix, transform, transform -> getMatrix ());
		undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrix, transform, matrix);
		transform -> setMatrix (matrix);
	}
}

// View Menu Item

void
OutlineEditor::on_show_extern_protos_toggled ()
{
	getConfig () -> setItem ("showExternProtos", getShowExternProtosMenuItem () .get_active ());
	treeView -> set_show_extern_protos (getShowExternProtosMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_prototypes_toggled ()
{
	getConfig () -> setItem ("showPrototypes", getShowPrototypesMenuItem () .get_active ());
	treeView -> set_show_prototypes (getShowPrototypesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_imported_nodes_toggled ()
{
	getConfig () -> setItem ("showImportedNodes", getShowImportedNodesMenuItem () .get_active ());
	treeView -> set_show_imported_nodes (getShowImportedNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_exported_nodes_toggled ()
{
	getConfig () -> setItem ("showExportedNodes", getShowExportedNodesMenuItem () .get_active ());
	treeView -> set_show_exported_nodes (getShowExportedNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_extern_protos_toggled ()
{
	getConfig () -> setItem ("expandExternProtos", getExpandExternProtosMenuItem () .get_active ());
	treeView -> set_expand_extern_protos (getExpandExternProtosMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_prototype_instances_toggled ()
{
	getConfig () -> setItem ("expandPrototypeInstances", getExpandPrototypeInstancesMenuItem () .get_active ());
	treeView -> set_expand_prototype_instances (getExpandPrototypeInstancesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_inline_nodes_toggled ()
{
	getConfig () -> setItem ("expandInlineNodes", getExpandInlineNodesMenuItem () .get_active ());
	treeView -> set_expand_inline_nodes (getExpandInlineNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_colorize_tree_view_menu_item_toggled ()
{
	getConfig () -> setItem ("colorizeTreeView", getColorizeTreeViewMenuItem () .get_active ());
	treeView -> set_colorize_tree_view (getColorizeTreeViewMenuItem () .get_active ());
}

void
OutlineEditor::on_use_locale_menu_item_toggled ()
{
	getConfig () -> setItem ("useLocale", getUseLocaleMenuItem () .get_active ());
	treeView -> set_use_locale (getUseLocaleMenuItem () .get_active ());
}

void
OutlineEditor::selectNode (const double x, const double y)
{
	nodePath = getNodeAtPosition (x, y);

	bool isCloned            = false;
	bool isNull              = false;
	bool isBaseNode          = false;
	bool isExternProto       = false;
	bool isCompletelyLoaded  = false;
	bool isPrototype         = false;
	bool isPrototypeInstance = false;
	bool isInlineNode        = false;
	bool isUrlObject         = false;
	bool isLocalNode         = false;
	bool inScene             = false;
	bool isProtoNode         = false;
	bool isExecutionContext  = false;

	if (not nodePath .empty ())
	{
		const auto iter = treeView -> get_model () -> get_iter (nodePath);

		switch (treeView -> get_data_type (iter))
		{
			case OutlineIterType::NULL_:
			{
				isNull = true;
				break;
			}
			case OutlineIterType::X3DBaseNode:
			{
				const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());
				const auto   urlObject  = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

				if (not sfnode)
					break;

				isCloned            = sfnode -> getCloneCount () - sfnode -> getMetaCloneCount () > 1;
				isBaseNode          = bool (sfnode);
				isPrototypeInstance = sfnode -> isType ({ X3D::X3DConstants::X3DPrototypeInstance });
				isInlineNode        = inlineNode;
				isUrlObject         = urlObject;
				isCompletelyLoaded  = urlObject and urlObject -> checkLoadState () == X3D::COMPLETE_STATE;
				isLocalNode         = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene             = getInScene (sfnode);
				isProtoNode         = sfnode -> getExecutionContext () -> isType ({ X3D::X3DConstants::ProtoDeclaration }) and inScene;
				break;
			}
			case OutlineIterType::ExternProtoDeclaration:
			{
				const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

				isExternProto      = true;
				isUrlObject        = true;
				isCompletelyLoaded = externProto -> checkLoadState () == X3D::COMPLETE_STATE;
				isLocalNode        = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene            = getInScene (sfnode);
				break;
			}
			case OutlineIterType::ProtoDeclaration:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isPrototype        = true;
				isLocalNode        = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene            = getInScene (sfnode);
				isExecutionContext = true;
				break;
			}
			case OutlineIterType::ExportedNode:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isLocalNode = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene     = getInScene (sfnode);
				break;
			}
			case OutlineIterType::X3DExecutionContext:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isExecutionContext = sfnode -> isType ({ X3D::X3DConstants::ProtoDeclaration });
				break;
			}
			default:
				break;
		}
	}

	const bool isNode = not inPrototypeInstance () and (isProtoNode or isLocalNode) and isBaseNode;

	// Common

	getRenameMenuItem () .set_visible ((isExternProto or isPrototype or isNode) and inScene);
	getCommonSeparator () .set_visible (getRenameMenuItem () .get_visible ());

	// Clipboard

	getCutMenuItem ()   .set_visible (isNode);
	getCopyMenuItem ()  .set_visible (isNode or isExternProto or isPrototype);
	getPasteMenuItem () .set_visible (isNode or isExecutionContext or nodePath .empty ());
	getPasteMenuItem () .set_sensitive (not getBrowserWindow () -> getClipboard () -> string_changed () .empty ());

	getClipboardSeparator () .set_visible (getCutMenuItem ()   .get_visible () or
                                          getCopyMenuItem ()  .get_visible () or
                                          getPasteMenuItem () .get_visible ());

	// Proto Edit

	getSetAsCurrentSceneMenuItem () .set_visible (((isExternProto or isInlineNode) and isCompletelyLoaded) or isPrototype or isPrototypeInstance);
	getCreateInstanceMenuItem ()    .set_visible ((isExternProto or isPrototype) and not inPrototypeInstance () and isLocalNode);
	getReloadMenuItem ()            .set_visible (isUrlObject and inScene and not isInlineNode);
	getUpdateInstancesMenuItem ()   .set_visible ((isPrototype or isExternProto) and not inPrototypeInstance () and isLocalNode);

	getProtoSeparator () .set_visible (getSetAsCurrentSceneMenuItem () .get_visible () or
                                      getCreateInstanceMenuItem ()    .get_visible () or
                                      getReloadMenuItem ()            .get_visible () or
	                                   getUpdateInstancesMenuItem ()   .get_visible ());

	// Node Edit

	getRemoveMenuItem ()            .set_visible (isNode or isNull);
	getCreateCloneMenuItem ()       .set_visible (isNode);
	getUnlinkCloneMenuItem ()       .set_visible (isNode and isCloned);
	getDetachFromGroupMenuItem ()   .set_visible (isNode);
	getCreateParentGroupMenuItem () .set_visible (isNode); // XXX: and is X3DChildNode
	getRemoveParentMenuItem ()      .set_visible (isNode and nodePath .size () > 1);

	getEditSeparator () .set_visible (getCreateCloneMenuItem ()       .get_visible () or
                                     getUnlinkCloneMenuItem ()       .get_visible () or
                                     getDetachFromGroupMenuItem ()   .get_visible () or
                                     getCreateParentGroupMenuItem () .get_visible () or
                                     getRemoveParentMenuItem ()      .get_visible () or
                                     getRemoveMenuItem ()            .get_visible ());
}

void
OutlineEditor::selectField (const double x, const double y)
{
	fieldPath = getFieldAtPosition (x, y);

	const bool isField            = not fieldPath .empty ();
	bool       inProtoDeclaration = false;
	bool       hasReferences      = false;

	if (isField)
	{
		const auto   iter             = treeView -> get_model () -> get_iter (fieldPath);
		const auto   field            = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
		const auto & node             = *static_cast <X3D::SFNode*> (treeView -> get_object (iter -> parent ()));
		const auto   executionContext = node -> getExecutionContext ();

		inProtoDeclaration = executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration });

		if (inProtoDeclaration)
		{
			hasReferences = not field -> getReferences () .empty ();

			// Create reference menu

			for (const auto & widget : getAddReferenceMenu () .get_children ())
				getAddReferenceMenu () .remove (*widget);

			for (const auto & reference : executionContext -> getFieldDefinitions ())
			{
				if (field -> getType () not_eq reference -> getType ())
					continue;

				if (reference -> isReference (field -> getAccessType ()))
				{
					try
					{
						if (field -> getReferences () .count (reference))
							continue;

						const auto menuItem = Gtk::manage (new Gtk::MenuItem (reference -> getName ()));

						menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineEditor::on_add_reference_activate),
						                                                     node,
						                                                     X3D::FieldPtr (field),
						                                                     X3D::FieldPtr (reference)));
						menuItem -> show ();

						getAddReferenceMenu () .append (*menuItem);
					}
					catch (const X3D::X3DError & error)
					{
						// menuItem ???
					}
				}
			}

			// Remove reference menu

			for (const auto & widget : getRemoveReferenceMenu () .get_children ())
				getRemoveReferenceMenu () .remove (*widget);

			for (const auto & reference : field -> getReferences ())
			{
				try
				{
					const auto menuItem = Gtk::manage (new Gtk::MenuItem (reference -> getName ()));

					menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineEditor::on_remove_reference_activate),
					                                                     node,
					                                                     X3D::FieldPtr (field),
					                                                     X3D::FieldPtr (reference)));
					menuItem -> show ();

					getRemoveReferenceMenu () .append (*menuItem);
				}
				catch (const X3D::X3DError &)
				{
					// menuItem ???
				}
			}
		}
	}

	getAddReferenceMenuItem ()    .set_visible (isField and inProtoDeclaration and not getAddReferenceMenu () .get_children () .empty ());
	getRemoveReferenceMenuItem () .set_visible (isField and inProtoDeclaration and hasReferences);
	getReferencesSeparator ()     .set_visible (getAddReferenceMenuItem () .get_visible () or getRemoveReferenceMenuItem () .get_visible ());
}

Gtk::TreePath
OutlineEditor::getNodeAtPosition (const double x, const double y)
{
	const auto path = getPathAtPosition (x, y);

	if (path .empty ())
		return Gtk::TreePath ();

	const auto iter = treeView -> get_model () -> get_iter (path);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DExecutionContext:
		{
			auto nodePath = path;

			if (not nodePath .up ())
				break;

			const auto parentIter = treeView -> get_model () -> get_iter (nodePath);

			switch (treeView -> get_data_type (parentIter))
			{
				case OutlineIterType::Separator:
				case OutlineIterType::NULL_:
				case OutlineIterType::X3DBaseNode:
				case OutlineIterType::ExternProtoDeclaration:
				case OutlineIterType::ProtoDeclaration:
				case OutlineIterType::ImportedNode:
				case OutlineIterType::ExportedNode:
					return nodePath;
				default:
					break;
			}

			break;
		}
		case OutlineIterType::Separator:
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
			return path;
		default:
			break;
	}

	return Gtk::TreePath ();
}

Gtk::TreePath
OutlineEditor::getFieldAtPosition (const double x, const double y)
{
	const auto path = getPathAtPosition (x, y);

	if (path .empty ())
		return Gtk::TreePath ();

	const auto iter = treeView -> get_model () -> get_iter (path);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
			break;
		default:
			return Gtk::TreePath ();
	}

	return path;
}

Gtk::TreePath
OutlineEditor::getPathAtPosition (const double x, const double y)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	treeView -> get_path_at_pos (x, y, path, column, cell_x, cell_y);

	return path;
}

X3D::X3DExecutionContextPtr
OutlineEditor::getParentContext (Gtk::TreePath parentPath) const
{
	while (parentPath .size () > 1)
	{
		parentPath .up ();

		const auto parentIter = treeView -> get_model () -> get_iter (parentPath);

		if (treeView -> get_data_type (parentIter) == OutlineIterType::X3DExecutionContext)
		{
			return X3D::X3DExecutionContextPtr (*static_cast <X3D::SFNode*> (treeView -> get_object (parentIter)));
		}
	}

	return treeView -> get_execution_context ();
}

bool
OutlineEditor::getInScene (const X3D::SFNode & node) const
{
	if (treeView -> get_execution_context () -> isType ({ X3D::X3DConstants::X3DScene }))
		return node -> getScene () == treeView -> get_execution_context ();

	return node -> getScene () == treeView -> get_execution_context () -> getScene ();
}

/***********************************************************************************************************************
 *
 * Save & restore expanded nodes
 *
 **********************************************************************************************************************/

void
OutlineEditor::restoreExpanded ()
{
	try
	{
		const auto & executionContext = treeView -> get_execution_context ();

		OutlineEditorDatabase database;

		const auto item  = database .getItem (getPathFromContext (executionContext));
		auto       paths = std::vector <std::string> ();

		basic::split (std::back_inserter (paths), std::get <0> (item), ";");

		treeView -> getScrollFreezer () -> restore (std::get <1> (item), std::get <2> (item));

		for (const auto & path_expanded : paths)
		{
			auto pair = std::vector <std::string> ();

			basic::split (std::back_inserter (pair), path_expanded, "\t");

			if (pair .empty ())
				continue;

			auto expanded = OutlineExpanded::UNDEFINED;

			try
			{
				static const std::map <std::string, OutlineExpanded> expandedTypes = {
					std::pair ("0", OutlineExpanded::UNDEFINED),
					std::pair ("1", OutlineExpanded::COLLAPSED),
					std::pair ("2", OutlineExpanded::CHANGED),
					std::pair ("3", OutlineExpanded::FULL),
				};

				expanded = expandedTypes .at (pair .at (1));
			}
			catch (const std::out_of_range & error)
			{
				expanded = OutlineExpanded::UNDEFINED;
			}

			treeView -> expand_row (Gtk::TreePath (pair [0]), false, expanded);
		}
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
OutlineEditor::saveExpanded ()
{
	if (not realized)
		return;

	const auto & executionContext = treeView -> get_execution_context ();

	saveExpandedImpl ();

	if (executionContext -> isType ({ X3D::X3DConstants::X3DScene }))
		return;

	treeView -> set_execution_context (X3D::X3DExecutionContextPtr (executionContext -> getScene ()));

	saveExpandedImpl ();
}

void
OutlineEditor::saveExpandedImpl ()
{
	try
	{
		const auto & executionContext = treeView -> get_execution_context ();

		if (executionContext -> getWorldURL () .empty ())
			return;

		std::deque <std::string> paths;

		getExpanded (treeView -> get_model () -> children (), paths);

		OutlineEditorDatabase database;

		database .setItem (getPathFromContext (executionContext),
		                   basic::join (paths, ";"),
		                   getScrolledWindow () .get_hadjustment () -> get_value (),
		                   getScrolledWindow () .get_vadjustment () -> get_value ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
OutlineEditor::getExpanded (const Gtk::TreeModel::Children & children, std::deque <std::string> & paths) const
{
	for (const auto & child : children)
	{
		const auto path     = treeView -> get_model () -> get_path (child);
		const auto expanded = treeView -> get_expanded (child);

		paths .emplace_back (path .to_string () + "\t" + basic::to_string (size_t (expanded), std::locale::classic ()));
		getExpanded (child -> children (), paths);
	}
}

OutlineEditor::~OutlineEditor ()
{
	saveExpanded ();

	dispose ();
}

} // puck
} // titania
