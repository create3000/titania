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
#include "X3DTextureCoordinateEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DTextureCoordinateEditorInterface::m_widgetName = "TextureCoordinateEditor";

void
X3DTextureCoordinateEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_TextureStageAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureStageAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("UndoMenuItem", m_UndoMenuItem);
	m_builder -> get_widget ("RedoMenuItem", m_RedoMenuItem);
	m_builder -> get_widget ("SelectionMenuItem", m_SelectionMenuItem);
	m_builder -> get_widget ("SelectAllMenuItem", m_SelectAllMenuItem);
	m_builder -> get_widget ("DeselectAllMenuItem", m_DeselectAllMenuItem);
	m_builder -> get_widget ("LeftBox", m_LeftBox);
	m_builder -> get_widget ("LeftHandButton", m_LeftHandButton);
	m_builder -> get_widget ("LeftArrowButton", m_LeftArrowButton);
	m_builder -> get_widget ("LeftLookAtButton", m_LeftLookAtButton);
	m_builder -> get_widget ("RightBox", m_RightBox);
	m_builder -> get_widget ("RightHandButton", m_RightHandButton);
	m_builder -> get_widget ("RightArrowButton", m_RightArrowButton);
	m_builder -> get_widget ("RightLookAtAllButton", m_RightLookAtAllButton);
	m_builder -> get_widget ("RightLookAtButton", m_RightLookAtButton);
	m_builder -> get_widget ("RemoveButton", m_RemoveButton);
	m_builder -> get_widget ("ApplyButton", m_ApplyButton);

	// Connect object Gtk::Adjustment with id 'TextureStageAdjustment'.
	m_TextureStageAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_texture_stage_changed));

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_key_press_event));
	m_Widget -> signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_key_release_event));

	// Connect object Gtk::ImageMenuItem with id 'UndoMenuItem'.
	m_UndoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_undo_activate));
	m_RedoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_redo_activate));
	m_SelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_select_all_activate));

	// Connect object Gtk::MenuItem with id 'DeselectAllMenuItem'.
	m_DeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_deselect_all_activate));

	// Connect object Gtk::RadioToolButton with id 'LeftHandButton'.
	m_LeftHandButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_left_hand_toggled));
	m_LeftArrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_left_arrow_toggled));

	// Connect object Gtk::ToolButton with id 'LeftLookAtButton'.
	m_LeftLookAtButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_left_look_at_all_clicked));

	// Connect object Gtk::RadioToolButton with id 'RightHandButton'.
	m_RightHandButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_right_hand_toggled));
	m_RightArrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_right_arrow_toggled));

	// Connect object Gtk::ToolButton with id 'RightLookAtAllButton'.
	m_RightLookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_right_look_at_all_clicked));

	// Connect object Gtk::ToggleToolButton with id 'RightLookAtButton'.
	m_RightLookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_right_look_at_toggled));

	// Connect object Gtk::Button with id 'RemoveButton'.
	m_RemoveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_remove_clicked));
	m_ApplyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextureCoordinateEditorInterface::on_apply_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DTextureCoordinateEditorInterface::~X3DTextureCoordinateEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
