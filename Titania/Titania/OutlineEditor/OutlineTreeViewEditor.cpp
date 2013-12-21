/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "OutlineTreeViewEditor.h"

#include "../Browser/BrowserWindow.h"
#include "CellRenderer/OutlineCellRenderer.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeObserver.h"
#include "OutlineSelection.h"
#include "OutlineRouteGraph.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

const std::string OutlineTreeViewEditor::dragDataType = "titania/outline-tree/row";

OutlineTreeViewEditor::OutlineTreeViewEditor (BrowserWindow* const browserWindow, const X3D::X3DSFNode <X3D::X3DExecutionContext> & executionContext) :
	        Glib::ObjectBase (typeid (OutlineTreeViewEditor)),
	        X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	      X3DOutlineTreeView (executionContext),
	               selection (new OutlineSelection (browserWindow, this)),
	            overUserData (new OutlineUserData ()),
	        selectedUserData (new OutlineUserData ()),
	      matchingAccessType (0),
	              sourceNode (),
	             sourceField (),
	         destinationNode (),
	        destinationField (),
	motion_notify_connection (),
	           unwatchMotion (1)
{
	set_name ("OutlineTreeViewEditor");

	watch_motion ();

	get_cellrenderer () -> signal_edited () .connect (sigc::mem_fun (this, &OutlineTreeViewEditor::on_edited));

	// Drag targets
	std::vector <Gtk::TargetEntry> source_targets = {
		Gtk::TargetEntry (dragDataType, Gtk::TARGET_SAME_WIDGET)
	};

	drag_source_set (source_targets, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	// Drop targets
	std::vector <Gtk::TargetEntry> dest_targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list"),
		Gtk::TargetEntry (dragDataType, Gtk::TARGET_SAME_WIDGET)
	};

	drag_dest_set (dest_targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK);

	set_reorderable (true);
}

void
OutlineTreeViewEditor::watch_motion ()
{
	-- unwatchMotion;

	if (not unwatchMotion)	
		motion_notify_connection = signal_motion_notify_event () .connect (sigc::mem_fun (*this, &OutlineTreeViewEditor::set_motion_notify_event), false);
}

void
OutlineTreeViewEditor::unwatch_motion ()
{
	++ unwatchMotion;

	motion_notify_connection .disconnect ();
}

void
OutlineTreeViewEditor::on_drag_begin (const Glib::RefPtr <Gdk::DragContext> & context)
{
	getBrowser () -> endUpdate ();
}

void
OutlineTreeViewEditor::on_drag_end (const Glib::RefPtr <Gdk::DragContext> & context)
{
	getBrowser () -> beginUpdate ();
}

void
OutlineTreeViewEditor::on_drag_data_delete (const Glib::RefPtr <Gdk::DragContext> & context)
{
	__LOG__ << std::endl;
}

void
OutlineTreeViewEditor::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		if (selection_data .get_data_type () == dragDataType)
		{
			TreeModel::Path      path;
			TreeViewDropPosition pos;

			if (get_dest_row_at_pos (x, y, path, pos))
			{
				__LOG__ << path .to_string () << std::endl;
				__LOG__ << selection_data .get_data_as_string () << std::endl;

				if (context -> get_suggested_action () == Gdk::ACTION_MOVE)
				{
					__LOG__ << "move" << std::endl;
					context -> drag_finish (true, true, time);
				}
				else
				{
					__LOG__ << "link" << std::endl;
					context -> drag_finish (true, false, time);
				}
			}
		}
	}

	X3DOutlineTreeView::on_drag_data_received (context, x, y, selection_data, info, time);
	return;

	//	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	//	{
	//		if (selection_data .get_data_type () == DND_OUTLINE_TREE_ROW)
	//		{
	//			__LOG__ << selection_data .get_data_as_string () << std::endl;
	//
	//			context -> drag_finish (true, false, time);
	//			return;
	//		}
	//
	//		if (selection_data .get_data_type () == "text/uri-list")
	//		{
	//			auto uri = selection_data .get_uris ();
	//
	//			if (uri .size ())
	//			{
	//				getBrowserWindow () -> import (Glib::uri_unescape_string (uri [0]));
	//
	//				context -> drag_finish (true, false, time);
	//				return;
	//			}
	//		}
	//
	//		if (selection_data .get_data_type () == "text/plain" or selection_data .get_data_type () == "STRING")
	//		{
	//			getBrowserWindow () -> import (Glib::uri_unescape_string (basic::trim (selection_data .get_data_as_string ())));
	//
	//			context -> drag_finish (true, false, time);
	//			return;
	//		}
	//	}
	//
	//	context -> drag_finish (false, false, time);
}

