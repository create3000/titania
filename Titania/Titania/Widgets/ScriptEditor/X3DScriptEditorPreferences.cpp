/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#include "X3DScriptEditorPreferences.h"

#include <gtksourceview/gtksource.h>

namespace titania {
namespace puck {

namespace ColorTheme {
   static constexpr size_t ID          = 0;
   static constexpr size_t NAME        = 1;
   static constexpr size_t DESCRIPTION = 2;
   static constexpr size_t WEIGHT      = 3;
}

namespace Weight {

static constexpr int NORMAL = 400;
static constexpr int BOLD   = 700;

};

X3DScriptEditorPreferences::X3DScriptEditorPreferences () :
	X3DScriptEditor (),
	    themeIndex (0)
{
}

void
X3DScriptEditorPreferences::initialize ()
{ }

void
X3DScriptEditorPreferences::configure ()
{
	// View

	if (getConfig () -> hasItem ("showLineNumbers"))
		 getShowLineNumbersCheckButton () .set_active (getConfig () -> getItem <bool> ("showLineNumbers"));
	else
		 getShowLineNumbersCheckButton () .set_active (true);

	if (getConfig () -> hasItem ("showRightMargin"))
		 getShowRightMarginCheckButton () .set_active (getConfig () -> getItem <bool> ("showRightMargin"));
	else
		getShowRightMarginCheckButton () .set_active (true);

	if (getConfig () -> hasItem ("rightMarginPosition"))
		 getRightMarginSpinButton () .set_value (getConfig () -> getItem <int32_t> ("rightMarginPosition"));
	else
		getRightMarginSpinButton () .set_value (100);
	
	if (getConfig () -> hasItem ("wrapMode"))
		getWrapModeComboBoxText () .set_active (getConfig () -> getItem <int32_t> ("wrapMode"));
	else
		getWrapModeComboBoxText () .set_active (0);

	if (getConfig () -> hasItem ("highlightCurrentLine"))
		getHighlightCurrentLineCheckButton () .set_active (getConfig () -> getItem <bool> ("highlightCurrentLine"));
	else
		getHighlightCurrentLineCheckButton () .set_active (false);
	
	if (getConfig () -> hasItem ("highlightMatchingBrackets"))
		getHighlightMatchingBracketsCheckButton () .set_active (getConfig () -> getItem <bool> ("highlightMatchingBrackets"));
	else
		getHighlightMatchingBracketsCheckButton () .set_active (true);

	// Editor

	if (getConfig () -> hasItem ("tabWidth"))
		getTabWidthSpinButton () .set_value (getConfig () -> getItem <int32_t> ("tabWidth"));
	else
		getTabWidthSpinButton () .set_value (3);
	
	if (getConfig () -> hasItem ("insertSpacesInsteadOfTabs"))
		getInsertSpacesInsteadOfTabsCheckButton () .set_active (getConfig () -> getItem <bool> ("insertSpacesInsteadOfTabs"));
	else
		getInsertSpacesInsteadOfTabsCheckButton () .set_active (false);

	// Color Themes

	std::string themeId = "x_ite";
	
	if (getConfig () -> hasItem ("colorTheme"))
		themeId = getConfig () -> getItem <std::string> ("colorTheme");

	getTextBuffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme (themeId));

	// Build color themes list view

	size_t i = 0;
	
	for (const auto & id : Gsv::StyleSchemeManager::get_default () -> get_scheme_ids ())
	{
		const auto scheme = Gsv::StyleSchemeManager::get_default () -> get_scheme (id);
		const auto iter   = getColorThemeListStore () -> append ();

		iter -> set_value (ColorTheme::ID,          id);
		iter -> set_value (ColorTheme::NAME,        scheme -> get_name ());
		iter -> set_value (ColorTheme::DESCRIPTION, scheme -> get_description ());

		if (id == themeId)
		{
		   themeIndex = i;
			iter -> set_value (ColorTheme::WEIGHT, Weight::BOLD);
		}
		else
			iter -> set_value (ColorTheme::WEIGHT, Weight::NORMAL);
		
		++ i;
	}

	// Defaults

	getTextBuffer () -> set_highlight_syntax (true);

