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

#ifndef __TITANIA_EDITORS_NODE_INDEX_NODE_INDEX_H__
#define __TITANIA_EDITORS_NODE_INDEX_NODE_INDEX_H__

#include "../../Base/X3DEditorObject.h"
#include "../../Browser/UserData.h"
#include "../../UserInterfaces/X3DNodeIndexInterface.h"

namespace titania {
namespace puck {

class X3DProtoDeclarationNode;
class ScrollFreezer;

class NodeIndex :
	virtual public X3DNodeIndexInterface
{
public:

	///  @name Member types

	struct Columns
	{
		static constexpr int IMAGE          = 0;
		static constexpr int TYPE_NAME      = 1;
		static constexpr int NAME           = 2;
		static constexpr int IMPORTED_NODES = 3;
		static constexpr int EXPORTED_NODES = 4;
		static constexpr int INDEX          = 5;
		static constexpr int PROTO          = 6;
	};

	///  @name Construction

	NodeIndex (X3DBrowserWindow* const browserWindow);

	///  @name Operations

	int
	getIndexColumn () const;

	void
	setShowWidget (const bool value);

	void
	setObserveNodes (const bool value)
	{ observeNodes = value; }

	void
	setSelect (const bool value)
	{ select = value; }

	void
	setDisplayProtoNodes (const bool value);

	bool
	getDisplayProtoNodes () const
	{ return displayProtoNodes; }

	void
	setNamedNodes ();

	void
	setTypes (const std::set <X3D::X3DConstants::NodeType> & value);

	void
	setAnimations ();

	void
	setProto (const X3D::X3DPtr <X3D::X3DProtoDeclarationNode> &);

	bool
	setSelection (const X3D::SFNode & node);

	void
	scrollToRow (const X3D::SFNode & node);

	void
	updateRow (const size_t index);

	const X3D::MFNode &
	getNodes () const
	{ return nodes; }

	const X3D::SFNode &
	getNode () const
	{ return node; }

	///  @name Destruction

	virtual
	~NodeIndex () final override;


private:

	///  @name Member types

	enum IndexType
	{
		NAMED_NODES_INDEX,
		TYPE_INDEX,
		ANIMATION_INDEX,
		PROTO_INDEX
	};

	struct Search;

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	virtual
	void
	on_column_clicked () final override;

	int
	on_compare_type_name (const Gtk::TreeModel::iterator & lhs, const Gtk::TreeModel::iterator & rhs);

	int
	on_compare_name (const Gtk::TreeModel::iterator & lhs, const Gtk::TreeModel::iterator & rhs);

	void
	refresh ();

	void
	set_refresh ();

	///  @name Operations

	Gtk::TreePath
	getPath (const X3D::SFNode & node);

	void
	setNodes (X3D::MFNode && value);

	std::string
	getNameFromNode (const X3D::SFNode & node) const;

	X3D::MFNode
	getCurrentNodes (const std::set <X3D::X3DConstants::NodeType> & types);

	X3D::MFNode
	getCurrentNodes ();

	void
	getCurrentProtoNodes (X3D::X3DExecutionContext* const executionContext, X3D::MFNode & nodes);

	std::set <X3D::SFNode>
	getImportingInlines () const;

	std::set <X3D::SFNode>
	getExportedNodes () const;

	///  @name Event handlers

	void
	set_executionContext (const X3D::X3DExecutionContextPtr & value);

	void
	set_name (const size_t index);

	virtual
	bool
	on_search_entry_key_press_event (GdkEventKey* event) final override;

	virtual
	bool
	on_search_entry_match_selected (const Gtk::TreeModel::iterator & iter) final override;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) final override;

	///  @name Members

	X3D::X3DExecutionContextPtr                         executionContext;
	X3D::X3DPtr <X3D::X3DProtoDeclarationNode>          protoNode;
	X3D::MFNode                                         nodes;
	X3D::SFNode                                         node;
	X3D::SFTime                                         refreshBuffer;
	IndexType                                           indexType;
	bool                                                showWidget;
	bool                                                observeNodes;
	bool                                                select;
	bool                                                displayProtoNodes;
	std::set <X3D::X3DConstants::NodeType>              types;
	std::map <std::string, X3D::X3DConstants::NodeType> nodeTypes;
	std::vector <Glib::RefPtr <Gtk::TreeViewColumn>>    columns;

	std::unique_ptr <ScrollFreezer> scrollFreezer;

};

} // puck
} // titania

#endif
