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

#include "X3DOutlineTreeView.h"

#include "../../Base/AdjustmentObject.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "CellRenderer/OutlineCellRenderer.h"
#include "CellRenderer/OutlineSeparator.h"
#include "OutlineRouteGraph.h"
#include "OutlineSelection.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeObserver.h"

#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Basic/Traverse.h>

#include <Titania/String/sprintf.h>

namespace titania {
namespace puck {

X3DOutlineTreeView::X3DOutlineTreeView (const X3D::X3DExecutionContextPtr & executionContext) :
	        X3DBaseInterface (),
	           Gtk::TreeView (),
	                   model (OutlineTreeModel::create (executionContext)),
	               selection (new OutlineSelection (getBrowserWindow (), this)),
	                  column (nullptr),
	            treeObserver (new OutlineTreeObserver (this)),
	              routeGraph (new OutlineRouteGraph (this)),
	            cellrenderer (Gtk::manage (new OutlineCellRenderer (getCurrentBrowser (), this))),
				padCellrenderer (Gtk::manage (new Gtk::CellRendererText ())),
	             expandLevel (0),
	            externProtos (false),
	              prototypes (false),
	           importedNodes (false),
	           exportedNodes (false),
	      expandExternProtos (false),
	expandPrototypeInstances (false),
	       expandInlineNodes (false),
	               useLocale (true),
	             hadjustment (new AdjustmentObject ()),
	             vadjustment (new AdjustmentObject ())
{
	// Options

	set_headers_visible (false);
	set_enable_search (false);
	Gtk::TreeView::get_selection () -> set_mode (Gtk::SELECTION_NONE);

	// Columns

	Gtk::TreeViewColumn* treeviewcolumn_name = Gtk::manage (new Gtk::TreeViewColumn (_ ("Scene")));
	treeviewcolumn_name -> set_expand (false);

	// CellRenderer

	treeviewcolumn_name -> pack_start (*cellrenderer, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "tree-data",           OutlineTreeModel::DATA_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "foreground-set",      OutlineTreeModel::SELECTED_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);

	column = treeviewcolumn_name;

	// Append column

	append_column (*treeviewcolumn_name);

	// Pad column

	Gtk::TreeViewColumn* treeviewcolumn_pad = Gtk::manage (new Gtk::TreeViewColumn ());
	treeviewcolumn_pad -> set_expand (true);

	treeviewcolumn_pad -> pack_start (*padCellrenderer, true);
	treeviewcolumn_pad -> add_attribute (*padCellrenderer, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);

	append_column (*treeviewcolumn_pad);

	on_style_updated ();

	//

	set_execution_context (executionContext);
}

void
X3DOutlineTreeView::on_style_updated ()
{
	// Color

	const auto selectedForegroundColor = get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto selectedBackgroundColor = get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);

	cellrenderer -> property_foreground_rgba ()      = selectedForegroundColor;
	cellrenderer -> property_cell_background_rgba () = selectedBackgroundColor;

