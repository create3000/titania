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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__

#include "../Base/X3DBaseInterface.h"
#include "../OutlineEditor/OutlineTree.h"
#include "../OutlineEditor/OutlineTreeData.h"
#include "../OutlineEditor/OutlineUserData.h"
#include <Titania/X3D.h>

#include <deque>
#include <gtkmm.h>

namespace titania {
namespace puck {

class OutlineTreeModel :
	public Glib::Object, public Gtk::TreeModel, public X3DBaseInterface
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

	static
	Glib::RefPtr <OutlineTreeModel>
	create (const X3D::X3DSFNode <X3D::Browser> &);

	OutlineUserDataPtr
	get_user_data (const iterator &) const;

	static
	OutlineUserDataPtr
	get_user_data (X3D::X3DChildObject*);

	static
	OutlineIterType
	get_data_type (const iterator &);

	static
	X3D::X3DChildObject*
	get_object (const iterator &);

	iterator
	append (OutlineIterType type, X3D::X3DChildObject*);

	iterator
	append (const iterator &, OutlineIterType type, X3D::X3DChildObject*);

	void
	clear ();

	void
	clear (const iterator &);

	virtual
	~OutlineTreeModel ();


private:

	OutlineTreeModel (const X3D::X3DSFNode <X3D::Browser> &);

	bool
	iter_is_valid (const iterator & iter) const;

	void
	set_data (iterator &, OutlineIterType type, X3D::X3DChildObject* object, const Path & path) const;

	void
	set_data (iterator &, OutlineTreeData*) const;

	static
	OutlineTreeData*
	get_data (const iterator &);

	std::deque <OutlineTreeData*>
	get_parents (const iterator &) const;

	virtual
	Gtk::TreeModelFlags
	get_flags_vfunc () const;

	virtual
	int
	get_n_columns_vfunc () const;

	virtual GType
	get_column_type_vfunc (int index) const;

	virtual
	void
	get_value_vfunc (const iterator & iter, int column, Glib::ValueBase & value) const;

	virtual
	Path
	get_path_vfunc (const iterator & iter) const;

	virtual
	bool
	get_iter_vfunc (const Path & path, iterator & iter) const;

	virtual
	int
	iter_n_root_children_vfunc () const;

	virtual
	bool
	iter_nth_root_child_vfunc (int n, iterator & iter) const;

	virtual
	bool
	iter_has_child_vfunc (const iterator & iter) const;

	virtual
	int
	iter_n_children_vfunc (const iterator & iter) const;

	virtual
	bool
	iter_children_vfunc (const iterator & parent, iterator & iter) const;

	virtual
	bool
	iter_nth_child_vfunc (const iterator & parent, int n, iterator & iter) const;

	virtual
	bool
	iter_next_vfunc (const iterator & iter, iterator & iter_next) const;

	virtual
	bool
	iter_parent_vfunc (const iterator & child, iterator & iter) const;

	virtual
	void
	ref_node_vfunc (const iterator &) const;

	virtual
	void
	unref_node_vfunc (const iterator &) const;

	virtual
	void
	on_row_changed (const Path &, const iterator &);

	virtual
	void
	on_row_inserted (const Path & path, const iterator &);

	virtual
	void
	on_row_has_child_toggled (const Path &, const iterator &);

	virtual
	void
	on_row_deleted (const Path &);

	virtual
	void
	on_rows_reordered (const Path &, const iterator &, int*);

	typedef Gtk::TreeModelColumn <OutlineTreeData*> DataColumn;
	typedef Gtk::TreeModelColumn <bool>             SelectedColumn;

	X3D::X3DSFNode <X3D::X3DExecutionContext> executionContext;

	DataColumn     data_column;
	SelectedColumn selected_column;

	mutable OutlineTree tree;
	int                 stamp;

};

} // puck
} // titania

#endif
