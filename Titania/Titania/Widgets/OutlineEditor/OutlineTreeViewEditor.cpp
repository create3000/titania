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

#include "OutlineTreeViewEditor.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/X3DBrowserWindow.h"

#include "CellRenderer/OutlineCellRenderer.h"
#include "OutlineDragDrop.h"
#include "OutlineRouteGraph.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeObserver.h"

#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

OutlineTreeViewEditor::OutlineTreeViewEditor (X3DBrowserWindow* const browserWindow, const X3D::X3DExecutionContextPtr & executionContext, OutlineEditor* const outlineEditor) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	        Glib::ObjectBase (typeid (OutlineTreeViewEditor)),
	      X3DOutlineTreeView (executionContext),
	                dragDrop (new OutlineDragDrop (outlineEditor, this)),
	            overUserData (new UserData ()),
	        selectedUserData (new UserData ()),
	         matchingContext (),
	       matchingFieldType (),
	      matchingAccessType (0),
	              sourcePath (),
	              sourceNode (),
	             sourceField (),
	         destinationPath (),
	         destinationNode (),
	        destinationField (),
	motion_notify_connection ()
{
	addChildObjects (sourceNode, destinationNode);

	set_name ("OutlineTreeViewEditor");

	watch_motion (true);

	get_cellrenderer () -> signal_edited () .connect (sigc::mem_fun (this, &OutlineTreeViewEditor::on_edited));

	setup ();
}

void
OutlineTreeViewEditor::watch_motion (const bool value)
{
	motion_notify_connection .disconnect ();

	if (value)
		motion_notify_connection = signal_motion_notify_event () .connect (sigc::mem_fun (this, &OutlineTreeViewEditor::set_motion_notify_event), false);
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

			return Gtk::TreeView::on_button_press_event (event);
		}
		default:
			break;
	}

	return false;
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

			return Gtk::TreeView::on_button_release_event (event);
		}
		default:
			break;
	}

	return false;
}

bool
OutlineTreeViewEditor::set_motion_notify_event (GdkEventMotion* event)
{
	if (hover_access_type (event -> x, event -> y))
		return true;

	return false;
}

bool
OutlineTreeViewEditor::select_field_value (const double x, const double y)
{
	Gtk::TreeViewColumn* column = nullptr;
	const auto path = get_path_at_position (x, y, column);

	if (column != getColumn ())
		return false;

	if (path .size () < 3)
		return false;

	const auto iter     = get_model () -> get_iter (path);
	const auto nodeIter = iter -> parent () -> parent ();

	if (get_data_type (iter) not_eq OutlineIterType::X3DFieldValue)
		return false;

	const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

	switch (get_data_type (nodeIter))
	{
		case OutlineIterType::ExternProtoDeclaration:
		{
			if (field -> getUserData <UserData> () -> selected [OUTLINE_SPECIAL])
				break;
			
			return false;
		}
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExportedNode:
			break;
		default:
			return false;
	}

	const auto scene = get_execution_context () -> isType ({ X3D::X3DConstants::X3DScene }) ? get_execution_context () : get_execution_context () -> getScene ();
	const auto node  = *static_cast <X3D::SFNode*> (get_object (nodeIter));

	if (node -> getScene () not_eq scene)
		return true;

	if (node -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
		return true;

	if (field -> getAccessType () == X3D::outputOnly)
		return true;

	// First grab focus to release any previous selected field.
	grab_focus ();

	getBrowserWindow () -> setAccelerators (false);
	get_tree_observer () -> unwatch_tree (iter);
	watch_motion (false);
	set_cursor (path, *column, true);
	return true;
}

bool
OutlineTreeViewEditor::is_node (const Gtk::TreeModel::iterator & iter) const
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		{
			return *static_cast <X3D::SFNode*> (get_object (iter));
		}
		default:
			break;
	}

	return false;
}

X3D::X3DExecutionContext*
OutlineTreeViewEditor::get_context (const Gtk::TreeModel::iterator & iter) const
{
	const auto & sfnode = *static_cast <X3D::SFNode*> (get_object (iter));

	if (not sfnode)
		return nullptr;

	return sfnode -> getExecutionContext ();
}