	padCellrenderer -> property_cell_background_rgba () = selectedBackgroundColor;

//	queue_draw (); // XXX: forces infinite loop of redraw of whole window
}

void
X3DOutlineTreeView::set_use_locale (const bool value)
{
	useLocale = value;
	queue_draw ();
}

Gtk::TreeModel::Path
X3DOutlineTreeView::get_path_at_position (const double x, const double y) const
{
	Gtk::TreeViewColumn* column = nullptr;

	return get_path_at_position (x, y, column);
}

Gtk::TreeModel::Path
X3DOutlineTreeView::get_path_at_position (const double x, const double y, Gtk::TreeViewColumn* & column) const
{
	Gtk::TreeModel::Path path;
	int                  cell_x = 0;
	int                  cell_y = 0;

	get_path_at_pos (x, y, path, column, cell_x, cell_y);

	return path;
}

void
X3DOutlineTreeView::expand_to (X3D::X3DChildObject* const object)
{
	Gtk::TreeModel::Path path;

	disable_shift_key ();

	int flags = X3D::TRAVERSE_ROOT_NODES;

	if (expandPrototypeInstances)
		flags |= X3D::TRAVERSE_PROTOTYPE_INSTANCES;

	if (expandInlineNodes)
		flags |= X3D::TRAVERSE_INLINE_NODES;

	if (expandExternProtos and externProtos)
		flags |= X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS | X3D::TRAVERSE_EXTERNPROTO_PROTO_DECLARATIONS;

	if (prototypes)
		flags |= X3D::TRAVERSE_PROTO_DECLARATIONS;

	if (importedNodes)
		flags |= X3D::TRAVERSE_IMPORTED_NODES;

	if (exportedNodes)
		flags |= X3D::TRAVERSE_EXPORTED_NODES;

	auto hierarchy = X3D::find (get_execution_context (), object, flags);

	if (not hierarchy .empty ())
		hierarchy .erase (hierarchy .begin ());

	if (not hierarchy .empty ())
		expand_to (get_model () -> children (), hierarchy, path);

	enable_shift_key ();
}

bool
X3DOutlineTreeView::expand_to (const Gtk::TreeModel::Children & children, std::vector <X3D::X3DChildObject*> & hierarchy, Gtk::TreeModel::Path & path)
{
	path .push_back (0);

	for (const auto & child : children)
	{
		const auto data   = get_model () -> get_data (child);
		auto       object = data -> get_object ();

		switch (data -> get_type ())
		{
			case OutlineIterType::X3DBaseNode:
			case OutlineIterType::X3DExecutionContext:
			case OutlineIterType::ExternProtoDeclaration:
			case OutlineIterType::ProtoDeclaration:
			{
				object = static_cast <X3D::SFNode*> (object) -> getValue ();
				break;
			}
			case OutlineIterType::ImportedNode:
			{
				try
				{
					const auto sfnode       = static_cast <X3D::SFNode*> (object);
					const auto importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode -> getValue ());
					const auto exportedNode = importedNode -> getExportedNode ();

					object = exportedNode .getValue ();
				}
				catch (...)
				{ }

				break;
			}
			case OutlineIterType::ExportedNode:
			{
				try
				{
					const auto sfnode       = static_cast <X3D::SFNode*> (object);
					const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode -> getValue ());
					const auto localNode    = exportedNode -> getLocalNode ();

					object = localNode .getValue ();
				}
				catch (...)
				{ }

				break;
			}
			default:
				break;
		}

		if (object == hierarchy .front ())
		{
			hierarchy .erase (hierarchy .begin ());

			if (hierarchy .empty ())
				return true;

			expand_row (path, false);
			return expand_to (child -> children (), hierarchy, path);
		}

		path .next ();
	}

	return false;
}

void
X3DOutlineTreeView::collapse_row (const Gtk::TreeModel::Path & path)
{
	if (row_expanded (path))
		Gtk::TreeView::collapse_row (path);
}

void
X3DOutlineTreeView::set_model (const Glib::RefPtr <OutlineTreeModel> & value)
{
	Gtk::TreeView::set_model (value);

	model = value;
}

void
X3DOutlineTreeView::preserve_adjustments ()
{
	hadjustment -> preserve (get_hadjustment ());
	vadjustment -> preserve (get_vadjustment ());
}

void
X3DOutlineTreeView::set_adjustments (const double h, const double v)
{
	hadjustment -> restore (get_hadjustment (), h);
	vadjustment -> restore (get_vadjustment (), v);
}

std::vector <Gtk::TreeModel::iterator>
X3DOutlineTreeView::get_iters (X3D::X3DChildObject* const object) const
{
	return get_model () -> get_iters (object);
}

UserDataPtr
X3DOutlineTreeView::get_user_data (const Gtk::TreeModel::iterator & iter) const
{
	UserDataPtr user_data = get_model () -> get_user_data (iter);

	if (get_data_type (iter) == OutlineIterType::X3DExecutionContext)
	{
		if (not user_data -> user_data)
			user_data -> user_data .reset (new UserData ());

		return user_data -> user_data;
	}

	return user_data;
}

UserDataPtr
X3DOutlineTreeView::get_user_data (X3D::X3DChildObject* const object)
{
	return OutlineTreeModel::get_user_data (object);
}

OutlineIterType
X3DOutlineTreeView::get_data_type (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_data_type (iter);
}

X3D::X3DChildObject*
X3DOutlineTreeView::get_object (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_object (iter);
}

size_t
X3DOutlineTreeView::get_index (const Gtk::TreeModel::iterator & iter) const
{
	return get_model () -> get_data (iter) -> get_index ();
}

