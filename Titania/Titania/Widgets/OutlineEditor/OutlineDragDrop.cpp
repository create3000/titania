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

#include "OutlineDragDrop.h"

#include "OutlineEditor.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Widgets/Sidebar/Sidebar.h"

#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Parser/Filter.h>
#include <Titania/X3D/Parser/Parser.h>

#include <Titania/String.h>

namespace titania {
namespace puck {

/*
 * Enable drag & drop for a row in the model.
 */

OutlineDragDrop::OutlineDragDrop (OutlineEditor* const outlineEditor, OutlineTreeViewEditor* const treeView) :
	outlineEditor (outlineEditor),
	     treeView (treeView),
	   sourcePath (),
	     sourceId (0),
	   sourceType (OutlineIterType::NULL_),
	       nodeId (0),
	  protoLimits (),
	      pointer ()
{
	// Drag & Drop
	treeView -> set_reorderable (true);

	treeView -> enable_model_drag_source ({ Gtk::TargetEntry ("titania/externproto-id", Gtk::TARGET_SAME_WIDGET),
	                                        Gtk::TargetEntry ("titania/node-id", Gtk::TARGET_SAME_APP) },
	                                        Gdk::BUTTON1_MASK, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK | Gdk::ACTION_ASK);

	treeView -> enable_model_drag_dest ({ Gtk::TargetEntry ("titania/externproto-id", Gtk::TARGET_SAME_WIDGET),
	                                      Gtk::TargetEntry ("titania/node-id", Gtk::TARGET_SAME_WIDGET) },
	                                      Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK | Gdk::ACTION_ASK);

	treeView -> signal_button_press_event () .connect (sigc::mem_fun (this, &OutlineDragDrop::on_button_press_event), false);
	treeView -> signal_drag_data_get ()      .connect (sigc::mem_fun (this, &OutlineDragDrop::on_drag_data_get));
	treeView -> signal_drag_motion ()        .connect (sigc::mem_fun (this, &OutlineDragDrop::on_drag_motion), false);
	treeView -> signal_drag_data_received () .connect (sigc::mem_fun (this, &OutlineDragDrop::on_drag_data_received));

	outlineEditor -> getDragMoveMenuItem ()   .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineDragDrop::on_drag_action_activate), Gdk::ACTION_MOVE));
	outlineEditor -> getDragCopyMenuItem ()   .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineDragDrop::on_drag_action_activate), Gdk::ACTION_COPY));
	outlineEditor -> getDragLinkMenuItem ()   .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineDragDrop::on_drag_action_activate), Gdk::ACTION_LINK));
	outlineEditor -> getDragCancelMenuItem () .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &OutlineDragDrop::on_drag_action_activate), Gdk::ACTION_PRIVATE));
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
		sourceId   = treeView -> get_execution_context () -> getId ();
		sourceType = OutlineIterType::NULL_;
		nodeId     = 0;

		const auto iter = treeView -> get_model () -> get_iter (sourcePath);

		if (treeView -> get_model () -> iter_is_valid (iter))
		{
			sourceType = treeView -> get_data_type (iter);

			switch (sourceType)
			{
				case OutlineIterType::ProtoDeclaration:
				{
					const auto sfnode = static_cast <X3D::SFNode*> (treeView -> get_object (iter));

					protoLimits = get_proto_limits (sourcePath, dynamic_cast <X3D::ProtoDeclaration*> (sfnode -> getValue ()));

					[[fallthrough]];
				}
				case OutlineIterType::ExternProtoDeclaration:
				case OutlineIterType::X3DBaseNode:
				{
					nodeId = static_cast <X3D::SFNode*> (treeView -> get_object (iter)) -> getValue () -> getId ();
					break;
				}
				case OutlineIterType::NULL_:
				default:
					break;
			}
		}
	}

	return false;
}

std::pair <Gtk::TreePath, Gtk::TreePath>
OutlineDragDrop::get_proto_limits (const Gtk::TreePath & path, X3D::ProtoDeclaration* const prototype) const
{
	const auto executionContext = prototype -> getExecutionContext ();

	std::vector <X3D::ProtoDeclarationPtr> prototypes (executionContext -> getProtoDeclarations () .begin (),
	                                                   executionContext -> getProtoDeclarations () .end ());

	const auto first  = get_proto_first_limit  (path, prototype, prototypes);
	const auto second = get_proto_second_limit (path, prototype, prototypes);

	return std::make_pair (first, second);
}