void
OutlineTreeViewEditor::on_rename_node_activate ()
{
	//__LOG__ << std::endl;

	//
	//	Gtk::TreeModel::Path path;
	//	Gtk::TreeViewColumn* column = nullptr;
	//	int                  cell_x = 0;
	//	int                  cell_y = 0;
	//
	//	get_path_at_pos (x, y, path, column, cell_x, cell_y);
	//
	//	if (path .size ())
	//	{
	//		Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);
	//
	//		if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	//		{
	//			set_cursor (path, *column, true);
	//			return;
	//		}
	//	}
}

void
OutlineTreeViewEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select_node (get_model () -> get_iter (path), path);

	set_cursor (path, *column, true);
}

bool
OutlineTreeViewEditor::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 1:
		{
			if (select_field_value (event -> x, event -> y))
				return true;

			break;
		}
		case 3:
		{
			// getPopupMenu () .popup (event -> button, event -> time);
			// return true;
			break;
		}
		default:
			break;
	}

	return Gtk::TreeView::on_button_press_event (event);
}

bool
OutlineTreeViewEditor::on_button_release_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 1:
		{
			if (select_access_type (event -> x, event -> y))
				return true;

			break;
		}
		default:
			break;
	}

	return Gtk::TreeView::on_button_release_event (event);
}

bool
OutlineTreeViewEditor::set_motion_notify_event (GdkEventMotion* event)
{
	if (hover_access_type (event -> x, event -> y))
		return true;

	return false;
}

void
OutlineTreeViewEditor::select_node (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	if (get_data_type (iter) == OutlineIterType::X3DBaseNode)
	{
		selection -> set_select_multiple (get_shift_key ());

		selection -> select (*static_cast <X3D::SFNode*> (get_object (iter)));
	}
}

bool
OutlineTreeViewEditor::select_field_value (double x, double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (path .size ())
	{
		Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

		if (get_data_type (iter) == OutlineIterType::X3DFieldValue)
		{
			getBrowserWindow () -> hasShortcuts (false);
			get_tree_observer () -> unwatch_tree (iter);
			unwatch_motion ();
			set_cursor (path, *column, true);
			return true;
		}
	}

	return false;
}

void
OutlineTreeViewEditor::on_edited (const Glib::ustring & string_path, const Glib::ustring & text)
{
	Gtk::TreeModel::Path     path (string_path);
	Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

	get_model () -> row_changed (path, iter);
	get_tree_observer () -> watch_child (iter, path);

	getBrowserWindow () -> hasShortcuts (true);
	watch_motion ();
}

bool
OutlineTreeViewEditor::hover_access_type (double x, double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	// Clear over state

	overUserData -> selected &= OUTLINE_SELECTED | OUTLINE_SELECTED_INPUT | OUTLINE_SELECTED_OUTPUT;

	for (const auto & path : overUserData -> paths)
		get_model () -> row_changed (path, get_model () -> get_iter (path));

	// Test for over

	if (path .size ())
	{
		auto iter = get_model () -> get_iter (path);
		auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DField:
			{
				auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

				overUserData = data -> get_user_data ();

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT:
					{
						if (not matchingAccessType or (field -> getAccessType () & (matchingAccessType & X3D::inputOnly) and field -> getType () == matchingFieldType))
						{
							overUserData -> selected |= OUTLINE_OVER_INPUT;
							get_model () -> row_changed (path, iter);
							return true;
						}

						break;
					}
					case OutlineCellContent::OUTPUT:
					{
						if (not matchingAccessType or (field -> getAccessType () & (matchingAccessType & X3D::outputOnly) and field -> getType () == matchingFieldType))
						{
							overUserData -> selected |= OUTLINE_OVER_OUTPUT;
							get_model () -> row_changed (path, iter);
							return true;
						}

						break;
					}
					default:
						break;
				}
			}
			default:
				break;
		}
	}

	return false;
}

bool
OutlineTreeViewEditor::select_access_type (double x, double y)
{
	if (get_control_key ())
		return remove_route (x, y);

	if (add_route (x, y))
		return true;

	return select_route (x, y);
}