void
X3DOutlineTreeView::set_open_path (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & value)
{
	const auto userData = get_user_data (iter);

	if (userData)
		userData -> open_path = value;
}

Gtk::TreeModel::Path
X3DOutlineTreeView::get_open_path (const Gtk::TreeModel::iterator & iter) const
{
	const auto userData = get_user_data (iter);

	if (userData)
		return userData -> open_path;

	return Gtk::TreeModel::Path ();
}

void
X3DOutlineTreeView::is_expanded (const Gtk::TreeModel::iterator & iter, const bool value)
{
	const auto userData = get_user_data (iter);

	if (userData)
		userData -> expanded = value;
}

bool
X3DOutlineTreeView::is_expanded (const Gtk::TreeModel::iterator & iter) const
{
	const auto userData = get_user_data (iter);

	if (userData)
		return userData -> expanded;

	return false;
}

void
X3DOutlineTreeView::is_full_expanded (const Gtk::TreeModel::iterator & iter, const bool value)
{
	const auto userData = get_user_data (iter);

	if (userData)
		userData -> full_expanded = value;
}

bool
X3DOutlineTreeView::is_full_expanded (const Gtk::TreeModel::iterator & iter) const
{
	const auto userData = get_user_data (iter);

	if (userData)
		return userData -> full_expanded;

	return false;
}

bool
X3DOutlineTreeView::get_shift_key ()
{
	return expandLevel == 0 and getBrowserWindow () -> getKeys () .shift ();
}

bool
X3DOutlineTreeView::get_control_key ()
{
	return getBrowserWindow () -> getKeys () .control ();
}

bool
X3DOutlineTreeView::get_alt_key ()
{
	return getBrowserWindow () -> getKeys () .alt ();
}

void
X3DOutlineTreeView::set_execution_context (const X3D::X3DExecutionContextPtr & executionContext)
{
	//__LOG__ << std::endl;
	
	get_hadjustment () -> set_value (0);
	get_vadjustment () -> set_value (0);

	// Remove model.

	X3D::X3DScenePtr scene (get_execution_context ());

	for (const auto & child : get_model () -> children ())
	{
		routeGraph -> collapse_tree (child);
		treeObserver -> unwatch_tree (child, false);
	}

	get_execution_context () -> getScene () -> units_changed () .removeInterest (this, &X3DOutlineTreeView::queue_draw);

	get_execution_context () -> getRootNodes ()          .removeInterest (this, &X3DOutlineTreeView::set_rootNodes);
	get_execution_context () -> importedNodes_changed () .removeInterest (this, &X3DOutlineTreeView::set_rootNodes);
	get_execution_context () -> prototypes_changed ()    .removeInterest (this, &X3DOutlineTreeView::set_rootNodes);
	get_execution_context () -> externProtos_changed ()  .removeInterest (this, &X3DOutlineTreeView::set_rootNodes);

	if (scene)
		scene -> exportedNodes_changed () .removeInterest (this, &X3DOutlineTreeView::set_rootNodes);

	// Set model.

	scene = executionContext;

	if (executionContext -> isProtoDeclaration ())
		executionContext -> realize ();

	set_model (OutlineTreeModel::create (executionContext));
	get_model () -> set_show_all_routes (get_expand_prototype_instances ());

	get_execution_context () -> getScene () -> units_changed () .addInterest (this, &X3DOutlineTreeView::queue_draw);

	executionContext -> getRootNodes ()          .addInterest (this, &X3DOutlineTreeView::set_rootNodes);
	executionContext -> importedNodes_changed () .addInterest (this, &X3DOutlineTreeView::set_rootNodes);
	executionContext -> prototypes_changed ()    .addInterest (this, &X3DOutlineTreeView::set_rootNodes);
	executionContext -> externProtos_changed ()  .addInterest (this, &X3DOutlineTreeView::set_rootNodes);

	if (scene)
		scene -> exportedNodes_changed () .addInterest (this, &X3DOutlineTreeView::set_rootNodes);

	set_rootNodes ();
}

const X3D::X3DExecutionContextPtr &
X3DOutlineTreeView::get_execution_context () const
{
	return get_model () -> get_execution_context ();
}