Gtk::TreePath
OutlineDragDrop::get_proto_first_limit (Gtk::TreePath path,
                                        X3D::ProtoDeclaration* const prototype,
                                        const std::vector <X3D::ProtoDeclarationPtr> & prototypes) const
{
	auto iter = std::find (prototypes .rbegin (), prototypes .rend (), prototype);

	if (iter == prototypes .rend ())
		return path;

	++ iter;

	while (iter not_eq prototypes .rend ())
	{
		path .prev ();

		if (X3D::X3DEditor::isProtoUsedInProto (*iter, prototype))
		{
			path .next ();
			break;
		}
		
		++ iter;
	}

	return path;
}

Gtk::TreePath
OutlineDragDrop::get_proto_second_limit (Gtk::TreePath path,
                                         X3D::ProtoDeclaration* const prototype,
                                         const std::vector <X3D::ProtoDeclarationPtr> & prototypes) const
{
	auto iter = std::find (prototypes .begin (), prototypes .end (), prototype);

	if (iter == prototypes .end ())
		return path;

	++ iter;

	while (iter not_eq prototypes .end ())
	{
		path .next ();

		if (X3D::X3DEditor::isProtoUsedInProto (prototype, *iter))
		{
			path .prev ();
			break;
		}

		++ iter;
	}

	return path;
}

void
OutlineDragDrop::on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context,
                                   Gtk::SelectionData & selection_data,
                                   guint info,
                                   guint time)
{
	switch (sourceType)
	{
		case OutlineIterType::X3DBaseNode:
			selection_data .set ("titania/node-id", basic::to_string (nodeId, std::locale::classic ()));
			break;
		case OutlineIterType::NULL_:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ExternProtoDeclaration:
		default:
			break;
	}
}

bool
OutlineDragDrop::on_drag_motion (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	// Returns false to allow drop and true to reject drop.

	if (time)
		context -> drag_status (Gdk::ACTION_MOVE, time);

	if (treeView -> get_execution_context () -> getId () not_eq sourceId)
		return true;

	const auto iter = treeView -> get_model () -> get_iter (sourcePath);

	if (not treeView -> get_model () -> iter_is_valid (iter))
		return true;

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ProtoDeclaration:
			return not on_drag_motion_proto (context, x, y, time, OutlineIterType::ProtoDeclaration);
		case OutlineIterType::ExternProtoDeclaration:
			return not on_drag_motion_proto (context, x, y, time, OutlineIterType::ExternProtoDeclaration);
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			return not on_drag_motion_base_node (context, x, y, time);
		default:
			break;
	}

	// Reject
	return true;
}

bool
OutlineDragDrop::on_drag_motion_proto (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time, const OutlineIterType type)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	//__LOG__ << "on_drag_motion_extern_proto" << std::endl;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		if (sourcePath == destinationPath)
			return false;

		const auto   sourceIter    = treeView -> get_model () -> get_iter (sourcePath);
		const auto & sourceNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceIter));
		const auto   sourceContext = sourceNode ? sourceNode -> getExecutionContext () : treeView -> get_execution_context ();

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				if (time)
					return false;
				// If it is a call from on_drag_data_received procced with next step.
			}
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_BEFORE:
			{
				const auto iter = treeView -> get_model () -> get_iter (destinationPath);

				if (not treeView -> get_model () -> iter_is_valid (iter))
					return false;

				if (treeView -> get_data_type (iter) == OutlineIterType::ProtoDeclaration)
				{
					if (destinationPath < protoLimits .first)
						break;

					if (destinationPath > protoLimits .second)
						break;
				}

				if (treeView -> get_data_type (iter) not_eq type)
					break;

				const auto & destNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   destContext = destNode -> getExecutionContext ();

				if (destContext not_eq sourceContext)
					return false;

				if (destContext -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return false;

				return true;
			}
		}
	}

	// Reject
	return false;
}

