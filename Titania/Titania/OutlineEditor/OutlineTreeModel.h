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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_MODEL_H__

#include "../Base/X3DBaseInterface.h"
#include "../OutlineEditor/OutlineUserData.h"
#include "../OutlineEditor/OutlineIterData.h"
#include "../OutlineEditor/OutlineTree.h"
#include <Titania/X3D.h>

#include <deque>
#include <gtkmm.h>
#include <iostream>

namespace titania {
namespace puck {

class OutlineTreeModel :
	public Glib::Object, public Gtk::TreeModel//, public X3DBaseInterface
{
public:

	OutlineTreeModel (const X3D::X3DSFNode <X3D::Browser> &);

	static
	Glib::RefPtr <OutlineTreeModel>
	create (const X3D::X3DSFNode <X3D::Browser> &);

	const X3D::X3DSFNode <X3D::X3DExecutionContext> &
	getExecutionContext ()
	{ return executionContext; }

	void
	collapse_row (const Path &);

	static
	OutlineUserData*
	get_user_data (const iterator &);

	static
	OutlineUserData*
	get_user_data (X3D::X3DChildObject*);

	static
	OutlineIterType
	get_data_type (const iterator &);

	static
	X3D::X3DChildObject*
	get_object (const iterator &);

	void
	select_fields (const iterator & iter);

	virtual
	~OutlineTreeModel ();


private:

	void
	set_data (iterator &, OutlineIterType type, X3D::X3DChildObject* object, size_t index, const OutlineIterData::parents_type & parents) const;

	static
	OutlineIterData*
	get_data (const iterator &);

	static
	const OutlineIterData::parents_type &
	get_parents (const iterator &);

	static
	size_t
	get_index (const iterator &);

	static
	X3D::FieldDefinitionArray
	get_fields (X3D::X3DChildObject*);

	void
	set_rootNodes ();

	void
	clear ();

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

	Path
	get_path (const OutlineIterData::parents_type & parents, size_t) const;

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

	typedef Gtk::TreeModelColumn <Glib::RefPtr <Gdk::Pixbuf>> icon_column_type;
	typedef Gtk::TreeModelColumn <OutlineIterData*>            data_column_type;
	typedef Gtk::TreeModelColumn <bool>                        selected_color_column_type;
	typedef Gtk::TreeModelColumn <Glib::ustring>               debug_column_type;

	typedef std::map <const X3D::X3DConstants::FieldType, Glib::RefPtr <Gdk::Pixbuf>> FieldTypeImageIndex;

	X3D::X3DSFNode <X3D::X3DExecutionContext> executionContext;

	icon_column_type           icon_column;
	data_column_type           data_column;
	selected_color_column_type selected_color_column;
	debug_column_type          debug_column;

	Glib::RefPtr <Gdk::Pixbuf> noneImage;
	Glib::RefPtr <Gdk::Pixbuf> baseNodeImage;
	FieldTypeImageIndex        fieldTypeImages;

	int                 stamp;
	mutable OutlineTree tree;

};

} // puck
} // titania

#endif
