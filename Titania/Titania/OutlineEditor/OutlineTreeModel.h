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

#include "../OutlineEditor/OutlineUserData.h"
#include <Titania/X3D.h>

#include <deque>
#include <gtkmm.h>
#include <iostream>

namespace titania {
namespace puck {

enum class OutlineIterType
{
	X3DFieldValue,
	X3DField,
	X3DBaseNode

};

class OutlineIterParent
{
public:

	OutlineIterParent (OutlineIterType type, X3D::X3DChildObject* object, size_t index) :
		object (object),
		type (type),
		index (index)
	{ }

	X3D::X3DChildObject* const object;
	const OutlineIterType      type;
	const size_t               index;

};

class OutlineIterData :
	public OutlineIterParent
{
public:

	typedef std::deque <OutlineIterParent> parents_type;

	OutlineIterData (OutlineIterType type, X3D::X3DChildObject* object, int index, const parents_type & parents) :
		OutlineIterParent (type, object, index),
		parents (parents)
	{ }

	const parents_type parents;

};

class OutlineNode
{
public:

	OutlineNode () :
		data (NULL),
		children ()
	{ }

	const std::deque <OutlineNode> &
	getChildren ()
	{ return children; }

	void
	clear ()
	{ children .clear (); }

	~OutlineNode ()
	{
		if (data)
			delete data;
	}

	OutlineIterData*         data;
	std::deque <OutlineNode> children;

};

class OutlineTree :
	public OutlineNode
{
public:

	OutlineTree () :
		OutlineNode ()
	{ }

	OutlineNode &
	getNode (const Gtk::TreePath & path)
	{
		OutlineNode* node = this;

		for (const auto & index : path)
			node = &getChild (node, index);

		return *node;
	}

	void
	removeChildren (const Gtk::TreePath & path)
	{
		getNode (path) .children .clear ();
	}

private:

	OutlineNode &
	getChild (OutlineNode* parent, size_t index)
	{
		if (index + 1 > parent -> children .size ())
			parent -> children .resize (index + 1);

		return parent -> children [index];
	}

};

class OutlineTreeModel :
	public Glib::Object, public Gtk::TreeModel
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
	collapse_row (const Path &, const iterator &);

	static
	OutlineUserData*
	getUserData (const iterator &);

	static
	OutlineUserData*
	getUserData (X3D::X3DChildObject*);

	static
	OutlineIterData*
	getData (const iterator &);

	virtual
	~OutlineTreeModel ();


private:

	void
	set_rootNodes ();

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

	static
	X3D::FieldDefinitionArray
	getFields (X3D::X3DChildObject*);

	void
	setData (iterator &, OutlineIterType type, X3D::X3DChildObject* object, size_t index, const OutlineIterData::parents_type & parents) const;

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