void
OutlineTreeViewEditor::on_edited (const Glib::ustring & string_path, const Glib::ustring & text)
{
	const Gtk::TreeModel::Path     path (string_path);
	const Gtk::TreeModel::iterator iter = get_model () -> get_iter (path);

	get_tree_observer () -> watch_child (iter, path);

	getBrowserWindow () -> setAccelerators (true);
	watch_motion (true);
}

bool
OutlineTreeViewEditor::hover_access_type (const double x, const double y)
{
	if (inPrototypeInstance ())
		return false;

	Gtk::TreeViewColumn*       column = nullptr;
	const Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (column != getColumn ())
		return false;

	// Clear over state

	overUserData -> selected .reset (OUTLINE_OVER_INPUT);
	overUserData -> selected .reset (OUTLINE_OVER_OUTPUT);

	for (const auto & path : overUserData -> paths)
		get_model () -> row_changed (path, get_model () -> get_iter (path));

	// Test for over

	if (path .size ())
	{
		const auto iter = get_model () -> get_iter (path);
		const auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DField:
			{
				const auto parentIter = iter -> parent ();

				if (not is_node (parentIter))
					return false;

				const auto context = get_context (parentIter);

				if (context -> getScene () not_eq get_execution_context () -> getScene ())
					return false;

				if (context -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return false;

				if (matchingContext and context not_eq matchingContext)
					return false;

				const auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

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
							overUserData -> selected .set (OUTLINE_OVER_INPUT);
							get_model () -> row_changed (path, iter);
							return true;
						}

						return false;
					}
					case OutlineCellContent::OUTPUT:
					{
						if (not matchingAccessType or (field -> getAccessType () & (matchingAccessType & X3D::outputOnly) and field -> getType () == matchingFieldType))
						{
							overUserData -> selected .set (OUTLINE_OVER_OUTPUT);
							get_model () -> row_changed (path, iter);
							return true;
						}

						return false;
					}
					default:
						return false;
				}

				return false;
			}
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				const auto parentIter = iter -> parent () -> parent ();

				if (not is_node (parentIter))
					return false;

				const auto context = get_context (parentIter);

				if (context -> getScene () not_eq get_execution_context () -> getScene ())
					return false;

				if (context -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return false;

				overUserData = data -> get_user_data ();

				Gdk::Rectangle cell_area;
				get_cell_area (path, *column, cell_area);
				get_cellrenderer () -> property_data () .set_value (data);

				switch (get_cellrenderer () -> pick (*this, cell_area, x, y))
				{
					case OutlineCellContent::INPUT:
					{
						if (not matchingAccessType)
						{
							overUserData -> selected .set (OUTLINE_OVER_INPUT);
							get_model () -> row_changed (path, iter);
							return true;
						}

						return false;
					}
					case OutlineCellContent::OUTPUT:
					{
						if (not matchingAccessType)
						{
							overUserData -> selected .set (OUTLINE_OVER_OUTPUT);
							get_model () -> row_changed (path, iter);
							return true;
						}

						return false;
					}
					default:
						return false;
				}

				return false;
			}
			default:
				return false;
		}
	}

	return false;
}

bool
OutlineTreeViewEditor::select_access_type (const double x, const double y)
{
	if (get_control_key ())
		return remove_route (x, y);

	if (add_route (x, y))
		return true;

	return select_route (x, y);
}

X3D::SFNode
OutlineTreeViewEditor::get_node (OutlineTreeData* const nodeData) const
{
	switch (nodeData -> get_type ())
	{
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (nodeData -> get_object ());

			if (not sfnode)   
				break;

			if (sfnode -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
				break;

			return sfnode;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (nodeData -> get_object ());
				const auto importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode -> getValue ());
				const auto exportedNode = importedNode -> getExportedNode ();

				if (importedNode -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					break;

				return exportedNode;
			}
			catch (...)
			{ }

			break;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (nodeData -> get_object ());
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode -> getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();

				if (exportedNode -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					break;

				return localNode;
			}
			catch (...)
			{ }

			break;
		}
		default:
			break;
	}

	return nullptr;
}

