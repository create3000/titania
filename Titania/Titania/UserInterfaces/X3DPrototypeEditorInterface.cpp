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
#include "X3DPrototypeEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DPrototypeEditorInterface::m_widgetName = "PrototypeEditor";

void
X3DPrototypeEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_URLCellRendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("URLCellRendererText"));
	m_URLChooserColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("URLChooserColumn"));
	m_URLCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("URLCellrendererPixbuf"));

	// Get widgets.
	m_builder -> get_widget ("PrototypeMenu", m_PrototypeMenu);
	m_builder -> get_widget ("CreateProtoMenu", m_CreateProtoMenu);
	m_builder -> get_widget ("CreatePrototypeMenuItem", m_CreatePrototypeMenuItem);
	m_builder -> get_widget ("CreateExternProtoMenuItem", m_CreateExternProtoMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("EditPrototypeImage", m_EditPrototypeImage);
	m_builder -> get_widget ("EditLabel", m_EditLabel);
	m_builder -> get_widget ("HeaderLabel", m_HeaderLabel);
	m_builder -> get_widget ("CreateInstanceButton", m_CreateInstanceButton);
	m_builder -> get_widget ("MenuButton", m_MenuButton);
	m_builder -> get_widget ("PrototypeImage", m_PrototypeImage);
	m_builder -> get_widget ("PrototypeLabel", m_PrototypeLabel);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("PrototypeNotebook", m_PrototypeNotebook);
	m_builder -> get_widget ("InterfaceExpander", m_InterfaceExpander);
	m_builder -> get_widget ("InterfaceBox", m_InterfaceBox);
	m_builder -> get_widget ("URLBox", m_URLBox);
	m_builder -> get_widget ("URLTreeView", m_URLTreeView);
	m_builder -> get_widget ("URLAddButton", m_URLAddButton);
	m_builder -> get_widget ("URLRemoveButton", m_URLRemoveButton);
	m_builder -> get_widget ("URLReloadButton", m_URLReloadButton);
	m_builder -> get_widget ("InstancesBox", m_InstancesBox);

	// Connect object Gtk::ImageMenuItem with id 'CreatePrototypeMenuItem'.
	m_CreatePrototypeMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_create_proto_clicked));
	m_CreateExternProtoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_create_externproto_clicked));

	// Connect object Gtk::Button with id 'CreateInstanceButton'.
	m_CreateInstanceButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_create_instance_clicked));

	// Connect object Gtk::Entry with id 'NameEntry'.
	m_NameEntry -> signal_delete_text () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_name_delete_text), false);
	m_NameEntry -> signal_insert_text () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_name_insert_text), false);
	m_NameEntry -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_name_key_press_event), false);

	// Connect object Gtk::Button with id 'RenameButton'.
	m_RenameButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrototypeEditorInterface::on_rename_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DPrototypeEditorInterface::~X3DPrototypeEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
