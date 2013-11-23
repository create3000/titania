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

#include "OutlineRouteGraph.h"

#include "X3DOutlineTreeView.h"
#include "OutlineTreeModel.h"

namespace titania {
namespace puck {

OutlineRouteGraph::OutlineRouteGraph (X3DOutlineTreeView* const treeView) :
	treeView (treeView)
{ }

void
OutlineRouteGraph::expand (const Gtk::TreeModel::iterator & iter)
{
	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			expand_node (iter);	
			break;
		}
		case OutlineIterType::X3DField:
		{
			expand_field (iter);	
			break;
		}
		default:
			break;
	}
}

void
OutlineRouteGraph::expand_node (const Gtk::TreeModel::iterator & parent)
{
	for (const auto & iter : parent -> children ())
	{
		auto field    = static_cast <X3D::X3DFieldDefinition*> (treeView -> get_object (iter));
		auto userData = treeView -> get_user_data (field);
		auto data     = treeView -> get_model () -> get_data (iter);
	
		for (const auto & path : userData -> paths)
		{
			for (const auto & route : field -> getInputRoutes ())
				add_input_route (path, data, route);

			for (const auto & route : field -> getOutputRoutes ())
				add_output_route (path, data, route);

			// There should only be one path;
			break;
		}
	}
}

void
OutlineRouteGraph::expand_field (const Gtk::TreeModel::iterator & parent)
{
	auto parentPath     = treeView -> get_model () -> get_path (parent);
	auto parentData     = treeView -> get_model () -> get_data (parent);
	auto parentUserData = parentData -> get_user_data ();
	auto field          = static_cast <X3D::X3DFieldDefinition*> (parentData -> get_object ());
	
	// Forward connections
	
	for (const auto & iter : parent -> children ())
	{
		auto data = treeView -> get_model () -> get_data (iter);
			
		for (const auto & route : parentData -> get_connections ())
			data -> get_connections () .emplace (route);
	}

	//
	
	if (parentUserData -> all_expanded)
	{
		for (const auto & route : field -> getInputRoutes ())
			remove_input_route (parentPath, parentData, route);
		
		for (const auto & route : field -> getOutputRoutes ())
			remove_output_route (parentPath, parentData, route);
			
		parentData -> get_self_connection () = false;

		for (const auto & iter : parent -> children ())
		{
			auto path = treeView -> get_model () -> get_path (iter);
			auto data = treeView -> get_model () -> get_data (iter);

			switch (data -> get_type ())
			{
				case OutlineIterType::X3DInputRoute:
				{
					auto route = static_cast <X3D::Route*> (data -> get_object ());

					add_input_route (path, data, route);

					break;
				}
				case OutlineIterType::X3DOutputRoute:
				{
					auto route = static_cast <X3D::Route*> (data -> get_object ());

					add_output_route (path, data, route);

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		for (const auto & iter : parent -> children ())
		{
			auto data = treeView -> get_model () -> get_data (iter);
			
			for (const auto & route : parentData -> get_inputs_below ())
				data -> get_connections () .emplace (route);
		
			for (const auto & route : parentData -> get_outputs_below ())
				data -> get_connections () .emplace (route);
		}
	}
}

void
OutlineRouteGraph::add_input_route (const Gtk::TreeModel::Path & destinationPath, OutlineTreeData* const destinationData, X3D::Route* const route)
{
	auto sourceNode = route -> getSourceNode ();

	if (sourceNode -> getExecutionContext () not_eq treeView -> get_model () -> get_execution_context ())
		return;
	
	try
	{
		auto sourceField    = sourceNode -> getField (route -> getSourceField ());
		auto sourceUserData = treeView -> get_user_data (sourceField);

		for (auto sourcePath : sourceUserData -> paths)
		{
			auto sourceIter = treeView -> get_model () -> get_iter (sourcePath);
			auto sourceData = treeView -> get_model () -> get_data (sourceIter);
			
			// Connect to output route if field is all expanded

			if (sourceData -> get_user_data () -> all_expanded)
			{
				for (const auto & iter : sourceIter -> children ())
				{
					auto data = treeView -> get_model () -> get_data (iter);

					if (data -> get_type () == OutlineIterType::X3DOutputRoute)
					{
						auto outputRoute = static_cast <X3D::Route*> (data -> get_object ());
					
						if (outputRoute == route)
						{
							sourcePath = treeView -> get_model () -> get_path (iter);
							sourceData = data;
							break;
						}
					}
				}
			}
			
			// Add route
			
			if (sourcePath < destinationPath)
			{
				// The destination path is above path
				destinationData -> get_inputs_above () .emplace (sourcePath, destinationPath);
				sourceData -> get_outputs_below () .emplace (sourcePath, destinationPath);

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::add_connection_below), sourcePath, destinationPath));
			}
			else if (sourcePath > destinationPath)
			{
				// The destination path is below path

				destinationData -> get_inputs_below () .emplace (sourcePath, destinationPath);
				sourceData -> get_outputs_above () .emplace (sourcePath, destinationPath);
				
				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::add_connection_above), sourcePath, destinationPath));
			}
			else
			{
				destinationData -> get_self_connection () = true;
			}

			// There should only be one path;
			break;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
