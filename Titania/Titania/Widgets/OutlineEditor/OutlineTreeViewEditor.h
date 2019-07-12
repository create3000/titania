/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�1�7e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_EDITOR_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_EDITOR_H__

#include "X3DOutlineTreeView.h"
#include "../../Base/X3DEditorObject.h"

namespace titania {
namespace puck {

class OutlineEditor;
class OutlineDragDrop;

class OutlineTreeViewEditor :
	public X3DOutlineTreeView,
	public X3DEditorObject
{
public:

	///  @name Construction

	OutlineTreeViewEditor (X3DBrowserWindow* const browserWindow, const X3D::X3DExecutionContextPtr & executionContext, OutlineEditor* const outlineEditor);

	///  @name Member access

	virtual
	void
	set_execution_context (const X3D::X3DExecutionContextPtr & value) final override;

	///  @name Destruction

	virtual
	~OutlineTreeViewEditor () final override;


private:

	///  @name Operations

	virtual
	void
	on_unmap () final override;

	void
	watch_motion (const bool);

	virtual
	bool
	on_button_press_event (GdkEventButton*) final override;

	virtual
	bool
	on_button_release_event (GdkEventButton*) final override;

	bool
	set_motion_notify_event (GdkEventMotion*);

	bool
	on_query_tooltip (int x, int y, bool keyboard_tooltip, const Glib::RefPtr <Gtk::Tooltip> & tooltip);

	void
	set_camera (const X3D::SFNode & node);

	void
	set_camera (double width, double height);

	bool
	select_field_value (const double x, const double y);

	bool
	is_node (const Gtk::TreeModel::iterator &) const;

	X3D::X3DExecutionContext*
	get_context (const Gtk::TreeModel::iterator & iter) const;

	void
	on_edited (const Glib::ustring &, const Glib::ustring &);

	bool
	hover_icon (const double x, const double y);

	bool
	hover_access_type (const double x, const double y);

	bool
	select_color (const double x, const double y);

	void
	set_color_node_live (const bool value);

	void
	on_color_selection_unmap ();

	void
	on_color_changed ();

	void
	set_color ();

	void
	connectColor ();

	bool
	select_access_type (const double x, const double y);

	X3D::SFNode
	get_node (OutlineTreeData* const) const;

	bool
	add_route (const double, const double);

	void
	set_access_type_selection (const UserDataPtr &, const size_t);

	void
	clear_access_type_selection (const UserDataPtr &);

	bool
	remove_route (const double, const double);

	void
	remove_route (const Gtk::TreeModel::Path &, const std::vector <X3D::Route*> &);

	bool
	select_route (const double, const double);

	void
	clear_routes ();

	X3D::X3DScene*
	get_scene (X3D::X3DExecutionContext* executionContext);

	///  @name Members

	using FieldType = X3D::FieldType;

	std::unique_ptr <OutlineDragDrop> dragDrop;
	UserDataPtr overUserData;
	UserDataPtr selectedUserData;
	X3D::X3DExecutionContextPtr matchingContext;
	FieldType matchingFieldType;
	int matchingAccessType;
	Gtk::TreePath sourcePath;
	X3D::SFNode sourceNode;
	std::string sourceField;
	Gtk::TreePath destinationPath;
	X3D::SFNode destinationNode;
	std::string destinationField;

	X3D::SFNode                           colorNode;
	X3D::X3DPtr <X3D::X3DFieldDefinition> colorField;
	Gtk::ColorSelectionDialog             colorSelectionDialog;
	std::shared_ptr <X3D::UndoStep>       colorUndoStep;
	bool                                  changing;

	sigc::connection motion_notify_connection;

};

} // puck
} // titania

#endif
