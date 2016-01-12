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

#include "OutlineDragDrop.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Parser/Parser.h>

namespace titania {
namespace puck {

/*
 * Enable drag & drop for a row in the model.
 */

const std::string OutlineDragDrop::dragDataType = "TITANIA_OUTLINE_TREE_ROW";

OutlineDragDrop::OutlineDragDrop (OutlineTreeViewEditor* const treeView) :
	  treeView (treeView),
	sourcePath ()
{
	// Drag & Drop
	treeView -> set_reorderable (true);

	treeView -> enable_model_drag_source ({ Gtk::TargetEntry (dragDataType, Gtk::TARGET_SAME_WIDGET) },
	                                      Gdk::BUTTON1_MASK, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	treeView -> enable_model_drag_dest ({ Gtk::TargetEntry (dragDataType, Gtk::TARGET_SAME_WIDGET) },
	                                    Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	treeView -> signal_button_press_event () .connect (sigc::mem_fun (*this, &OutlineDragDrop::on_button_press_event), false);
	treeView -> signal_drag_motion ()        .connect (sigc::mem_fun (*this, &OutlineDragDrop::on_drag_motion), false);
	treeView -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &OutlineDragDrop::on_drag_data_received));
}

X3DBrowserWindow*
OutlineDragDrop::getBrowserWindow () const
{
	return treeView -> getBrowserWindow ();
}

bool
OutlineDragDrop::on_button_press_event (GdkEventButton* event)
{
	//__LOG__ << "on_button_press_event" << std::endl;

	if (event -> button == 1)
	{
		Gtk::TreeViewColumn* column = nullptr;
		sourcePath = treeView -> get_path_at_position (event -> x, event -> y, column);
	}

	return false;
}

bool
OutlineDragDrop::on_drag_motion (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	// Returns false to allow drop and true to reject drop.

	if (time)
		context -> drag_status (Gdk::ACTION_MOVE, time);

	const auto iter = treeView -> get_model () -> get_iter (sourcePath);

	if (not treeView -> get_model () -> iter_is_valid (iter))
		return true;

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
			return on_drag_motion_extern_proto (context, x, y, time);
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			return on_drag_motion_base_node (context, x, y, time);
		default:
			break;
	}

	return true;
}

bool
OutlineDragDrop::on_drag_motion_extern_proto (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	//__LOG__ << "on_drag_motion_extern_proto" << std::endl;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				if (time)
					return true;
				// If it is a call from on_drag_data_received procced with next step.
			}
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_BEFORE:
			{
				const auto iter = treeView -> get_model () -> get_iter (destinationPath);

				if (not treeView -> get_model () -> iter_is_valid (iter))
					return true;

				if (treeView -> get_data_type (iter) not_eq OutlineIterType::ExternProtoDeclaration)
					break;

				const auto & sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				if (sfnode -> getExecutionContext () not_eq treeView -> get_execution_context ())
					return true;

				return false;
			}
		}
	}

	return true;
}