bool
OutlineTreeViewEditor::add_route (const double x, const double y)
{
	if (inPrototypeInstance ())
		return false;

	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (column != getColumn ())
		return false;

	if (path .size ())
	{
		const auto iter = get_model () -> get_iter (path);
		const auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DField:
			{
				const auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

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
								   destinationPath = path;

									path .up ();
									const auto nodeIter = get_model () -> get_iter (path);
									const auto nodeData = get_model () -> get_data (nodeIter);

									const auto destinationContext = get_context (nodeIter);
									const auto destinationNode    = get_node (nodeData);
									const auto destinationField   = field -> getName ();

									if (not destinationNode)
										return false;

									if (destinationContext not_eq matchingContext)
										return false;

									// Add route

									try
									{
										const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Route"));
										X3D::X3DEditor::storeMatrix (destinationNode, undoStep);
										X3D::X3DEditor::addRoute (matchingContext, sourceNode, sourceField, destinationNode, destinationField, undoStep);
										getBrowserWindow () -> addUndoStep (undoStep);
									}
									catch (const X3D::X3DError &)
									{ }

									// Clear selection

									clear_access_type_selection (selectedUserData);

									// Clear routes

									clear_routes ();

									// Select route

									get_cellrenderer () -> add_routes ({ std::make_pair (sourcePath, destinationPath) });
									get_route_graph () -> update (get_cellrenderer () -> get_routes ());
								}
							}

							return true;
						}

						destinationPath = path;

						path .up ();
						const auto nodeIter = get_model () -> get_iter (path);
						const auto nodeData = get_model () -> get_data (nodeIter);
						const auto context  = get_context (nodeIter);

						destinationNode  = get_node (nodeData);
						destinationField = field -> getName ();

						if (not destinationNode)
							return false;

						if (context -> getScene () not_eq get_execution_context () -> getScene ())
							return false;

						if (context -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
							return false;

						set_access_type_selection (data -> get_user_data (), OUTLINE_SELECTED_INPUT);

						matchingContext    = context;
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
								   sourcePath = path;

									path .up ();
									const auto nodeIter = get_model () -> get_iter (path);
									const auto nodeData = get_model () -> get_data (nodeIter);

									const auto sourceContext = get_context (nodeIter);
									const auto sourceNode    = get_node (nodeData);
									const auto sourceField   = field -> getName ();

									if (not sourceNode)
										return false;

									if (sourceContext not_eq matchingContext)
										return false;

									// Add route

									try
									{
										const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Route"));
										X3D::X3DEditor::storeMatrix (destinationNode, undoStep);
										X3D::X3DEditor::addRoute (matchingContext, sourceNode, sourceField, destinationNode, destinationField, undoStep);
										getBrowserWindow () -> addUndoStep (undoStep);
									}
									catch (const X3D::X3DError &)
									{ }

									// Clear selection

									clear_access_type_selection (selectedUserData);

									// Clear routes

									clear_routes ();

									// Select route

									get_cellrenderer () -> add_routes ({ std::make_pair (sourcePath, destinationPath) });
									get_route_graph () -> update (get_cellrenderer () -> get_routes ());
								}
							}

							return true;
						}

						sourcePath = path;

						path .up ();
						const auto nodeIter = get_model () -> get_iter (path);
						const auto nodeData = get_model () -> get_data (nodeIter);
						const auto context  = get_context (nodeIter);

						sourceNode  = get_node (nodeData);
						sourceField = field -> getName ();

						if (not sourceNode)
							return false;

						if (context -> getScene () not_eq get_execution_context () -> getScene ())
							return false;

						if (context -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
							return false;

						set_access_type_selection (data -> get_user_data (), OUTLINE_SELECTED_OUTPUT);

						matchingContext    = context;
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
OutlineTreeViewEditor::set_access_type_selection (const UserDataPtr & userData, const size_t type)
{
	userData -> selected .reset (OUTLINE_SELECTED_INPUT);
	userData -> selected .reset (OUTLINE_SELECTED_OUTPUT);
	userData -> selected .reset (OUTLINE_OVER_INPUT);
	userData -> selected .reset (OUTLINE_OVER_OUTPUT);

	userData -> selected .set (type);

	selectedUserData = userData;

	for (const auto & path : userData -> paths)
		get_model () -> row_changed (path, get_model () -> get_iter (path));
}

void
OutlineTreeViewEditor::clear_access_type_selection (const UserDataPtr & userData)
{
	if (not get_shift_key ())
	{
		userData -> selected .reset (OUTLINE_SELECTED_INPUT);
		userData -> selected .reset (OUTLINE_SELECTED_OUTPUT);
		userData -> selected .reset (OUTLINE_OVER_INPUT);
		userData -> selected .reset (OUTLINE_OVER_OUTPUT);

		for (const auto & path : userData -> paths)
			get_model () -> row_changed (path, get_model () -> get_iter (path));

		matchingContext    = nullptr;
		matchingAccessType = 0;
		sourceNode         = nullptr;
		destinationNode    = nullptr;
	}
}

bool
OutlineTreeViewEditor::remove_route (const double x, const double y)
{
	if (inPrototypeInstance ())
		return false;

	Gtk::TreeViewColumn* column = nullptr;
	Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (column != getColumn ())
		return false;

	if (path .size ())
	{
		const auto iter = get_model () -> get_iter (path);
		const auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				const auto route = static_cast <X3D::Route*> (data -> get_object ());

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
							if (route -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
								return false;

							const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Route"));

							X3D::X3DEditor::deleteRoute (X3D::X3DExecutionContextPtr (route -> getExecutionContext ()),
							                             route -> getSourceNode (),
							                             route -> getSourceField (),
							                             route -> getDestinationNode (),
							                             route -> getDestinationField (),
							                             undoStep);

							getBrowserWindow () -> addUndoStep (undoStep);
							return true;
						}
						catch (const X3D::X3DError &)
						{ }

						break;
					}
					default:
						break;
				}

				return false;
			}
			case OutlineIterType::X3DField:
			{
				const auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

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

				break;
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
				const auto route = routes [0];

				if (route -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					return;

				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Route"));

				X3D::X3DEditor::deleteRoute (X3D::X3DExecutionContextPtr (route -> getExecutionContext ()),
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
			getScrollFreezer () -> freeze ();
			collapse_row (path);
			is_full_expanded (get_model () -> get_iter (path), true);
			expand_row (path, false);
			return;
		}
	}

	return;
}

