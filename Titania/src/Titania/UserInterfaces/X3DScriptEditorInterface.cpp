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

#include "X3DScriptEditorInterface.h"

namespace titania {
namespace puck {

void
X3DScriptEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DScriptEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DScriptEditorInterface::create ()
{
	// Get objects.
	m_AccelGroup            = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("AccelGroup"));
	m_ColorThemeListStore   = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ColorThemeListStore"));
	m_RightMarginAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("RightMarginAdjustment"));
	m_TabWidthAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TabWidthAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("SearchMenu", m_SearchMenu);
	m_builder -> get_widget ("CaseSensitiveMenuItem", m_CaseSensitiveMenuItem);
	m_builder -> get_widget ("AtWordBoundariesMenuItem", m_AtWordBoundariesMenuItem);
	m_builder -> get_widget ("RegularExpressionMenuItem", m_RegularExpressionMenuItem);
	m_builder -> get_widget ("WithinSelectionMenuItem", m_WithinSelectionMenuItem);
	m_builder -> get_widget ("WrapAroundMenuItemMenuItem", m_WrapAroundMenuItemMenuItem);
	m_builder -> get_widget ("RecentSearchesMenuItem", m_RecentSearchesMenuItem);
	m_builder -> get_widget ("ToggleReplaceImage", m_ToggleReplaceImage);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Paned", m_Paned);
	m_builder -> get_widget ("SidePaned", m_SidePaned);
	m_builder -> get_widget ("NodeIndexBox", m_NodeIndexBox);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("ConsoleBox", m_ConsoleBox);
	m_builder -> get_widget ("ScriptEditorBox", m_ScriptEditorBox);
	m_builder -> get_widget ("SearchOverlay", m_SearchOverlay);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("NewButton", m_NewButton);
	m_builder -> get_widget ("Toolbar", m_Toolbar);
	m_builder -> get_widget ("ApplyButton", m_ApplyButton);
	m_builder -> get_widget ("UndoButton", m_UndoButton);
	m_builder -> get_widget ("RedoButton", m_RedoButton);
	m_builder -> get_widget ("DirectOutputToggleButton", m_DirectOutputToggleButton);
	m_builder -> get_widget ("MustEvaluateToggleButton", m_MustEvaluateToggleButton);
	m_builder -> get_widget ("ShaderTypeButton", m_ShaderTypeButton);
	m_builder -> get_widget ("PreferencesButton", m_PreferencesButton);
	m_builder -> get_widget ("NewScriptPopover", m_NewScriptPopover);
	m_builder -> get_widget ("NewScriptButton", m_NewScriptButton);
	m_builder -> get_widget ("NewShaderPartButton1", m_NewShaderPartButton1);
	m_builder -> get_widget ("NewShaderPartButton", m_NewShaderPartButton);
	m_builder -> get_widget ("NewShaderProgramButton1", m_NewShaderProgramButton1);
	m_builder -> get_widget ("NewShaderProgramButton", m_NewShaderProgramButton);
	m_builder -> get_widget ("PreferencesDialog", m_PreferencesDialog);
	m_builder -> get_widget ("PreferencesHeaderBar", m_PreferencesHeaderBar);
	m_builder -> get_widget ("WordWrapExpander", m_WordWrapExpander);
	m_builder -> get_widget ("ShowLineNumbersCheckButton", m_ShowLineNumbersCheckButton);
	m_builder -> get_widget ("ShowRightMarginCheckButton", m_ShowRightMarginCheckButton);
	m_builder -> get_widget ("RightMarginSpinButton", m_RightMarginSpinButton);
	m_builder -> get_widget ("WrapModeComboBoxText", m_WrapModeComboBoxText);
	m_builder -> get_widget ("HighlightCurrentLineCheckButton", m_HighlightCurrentLineCheckButton);
	m_builder -> get_widget ("HighlightMatchingBracketsCheckButton", m_HighlightMatchingBracketsCheckButton);
	m_builder -> get_widget ("EditorExpander", m_EditorExpander);
	m_builder -> get_widget ("TabWidthSpinButton", m_TabWidthSpinButton);
	m_builder -> get_widget ("InsertSpacesInsteadOfTabsCheckButton", m_InsertSpacesInsteadOfTabsCheckButton);
	m_builder -> get_widget ("ColorThemeExpander", m_ColorThemeExpander);
	m_builder -> get_widget ("ColorThemeTreeView", m_ColorThemeTreeView);
	m_builder -> get_widget ("ShaderTypePopover", m_ShaderTypePopover);
	m_builder -> get_widget ("VertexShaderButton", m_VertexShaderButton);
	m_builder -> get_widget ("TessControllShaderButton", m_TessControllShaderButton);
	m_builder -> get_widget ("TessEvaluationShaderButton", m_TessEvaluationShaderButton);
	m_builder -> get_widget ("GeometryShaderButton", m_GeometryShaderButton);
	m_builder -> get_widget ("FragmentShaderButton", m_FragmentShaderButton);
	m_builder -> get_widget ("ComputeShaderButton", m_ComputeShaderButton);
	m_builder -> get_widget ("SearchRevealer", m_SearchRevealer);
	m_builder -> get_widget ("SearchBox", m_SearchBox);
	m_builder -> get_widget ("SearchAndReplaceBox", m_SearchAndReplaceBox);
	m_builder -> get_widget ("ReplaceButtonsBox", m_ReplaceButtonsBox);
	m_builder -> get_widget ("ReplaceButton", m_ReplaceButton);
	m_builder -> get_widget ("ReplaceAllButton", m_ReplaceAllButton);
	m_builder -> get_widget ("ReplaceEntry", m_ReplaceEntry);
	m_builder -> get_widget ("SearchBackwardButton", m_SearchBackwardButton);
	m_builder -> get_widget ("SearchForwardButton", m_SearchForwardButton);
	m_builder -> get_widget ("HideSearchButton", m_HideSearchButton);
	m_builder -> get_widget ("SearchEntry", m_SearchEntry);
	m_builder -> get_widget ("ToggleReplaceButton", m_ToggleReplaceButton);
	m_builder -> get_widget ("GoToLineBox", m_GoToLineBox);
	m_builder -> get_widget ("GoToLineEntry", m_GoToLineEntry);
	m_builder -> get_widget ("GoToLineButton", m_GoToLineButton);
	m_builder -> get_widget ("HideGoToLineButton", m_HideGoToLineButton);
	m_CaseSensitiveMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_case_sensitve_toggled));
	m_AtWordBoundariesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_at_word_boundaries_toggled));
	m_RegularExpressionMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_regex_toggled));
	m_WithinSelectionMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_within_selection_toggled));
	m_WrapAroundMenuItemMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_wrap_around_toggled));

	// Connect object Gtk::ToolButton with id 'NewButton'.
	m_NewButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_clicked));
	m_ApplyButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_apply_clicked));
	m_UndoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_undo_clicked));
	m_RedoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_redo_clicked));
	m_ShaderTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_shader_type_clicked));
	m_PreferencesButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_preferences_clicked));

	// Connect object Gtk::Button with id 'NewScriptButton'.
	m_NewScriptButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_script_clicked));
	m_NewShaderPartButton1 -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_composed_shader_clicked));
	m_NewShaderPartButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_shader_part_clicked));
	m_NewShaderProgramButton1 -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_program_shader_clicked));
	m_NewShaderProgramButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_new_shader_program_clicked));

	// Connect object Gtk::Dialog with id 'PreferencesDialog'.
	m_PreferencesDialog -> signal_delete_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_preferences_delete_event));

	// Connect object Gtk::CheckButton with id 'ShowLineNumbersCheckButton'.
	m_ShowLineNumbersCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_show_line_numbers_toggled));
	m_ShowRightMarginCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_show_right_margin_toggled));

	// Connect object Gtk::SpinButton with id 'RightMarginSpinButton'.
	m_RightMarginSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_right_margin_changed));

	// Connect object Gtk::ComboBoxText with id 'WrapModeComboBoxText'.
	m_WrapModeComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_wrap_mode_changed));

	// Connect object Gtk::CheckButton with id 'HighlightCurrentLineCheckButton'.
	m_HighlightCurrentLineCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_highlight_current_line_togged));
	m_HighlightMatchingBracketsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_highlight_matching_brackets_toggled));

	// Connect object Gtk::SpinButton with id 'TabWidthSpinButton'.
	m_TabWidthSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_tab_width_changed));

	// Connect object Gtk::CheckButton with id 'InsertSpacesInsteadOfTabsCheckButton'.
	m_InsertSpacesInsteadOfTabsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_insert_spaces_instead_of_tabs_toggled));

	// Connect object Gtk::TreeView with id 'ColorThemeTreeView'.
	m_ColorThemeTreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_color_theme_activated));

	// Connect object Gtk::Button with id 'VertexShaderButton'.
	m_VertexShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_vertex_clicked));
	m_TessControllShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_tess_control_clicked));
	m_TessEvaluationShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_tess_evaluate_clicked));
	m_GeometryShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_geometry_clicked));
	m_FragmentShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_fragment_clicked));
	m_ComputeShaderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_compute_clicked));
	m_ReplaceButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_replace_forward_clicked));
	m_ReplaceAllButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_replace_all_clicked));

	// Connect object Gtk::SearchEntry with id 'ReplaceEntry'.
	m_ReplaceEntry -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_in_event));
	m_ReplaceEntry -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_out_event));
	m_ReplaceEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_press_event), false);
	m_ReplaceEntry -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_release_event), false);

	// Connect object Gtk::Button with id 'SearchBackwardButton'.
	m_SearchBackwardButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_backward_clicked));
	m_SearchForwardButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_forward_clicked));
	m_HideSearchButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_hide_search_clicked));

	// Connect object Gtk::SearchEntry with id 'SearchEntry'.
	m_SearchEntry -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_in_event));
	m_SearchEntry -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_out_event));
	m_SearchEntry -> signal_icon_release () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_search_menu_icon_released), false);
	m_SearchEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_press_event), false);
	m_SearchEntry -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_release_event), false);

	// Connect object Gtk::ToggleButton with id 'ToggleReplaceButton'.
	m_ToggleReplaceButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_replace_toggled));

	// Connect object Gtk::SearchEntry with id 'GoToLineEntry'.
	m_GoToLineEntry -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_in_event));
	m_GoToLineEntry -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_entry_focus_out_event));
	m_GoToLineEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_go_to_line_insert_text), false);
	m_GoToLineEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_go_to_line_key_press_event), false);
	m_GoToLineEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_press_event), false);
	m_GoToLineEntry -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_key_release_event), false);

	// Connect object Gtk::Button with id 'GoToLineButton'.
	m_GoToLineButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_go_to_line_button_clicked));
	m_HideGoToLineButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DScriptEditorInterface::on_hide_search_clicked));
}

X3DScriptEditorInterface::~X3DScriptEditorInterface ()
{
	delete m_Window;
	delete m_PreferencesDialog;
}

} // puck
} // titania