OutlineRouteGraph::add_output_route (const Gtk::TreeModel::Path & sourcePath, OutlineTreeData* const sourceData, X3D::Route* const route)
{
	auto destinationNode = route -> getDestinationNode ();

	if (destinationNode -> getExecutionContext () not_eq treeView -> get_model () -> get_execution_context ())
		return;

	try
	{
		auto destinationField    = destinationNode -> getField (route -> getDestinationField ());
		auto destinationUserData = treeView -> get_user_data (destinationField);

		for (auto destinationPath : destinationUserData -> paths)
		{
			auto destinationIter = treeView -> get_model () -> get_iter (destinationPath);
			auto destinationData = treeView -> get_model () -> get_data (destinationIter);
			
			// Connect to input route if field is all expanded

			if (destinationData -> get_user_data () -> all_expanded)
			{
				for (const auto & iter : destinationIter -> children ())
				{
					auto data = treeView -> get_model () -> get_data (iter);

					if (data -> get_type () == OutlineIterType::X3DInputRoute)
					{
						auto inputRoute = static_cast <X3D::Route*> (data -> get_object ());
					
						if (inputRoute == route)
						{
							destinationPath = treeView -> get_model () -> get_path (iter);
							destinationData = data;
							break;
						}
					}
				}
			}

			// Add route

			if (destinationPath < sourcePath)
			{
				// The destination path is above path
				sourceData -> get_outputs_above () .emplace (sourcePath, destinationPath);
				destinationData -> get_inputs_below () .emplace (sourcePath, destinationPath);

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::add_connection_above), sourcePath, destinationPath));
			}
			else if (destinationPath > sourcePath)
			{
				// The destination path is below path
				
				sourceData -> get_outputs_below () .emplace (sourcePath, destinationPath);
				destinationData -> get_inputs_above () .emplace (sourcePath, destinationPath);

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::add_connection_below), sourcePath, destinationPath));
			}
			else
			{
				sourceData -> get_self_connection () = true;
			}

			// There should only be one path;
			break;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
OutlineRouteGraph::add_connection_above (const Gtk::TreeModel::iterator & iter,
                                         const Gtk::TreeModel::Path & sourcePath,
                                         const Gtk::TreeModel::Path & destinationPath)
{
	auto path = treeView -> get_model () -> get_path (iter);
	auto data = treeView -> get_model () -> get_data (iter);
	
	if (path > destinationPath)
	{
		if (path < sourcePath)
		{
			data -> get_connections () .emplace (sourcePath, destinationPath);
			return false;
		}

		return true; // exit
	}

	return false;
}

bool
OutlineRouteGraph::add_connection_below (const Gtk::TreeModel::iterator & iter,
                                         const Gtk::TreeModel::Path & sourcePath,
                                         const Gtk::TreeModel::Path & destinationPath)
{
	auto path = treeView -> get_model () -> get_path (iter);
	auto data = treeView -> get_model () -> get_data (iter);
	
	if (path < destinationPath)
	{
		if (path > sourcePath)
			data -> get_connections () .emplace (sourcePath, destinationPath);

		return false;
	}

	return true; // exit
}

void
OutlineRouteGraph::collapse (const Gtk::TreeModel::iterator & iter)
{
	switch (treeView -> get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			collapse_node (iter);
			break;
		}
		case OutlineIterType::X3DField:
		{
			collapse_field (iter);
			break;
		}
		default:
			break;
	}
}

void
OutlineRouteGraph::collapse_node (const Gtk::TreeModel::iterator & parent)
{
}

void
OutlineRouteGraph::collapse_field (const Gtk::TreeModel::iterator & parent)
{
	auto parentPath     = treeView -> get_model () -> get_path (parent);
	auto parentData     = treeView -> get_model () -> get_data (parent);
	auto parentUserData = parentData -> get_user_data ();
	auto field          = static_cast <X3D::X3DFieldDefinition*> (parentData -> get_object ());

	for (const auto & iter : parent -> children ())
	{
		auto path = treeView -> get_model () -> get_path (iter);
		auto data = treeView -> get_model () -> get_data (iter);

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DInputRoute:
			{
				auto route = static_cast <X3D::Route*> (data -> get_object ());

				remove_input_route (path, data, route);

				break;
			}
			case OutlineIterType::X3DOutputRoute:
			{
				auto route = static_cast <X3D::Route*> (data -> get_object ());

				remove_output_route (path, data, route);

				break;
			}
			default:
				break;
		}
	}

	for (const auto & route : field -> getInputRoutes ())
		add_input_route (parentPath, parentData, route);

	for (const auto & route : field -> getOutputRoutes ())
		add_output_route (parentPath, parentData, route);
}

