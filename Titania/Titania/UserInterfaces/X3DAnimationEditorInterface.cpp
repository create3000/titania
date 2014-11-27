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
#include "X3DAnimationEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DAnimationEditorInterface::m_widgetName = "AnimationEditor";

void
X3DAnimationEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_FrameAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FrameAdjustment"));
	m_TextBuffer      = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("TextBuffer"));
	m_TreeStore       = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("TreeStore"));
	m_TreeModelFilter = Glib::RefPtr <Gtk::TreeModelFilter>::cast_dynamic (m_builder -> get_object ("TreeModelFilter"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("NewButton", m_NewButton);
	m_builder -> get_widget ("OpenButton", m_OpenButton);
	m_builder -> get_widget ("AddObjectButton", m_AddObjectButton);
	m_builder -> get_widget ("CutButton", m_CutButton);
	m_builder -> get_widget ("CopyButton", m_CopyButton);
	m_builder -> get_widget ("PasteButton", m_PasteButton);
	m_builder -> get_widget ("PlayButton", m_PlayButton);
	m_builder -> get_widget ("FrameSpinButton", m_FrameSpinButton);
	m_builder -> get_widget ("AnimationBox", m_AnimationBox);
	m_builder -> get_widget ("TreeView", m_TreeView);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("DrawingArea", m_DrawingArea);
	m_builder -> get_widget ("RedButton", m_RedButton);
	m_builder -> get_widget ("GreenButton", m_GreenButton);
	m_builder -> get_widget ("BlueButton", m_BlueButton);
	m_builder -> get_widget ("NewDialog", m_NewDialog);
	m_builder -> get_widget ("NewCancelButton", m_NewCancelButton);
	m_builder -> get_widget ("NewOkButton", m_NewOkButton);
	m_builder -> get_widget ("NewNameEntry", m_NewNameEntry);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_unmap));

	// Connect object Gtk::ToolButton with id 'NewButton'.
	m_NewButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_new));
	m_OpenButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_open));
	m_AddObjectButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_add_object));

	// Connect object Gtk::Entry with id 'NewNameEntry'.
	m_NewNameEntry -> signal_changed () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_new_name_changed));
	m_NewNameEntry -> signal_delete_text () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_new_name_delete_text), false);
	m_NewNameEntry -> signal_insert_text () .connect (sigc::mem_fun (*this, &X3DAnimationEditorInterface::on_new_name_insert_text), false);

	// Call construct handler of base class.
	construct ();
}

X3DAnimationEditorInterface::~X3DAnimationEditorInterface ()
{
	delete m_Window;
	delete m_NewDialog;
}

} // puck
} // titania
