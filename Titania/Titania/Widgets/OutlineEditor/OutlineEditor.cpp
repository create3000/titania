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

#include "OutlineEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "OutlineEditorDatabase.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

OutlineEditor::OutlineEditor (X3DBrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DOutlineEditorInterface (get_ui ("OutlineEditor.glade")),
	                 treeView (new OutlineTreeViewEditor (browserWindow, getCurrentContext ())),
	               sceneGroup (),
	               sceneIndex (),
	                   scenes (),
	                 nodePath (),
	                fieldPath (),
	                 realized (false)
{
	setup ();
}

void
OutlineEditor::on_map ()
{
	getBrowserWindow () -> worldURL_changed () .addInterest (this, &OutlineEditor::set_scenes_menu);

	set_scenes_menu ();
	treeView -> queue_draw ();
}

void
OutlineEditor::on_unmap ()
{
	getBrowserWindow () -> worldURL_changed () .removeInterest (this, &OutlineEditor::set_scenes_menu);
}

void
OutlineEditor::initialize ()
{
	X3DOutlineEditorInterface::initialize ();

	getScrolledWindow () .add (*treeView);
	treeView -> show ();

	getShowExternProtosMenuItem ()         .set_active (getConfig () -> getBoolean ("showExternProtos"));
	getShowPrototypesMenuItem ()           .set_active (getConfig () -> getBoolean ("showPrototypes"));
	getShowImportedNodesMenuItem ()        .set_active (getConfig () -> getBoolean ("showImportedNodes"));
	getShowExportedNodesMenuItem ()        .set_active (getConfig () -> getBoolean ("showExportedNodes"));
	getExpandExternProtosMenuItem ()       .set_active (getConfig () -> getBoolean ("expandExternProtos"));
	getExpandPrototypeInstancesMenuItem () .set_active (getConfig () -> getBoolean ("expandPrototypeInstances"));
	getExpandInlineNodesMenuItem ()        .set_active (getConfig () -> getBoolean ("expandInlineNodes"));

	if (getConfig () -> hasItem ("useLocale"))
	 getUseLocaleMenuItem () .set_active (getConfig () -> getBoolean ("useLocale"));

	getCurrentScene ()   .addInterest (this, &OutlineEditor::set_scene);
	getCurrentContext () .addInterest (this, &OutlineEditor::set_executionContext);

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
	getSceneMenuButton () .set_tooltip_text (getCurrentContext () -> getWorldURL () .str ());

	// Scene menu

	getSceneImage ()          .set_sensitive (not inPrototypeInstance ());
	getPreviousSceneButton () .set_sensitive (menuItem .second not_eq 0);
	getNextSceneButton ()     .set_sensitive (menuItem .second not_eq scenes .size () - 1);

	// Tree view

	const auto & currentScene = treeView -> get_execution_context ();

	if (realized)
		saveExpanded (currentScene);

	treeView -> set_execution_context (getCurrentContext ());

	restoreExpanded (getCurrentContext ());
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

			getPopupMenu () .popup (event -> button, event -> time);
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
				setCurrentContext (instance);

			else
			{
				const auto inlineNode = dynamic_cast <X3D::Inline*> (node .getValue ());

				if (inlineNode)
					setCurrentContext (X3D::X3DExecutionContextPtr (inlineNode -> getInternalScene ()));

				else
					setCurrentContext (node -> getExecutionContext ());
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

			setCurrentContext (prototype);
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto & node         = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (node .getValue ());
				const auto   exportedNode = importedNode -> getExportedNode ();

				setCurrentContext (exportedNode -> getExecutionContext ());
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
			getSceneMenuButton () .set_tooltip_text (currentScene  -> getWorldURL () .str ());
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
	   return std::make_pair (scenes [index - 1] .second, index - 1);

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
		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_scene_activate), menuItem, scenes .size ()));
		menuItem -> show ();

		sceneIndex .emplace (executionContext, i);
		scenes .emplace_back (executionContext, menuItem);

		getSceneMenu () .append (*menuItem);

		menuPair = std::make_pair (menuItem, i);
	}

	return menuPair;
}