bool
OutlineDragDrop::on_drag_motion_base_node (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		if (sourcePath == destinationPath)
			return false;

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

				const auto   destfield   = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
				const auto & destNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (parentIter));
				const auto   destContext = destNode -> getExecutionContext ();

				// Field must be SFNode or MFNode

				if (destfield -> getType () not_eq X3D::X3DConstants::SFNode and destfield -> getType () not_eq X3D::X3DConstants::MFNode)
					return false;

				// In scene drag n drop

				if (destNode -> getType () .back () == X3D::X3DConstants::ImportedNode)
					return false;

				if (destContext -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return false;

				return true;
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
				// Test if destination is node or NULL.

				const auto iter = treeView -> get_model () -> get_iter (destinationPath);

				switch (treeView -> get_data_type (iter))
				{
					case OutlineIterType::NULL_:
						return false;
					case OutlineIterType::X3DBaseNode:
						break;
					default:
						return false;
				}

				const auto & destNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));
				const auto   destContext = destNode -> getExecutionContext ();

				if (destContext -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return false;

				// If destination is root node, success.

				if (destinationPath .size () == 1)
				   return true;

				// Test the parent is a X3DExecutionContext.

				const auto parentIter = iter -> parent ();

				if (treeView -> get_data_type (parentIter) == OutlineIterType::X3DExecutionContext)
					return true;

				// If the parent of the node is not a X3DField reject.

				if (treeView -> get_data_type (parentIter) not_eq OutlineIterType::X3DField)
					return false;

				// Test if the parent field is a SFNode or MFNode.

				const auto destField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (parentIter));

				if (not (destField -> getType () == X3D::X3DConstants::SFNode or destField -> getType () == X3D::X3DConstants::MFNode))
					return false;

				// Test if the parent of the field is a X3DBaseNode.

				const auto parentParentIter = parentIter -> parent ();

				if (treeView -> get_data_type (parentParentIter) not_eq OutlineIterType::X3DBaseNode)
					return false;

				return true;
			}
		}
	}

	// Reject
	return false;
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
		case OutlineIterType::ProtoDeclaration:
		{
			on_drag_data_proto_received (context, x, y, selection_data, info, time);
			return;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			on_drag_data_extern_proto_received (context, x, y, selection_data, info, time);
			return;
		}
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
		{
			const auto action = context -> get_selected_action ();

			if (action == Gdk::ACTION_ASK)
			{
				pointer = X3D::Vector2i (x, y);

				outlineEditor -> getDragActionMenu () .popup (x, y);
				return;
			}

			on_drag_data_base_node_received (action, x, y);
			return;
		}
		default:
			break;
	}
}

void
OutlineDragDrop::on_drag_data_proto_received (const Glib::RefPtr <Gdk::DragContext> & context,
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

	const auto   sourceIter    = treeView -> get_model () -> get_iter (sourcePath);
	const auto & sourceNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceIter));
	const auto   sourceProto   = X3D::ProtoDeclarationPtr (sourceNode);
	const auto   sourceContext = sourceProto -> getExecutionContext ();

	// Get destination extern proto.

	const auto   destIter  = treeView -> get_model () -> get_iter (destinationPath);
	const auto & destNode  = *static_cast <X3D::SFNode*> (treeView -> get_object (destIter));
	const auto   destProto = X3D::ProtoDeclarationPtr (destNode);

	// Get destination extern protos.
	

	std::vector <X3D::ProtoDeclarationPtr> destProtos (sourceContext -> getProtoDeclarations () .begin (),
	                                                   sourceContext -> getProtoDeclarations () .end ());

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
				[[fallthrough]];
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
				if (eraseIndex < insertIndex)
				{
					destProtos .emplace (destProtos .begin () + insertIndex, sourceProto);
					destProtos .erase (destProtos .begin () + eraseIndex);
				}
				else
				{
					destProtos .erase (destProtos .begin () + eraseIndex);
					destProtos .emplace (destProtos .begin () + insertIndex, sourceProto);
				}

				// Reorder extern protos.

				const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Move Prototype »%s«"), sourceProto -> getName () .c_str ()));

				// Remove extern protos.
				
				const auto currentProtos = sourceContext -> getProtoDeclarations ();

				for (const auto & externProto : basic::make_reverse_range (currentProtos))
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateProtoDeclaration, sourceContext, externProto -> getName (), externProto);
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeProtoDeclaration, sourceContext, externProto -> getName ());

					sourceContext -> removeProtoDeclaration (externProto -> getName ());
				}

				// Add extern protos.

				for (const auto & externProto : destProtos)
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeProtoDeclaration, sourceContext, externProto -> getName ());
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateProtoDeclaration, sourceContext, externProto -> getName (), externProto);

					sourceContext -> updateProtoDeclaration (externProto -> getName (), externProto);
				}

				getBrowserWindow () -> addUndoStep (undoStep);
				break;
			}
		}
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

	const auto   sourceIter        = treeView -> get_model () -> get_iter (sourcePath);
	const auto & sourceNode        = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceIter));
	const auto   sourceExternProto = X3D::ExternProtoDeclarationPtr (sourceNode);
	const auto   sourceContext     = sourceExternProto -> getExecutionContext ();

	// Get destination extern proto.

	const auto   destIter        = treeView -> get_model () -> get_iter (destinationPath);
	const auto & destNode        = *static_cast <X3D::SFNode*> (treeView -> get_object (destIter));
	const auto   destExternProto = X3D::ExternProtoDeclarationPtr (destNode);

	// Get destination extern protos.
	

	std::vector <X3D::ExternProtoDeclarationPtr> destExternProtos (sourceContext -> getExternProtoDeclarations () .begin (),
	                                                               sourceContext -> getExternProtoDeclarations () .end ());

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
				[[fallthrough]];
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

				const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Move Extern Prototype »%s«"), sourceExternProto -> getName () .c_str ()));

				// Remove extern protos.
				
				const auto currentExternProtos = sourceContext -> getExternProtoDeclarations ();

				for (const auto & externProto : basic::make_reverse_range (currentExternProtos))
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration, sourceContext, externProto -> getName (), externProto);
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, sourceContext, externProto -> getName ());

					sourceContext -> removeExternProtoDeclaration (externProto -> getName ());
				}

				// Add extern protos.

				for (const auto & externProto : destExternProtos)
				{
					undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeExternProtoDeclaration, sourceContext, externProto -> getName ());
					undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateExternProtoDeclaration, sourceContext, externProto -> getName (), externProto);

					sourceContext -> updateExternProtoDeclaration (externProto -> getName (), externProto);
				}

				getBrowserWindow () -> addUndoStep (undoStep);
				break;
			}
		}
	}
}

