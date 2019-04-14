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

#include "CellRenderer/OutlineCellRenderer.h"
#include "CellRenderer/OutlineSeparator.h"
#include "OutlineRouteGraph.h"
#include "OutlineSelection.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeObserver.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>

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
	            cellrenderer (Gtk::manage (new OutlineCellRenderer (getMasterBrowser (), this))),
				padCellrenderer (Gtk::manage (new Gtk::CellRendererText ())),
	             expandLevel (0),
	            externProtos (false),
	              prototypes (false),
	           importedNodes (false),
	           exportedNodes (false),
	      expandExternProtos (false),
	expandPrototypeInstances (false),
	       expandInlineNodes (false),
	        colorizeTreeView (true),
	               useLocale (true),
	           scrollFreezer (new ScrollFreezer (this)),
	         rootNodesBuffer ()
{
	addChildObjects (rootNodesBuffer);

	rootNodesBuffer .addInterest (&X3DOutlineTreeView::set_rootNodes, this, true);

	// Options

	set_headers_visible (false);
	set_enable_search (false);
	Gtk::TreeView::get_selection () -> set_mode (Gtk::SELECTION_NONE);

	// Columns
	// CellRenderer

	Gtk::TreeViewColumn* treeviewcolumn_name = Gtk::manage (new Gtk::TreeViewColumn (_ ("Scene")));

	treeviewcolumn_name -> set_expand (false);
	treeviewcolumn_name -> pack_start (*cellrenderer, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "tree-data",           OutlineTreeModel::DATA_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "foreground-set",      OutlineTreeModel::SELECTED_COLUMN);
	treeviewcolumn_name -> add_attribute (*cellrenderer, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);

	append_column (*treeviewcolumn_name);

	column = treeviewcolumn_name;

	// Pad column

	Gtk::TreeViewColumn* treeviewcolumn_pad = Gtk::manage (new Gtk::TreeViewColumn ());

	treeviewcolumn_pad -> set_expand (true);
	treeviewcolumn_pad -> pack_start (*padCellrenderer, true);
	treeviewcolumn_pad -> add_attribute (*padCellrenderer, "cell-background-set", OutlineTreeModel::SELECTED_COLUMN);

	append_column (*treeviewcolumn_pad);

	//

	on_style_updated ();
	set_execution_context (executionContext);
}

void
X3DOutlineTreeView::on_style_updated ()
{
	// Color

	const auto selectedForegroundColor = get_style_context () -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto selectedBackgroundColor = get_style_context () -> get_background_color (Gtk::STATE_FLAG_SELECTED);

	cellrenderer    -> property_foreground_rgba ()      = selectedForegroundColor;
	cellrenderer    -> property_cell_background_rgba () = selectedBackgroundColor;
	padCellrenderer -> property_cell_background_rgba () = selectedBackgroundColor;

	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &X3DOutlineTreeView::queue_draw));
}

void
X3DOutlineTreeView::set_colorize_tree_view (const bool value)
{
	colorizeTreeView = value;
	queue_draw ();
}

void
X3DOutlineTreeView::set_use_locale (const bool value)
{
	useLocale = value;
	queue_draw ();
}

Gtk::TreePath
X3DOutlineTreeView::get_path_at_position (const double x, const double y) const
{
	Gtk::TreeViewColumn* column = nullptr;

	return get_path_at_position (x, y, column);
}

Gtk::TreePath
X3DOutlineTreeView::get_path_at_position (const double x, const double y, Gtk::TreeViewColumn* & column) const
{
	Gtk::TreePath path;
	int                  cell_x = 0;
	int                  cell_y = 0;

	get_path_at_pos (x, y, path, column, cell_x, cell_y);

	return path;
}

