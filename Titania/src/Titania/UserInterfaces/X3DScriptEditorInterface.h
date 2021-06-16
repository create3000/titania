/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TMP_GLAD2CPP_SCRIPT_EDITOR_H__
#define __TMP_GLAD2CPP_SCRIPT_EDITOR_H__

#include "../Base/X3DUserInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for ScriptEditor.
 */
class X3DScriptEditorInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DScriptEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DScriptEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DScriptEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getAccelGroup () const
	{ return m_AccelGroup; }

	const Glib::RefPtr <Gtk::ListStore> &
	getColorThemeListStore () const
	{ return m_ColorThemeListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRightMarginAdjustment () const
	{ return m_RightMarginAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTabWidthAdjustment () const
	{ return m_TabWidthAdjustment; }

	Gtk::Menu &
	getSearchMenu () const
	{ return *m_SearchMenu; }

	Gtk::CheckMenuItem &
	getCaseSensitiveMenuItem () const
	{ return *m_CaseSensitiveMenuItem; }

	Gtk::CheckMenuItem &
	getAtWordBoundariesMenuItem () const
	{ return *m_AtWordBoundariesMenuItem; }

	Gtk::CheckMenuItem &
	getRegularExpressionMenuItem () const
	{ return *m_RegularExpressionMenuItem; }

	Gtk::CheckMenuItem &
	getWithinSelectionMenuItem () const
	{ return *m_WithinSelectionMenuItem; }

	Gtk::CheckMenuItem &
	getWrapAroundMenuItemMenuItem () const
	{ return *m_WrapAroundMenuItemMenuItem; }

	Gtk::MenuItem &
	getRecentSearchesMenuItem () const
	{ return *m_RecentSearchesMenuItem; }

	Gtk::Image &
	getToggleReplaceImage () const
	{ return *m_ToggleReplaceImage; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::Paned &
	getPaned () const
	{ return *m_Paned; }

	Gtk::Paned &
	getSidePaned () const
	{ return *m_SidePaned; }

	Gtk::Box &
	getNodeIndexBox () const
	{ return *m_NodeIndexBox; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Box &
	getConsoleBox () const
	{ return *m_ConsoleBox; }

	Gtk::Box &
	getScriptEditorBox () const
	{ return *m_ScriptEditorBox; }

	Gtk::Overlay &
	getSearchOverlay () const
	{ return *m_SearchOverlay; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_NewButton; }

	Gtk::Toolbar &
	getToolbar () const
	{ return *m_Toolbar; }

	Gtk::ToolButton &
	getApplyButton () const
	{ return *m_ApplyButton; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_RedoButton; }

	Gtk::ToggleToolButton &
	getDirectOutputToggleButton () const
	{ return *m_DirectOutputToggleButton; }

	Gtk::ToggleToolButton &
	getMustEvaluateToggleButton () const
	{ return *m_MustEvaluateToggleButton; }

	Gtk::ToolButton &
	getShaderTypeButton () const
	{ return *m_ShaderTypeButton; }

	Gtk::ToolButton &
	getPreferencesButton () const
	{ return *m_PreferencesButton; }

	Gtk::Popover &
	getNewScriptPopover () const
	{ return *m_NewScriptPopover; }

	Gtk::Button &
	getNewScriptButton () const
	{ return *m_NewScriptButton; }

	Gtk::Button &
	getNewShaderPartButton1 () const
	{ return *m_NewShaderPartButton1; }

	Gtk::Button &
	getNewShaderPartButton () const
	{ return *m_NewShaderPartButton; }

	Gtk::Button &
	getNewShaderProgramButton1 () const
	{ return *m_NewShaderProgramButton1; }

	Gtk::Button &
	getNewShaderProgramButton () const
	{ return *m_NewShaderProgramButton; }

	Gtk::Dialog &
	getPreferencesDialog () const
	{ return *m_PreferencesDialog; }

	Gtk::HeaderBar &
	getPreferencesHeaderBar () const
	{ return *m_PreferencesHeaderBar; }

	Gtk::Expander &
	getWordWrapExpander () const
	{ return *m_WordWrapExpander; }

	Gtk::CheckButton &
	getShowLineNumbersCheckButton () const
	{ return *m_ShowLineNumbersCheckButton; }

	Gtk::CheckButton &
	getShowRightMarginCheckButton () const
	{ return *m_ShowRightMarginCheckButton; }

	Gtk::SpinButton &
	getRightMarginSpinButton () const
	{ return *m_RightMarginSpinButton; }

	Gtk::ComboBoxText &
	getWrapModeComboBoxText () const
	{ return *m_WrapModeComboBoxText; }

	Gtk::CheckButton &
	getHighlightCurrentLineCheckButton () const
	{ return *m_HighlightCurrentLineCheckButton; }

	Gtk::CheckButton &
	getHighlightMatchingBracketsCheckButton () const
	{ return *m_HighlightMatchingBracketsCheckButton; }

	Gtk::Expander &
	getEditorExpander () const
	{ return *m_EditorExpander; }

	Gtk::SpinButton &
	getTabWidthSpinButton () const
	{ return *m_TabWidthSpinButton; }

	Gtk::CheckButton &
	getInsertSpacesInsteadOfTabsCheckButton () const
	{ return *m_InsertSpacesInsteadOfTabsCheckButton; }

	Gtk::Expander &
	getColorThemeExpander () const
	{ return *m_ColorThemeExpander; }

	Gtk::TreeView &
	getColorThemeTreeView () const
	{ return *m_ColorThemeTreeView; }

	Gtk::Popover &
	getShaderTypePopover () const
	{ return *m_ShaderTypePopover; }

	Gtk::Button &
	getVertexShaderButton () const
	{ return *m_VertexShaderButton; }

	Gtk::Button &
	getTessControllShaderButton () const
	{ return *m_TessControllShaderButton; }

	Gtk::Button &
	getTessEvaluationShaderButton () const
	{ return *m_TessEvaluationShaderButton; }

	Gtk::Button &
	getGeometryShaderButton () const
	{ return *m_GeometryShaderButton; }

	Gtk::Button &
	getFragmentShaderButton () const
	{ return *m_FragmentShaderButton; }

	Gtk::Button &
	getComputeShaderButton () const
	{ return *m_ComputeShaderButton; }

	Gtk::Revealer &
	getSearchRevealer () const
	{ return *m_SearchRevealer; }

	Gtk::Box &
	getSearchBox () const
	{ return *m_SearchBox; }

	Gtk::Grid &
	getSearchAndReplaceBox () const
	{ return *m_SearchAndReplaceBox; }

	Gtk::Box &
	getReplaceButtonsBox () const
	{ return *m_ReplaceButtonsBox; }

	Gtk::Button &
	getReplaceButton () const
	{ return *m_ReplaceButton; }

	Gtk::Button &
	getReplaceAllButton () const
	{ return *m_ReplaceAllButton; }

	Gtk::SearchEntry &
	getReplaceEntry () const
	{ return *m_ReplaceEntry; }

	Gtk::Button &
	getSearchBackwardButton () const
	{ return *m_SearchBackwardButton; }

	Gtk::Button &
	getSearchForwardButton () const
	{ return *m_SearchForwardButton; }

	Gtk::Button &
	getHideSearchButton () const
	{ return *m_HideSearchButton; }

	Gtk::SearchEntry &
	getSearchEntry () const
	{ return *m_SearchEntry; }

	Gtk::ToggleButton &
	getToggleReplaceButton () const
	{ return *m_ToggleReplaceButton; }

	Gtk::Box &
	getGoToLineBox () const
	{ return *m_GoToLineBox; }

	Gtk::SearchEntry &
	getGoToLineEntry () const
	{ return *m_GoToLineEntry; }

	Gtk::Button &
	getGoToLineButton () const
	{ return *m_GoToLineButton; }

	Gtk::Button &
	getHideGoToLineButton () const
	{ return *m_HideGoToLineButton; }

	///  @name Signal handlers

	virtual
	void
	on_search_case_sensitve_toggled () = 0;

	virtual
	void
	on_search_at_word_boundaries_toggled () = 0;

	virtual
	void
	on_search_regex_toggled () = 0;

	virtual
	void
	on_search_within_selection_toggled () = 0;

	virtual
	void
	on_search_wrap_around_toggled () = 0;

	virtual
	void
	on_new_clicked () = 0;

	virtual
	void
	on_apply_clicked () = 0;

	virtual
	void
	on_undo_clicked () = 0;

	virtual
	void
	on_redo_clicked () = 0;

	virtual
	void
	on_shader_type_clicked () = 0;

	virtual
	void
	on_preferences_clicked () = 0;

	virtual
	void
	on_new_script_clicked () = 0;

	virtual
	void
	on_new_composed_shader_clicked () = 0;

	virtual
	void
	on_new_shader_part_clicked () = 0;

	virtual
	void
	on_new_program_shader_clicked () = 0;

	virtual
	void
	on_new_shader_program_clicked () = 0;

	virtual
	bool
	on_preferences_delete_event (GdkEventAny* any_event) = 0;

	virtual
	void
	on_show_line_numbers_toggled () = 0;

	virtual
	void
	on_show_right_margin_toggled () = 0;

	virtual
	void
	on_right_margin_changed () = 0;

	virtual
	void
	on_wrap_mode_changed () = 0;

	virtual
	void
	on_highlight_current_line_togged () = 0;

	virtual
	void
	on_highlight_matching_brackets_toggled () = 0;

	virtual
	void
	on_tab_width_changed () = 0;

	virtual
	void
	on_insert_spaces_instead_of_tabs_toggled () = 0;

	virtual
	void
	on_color_theme_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_vertex_clicked () = 0;

	virtual
	void
	on_tess_control_clicked () = 0;

	virtual
	void
	on_tess_evaluate_clicked () = 0;

	virtual
	void
	on_geometry_clicked () = 0;

	virtual
	void
	on_fragment_clicked () = 0;

	virtual
	void
	on_compute_clicked () = 0;

	virtual
	void
	on_replace_forward_clicked () = 0;

	virtual
	void
	on_replace_all_clicked () = 0;

	virtual
	bool
	on_entry_focus_in_event (GdkEventFocus* focus_event) = 0;

	virtual
	bool
	on_entry_focus_out_event (GdkEventFocus* gdk_event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_search_backward_clicked () = 0;

	virtual
	void
	on_search_forward_clicked () = 0;

	virtual
	void
	on_hide_search_clicked () = 0;

	virtual
	void
	on_search_menu_icon_released (Gtk::EntryIconPosition icon_position, const GdkEventButton* event) = 0;

	virtual
	void
	on_replace_toggled () = 0;

	virtual
	void
	on_go_to_line_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	bool
	on_go_to_line_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_go_to_line_button_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DScriptEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::AccelGroup> m_AccelGroup;
	Glib::RefPtr <Gtk::ListStore> m_ColorThemeListStore;
	Glib::RefPtr <Gtk::Adjustment> m_RightMarginAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TabWidthAdjustment;
	Gtk::Menu* m_SearchMenu;
	Gtk::CheckMenuItem* m_CaseSensitiveMenuItem;
	Gtk::CheckMenuItem* m_AtWordBoundariesMenuItem;
	Gtk::CheckMenuItem* m_RegularExpressionMenuItem;
	Gtk::CheckMenuItem* m_WithinSelectionMenuItem;
	Gtk::CheckMenuItem* m_WrapAroundMenuItemMenuItem;
	Gtk::MenuItem* m_RecentSearchesMenuItem;
	Gtk::Image* m_ToggleReplaceImage;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Paned* m_Paned;
	Gtk::Paned* m_SidePaned;
	Gtk::Box* m_NodeIndexBox;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Box* m_ConsoleBox;
	Gtk::Box* m_ScriptEditorBox;
	Gtk::Overlay* m_SearchOverlay;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::ToolButton* m_NewButton;
	Gtk::Toolbar* m_Toolbar;
	Gtk::ToolButton* m_ApplyButton;
	Gtk::ToolButton* m_UndoButton;
	Gtk::ToolButton* m_RedoButton;
	Gtk::ToggleToolButton* m_DirectOutputToggleButton;
	Gtk::ToggleToolButton* m_MustEvaluateToggleButton;
	Gtk::ToolButton* m_ShaderTypeButton;
	Gtk::ToolButton* m_PreferencesButton;
	Gtk::Popover* m_NewScriptPopover;
	Gtk::Button* m_NewScriptButton;
	Gtk::Button* m_NewShaderPartButton1;
	Gtk::Button* m_NewShaderPartButton;
	Gtk::Button* m_NewShaderProgramButton1;
	Gtk::Button* m_NewShaderProgramButton;
	Gtk::Dialog* m_PreferencesDialog;
	Gtk::HeaderBar* m_PreferencesHeaderBar;
	Gtk::Expander* m_WordWrapExpander;
	Gtk::CheckButton* m_ShowLineNumbersCheckButton;
	Gtk::CheckButton* m_ShowRightMarginCheckButton;
	Gtk::SpinButton* m_RightMarginSpinButton;
	Gtk::ComboBoxText* m_WrapModeComboBoxText;
	Gtk::CheckButton* m_HighlightCurrentLineCheckButton;
	Gtk::CheckButton* m_HighlightMatchingBracketsCheckButton;
	Gtk::Expander* m_EditorExpander;
	Gtk::SpinButton* m_TabWidthSpinButton;
	Gtk::CheckButton* m_InsertSpacesInsteadOfTabsCheckButton;
	Gtk::Expander* m_ColorThemeExpander;
	Gtk::TreeView* m_ColorThemeTreeView;
	Gtk::Popover* m_ShaderTypePopover;
	Gtk::Button* m_VertexShaderButton;
	Gtk::Button* m_TessControllShaderButton;
	Gtk::Button* m_TessEvaluationShaderButton;
	Gtk::Button* m_GeometryShaderButton;
	Gtk::Button* m_FragmentShaderButton;
	Gtk::Button* m_ComputeShaderButton;
	Gtk::Revealer* m_SearchRevealer;
	Gtk::Box* m_SearchBox;
	Gtk::Grid* m_SearchAndReplaceBox;
	Gtk::Box* m_ReplaceButtonsBox;
	Gtk::Button* m_ReplaceButton;
	Gtk::Button* m_ReplaceAllButton;
	Gtk::SearchEntry* m_ReplaceEntry;
	Gtk::Button* m_SearchBackwardButton;
	Gtk::Button* m_SearchForwardButton;
	Gtk::Button* m_HideSearchButton;
	Gtk::SearchEntry* m_SearchEntry;
	Gtk::ToggleButton* m_ToggleReplaceButton;
	Gtk::Box* m_GoToLineBox;
	Gtk::SearchEntry* m_GoToLineEntry;
	Gtk::Button* m_GoToLineButton;
	Gtk::Button* m_HideGoToLineButton;

};

} // puck
} // titania

#endif
