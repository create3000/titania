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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_EDITOR_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_EDITOR_H__

#include "../../UserInterfaces/X3DOutlineEditorInterface.h"

namespace titania {
namespace puck {

class OutlineTreeViewEditor;

class OutlineEditor :
	public X3DOutlineEditorInterface
{
public:

	///  @name Construction

	OutlineEditor (BrowserWindow* const);

	///  @name Member access

	const std::shared_ptr <OutlineTreeViewEditor> &
	getTreeView () const
	{ return treeView; }

	///  @name Destruction

	~OutlineEditor ();


private:

	///  @name Construction

	virtual
	void
	on_map () final override;

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_scene ();

	void
	set_executionContext ();

	// Pointing Device

	virtual
	bool
	on_button_press_event (GdkEventButton*) final override;

	// Edit Menu Item

	// Set current scene

	virtual
	void
	on_set_as_current_scene_activate () final override;

	void
	on_scene_activate (Gtk::RadioMenuItem* const, const size_t);

	virtual
	void
	on_previous_scene_clicked () final override;

	virtual
	void
	on_next_scene_clicked () final override;

	std::pair <Gtk::RadioMenuItem*, size_t>
	addSceneMenuItem (const X3D::X3DExecutionContextPtr &, const X3D::X3DExecutionContextPtr &);

	// Create Instance

	virtual
	void
	on_create_instance_activate () final override;

	// Field references

	void
	on_create_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr);

	void
	on_remove_reference_activate (const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr);

	// Remove

	virtual
	void
	on_unlink_clone_activate () final override;

	virtual
	void
	on_remove_activate () final override;

	// View Menu Item

	virtual
	void
	on_show_extern_protos_toggled () final override;

	virtual
	void
	on_show_prototypes_toggled () final override;

	virtual
	void
	on_show_imported_nodes_toggled () final override;

	virtual
	void
	on_show_exported_nodes_toggled () final override;

	virtual
	void
	on_expand_extern_protos_toggled () final override;

	virtual
	void
	on_expand_prototype_instances_toggled () final override;

	virtual
	void
	on_expand_inline_nodes_toggled () final override;

	///  @name Operations

	void
	selectNode (const double, const double);

	void
	selectField (const double, const double);

	Gtk::TreePath
	getNodeAtPosition (const double, const double);

	Gtk::TreePath
	getFieldAtPosition (const double, const double);

	Gtk::TreePath
	getPathAtPosition (const double, const double);

	// Expanded

	void
	restoreExpanded (const X3D::X3DExecutionContextPtr &);

	void
	saveExpanded (const X3D::X3DExecutionContextPtr &);

	void
	getExpanded (const Gtk::TreeModel::Children &, std::deque <std::string> &) const;

	void
	setAdjustments (const double, const double);

	///  @name Members
	
	using MenuItemPair = std::pair <X3D::X3DExecutionContextPtr, Gtk::RadioMenuItem*>;

	std::shared_ptr <OutlineTreeViewEditor>        treeView;
	Gtk::RadioButtonGroup                          sceneGroup;
	std::map <X3D::X3DExecutionContextPtr, size_t> sceneIndex;
	std::deque <MenuItemPair>                      scenes;

	Gtk::TreePath nodePath;
	Gtk::TreePath fieldPath;
	bool          realized;

};

} // puck
} // titania

#endif