	getTextView () .set_show_line_marks (true);
	getTextView () .set_auto_indent (true);
	getTextView () .set_indent_on_tab (true);
}

void
X3DScriptEditorPreferences::on_preferences_clicked ()
{
	getPreferencesDialog () .show ();
}

bool
X3DScriptEditorPreferences::on_preferences_delete_event (GdkEventAny*)
{
	getPreferencesDialog () .hide ();
	return true;
}

void
X3DScriptEditorPreferences::on_show_line_numbers_toggled ()
{
	getConfig () -> setItem ("showLineNumbers", getShowLineNumbersCheckButton () .get_active ());

	getTextView () .set_show_line_numbers (getShowLineNumbersCheckButton () .get_active ());
}

void
X3DScriptEditorPreferences::on_show_right_margin_toggled ()
{
	getConfig () -> setItem ("showRightMargin", getShowRightMarginCheckButton () .get_active ());

	getTextView () .set_show_right_margin (getShowRightMarginCheckButton () .get_active ());
}

void
X3DScriptEditorPreferences::on_right_margin_changed ()
{
	getConfig () -> setItem ("rightMarginPosition", (int) getRightMarginSpinButton () .get_value ());

	getTextView () .set_right_margin_position (getRightMarginSpinButton () .get_value ());
}

void
X3DScriptEditorPreferences::on_wrap_mode_changed ()
{
	static std::map <uint8_t, Gtk::WrapMode> wrapModes = {
		std::pair (0, Gtk::WRAP_NONE),
		std::pair (1, Gtk::WRAP_CHAR),
		std::pair (2, Gtk::WRAP_WORD),
		std::pair (3, Gtk::WRAP_WORD_CHAR),
	};
		
	try
	{
		getTextView () .set_wrap_mode (wrapModes .at (getWrapModeComboBoxText () .get_active_row_number ()));

		getConfig () -> setItem ("wrapMode", getWrapModeComboBoxText () .get_active_row_number ());
	}
	catch (const std::out_of_range &)
	{
		getTextView () .set_wrap_mode (Gtk::WRAP_NONE);

		getConfig () -> setItem ("wrapMode", Gtk::WRAP_NONE);  
	}
}

void
X3DScriptEditorPreferences::on_highlight_current_line_togged ()
{
	getConfig () -> setItem ("highlightCurrentLine", getHighlightCurrentLineCheckButton () .get_active ());

	getTextView () .set_highlight_current_line (getHighlightCurrentLineCheckButton () .get_active ());
}

void
X3DScriptEditorPreferences::on_highlight_matching_brackets_toggled ()
{
	getConfig () -> setItem ("highlightMatchingBrackets", getHighlightMatchingBracketsCheckButton () .get_active ());

	getTextBuffer () -> set_highlight_matching_brackets (getHighlightMatchingBracketsCheckButton () .get_active ());
}

void
X3DScriptEditorPreferences::on_tab_width_changed ()
{
	getConfig () -> setItem ("tabWidth", (int) getTabWidthSpinButton () .get_value ());

	getTextView () .property_tab_width () = getTabWidthSpinButton () .get_value ();
	getTextView () .set_indent_width (getTabWidthSpinButton () .get_value ());
}

void
X3DScriptEditorPreferences::on_insert_spaces_instead_of_tabs_toggled ()
{
	getConfig () -> setItem ("insertSpacesInsteadOfTabs", getInsertSpacesInsteadOfTabsCheckButton () .get_active ());

	getTextView () .set_insert_spaces_instead_of_tabs (getInsertSpacesInsteadOfTabsCheckButton () .get_active ());
}

void
X3DScriptEditorPreferences::on_color_theme_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	// Unset old theme

	Gtk::TreePath previousPath;

	previousPath .push_back (themeIndex);

	const auto previousIter = getColorThemeListStore () -> get_iter (previousPath);

	previousIter -> set_value (ColorTheme::WEIGHT, Weight::NORMAL);

	// Set new theme

	themeIndex = path .front ();

	Gtk::TreePath newPath;

	newPath .push_back (themeIndex);

	const auto  newIter = getColorThemeListStore () -> get_iter (newPath);
	std::string themeId;

	newIter -> set_value (ColorTheme::WEIGHT, Weight::BOLD);
	newIter -> get_value (ColorTheme::ID,     themeId);

	getTextBuffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme (themeId));

	getConfig () -> setItem ("colorTheme", themeId);
}

X3DScriptEditorPreferences::~X3DScriptEditorPreferences ()
{ }

} // puck
} // titania