void
OutlineDragDrop::on_drag_action_activate (const Gdk::DragAction & action)
{
	if (action == Gdk::ACTION_PRIVATE)
		return;

	on_drag_data_base_node_received (action, pointer .x (), pointer .y ());
}

void
OutlineDragDrop::on_drag_data_base_node_received (const Gdk::DragAction action, int x, int y)
{
	if (on_drag_data_base_node_insert_into_node_received (action, x, y))
		return;

	if (on_drag_data_base_node_insert_into_array_received (action, x, y))
		return;

	if (on_drag_data_base_node_on_field_received (action, x, y))
		return;
}

bool
OutlineDragDrop::on_drag_data_base_node_insert_into_node_received (const Gdk::DragAction action, int x, int y)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return false;

	switch (position)
	{
		case Gtk::TREE_VIEW_DROP_AFTER:
		case Gtk::TREE_VIEW_DROP_BEFORE:
		   return false;
		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			break;
	}

	// Get destination node.

	const auto destNodeIter = treeView -> get_model () -> get_iter (destinationPath);
				
	if (treeView -> get_data_type (destNodeIter) not_eq OutlineIterType::X3DBaseNode)
		return false;
	
	const auto & destNode        = *static_cast <X3D::SFNode*> (treeView -> get_object (destNodeIter));
	const auto   destContext     = X3D::X3DExecutionContextPtr (destNode -> getExecutionContext ());
	auto         destContextNode = X3D::SFNode (destContext);

	// Get source node.

	const auto  sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto &      sourceNode     = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));
	auto        sourceContext  = X3D::X3DExecutionContextPtr (sourceNode -> getExecutionContext ());

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &sourceContext -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return true;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);

		if (treeView -> get_data_type (sourceFieldIter) == OutlineIterType::X3DField)
			sourceField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	auto sourceContextNode = X3D::SFNode (sourceContext);
	auto sourceParent      = &sourceContextNode;

	if (sourceField not_eq &sourceContext -> getRootNodes ())
	{
		if (sourcePath .size () > 1)
		{
			if (not sourcePath .up ())
				return true;
	
			const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
	
			sourceParent = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
		}
	}

	// Handle copy

	const auto foreignSourceContext = sourceContext;
	const auto foreignSourceParent  = sourceParent;
	const auto foreignSourceField   = sourceField;
	const auto foreignSourceIndex   = sourceIndex;

	X3D::MFNode copyField;

	if (action == Gdk::ACTION_COPY or sourceContext not_eq destContext)
	{
		// Copy source node into scene.

		const auto exportedNode = sourceNode;

		copyField = X3D::X3DEditor::deepCopyNodes (sourceContext, destContext, { sourceNode }, std::make_shared <X3D::UndoStep> ());

		// Change source values.

		sourceParent = &destContextNode;
		sourceNode   = copyField .back ();
		sourceIndex  = copyField .size () - 1;
		sourceField  = &copyField;

		// Adjust transformation like detach from group of copy.

		const auto transform = X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> (sourceNode);

		if (transform)
		{
			auto modelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, exportedNode);

			modelMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DPtr <X3D::X3DTransformNode> sourceTransfrom (sourceNode);

			if (sourceTransfrom)
				sourceTransfrom -> setMatrix (modelMatrix);
		}
	}

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string (action, sourceNode)));

	// Handle link

	if (action == Gdk::ACTION_LINK)
	{
		if (sourceContext not_eq destContext)
			return true;
	}
	else
	{
		try
		{
			// Handle X3DTransformNode nodes.

			const X3D::X3DPtr <X3D::X3DTransformNode> childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				auto       groupModelMatrix = X3D::X3DEditor::getModelMatrix (destContext, destNode);
				const auto groupTransform   = X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> (destNode);

				if (groupTransform)
					groupModelMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				auto childModelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, sourceNode);

				childModelMatrix .mult_left (childTransform -> getMatrix ());
				childModelMatrix .mult_right (inverse (groupModelMatrix));

				X3D::X3DEditor::setMatrix (childTransform, childModelMatrix, undoStep);
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

		const auto containerField = X3D::X3DEditor::getContainerField (destNode, sourceNode); // throw

		// Add child to group

		const auto sfnode = dynamic_cast <X3D::SFNode*> (containerField);

		if (sfnode)
		{
			X3D::X3DEditor::replaceNode (destContext, destNode, *sfnode, sourceNode, undoStep);
		}
		else
		{
			const auto mfnode = dynamic_cast <X3D::MFNode*> (containerField);

			if (mfnode)
				X3D::X3DEditor::pushBackIntoArray (destNode, *mfnode, sourceNode, undoStep);

			// else shouldn't happen.
		}

		// Remove source node if action was move.

		switch (action)
		{
			case Gdk::ACTION_DEFAULT:
			case Gdk::ACTION_MOVE:
			{
				if (sourceContext not_eq destContext)
					remove_source_node (foreignSourceContext, foreignSourceParent, foreignSourceField, foreignSourceIndex, undoStep);
				else
					remove_source_node (sourceContext, sourceParent, sourceField, sourceIndex, undoStep);
	
				break;
			}
			case Gdk::ACTION_COPY:
			case Gdk::ACTION_LINK:
			case Gdk::ACTION_ASK:
			case Gdk::ACTION_PRIVATE:
				break;
		}

		// Proto support
	
		X3D::X3DEditor::requestUpdateInstances (sourceNode, undoStep);
		X3D::X3DEditor::requestUpdateInstances (destNode,   undoStep);
	
		// Add undo step.

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{
		__LOG__ << "No container field found." << std::endl;

		undoStep -> undo ();
	}

	return true;
}

