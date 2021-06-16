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

#include "X3DTextEditorInterface.h"

namespace titania {
namespace puck {

void
X3DTextEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DTextEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DTextEditorInterface::create ()
{
	// Get objects.
	m_FontStyleBezierDimensionAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStyleBezierDimensionAdjustment"));
	m_FontStyleFamilyListStore           = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyListStore"));
	m_FontStylePointSizeAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStylePointSizeAdjustment"));
	m_FontStyleSizeAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStyleSizeAdjustment"));
	m_FontStyleSpacingAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FontStyleSpacingAdjustment"));
	m_TextCharSpacingAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextCharSpacingAdjustment"));
	m_TextMaxExtentAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextMaxExtentAdjustment"));
	m_TextStringTextBuffer               = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("TextStringTextBuffer"));
	m_FontStyleFamilySelection           = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FontStyleFamilySelection"));
	m_FontStyleFamilyColumn              = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyColumn"));
	m_FontStyleFamilyCellrendererText    = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyCellrendererText"));
	m_FontStyleFamilyChooserColumn       = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFamilyChooserColumn"));
	m_FontStyleFileChooserColumn         = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FontStyleFileChooserColumn"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("TextNotebook", m_TextNotebook);
	m_builder -> get_widget ("TextExpander", m_TextExpander);
	m_builder -> get_widget ("SelectTextBox", m_SelectTextBox);
	m_builder -> get_widget ("TextCheckButton", m_TextCheckButton);
	m_builder -> get_widget ("TextUnlinkButton", m_TextUnlinkButton);
	m_builder -> get_widget ("TextBox", m_TextBox);
	m_builder -> get_widget ("TextMaxExtentSpinButton", m_TextMaxExtentSpinButton);
	m_builder -> get_widget ("TextStringTextView", m_TextStringTextView);
	m_builder -> get_widget ("LenghtBox", m_LenghtBox);
	m_builder -> get_widget ("TextCharSpacingSpinButton", m_TextCharSpacingSpinButton);
	m_builder -> get_widget ("TextWrappingCheckButton", m_TextWrappingCheckButton);
	m_builder -> get_widget ("FontStyleExpander", m_FontStyleExpander);
	m_builder -> get_widget ("FontStyleBox", m_FontStyleBox);
	m_builder -> get_widget ("SelectFontStyleBox", m_SelectFontStyleBox);
	m_builder -> get_widget ("FontStyleComboBoxText", m_FontStyleComboBoxText);
	m_builder -> get_widget ("FontStyleUnlinkButton", m_FontStyleUnlinkButton);
	m_builder -> get_widget ("FontStyleNodeBox", m_FontStyleNodeBox);
	m_builder -> get_widget ("FontStyleSizeLabel", m_FontStyleSizeLabel);
	m_builder -> get_widget ("FontStylePointSizeLabel", m_FontStylePointSizeLabel);
	m_builder -> get_widget ("FontStyleSizeSpinButton", m_FontStyleSizeSpinButton);
	m_builder -> get_widget ("FontStyleSpacingSpinButton", m_FontStyleSpacingSpinButton);
	m_builder -> get_widget ("FontStyleHorizontalCheckButton", m_FontStyleHorizontalCheckButton);
	m_builder -> get_widget ("FontStyleLeftToRightCheckButton", m_FontStyleLeftToRightCheckButton);
	m_builder -> get_widget ("FontStyleTopToBottomCheckButton", m_FontStyleTopToBottomCheckButton);
	m_builder -> get_widget ("FontStyleMajorAlignmentComboBoxText", m_FontStyleMajorAlignmentComboBoxText);
	m_builder -> get_widget ("FontStyleMinorAlignmentComboBoxText", m_FontStyleMinorAlignmentComboBoxText);
	m_builder -> get_widget ("FontStyleBoldToggleButton", m_FontStyleBoldToggleButton);
	m_builder -> get_widget ("FontStyleItalicToggleButton", m_FontStyleItalicToggleButton);
	m_builder -> get_widget ("FontStyleFamilyTreeView", m_FontStyleFamilyTreeView);
	m_builder -> get_widget ("FontStyleFamilyAddButton", m_FontStyleFamilyAddButton);
	m_builder -> get_widget ("FontStyleFamilyRemoveButton", m_FontStyleFamilyRemoveButton);
	m_builder -> get_widget ("FontStylePointSizeSpinButton", m_FontStylePointSizeSpinButton);
	m_builder -> get_widget ("FontStyleGlobalOptionsBox", m_FontStyleGlobalOptionsBox);
	m_builder -> get_widget ("FontStyleBezierDimensionSpinButton", m_FontStyleBezierDimensionSpinButton);

	// Connect object Gtk::Adjustment with id 'TextCharSpacingAdjustment'.
	m_TextCharSpacingAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_char_spacing_changed));

	// Connect object Gtk::CheckButton with id 'TextCheckButton'.
	m_TextCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_text_toggled));

	// Connect object Gtk::Button with id 'TextUnlinkButton'.
	m_TextUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_text_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'FontStyleComboBoxText'.
	m_FontStyleComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_fontStyle_changed));

	// Connect object Gtk::Button with id 'FontStyleUnlinkButton'.
	m_FontStyleUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_fontStyle_unlink_clicked));

	// Connect object Gtk::ToggleButton with id 'FontStyleBoldToggleButton'.
	m_FontStyleBoldToggleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_style_toggled));
	m_FontStyleItalicToggleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextEditorInterface::on_style_toggled));
}

X3DTextEditorInterface::~X3DTextEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