bool
OutlineDragDrop::on_drag_motion_base_node (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		if (sourcePath == destinationPath)
			return true;

		// Drag on field

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_BEFORE:
			{
				if (time)
					break;
				// If it is a call from on_drag_data_received procced with next step.
			}
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				if (destinationPath .size () < 2)
					break;

				const auto iter       = treeView -> get_model () -> get_iter (destinationPath);
				const auto parentIter = iter -> parent ();

				if (treeView -> get_data_type (iter) not_eq OutlineIterType::X3DField)
					break;

				const auto   field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
				const auto & node  = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));

				if (field -> getType () not_eq X3D::X3DConstants::SFNode and field -> getType () not_eq X3D::X3DConstants::MFNode)
					return true;

				if (node -> getExecutionContext () not_eq treeView -> get_execution_context ())
					return true;

				if (node -> getType () .back () == X3D::X3DConstants::ImportedNode)
					return true;

				return false;
			}
		}

		// Drag on X3DBaseNode

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_BEFORE:
			{
				const auto iter = treeView -> get_model () -> get_iter (destinationPath);

				switch (treeView -> get_data_type (iter))
				{
					case OutlineIterType::NULL_:
					case OutlineIterType::X3DBaseNode:
						break;
					default:
						return true;
				}

				// Root node
				if (destinationPath .size () == 1)
				   return false;

				const auto parentIter = iter -> parent ();

				if (treeView -> get_data_type (parentIter) not_eq OutlineIterType::X3DField)
					return true;

				const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (parentIter));

				if (not (field -> getType () == X3D::X3DConstants::SFNode or field -> getType () == X3D::X3DConstants::MFNode))
					return true;
				
				const auto parentParentIter = parentIter -> parent ();

				if (treeView -> get_data_type (parentParentIter) not_eq OutlineIterType::X3DBaseNode)
					return true;

				const auto & node = *static_cast <X3D::SFNode*> (treeView -> get_object (parentParentIter));
				
				if (node -> getExecutionContext () not_eq treeView -> get_execution_context ())
					return true;

				return false;
			}
		}
	}

	return true;
}

void
OutlineDragDrop::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                        int x, int y,
                                        const Gtk::SelectionData & selection_data,
                                        guint info,
                                        guint time)
{
	//__LOG__ << "on_drag_data_received " << time << std::endl;
	
	if (on_drag_motion (context, x, y, 0))
		return;

	const auto iter = treeView -> get_model () -> get_iter (sourcePath);

	if (not treeView -> get_model () -> iter_is_valid (iter))
		return;

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
			on_drag_data_extern_proto_received (context, x, y, selection_data, info, time);
			return;
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			on_drag_data_base_node_insert_into_node_received (context, x, y, selection_data, info, time);
			on_drag_data_base_node_on_field_received (context, x, y, selection_data, info, time);
			on_drag_data_base_node_insert_into_array_received (context, x, y, selection_data, info, time);
			return;
		default:
			break;
	}
}

void
OutlineDragDrop::on_drag_data_extern_proto_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                     int x, int y,
                                                     const Gtk::SelectionData & selection_data,
                                                     guint info,
                                                     guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return;

	//__LOG__ << destinationPath .to_string () << std::endl;

	// Get source extern proto.

	const auto   sourceIter = treeView -> get_model () -> get_iter (sourcePath);
	const auto & sourceNode = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceIter));

	const X3D::ExternProtoDeclarationPtr sourceExternProto (sourceNode);

	// Get destination extern proto.

	const auto   destIter = treeView -> get_model () -> get_iter (destinationPath);
	const auto & destNode = *static_cast <X3D::SFNode*> (treeView -> get_object (destIter));

	const X3D::ExternProtoDeclarationPtr destExternProto (destNode);

	// Get destination extern protos.
	
	const auto executionContext = destExternProto -> getExecutionContext ();

	std::vector <X3D::ExternProtoDeclarationPtr> destExternProtos (executionContext -> getExternProtoDeclarations () .begin (),
	                                                               executionContext -> getExternProtoDeclarations () .end ());

	// Insert source extern proto in destination extern protos.

	{
		const auto eraseIndex  = treeView -> get_index (sourceIter);
		auto       insertIndex = treeView -> get_index (destIter);

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				++ insertIndex;
				// Procced with next case.
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
				if (eraseIndex < insertIndex)
				{
					destExternProtos .emplace (destExternProtos .begin () + insertIndex, sourceExternProto);
					destExternProtos .erase (destExternProtos .begin () + eraseIndex);
				}
				else
				{
					destExternProtos .erase (destExternProtos .begin () + eraseIndex);
					destExternProtos .emplace (destExternProtos .begin () + insertIndex, sourceExternProto);
				}

				// Reorder extern protos.

				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Reorder Extern Prototypes"));

				// Remove extern protos.
				
				const auto currentExternProtos = executionContext -> getExternProtoDeclarations ();

				for (const auto & externProto : basic::make_reverse_range (currentExternProtos))
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration, executionContext, externProto -> getName (), externProto);
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, executionContext, externProto -> getName ());

					executionContext -> removeExternProtoDeclaration (externProto -> getName ());
				}

				// Add extern protos.

				for (const auto & externProto : destExternProtos)
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, executionContext, externProto -> getName ());
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration, executionContext, externProto -> getName (), externProto);

					executionContext -> updateExternProtoDeclaration (externProto -> getName (), externProto);
				}

				getBrowserWindow () -> addUndoStep (undoStep);
				break;
			}
		}
	}
}

