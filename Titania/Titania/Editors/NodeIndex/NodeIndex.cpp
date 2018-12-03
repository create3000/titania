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

#include "NodeIndex.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/NamedNode.h>
#include <Titania/X3D/Prototype/X3DProtoDeclarationNode.h>

#include <Titania/String.h>

namespace titania {
namespace puck {

struct NodeIndex::Search
{
	static constexpr int TYPE_NAME = 0;
};

NodeIndex::NodeIndex (X3DBrowserWindow* const browserWindow) :
	     X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DNodeIndexInterface (get_ui ("Editors/NodeIndex.glade")),
	     executionContext (getMasterBrowser () -> getExecutionContext ()),
	            protoNode (),
	                nodes (),
	                 node (),
	        refreshBuffer (),
	            indexType (NAMED_NODES_INDEX),
	           showWidget (false),
	         observeNodes (false),
	               select (true),
	    displayProtoNodes (false),
	                types (),
	            nodeTypes (),
	              columns (),
	        scrollFreezer (new ScrollFreezer (getTreeView ()))
{
	addChildObjects (executionContext,
	                 protoNode,
	                 nodes,
	                 node,
	                 refreshBuffer);

	columns .emplace_back (getImageColumn ());
	columns .emplace_back (getTypeNameColumn ());
	columns .emplace_back (getNameColumn ());
	columns .emplace_back (getImportColumn ());
	columns .emplace_back (getExportColumn ());

	setup ();
}

void
NodeIndex::initialize ()
{
	X3DNodeIndexInterface::initialize ();

	// Initialize tree view:

	getTreeModelSort () -> set_sort_func (Columns::TYPE_NAME, sigc::mem_fun (this, &NodeIndex::on_compare_type_name));
	getTreeModelSort () -> set_sort_func (Columns::NAME,      sigc::mem_fun (this, &NodeIndex::on_compare_name));

	// Initialize SearchEntryCompletion:

	for (const auto & pair : getCurrentBrowser () -> getSupportedNodes ())
	{
		const auto node = pair .second;
		const auto row  = getSearchListStore () -> append ();

		row -> set_value (Search::TYPE_NAME, node -> getTypeName ());

		nodeTypes .emplace (node -> getTypeName (), node -> getType () .back ());
	}

	getSearchEntry () .grab_focus ();
}

void
NodeIndex::configure ()
{
	if (getConfig () -> hasItem ("sortColumn"))
	{
		const auto sortColumn = getConfig () -> getItem <int32_t> ("sortColumn");
		const auto sortOrder  = getConfig () -> getItem <int32_t> ("sortOrder");

		getTreeModelSort () -> set_sort_column (sortColumn, Gtk::SortType (sortOrder));
	}
	else
	{
		getTreeModelSort () -> set_sort_column (Columns::NAME,      Gtk::SORT_ASCENDING);
		getTreeModelSort () -> set_sort_column (Columns::TYPE_NAME, Gtk::SORT_ASCENDING);
	}
}

void
NodeIndex::on_map () 
{
	refreshBuffer        .addInterest (&NodeIndex::set_refresh,          this);
	getCurrentContext () .addInterest (&NodeIndex::set_executionContext, this);

	set_executionContext (getCurrentContext ());
}

void
NodeIndex::on_unmap () 
{
	refreshBuffer        .removeInterest (&NodeIndex::set_refresh, this);
	getCurrentContext () .removeInterest (&NodeIndex::set_executionContext, this);

	set_executionContext (nullptr);
}

int
NodeIndex::getIndexColumn () const
{
	return Columns::INDEX;
}

void
NodeIndex::on_column_clicked ()
{
	int  sortColumn = 0;
	auto sortOrder  = Gtk::SortType ();

	getTreeModelSort () -> get_sort_column_id (sortColumn, sortOrder);

	getConfig () -> setItem ("sortOrder",  int32_t (sortOrder));
	getConfig () -> setItem ("sortColumn", sortColumn);
}

int
NodeIndex::on_compare_type_name (const Gtk::TreeModel::iterator & lhs, const Gtk::TreeModel::iterator & rhs)
{
	bool lhsProto = false;
	bool rhsProto = false;

	lhs -> get_value (Columns::PROTO, lhsProto);
	rhs -> get_value (Columns::PROTO, rhsProto);

	if (lhsProto xor rhsProto)
		return lhsProto ? -1 : 1;

	std::string lhsString;
	std::string rhsString;

	lhs -> get_value (Columns::TYPE_NAME, lhsString);
	rhs -> get_value (Columns::TYPE_NAME, rhsString);

	if (lhsString == rhsString)
		return 0;

	return lhsString < rhsString ? -1 : 1;
}

int
NodeIndex::on_compare_name (const Gtk::TreeModel::iterator & lhs, const Gtk::TreeModel::iterator & rhs)
{
	bool lhsProto = false;
	bool rhsProto = false;

	lhs -> get_value (Columns::PROTO, lhsProto);
	rhs -> get_value (Columns::PROTO, rhsProto);

	if (lhsProto xor rhsProto)
		return lhsProto ? -1 : 1;

	std::string lhsString;
	std::string rhsString;

	lhs -> get_value (Columns::NAME, lhsString);
	rhs -> get_value (Columns::NAME, rhsString);

	if (lhsString == rhsString)
		return 0;

	return basic::naturally_compare (lhsString, rhsString) ? -1 : 1;
}

void
NodeIndex::refresh ()
{
	refreshBuffer = X3D::SFTime::now ();
}

void
NodeIndex::set_refresh ()
{
	if (executionContext)
	{
		switch (indexType)
		{
			case NAMED_NODES_INDEX:
			{
				setNodes (getCurrentNodes ());
				break;
			}
			case TYPE_INDEX:
			{
				setNodes (getCurrentNodes (types));
				break;
			}
			case ANIMATION_INDEX:
			{
				X3D::MFNode animations;
	
				for (const auto & basenode : getCurrentNodes ({ X3D::X3DConstants::Group }))
				{
					X3D::X3DPtr <X3D::X3DNode> node (basenode);
	
					if (node -> getMetaData <int32_t> ("/Animation/duration"))
						animations .emplace_back (basenode);
				}
	
				setNodes (std::move (animations));
				break;
			}
			case PROTO_INDEX:
			{
				X3D::MFNode nodes;
	
				if (protoNode)
				{
					for (const auto & node : protoNode -> getInstances ())
					{
						if (node -> getExecutionContext () == executionContext)
							nodes .emplace_back (node);
					}
				}
	
				setNodes (std::move (nodes));
				break;
			}
		}
	}
	else
	{
		setNodes ({ });
	}
}

void
NodeIndex::setShowWidget (const bool value)
{
	showWidget = value;

	if (showWidget)
	{
		getHeaderBar () .set_visible (false);
		getFooterBox () .set_visible (false);
	}
	else
	{
		getHeaderBar () .set_visible (true);
		getFooterBox () .set_visible (true);
	}
}

void
NodeIndex::setDisplayProtoNodes (const bool value)
{
	displayProtoNodes = value;
	refresh ();
}

void
NodeIndex::setNamedNodes ()
{
	if (executionContext)
		executionContext -> sceneGraph_changed () .removeInterest (&NodeIndex::refresh, this);

	indexType = NAMED_NODES_INDEX;

	refresh ();
}

void
NodeIndex::setTypes (const std::set <X3D::X3DConstants::NodeType> & value)
{
	if (executionContext)
		executionContext -> sceneGraph_changed () .addInterest (&NodeIndex::refresh, this);

	indexType = TYPE_INDEX;
	types     = value;

	refresh ();
}

void
NodeIndex::setAnimations ()
{
	if (executionContext)
		executionContext -> sceneGraph_changed () .addInterest (&NodeIndex::refresh, this);

	indexType = ANIMATION_INDEX;

	refresh ();
}

void
NodeIndex::setProto (const X3D::X3DPtr <X3D::X3DProtoDeclarationNode> & value)
{
	if (executionContext)
		executionContext -> sceneGraph_changed () .addInterest (&NodeIndex::refresh, this);

	protoNode = value;
	indexType = PROTO_INDEX;

	refresh ();
}

bool
NodeIndex::setSelection (const X3D::SFNode & selection)
{
	node .set (selection);

	const auto path = getPath (node);

	if (path .empty ())
	{
		getTreeView () .get_selection () -> unselect_all ();
		return false;
	}

	getTreeView () .get_selection () -> select (path);
	return true;
}

void
NodeIndex::scrollToRow (const X3D::SFNode & node)
{
	const auto path = getPath (node);

	if (path .empty ())
		return;

	getTreeView () .scroll_to_row (path, 2 - math::phi <double>);
}

Gtk::TreePath
NodeIndex::getPath (const X3D::SFNode & node)
{
	const auto iter = std::find (nodes .begin (), nodes .end (), node);

	if (iter == nodes .end ())
		return Gtk::TreePath ();

	Gtk::TreePath childPath;

	childPath .push_back (iter - nodes .begin ());

	return getTreeModelSort () -> convert_child_path_to_path (childPath);
}

void
NodeIndex::setNodes (X3D::MFNode && value)
{
	static const std::string empty_string;
	static const std::string document_import ("document-import");
	static const std::string document_export ("document-export");

	if (value == nodes)
		return;

	for (const auto & node : nodes)
	{
		node -> getExecutionContext () -> name_changed () .removeInterest (&NodeIndex::set_name, this);
		node -> name_changed () .removeInterest (&NodeIndex::set_name, this);
	}

	if (observeNodes)
	{
		for (const auto & node : nodes)
			node -> removeInterest (&NodeIndex::updateRow, this);
	}

	nodes = std::move (value);

	// Fill model.

	scrollFreezer -> freeze ();

	getTreeView () .unset_model ();
	getListStore () -> clear ();

	const auto importingInlines = getImportingInlines ();
	const auto exportedNodes    = getExportedNodes ();
	size_t     index            = 0;

	for (const auto & node : nodes)
	{
		const auto row = getListStore () -> append ();

		row -> set_value (Columns::IMAGE,          getName () + basic::to_string (index));
		row -> set_value (Columns::TYPE_NAME,      node -> getTypeName ());
		row -> set_value (Columns::NAME,           getNameFromNode (node));
		row -> set_value (Columns::IMPORTED_NODES, importingInlines .count (node) ? document_import : empty_string);
		row -> set_value (Columns::EXPORTED_NODES, exportedNodes .count (node)    ? document_export : empty_string);
		row -> set_value (Columns::PROTO,          node -> getExecutionContext () -> isType ({ X3D::X3DConstants::ProtoDeclaration }));
		row -> set_value (Columns::INDEX,          index);

		node -> getExecutionContext () -> name_changed () .addInterest (&NodeIndex::set_name, this, index);
		node -> name_changed () .addInterest (&NodeIndex::set_name, this, index);

		if (observeNodes)
			node -> addInterest (&NodeIndex::updateRow, this, index);

		++ index;
	}

	getTreeView () .set_model (getTreeModelSort ());
	getTreeView () .set_search_column (Columns::NAME);

	// If the current selection is not in nodes, send event.

	const bool selected = setSelection (node);

	if (node)
	{
		if (not selected)
			node .set (nullptr);
	}
}

std::string
NodeIndex::getNameFromNode (const X3D::SFNode & node) const
{
	const auto nodeName  = Glib::Markup::escape_text (getNodeName (node));
	const auto protoPath = getProtoPath (X3D::X3DExecutionContextPtr (node -> getExecutionContext ()));

	if (protoPath .empty ())
		return nodeName;

	return "<b>" + basic::join (protoPath .begin (), protoPath .end (), ".") + "</b>." + nodeName;
}

/***
 *  Returns a list of all nodes where type is @a types.
 */
X3D::MFNode
NodeIndex::getCurrentNodes (const std::set <X3D::X3DConstants::NodeType> & types)
{
	// Find nodes

	X3D::MFNode nodes;

	if (inPrototypeInstance ())
		return nodes;

	if (displayProtoNodes)
		getCurrentProtoNodes (executionContext, nodes);

	X3D::traverse (executionContext -> getRootNodes (), [&] (X3D::SFNode & node)
	{
		if (node -> getExecutionContext () not_eq executionContext)
			return true;
		
		for (const auto & type: basic::make_reverse_range (node -> getType ()))
		{
			if (types .count (type))
			{
				nodes .emplace_back (node);
				break;
			}
		}
		
		return true;
	});

	return nodes;
}

void
NodeIndex::getCurrentProtoNodes (X3D::X3DExecutionContext* const executionContext, X3D::MFNode & nodes)
{
	for (const auto & proto : executionContext -> getProtoDeclarations ())
	{
		getCurrentProtoNodes (proto, nodes);

		X3D::traverse (proto, [&] (X3D::SFNode & node)
		{
			for (const auto & type: basic::make_reverse_range (node -> getType ()))
			{
				if (types .count (type))
				{
					nodes .emplace_back (node);
					break;
				}
			}
			
			return true;
		});
	}
}

/***
 *  Returns a list of all named nodes.
 */
X3D::MFNode
NodeIndex::getCurrentNodes ()
{
	// Find nodes

	X3D::MFNode nodes;

	if (inPrototypeInstance ())
		return nodes;

	if (not executionContext)
		return nodes;

	for (const auto & pair : executionContext -> getNamedNodes ())
	{
		try
		{
			nodes .emplace_back (pair .second -> getLocalNode ());
		}
		catch (...)
		{ }
	}

	return nodes;
}

std::set <X3D::SFNode>
NodeIndex::getImportingInlines () const
{
	std::set <X3D::SFNode> importingInlines;

	if (not executionContext)
		return importingInlines;

	for (const auto & pair : executionContext -> getImportedNodes ())
	{
		try
		{
			importingInlines .emplace (pair .second -> getInlineNode ());
		}
		catch (...)
		{ }
	}

	return importingInlines;
}

std::set <X3D::SFNode>
NodeIndex::getExportedNodes () const
{
	std::set <X3D::SFNode> exportedNodes;

	X3D::X3DScenePtr scene (executionContext);

	if (not scene)
		return exportedNodes;

	for (const auto & pair : scene -> getExportedNodes ())
	{
		try
		{
			exportedNodes .emplace (pair .second -> getLocalNode ());
		}
		catch (...)
		{ }
	}

	return exportedNodes;
}

void
NodeIndex::set_executionContext (const X3D::X3DExecutionContextPtr & value)
{
	if (executionContext)
	{
		executionContext -> namedNodes_changed ()    .removeInterest (&NodeIndex::refresh, this);
		executionContext -> importedNodes_changed () .removeInterest (&NodeIndex::refresh, this);
		executionContext -> sceneGraph_changed ()    .removeInterest (&NodeIndex::refresh, this);
	
		X3D::X3DScenePtr scene (executionContext);
	
		if (scene)
			scene -> exportedNodes_changed () .removeInterest (&NodeIndex::refresh, this);
	}

	executionContext = value;

	if (executionContext)
	{
		executionContext -> namedNodes_changed ()    .addInterest (&NodeIndex::refresh, this);
		executionContext -> importedNodes_changed () .addInterest (&NodeIndex::refresh, this);
	
		if (indexType == TYPE_INDEX)
			executionContext -> sceneGraph_changed () .addInterest (&NodeIndex::refresh, this);

		X3D::X3DScenePtr scene (executionContext);

		if (scene)
			scene -> exportedNodes_changed () .addInterest (&NodeIndex::refresh, this);

		refresh ();
	}
	else
		setNodes ({ });
}

void
NodeIndex::set_name (const size_t index)
{
	try
	{
		Gtk::TreePath path;
	
		path .push_back (index);
	
		const auto iter = getListStore () -> get_iter (path);

		iter -> set_value (Columns::NAME, getNameFromNode (nodes .at (index)));
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
NodeIndex::updateRow (const size_t index)
{
	Gtk::TreePath path;

	path .push_back (index);

	const auto iter = getListStore () -> get_iter (path);

	getListStore () -> row_changed (path, iter);
}

bool
NodeIndex::on_search_entry_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			try
			{
				setTypes ({ nodeTypes .at (getSearchEntry () .get_text ()) });
			}
			catch (const std::out_of_range &)
			{
				setTypes ({ });
			}

			return true;
		}
		case GDK_KEY_Escape:
		{
			getSearchEntry () .set_text ("");
			getTreeView () .grab_focus ();
			return true;
		}
		default:
			break;
	}

	return false;
}

bool
NodeIndex::on_search_entry_match_selected (const Gtk::TreeModel::iterator & iter)
{
	try
	{
		Glib::ustring typeName;

		iter -> get_value (Search::TYPE_NAME, typeName);

		setTypes ({ nodeTypes .at (typeName) });
	}
	catch (const std::out_of_range &)
	{
		setTypes ({ });
	}

	return false;
}

void
NodeIndex::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	node = nodes [getTreeModelSort () -> convert_path_to_child_path (path) .front ()];

	// Select node.

	switch (indexType)
	{
		case NAMED_NODES_INDEX:
		case TYPE_INDEX:
		case PROTO_INDEX:
		{
			const X3D::MFNode selection = { node };

			if (select)
				getBrowserWindow () -> getSelection () -> setNodes (selection);

			getBrowserWindow () -> expandNodes (selection);
			break;
		}
		default:
			break;
	}
}

NodeIndex::~NodeIndex ()
{
	dispose ();
}

} // puck
} // titania