void
X3DOutlineTreeView::expand_to (X3D::X3DChildObject* const object)
{
	Gtk::TreePath path;

	disable_shift_key ();

	int flags = X3D::TRAVERSE_ROOT_NODES;

	//if (expandPrototypeInstances)
	//	flags |= X3D::TRAVERSE_PROTOTYPE_INSTANCES;

	if (expandInlineNodes)
		flags |= X3D::TRAVERSE_INLINE_NODES;

	if (expandExternProtos and externProtos)
		flags |= X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS | X3D::TRAVERSE_EXTERNPROTO_DECLARATION_SCENE;

	if (prototypes)
		flags |= X3D::TRAVERSE_PROTO_DECLARATIONS | X3D::TRAVERSE_PROTO_DECLARATION_BODY;

	if (importedNodes)
		flags |= X3D::TRAVERSE_IMPORTED_NODES;

	if (exportedNodes)
		flags |= X3D::TRAVERSE_EXPORTED_NODES;

	auto hierarchies = X3D::find (get_execution_context (), object, flags);

	if (not hierarchies .empty ())
	{
		auto & hierarchy = hierarchies .front ();

		hierarchy .erase (hierarchy .begin ());

		if (not hierarchy .empty ())
			expand_to (get_model () -> children (), hierarchy, path);
	}

	enable_shift_key ();
}

bool
X3DOutlineTreeView::expand_to (const Gtk::TreeModel::Children & children, std::vector <X3D::X3DChildObject*> & hierarchy, Gtk::TreePath & path)
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
X3DOutlineTreeView::expand_row (const Gtk::TreePath & path, const bool open_all, const OutlineExpanded & expanded)
{
	const auto iter = get_model () -> get_iter (path);

	if (not get_model () -> iter_is_valid (iter))
		return;

	const auto userData = get_user_data (iter);

	userData -> expanded = expanded;

	if (expanded == OutlineExpanded::UNDEFINED)
		return;

	if (expanded == OutlineExpanded::COLLAPSED)
	{
		disable_shift_key ();
		collapse_row (path);
		enable_shift_key ();
		return;
	}

	disable_shift_key ();
	expand_row (path, open_all);
	enable_shift_key ();
}

void
X3DOutlineTreeView::collapse_row (const Gtk::TreePath & path)
{
	if (not row_expanded (path))
		return;

	Gtk::TreeView::collapse_row (path);
}

void
X3DOutlineTreeView::set_model (const Glib::RefPtr <OutlineTreeModel> & value)
{
	Gtk::TreeView::set_model (value);

	model = value;
}

std::vector <Gtk::TreeIter>
X3DOutlineTreeView::get_iters (X3D::X3DChildObject* const object) const
{
	return get_model () -> get_iters (object);
}

UserDataPtr
X3DOutlineTreeView::get_user_data (const Gtk::TreeIter & iter) const
{
	const auto userData = get_model () -> get_user_data (iter);

	if (get_data_type (iter) == OutlineIterType::X3DExecutionContext)
	{
		if (not userData -> userData)
			userData -> userData .reset (new UserData ());

		return userData -> userData;
	}

	return userData;
}

OutlineIterType
X3DOutlineTreeView::get_data_type (const Gtk::TreeIter & iter) const
{
	return get_model () -> get_data_type (iter);
}

X3D::X3DChildObject*
X3DOutlineTreeView::get_object (const Gtk::TreeIter & iter) const
{
	return get_model () -> get_object (iter);
}

size_t
X3DOutlineTreeView::get_index (const Gtk::TreeIter & iter) const
{
	return get_model () -> get_data (iter) -> get_index ();
}

void
X3DOutlineTreeView::set_open_path (const Gtk::TreeIter & iter, const Gtk::TreePath & value)
{
	const auto userData = get_user_data (iter);

	if (userData)
		userData -> openPath = value;
}

Gtk::TreePath
X3DOutlineTreeView::get_open_path (const Gtk::TreeIter & iter) const
{
	const auto userData = get_user_data (iter);

	if (userData)
		return userData -> openPath;

	return Gtk::TreePath ();
}