bool
OutlineTreeViewEditor::select_route (const double x, const double y)
{
	Gtk::TreeViewColumn*       column = nullptr;
	const Gtk::TreeModel::Path path   = get_path_at_position (x, y, column);

	if (column != getColumn ())
		return false;

	if (path .size ())
	{
		const auto iter = get_model () -> get_iter (path);
		const auto data = get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DInputRoute:
			case OutlineIterType::X3DOutputRoute:
			{
				const auto route = static_cast <X3D::Route*> (data -> get_object ());

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

						clear_routes ();

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

						clear_routes ();

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
				const auto field = static_cast <X3D::X3DFieldDefinition*> (data -> get_object ());

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

						clear_routes ();

						// Select routes

						get_cellrenderer () -> add_routes (data -> get_inputs_above ());
						get_cellrenderer () -> add_routes (data -> get_inputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

						return true;
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

						clear_routes ();

						// Select routes

						get_cellrenderer () -> add_routes (data -> get_outputs_above ());
						get_cellrenderer () -> add_routes (data -> get_outputs_below ());
						get_route_graph () -> update (get_cellrenderer () -> get_routes ());

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
OutlineTreeViewEditor::clear_routes ()
{
	const OutlineRoutes routes = get_cellrenderer () -> get_routes ();
	get_cellrenderer () -> clear_routes ();
	get_route_graph () -> update (routes);
}

OutlineTreeViewEditor::~OutlineTreeViewEditor ()
{
	dispose ();
}

} // puck
} // titania
