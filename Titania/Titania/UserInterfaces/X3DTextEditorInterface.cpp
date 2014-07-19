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
#include "X3DTextEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DTextEditorInterface::m_widgetName = "TextEditor";

void
X3DTextEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_FamilyListStore              = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FamilyListStore"));
	m_MaxExtentAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("MaxExtentAdjustment"));
	m_SizeAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SizeAdjustment"));
	m_SpacingAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpacingAdjustment"));
	m_StringTextBuffer             = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("StringTextBuffer"));
	m_FamilySelection              = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FamilySelection"));
	m_FamilyNameColumn             = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FamilyNameColumn"));
	m_FamilyNameCellrendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("FamilyNameCellrendererText"));
	m_FamilyPadColumn              = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FamilyPadColumn"));
	m_FamilyFontColumn             = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FamilyFontColumn"));
	m_FamilyFontCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("FamilyFontCellrendererPixbuf"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("TextExpander", m_TextExpander);
	m_TextExpander -> set_name ("TextExpander");
	m_builder -> get_widget ("TextCheckButton", m_TextCheckButton);
	m_TextCheckButton -> set_name ("TextCheckButton");
	m_builder -> get_widget ("TextUnlinkButton", m_TextUnlinkButton);
	m_TextUnlinkButton -> set_name ("TextUnlinkButton");
	m_builder -> get_widget ("TextBox", m_TextBox);
	m_TextBox -> set_name ("TextBox");
	m_builder -> get_widget ("MaxExtentSpinButton", m_MaxExtentSpinButton);
	m_MaxExtentSpinButton -> set_name ("MaxExtentSpinButton");
	m_builder -> get_widget ("StringTextView", m_StringTextView);
	m_StringTextView -> set_name ("StringTextView");
	m_builder -> get_widget ("LenghtBox", m_LenghtBox);
	m_LenghtBox -> set_name ("LenghtBox");
	m_builder -> get_widget ("FontStyleExpander", m_FontStyleExpander);
	m_FontStyleExpander -> set_name ("FontStyleExpander");
	m_builder -> get_widget ("FontStyleBox", m_FontStyleBox);
	m_FontStyleBox -> set_name ("FontStyleBox");
	m_builder -> get_widget ("FontStyleButton", m_FontStyleButton);
	m_FontStyleButton -> set_name ("FontStyleButton");
	m_builder -> get_widget ("FontStyleUnlinkButton", m_FontStyleUnlinkButton);
	m_FontStyleUnlinkButton -> set_name ("FontStyleUnlinkButton");
	m_builder -> get_widget ("FontStyleNodeBox", m_FontStyleNodeBox);
	m_FontStyleNodeBox -> set_name ("FontStyleNodeBox");
	m_builder -> get_widget ("SizeLabel", m_SizeLabel);
	m_SizeLabel -> set_name ("SizeLabel");
	m_builder -> get_widget ("SizeSpinButton", m_SizeSpinButton);
	m_SizeSpinButton -> set_name ("SizeSpinButton");
	m_builder -> get_widget ("SpacingSpinButton", m_SpacingSpinButton);
	m_SpacingSpinButton -> set_name ("SpacingSpinButton");
	m_builder -> get_widget ("HorizontalCheckButton", m_HorizontalCheckButton);
	m_HorizontalCheckButton -> set_name ("HorizontalCheckButton");
	m_builder -> get_widget ("LeftToRightCheckButton", m_LeftToRightCheckButton);
	m_LeftToRightCheckButton -> set_name ("LeftToRightCheckButton");
	m_builder -> get_widget ("TopToBottomCheckButton", m_TopToBottomCheckButton);
	m_TopToBottomCheckButton -> set_name ("TopToBottomCheckButton");
	m_builder -> get_widget ("MajorAlignmentButton", m_MajorAlignmentButton);
	m_MajorAlignmentButton -> set_name ("MajorAlignmentButton");
	m_builder -> get_widget ("MinorAlignmentButton", m_MinorAlignmentButton);
	m_MinorAlignmentButton -> set_name ("MinorAlignmentButton");
	m_builder -> get_widget ("BoldToggleButton", m_BoldToggleButton);
	m_BoldToggleButton -> set_name ("BoldToggleButton");
	m_builder -> get_widget ("ItalicToggleButton", m_ItalicToggleButton);
	m_ItalicToggleButton -> set_name ("ItalicToggleButton");
	m_builder -> get_widget ("FamilyTreeView", m_FamilyTreeView);
	m_FamilyTreeView -> set_name ("FamilyTreeView");
	m_builder -> get_widget ("AddFamilyButton", m_AddFamilyButton);
	m_AddFamilyButton -> set_name ("AddFamilyButton");
	m_builder -> get_widget ("RemoveFamilyButton", m_RemoveFamilyButton);
	m_RemoveFamilyButton -> set_name ("RemoveFamilyButton");
	m_builder -> get_widget ("FamilyChooserDialog", m_FamilyChooserDialog);
	m_FamilyChooserDialog -> set_name ("FamilyChooserDialog");

	// Connect object Gtk::Adjustment with id 'MaxExtentAdjustment'.
	m_MaxExtentAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_maxExtent_changed));
	m_SizeAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_size_changed));
	m_SpacingAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_spacing_changed));

	// Connect object Gtk::TextBuffer with id 'StringTextBuffer'.
	m_StringTextBuffer -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_string_changed));

	// Connect object Gtk::CheckButton with id 'TextCheckButton'.
	m_TextCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_text_toggled));

	// Connect object Gtk::Button with id 'TextUnlinkButton'.
	m_TextUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_text_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'FontStyleButton'.
	m_FontStyleButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_fontStyle_changed));

	// Connect object Gtk::Button with id 'FontStyleUnlinkButton'.
	m_FontStyleUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_fontStyle_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'HorizontalCheckButton'.
	m_HorizontalCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_horizontal_toggled));
	m_LeftToRightCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_leftToRight_toggled));
	m_TopToBottomCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_topToBottom_toggled));

	// Connect object Gtk::ComboBoxText with id 'MajorAlignmentButton'.
	m_MajorAlignmentButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_justify_changed));
	m_MinorAlignmentButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_justify_changed));

	// Connect object Gtk::ToggleButton with id 'BoldToggleButton'.
	m_BoldToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_style_toggled));
	m_ItalicToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_style_toggled));

	// Connect object Gtk::TreeView with id 'FamilyTreeView'.
	m_FamilyTreeView -> signal_button_release_event () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_button_release_event));
	m_FamilyTreeView -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_drag_data_received));

	// Connect object Gtk::TreeSelection with id 'FamilySelection'.
	m_FamilySelection -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_changed));

	// Connect object Gtk::CellRendererText with id 'FamilyNameCellrendererText'.
	m_FamilyNameCellrendererText -> signal_edited () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_edited));

	// Connect object Gtk::Button with id 'AddFamilyButton'.
	m_AddFamilyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_add_family_clicked));
	m_RemoveFamilyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_remove_family_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DTextEditorInterface::~X3DTextEditorInterface ()
{
	delete m_Window;
	delete m_FamilyChooserDialog;
}

} // puck
} // titania
