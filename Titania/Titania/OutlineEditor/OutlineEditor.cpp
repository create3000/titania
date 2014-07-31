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

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"
#include "../OutlineEditor/OutlineEditorDatabase.h"
#include "../OutlineEditor/OutlineTreeModel.h"
#include "../OutlineEditor/OutlineTreeViewEditor.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

OutlineEditor::OutlineEditor (BrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DOutlineEditorInterface (get_ui ("OutlineEditor.xml"), gconf_dir ()),
	                 treeView (new OutlineTreeViewEditor (browserWindow, getExecutionContext ())),
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
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("Outline Editor"));
}

void
OutlineEditor::initialize ()
{
	X3DOutlineEditorInterface::initialize ();

	getViewport () .add (*treeView);
	treeView -> show ();

	getShowExternProtosMenuItem ()         .set_active (getConfig () .getBoolean ("showExternProtos"));
	getShowPrototypesMenuItem ()           .set_active (getConfig () .getBoolean ("showPrototypes"));
	getShowImportedNodesMenuItem ()        .set_active (getConfig () .getBoolean ("showImportedNodes"));
	getShowExportedNodesMenuItem ()        .set_active (getConfig () .getBoolean ("showExportedNodes"));
	getExpandExternProtosMenuItem ()       .set_active (getConfig () .getBoolean ("expandExternProtos"));
	getExpandPrototypeInstancesMenuItem () .set_active (getConfig () .getBoolean ("expandPrototypeInstances"));
	getExpandInlineNodesMenuItem ()        .set_active (getConfig () .getBoolean ("expandInlineNodes"));

	realized = true;

	getScene ()            .addInterest (this, &OutlineEditor::set_scene);
	getExecutionContext () .addInterest (this, &OutlineEditor::set_executionContext);

	set_scene ();
	set_executionContext ();
}

void
OutlineEditor::set_scene ()
{
	addSceneMenuItem (X3D::X3DExecutionContextPtr (), X3D::X3DExecutionContextPtr (getScene ()));

	getPreviousSceneButton () .set_sensitive (false);
	getNextSceneButton ()     .set_sensitive (false);
}

void
OutlineEditor::set_executionContext ()
{
	if (not realized)
		return;

	// Scene menu

	const X3D::X3DExecutionContextPtr & currentScene = treeView -> get_model () -> get_execution_context ();

	const auto menuItem = addSceneMenuItem (currentScene, getExecutionContext ());
	menuItem .first -> set_active (true);

	getSceneImage ()          .set_sensitive (not inPrototypeInstance ());
	getPreviousSceneButton () .set_sensitive (menuItem .second not_eq 0);
	getNextSceneButton ()     .set_sensitive (menuItem .second not_eq scenes .size () - 1);

	// Tree view

	saveExpanded (currentScene);

	treeView -> set_execution_context (getExecutionContext ());

	restoreExpanded (getExecutionContext ());
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
			const auto & sfnode   = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   instance = dynamic_cast <X3D::X3DPrototypeInstance*> (sfnode .getValue ());

			if (instance)
			{
				const auto menuItem = addSceneMenuItem (getExecutionContext (),
				                                        X3D::X3DExecutionContextPtr (instance));
				menuItem .first -> activate ();
			}
			else
			{
				const auto inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

				if (inlineNode)
				{
					const auto menuItem = addSceneMenuItem (getExecutionContext (),
					                                        X3D::X3DExecutionContextPtr (inlineNode -> getInternalScene ()));
					menuItem .first -> activate ();
				}
				else if (sfnode)
				{
					const auto menuItem = addSceneMenuItem (getExecutionContext (),
					                                        sfnode -> getExecutionContext ());
					menuItem .first -> activate ();
				}
			}

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			try
			{
				if (externProto -> checkLoadState () == X3D::NOT_STARTED_STATE)
					externProto -> requestImmediateLoad ();

				if (externProto -> checkLoadState () == X3D::COMPLETE_STATE)
				{
					const auto menuItem = addSceneMenuItem (getExecutionContext (),
					                                        X3D::X3DExecutionContextPtr (externProto -> getInternalScene ()));
					menuItem .first -> activate ();
				}
			}
			catch (const X3D::X3DError &)
			{ }

			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode    = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   prototype = dynamic_cast <X3D::ProtoDeclaration*> (sfnode .getValue ());

			prototype -> realize ();

			addSceneMenuItem (getExecutionContext (), prototype) .first -> activate ();
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto & sfnode       = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode .getValue ());
				const auto   exportedNode = importedNode -> getExportedNode ();

				const auto menuItem = addSceneMenuItem (getExecutionContext (),
				                                        exportedNode -> getExecutionContext ());
				menuItem .first -> activate ();
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
		const auto & scene = scenes [index] .first;

		if (scene not_eq getExecutionContext ())
			setExecutionContext (scene);
	}
}

