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

#include "X3DExternalToolsEditorInterface.h"

namespace titania {
namespace puck {

void
X3DExternalToolsEditorInterface::create (const std::string & filename)
{
	Gsv::init ();

	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DExternalToolsEditorInterface::create (std::initializer_list <std::string> filenames)
{
	Gsv::init ();

	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DExternalToolsEditorInterface::create ()
{
	// Get objects.
	m_TreeStore     = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("TreeStore"));
	m_TreeSelection = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("TreeSelection"));
	m_NameColumn    = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("NameColumn"));
	m_NameRenderer  = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("TreeView", m_TreeView);
	m_builder -> get_widget ("AddToolButton", m_AddToolButton);
	m_builder -> get_widget ("RemoveToolButton", m_RemoveToolButton);
	m_builder -> get_widget ("ToolBox", m_ToolBox);
	m_builder -> get_widget ("SourceView", m_SourceView);
	m_builder -> get_widget ("OptionsExpander", m_OptionsExpander);
	m_builder -> get_widget ("ShortcutKeyEntry", m_ShortcutKeyEntry);
	m_builder -> get_widget ("SaveTypeButton", m_SaveTypeButton);
	m_builder -> get_widget ("OutputTypeButton", m_OutputTypeButton);
	m_builder -> get_widget ("ApplicabilityTypeButton", m_ApplicabilityTypeButton);
	m_builder -> get_widget ("InputEncodingButton", m_InputEncodingButton);
	m_builder -> get_widget ("InputTypeButton", m_InputTypeButton);

	// Connect object Gtk::TreeView with id 'TreeView'.
	m_TreeView -> signal_drag_data_received () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_drag_data_received));
	m_TreeView -> signal_row_collapsed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_row_collapsed));
	m_TreeView -> signal_row_expanded () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_row_expanded));

	// Connect object Gtk::TreeSelection with id 'TreeSelection'.
	m_TreeSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_tree_selection_changed));

	// Connect object Gtk::Button with id 'AddToolButton'.
	m_AddToolButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_add_tool_clicked));
	m_RemoveToolButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_remove_tool_clicked));

	// Connect object Gtk::Entry with id 'ShortcutKeyEntry'.
	m_ShortcutKeyEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_shortcut_key_press_event), false);
	m_ShortcutKeyEntry -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_shortcut_key_release_event));

	// Connect object Gtk::ComboBoxText with id 'SaveTypeButton'.
	m_SaveTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_save_type_changed));
	m_OutputTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_output_type_changed));
	m_ApplicabilityTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_applicability_type_changed));
	m_InputEncodingButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_input_format_changed));
	m_InputTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DExternalToolsEditorInterface::on_input_type_changed));
}

X3DExternalToolsEditorInterface::~X3DExternalToolsEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