void
X3DOutlineTreeView::set_rootNodes ()
{
	//__LOG__ << std::endl;

	preserve_adjustments ();

	// Unwatch model.

	for (const auto & child : get_model () -> children ())
	{
		routeGraph -> collapse_tree (child);
		treeObserver -> unwatch_tree (child, false);
	}

	// Fill model.

	const X3D::X3DExecutionContextPtr & executionContext = get_execution_context ();

	unset_model ();
	get_model () -> clear ();
	//get_cellrenderer () -> clear_routes (); // Clear selected routes.

	// ExternProtos

	if (externProtos and not executionContext -> getExternProtoDeclarations () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Extern Prototypes")));

		size_t i = 0;

		for (auto & externProto : executionContext -> getExternProtoDeclarations ())
			get_model () -> append (OutlineIterType::ExternProtoDeclaration, externProto, i ++);
	}

	// Prototypes

	if (prototypes and not executionContext -> getProtoDeclarations () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Prototypes")));

		size_t i = 0;

		for (auto & prototype : executionContext -> getProtoDeclarations ())
			get_model () -> append (OutlineIterType::ProtoDeclaration, prototype, i ++);
	}

	// Root nodes

	if (not executionContext -> getRootNodes () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Root Nodes")));

		size_t i = 0;

		for (auto & rootNode : executionContext -> getRootNodes ())
		{
			Gtk::TreeModel::iterator iter;

			if (rootNode)
				iter = get_model () -> append (OutlineIterType::X3DBaseNode, rootNode, i ++);
			else
				iter = get_model () -> append (OutlineIterType::NULL_, &executionContext -> getRootNodes (), i ++);

			treeObserver -> watch_child (iter, get_model () -> get_path (iter));
		}
	}

	// Imported nodes

	if (importedNodes and not executionContext -> getImportedNodes () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Imported Nodes")));

		for (auto & importedNode : executionContext -> getImportedNodes ())
			get_model () -> append (OutlineIterType::ImportedNode, importedNode .second);
	}

	// Exported nodes

	if (exportedNodes)
	{
		const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext .getValue ());

		if (scene)
		{
			if (not scene -> getExportedNodes () .empty ())
			{
				get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Exported Nodes")));

				for (auto & exportedNode : scene -> getExportedNodes ())
					get_model () -> append (OutlineIterType::ExportedNode, exportedNode .second);
			}
		}
	}

	// Add at least one child!!!
	if (not get_model () -> children () .size ())
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Empty Scene")));

	set_model (get_model ());

	// Expand.

	disable_shift_key ();

	for (auto & iter : get_model () -> children ())
	{
		// Expand row again if it was previously expanded.

		if (is_expanded (iter))
		{
			const auto open_path = get_open_path (iter);
			const auto path      = Gtk::TreePath (iter);

			if (open_path .empty ())
				expand_row (path, false);
		}
	}

	enable_shift_key ();
}

void
X3DOutlineTreeView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	select_node (get_model () -> get_iter (path), path);
}

void
X3DOutlineTreeView::select_node (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	selection -> set_select_multiple (get_shift_key ());

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			const auto & localNode = *static_cast <X3D::SFNode*> (get_object (iter));

			if (localNode)
			{
				if (localNode -> getExecutionContext () == get_execution_context ())
					selection -> select (localNode);
			}

			break;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (get_object (iter));
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode -> getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();

				if (exportedNode -> getExecutionContext () == get_execution_context ())
					selection -> select (localNode);
			}
			catch (...)
			{ }

			break;
		}
		default:
			break;
	}
}

bool
X3DOutlineTreeView::on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	collapse_clone (iter);

	model_expand_row (iter); // (model expand_row)

	// Return false to allow expansion, true to reject.
	return false;
}

void
X3DOutlineTreeView::on_row_expanded (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	//__LOG__ << path .to_string () << std::endl;

	// Set expanded first to prevent loop with clones.

	is_expanded (iter, true);
	set_open_path (iter, path);

	routeGraph -> expand (iter);
	treeObserver -> watch (iter, path);

	columns_autosize (); // Resizes all columns to their optimal width.

	auto_expand (iter);
}

bool
X3DOutlineTreeView::on_test_collapse_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	// Return false to allow collapse, true to reject.
	return false;
}