bool
OutlineTreeViewEditor::add_route (double x, double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (path .size ())
	{
		auto iter = get_model () -> get_iter (path);
		auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DField:
			{
				auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

				// Pick

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT:
					{
						if (matchingAccessType)
						{
							if (field -> getAccessType () & (matchingAccessType & X3D::inputOnly))
							{
								if (field -> getType () == matchingFieldType)
								{
									path .up ();
									auto nodeIter = get_model () -> get_iter (path);
									auto nodeData = get_model () -> get_data (nodeIter);

									if (nodeData -> get_type () not_eq OutlineIterType::X3DBaseNode)
										return false;

									X3D::SFNode destinationNode  = *static_cast <X3D::SFNode*> (nodeData -> get_object ());
									std::string destinationField = field -> getName ();

									// Add route

									try
									{
										auto undoStep = std::make_shared <UndoStep> (_ ("Add Route"));
										getBrowserWindow () -> saveMatrix (destinationNode, undoStep);
										getBrowserWindow () -> addRoute (getBrowser () -> getExecutionContext (), sourceNode, sourceField, destinationNode, destinationField, undoStep);
										getBrowserWindow () -> addUndoStep (undoStep);
									}
									catch (const X3D::X3DError &)
									{ }

									// Clear selection

									clear_access_type_selection (selectedUserData);
								}
							}

							return true;
						}

						path .up ();
						auto nodeIter = get_model () -> get_iter (path);
						auto nodeData = get_model () -> get_data (nodeIter);

						if (nodeData -> get_type () not_eq OutlineIterType::X3DBaseNode)
							return false;

						destinationNode  = *static_cast <X3D::SFNode*> (nodeData -> get_object ());
						destinationField = field -> getName ();

						set_access_type_selection (data -> get_user_data (), OUTLINE_SELECTED_INPUT);
						matchingFieldType  = field -> getType ();
						matchingAccessType = X3D::outputOnly;

						return true;
					}
					case OutlineCellContent::OUTPUT:
					{
						if (matchingAccessType)
						{
							if (field -> getAccessType () & (matchingAccessType & X3D::outputOnly))
							{
								if (field -> getType () == matchingFieldType)
								{
									path .up ();
									auto nodeIter = get_model () -> get_iter (path);
									auto nodeData = get_model () -> get_data (nodeIter);

									if (nodeData -> get_type () not_eq OutlineIterType::X3DBaseNode)
										return false;

									X3D::SFNode sourceNode  = *static_cast <X3D::SFNode*> (nodeData -> get_object ());
									std::string sourceField = field -> getName ();

									// Add route

									try
									{
										auto undoStep = std::make_shared <UndoStep> (_ ("Add Route"));
										getBrowserWindow () -> saveMatrix (destinationNode, undoStep);
										getBrowserWindow () -> addRoute (getBrowser () -> getExecutionContext (), sourceNode, sourceField, destinationNode, destinationField, undoStep);
										getBrowserWindow () -> addUndoStep (undoStep);
									}
									catch (const X3D::X3DError &)
									{ }

									// Clear selection

									clear_access_type_selection (selectedUserData);
								}
							}

							return true;
						}

						path .up ();
						auto nodeIter = get_model () -> get_iter (path);
						auto nodeData = get_model () -> get_data (nodeIter);

						if (nodeData -> get_type () not_eq OutlineIterType::X3DBaseNode)
							return false;

						sourceNode  = *static_cast <X3D::SFNode*> (nodeData -> get_object ());
						sourceField = field -> getName ();

						set_access_type_selection (data -> get_user_data (), OUTLINE_SELECTED_OUTPUT);
						matchingFieldType  = field -> getType ();
						matchingAccessType = X3D::inputOnly;

						return true;
					}
					default:
					{
						clear_access_type_selection (selectedUserData);
						break;
					}
				}
			}
			default:
				break;
		}
	}

	return false;
}

void
OutlineTreeViewEditor::set_access_type_selection (const OutlineUserDataPtr & userData, int type)
{
	userData -> selected &= OUTLINE_SELECTED;
	userData -> selected |= type;

	selectedUserData = userData;

	for (const auto & path : userData -> paths)
		get_model () -> row_changed (path, get_model () -> get_iter (path));
}

void
OutlineTreeViewEditor::clear_access_type_selection (const OutlineUserDataPtr & userData)
{
	if (not get_shift_key ())
	{
		userData -> selected &= OUTLINE_SELECTED; // clear over state

		for (const auto & path : userData -> paths)
			get_model () -> row_changed (path, get_model () -> get_iter (path));

		matchingAccessType = 0;
		sourceNode         = nullptr;
		destinationNode    = nullptr;
	}
}

bool
OutlineTreeViewEditor::remove_route (double x, double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (path .size ())
	{
		auto iter = get_model () -> get_iter (path);
		auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				auto route = static_cast <X3D::Route*> (data -> get_object ());

				// Pick

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT:
					case OutlineCellContent::OUTPUT:
					{
						try
						{
							auto undoStep = std::make_shared <UndoStep> (_ ("Remove Route"));

							getBrowserWindow () -> deleteRoute (getBrowser () -> getExecutionContext (),
							                                    route -> getSourceNode (),
							                                    route -> getSourceField (),
							                                    route -> getDestinationNode (),
							                                    route -> getDestinationField (),
							                                    undoStep);

							getBrowserWindow () -> addUndoStep (undoStep);
						}
						catch (const X3D::X3DError &)
						{ }

						return true;
					}
					default:
						break;
				}

				return false;
			}
			case OutlineIterType::X3DField:
			{
				auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

				// Pick

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT:
					{
						remove_route (path, get_model () -> get_input_routes (field));

						return true;
					}
					case OutlineCellContent::OUTPUT:
					{
						remove_route (path, get_model () -> get_output_routes (field));

						return true;
					}
					default:
						break;
				}
			}
			default:
				break;
		}
	}

	return false;
}