std::string
OutlineEditor::getSceneLabelText (const X3D::X3DExecutionContextPtr & scene) const
{
	const auto basename = scene -> getWorldURL () .basename ();
	const auto child    = getSceneMenuLabelText (scene, true);

	return "<i><b>" + std::string (_ ("Current Scene")) + "</b> »" + Glib::Markup::escape_text (basename) + "«</i>" +
	       "<i>" + (scene -> isScene () ? "" : " " + child) + "</i>";
}

std::string
OutlineEditor::getSceneMenuLabelText (const X3D::X3DExecutionContextPtr & scene, const bool markup) const
{
	if (markup)
	{
		const auto basename = scene -> getWorldURL () .basename ();

		return scene -> isType ({ X3D::X3DConstants::ProtoDeclaration, X3D::X3DConstants::X3DPrototypeInstance })
		       ? "<b>" + scene -> getTypeName () + "</b> " + scene -> getName ()
		       : scene -> getTypeName () + " »" + basename + "«";
	}
	else
	{
		const auto basename = scene -> getWorldURL () .basename ();

		return scene -> isType ({ X3D::X3DConstants::ProtoDeclaration, X3D::X3DConstants::X3DPrototypeInstance })
		       ? scene -> getTypeName () + " " + scene -> getName ()
		       : scene -> getTypeName () + " »" + basename + "«";
	}
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

			protoNode -> updateInstances ();
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
			const auto   instance = getBrowserWindow () -> addPrototypeInstance (treeView -> get_execution_context (), node -> getName (), undoStep);

			//getBrowserWindow () -> getSelection () -> setChildren ({ instance }, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_add_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	const auto field     = fieldPtr .getValue ();
	const auto reference = referencePtr .getValue ();
	const auto undoStep  = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Reference To »%s«"), reference -> getName () .c_str ()));

	undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	getBrowserWindow () -> addReference (field, reference, undoStep);
	
	undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	treeView -> queue_draw ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_remove_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	const auto field     = fieldPtr .getValue ();
	const auto reference = referencePtr .getValue ();
	const auto undoStep  = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Remove Reference To »%s«"), reference -> getName () .c_str ()));

	undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	getBrowserWindow () -> removeReference (field, reference, undoStep);
	
	undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
	treeView -> queue_draw ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineEditor::on_remove_activate ()
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

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Node"));

	if (nodePath .size () == 1)
	{
		// Root node

		const X3D::SFNode parent (treeView -> get_execution_context ());
		auto &            rootNodes = treeView -> get_execution_context () -> getRootNodes ();
		const auto        index     = treeView -> get_index (iter);

		getBrowserWindow () -> removeNode (treeView -> get_execution_context (), parent, rootNodes, index, undoStep);
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
				getBrowserWindow () -> removeNode (treeView -> get_execution_context (), parent, *static_cast <X3D::SFNode*> (field), undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				const auto index = treeView -> get_index (iter);

				getBrowserWindow () -> removeNode (treeView -> get_execution_context (), parent, *static_cast <X3D::MFNode*> (field), index, undoStep);
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

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Unlink Clone"));

	if (nodePath .size () == 1)
	{
		// Root node

		const X3D::SFNode parent (treeView -> get_execution_context ());
		auto &            rootNodes = treeView -> get_execution_context () -> getRootNodes ();
		const auto        index     = treeView -> get_index (iter);
		const X3D::SFNode copy      = rootNodes [index] -> copy (X3D::FLAT_COPY);

		getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, rootNodes, index, copy, undoStep);
		copy -> getExecutionContext () -> realize ();
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
				auto &            sfnode = *static_cast <X3D::SFNode*> (field);
				const X3D::SFNode copy   = sfnode -> copy (X3D::FLAT_COPY);

				getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, sfnode, copy, undoStep);
				copy -> getExecutionContext () -> realize ();
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &            mfnode = *static_cast <X3D::MFNode*> (field);
				const auto        index  = treeView -> get_index (iter);
				const X3D::SFNode copy   = mfnode [index] -> copy (X3D::FLAT_COPY);

				getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, mfnode, index, copy, undoStep);
				copy -> getExecutionContext () -> realize ();
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
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
OutlineEditor::on_create_parent (const std::string & typeName, const std::string & fieldName)
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Parent %s"), typeName .c_str ()));

	if (nodePath .size () == 1)
	{
		// Root node

		const auto &      executionContext = treeView -> get_execution_context ();
		const X3D::SFNode parent (executionContext);
		auto &            rootNodes = executionContext -> getRootNodes ();
		const auto        index     = treeView -> get_index (iter);
		const auto        child     = rootNodes [index];
		const auto        group     = executionContext -> createNode (typeName);
		auto &            children  = group -> getField <X3D::MFNode> (fieldName);

		executionContext -> addUninitializedNode (group);
		executionContext -> realize ();

		getBrowserWindow () -> pushBackIntoArray (group, children, child, undoStep);
		getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, rootNodes, index, group, undoStep);
		//getBrowserWindow () -> getSelection () -> setChildren ({ group });
		getBrowserWindow () -> expandNodes (X3D::MFNode ({ group }));
	}
	else
	{
		// Child node

		auto path = nodePath;

		if (not path .up ())
			return;

		const auto fieldIter = treeView -> get_model () -> get_iter (path);

		if (treeView -> get_data_type (fieldIter) not_eq OutlineIterType::X3DField)
			return;

		const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));

		if (not path .up ())
			return;

		const auto parentIter = treeView -> get_model () -> get_iter (path);

		if (treeView -> get_data_type (parentIter) not_eq OutlineIterType::X3DBaseNode)
			return;

		const auto & parent = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));

		//

		const auto & executionContext = treeView -> get_execution_context ();

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto &     child    = *static_cast <X3D::SFNode*> (field);
			   const auto group    = executionContext -> createNode (typeName);
				auto &     children = group -> getField <X3D::MFNode> (fieldName);

				executionContext -> addUninitializedNode (group);
				executionContext -> realize ();

				getBrowserWindow () -> pushBackIntoArray (group, children, child, undoStep);
				getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, child, group, undoStep);
				//getBrowserWindow () -> getSelection () -> setChildren ({ group });
				getBrowserWindow () -> expandNodes (X3D::MFNode ({ group }));
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &       mfnode   = *static_cast <X3D::MFNode*> (field);
				const auto   index    = treeView -> get_index (iter);
				const auto & child    = mfnode [index];
				const auto   group    = executionContext -> createNode (typeName);
				auto &       children = group -> getField <X3D::MFNode> (fieldName);

				executionContext -> addUninitializedNode (group);
				executionContext -> realize ();

				getBrowserWindow () -> pushBackIntoArray (group, children, child, undoStep);
				getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, mfnode, index, group, undoStep);
				//getBrowserWindow () -> getSelection () -> setChildren ({ group });
				getBrowserWindow () -> expandNodes (X3D::MFNode ({ group }));
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
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

	const auto   field    = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (fieldIter));
	const auto & parent   = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
	const auto   undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Parent"));

	if (nodePath .size () == 3)
	{
	   // Root node
		
		const X3D::SFNode root (treeView -> get_execution_context ());
		auto &            rootNodes        = treeView -> get_execution_context () -> getRootNodes ();
		const auto        index            = treeView -> get_index (parentIter);

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto & sfnode = *static_cast <X3D::SFNode*> (field);
	
				getBrowserWindow () -> insertIntoArray (X3D::SFNode (treeView -> get_execution_context ()), rootNodes, index, sfnode, undoStep);

				if (parent -> getCloneCount () < 2)
					getBrowserWindow () -> removeNode (treeView -> get_execution_context (), root, rootNodes, index + 1, undoStep);
				else
					getBrowserWindow () -> eraseFromArray (root, rootNodes, index + 1, undoStep);

				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto & mfnode = *static_cast <X3D::MFNode*> (field);
	
				getBrowserWindow () -> insertIntoArray (X3D::SFNode (treeView -> get_execution_context ()), rootNodes, index, mfnode .begin (), mfnode .end (), undoStep);

				if (parent -> getCloneCount () < 2)
					getBrowserWindow () -> removeNode (treeView -> get_execution_context (), root, rootNodes, index + mfnode .size (), undoStep);
				else
					getBrowserWindow () -> eraseFromArray (root, rootNodes, index + mfnode .size (), undoStep);

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
						getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, *static_cast <X3D::SFNode*> (secondField), sfnode, undoStep);
						break;
					}
					case X3D::X3DConstants::MFNode:
					{
						auto &     mfnode = *static_cast <X3D::MFNode*> (secondField);
						const auto index  = treeView -> get_index (parentIter);

						getBrowserWindow () -> insertIntoArray (secondParent, mfnode, index, sfnode, undoStep);

						if (parent -> getCloneCount () < 2)
							getBrowserWindow () -> removeNode (treeView -> get_execution_context (), parent, mfnode, index + 1, undoStep);
						else
							getBrowserWindow () -> eraseFromArray (secondParent, mfnode, index + 1, undoStep);

						break;
					}
					default:
						break;
				}

				getBrowserWindow () -> removeNode (treeView -> get_execution_context (), parent, sfnode, undoStep);
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
						getBrowserWindow () -> replaceNode (treeView -> get_execution_context (), parent, *static_cast <X3D::SFNode*> (secondField), mfnode [index], undoStep);
						break;
					}
					case X3D::X3DConstants::MFNode:
					{
						auto &     secondmfnode = *static_cast <X3D::MFNode*> (secondField);
						const auto secondIndex  = treeView -> get_index (parentIter);

						getBrowserWindow () -> insertIntoArray (secondParent, secondmfnode, secondIndex, mfnode .begin (), mfnode .end (), undoStep);

						if (parent -> getCloneCount () < 2)
							getBrowserWindow () -> removeNode (treeView -> get_execution_context (), secondParent, secondmfnode, secondIndex + mfnode .size (), undoStep);
						else
							getBrowserWindow () -> eraseFromArray (secondParent, secondmfnode, secondIndex + mfnode .size (), undoStep);

						break;
					}
					default:
						break;
				}
						
				getBrowserWindow () -> replaceNodes (treeView -> get_execution_context (), parent, mfnode, X3D::MFNode (), undoStep);
				break;
			}
			default:
				break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
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
	bool isBaseNode          = false;
	bool isExternProto       = false;
	bool isCompletelyLoaded  = false;
	bool isPrototype         = false;
	bool isPrototypeInstance = false;
	bool isInlineNode        = false;
	bool isUrlObject         = false;
	bool isLocalNode         = false;
	bool inScene             = false;

	if (not nodePath .empty ())
	{
		const auto iter = treeView -> get_model () -> get_iter (nodePath);

		switch (treeView -> get_data_type (iter))
		{
			case OutlineIterType::NULL_:
			{
				if (nodePath .size () > 2)
				{
					const auto parentFieldIter = iter -> parent ();
					const auto parentNodeIter  = parentFieldIter -> parent ();

					if (treeView -> get_data_type (parentFieldIter) not_eq OutlineIterType::X3DField)
						break;

					const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (parentFieldIter));

					if (field -> getType () not_eq X3D::X3DConstants::MFNode)
						break;

					switch (treeView -> get_data_type (parentNodeIter))
					{
						case OutlineIterType::X3DBaseNode:
						case OutlineIterType::ProtoDeclaration:
						case OutlineIterType::ExportedNode:
						{
							const auto & node = *static_cast <X3D::SFNode*> (treeView -> get_object (parentNodeIter));
							
							isBaseNode  = true;
							isLocalNode = node -> getExecutionContext () == treeView -> get_execution_context ();;
							break;
						}
						default:
							break;
					}
				}
				else
				{
					// Root node
					isBaseNode  = true;
					isLocalNode = true;
				}

				break;
			}
			case OutlineIterType::X3DBaseNode:
			{
				const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());
				const auto   urlObject  = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

				if (not sfnode)
					break;

				isCloned            = sfnode -> getCloneCount () > 1;
				isBaseNode          = bool (sfnode);
				isPrototypeInstance = sfnode -> isType ({ X3D::X3DConstants::X3DPrototypeInstance });
				isInlineNode        = inlineNode;
				isUrlObject         = urlObject;
				isCompletelyLoaded  = urlObject and urlObject -> checkLoadState () == X3D::COMPLETE_STATE;
				isLocalNode         = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene             = sfnode -> getScene () == treeView -> get_execution_context ();
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
				inScene            = sfnode -> getScene () == treeView -> get_execution_context ();
				break;
			}
			case OutlineIterType::ProtoDeclaration:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isPrototype = true;
				isLocalNode = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene     = sfnode -> getScene () == treeView -> get_execution_context ();
				break;
			}
			case OutlineIterType::ExportedNode:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isLocalNode = sfnode -> getExecutionContext () == treeView -> get_execution_context ();
				inScene     = sfnode -> getScene () == treeView -> get_execution_context ();
				break;
			}
			default:
				break;
		}
	}

	getSetAsCurrentSceneMenuItem () .set_visible (((isExternProto or isInlineNode) and isCompletelyLoaded) or isPrototype or isPrototypeInstance);
	getCreateInstanceMenuItem ()    .set_visible ((isExternProto or isPrototype) and not inPrototypeInstance () and isLocalNode);
	getReloadMenuItem ()            .set_visible (isUrlObject and inScene and not isInlineNode);
	getUpdateInstancesMenuItem ()   .set_visible ((isPrototype or isExternProto) and not inPrototypeInstance () and isLocalNode);
	
	getProtoSeparator () .set_visible (getSetAsCurrentSceneMenuItem () .get_visible () or
                                      getCreateInstanceMenuItem ()    .get_visible () or
                                      getReloadMenuItem ()            .get_visible () or
	                                   getUpdateInstancesMenuItem ()   .get_visible ());

	getRemoveMenuItem ()            .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode);
	getUnlinkCloneMenuItem ()       .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode and isCloned);
	getCreateParentGroupMenuItem () .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode); // XXX: and is X3DChildNode
	getRemoveParentMenuItem ()      .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode and nodePath .size () > 1);
	
	getEditSeparator () .set_visible (getUnlinkCloneMenuItem ()       .get_visible () or
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
	
				if (field -> getAccessType () == reference -> getAccessType () or field -> getAccessType () == X3D::inputOutput)
				{
					if (field -> getReferences () .count (reference))
						continue;
	
					try
					{
						const auto menuItem = Gtk::manage (new Gtk::MenuItem (reference -> getName ()));
	
						menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_add_reference_activate),
						                                                     X3D::FieldPtr (field),
						                                                     X3D::FieldPtr (reference)));
						menuItem -> show ();
	
						getAddReferenceMenu () .append (*menuItem);
					}
					catch (const X3D::X3DError &)
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
	
					menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_remove_reference_activate),
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