void
OutlineDragDrop::on_drag_data_base_node_insert_into_node_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                                   int x, int y,
                                                                   const Gtk::SelectionData & selection_data,
                                                                   guint info,
                                                                   guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return;

	switch (position)
	{
		case Gtk::TREE_VIEW_DROP_AFTER:
		case Gtk::TREE_VIEW_DROP_BEFORE:
		   return;
		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			break;
	}

	// Get destination node.

	const auto destNodeIter = treeView -> get_model () -> get_iter (destinationPath);
				
	if (treeView -> get_data_type (destNodeIter) not_eq OutlineIterType::X3DBaseNode)
		return;
	
	const auto & destNode = *static_cast <X3D::SFNode*> (treeView -> get_object (destNodeIter));

	// Get source node.

	const auto  sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto &      sourceNode     = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &treeView -> get_execution_context () -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceField                = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	X3D::SFNode executionContext (treeView -> get_execution_context ());
	X3D::SFNode* sourceParent = &executionContext;

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;

		const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceParent                = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
	}

	// Handle copy

	X3D::SFNode sceneNode;

	if (context -> get_selected_action () == Gdk::ACTION_COPY)
	{
		// Copy source node into scene.

		const auto & currentContext = treeView -> getCurrentContext ();

		std::stringstream sstream;
		X3D::MFNode toExport ({ sourceNode });

		getBrowserWindow () -> exportNodes (currentContext, sstream, toExport);

		basic::ifilestream text (sstream .str ());

		const auto scene         = treeView -> getBrowser () -> createX3DFromStream (currentContext -> getWorldURL (), text);
		const auto undoStep      = std::make_shared <X3D::UndoStep> ("");
		const auto importedNodes = getBrowserWindow () -> importScene (currentContext,
                                                                     X3D::SFNode (currentContext),
                                                                     currentContext -> getRootNodes (),
                                                                     scene,
                                                                     undoStep);

		// Change source values.

		sceneNode = currentContext;

		sourceParent = &sceneNode;
		sourceNode   = currentContext -> getRootNodes () .back ();
		sourceIndex  = currentContext -> getRootNodes () .size () - 1;
		sourceField  = &currentContext -> getRootNodes ();

		// Adjust transformation like detach from group of copy

		const X3D::X3DTransformNodePtr transform (toExport [0]);

		if (transform)
		{
			X3D::Matrix4d modelViewMatrix = getBrowserWindow () -> getModelViewMatrix (currentContext, toExport [0]);

			modelViewMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DTransformNodePtr (sourceNode) -> setMatrix (modelViewMatrix);
		}
	}

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string ()));

	if (context -> get_selected_action () not_eq Gdk::ACTION_LINK)
	{
		try
		{
			// Handle X3DTransformNode nodes.

			const X3D::X3DTransformNodePtr childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				X3D::Matrix4d groupModelViewMatrix (getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), destNode));

				const X3D::X3DTransformMatrix4DNodePtr groupTransform (destNode);

				if (groupTransform)
					groupModelViewMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				X3D::Matrix4d childModelViewMatrix = getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), sourceNode);

				childModelViewMatrix .mult_left (childTransform -> getMatrix ());
				childModelViewMatrix .mult_right (~groupModelViewMatrix);

				getBrowserWindow () -> setMatrix (childTransform, childModelViewMatrix, undoStep);
			}
		}
		catch (const std::domain_error & error)
		{ }
	}
			
	try
	{
		// Insert source node in destination node.

		// Clone save Add to Group

		undoStep -> addObjects (destNode);

		const auto containerField = getBrowserWindow () -> getContainerField (destNode, sourceNode); // throw

		// Add child to group

		const auto sfnode = dynamic_cast <X3D::SFNode*> (containerField);

		if (sfnode)
		{
			getBrowserWindow () -> replaceNode (treeView -> getCurrentContext (), destNode, *sfnode, sourceNode, undoStep);
		}
		else
		{
			const auto mfnode = dynamic_cast <X3D::MFNode*> (containerField);

			if (mfnode)
				getBrowserWindow () -> pushBackIntoArray (destNode, *mfnode, sourceNode, undoStep);

			// else shouldn't happen.
		}

		// Remove
		switch (context -> get_selected_action ())
		{
			case Gdk::ACTION_MOVE:
				remove_source_node (sourceParent, sourceField, sourceIndex, true, undoStep);
				break;
			case Gdk::ACTION_COPY:
				remove_source_node (sourceParent, sourceField, sourceIndex, false, undoStep);
				break;
			default:
				break;
		}

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{
		__LOG__ << "No container field found." << std::endl;
		
		switch (context -> get_selected_action ())
		{
			case Gdk::ACTION_COPY:
				remove_source_node (sourceParent, sourceField, sourceIndex, false, undoStep);
				break;
			default:
				break;
		}

		undoStep -> undo ();
	}
}