void
X3DOutlineTreeView::on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	//__LOG__ << path .to_string () << std::endl;

	is_expanded (iter, false);
	is_full_expanded (iter, false);

	routeGraph -> collapse_tree (iter);
	treeObserver -> unwatch_tree (iter);

	get_model () -> clear (iter);

	toggle_expand (iter, path);

	columns_autosize ();
}

void
X3DOutlineTreeView::collapse_clone (const Gtk::TreeModel::iterator & iter)
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		{
			disable_shift_key ();
			collapse_row (get_open_path (iter));
			enable_shift_key ();
			break;
		}
		default:
			break;
	}
}

void
X3DOutlineTreeView::model_expand_row (const Gtk::TreeModel::iterator & iter)
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::NULL_:
			break;

		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

			if (get_shift_key () or is_full_expanded (iter))
			{
				expand_routes (iter, field);
				is_full_expanded (iter, iter -> children () .size ());
			}
			else
				is_full_expanded (iter, false);

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					const auto & sfnode = *static_cast <X3D::SFNode*> (field);

					if (sfnode)
					{
						get_model () -> append (iter, OutlineIterType::X3DBaseNode, sfnode);
						selection -> update (field, sfnode);
					}
					else
						get_model () -> append (iter, OutlineIterType::NULL_, field);

					break;
				}
				case X3D::X3DConstants::MFNode:
				{
					const auto mfnode = static_cast <X3D::MFNode*> (field);

					if (mfnode -> empty () and not is_full_expanded (iter))
					{
						expand_routes (iter, field);
						is_full_expanded (iter, iter -> children () .size ());
					}

					size_t i = 0;

					for (auto & value : *mfnode)
					{
						if (value)
						{
							get_model () -> append (iter, OutlineIterType::X3DBaseNode, value, i ++);
							selection -> update (field, value);
						}
						else
							get_model () -> append (iter, OutlineIterType::NULL_, field, i ++);
					}

					break;
				}
				default:
				{
					get_model () -> append (iter, OutlineIterType::X3DFieldValue, field);
					break;
				}
			}

			break;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			const auto & sfnode           = *static_cast <X3D::SFNode*> (get_object (iter));
			const auto   executionContext = dynamic_cast <X3D::X3DExecutionContext*> (sfnode .getValue ());

			// ExternProtos

			if (externProtos and not executionContext -> getExternProtoDeclarations () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Extern Prototypes")));

				size_t i = 0;

				for (auto & externProto : executionContext -> getExternProtoDeclarations ())
					get_model () -> append (iter, OutlineIterType::ExternProtoDeclaration, externProto, i ++);
			}

			// Prototypes

			if (prototypes and not executionContext -> getProtoDeclarations () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Prototypes")));

				size_t i = 0;

				for (auto & prototype : executionContext -> getProtoDeclarations ())
					get_model () -> append (iter, OutlineIterType::ProtoDeclaration, prototype, i ++);
			}

			// Root nodes

			if (not executionContext -> getRootNodes () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Root Nodes")));

				size_t i = 0;

				for (auto & rootNode : executionContext -> getRootNodes ())
				{
					if (rootNode)
						get_model () -> append (iter, OutlineIterType::X3DBaseNode, rootNode, i ++);
					else
						get_model () -> append (iter, OutlineIterType::NULL_, &executionContext -> getRootNodes (), i ++);
				}
			}

			// Imported nodes

			if (importedNodes and not executionContext -> getImportedNodes () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Imported Nodes")));

				for (auto & importedNode : executionContext -> getImportedNodes ())
					get_model () -> append (iter, OutlineIterType::ImportedNode, importedNode .second);
			}

			// Exported nodes

			if (exportedNodes)
			{
				const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext);

				if (scene)
				{
					if (not scene -> getExportedNodes () .empty ())
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Exported Nodes")));

						for (auto & exportedNode : scene -> getExportedNodes ())
							get_model () -> append (iter, OutlineIterType::ExportedNode, exportedNode .second);
					}
				}
			}

			// Add at least one child!!!
			if (not iter -> children () .size ())
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Empty Scene")));

			break;
		}
		case OutlineIterType::X3DBaseNode:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object (iter));

			model_expand_node (sfnode, iter);

			// X3DPrototypeInstance handling

			if (expandPrototypeInstances)
			{
				const auto instance = dynamic_cast <X3D::X3DPrototypeInstance*> (sfnode .getValue ());

				if (instance)
					get_model () -> append (iter, OutlineIterType::X3DExecutionContext, instance);
			}

			// Inline handling

			const auto urlObject = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

			if (urlObject)
			{
				const auto executionContext = urlObject -> getExecutionContext ();

				switch (urlObject -> checkLoadState ()) 
				{
					case X3D::NOT_STARTED_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s not started"), urlObject -> getTypeName () .c_str ())));
						break;
					case X3D::IN_PROGRESS_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s in progess"), urlObject -> getTypeName () .c_str ())));
						break;
					case X3D::COMPLETE_STATE:
					{
						const auto inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

						if (inlineNode)
						{
							if (expandInlineNodes)
							{
								if (inlineNode -> getInternalScene () not_eq inlineNode -> getBrowser () -> getPrivateScene ())
								{
									get_model () -> append (iter, OutlineIterType::X3DExecutionContext, inlineNode -> getInternalScene ());
								}
							}
						}
						else
						   get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s completed"), urlObject -> getTypeName () .c_str ())));

						break;
					}
					case X3D::FAILED_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Failed to load %s"), urlObject -> getTypeName () .c_str ())));
						break;
				}
			}

			break;
		}
		case OutlineIterType::ExternProtoDeclaration:
		{
			const auto & sfnode           = *static_cast <X3D::SFNode*> (get_object (iter));
			const auto   externProto      = dynamic_cast <X3D::ExternProtoDeclaration*> (sfnode .getValue ());
			const auto   executionContext = externProto -> getExecutionContext ();
			const auto   url              = &externProto -> url ();

			model_expand_node (sfnode, iter);

			get_model () -> append (iter, OutlineIterType::X3DField, url);
			get_user_data (url) -> selected |= OUTLINE_SPECIAL;

			try
			{
			   switch (externProto -> checkLoadState () .getValue ())
			   {
					case X3D::NOT_STARTED_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Loading externproto not started")));
						break;
					case X3D::IN_PROGRESS_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Loading externproto in progess")));
						break;
					case X3D::COMPLETE_STATE:
					{
						if (expandExternProtos)
							get_model () -> append (iter, OutlineIterType::ProtoDeclaration, externProto -> getProtoDeclaration ());

						break;
					}
					case X3D::FAILED_STATE:
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Failed to load externproto")));
						break;
				}
			}
			catch (const X3D::X3DError &)
			{ }

			break;
		}
		case OutlineIterType::ProtoDeclaration:
		{
			const auto & sfnode = *static_cast <X3D::SFNode*> (get_object (iter));

			model_expand_node (sfnode, iter);

			get_model () -> append (iter, OutlineIterType::X3DExecutionContext, sfnode);
			break;
		}
		case OutlineIterType::ImportedNode:
		{
			try
			{
				const auto & sfnode       = *static_cast <X3D::SFNode*> (get_object (iter));
				const auto   importedNode = dynamic_cast <X3D::ImportedNode*> (sfnode .getValue ());
				const auto   exportedNode = importedNode -> getExportedNode ();

				model_expand_node (exportedNode, iter);
			}
			catch (...)
			{ }

			break;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto & sfnode       = *static_cast <X3D::SFNode*> (get_object (iter));
				const auto   exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode .getValue ());
				const auto   localNode    = exportedNode -> getLocalNode ();

				model_expand_node (localNode, iter);
			}
			catch (...)
			{ }

			break;
		}
	}
}

