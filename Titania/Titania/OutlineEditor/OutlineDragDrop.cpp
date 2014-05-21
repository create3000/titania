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

#include "../Browser/BrowserWindow.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

namespace titania {
namespace puck {

const std::string OutlineDragDrop::dragDataType = "TITANIA_OUTLINE_TREE_ROW";

OutlineDragDrop::OutlineDragDrop (BrowserWindow* const browserWindow, OutlineTreeViewEditor* const treeView) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
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

bool
OutlineDragDrop::on_button_press_event (GdkEventButton* event)
{
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
		default:
			break;
	}

	return true;
}

bool
OutlineDragDrop::on_drag_motion_extern_proto (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time)
{
	TreeModel::Path      destinationPath;
	TreeViewDropPosition position;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				if (time)
					break;
				// If it is a call from on_drag_data_received procced with next step.
			}
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_BEFORE:
			{
				const auto iter = treeView -> get_model () -> get_iter (destinationPath);

				if (not treeView -> get_model () -> iter_is_valid (iter))
					break;

				if (treeView -> get_data_type (iter) not_eq OutlineIterType::ExternProtoDeclaration)
					break;

				const auto sfnode = *static_cast <X3D::SFNode*> (treeView -> get_object (iter));

				if (sfnode -> getExecutionContext () not_eq treeView -> get_model () -> get_execution_context ())
					break;

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
	__LOG__ << time << std::endl;
	
	if (on_drag_motion (context, x, y, 0))
		return;

	const auto iter = treeView -> get_model () -> get_iter (sourcePath);

	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::ExternProtoDeclaration:
			on_drag_data_extern_proto_received (context, x, y, selection_data, info, time);
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
	TreeModel::Path      destinationPath;
	TreeViewDropPosition position;

	if (treeView -> get_dest_row_at_pos (x, y, destinationPath, position))
	{
		__LOG__ << destinationPath .to_string () << std::endl;

		// Get source extern proto.

		const auto sourceIter = treeView -> get_model () -> get_iter (sourcePath);
		const auto sourceNode = *static_cast <X3D::SFNode*> (treeView -> get_object (sourceIter));

		const X3D::ExternProtoPtr sourceExternProto (sourceNode);

		// Get destination extern proto.

		const auto destIter = treeView -> get_model () -> get_iter (destinationPath);
		const auto destNode = *static_cast <X3D::SFNode*> (treeView -> get_object (destIter));

		const X3D::ExternProtoPtr destExternProto (destNode);

		// Get destination extern protos.
		
		const auto executionContext = destExternProto -> getExecutionContext ();

		std::vector <X3D::ExternProtoPtr> destExternProtos (executionContext -> getExternProtoDeclarations () .begin (),
		                                                    executionContext -> getExternProtoDeclarations () .end ());

		// Insert source extern proto in destination extern protos.

		{
			const auto eraseIter = std::find (destExternProtos .begin (), destExternProtos .end (), sourceExternProto);
			
			if (eraseIter == destExternProtos .end ())
				return;

			auto insertIter = std::find (destExternProtos .begin (), destExternProtos .end (), destExternProto);
			
			if (insertIter == destExternProtos .end ())
				return;

			switch (position)
			{
				case Gtk::TREE_VIEW_DROP_AFTER:
				case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				{
					++ insertIter;
					// Procced with next case.
				}
				case Gtk::TREE_VIEW_DROP_BEFORE:
				case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
				{
					const auto eraseIndex  = eraseIter - destExternProtos .begin ();
					const auto insertIndex = insertIter - destExternProtos .begin ();

					if (eraseIndex < insertIndex)
					{
						destExternProtos .insert (destExternProtos .begin () + insertIndex, sourceExternProto);
						destExternProtos .erase (destExternProtos .begin () + eraseIndex);
					}
					else
					{
						destExternProtos .erase (destExternProtos .begin () + eraseIndex);
						destExternProtos .insert (destExternProtos .begin () + insertIndex, sourceExternProto);
					}

					// Reorder extern protos.

					const auto undoStep = std::make_shared <UndoStep> ("Reorder Extern Prototypes");

					// Remove extern protos.
					
					const auto currentExternProtos = executionContext -> getExternProtoDeclarations ();

					for (const auto & externProto : basic::reverse_adapter (currentExternProtos))
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