void
OutlineEditor::on_previous_scene_clicked ()
{
	const auto iter = sceneIndex .find (getExecutionContext ());

	if (iter == sceneIndex .end ())
		return;

	if (iter -> second > 0)
		scenes [iter -> second - 1] .second -> set_active (true);
}

void
OutlineEditor::on_next_scene_clicked ()
{
	const auto iter = sceneIndex .find (getExecutionContext ());

	if (iter == sceneIndex .end ())
		return;

	if (iter -> second + 1 < scenes .size ())
		scenes [iter -> second + 1] .second -> set_active (true);
}

std::pair <Gtk::RadioMenuItem*, size_t>
OutlineEditor::addSceneMenuItem (const X3D::X3DExecutionContextPtr & currentScene, const X3D::X3DExecutionContextPtr & scene)
{
	const auto basename = scene -> getWorldURL () .basename ();
	const auto iter     = sceneIndex .find (scene);

	getSceneLabel () .set_markup ("<i><b>" + std::string (_ ("Current Scene")) + "</b> »" + Glib::Markup::escape_text (basename) + "«</i>");
	getSceneMenuButton () .set_tooltip_text (scene -> getWorldURL () .str ());

	if (currentScene)
	{
		if (iter not_eq sceneIndex .end ())
			return std::make_pair (scenes [iter -> second] .second, iter -> second);

		// Remove menu items.

		const auto iter = sceneIndex .find (currentScene);

		if (iter not_eq sceneIndex .end ())
		{
			const size_t first = iter -> second + 1;

			for (size_t i = first, size = scenes .size (); i < size; ++ i)
			{
				sceneIndex .erase (scenes [i] .first);
				getSceneMenu () .remove (*scenes [i] .second);
			}

			scenes .resize (first);
		}
	}
	else
	{
		// Remove all menu items.

		for (const auto & widget : getSceneMenu () .get_children ())
			getSceneMenu () .remove (*widget);

		sceneIndex .clear ();
		scenes .clear ();
	}

	// Add menu item.

	const auto label = scene -> isProtoDeclaration () or dynamic_cast <X3D::X3DPrototypeInstance*> (scene .getValue ())
	                   ? scene -> getTypeName () + " " + scene -> getName ()
							 : scene -> getTypeName () + " »" + basename + "«";

	const auto menuItem = Gtk::manage (new Gtk::RadioMenuItem (sceneGroup, label));
	menuItem -> set_active (true);
	menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_scene_activate), menuItem, scenes .size ()));
	menuItem -> show ();

	sceneIndex .emplace (scene, scenes .size ());
	scenes .emplace_back (scene, menuItem);

	getSceneMenu () .append (*menuItem);

	return std::make_pair (menuItem, scenes .size () - 1);
}

void
OutlineEditor::OutlineEditor::on_create_instance_activate ()
{
	const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (nodePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

			getBrowserWindow () -> addPrototypeInstance (externProto -> getName ());
			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode    = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
			const auto   prototype = dynamic_cast <X3D::ProtoDeclaration*> (sfnode .getValue ());

			getBrowserWindow () -> addPrototypeInstance (prototype -> getName ());
			break;
		}
		default:
			break;
	}
}