void
X3DOutlineTreeView::set_expanded (const Gtk::TreeIter & iter, const OutlineExpanded value)
{
	const auto userData = get_user_data (iter);

	if (userData)
		userData -> expanded = value;
}

OutlineExpanded
X3DOutlineTreeView::get_expanded (const Gtk::TreeIter & iter) const
{
	const auto userData = get_user_data (iter);

	if (userData)
		return userData -> expanded;

	return OutlineExpanded::UNDEFINED;
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

X3D::X3DScene*
X3DOutlineTreeView::get_scene (X3D::X3DExecutionContext* const executionContext) const
{
	if (executionContext -> isType ({ X3D::X3DConstants::X3DScene }))
		return dynamic_cast <X3D::X3DScene*> (executionContext);

	return executionContext -> getScene ();
}

void
X3DOutlineTreeView::set_execution_context (const X3D::X3DExecutionContextPtr & executionContext)
{
	//__LOG__ << std::endl;

	// Remove model.

	X3D::X3DScenePtr scene (get_execution_context ());

	for (const auto & child : get_model () -> children ())
	{
		routeGraph -> collapse_tree (child);
		treeObserver -> unwatch_tree (child, false);
	}

	get_execution_context () -> getScene () -> units_changed () .removeInterest (&X3DOutlineTreeView::queue_draw, this);

	get_execution_context () -> getRootNodes ()          .removeInterest (&X3DOutlineTreeView::update, this);
	get_execution_context () -> importedNodes_changed () .removeInterest (&X3DOutlineTreeView::update, this);
	get_execution_context () -> prototypes_changed ()    .removeInterest (&X3DOutlineTreeView::update, this);
	get_execution_context () -> externProtos_changed ()  .removeInterest (&X3DOutlineTreeView::update, this);

	if (scene)
		scene -> exportedNodes_changed () .removeInterest (&X3DOutlineTreeView::update, this);

	// Set model.

	scene = executionContext;

	set_model (OutlineTreeModel::create (executionContext));
	get_model () -> set_show_all_routes (get_expand_prototype_instances ());

	get_execution_context () -> getScene () -> units_changed () .addInterest (&X3DOutlineTreeView::queue_draw, this);

	executionContext -> getRootNodes ()          .addInterest (&X3DOutlineTreeView::update, this);
	executionContext -> importedNodes_changed () .addInterest (&X3DOutlineTreeView::update, this);
	executionContext -> prototypes_changed ()    .addInterest (&X3DOutlineTreeView::update, this);
	executionContext -> externProtos_changed ()  .addInterest (&X3DOutlineTreeView::update, this);

	if (scene)
		scene -> exportedNodes_changed () .addInterest (&X3DOutlineTreeView::update, this);

	set_rootNodes (false);
}

const X3D::X3DExecutionContextPtr &
X3DOutlineTreeView::get_execution_context () const
{
	return get_model () -> get_execution_context ();
}

void
X3DOutlineTreeView::set_rootNodes (const bool reopen)
{
	//__LOG__ << std::endl;

	// Freeze scroll.

	getScrollFreezer () -> freeze ();

	// Determine open paths.

	std::map <size_t, std::tuple <size_t, bool, OutlineExpanded>> opened;

	if (reopen)
	{
		for (auto & iter : get_model () -> children ())
			get_opened_objects (iter, opened);
	}
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
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Extern Prototypes")), 0, false);

		size_t i = 0;

		for (auto & externProto : executionContext -> getExternProtoDeclarations ())
		{
			const auto iter = get_model () -> append (OutlineIterType::ExternProtoDeclaration, externProto, i ++, false);

			treeObserver -> watch_child (iter, get_model () -> get_path (iter));
		}
	}

	// Prototypes

	if (prototypes and not executionContext -> getProtoDeclarations () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Prototypes")), 0, false);

		size_t i = 0;

		for (auto & prototype : executionContext -> getProtoDeclarations ())
		{
			const auto iter = get_model () -> append (OutlineIterType::ProtoDeclaration, prototype, i ++, false);

			treeObserver -> watch_child (iter, get_model () -> get_path (iter));
		}
	}

	// Root nodes

	if (not executionContext -> getRootNodes () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Root Nodes")), 0, false);

		size_t i = 0;

		for (auto & rootNode : executionContext -> getRootNodes ())
		{
			Gtk::TreeIter iter;

			if (rootNode)
				iter = get_model () -> append (OutlineIterType::X3DBaseNode, rootNode, i ++, getBrowserWindow () -> getSelection () -> isSelected (rootNode));
			else
				iter = get_model () -> append (OutlineIterType::NULL_, &executionContext -> getRootNodes (), i ++, false);

			treeObserver -> watch_child (iter, get_model () -> get_path (iter));
		}
	}

	// Imported nodes

	if (importedNodes and not executionContext -> getImportedNodes () .empty ())
	{
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Imported Nodes")), 0, false);

		for (auto & importedNode : executionContext -> getImportedNodes ())
		{
			const auto iter = get_model () -> append (OutlineIterType::ImportedNode, importedNode .second, 0, false);

			treeObserver -> watch_child (iter, get_model () -> get_path (iter));
		}
	}

	// Exported nodes

	if (exportedNodes)
	{
		const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext .getValue ());

		if (scene)
		{
			if (not scene -> getExportedNodes () .empty ())
			{
				get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Exported Nodes")), 0, false);

				for (auto & exportedNode : scene -> getExportedNodes ())
				{
					const auto iter = get_model () -> append (OutlineIterType::ExportedNode, exportedNode .second, 0, false);

					treeObserver -> watch_child (iter, get_model () -> get_path (iter));
				}
			}
		}
	}

	// Add at least one child!!!
	if (not get_model () -> children () .size ())
		get_model () -> append (OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Empty Scene")), 0, false);

	set_model (get_model ());

	// Expand.

	if (reopen)
	{
		disable_shift_key ();

		for (auto & iter : get_model () -> children ())
			reopen_objects (iter, opened);

		enable_shift_key ();
	}
}

