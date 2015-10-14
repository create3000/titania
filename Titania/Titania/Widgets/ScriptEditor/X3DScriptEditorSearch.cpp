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

#include "X3DScriptEditorSearch.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/Stream/Base64.h>
#include <Titania/String.h>

#include <gtksourceview/gtksourcesearchcontext.h>
#include <gtksourceview/gtksourcesearchsettings.h>

namespace titania {
namespace puck {

constexpr size_t RECENT_SEARCHES_MAX = 12;

X3DScriptEditorSearch::X3DScriptEditorSearch () :
	X3DScriptEditorInterface (),
	          searchSettings (),
	           searchContext (),
	                    keys (),
	              searchMark (),
	        searchConnection (),
	          recentSearches ()
{
}

void
X3DScriptEditorSearch::initialize ()
{
	// Initialize members

	searchSettings = gtk_source_search_settings_new ();
	searchContext  = gtk_source_search_context_new (getTextBuffer () -> gobj (), searchSettings);
	searchMark     = getTextBuffer () -> get_insert ();

	getTextView () .signal_key_press_event ()   .connect (sigc::mem_fun (*this, &X3DScriptEditorSearch::on_key_press_event),   false);
	getTextView () .signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DScriptEditorSearch::on_key_release_event), false);
	getTextView () .signal_size_allocate ()     .connect (sigc::mem_fun (*this, &X3DScriptEditorSearch::on_size_allocate));

	// Search & Replace

	getSearchRevealer () .unparent ();
	getSearchOverlay () .add_overlay (getSearchRevealer ());

	g_signal_connect (searchContext, "notify::occurrences-count", G_CALLBACK (&X3DScriptEditorSearch::on_occurences_changed), this);

	gtk_source_search_context_set_highlight (searchContext, true);

	// Search Widget

	getToggleReplaceButton () .set_active (getConfig () .getBoolean ("replaceEnabled"));

	if (getConfig () .getBoolean ("searchEnabled"))
		on_enable_search ();

	// Search Menu

	getCaseSensitiveMenuItem ()      .set_active (getConfig () .getBoolean ("searchCaseSensitive"));
	getAtWordBoundariesMenuItem ()   .set_active (getConfig () .getBoolean ("searchAtWordBoundaries"));
	getRegularExpressionMenuItem ()  .set_active (getConfig () .getBoolean ("searchRegularExpression"));
	getWithinSelectionMenuItem ()    .set_active (getConfig () .getBoolean ("searchWithinSelection"));
	getWrapAroundMenuItemMenuItem () .set_active (getConfig () .getBoolean ("searchWrapAround"));

	recentSearches = basic::basic_split <Glib::ustring, std::deque> (getConfig () .getString ("recentSearches"), ";");

	for (auto search : recentSearches)
		search = basic::base64_decode (search);

	// Restore search and replace strings

	getSearchEntry ()  .set_text (getConfig () .getString ("searchString"));
	getReplaceEntry () .set_text (getConfig () .getString ("replaceString"));

	// Search Menu Icon (workaround issue, as settings are not applied from builder)

	getSearchEntry () .set_icon_activatable (true, Gtk::ENTRY_ICON_PRIMARY);
	getSearchEntry () .set_icon_sensitive   (Gtk::ENTRY_ICON_PRIMARY, true);
}

bool
X3DScriptEditorSearch::on_search_entry_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (false);

	searchMark = getTextBuffer () -> create_mark (getTextBuffer () -> get_iter_at_mark (getTextBuffer () -> get_insert ()));

	return false;
}

bool
X3DScriptEditorSearch::on_search_entry_focus_out_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (true);

	getTextBuffer () -> delete_mark (searchMark);

	// Don't leave the mark alone.
	searchMark = getTextBuffer () -> get_insert ();

	return false;
}

bool
X3DScriptEditorSearch::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);

	switch (event -> keyval)
	{
		case GDK_KEY_Escape:
		{
			on_hide_search_clicked ();
			return true;
		}
		case GDK_KEY_h:
		{
			if (keys .control ())
			{
				getToggleReplaceButton () .set_active (true);
				on_enable_search ();
				return true;
			}

			break;
		}
		case GDK_KEY_f:
		{
			if (keys .control ())
			{
				getToggleReplaceButton () .set_active (false);
				on_enable_search ();
				return true;
			}

			break;
		}
		case GDK_KEY_g:
		{
			if (keys .control ())
			{
				on_search_forward_clicked ();
				return true;
			}

			break;
		}
		case GDK_KEY_G:
		{
			if (keys .control ())
			{
				on_search_backward_clicked ();
				return true;
			}

			break;
		}
		default:
			break;
	}

	return false;
}

bool
X3DScriptEditorSearch::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	return false;
}

