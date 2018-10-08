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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_X3DOUTLINE_TREE_VIEW_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_X3DOUTLINE_TREE_VIEW_H__

#include <gtkmm.h>

#include "../../Base/X3DBaseInterface.h"
#include "../../Browser/UserData.h"
#include "OutlineTree.h"
#include "OutlineTreeData.h"

namespace titania {
namespace puck {

class OutlineCellRenderer;
class OutlineRouteGraph;
class OutlineSelection;
class OutlineTreeModel;
class OutlineTreeObserver;
class ScrollFreezer;

class X3DOutlineTreeView :
	virtual public X3DBaseInterface,
	public Gtk::TreeView
{
public:

	using Gtk::TreeView::expand_row;

	///  @name Construction

	///  @name Member access

	const Glib::RefPtr <OutlineTreeModel> &
	get_model () const
	{ return model; }

	Gtk::TreeViewColumn*
	getColumn () const
	{ return column; }

	const std::unique_ptr <ScrollFreezer> &
	getScrollFreezer () const
	{ return scrollFreezer; }

	void
	set_execution_context (const X3D::X3DExecutionContextPtr & value);

	const X3D::X3DExecutionContextPtr &
	get_execution_context () const;

	void
	set_show_extern_protos (const bool value)
	{ externProtos = value; }

	bool
	get_show_extern_protos () const
	{ return externProtos; }

	void
	set_show_prototypes (const bool value)
	{ prototypes = value; }

	bool
	get_show_prototypes () const
	{ return prototypes; }

	void
	set_show_imported_nodes (const bool value)
	{ importedNodes = value; }

	bool
	get_show_imported_nodes () const
	{ return importedNodes; }

	void
	set_show_exported_nodes (const bool value)
	{ exportedNodes = value; }

	bool
	get_show_exported_nodes () const
	{ return exportedNodes; }

	void
	set_expand_extern_protos (const bool value)
	{ expandExternProtos = value; }

	bool
	get_expand_extern_protos () const
	{ return expandExternProtos; }

	void
	set_expand_prototype_instances (const bool value)
	{ expandPrototypeInstances = value; }

	bool
	get_expand_prototype_instances () const
	{ return expandPrototypeInstances; }

	void
	set_expand_inline_nodes (const bool value)
	{ expandInlineNodes = value; }

	bool
	get_expand_inline_nodes () const
	{ return expandInlineNodes; }

	void
	set_use_locale (const bool);

	bool
	get_use_locale () const
	{ return useLocale; }

	///  @name Iter access

	std::vector <Gtk::TreeIter>
	get_iters (X3D::X3DChildObject* const) const;

	UserDataPtr
	get_user_data (const Gtk::TreeIter &) const;

	OutlineIterType
	get_data_type (const Gtk::TreeIter &) const;

	X3D::X3DChildObject*
	get_object (const Gtk::TreeIter &) const;

	size_t
	get_index (const Gtk::TreeIter &) const;

	void
	set_expanded (const Gtk::TreeIter &, const OutlineExpanded);

	OutlineExpanded
	get_expanded (const Gtk::TreeIter &) const;

	///  @name Operations

	Gtk::TreePath
	get_path_at_position (const double, const double) const;

	Gtk::TreePath
	get_path_at_position (const double, const double, Gtk::TreeViewColumn* &) const;

	void
	expand_to (X3D::X3DChildObject* const);

	void
	expand_row (const Gtk::TreePath & path, const bool open_all, const OutlineExpanded & expanded);

	void
	collapse_row (const Gtk::TreePath & path);

	void
	update ();

	///  @name Destruction

	virtual
	~X3DOutlineTreeView () override;


protected:

	///  @name Friends

	friend class OutlineTreeObserver;

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

	void
	get_opened_objects (const Gtk::TreeIter & parent, std::map <size_t,  std::tuple <size_t, bool, OutlineExpanded>> & opened);

	void
	reopen_objects (const Gtk::TreeIter & parent, std::map <size_t,  std::tuple <size_t, bool, OutlineExpanded>> & opened);


private:

	friend class OutlineTreeObserver;

	///  @name Operations

	bool
	expand_to (const Gtk::TreeModel::Children &, std::vector <X3D::X3DChildObject*> &, Gtk::TreePath &);

	///  @name Member access

	void
	set_model (const Glib::RefPtr <OutlineTreeModel> &);

	void
	set_open_path (const Gtk::TreeIter &, const Gtk::TreePath &);

	Gtk::TreePath
	get_open_path (const Gtk::TreeIter &) const;
	
	X3D::X3DScene*
	get_scene (X3D::X3DExecutionContext* const executionContext) const;

	void
	set_rootNodes (const bool reopen);

	size_t
	get_reopen_id (const Gtk::TreeIter & iter);

	virtual
	void
	on_style_updated () final override;

	virtual
	void
	on_row_activated (const Gtk::TreePath &, Gtk::TreeViewColumn*) final override;

	void
	select_node (const Gtk::TreeIter &, const Gtk::TreePath &);

	virtual
	bool
	on_test_expand_row (const Gtk::TreeIter &, const Gtk::TreePath &) final override;

	virtual
	void
	on_row_expanded (const Gtk::TreeIter &, const Gtk::TreePath &) final override;

	virtual
	bool
	on_test_collapse_row (const Gtk::TreeIter &, const Gtk::TreePath &) final override;

	virtual
	void
	on_row_collapsed (const Gtk::TreeIter & iter, const Gtk::TreePath & path) final override;

	void
	collapse_clone (const Gtk::TreeIter &);

	void
	model_expand_row (const Gtk::TreeIter &);

	void
	model_expand_node (const X3D::SFNode &, const Gtk::TreeIter &);

	X3D::FieldDefinitionArray
	get_fields (X3D::X3DBaseNode* const) const;

	void
	expand_routes (const Gtk::TreeIter & iter, X3D::X3DFieldDefinition* field);

	void
	toggle_expand (const Gtk::TreeIter & iter, const Gtk::TreePath & path, const OutlineExpanded & expanded);

	void
	auto_expand (const Gtk::TreeIter & parent);

	///  @name Members

	Glib::RefPtr <OutlineTreeModel>             model;
	const std::unique_ptr <OutlineSelection>    selection;
	Gtk::TreeViewColumn*                        column;
	const std::unique_ptr <OutlineTreeObserver> treeObserver;
	const std::unique_ptr <OutlineRouteGraph>   routeGraph;
	OutlineCellRenderer* const                  cellrenderer;
	Gtk::CellRendererText* const                padCellrenderer;
	size_t                                      expandLevel;

	bool externProtos;
	bool prototypes;
	bool importedNodes;
	bool exportedNodes;
	bool expandExternProtos;
	bool expandPrototypeInstances;
	bool expandInlineNodes;
	bool useLocale;

	std::unique_ptr <ScrollFreezer> scrollFreezer;
	X3D::SFTime                     rootNodesBuffer;

};

} // puck
} // titania

#endif