void
X3DOutlineTreeView::get_opened_objects (const Gtk::TreeIter & parent, std::map <size_t, std::tuple <size_t, bool, OutlineExpanded>> & opened)
{
	const auto id    = get_reopen_id (parent);
	auto &     tuple = opened [id];

	if (std::get <1> (tuple))
		return;

	++ std::get <0> (tuple);

	if (not row_expanded (get_model () -> get_path (parent)))
		return;

	std::get <1> (tuple) = true; // found
	std::get <2> (tuple) = get_expanded (parent);

	for (const auto & child : parent -> children ())
		get_opened_objects (child, opened);
}

void
X3DOutlineTreeView::reopen_objects (const Gtk::TreeIter & parent, std::map <size_t, std::tuple <size_t, bool, OutlineExpanded>> & opened)
{
	try
	{
		const auto id    = get_reopen_id (parent);
		auto &     tuple = opened .at (id);
	
		if (-- std::get <0> (tuple) == 0)
		{
			if (std::get <1> (tuple))
			{
				expand_row (get_model () -> get_path (parent), false, std::get <2> (tuple));

				for (const auto & child : parent -> children ())
					reopen_objects (child, opened);
			}
		}
	}
	catch (const std::out_of_range & error)
	{ }
}

size_t
X3DOutlineTreeView::get_reopen_id (const Gtk::TreeIter & iter)
{
	const auto object = get_object (iter);

	switch (get_data_type (iter))
	{
		case OutlineIterType::NULL_:
		{
			return 0;
		}
		case OutlineIterType::X3DField:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		{
			return object -> getId ();
		}
		case OutlineIterType::X3DBaseNode:
		case OutlineIterType::ExternProtoDeclaration:
		case OutlineIterType::ProtoDeclaration:
		case OutlineIterType::ImportedNode:
		case OutlineIterType::ExportedNode:
		case OutlineIterType::Separator:
		{
			return static_cast <X3D::SFNode &> (*object) -> getId ();
		}
		case OutlineIterType::X3DExecutionContext:
		{
			return size_t (dynamic_cast <X3D::X3DExecutionContext*> (static_cast <X3D::SFNode &> (*object) .getValue ()));
		}
	}

	return 0;
}