void
OutlineDragDrop::on_drag_data_base_node_on_field_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                           int x, int y,
                                                           const Gtk::SelectionData & selection_data,
                                                           guint info,
                                                           guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return;
	
	switch (position)
	{
		case Gtk::TREE_VIEW_DROP_AFTER:
		case Gtk::TREE_VIEW_DROP_BEFORE:
		   return;
		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			break;
	}

	//__LOG__ << destinationPath .to_string () << std::endl;

	// Get destination field.

	const auto destFieldIter = treeView -> get_model () -> get_iter (destinationPath);

	if (treeView -> get_data_type (destFieldIter) not_eq OutlineIterType::X3DField)
	   return;

	const auto destField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (destFieldIter));

	if (destField -> getType () not_eq X3D::X3DConstants::SFNode and destField -> getType () not_eq X3D::X3DConstants::MFNode)
		return;

	// Get destination node.

	if (not destinationPath .up ())
		return;
	
	const auto   destNodeIter = treeView -> get_model () -> get_iter (destinationPath);
	const auto & destNode     = *static_cast <X3D::SFNode*> (treeView -> get_object (destNodeIter));

	// Get source node.

	const auto sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto       sourceNode     = X3D::SFNode ();

	if (treeView -> get_data_type (sourceNodeIter) == OutlineIterType::X3DBaseNode)
		sourceNode = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &treeView -> get_execution_context () -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceField                = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	X3D::SFNode executionContext (treeView -> get_execution_context ());
	X3D::SFNode* sourceParent = &executionContext;

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;;

		const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceParent                = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
	}

	// Handle copy

	X3D::SFNode sceneNode;

	if (context -> get_selected_action () == Gdk::ACTION_COPY)
	{
		// Copy source node into scene.

		const auto & currentContext = treeView -> getCurrentContext ();

		std::stringstream sstream;
		X3D::MFNode toExport ({ sourceNode });

		getBrowserWindow () -> exportNodes (currentContext, sstream, toExport);

		basic::ifilestream text (sstream .str ());

		const auto scene         = treeView -> getBrowser () -> createX3DFromStream (currentContext -> getWorldURL (), text);
		const auto undoStep      = std::make_shared <X3D::UndoStep> ("");
		const auto importedNodes = getBrowserWindow () -> importScene (currentContext,
                                                                     X3D::SFNode (currentContext),
                                                                     currentContext -> getRootNodes (),
                                                                     scene,
                                                                     undoStep);

		// Change source values.

		sceneNode = currentContext;

		sourceParent = &sceneNode;
		sourceNode   = currentContext -> getRootNodes () .back ();
		sourceIndex  = currentContext -> getRootNodes () .size () - 1;
		sourceField  = &currentContext -> getRootNodes ();

		// Adjust transformation like detach from group of copy

		const X3D::X3DTransformNodePtr transform (toExport [0]);

		if (transform)
		{
			X3D::Matrix4d modelViewMatrix = getBrowserWindow () -> getModelViewMatrix (currentContext, toExport [0]);

			modelViewMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DTransformNodePtr (sourceNode) -> setMatrix (modelViewMatrix);
		}
	}

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string ()));

	if (context -> get_selected_action () not_eq Gdk::ACTION_LINK)
	{
		try
		{
			// Handle X3DTransformNode nodes.

			const X3D::X3DTransformNodePtr childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				X3D::Matrix4d groupModelViewMatrix (getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), destNode));

				const X3D::X3DTransformMatrix4DNodePtr groupTransform (destNode);

				if (groupTransform)
					groupModelViewMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				X3D::Matrix4d childModelViewMatrix = getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), sourceNode);

				childModelViewMatrix .mult_left (childTransform -> getMatrix ());
				childModelViewMatrix .mult_right (~groupModelViewMatrix);

				getBrowserWindow () -> setMatrix (childTransform, childModelViewMatrix, undoStep);
			}
		}
		catch (const std::domain_error & error)
		{ }
	}

	// Insert source node in destination field.

	// Add

	switch (destField -> getType ())
	{
	   case X3D::X3DConstants::SFNode:
	   {
			auto & sfnode = *static_cast <X3D::SFNode*> (destField);

			getBrowserWindow () -> replaceNode (treeView -> getCurrentContext (), destNode, sfnode, sourceNode, undoStep);
	      break;
	   }
	   case X3D::X3DConstants::MFNode:
	   {
			auto & mfnode = *static_cast <X3D::MFNode*> (destField);
	   
			getBrowserWindow () -> pushBackIntoArray (destNode, mfnode, sourceNode, undoStep);

	      break;
	   }
	   default:
	      break;
	}

	// Remove
	switch (context -> get_selected_action ())
	{
		case Gdk::ACTION_MOVE:
			remove_source_node (sourceParent, sourceField, sourceIndex, true, undoStep);
			break;
		case Gdk::ACTION_COPY:
			remove_source_node (sourceParent, sourceField, sourceIndex, false, undoStep);
			break;
		default:
			break;
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineDragDrop::on_drag_data_base_node_insert_into_array_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                                    int x, int y,
                                                                    const Gtk::SelectionData & selection_data,
                                                                    guint info,
                                                                    guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return;

	switch (position)
	{
		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			return;
		default:
			break;
	}

	// Get destination field.

	const auto destNodeIter = treeView -> get_model () -> get_iter (destinationPath);

	switch (treeView -> get_data_type (destNodeIter))
	{
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			break;
		default:
		   return;
	}

	// Get source node.

	const auto sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto       sourceNode     = X3D::SFNode ();

	if (treeView -> get_data_type (sourceNodeIter) == OutlineIterType::X3DBaseNode)
		sourceNode = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &treeView -> get_execution_context () -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceField                = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	X3D::SFNode executionContext (treeView -> get_execution_context ());
	X3D::SFNode* sourceParent = &executionContext;

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return;

		const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
		sourceParent                = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
	}

	//

	X3D::X3DFieldDefinition* destField   = &treeView -> get_execution_context () -> getRootNodes ();
	X3D::SFNode*             destParent  = &executionContext;

	if (destinationPath .size () > 1)
	{
		destinationPath .up ();

		const auto destFieldIter = treeView -> get_model () -> get_iter (destinationPath);
		destField                = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (destFieldIter));

		if (destField -> getType () not_eq X3D::X3DConstants::MFNode)
		   return;

		destinationPath .up ();

		const auto destParentIter = treeView -> get_model () -> get_iter (destinationPath);
		destParent                = static_cast <X3D::SFNode*> (treeView -> get_object (destParentIter));
	}

	// Handle copy

	X3D::SFNode sceneNode;

	if (context -> get_selected_action () == Gdk::ACTION_COPY)
	{
		// Copy source node into scene.

		const auto & currentContext = treeView -> getCurrentContext ();

		std::stringstream sstream;
		X3D::MFNode toExport ({ sourceNode });

		getBrowserWindow () -> exportNodes (currentContext, sstream, toExport);

		basic::ifilestream text (sstream .str ());

		const auto scene         = treeView -> getBrowser () -> createX3DFromStream (currentContext -> getWorldURL (), text);
		const auto undoStep      = std::make_shared <X3D::UndoStep> ("");
		const auto importedNodes = getBrowserWindow () -> importScene (currentContext,
                                                                     X3D::SFNode (currentContext),
                                                                     currentContext -> getRootNodes (),
                                                                     scene,
                                                                     undoStep);

		// Change source values.

		sceneNode = currentContext;

		sourceParent = &sceneNode;
		sourceNode   = currentContext -> getRootNodes () .back ();
		sourceIndex  = currentContext -> getRootNodes () .size () - 1;
		sourceField  = &currentContext -> getRootNodes ();

		// Adjust transformation like detach from group of copy

		const X3D::X3DTransformNodePtr transform (toExport [0]);

		if (transform)
		{
			X3D::Matrix4d modelViewMatrix = getBrowserWindow () -> getModelViewMatrix (currentContext, toExport [0]);

			modelViewMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DTransformNodePtr (sourceNode) -> setMatrix (modelViewMatrix);
		}
	}

	if (treeView -> is_expanded (destNodeIter) and position == Gtk::TREE_VIEW_DROP_AFTER and
	    treeView -> get_data_type (destNodeIter) not_eq OutlineIterType::NULL_)
	   return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string ()));

	// Handle X3DTransformNode nodes.

	if (context -> get_selected_action () not_eq Gdk::ACTION_LINK)
	{
		try
		{
			const X3D::X3DTransformNodePtr childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				X3D::Matrix4d groupModelViewMatrix (getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), *destParent));

				const X3D::X3DTransformMatrix4DNodePtr groupTransform (*destParent);

				if (groupTransform)
					groupModelViewMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				X3D::Matrix4d childModelViewMatrix = getBrowserWindow () -> getModelViewMatrix (treeView -> getCurrentContext (), sourceNode);

				childModelViewMatrix .mult_left (childTransform -> getMatrix ());
				childModelViewMatrix .mult_right (~groupModelViewMatrix);

				getBrowserWindow () -> setMatrix (childTransform, childModelViewMatrix, undoStep);
			}
		}
		catch (const std::domain_error & error)
		{ }
	}

	// Insert

	if (destField == sourceField)
	{
	   __LOG__ << "destField == sourceField" << std::endl;
		
		auto insertIndex = treeView -> get_index (destNodeIter);

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				++ insertIndex;
				// Procced with next case.
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
				auto & mfnode = *static_cast <X3D::MFNode*> (destField);

				if (sourceIndex >= insertIndex)
					++ sourceIndex;

				getBrowserWindow () -> insertIntoArray (*destParent, mfnode, insertIndex, sourceNode, undoStep);
				break;
			}
		}
	}
	else
	{
		auto insertIndex = treeView -> get_index (destNodeIter);

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				++ insertIndex;
				// Procced with next case.
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
			   auto & mfnode = *static_cast <X3D::MFNode*> (destField);
				
				getBrowserWindow () -> insertIntoArray (*destParent, mfnode, insertIndex, sourceNode, undoStep);
			   break;
			}
		}
	}

	// Remove

	switch (context -> get_selected_action ())
	{
		case Gdk::ACTION_MOVE:
			remove_source_node (sourceParent, sourceField, sourceIndex, true, undoStep);
			break;
		case Gdk::ACTION_COPY:
			remove_source_node (sourceParent, sourceField, sourceIndex, false, undoStep);
			break;
		default:
			break;
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
OutlineDragDrop::remove_source_node (X3D::SFNode* const sourceParent, X3D::X3DFieldDefinition* const sourceField, size_t sourceIndex, const bool undo, const X3D::UndoStepPtr & undoStep)
{
	switch (sourceField -> getType ())
	{
	   case X3D::X3DConstants::SFNode:
	   {
			auto & sfnode = *static_cast <X3D::SFNode*> (sourceField);

			if (undo)
				getBrowserWindow () -> replaceNode (treeView -> getCurrentContext (), *sourceParent, sfnode, nullptr, undoStep);
			else
				sfnode = nullptr;

	      break;
	   }
	   case X3D::X3DConstants::MFNode:
	   {
			auto & mfnode = *static_cast <X3D::MFNode*> (sourceField);

			if (undo)
				getBrowserWindow () -> eraseFromArray (*sourceParent, mfnode, sourceIndex, undoStep);
			else
			   mfnode .erase (mfnode .begin () + sourceIndex);
			
			break;
	   }
	   default:
	      break;
	}
}

const char*
OutlineDragDrop::get_node_action_string () const
{
   if (getBrowserWindow () -> getKeys () .control ())
   {
      if (getBrowserWindow () -> getKeys () .shift ())
         return "Clone Node";
      
		return "Copy Node";
   }

   return "Move Node";
}

//bool
//OutlineDragDrop::on_drag_motion (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
//{
//	context -> drag_status (Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK, time);
//
//	TreeModel::Path      destinationPath;
//	TreeViewDropPosition position;
//
//	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
//	{
//		switch (position)
//		{
//			case Gtk::TREE_VIEW_DROP_AFTER:
//			case Gtk::TREE_VIEW_DROP_BEFORE:
//				break;
//			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
//			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
//			{
//				const auto iter = treeView -> get_model () -> get_iter (destinationPath);
//
//				switch (treeView -> get_data_type (iter))
//				{
//					case OutlineIterType::X3DField:
//					{
//						const auto field = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
//
//						switch (field -> getType ())
//						{
//							case X3D::X3DConstants::SFNode:
//							case X3D::X3DConstants::MFNode:
//								break;
//							default:
//								// Reject drop.
//								return true;
//						}
//					}
//					case OutlineIterType::X3DBaseNode:
//						break;
//					default:
//						// Reject drop.
//						return true;
//				}
//			}
//		}
//	}
//
//	// Allow drop.
//	return false;
//}

//void
//OutlineDragDrop::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
//                                        int x, int y,
//                                        const Gtk::SelectionData & selection_data,
//                                        guint info,
//                                        guint time)
//{
//	__LOG__ << time << " : " << selection_data .get_data_as_string () << std::endl;
//
//	TreeModel::Path      destinationPath;
//	TreeViewDropPosition position;
//
//	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
//	{
//		switch (context -> get_suggested_action ())
//		{
//			case Gdk::ACTION_COPY:
//			{
//				__LOG__ << "copy" << std::endl;
//				context -> drag_finish (true, true, time);
//				break;
//			}
//			case Gdk::ACTION_MOVE:
//			{
//				__LOG__ << "move" << std::endl;
//				context -> drag_finish (true, true, time);
//				break;
//			}
//			case Gdk::ACTION_LINK:
//			{
//				__LOG__ << "link" << std::endl;
//				context -> drag_finish (true, false, time);
//				break;
//			}
//			default:
//				break;
//		}
//	}
//}

} // puck
} // titania
