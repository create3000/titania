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

#ifndef __TITANIA_OUTLINE_EDITOR_X3DOUTLINE_TREE_VIEW_H__
#define __TITANIA_OUTLINE_EDITOR_X3DOUTLINE_TREE_VIEW_H__

#include <gtkmm.h>

#include "../UserInterfaces/X3DOutlineTreeViewInterface.h"
#include "OutlineTree.h"
#include "OutlineTreeData.h"
#include "OutlineUserData.h"

namespace titania {
namespace puck {

class OutlineCellRenderer;
class OutlineRouteGraph;
class OutlineTreeModel;
class OutlineTreeObserver;

class X3DOutlineTreeView :
	public Gtk::TreeView, public X3DOutlineTreeViewInterface
{
public:

	///  @name Member access

	const Glib::RefPtr <OutlineTreeModel> &
	get_model () const
	{ return model; }

	void
	set_execution_context (const X3D::X3DExecutionContextPtr &);

	///  @name Iter access

	std::vector <Gtk::TreeModel::iterator>
	get_iters (X3D::X3DChildObject* const) const;

	OutlineUserDataPtr
	get_user_data (const Gtk::TreeModel::iterator &) const;

	OutlineUserDataPtr
	get_user_data (X3D::X3DChildObject* const) const;

	OutlineIterType
	get_data_type (const Gtk::TreeModel::iterator &) const;

	X3D::X3DChildObject*
	get_object (const Gtk::TreeModel::iterator &) const;

	void
	is_expanded (const Gtk::TreeModel::iterator &, const bool);

	bool
	is_expanded (const Gtk::TreeModel::iterator &) const;

	void
	is_full_expanded (const Gtk::TreeModel::iterator &, const bool);

	bool
	is_full_expanded (const Gtk::TreeModel::iterator &) const;

	///  @name Operations

	Gtk::TreeModel::Path
	get_path_at_position (const double, const double) const;

	Gtk::TreeModel::Path
	get_path_at_position (const double, const double, Gtk::TreeViewColumn* &) const;

	void
	update (X3D::X3DChildObject* const);

	void
	update_row (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	void
	expand_to (X3D::X3DChildObject* const);

	void
	collapse_row (const Gtk::TreeModel::Path &);

	///  @name Destruction

	virtual
	~X3DOutlineTreeView ();


protected:

	///  @name Construction

	X3DOutlineTreeView (const X3D::X3DExecutionContextPtr &);

	///  @name Member access

	const std::unique_ptr <OutlineTreeObserver> &
	get_tree_observer () const
	{ return treeObserver; }

	const std::unique_ptr <OutlineRouteGraph> &
	get_route_graph () const
	{ return routeGraph; }

	OutlineCellRenderer*
	get_cellrenderer () const
	{ return cellrenderer; }

	bool
	get_shift_key ();

	bool
	get_control_key ();

	bool
	get_alt_key ();

	void
	disable_shift_key ()
	{ ++ expandLevel; }

	void
	enable_shift_key ()
	{ -- expandLevel; }


private:

	friend class OutlineTreeObserver;

	///  @name Operations

	bool
	expand_to (const Gtk::TreeModel::Children &, std::vector <X3D::X3DChildObject*> &, Gtk::TreeModel::Path &);

	///  @name Member access

	void
	set_model (const Glib::RefPtr <OutlineTreeModel> &);

	void
	set_open_path (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	Gtk::TreeModel::Path
	get_open_path (const Gtk::TreeModel::iterator &) const;

	void
	set_rootNodes ();

	virtual
	bool
	on_test_expand_row (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &) final override;

	virtual
	void
	on_row_expanded (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &) final override;

	virtual
	bool
	on_test_collapse_row (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &) final override;

	virtual
	void
	on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) final override;

	void
	collapse_clone (const Gtk::TreeModel::iterator &);

	void
	model_expand_row (const Gtk::TreeModel::iterator &);

	X3D::FieldDefinitionArray
	get_fields (X3D::X3DBaseNode* const) const;

	void
	expand_routes (const Gtk::TreeModel::iterator &, X3D::X3DFieldDefinition*);

	void
	toggle_expand (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path & path);

	void
	auto_expand (const Gtk::TreeModel::iterator &);

	///  @name Members

	std::unique_ptr <OutlineTreeObserver> treeObserver;
	std::unique_ptr <OutlineRouteGraph>   routeGraph;
	Glib::RefPtr <OutlineTreeModel>       model;
	OutlineCellRenderer*                  cellrenderer;
	size_t                                expandLevel;

};

} // puck
} // titania

#endif
