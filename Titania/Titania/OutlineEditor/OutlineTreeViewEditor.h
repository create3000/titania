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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_EDITOR_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_VIEW_EDITOR_H__

#include "X3DOutlineTreeView.h"

#include "OutlineSelection.h"

namespace titania {
namespace puck {

class OutlineTreeViewEditor :
	public X3DOutlineTreeView
{
public:

	///  @name Construction

	OutlineTreeViewEditor (BrowserWindow* const, const X3D::X3DSFNode <X3D::X3DExecutionContext> &);


private:

	OutlineSelection &
	get_selection ()
	{ return selection; }

	const OutlineSelection &
	get_selection () const
	{ return selection; }
	
	static
	const std::string &
	get_drag_data_type ()
	{ return dragDataType; }

	virtual
	void
	on_drag_begin (const Glib::RefPtr <Gdk::DragContext> &) final;

	virtual
	void
	on_drag_end (const Glib::RefPtr <Gdk::DragContext> &) final;

	virtual
	void
	on_drag_data_delete (const Glib::RefPtr <Gdk::DragContext> &) final;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext>&, int, int, const Gtk::SelectionData &, guint info, guint) final;

	virtual
	void
	on_rename_node_activate () final;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) final;

	virtual
	bool
	on_button_press_event (GdkEventButton*) final;

	virtual
	bool
	on_button_release_event (GdkEventButton*) final;

	virtual
	bool
	set_motion_notify_event (GdkEventMotion*) final;

	void
	select_node (const Gtk::TreeModel::iterator &, const Gtk::TreeModel::Path &);

	bool
	select_field_value (double x, double y);

	void
	on_edited (const Glib::ustring &, const Glib::ustring &);

	bool
	hover_access_type (double, double);

	bool
	select_access_type (double, double);

	void
	set_access_type_selection (const OutlineUserDataPtr &, int);

	void
	clear_access_type_selection (const OutlineUserDataPtr &);

	///  @name Static members

	static const std::string dragDataType;

	///  @name Members
	
	using FieldType = X3D::X3DConstants::FieldType;

	OutlineSelection   selection;
	OutlineUserDataPtr overUserData;
	OutlineUserDataPtr selectedUserData;
	FieldType          matchingFieldType;
	int                matchingAccessType;
	X3D::SFNode        sourceNode;
	std::string        sourceField;
	X3D::SFNode        destinationNode;
	std::string        destinationField;

	sigc::connection 	motion_notify_connection;

};

} // puck
} // titania

#endif
