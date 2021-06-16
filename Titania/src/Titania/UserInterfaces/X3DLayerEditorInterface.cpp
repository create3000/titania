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

#include "X3DLayerEditorInterface.h"

namespace titania {
namespace puck {

void
X3DLayerEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DLayerEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DLayerEditorInterface::create ()
{
	// Get objects.
	m_LayerListStore    = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("LayerListStore"));
	m_LayerSelection    = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("LayerSelection"));
	m_VisibilityColumn  = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("VisibilityColumn"));
	m_PickableColumn    = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("PickableColumn"));
	m_TypeNameColumn    = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TypeNameColumn"));
	m_NameColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("NameColumn"));
	m_SpacerColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("SpacerColumn"));
	m_ActiveLayerColumn = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ActiveLayerColumn"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("NewLayerSetButton", m_NewLayerSetButton);
	m_builder -> get_widget ("RemoveLayerSetButton", m_RemoveLayerSetButton);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("LayerSetExpander", m_LayerSetExpander);
	m_builder -> get_widget ("LayerScrolledWindow", m_LayerScrolledWindow);
	m_builder -> get_widget ("LayerTreeView", m_LayerTreeView);
	m_builder -> get_widget ("LayerNameBox", m_LayerNameBox);
	m_builder -> get_widget ("LayerNameEntry", m_LayerNameEntry);
	m_builder -> get_widget ("LayerRenameButton", m_LayerRenameButton);
	m_builder -> get_widget ("LayerActionBox", m_LayerActionBox);
	m_builder -> get_widget ("NewLayerPopupButton", m_NewLayerPopupButton);
	m_builder -> get_widget ("RemoveLayerButton", m_RemoveLayerButton);
	m_builder -> get_widget ("MoveLayerBox", m_MoveLayerBox);
	m_builder -> get_widget ("TopButton", m_TopButton);
	m_builder -> get_widget ("UpButton", m_UpButton);
	m_builder -> get_widget ("DownButton", m_DownButton);
	m_builder -> get_widget ("BottomButton", m_BottomButton);
	m_builder -> get_widget ("LayerSetLabel", m_LayerSetLabel);
	m_builder -> get_widget ("NewLayerPopover", m_NewLayerPopover);
	m_builder -> get_widget ("NewLayerButton", m_NewLayerButton);
	m_builder -> get_widget ("NewLayoutLayerButton", m_NewLayoutLayerButton);

	// Connect object Gtk::Button with id 'NewLayerSetButton'.
	m_NewLayerSetButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_new_layer_set_button_clicked));
	m_RemoveLayerSetButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_remove_layer_set_clicked));
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_index_clicked));

	// Connect object Gtk::TreeView with id 'LayerTreeView'.
	m_LayerTreeView -> signal_button_release_event () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_layers_button_release_event), false);
	m_LayerTreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_layer_activated));

	// Connect object Gtk::TreeSelection with id 'LayerSelection'.
	m_LayerSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_layer_selection_changed));

	// Connect object Gtk::Button with id 'NewLayerPopupButton'.
	m_NewLayerPopupButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_new_layer_popup_clicked));
	m_RemoveLayerButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_remove_layer_button_clicked));
	m_TopButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_top_clicked));
	m_UpButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_up_clicked));
	m_DownButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_down_clicked));
	m_BottomButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_bottom_clicked));
	m_NewLayerButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_new_layer_clicked));
	m_NewLayoutLayerButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLayerEditorInterface::on_new_layout_layer_clicked));
}

X3DLayerEditorInterface::~X3DLayerEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