void
OutlineTreeViewEditor::remove_route (const Gtk::TreeModel::Path & path, const std::vector <X3D::Route*> & routes)
{
	switch (routes .size ())
	{
		case 0:
			return;
		case 1:
		{
			try
			{
				auto route    = routes [0];
				auto undoStep = std::make_shared <UndoStep> (_ ("Remove Route"));

				getBrowserWindow () -> deleteRoute (getBrowser () -> getExecutionContext (),
				                                    route -> getSourceNode (),
				                                    route -> getSourceField (),
				                                    route -> getDestinationNode (),
				                                    route -> getDestinationField (),
				                                    undoStep);

				getBrowserWindow () -> addUndoStep (undoStep);
			}
			catch (const X3D::X3DError &)
			{ }

			return;
		}
		default:
		{
			collapse_row (path);

			is_full_expanded (get_model () -> get_iter (path), true);

			expand_row (path, false);

			return;
		}
	}

	return;
}

bool
OutlineTreeViewEditor::select_route (double x, double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (path .size ())
	{
		auto iter = get_model () -> get_iter (path);
		auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				auto route = static_cast <X3D::Route*> (data -> get_object ());

				// Pick

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT_CONNECTOR:
					{
						// Expand matching field
					
						try
						{
							expand_to (route -> getSourceNode () -> getField (route -> getSourceField ()));
						}
						catch (const X3D::X3DError &)
						{ }

						// Clear routes
					
						OutlineRoutes routes = get_cellrenderer () -> get_routes ();
						get_cellrenderer () -> clear_routes ();
						get_route_graph () -> update (routes);
						
						// Select routes
						
						get_cellrenderer () -> add_routes (data -> get_inputs_above ());
						get_cellrenderer () -> add_routes (data -> get_inputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

						return true;
					}
					case OutlineCellContent::OUTPUT_CONNECTOR:
					{
						// Expand matching field
					
						try
						{
							expand_to (route -> getDestinationNode () -> getField (route -> getDestinationField ()));
						}
						catch (const X3D::X3DError &)
						{ }

						// Clear routes
					
						OutlineRoutes routes = get_cellrenderer () -> get_routes ();
						get_cellrenderer () -> clear_routes ();
						get_route_graph () -> update (routes);
						
						// Select routes
						
						get_cellrenderer () -> add_routes (data -> get_outputs_above ());
						get_cellrenderer () -> add_routes (data -> get_outputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

						return true;
					}
					default:
						break;
				}
				
				break;
			}
			case OutlineIterType::X3DField:
			{
				auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

				// Pick

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT_CONNECTOR:
					{
						// Expand matching field
					
						for (const auto & route : get_model () -> get_input_routes (field))
						{
							try
							{
								expand_to (route -> getSourceNode () -> getField (route -> getSourceField ()));
							}
							catch (const X3D::X3DError &)
							{ }
						}

						// Clear routes
					
						OutlineRoutes routes = get_cellrenderer () -> get_routes ();
						get_cellrenderer () -> clear_routes ();
						get_route_graph () -> update (routes);
						
						// Select routes
						
						get_cellrenderer () -> add_routes (data -> get_inputs_above ());
						get_cellrenderer () -> add_routes (data -> get_inputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

						return true;;
					}
					case OutlineCellContent::OUTPUT_CONNECTOR:
					{
						// Expand matching field
					
						for (const auto & route : get_model () -> get_output_routes (field))
						{
							try
							{
								expand_to (route -> getDestinationNode () -> getField (route -> getDestinationField ()));
							}
							catch (const X3D::X3DError &)
							{ }
						}

						// Clear routes
					
						OutlineRoutes routes = get_cellrenderer () -> get_routes ();
						get_cellrenderer () -> clear_routes ();
						get_route_graph () -> update (routes);
						
						// Select routes
						
						get_cellrenderer () -> add_routes (data -> get_outputs_above ());
						get_cellrenderer () -> add_routes (data -> get_outputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

						return true;;
					}
					default:
						break;
				}
			}
			default:
				break;
		}
	}

	return false;
}

OutlineTreeViewEditor::~OutlineTreeViewEditor ()
{ }

} // puck
} // titania