void
X3DOutlineTreeView::update ()
{
	rootNodesBuffer = X3D::SFTime::now ();
}

void
X3DOutlineTreeView::on_row_activated (const Gtk::TreePath & path, Gtk::TreeViewColumn* column)
{
	select_node (get_model () -> get_iter (path), path);
}

void
X3DOutlineTreeView::select_node (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	selection -> set_select_multiple (get_shift_key ());

	switch (get_data_type (iter))
	{
		case OutlineIterType::X3DBaseNode:
		{
			const auto & localNode = *static_cast <X3D::SFNode*> (get_object (iter));

			if (not localNode)
				break;

			if (get_scene (localNode -> getExecutionContext ()) not_eq get_scene (get_execution_context ()))
				break;

			if (localNode -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
				break;

			selection -> select (localNode);
			break;
		}
		case OutlineIterType::ExportedNode:
		{
			try
			{
				const auto sfnode       = static_cast <X3D::SFNode*> (get_object (iter));
				const auto exportedNode = dynamic_cast <X3D::ExportedNode*> (sfnode -> getValue ());
				const auto localNode    = exportedNode -> getLocalNode ();

				if (not localNode)
					break;
	
				if (get_scene (localNode -> getExecutionContext ()) not_eq get_scene (get_execution_context ()))
					break;
	
				if (localNode -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DPrototypeInstance }))
					break;

				selection -> select (localNode);
				break;
			}
			catch (...)
			{
				break;
			}
		}
		default:
			break;
	}
}

bool
X3DOutlineTreeView::on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	collapse_clone (iter);

	model_expand_row (iter); // (model expand_row)

	// Return false to allow expansion, true to reject.
	return false;
}

void
X3DOutlineTreeView::on_row_expanded (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	//__LOG__ << path .to_string () << std::endl;

	// Set expanded first to prevent loop with clones.

	if (get_expanded (iter) == OutlineExpanded::UNDEFINED || get_expanded (iter) == OutlineExpanded::COLLAPSED)
		set_expanded (iter, OutlineExpanded::CHANGED);

	set_open_path (iter, path);

	routeGraph -> expand (iter);
	treeObserver -> watch (iter, path);

	columns_autosize (); // Resizes all columns to their optimal width.

	auto_expand (iter);
}

bool
X3DOutlineTreeView::on_test_collapse_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	// Return false to allow collapse, true to reject.
	return false;
}

void
X3DOutlineTreeView::on_row_collapsed (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	//__LOG__ << path .to_string () << std::endl;

	const auto expanded = get_expanded (iter);

	set_expanded (iter, OutlineExpanded::COLLAPSED);

	routeGraph -> collapse_tree (iter);
	treeObserver -> unwatch_tree (iter);

	get_model () -> clear (iter);

	toggle_expand (iter, path, expanded);

	columns_autosize ();
}

