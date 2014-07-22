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
	m_FontStyleFamilyListStore              = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyListStore"));
	m_FontStyleSizeAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStyleSizeAdjustment"));
	m_FontStyleSpacingAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStyleSpacingAdjustment"));
	m_TextMaxExtentAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextMaxExtentAdjustment"));
	m_TextStringTextBuffer                  = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("TextStringTextBuffer"));
	m_FontStyleFamilySelection              = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FontStyleFamilySelection"));
	m_FontStyleFamilyNameColumn             = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyNameColumn"));
	m_FontStyleFamilyNameCellrendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyNameCellrendererText"));
	m_FontStyleFamilyPadColumn              = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyPadColumn"));
	m_FontStyleFamilyFontColumn             = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyFontColumn"));
	m_FontStyleFamilyFontCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyFontCellrendererPixbuf"));

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
	m_builder -> get_widget ("TextMaxExtentSpinButton", m_TextMaxExtentSpinButton);
	m_TextMaxExtentSpinButton -> set_name ("TextMaxExtentSpinButton");
	m_builder -> get_widget ("TextStringTextView", m_TextStringTextView);
	m_TextStringTextView -> set_name ("TextStringTextView");
	m_builder -> get_widget ("LenghtBox", m_LenghtBox);
	m_LenghtBox -> set_name ("LenghtBox");
	m_builder -> get_widget ("FontStyleExpander", m_FontStyleExpander);
	m_FontStyleExpander -> set_name ("FontStyleExpander");
	m_builder -> get_widget ("FontStyleBox", m_FontStyleBox);
	m_FontStyleBox -> set_name ("FontStyleBox");
	m_builder -> get_widget ("FontStyleComboBoxText", m_FontStyleComboBoxText);
	m_FontStyleComboBoxText -> set_name ("FontStyleComboBoxText");
	m_builder -> get_widget ("FontStyleUnlinkButton", m_FontStyleUnlinkButton);
	m_FontStyleUnlinkButton -> set_name ("FontStyleUnlinkButton");
	m_builder -> get_widget ("FontStyleNodeBox", m_FontStyleNodeBox);
	m_FontStyleNodeBox -> set_name ("FontStyleNodeBox");
	m_builder -> get_widget ("SizeLabel", m_SizeLabel);
	m_SizeLabel -> set_name ("SizeLabel");
	m_builder -> get_widget ("FontStyleSizeSpinButton", m_FontStyleSizeSpinButton);
	m_FontStyleSizeSpinButton -> set_name ("FontStyleSizeSpinButton");
	m_builder -> get_widget ("FontStyleSpacingSpinButton", m_FontStyleSpacingSpinButton);
	m_FontStyleSpacingSpinButton -> set_name ("FontStyleSpacingSpinButton");
	m_builder -> get_widget ("FontStyleHorizontalCheckButton", m_FontStyleHorizontalCheckButton);
	m_FontStyleHorizontalCheckButton -> set_name ("FontStyleHorizontalCheckButton");
	m_builder -> get_widget ("FontStyleLeftToRightCheckButton", m_FontStyleLeftToRightCheckButton);
	m_FontStyleLeftToRightCheckButton -> set_name ("FontStyleLeftToRightCheckButton");
	m_builder -> get_widget ("FontStyleTopToBottomCheckButton", m_FontStyleTopToBottomCheckButton);
	m_FontStyleTopToBottomCheckButton -> set_name ("FontStyleTopToBottomCheckButton");
	m_builder -> get_widget ("FontStyleMajorAlignmentComboBoxText", m_FontStyleMajorAlignmentComboBoxText);
	m_FontStyleMajorAlignmentComboBoxText -> set_name ("FontStyleMajorAlignmentComboBoxText");
	m_builder -> get_widget ("FontStyleMinorAlignmentComboBoxText", m_FontStyleMinorAlignmentComboBoxText);
	m_FontStyleMinorAlignmentComboBoxText -> set_name ("FontStyleMinorAlignmentComboBoxText");
	m_builder -> get_widget ("FontStyleBoldToggleButton", m_FontStyleBoldToggleButton);
	m_FontStyleBoldToggleButton -> set_name ("FontStyleBoldToggleButton");
	m_builder -> get_widget ("FontStyleItalicToggleButton", m_FontStyleItalicToggleButton);
	m_FontStyleItalicToggleButton -> set_name ("FontStyleItalicToggleButton");
	m_builder -> get_widget ("FontStyleFamilyTreeView", m_FontStyleFamilyTreeView);
	m_FontStyleFamilyTreeView -> set_name ("FontStyleFamilyTreeView");
	m_builder -> get_widget ("FontStyleAddFamilyButton", m_FontStyleAddFamilyButton);
	m_FontStyleAddFamilyButton -> set_name ("FontStyleAddFamilyButton");
	m_builder -> get_widget ("FontStyleRemoveFamilyButton", m_FontStyleRemoveFamilyButton);
	m_FontStyleRemoveFamilyButton -> set_name ("FontStyleRemoveFamilyButton");
	m_builder -> get_widget ("FamilyChooserDialog", m_FamilyChooserDialog);
	m_FamilyChooserDialog -> set_name ("FamilyChooserDialog");

	// Connect object Gtk::Adjustment with id 'FontStyleSizeAdjustment'.
	m_FontStyleSizeAdjustment -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_size_changed));

	// Connect object Gtk::TextBuffer with id 'TextStringTextBuffer'.
	m_TextStringTextBuffer -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_string_changed));

	// Connect object Gtk::CheckButton with id 'TextCheckButton'.
	m_TextCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_text_toggled));

	// Connect object Gtk::Button with id 'TextUnlinkButton'.
	m_TextUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_text_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'FontStyleComboBoxText'.
	m_FontStyleComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_fontStyle_changed));

	// Connect object Gtk::Button with id 'FontStyleUnlinkButton'.
	m_FontStyleUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_fontStyle_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'FontStyleMajorAlignmentComboBoxText'.
	m_FontStyleMajorAlignmentComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_justify_changed));
	m_FontStyleMinorAlignmentComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_justify_changed));

	// Connect object Gtk::ToggleButton with id 'FontStyleBoldToggleButton'.
	m_FontStyleBoldToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_style_toggled));
	m_FontStyleItalicToggleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_style_toggled));

	// Connect object Gtk::TreeView with id 'FontStyleFamilyTreeView'.
	m_FontStyleFamilyTreeView -> signal_button_release_event () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_button_release_event));
	m_FontStyleFamilyTreeView -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_drag_data_received));

	// Connect object Gtk::TreeSelection with id 'FontStyleFamilySelection'.
	m_FontStyleFamilySelection -> signal_changed () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_changed));

	// Connect object Gtk::CellRendererText with id 'FontStyleFamilyNameCellrendererText'.
	m_FontStyleFamilyNameCellrendererText -> signal_edited () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_family_edited));

	// Connect object Gtk::Button with id 'FontStyleAddFamilyButton'.
	m_FontStyleAddFamilyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_add_family_clicked));
	m_FontStyleRemoveFamilyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DTextEditorInterface::on_remove_family_clicked));

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