bool
OutlineDragDrop::on_drag_data_base_node_insert_into_array_received (const Gdk::DragAction action, int x, int y)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return false;

//	switch (position)
//	{
//		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
//		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
//			return false;
//		default:
//			break;
//	}

	// Get destination field.

	const auto destNodeIter = treeView -> get_model () -> get_iter (destinationPath);

	switch (treeView -> get_data_type (destNodeIter))
	{
		case OutlineIterType::NULL_:
		case OutlineIterType::X3DBaseNode:
			break;
		default:
		   return false;
	}

	// Get source node.

	const auto sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto       sourceNode     = X3D::SFNode ();
	auto       sourceContext  = treeView -> get_execution_context ();

	if (treeView -> get_data_type (sourceNodeIter) == OutlineIterType::X3DBaseNode)
	{
		sourceNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));
		sourceContext = X3D::X3DExecutionContextPtr (sourceNode -> getExecutionContext ());
	}

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &sourceContext -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return true;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);

		if (treeView -> get_data_type (sourceFieldIter) == OutlineIterType::X3DField)
			sourceField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	X3D::SFNode sourceContextNode (sourceContext);
	X3D::SFNode* sourceParent = &sourceContextNode;

	if (sourceField not_eq &sourceContext -> getRootNodes ())
	{
		if (sourcePath .size () > 1)
		{
			if (not sourcePath .up ())
				return true;
	
			const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
	
			sourceParent = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
		}
	}

	// Determine destContext, destField and destNode.

	auto destContext     = treeView -> get_execution_context ();
	auto destContextNode = X3D::SFNode (destContext);
	auto destField       = static_cast <X3D::X3DFieldDefinition*> (&destContext -> getRootNodes ());
	auto destNode        = &destContextNode;

	if (destinationPath .size () > 1)
	{
		destinationPath .up ();

		const auto destFieldIter = treeView -> get_model () -> get_iter (destinationPath);

		if (treeView -> get_data_type (destFieldIter) == OutlineIterType::X3DField)
		{
			destField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (destFieldIter));

			if (destField -> getType () not_eq X3D::X3DConstants::MFNode)
			   return true;

			destinationPath .up ();

			const auto destParentIter = treeView -> get_model () -> get_iter (destinationPath);

			destNode    = static_cast <X3D::SFNode*> (treeView -> get_object (destParentIter));
			destContext = destNode -> getValue () -> getExecutionContext ();
		}
		else if (treeView -> get_data_type (destFieldIter) == OutlineIterType::X3DExecutionContext)
		{
			const auto sfnode = static_cast <X3D::SFNode*> (treeView -> get_object (destFieldIter));

			destContext = *sfnode;
			destField   = &destContext -> getRootNodes ();
			destNode    = sfnode;
		}
	}

	// Handle copy

	const auto foreignSourceContext = sourceContext;
	const auto foreignSourceParent  = sourceParent;
	const auto foreignSourceField   = sourceField;
	const auto foreignSourceIndex   = sourceIndex;

	X3D::MFNode copyField;

	if (action == Gdk::ACTION_COPY or sourceContext not_eq destContext)
	{
		// Copy source node into scene.

		const auto exportedNode = sourceNode;

		copyField = X3D::X3DEditor::deepCopyNodes (sourceContext, destContext, { sourceNode }, std::make_shared <X3D::UndoStep> ());

		// Change source values.

		sourceParent = &destContextNode;
		sourceNode   = copyField .back ();
		sourceIndex  = copyField .size () - 1;
		sourceField  = &copyField;

		// Adjust transformation like detach from group of copy.

		const X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> transform (exportedNode);

		if (transform)
		{
			auto modelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, exportedNode);

			modelMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DPtr <X3D::X3DTransformNode> sourceTransfrom (sourceNode);

			if (sourceTransfrom)
				sourceTransfrom -> setMatrix (modelMatrix);
		}
	}

	if ((treeView -> get_expanded (destNodeIter) == OutlineExpanded::CHANGED || treeView -> get_expanded (destNodeIter) == OutlineExpanded::FULL) and
	    position == Gtk::TREE_VIEW_DROP_AFTER and
	    treeView -> get_data_type (destNodeIter) not_eq OutlineIterType::NULL_)
	   return true;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string (action, sourceNode)));

	// Handle link

	if (action == Gdk::ACTION_LINK)
	{
		if (sourceContext not_eq destContext)
			return true;
	}
	else
	{
		try
		{
			// Handle X3DTransformNode nodes.

			const X3D::X3DPtr <X3D::X3DTransformNode> childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				auto       groupModelMatrix = X3D::X3DEditor::getModelMatrix (destContext, *destNode);
				const auto groupTransform   = X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> (*destNode);

				if (groupTransform)
					groupModelMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				auto childModelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, sourceNode);

				childModelMatrix .mult_left (childTransform -> getMatrix ());
				childModelMatrix .mult_right (inverse (groupModelMatrix));

				X3D::X3DEditor::setMatrix (childTransform, childModelMatrix, undoStep);
			}
		}
		catch (const std::domain_error & error)
		{ }
	}

	// Insert

	if (destField == sourceField)
	{
	   //__LOG__ << "destField == sourceField" << std::endl;

		auto insertIndex = treeView -> get_index (destNodeIter);

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				++ insertIndex;
				[[fallthrough]];
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
				auto & mfnode = *static_cast <X3D::MFNode*> (destField);

				if (sourceIndex >= insertIndex)
					++ sourceIndex;

				X3D::X3DEditor::insertIntoArray (*destNode, mfnode, insertIndex, sourceNode, undoStep);
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
				[[fallthrough]];
			}
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			{
			   auto & mfnode = *static_cast <X3D::MFNode*> (destField);
				
				X3D::X3DEditor::insertIntoArray (*destNode, mfnode, insertIndex, sourceNode, undoStep);
			   break;
			}
		}
	}

	// Remove source node if action was move.

	switch (action)
	{
		case Gdk::ACTION_DEFAULT:
		case Gdk::ACTION_MOVE:
		{
			if (sourceContext not_eq destContext)
				remove_source_node (foreignSourceContext, foreignSourceParent, foreignSourceField, foreignSourceIndex, undoStep);
			else
				remove_source_node (sourceContext, sourceParent, sourceField, sourceIndex, undoStep);

			break;
		}
		case Gdk::ACTION_COPY:
		case Gdk::ACTION_LINK:
		case Gdk::ACTION_ASK:
		case Gdk::ACTION_PRIVATE:
			break;
	}

	// Proto support

	X3D::X3DEditor::requestUpdateInstances (sourceNode, undoStep);
	X3D::X3DEditor::requestUpdateInstances (*destNode,  undoStep);

	// Add undo step.

	getBrowserWindow () -> addUndoStep (undoStep);

	return true;
}