void
X3DOutlineTreeView::collapse_clone (const Gtk::TreeIter & iter)
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
X3DOutlineTreeView::model_expand_row (const Gtk::TreeIter & iter)
{
	switch (get_data_type (iter))
	{
		case OutlineIterType::Separator:
		case OutlineIterType::X3DInputRoute:
		case OutlineIterType::X3DOutputRoute:
		case OutlineIterType::X3DFieldValue:
		case OutlineIterType::NULL_:
		{
			break;
		}
		case OutlineIterType::X3DField:
		{
			const auto field = static_cast <X3D::X3DFieldDefinition*> (get_object (iter));

			if (get_shift_key () or get_expanded (iter) == OutlineExpanded::FULL)
			{
				expand_routes (iter, field);
				set_expanded (iter, iter -> children () .size () ? OutlineExpanded::FULL : OutlineExpanded::CHANGED);
			}
			else
			{
				set_expanded (iter, OutlineExpanded::CHANGED);
			}

			switch (field -> getType ())
			{
				case X3D::X3DConstants::SFNode:
				{
					const auto & sfnode = *static_cast <X3D::SFNode*> (field);

					if (sfnode)
					{
						get_model () -> append (iter, OutlineIterType::X3DBaseNode, sfnode, 0, get_model () -> get_data (iter) -> getSelected () or getBrowserWindow () -> getSelection () -> isSelected (sfnode));
					}
					else
					{
						get_model () -> append (iter, OutlineIterType::NULL_, field, 0, get_model () -> get_data (iter) -> getSelected ());
					}

					break;
				}
				case X3D::X3DConstants::MFNode:
				{
					const auto mfnode = static_cast <X3D::MFNode*> (field);

					if (mfnode -> empty () and get_expanded (iter) != OutlineExpanded::FULL)
					{
						expand_routes (iter, field);
						set_expanded (iter, iter -> children () .size () ? OutlineExpanded::FULL : OutlineExpanded::CHANGED);
					}

					size_t i = 0;

					for (auto & value : *mfnode)
					{
						if (value)
						{
							get_model () -> append (iter, OutlineIterType::X3DBaseNode, value, i ++, get_model () -> get_data (iter) -> getSelected () or getBrowserWindow () -> getSelection () -> isSelected (value));
						}
						else
							get_model () -> append (iter, OutlineIterType::NULL_, field, i ++, get_model () -> get_data (iter) -> getSelected ());
					}

					break;
				}
				default:
				{
					get_model () -> append (iter, OutlineIterType::X3DFieldValue, field, 0, get_model () -> get_data (iter) -> getSelected ());
					break;
				}
			}

			break;
		}
		case OutlineIterType::X3DExecutionContext:
		{
			const auto & sfnode           = *static_cast <X3D::SFNode*> (get_object (iter));
			const auto   executionContext = dynamic_cast <X3D::X3DExecutionContext*> (sfnode .getValue ());

			if (executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
				executionContext -> realize ();

			// ExternProtos

			if (externProtos and not executionContext -> getExternProtoDeclarations () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Extern Prototypes")), 0, get_model () -> get_data (iter) -> getSelected ());

				size_t i = 0;

				for (auto & externProto : executionContext -> getExternProtoDeclarations ())
					get_model () -> append (iter, OutlineIterType::ExternProtoDeclaration, externProto, i ++, get_model () -> get_data (iter) -> getSelected ());
			}

			// Prototypes

			if (prototypes and not executionContext -> getProtoDeclarations () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Prototypes")), 0, get_model () -> get_data (iter) -> getSelected ());

				size_t i = 0;

				for (auto & prototype : executionContext -> getProtoDeclarations ())
					get_model () -> append (iter, OutlineIterType::ProtoDeclaration, prototype, i ++, get_model () -> get_data (iter) -> getSelected ());
			}

			// Root nodes

			if (not executionContext -> getRootNodes () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Root Nodes")), 0, get_model () -> get_data (iter) -> getSelected ());

				size_t i = 0;

				for (auto & rootNode : executionContext -> getRootNodes ())
				{
					if (rootNode)
						get_model () -> append (iter, OutlineIterType::X3DBaseNode, rootNode, i ++, get_model () -> get_data (iter) -> getSelected () or getBrowserWindow () -> getSelection () -> isSelected (rootNode));
					else
						get_model () -> append (iter, OutlineIterType::NULL_, &executionContext -> getRootNodes (), i ++, get_model () -> get_data (iter) -> getSelected ());
				}
			}

			// Imported nodes

			if (importedNodes and not executionContext -> getImportedNodes () .empty ())
			{
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Imported Nodes")), 0, get_model () -> get_data (iter) -> getSelected ());

				for (auto & importedNode : executionContext -> getImportedNodes ())
					get_model () -> append (iter, OutlineIterType::ImportedNode, importedNode .second, 0, get_model () -> get_data (iter) -> getSelected ());
			}

			// Exported nodes

			if (exportedNodes)
			{
				const auto scene = dynamic_cast <X3D::X3DScene*> (executionContext);

				if (scene)
				{
					if (not scene -> getExportedNodes () .empty ())
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Exported Nodes")), 0, get_model () -> get_data (iter) -> getSelected ());

						for (auto & exportedNode : scene -> getExportedNodes ())
							get_model () -> append (iter, OutlineIterType::ExportedNode, exportedNode .second, 0, get_model () -> get_data (iter) -> getSelected ());
					}
				}
			}

			// Add at least one child!!!
			if (not iter -> children () .size ())
				get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Empty Scene")), 0, get_model () -> get_data (iter) -> getSelected ());

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
					get_model () -> append (iter, OutlineIterType::X3DExecutionContext, instance, 0, get_model () -> get_data (iter) -> getSelected ());
			}

			// Inline handling

			const auto urlObject = dynamic_cast <X3D::X3DUrlObject*> (sfnode .getValue ());

			if (urlObject)
			{
				const auto executionContext = urlObject -> getExecutionContext ();

				switch (urlObject -> checkLoadState ()) 
				{
					case X3D::NOT_STARTED_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s not started"), urlObject -> getTypeName () .c_str ())), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
					case X3D::IN_PROGRESS_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s in progess"), urlObject -> getTypeName () .c_str ())), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
					case X3D::COMPLETE_STATE:
					{
						const auto inlineNode = dynamic_cast <X3D::Inline*> (sfnode .getValue ());

						if (inlineNode)
						{
							if (expandInlineNodes)
							{
								if (inlineNode -> checkLoadState () == X3D::COMPLETE_STATE)
								{
									get_model () -> append (iter, OutlineIterType::X3DExecutionContext, inlineNode -> getInternalScene (), 0, get_model () -> get_data (iter) -> getSelected ());
								}
							}
						}
						else
						{
						   get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Loading %s completed"), urlObject -> getTypeName () .c_str ())), 0, get_model () -> get_data (iter) -> getSelected ());
						}

						break;
					}
					case X3D::FAILED_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, basic::sprintf (_ ("Failed to load %s"), urlObject -> getTypeName () .c_str ())), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
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

			get_model () -> append (iter, OutlineIterType::X3DField, url, 0, get_model () -> get_data (iter) -> getSelected ());
			url -> getUserData <UserData> () -> selected .set (OUTLINE_SPECIAL);

			try
			{
			   switch (externProto -> checkLoadState () .getValue ())
			   {
					case X3D::NOT_STARTED_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Loading externproto not started")), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
					case X3D::IN_PROGRESS_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Loading externproto in progess")), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
					case X3D::COMPLETE_STATE:
					{
						if (expandExternProtos)
							get_model () -> append (iter, OutlineIterType::ProtoDeclaration, externProto -> getProtoDeclaration (), 0, get_model () -> get_data (iter) -> getSelected ());

						break;
					}
					case X3D::FAILED_STATE:
					{
						get_model () -> append (iter, OutlineIterType::Separator, new OutlineSeparator (executionContext, _ ("Failed to load externproto")), 0, get_model () -> get_data (iter) -> getSelected ());
						break;
					}
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

			get_model () -> append (iter, OutlineIterType::X3DExecutionContext, sfnode, 0, get_model () -> get_data (iter) -> getSelected ());
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
X3DOutlineTreeView::model_expand_node (const X3D::SFNode & sfnode, const Gtk::TreeIter & iter)
{
	const auto nodeUserData = get_user_data (iter);

	if (nodeUserData)
	{
		const auto node = sfnode .getValue ();

		if (node)
		{
			// Select visible fields

			const auto fields = get_fields (node);

			if (get_shift_key () or get_expanded (iter) == OutlineExpanded::FULL)
			{
				for (const auto & field : fields)
				{
					get_model () -> append (iter, OutlineIterType::X3DField, field, 0, get_model () -> get_data (iter) -> getSelected ());
				}

				set_expanded (iter, OutlineExpanded::FULL);
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

					get_model () -> append (iter, OutlineIterType::X3DField, field, 0, get_model () -> get_data (iter) -> getSelected ());
					visibleFields = true;
				}

				if (not visibleFields)
				{
					for (const auto & field : fields)
					{
						get_model () -> append (iter, OutlineIterType::X3DField, field, 0, get_model () -> get_data (iter) -> getSelected ());
					}
				}

				set_expanded (iter, OutlineExpanded::CHANGED);
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
X3DOutlineTreeView::expand_routes (const Gtk::TreeIter & iter, X3D::X3DFieldDefinition* field)
{
	for (const auto & route : get_model () -> get_input_routes (field))
	{
		get_model () -> append (iter, OutlineIterType::X3DInputRoute, route, 0, get_model () -> get_data (iter) -> getSelected ());
	}

	for (const auto & route : get_model () -> get_output_routes (field))
	{
		get_model () -> append (iter, OutlineIterType::X3DOutputRoute, route, 0, get_model () -> get_data (iter) -> getSelected ());
	}
}

void
X3DOutlineTreeView::toggle_expand (const Gtk::TreeIter & iter, const Gtk::TreePath & path, const OutlineExpanded & expanded)
{
	if (not get_shift_key ())
		return;

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
			expand_row (path, false, expanded == OutlineExpanded::CHANGED ? OutlineExpanded::FULL : OutlineExpanded::CHANGED);
			break;
		}
		default:
			break;
	}
}

void
X3DOutlineTreeView::auto_expand (const Gtk::TreeIter & parent)
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
				const auto expanded = get_expanded (child);

				if (expanded == OutlineExpanded::UNDEFINED)
					continue;

				if (expanded == OutlineExpanded::COLLAPSED)
					continue;

				switch (get_data_type (child))
				{
					case OutlineIterType::X3DBaseNode:
					case OutlineIterType::ExternProtoDeclaration:
					case OutlineIterType::ProtoDeclaration:
					case OutlineIterType::ImportedNode:
					case OutlineIterType::ExportedNode:
					{
						const auto open_path = get_open_path (child);
						const auto path      = Gtk::TreePath (child);

						if (open_path .empty ())
							expand_row (path, false, expanded);

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
				const auto expanded = get_expanded (child);

				if (expanded == OutlineExpanded::COLLAPSED)
					continue;

				switch (get_data_type (child))
				{
					case OutlineIterType::X3DExecutionContext:
					case OutlineIterType::ProtoDeclaration:
					{
						expand_row (Gtk::TreePath (child), false, expanded);
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

								if (expanded == OutlineExpanded::CHANGED or expanded == OutlineExpanded::FULL)
								{
									expand_row (Gtk::TreePath (child), false, expanded);
								}
								else if (field -> isInitializable () and *sfnode)
								{
									expand_row (Gtk::TreePath (child), false, OutlineExpanded::CHANGED);
								}

								break;
							}
							case X3D::X3DConstants::MFNode:
							{
								const auto mfnode = static_cast <X3D::MFNode*> (field);

								if (expanded == OutlineExpanded::CHANGED or expanded == OutlineExpanded::FULL)
								{
									expand_row (Gtk::TreePath (child), false, expanded);
								}
								else if (field -> isInitializable () and not mfnode -> empty () and mfnode -> size () < 51)
								{
									expand_row (Gtk::TreePath (child), false, OutlineExpanded::CHANGED);
								}

								break;
							}
							default:
							{
								if (expanded == OutlineExpanded::CHANGED or expanded == OutlineExpanded::FULL)
									expand_row (Gtk::TreePath (child), false, expanded);

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