void
OutlineRouteGraph::remove_input_route (const Gtk::TreeModel::Path & destinationPath, OutlineTreeData* const destinationData, X3D::Route* const route)
{
	auto sourceNode = route -> getSourceNode ();

	if (sourceNode -> getExecutionContext () not_eq treeView -> get_model () -> get_execution_context ())
		return;
	
	try
	{
		auto sourceField    = sourceNode -> getField (route -> getSourceField ());
		auto sourceUserData = treeView -> get_user_data (sourceField);

		for (auto sourcePath : sourceUserData -> paths)
		{
			auto sourceIter = treeView -> get_model () -> get_iter (sourcePath);
			auto sourceData = treeView -> get_model () -> get_data (sourceIter);
			
			// Connect to output route if field is all expanded

			if (sourceData -> get_user_data () -> all_expanded)
			{
				for (const auto & iter : sourceIter -> children ())
				{
					auto data = treeView -> get_model () -> get_data (iter);

					if (data -> get_type () == OutlineIterType::X3DOutputRoute)
					{
						auto outputRoute = static_cast <X3D::Route*> (data -> get_object ());
					
						if (outputRoute == route)
						{
							sourcePath = treeView -> get_model () -> get_path (iter);
							sourceData = data;
							break;
						}
					}
				}
			}
			
			// Add route
			
			if (sourcePath < destinationPath)
			{
				// The destination path is above path
				destinationData -> get_inputs_above () .erase (std::make_pair (sourcePath, destinationPath));
				sourceData -> get_outputs_below () .erase (std::make_pair (sourcePath, destinationPath));

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::remove_connection_below), sourcePath, destinationPath));
			}
			else if (sourcePath > destinationPath)
			{
				// The destination path is below path

				destinationData -> get_inputs_below () .erase (std::make_pair (sourcePath, destinationPath));
				sourceData -> get_outputs_above () .erase (std::make_pair (sourcePath, destinationPath));
				
				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::remove_connection_above), sourcePath, destinationPath));
			}
			else
			{
				destinationData -> get_self_connection () = false;
			}

			// There should only be one path;
			break;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
OutlineRouteGraph::remove_output_route (const Gtk::TreeModel::Path & sourcePath, OutlineTreeData* const sourceData, X3D::Route* const route)
{
	auto destinationNode = route -> getDestinationNode ();

	if (destinationNode -> getExecutionContext () not_eq treeView -> get_model () -> get_execution_context ())
		return;

	try
	{
		auto destinationField    = destinationNode -> getField (route -> getDestinationField ());
		auto destinationUserData = treeView -> get_user_data (destinationField);

		for (auto destinationPath : destinationUserData -> paths)
		{
			auto destinationIter = treeView -> get_model () -> get_iter (destinationPath);
			auto destinationData = treeView -> get_model () -> get_data (destinationIter);
			
			// Connect to input route if field is all expanded

			if (destinationData -> get_user_data () -> all_expanded)
			{
				for (const auto & iter : destinationIter -> children ())
				{
					auto data = treeView -> get_model () -> get_data (iter);

					if (data -> get_type () == OutlineIterType::X3DInputRoute)
					{
						auto inputRoute = static_cast <X3D::Route*> (data -> get_object ());
					
						if (inputRoute == route)
						{
							destinationPath = treeView -> get_model () -> get_path (iter);
							destinationData = data;
							break;
						}
					}
				}
			}

			// Add route

			if (destinationPath < sourcePath)
			{
				// The destination path is above path
				sourceData -> get_outputs_above () .erase (std::make_pair (sourcePath, destinationPath));
				destinationData -> get_inputs_below () .erase (std::make_pair (sourcePath, destinationPath));

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::remove_connection_above), sourcePath, destinationPath));
			}
			else if (destinationPath > sourcePath)
			{
				// The destination path is below path
				
				sourceData -> get_outputs_below () .erase (std::make_pair (sourcePath, destinationPath));
				destinationData -> get_inputs_above () .erase (std::make_pair (sourcePath, destinationPath));

				treeView -> get_model () -> foreach_iter (sigc::bind (sigc::mem_fun (*this, &OutlineRouteGraph::remove_connection_below), sourcePath, destinationPath));
			}
			else
			{
				sourceData -> get_self_connection () = false;
			}

			// There should only be one path;
			break;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
OutlineRouteGraph::remove_connection_above (const Gtk::TreeModel::iterator & iter,
                                            const Gtk::TreeModel::Path & sourcePath,
                                            const Gtk::TreeModel::Path & destinationPath)
{
	auto path = treeView -> get_model () -> get_path (iter);
	auto data = treeView -> get_model () -> get_data (iter);
	
	if (path > destinationPath)
	{
		if (path < sourcePath)
		{
			data -> get_connections () .erase (std::make_pair (sourcePath, destinationPath));
			return false;
		}

		return true; // exit
	}

	return false;
}

bool
OutlineRouteGraph::remove_connection_below (const Gtk::TreeModel::iterator & iter,
                                            const Gtk::TreeModel::Path & sourcePath,
                                            const Gtk::TreeModel::Path & destinationPath)
{
	auto path = treeView -> get_model () -> get_path (iter);
	auto data = treeView -> get_model () -> get_data (iter);
	
	if (path < destinationPath)
	{
		if (path > sourcePath)
			data -> get_connections () .erase (std::make_pair (sourcePath, destinationPath));

		return false;
	}

	return true; // exit
}

} // puck
} // titania