void
X3DOutlineTreeView::model_expand_node (const X3D::SFNode & sfnode, const Gtk::TreeModel::iterator & iter)
{
	const auto nodeUserData = get_user_data (iter);

	if (nodeUserData)
	{
		const auto node = sfnode .getValue ();

		if (node)
		{
			// Select visible fields

			const auto fields = get_fields (node);

			if (get_shift_key () or is_full_expanded (iter))
			{
				for (const auto & field : fields)
				{
					get_model () -> append (iter, OutlineIterType::X3DField, field);
					selection -> update (sfnode, field);
				}

				is_full_expanded (iter, true);
			}
			else
			{
				bool visibleFields = false;

				for (const auto & field : fields)
				{
					try
					{
						if (node -> isDefaultValue (field) or not field -> isInitializable ())
						{
							if (not get_model () -> get_input_routes_size (field) and not get_model () -> get_output_routes_size (field))
							{
								if (field -> getReferences () .empty ())
									continue;
							}
						}
					}
					catch (const X3D::Error <X3D::INVALID_NAME> &)
					{
						// This can happen when the field is in the extern proto but not in the proto.
						continue;
					}

					get_model () -> append (iter, OutlineIterType::X3DField, field);
					selection -> update (sfnode, field);
					visibleFields = true;
				}

				if (not visibleFields)
				{
					for (const auto & field : fields)
					{
						get_model () -> append (iter, OutlineIterType::X3DField, field);
						selection -> update (sfnode, field);
					}
				}

				is_full_expanded (iter, false);
			}
		}
	}
}

