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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_DRAG_DROP_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_DRAG_DROP_H__

#include "../../Base/X3DBaseInterface.h"

#include "OutlineTreeData.h"

#include <gtkmm.h>

namespace titania {
namespace puck {

class OutlineEditor;
class OutlineTreeViewEditor;

class OutlineDragDrop
{
public:

	OutlineDragDrop (OutlineEditor* const outlineEditor, OutlineTreeViewEditor* const treeView);


private:

	/// @name Member access

	X3DBrowserWindow*
	getBrowserWindow () const;

	///  @name Event handlers

	bool
	on_button_press_event (GdkEventButton* event);

	std::pair <Gtk::TreePath, Gtk::TreePath>
	get_proto_limits (const Gtk::TreePath & path, X3D::ProtoDeclaration* const prototype) const;

	Gtk::TreePath
	get_proto_first_limit (Gtk::TreePath path,
	                       X3D::ProtoDeclaration* const prototype,
	                       const std::vector <X3D::ProtoDeclarationPtr> & prototypes) const;
	
	Gtk::TreePath
	get_proto_second_limit (Gtk::TreePath path,
	                         X3D::ProtoDeclaration* const prototype,
                            const std::vector <X3D::ProtoDeclarationPtr> & prototypes) const;

	void
	on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context, Gtk::SelectionData & selection_data, guint info, guint time);

	bool
	on_drag_motion (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time);

	bool
	on_drag_motion_proto (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time, const OutlineIterType type);

	bool
	on_drag_motion_base_node (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, guint time);

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
	                       int x, int y,
	                       const Gtk::SelectionData & selection_data,
	                       guint info,
	                       guint time);

	void
	on_drag_data_proto_received (const Glib::RefPtr <Gdk::DragContext> & context,
	                             int x, int y,
	                             const Gtk::SelectionData & selection_data,
	                             guint info,
	                             guint time);

	void
	on_drag_data_extern_proto_received (const Glib::RefPtr <Gdk::DragContext> & context,
	                                    int x, int y,
	                                    const Gtk::SelectionData & selection_data,
	                                    guint info,
	                                    guint time);

	void
	on_drag_action_activate (const Gdk::DragAction & action);

	void
	on_drag_data_base_node_received (const Gdk::DragAction action, int x, int y);

	bool
	on_drag_data_base_node_insert_into_node_received (const Gdk::DragAction action, int x, int y);

	bool
	on_drag_data_base_node_on_field_received (const Gdk::DragAction action, int x, int y);

	bool
	on_drag_data_base_node_insert_into_array_received (const Gdk::DragAction action, int x, int y);

	void
	remove_source_node (const X3D::X3DExecutionContextPtr & sourceContext,
	                    X3D::SFNode* const sourceParent,
	                    X3D::X3DFieldDefinition* const sourceField,
	                    size_t sourceIndex,
	                    const X3D::UndoStepPtr & undoStep);

	std::string
	get_node_action_string (Gdk::DragAction, const X3D::SFNode & node) const;

	///  @name Members

	OutlineEditor* const                     outlineEditor;
	OutlineTreeViewEditor* const             treeView;
	Gtk::TreePath                            sourcePath;
	size_t                                   sourceId;
	OutlineIterType                          sourceType;
	size_t                                   nodeId;
	std::pair <Gtk::TreePath, Gtk::TreePath> protoLimits;
	X3D::Vector2i                            pointer;

};

} // puck
} // titania

#endif