void
OutlineEditor::on_create_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	try
	{
		const auto field     = fieldPtr .getValue ();
		const auto reference = referencePtr .getValue ();
		const auto undoStep  = std::make_shared <UndoStep> (basic::sprintf (_ ("Create Reference To »%s«"), reference -> getName () .c_str ()));

		undoStep -> addVariables (fieldPtr, referencePtr);
		undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
		undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::removeIsReference, field, reference);
		undoStep -> addRedoFunction (&X3D::X3DFieldDefinition::addIsReference,    field, reference);
		undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);

		field -> addIsReference (reference);
		treeView -> queue_draw ();

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
OutlineEditor::on_remove_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr)
{
	try
	{
		const auto field     = fieldPtr .getValue ();
		const auto reference = referencePtr .getValue ();
		const auto undoStep  = std::make_shared <UndoStep> (basic::sprintf (_ ("Remove Reference To »%s«"), reference -> getName () .c_str ()));

		undoStep -> addVariables (fieldPtr, referencePtr);
		undoStep -> addUndoFunction (&OutlineTreeViewEditor::queue_draw, treeView);
		undoStep -> addUndoFunction (&X3D::X3DFieldDefinition::addIsReference,    field, reference);
		undoStep -> addRedoFunction (&X3D::X3DFieldDefinition::removeIsReference, field, reference);
		undoStep -> addRedoFunction (&OutlineTreeViewEditor::queue_draw, treeView);

		field -> removeIsReference (reference);
		treeView -> queue_draw ();

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
OutlineEditor::on_unlink_clone_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Unlink Clone"));

	if (nodePath .size () == 1)
	{
		// Root node

		const X3D::SFNode parent (treeView -> get_model () -> get_execution_context ());
		auto &            rootNodes = treeView -> get_model () -> get_execution_context () -> getRootNodes ();
		const auto        index     = treeView -> get_index (iter);
		const X3D::SFNode copy      = rootNodes [index] -> copy (X3D::FLAT_COPY);

		getBrowserWindow () -> replaceNode (parent, rootNodes, index, copy, undoStep);
		copy -> getExecutionContext () -> realize ();
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
	
		const auto parent = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				auto &            sfnode = *static_cast <X3D::SFNode*> (field);
				const X3D::SFNode copy   = sfnode -> copy (X3D::FLAT_COPY);

				getBrowserWindow () -> replaceNode (parent, sfnode, copy, undoStep);
				copy -> getExecutionContext () -> realize ();
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				auto &            mfnode = *static_cast <X3D::MFNode*> (field);
				const auto        index  = treeView -> get_index (iter);
				const X3D::SFNode copy   = mfnode [index] -> copy (X3D::FLAT_COPY);

				getBrowserWindow () -> replaceNode (parent, mfnode, index, copy, undoStep);
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
OutlineEditor::on_remove_activate ()
{
	if (nodePath .empty ())
		return;

	const auto iter = treeView -> get_model () -> get_iter (nodePath);

	if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DBaseNode)
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Delete Node"));

	if (nodePath .size () == 1)
	{
		// Root node

		const X3D::SFNode parent (treeView -> get_model () -> get_execution_context ());
		auto &            rootNodes = treeView -> get_model () -> get_execution_context () -> getRootNodes ();
		const auto        index     = treeView -> get_index (iter);

		getBrowserWindow () -> removeNode (parent, rootNodes, index, undoStep);
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

		const auto parent = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));

		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode:
			{
				getBrowserWindow () -> removeNode (parent, *static_cast <X3D::SFNode*> (field), undoStep);
				break;
			}
			case X3D::X3DConstants::MFNode:
			{
				const auto index = treeView -> get_index (iter);

				getBrowserWindow () -> removeNode (parent, *static_cast <X3D::MFNode*> (field), index, undoStep);
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
	getConfig () .setItem ("showExternProtos", getShowExternProtosMenuItem () .get_active ());
	treeView -> set_show_extern_protos (getShowExternProtosMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_prototypes_toggled ()
{
	getConfig () .setItem ("showPrototypes", getShowPrototypesMenuItem () .get_active ());
	treeView -> set_show_prototypes (getShowPrototypesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_imported_nodes_toggled ()
{
	getConfig () .setItem ("showImportedNodes", getShowImportedNodesMenuItem () .get_active ());
	treeView -> set_show_imported_nodes (getShowImportedNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_show_exported_nodes_toggled ()
{
	getConfig () .setItem ("showExportedNodes", getShowExportedNodesMenuItem () .get_active ());
	treeView -> set_show_exported_nodes (getShowExportedNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_extern_protos_toggled ()
{
	getConfig () .setItem ("expandExternProtos", getExpandExternProtosMenuItem () .get_active ());
	treeView -> set_expand_extern_protos (getExpandExternProtosMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_prototype_instances_toggled ()
{
	getConfig () .setItem ("expandPrototypeInstances", getExpandPrototypeInstancesMenuItem () .get_active ());
	treeView -> set_expand_prototype_instances (getExpandPrototypeInstancesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::on_expand_inline_nodes_toggled ()
{
	getConfig () .setItem ("expandInlineNodes", getExpandInlineNodesMenuItem () .get_active ());
	treeView -> set_expand_inline_nodes (getExpandInlineNodesMenuItem () .get_active ());
	set_executionContext ();
}

void
OutlineEditor::selectNode (const double x, const double y)
{
	nodePath = getNodeAtPosition (x, y);

	bool isCloned            = false;
	bool isBaseNode          = false;
	bool isExternProto       = false;
	bool isPrototype         = false;
	bool isPrototypeInstance = false;
	bool isInlineNode        = false;
	bool isLocalNode         = true;

	if (not nodePath .empty ())
	{
		const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (nodePath);

		switch (treeView -> get_data_type (iter))
		{
			case OutlineIterType::X3DBaseNode:
			{
				const auto & sfnode     = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

				if (not sfnode)
					break;

				isCloned            = sfnode -> getCloneCount () > 1;
				isBaseNode          = bool (sfnode);
				isPrototypeInstance = dynamic_cast <X3D::X3DPrototypeInstance*> (sfnode .getValue ());
				isInlineNode        = inlineNode and inlineNode -> checkLoadState () == X3D::COMPLETE_STATE;
				isLocalNode         = sfnode -> getExecutionContext () == getExecutionContext ();
				break;
			}
			case OutlineIterType::ExternProtoDeclaration:
			{
				const auto & sfnode      = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   externProto = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());

				isExternProto = externProto -> checkLoadState () not_eq X3D::FAILED_STATE;
				isLocalNode   = sfnode -> getExecutionContext () == getExecutionContext ();
				break;
			}
			case OutlineIterType::ProtoDeclaration:
			{
				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				isPrototype = bool (sfnode);
				isLocalNode = sfnode -> getExecutionContext () == getExecutionContext ();
				break;
			}
			case OutlineIterType::ImportedNode:
			{
				isLocalNode = false;
				break;
			}
			case OutlineIterType::ExportedNode:
			default:
				break;
		}
	}

	getSetAsCurrentSceneMenuItem () .set_visible (isExternProto or isPrototype or isPrototypeInstance or isInlineNode or not isLocalNode);
	getCreateInstanceMenuItem ()    .set_visible (not inPrototypeInstance () and isLocalNode and (isPrototype or isExternProto));
	getUnlinkCloneMenuItem ()       .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode and isCloned);
	getRemoveMenuItem ()            .set_visible (not inPrototypeInstance () and isLocalNode and isBaseNode);
}

void
OutlineEditor::selectField (const double x, const double y)
{
	fieldPath = getFieldAtPosition (x, y);

	const bool isField       = not fieldPath .empty ();
	bool       hasReferences = false;

	if (inProtoDeclaration () and isField)
	{
		const Gtk::TreeIter iter  = treeView -> get_model () -> get_iter (fieldPath);
		const auto          field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));

		hasReferences = not field -> getIsReferences () .empty ();

		// Create reference menu

		for (const auto & widget : getCreateReferenceMenu () .get_children ())
			getCreateReferenceMenu () .remove (*widget);

		for (const auto & reference : getExecutionContext () -> getFieldDefinitions ())
		{
			if (field -> getType () not_eq reference -> getType ())
				continue;
	
			if (field -> getAccessType () == reference -> getAccessType () or field -> getAccessType () == X3D::inputOutput)
			{
				if (field -> getIsReferences () .count (reference))
					continue;

				try
				{
					const auto menuItem = Gtk::manage (new Gtk::MenuItem (reference -> getName ()));
					menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &OutlineEditor::on_create_reference_activate),
					                                                     X3D::FieldPtr (field),
					                                                     X3D::FieldPtr (reference)));
					menuItem -> show ();

					getCreateReferenceMenu () .append (*menuItem);
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

		for (const auto & reference : field -> getIsReferences ())
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

	getCreateReferenceMenuItem () .set_visible (inProtoDeclaration () and isField and not getCreateReferenceMenu () .get_children () .empty ());
	getRemoveReferenceMenuItem () .set_visible (inProtoDeclaration () and isField and hasReferences);
}

Gtk::TreePath
OutlineEditor::getNodeAtPosition (const double x, const double y)
{
	Gtk::TreePath path = getPathAtPosition (x, y);

	if (path .empty ())
		return path;

	const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (path);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DExecutionContext:
		{
			path .up ();

			const Gtk::TreeIter parent = treeView -> get_model () -> get_iter (path);

			switch (treeView -> get_data_type (parent))
			{
				case OutlineIterType::X3DBaseNode:
				case OutlineIterType::ExternProtoDeclaration:
				case OutlineIterType::ProtoDeclaration:
				case OutlineIterType::ImportedNode:
				case OutlineIterType::ExportedNode:
					break;
				default:
					path .clear ();
					break;
			}

			break;
		}
		case OutlineIterType::Separator:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
			break;
		default:
			path .clear ();
			break;
	}

	return path;
}

Gtk::TreePath
OutlineEditor::getFieldAtPosition (const double x, const double y)
{
	Gtk::TreePath path = getPathAtPosition (x, y);

	if (path .empty ())
		return path;

	const Gtk::TreeIter iter = treeView -> get_model () -> get_iter (path);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DField:
			break;
		default:
			path .clear ();
			break;
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
	if (not executionContext -> isRootContext ())
		return;

	try
	{
		OutlineEditorDatabase database;

		const auto & expanded = database .getItem (executionContext -> getWorldURL ());
		const auto   paths    = basic::split (expanded, ";");

		__LOG__ << expanded << std::endl;

		for (const auto & path : paths)
			treeView -> expand_row (Gtk::TreePath (path), false);
	}
	catch (...)
	{ }
}

void
OutlineEditor::saveExpanded (const X3D::X3DExecutionContextPtr & executionContext)
{
	if (executionContext -> getWorldURL () .empty ())
		return;

	if (not executionContext -> isRootContext ())
		return;

	std::deque <std::string> paths;

	getExpanded (treeView -> get_model () -> children (), paths);

	OutlineEditorDatabase database;

	database .setItem (executionContext -> getWorldURL (), basic::join (paths, ";"));
	
	__LOG__ << basic::join (paths, ";") << std::endl;
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

OutlineEditor::~OutlineEditor ()
{
	if (isInitialized ())
		saveExpanded (treeView -> get_model () -> get_execution_context ());

	dispose ();
}

} // puck
} // titania