void
X3DScriptEditorSearch::on_size_allocate (const Gtk::Allocation & allocation)
{
	const auto width = allocation .get_width () * (2 - math::M_PHI);
	const auto box   = getSearchBox ()   .get_allocation () .get_width ();
	const auto entry = getSearchEntry () .get_allocation () .get_width ();

	getSearchEntry ()  .set_size_request (width * entry / box, -1);
	getReplaceEntry () .set_size_request (width * entry / box, -1);
}

void
X3DScriptEditorSearch::on_enable_search ()
{
	searchConnection .disconnect ();

	Gsv::Buffer::iterator selectionBegin;
	Gsv::Buffer::iterator selectionEnd;

	getTextBuffer () -> get_selection_bounds (selectionBegin, selectionEnd);

	std::string selection = getTextBuffer () -> get_text (selectionBegin, selectionEnd);

	if (selection .size ())
	{
		if (getRegularExpressionMenuItem () .get_active ())
			selection = pcrecpp::RE::QuoteMeta (selection);

		on_add_search (selection);
		getSearchEntry () .set_text (selection);
		gtk_source_search_settings_set_search_text (searchSettings, selection .c_str ());
	}

	searchConnection = getSearchEntry () .signal_changed () .connect (sigc::mem_fun (*this, &X3DScriptEditorSearch::on_search_entry_changed));

	on_size_allocate (getTextView () .get_allocation ());
	getSearchEntry ()    .grab_focus ();
	getSearchRevealer () .set_reveal_child (true);

	if (selection .size ())
		getTextBuffer () -> select_range (selectionBegin, selectionEnd);
}

void
X3DScriptEditorSearch::on_replace_toggled ()
{
	const bool active = getToggleReplaceButton () .get_active ();

	getReplaceEntry ()       .set_visible (active);
	getReplaceButtonsBox ()  .set_visible (active);
	getToggleReplaceImage () .set (Gtk::StockID (active ? "gtk-yes" : "gtk-no"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
}

void
X3DScriptEditorSearch::on_search_menu_icon_released (Gtk::EntryIconPosition icon_position, const GdkEventButton* event)
{
	switch (icon_position)
	{
		case Gtk::ENTRY_ICON_PRIMARY:
			on_build_search_menu ();
			getSearchMenu () .popup (event -> button, event -> time);
			break;
		case Gtk::ENTRY_ICON_SECONDARY:
			break;
	}
}

void
X3DScriptEditorSearch::on_build_search_menu ()
{
	// Remove menu items

	const auto menuItems = getSearchMenu () .get_children ();
	const auto size      = menuItems .size ();
	size_t     i         = 0;

	for (auto size = menuItems .size (); i < size; ++ i)
	{
		const auto separator = dynamic_cast <Gtk::SeparatorMenuItem*> (menuItems [i]);

		if (separator)
			break;
	}

	i += 2;

	for (; i < size; ++ i)
		getSearchMenu () .remove (*menuItems [i]);

	// Add menu items

	for (const auto search : recentSearches)
	{
		const auto menuItem = Gtk::manage (new Gtk::MenuItem (search));
		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (*this, &X3DScriptEditorSearch::on_search_activate), search));
		menuItem -> show ();

		getSearchMenu () .append (*menuItem);
	}
}

void
X3DScriptEditorSearch::on_search_activate (const Glib::ustring & search)
{
	getSearchEntry () .set_text (search);
}

void
X3DScriptEditorSearch::on_add_search (const Glib::ustring & search)
{
	// Add search to recentSearches

	recentSearches .emplace_front (search);

	// Constrain recentSearches

	if (recentSearches .size () > RECENT_SEARCHES_MAX)
		recentSearches .pop_back ();
}

void
X3DScriptEditorSearch::on_search_case_sensitve_toggled ()
{
	gtk_source_search_settings_set_case_sensitive (searchSettings, getCaseSensitiveMenuItem () .get_active ());
}

void
X3DScriptEditorSearch::on_search_at_word_boundaries_toggled ()
{
	gtk_source_search_settings_set_at_word_boundaries (searchSettings, getAtWordBoundariesMenuItem () .get_active ());
}

void
X3DScriptEditorSearch::on_search_regex_toggled ()
{
	gtk_source_search_settings_set_regex_enabled (searchSettings, getRegularExpressionMenuItem () .get_active ());
}

void
X3DScriptEditorSearch::on_search_within_selection_toggled ()
{ }

void
X3DScriptEditorSearch::on_search_wrap_around_toggled ()
{
	gtk_source_search_settings_set_wrap_around (searchSettings, getWrapAroundMenuItemMenuItem () .get_active ());
}

void
X3DScriptEditorSearch::on_search_entry_changed ()
{
	on_add_search (getSearchEntry () .get_text ());

	gtk_source_search_settings_set_search_text (searchSettings, getSearchEntry () .get_text () .c_str ());

	on_search_forward_clicked (searchMark);
}