X3D::FieldDefinitionArray
X3DOutlineTreeView::get_fields (X3D::X3DBaseNode* const node) const
{
	auto       fields            = node -> getPreDefinedFields ();
	const auto userDefinedFields = node -> getUserDefinedFields ();

	// If X3DNode, insert userDefined fields after metadata otherwise prepend.

	if (not fields .empty () and dynamic_cast <X3D::X3DNode*> (node))
		fields .insert (fields .begin () + 1, userDefinedFields .begin (), userDefinedFields .end ());

	else
		fields .insert (fields .begin (), userDefinedFields .begin (), userDefinedFields .end ());

	return fields;
}

void
X3DOutlineTreeView::expand_routes (const Gtk::TreeModel::iterator & iter, X3D::X3DFieldDefinition* field)
{
	for (const auto & route : get_model () -> get_input_routes (field))
	{
		get_model () -> append (iter, OutlineIterType::X3DInputRoute, route);
	}

	for (const auto & route : get_model () -> get_output_routes (field))
	{
		get_model () -> append (iter, OutlineIterType::X3DOutputRoute, route);
	}
}

void
X3DOutlineTreeView::toggle_expand (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path)
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		{
			if (get_shift_key ())
				expand_row (path, false);

			break;
		}
		default:
			break;
	}
}

void
X3DOutlineTreeView::auto_expand (const Gtk::TreeModel::iterator & parent)
{
	disable_shift_key ();

	switch (get_data_type (parent))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::NULL_:
		{
			break;
		}
		case OutlineIterType::X3DExecutionContext:
		case OutlineIterType::X3DField:
		{
			for (const auto & child : parent -> children ())
			{
				switch (get_data_type (child))
				{
					case OutlineIterType::X3DBaseNode:
					case OutlineIterType::ExternProtoDeclaration:
					case OutlineIterType::ProtoDeclaration:
					case OutlineIterType::ImportedNode:
					case OutlineIterType::ExportedNode:
					{
						if (is_expanded (child))
						{
							const auto open_path = get_open_path (child);
							const auto path      = Gtk::TreePath (child);

							if (open_path .empty ())
								expand_row (path, false);
						}

						break;
					}
					default:
						break;
				}
			}

			break;
		}
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		{
			for (const auto & child : parent -> children ())
			{
				switch (get_data_type (child))
				{
					case OutlineIterType::X3DExecutionContext:
					case OutlineIterType::ProtoDeclaration:
					{
						if (is_expanded (child))
							expand_row (Gtk::TreePath (child), false);

						break;
					}
					default:
					{
						const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (child));

						switch (field -> getType ())
						{
							case X3D::X3DConstants::SFNode:
							{
								const auto sfnode = static_cast <X3D::SFNode*> (field);

								if ((field -> isInitializable () and *sfnode) or is_expanded (child))
								{
									expand_row (Gtk::TreePath (child), false);
								}

								break;
							}
							case X3D::X3DConstants::MFNode:
							{
								const auto mfnode = static_cast <X3D::MFNode*> (field);

								if ((field -> isInitializable () and not mfnode -> empty () and mfnode -> size () < 51) or is_expanded (child))
								{
									expand_row (Gtk::TreePath (child), false);
								}

								break;
							}
							default:
							{
								if (is_expanded (child))
									expand_row (Gtk::TreePath (child), false);

								break;
							}
						}

						break;
					}
				}
			}

			break;
		}
	}

	enable_shift_key ();
}

X3DOutlineTreeView::~X3DOutlineTreeView ()
{ }

} // puck
} // titania