bool
OutlineDragDrop::on_drag_data_base_node_on_field_received (const Gdk::DragAction action, int x, int y)
{
	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (not treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	   return false;
	
//	switch (position)
//	{
//		case Gtk::TREE_VIEW_DROP_AFTER:
//		case Gtk::TREE_VIEW_DROP_BEFORE:
//		   return false;
//		case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
//		case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
//			break;
//	}

	//__LOG__ << destinationPath .to_string () << std::endl;

	// Get destination field.

	const auto destFieldIter = treeView -> get_model () -> get_iter (destinationPath);

	if (treeView -> get_data_type (destFieldIter) not_eq OutlineIterType::X3DField)
	   return false;

	const auto destField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (destFieldIter));

	if (destField -> getType () not_eq X3D::X3DConstants::SFNode and destField -> getType () not_eq X3D::X3DConstants::MFNode)
		return false;

	// Get destination node.

	if (not destinationPath .up ())
		return false;
	
	const auto   destNodeIter    = treeView -> get_model () -> get_iter (destinationPath);
	const auto & destNode        = *static_cast <X3D::SFNode*> (treeView -> get_object (destNodeIter));
	const auto   destContext     = X3D::X3DExecutionContextPtr (destNode -> getExecutionContext ());
	auto         destContextNode = X3D::SFNode (destContext);

	// Get source node.

	const auto sourceNodeIter = treeView -> get_model () -> get_iter (sourcePath);
	auto       sourceNode     = X3D::SFNode ();
	auto       sourceContext  = treeView -> get_execution_context ();

	if (treeView -> get_data_type (sourceNodeIter) == OutlineIterType::X3DBaseNode)
	{
		sourceNode    = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceNodeIter));
		sourceContext = X3D::X3DExecutionContextPtr (sourceNode -> getExecutionContext ());
	}

	// Get source field.

	auto                     sourceIndex = treeView -> get_index (sourceNodeIter);
	X3D::X3DFieldDefinition* sourceField = &sourceContext -> getRootNodes ();

	if (sourcePath .size () > 1)
	{
		if (not sourcePath .up ())
			return true;

		const auto sourceFieldIter = treeView -> get_model () -> get_iter (sourcePath);

		if (treeView -> get_data_type (sourceFieldIter) == OutlineIterType::X3DField)
			sourceField = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (sourceFieldIter));
	}

	// Get source parent node.

	X3D::SFNode sourceContextNode (sourceContext);
	X3D::SFNode* sourceParent = &sourceContextNode;

	if (sourceField not_eq &sourceContext -> getRootNodes ())
	{
		if (sourcePath .size () > 1)
		{
			if (not sourcePath .up ())
				return true;
	
			const auto sourceParentIter = treeView -> get_model () -> get_iter (sourcePath);
	
			sourceParent = static_cast <X3D::SFNode*> (treeView -> get_object (sourceParentIter));
		}
	}

	// Handle copy

	const auto foreignSourceContext = sourceContext;
	const auto foreignSourceParent  = sourceParent;
	const auto foreignSourceField   = sourceField;
	const auto foreignSourceIndex   = sourceIndex;

	X3D::MFNode copyField;

	if (action == Gdk::ACTION_COPY or sourceContext not_eq destContext)
	{
		// Copy source node into scene.

		const auto exportedNode = sourceNode;

		copyField = X3D::X3DEditor::deepCopyNodes (sourceContext, destContext, { sourceNode }, std::make_shared <X3D::UndoStep> ());

		// Change source values.

		sourceParent = &destContextNode;
		sourceNode   = copyField .back ();
		sourceIndex  = copyField .size () - 1;
		sourceField  = &copyField;

		// Adjust transformation like detach from group of copy.

		const X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> transform (exportedNode);

		if (transform)
		{
			auto modelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, exportedNode);

			modelMatrix .mult_left (transform -> getMatrix ());

			X3D::X3DPtr <X3D::X3DTransformNode> sourceTransfrom (sourceNode);

			if (sourceTransfrom)
				sourceTransfrom -> setMatrix (modelMatrix);
		}
	}

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ (get_node_action_string (action, sourceNode)));

	// Handle link

	if (action == Gdk::ACTION_LINK)
	{
		if (sourceContext not_eq destContext)
			return true;
	}
	else
	{
		try
		{
			// Handle X3DTransformNode nodes.

			const X3D::X3DPtr <X3D::X3DTransformNode> childTransform (sourceNode);

			if (childTransform)
			{
				// Get group modelview matrix

				auto       groupModelMatrix = X3D::X3DEditor::getModelMatrix (destContext, destNode);
				const auto groupTransform   = X3D::X3DPtr <X3D::X3DTransformMatrix3DObject> (destNode);

				if (groupTransform)
					groupModelMatrix .mult_left (groupTransform -> getMatrix ());
				
				// Adjust child transformation

				auto childModelMatrix = X3D::X3DEditor::getModelMatrix (sourceContext, sourceNode);

				childModelMatrix .mult_left (childTransform -> getMatrix ());
				childModelMatrix .mult_right (inverse (groupModelMatrix));

				X3D::X3DEditor::setMatrix (childTransform, childModelMatrix, undoStep);
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

			X3D::X3DEditor::replaceNode (destContext, destNode, sfnode, sourceNode, undoStep);
	      break;
	   }
	   case X3D::X3DConstants::MFNode:
	   {
			auto & mfnode = *static_cast <X3D::MFNode*> (destField);
	   
			X3D::X3DEditor::pushBackIntoArray (destNode, mfnode, sourceNode, undoStep);
	      break;
	   }
	   default:
	      break;
	}

	// Remove source node if action was move.

	switch (action)
	{
		case Gdk::ACTION_DEFAULT:
		case Gdk::ACTION_MOVE:
		{
			if (sourceContext not_eq destContext)
				remove_source_node (foreignSourceContext, foreignSourceParent, foreignSourceField, foreignSourceIndex, undoStep);
			else
				remove_source_node (sourceContext, sourceParent, sourceField, sourceIndex, undoStep);

			break;
		}
		case Gdk::ACTION_COPY:
		case Gdk::ACTION_LINK:
		case Gdk::ACTION_ASK:
		case Gdk::ACTION_PRIVATE:
			break;
	}

	// Proto support

	X3D::X3DEditor::requestUpdateInstances (sourceNode, undoStep);
	X3D::X3DEditor::requestUpdateInstances (destNode,   undoStep);

	// Add undo step.

	getBrowserWindow () -> addUndoStep (undoStep);

	return true;
}

