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
#include "X3DOutlineEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DOutlineEditorInterface::m_widgetName = "OutlineEditor";

void
X3DOutlineEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_IconFactory = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));

	// Get widgets.
	m_builder -> get_widget ("PopupMenu", m_PopupMenu);
	m_PopupMenu -> set_name ("PopupMenu");
	m_builder -> get_widget ("SetAsCurrentSceneMenuItem", m_SetAsCurrentSceneMenuItem);
	m_SetAsCurrentSceneMenuItem -> set_name ("SetAsCurrentSceneMenuItem");
	m_builder -> get_widget ("CreateInstanceMenuItem", m_CreateInstanceMenuItem);
	m_CreateInstanceMenuItem -> set_name ("CreateInstanceMenuItem");
	m_builder -> get_widget ("RemoveMenuItem", m_RemoveMenuItem);
	m_RemoveMenuItem -> set_name ("RemoveMenuItem");
	m_builder -> get_widget ("ViewMenuItem", m_ViewMenuItem);
	m_ViewMenuItem -> set_name ("ViewMenuItem");
	m_builder -> get_widget ("ShowExternProtosMenuItem", m_ShowExternProtosMenuItem);
	m_ShowExternProtosMenuItem -> set_name ("ShowExternProtosMenuItem");
	m_builder -> get_widget ("ShowPrototypesMenuItem", m_ShowPrototypesMenuItem);
	m_ShowPrototypesMenuItem -> set_name ("ShowPrototypesMenuItem");
	m_builder -> get_widget ("ShowImportedNodesMenuItem", m_ShowImportedNodesMenuItem);
	m_ShowImportedNodesMenuItem -> set_name ("ShowImportedNodesMenuItem");
	m_builder -> get_widget ("ShowExportedNodesMenuItem", m_ShowExportedNodesMenuItem);
	m_ShowExportedNodesMenuItem -> set_name ("ShowExportedNodesMenuItem");
	m_builder -> get_widget ("ExpandExternProtosMenuItem", m_ExpandExternProtosMenuItem);
	m_ExpandExternProtosMenuItem -> set_name ("ExpandExternProtosMenuItem");
	m_builder -> get_widget ("ExpandInlineNodesMenuItem", m_ExpandInlineNodesMenuItem);
	m_ExpandInlineNodesMenuItem -> set_name ("ExpandInlineNodesMenuItem");
	m_builder -> get_widget ("SceneMenu", m_SceneMenu);
	m_SceneMenu -> set_name ("SceneMenu");
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("SceneMenuBox", m_SceneMenuBox);
	m_SceneMenuBox -> set_name ("SceneMenuBox");
	m_builder -> get_widget ("SceneMenuButton", m_SceneMenuButton);
	m_SceneMenuButton -> set_name ("SceneMenuButton");
	m_builder -> get_widget ("SceneLabel", m_SceneLabel);
	m_SceneLabel -> set_name ("SceneLabel");
	m_builder -> get_widget ("PreviousSceneButton", m_PreviousSceneButton);
	m_PreviousSceneButton -> set_name ("PreviousSceneButton");
	m_builder -> get_widget ("NextSceneButton", m_NextSceneButton);
	m_NextSceneButton -> set_name ("NextSceneButton");
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_ScrolledWindow -> set_name ("ScrolledWindow");
	m_builder -> get_widget ("Viewport", m_Viewport);
	m_Viewport -> set_name ("Viewport");

	// Connect object Gtk::MenuItem with id 'SetAsCurrentSceneMenuItem'.
	m_SetAsCurrentSceneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_set_as_current_scene_activate));
	m_CreateInstanceMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_create_instance_activate));

	// Connect object Gtk::ImageMenuItem with id 'RemoveMenuItem'.
	m_RemoveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_remove_activate));

	// Connect object Gtk::CheckMenuItem with id 'ShowExternProtosMenuItem'.
	m_ShowExternProtosMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_show_extern_protos_toggled));
	m_ShowPrototypesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_show_prototypes_toggled));
	m_ShowImportedNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_show_imported_nodes_toggled));
	m_ShowExportedNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_show_exported_nodes_toggled));
	m_ExpandExternProtosMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_expand_extern_protos_toggled));
	m_ExpandInlineNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_expand_inline_nodes_toggled));

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_map));

	// Connect object Gtk::Button with id 'PreviousSceneButton'.
	m_PreviousSceneButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_previous_scene_clicked));
	m_NextSceneButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_next_scene_clicked));

	// Connect object Gtk::ScrolledWindow with id 'ScrolledWindow'.
	m_ScrolledWindow -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DOutlineEditorInterface::on_button_press_event));

	// Call construct handler of base class.
	construct ();
}

X3DOutlineEditorInterface::~X3DOutlineEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