/***********************************************************************************************************************
 *
 * Save & restore expanded nodes
 *
 **********************************************************************************************************************/

void
OutlineEditor::restoreExpanded (const X3D::X3DExecutionContextPtr & executionContext)
{
	if (not executionContext -> isScene ())
		return;

	try
	{
		OutlineEditorDatabase database;

		const auto item  = database .getItem (executionContext -> getWorldURL () .filename ());
		const auto paths = basic::split (std::get <0> (item), ";");

		treeView -> set_adjustments (std::get <1> (item), std::get <2> (item));

		for (const auto & path : paths)
			treeView -> expand_row (Gtk::TreePath (path), false);

		//Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::setAdjustments), std::get <1> (item), std::get <2> (item)));
	}
	catch (...)
	{ }
}

void
OutlineEditor::saveExpanded (const X3D::X3DExecutionContextPtr & executionContext)
{
	if (executionContext -> getWorldURL () .empty ())
		return;

	if (not executionContext -> isScene ())
		return;

	std::deque <std::string> paths;

	getExpanded (treeView -> get_model () -> children (), paths);

	OutlineEditorDatabase database;

	database .setItem (executionContext -> getWorldURL () .filename (),
	                   basic::join (paths, ";"),
	                   getScrolledWindow () .get_hadjustment () -> get_value (),
	                   getScrolledWindow () .get_vadjustment () -> get_value ());
}

void
OutlineEditor::getExpanded (const Gtk::TreeModel::Children & children, std::deque <std::string> & paths) const
{
	for (const auto & child : children)
	{
		const auto path     = treeView -> get_model () -> get_path (child);
		const bool expanded = treeView -> row_expanded (path);

		if (expanded)
		{
			paths .emplace_back (path .to_string ());
			getExpanded (child -> children (), paths);
		}
	}
}

void
OutlineEditor::setAdjustments (const double h, const double v)
{
	getScrolledWindow () .get_hadjustment () -> set_value (h);
	getScrolledWindow () .get_vadjustment () -> set_value (v);
}

OutlineEditor::~OutlineEditor ()
{
	if (isInitialized ())
		saveExpanded (treeView -> get_execution_context ());

	dispose ();
}

} // puck
} // titania