void
OutlineDragDrop::remove_source_node (const X3D::X3DExecutionContextPtr & sourceContext,
                                     X3D::SFNode* const sourceParent,
                                     X3D::X3DFieldDefinition* const sourceField,
                                     size_t sourceIndex,
                                     const X3D::UndoStepPtr & undoStep)
{
	switch (sourceField -> getType ())
	{
	   case X3D::X3DConstants::SFNode:
	   {
			auto & sfnode = *static_cast <X3D::SFNode*> (sourceField);

			getBrowserWindow () -> getSelection () -> removeNodes ({ sfnode }, undoStep);
		
			X3D::X3DEditor::replaceNode (sourceContext, *sourceParent, sfnode, nullptr, undoStep);
	      break;
	   }
	   case X3D::X3DConstants::MFNode:
	   {
			auto & mfnode = *static_cast <X3D::MFNode*> (sourceField);

			getBrowserWindow () -> getSelection () -> removeNodes ({ mfnode [sourceIndex] }, undoStep);

			X3D::X3DEditor::removeNode (sourceContext, *sourceParent, mfnode, sourceIndex, undoStep);
			break;
	   }
	   default:
	      break;
	}
}

std::string
OutlineDragDrop::get_node_action_string (Gdk::DragAction action, const X3D::SFNode & node) const
{
	const auto description = X3D::GetDescription (node);

	switch (action)
	{
		case Gdk::ACTION_LINK:
         return "Clone Node " + description;
		case Gdk::ACTION_MOVE:
         return "Move Node " + description;
		case Gdk::ACTION_COPY:
			return "Copy Node " + description;
		case Gdk::ACTION_DEFAULT:
		case Gdk::ACTION_ASK:
		case Gdk::ACTION_PRIVATE:
		   break;
	}

   return "Move Node " + description;
}

} // puck
} // titania
