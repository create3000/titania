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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__

#include "../../Base/X3DBaseInterface.h"
#include "../../Browser/UserData.h"
#include "OutlineTree.h"
#include "OutlineTreeData.h"

#include <Titania/X3D.h>

#include <deque>
#include <gtkmm.h>

namespace titania {
namespace puck {

class OutlineTreeModel :
	public Gtk::TreeModel,
	public Gtk::TreeDragSource,
	public Gtk::TreeDragDest,
	public Glib::Object
{
public:

	using Gtk::TreeModel::get_path;

	enum
	{
		DATA_COLUMN,
		SELECTED_COLUMN,
	};

	//	class Columns :
	//		public Gtk::TreeModel::ColumnRecord
	//	{
	//	public:
	//
	//		Columns ()
	//		{
	//			add (col_id);
	//			add (col_name);
	//			add (col_price);
	//			add (col_stock);
	//		}
	//
	//		~Columns () { }
	//
	//		typedef Gtk::TreeModelColumn <OutlineTreeData*> DataColumn;
	//		typedef Gtk::TreeModelColumn <bool>             SelectedColumn;
	//
	//		Gtk::TreeModelColumn <unsigned>      col_id;
	//		Gtk::TreeModelColumn <Glib::ustring> col_name;
	//		Gtk::TreeModelColumn <float>         col_price;
	//		Gtk::TreeModelColumn <bool>          col_stock;
	//
	//		DataColumn     data_column;
	//		SelectedColumn selected_column;
	//	};

	///  @name Construction

	static
	Glib::RefPtr <OutlineTreeModel>
	create (const X3D::X3DExecutionContextPtr &);
	
	///  @name Member access

	const X3D::X3DExecutionContextPtr &
	get_execution_context () const
	{ return executionContext; }

	///  @name Tree node

	static
	OutlineTreeData*
	get_data (const iterator &);

	UserDataPtr
	get_user_data (const iterator &) const;

	static
	UserDataPtr
	get_user_data (X3D::X3DChildObject*);

	static
	OutlineIterType
	get_data_type (const iterator &);

	static
	X3D::X3DChildObject*
	get_object (const iterator &);

	///  @name Route handling

	bool
	get_show_all_routes () const
	{ return show_all_routes; }

	void
	set_show_all_routes (const bool value)
	{ show_all_routes = value; }

	std::vector <X3D::Route*>
	get_input_routes (X3D::X3DFieldDefinition* const) const;

	size_t
	get_input_routes_size (X3D::X3DFieldDefinition* const) const;

	std::vector <X3D::Route*>
	get_output_routes (X3D::X3DFieldDefinition* const) const;

	size_t
	get_output_routes_size (X3D::X3DFieldDefinition* const) const;

	bool
	is_visible_route (const X3D::Route* const) const;

	///  @name Operations

	std::vector <Gtk::TreeModel::iterator>
	get_iters (X3D::X3DChildObject* const) const;

	iterator
	append (OutlineIterType type, X3D::X3DChildObject*, const size_t = 0);

	iterator
	append (const iterator &, OutlineIterType type, X3D::X3DChildObject*, const size_t = 0);

	void
	clear ();

	void
	clear (const iterator &);

	bool
	iter_is_valid (const iterator & iter) const;

	///  @name Destruction

	virtual
	~OutlineTreeModel ();


private:

	OutlineTreeModel (const X3D::X3DExecutionContextPtr &);

	void
	set_data (iterator &, OutlineIterType type, X3D::X3DChildObject* object, const Path & path, const size_t) const;

	void
	set_data (iterator &, OutlineTreeData*) const;

	std::vector <OutlineTreeData*>
	get_parents (const iterator &) const;

	virtual
	Gtk::TreeModelFlags
	get_flags_vfunc () const final override;

	virtual
	int
	get_n_columns_vfunc () const final override;

	virtual GType
	get_column_type_vfunc (int index) const final override;

	virtual
	void
	get_value_vfunc (const iterator & iter, int column, Glib::ValueBase & value) const final override;

	virtual
	Path
	get_path_vfunc (const iterator & iter) const final override;

	virtual
	bool
	get_iter_vfunc (const Path & path, iterator & iter) const;

	virtual
	int
	iter_n_root_children_vfunc () const final override;

	virtual
	bool
	iter_nth_root_child_vfunc (int n, iterator & iter) const final override;

	virtual
	bool
	iter_has_child_vfunc (const iterator & iter) const final override;

	bool
	is_in_parents (const X3D::SFNode & sfnode, const iterator &) const;

	virtual
	int
	iter_n_children_vfunc (const iterator & iter) const final override;

	virtual
	bool
	iter_children_vfunc (const iterator & parent, iterator & iter) const final override;

	virtual
	bool
	iter_nth_child_vfunc (const iterator & parent, int n, iterator & iter) const final override;

	virtual
	bool
	iter_next_vfunc (const iterator & iter, iterator & iter_next) const final override;

	virtual
	bool
	iter_parent_vfunc (const iterator & child, iterator & iter) const final override;

	virtual
	void
	ref_node_vfunc (const iterator &) const final override;

	virtual
	void
	unref_node_vfunc (const iterator &) const final override;

	virtual
	void
	on_row_changed (const Path &, const iterator &) final override;

	virtual
	void
	on_row_inserted (const Path & path, const iterator &) final override;

	virtual
	void
	on_row_has_child_toggled (const Path &, const iterator &) final override;

	virtual
	void
	on_row_deleted (const Path &) final override;

	virtual
	void
	on_rows_reordered (const Path &, const iterator &, int*) final override;
	
	virtual
	bool
	row_draggable_vfunc (const Path & path) const final override;
	
	virtual
	bool
	drag_data_get_vfunc (const Path & path, Gtk::SelectionData & selection_data) const final override;

	///  @name Members

	typedef Gtk::TreeModelColumn <OutlineTreeData*> DataColumn;
	typedef Gtk::TreeModelColumn <bool>             SelectedColumn;

	X3D::X3DExecutionContextPtr executionContext;

	DataColumn     data_column;
	SelectedColumn selected_column;

	mutable OutlineTree tree;
	int                 stamp;

	bool show_all_routes;

};

} // puck
} // titania

#endif