void
X3DScriptEditorSearch::on_occurences_changed (GObject* gobject, GParamSpec* pspec, gpointer user_data)
{
	const auto self           = static_cast <X3DScriptEditorSearch*> (user_data);
	const bool hasOccurrences = gtk_source_search_context_get_occurrences_count (self -> searchContext) > 0;

	self -> getSearchBackwardButton () .set_sensitive (hasOccurrences);
	self -> getSearchForwardButton ()  .set_sensitive (hasOccurrences);
	self -> getReplaceButton ()        .set_sensitive (hasOccurrences);
	self -> getReplaceAllButton ()     .set_sensitive (hasOccurrences);
}

void
X3DScriptEditorSearch::on_search_backward_clicked ()
{
	const auto insertIter = getTextBuffer () -> get_iter_at_mark (getTextBuffer () -> get_insert ());

	gtk_source_search_context_backward_async (searchContext, insertIter .gobj (), nullptr, &X3DScriptEditorSearch::on_search_backward_callback, this);
}

void
X3DScriptEditorSearch::on_search_backward_callback (GObject* const source_object, GAsyncResult* const result, const gpointer self)
{
	static_cast <X3DScriptEditorSearch*> (self) -> on_search_backward (result);
}

void
X3DScriptEditorSearch::on_search_backward (GAsyncResult* const result)
{
	Gsv::Buffer::iterator matchBegin;
	Gsv::Buffer::iterator matchEnd;

	GError* error = nullptr;

	if (not gtk_source_search_context_backward_finish (searchContext, result, matchBegin .gobj (), matchEnd .gobj (), &error))
		return;

	const auto match = getTextBuffer () -> get_text (matchBegin, matchEnd);

	getTextView () .scroll_to (matchBegin, 0, 0.5, 2 - math::M_PHI);
	getTextBuffer () -> select_range (matchBegin, matchEnd);
}

void
X3DScriptEditorSearch::on_search_forward_clicked ()
{
	Gsv::Buffer::iterator selectionBegin;
	Gsv::Buffer::iterator selectionEnd;

	if (getTextBuffer () -> get_selection_bounds (selectionBegin, selectionEnd))
		getTextBuffer () -> place_cursor (selectionEnd);

	on_search_forward_clicked (getTextBuffer () -> get_insert ());
}

void
X3DScriptEditorSearch::on_search_forward_clicked (const Glib::RefPtr <Gsv::Buffer::Mark> & mark)
{
	const auto insertIter = getTextBuffer () -> get_iter_at_mark (mark);

	gtk_source_search_context_forward_async (searchContext, insertIter .gobj (), nullptr, &X3DScriptEditorSearch::on_search_forward_callback, this);
}

void
X3DScriptEditorSearch::on_search_forward_callback (GObject* const source_object, GAsyncResult* const result, const gpointer self)
{
	static_cast <X3DScriptEditorSearch*> (self) -> on_search_forward (result);
}

void
X3DScriptEditorSearch::on_search_forward (GAsyncResult* const result)
{
	Gsv::Buffer::iterator matchBegin;
	Gsv::Buffer::iterator matchEnd;

	GError* error = nullptr;

	if (not gtk_source_search_context_forward_finish (searchContext, result, matchBegin .gobj (), matchEnd .gobj (), &error))
		return;

	const auto match = getTextBuffer () -> get_text (matchBegin, matchEnd);

	getTextView () .scroll_to (matchBegin, 0, 0.5, 2 - math::M_PHI);
	getTextBuffer () -> select_range (matchBegin, matchEnd);
}

void
X3DScriptEditorSearch::on_hide_search_clicked ()
{
	getSearchRevealer ()      .set_reveal_child (false);
	getToggleReplaceButton () .set_active (false);
	getTextView ()            .grab_focus ();

	gtk_source_search_settings_set_search_text (searchSettings, nullptr);
}

X3DScriptEditorSearch::~X3DScriptEditorSearch ()
{
	// Search & Replace

	getConfig () .setItem ("searchEnabled",  getSearchRevealer ()      .get_child_revealed ());
	getConfig () .setItem ("replaceEnabled", getToggleReplaceButton () .get_active ());

	getConfig () .setItem ("searchString",  getSearchEntry ()  .get_text ());
	getConfig () .setItem ("replaceString", getReplaceEntry () .get_text ());

	getConfig () .setItem ("searchCaseSensitive",     getCaseSensitiveMenuItem ()      .get_active ());
	getConfig () .setItem ("searchAtWordBoundaries",  getAtWordBoundariesMenuItem ()   .get_active ());
	getConfig () .setItem ("searchRegularExpression", getRegularExpressionMenuItem ()  .get_active ());
	getConfig () .setItem ("searchWithinSelection",   getWithinSelectionMenuItem ()    .get_active ());
	getConfig () .setItem ("searchWrapAround",        getWrapAroundMenuItemMenuItem () .get_active ());

	for (auto search : recentSearches)
		search = basic::base64_encode (search);

	getConfig () .setItem ("recentSearches", basic::join (recentSearches, ";"));

	//how to delete searchContext;
	//how to delete searchSettings;
}

} // puck
} // titania
