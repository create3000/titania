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

#include "X3DAnimationEditorInterface.h"

namespace titania {
namespace puck {

void
X3DAnimationEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DAnimationEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DAnimationEditorInterface::create ()
{
	// Get objects.
	m_AccelGroup            = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("AccelGroup"));
	m_DurationAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DurationAdjustment"));
	m_FrameAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FrameAdjustment"));
	m_FrameRateAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FrameRateAdjustment"));
	m_TranslationAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TranslationAdjustment"));
	m_TreeViewSelection     = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("TreeViewSelection"));
	m_NameColumn            = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("NameColumn"));
	m_IconCellRenderer      = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconCellRenderer"));
	m_NameCellRenderer      = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameCellRenderer"));
	m_TaintedColumn         = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TaintedColumn"));
	m_TaintedCellRenderer   = Glib::RefPtr <Gtk::CellRendererToggle>::cast_dynamic (m_builder -> get_object ("TaintedCellRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("NewButton", m_NewButton);
	m_builder -> get_widget ("AddMemberButton", m_AddMemberButton);
	m_builder -> get_widget ("RemoveMemberButton", m_RemoveMemberButton);
	m_builder -> get_widget ("CutButton", m_CutButton);
	m_builder -> get_widget ("CopyButton", m_CopyButton);
	m_builder -> get_widget ("PasteButton", m_PasteButton);
	m_builder -> get_widget ("FirstFrameButton", m_FirstFrameButton);
	m_builder -> get_widget ("PlayPauseButton", m_PlayPauseButton);
	m_builder -> get_widget ("LastFrameButton", m_LastFrameButton);
	m_builder -> get_widget ("FrameSpinButton", m_FrameSpinButton);
	m_builder -> get_widget ("TimeButton", m_TimeButton);
	m_builder -> get_widget ("KeyTypeButton", m_KeyTypeButton);
	m_builder -> get_widget ("TimeLabel", m_TimeLabel);
	m_builder -> get_widget ("AnimationBox", m_AnimationBox);
	m_builder -> get_widget ("Notebook", m_Notebook);
	m_builder -> get_widget ("NodeIndexBox", m_NodeIndexBox);
	m_builder -> get_widget ("TreeView", m_TreeView);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("DrawingAreaBox", m_DrawingAreaBox);
	m_builder -> get_widget ("DrawingArea", m_DrawingArea);
	m_builder -> get_widget ("TranslationSlider", m_TranslationSlider);
	m_builder -> get_widget ("ToolBar2", m_ToolBar2);
	m_builder -> get_widget ("ZoomOutButton", m_ZoomOutButton);
	m_builder -> get_widget ("ZoomInButton", m_ZoomInButton);
	m_builder -> get_widget ("ZoomFitButton", m_ZoomFitButton);
	m_builder -> get_widget ("Zoom100Button", m_Zoom100Button);
	m_builder -> get_widget ("ToolBar3", m_ToolBar3);
	m_builder -> get_widget ("CloseAnimationButton", m_CloseAnimationButton);
	m_builder -> get_widget ("PropertiesDialog", m_PropertiesDialog);
	m_builder -> get_widget ("NewCancelButton", m_NewCancelButton);
	m_builder -> get_widget ("NewOkButton", m_NewOkButton);
	m_builder -> get_widget ("PropertiesHeaderBar", m_PropertiesHeaderBar);
	m_builder -> get_widget ("NewNameEntry", m_NewNameEntry);
	m_builder -> get_widget ("DurationSspinButton", m_DurationSspinButton);
	m_builder -> get_widget ("ScaleKeyframesButton", m_ScaleKeyframesButton);
	m_builder -> get_widget ("LoopSwitch", m_LoopSwitch);
	m_builder -> get_widget ("CycleIntervalLabel", m_CycleIntervalLabel);
	m_builder -> get_widget ("FPSSpinButton", m_FPSSpinButton);

	// Connect object Gtk::Adjustment with id 'FrameAdjustment'.
	m_FrameAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_current_frame_changed));
	m_TranslationAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_translation_changed));

	// Connect object Gtk::ToolButton with id 'NewButton'.
	m_NewButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new));
	m_AddMemberButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_add_member));
	m_RemoveMemberButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_remove_member));
	m_CutButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_cut));
	m_CopyButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_copy));
	m_PasteButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_paste));
	m_FirstFrameButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_first_frame));
	m_PlayPauseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_play_pause));
	m_LastFrameButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_last_frame));
	m_TimeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_time));

	// Connect object Gtk::ComboBoxText with id 'KeyTypeButton'.
	m_KeyTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_key_type_changed));

	// Connect object Gtk::TreeView with id 'TreeView'.
	m_TreeView -> signal_draw () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_tree_view_draw));
	m_TreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_row_activated));

	// Connect object Gtk::TreeSelection with id 'TreeViewSelection'.
	m_TreeViewSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_tree_view_selection_changed));

	// Connect object Gtk::CellRendererToggle with id 'TaintedCellRenderer'.
	m_TaintedCellRenderer -> signal_toggled () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_tainted_toggled));

	// Connect object Gtk::DrawingArea with id 'DrawingArea'.
	m_DrawingArea -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_button_press_event));
	m_DrawingArea -> signal_button_release_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_button_release_event));
	m_DrawingArea -> signal_configure_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_configure_event));
	m_DrawingArea -> signal_draw () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_draw));
	m_DrawingArea -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_focus_in_event));
	m_DrawingArea -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_focus_out_event));
	m_DrawingArea -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_key_press_event));
	m_DrawingArea -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_key_release_event));
	m_DrawingArea -> signal_motion_notify_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_motion_notify_event));
	m_DrawingArea -> signal_scroll_event () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_scroll_event));

	// Connect object Gtk::ToolButton with id 'ZoomOutButton'.
	m_ZoomOutButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_zoom_out));
	m_ZoomInButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_zoom_in));
	m_ZoomFitButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_zoom_fit));
	m_Zoom100Button -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_zoom_100));
	m_CloseAnimationButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_close));

	// Connect object Gtk::Entry with id 'NewNameEntry'.
	m_NewNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new_name_changed));
	m_NewNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new_name_delete_text), false);
	m_NewNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new_name_insert_text), false);

	// Connect object Gtk::SpinButton with id 'DurationSspinButton'.
	m_DurationSspinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new_cycle_interval_changed));
	m_FPSSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DAnimationEditorInterface::on_new_cycle_interval_changed));
}

X3DAnimationEditorInterface::~X3DAnimationEditorInterface ()
{
	delete m_Window;
	delete m_PropertiesDialog;
}

} // puck
} // titania
