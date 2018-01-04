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

#ifndef __TITANIA_WIDGETS_SCRIPT_EDITOR_X3DSCRIPT_EDITOR_SEARCH_H__
#define __TITANIA_WIDGETS_SCRIPT_EDITOR_X3DSCRIPT_EDITOR_SEARCH_H__

#include "X3DScriptEditor.h"

#include <Titania/X3D/Browser/KeyDeviceSensor/Keys.h>

namespace titania {
namespace puck {

class X3DScriptEditorSearch :
	virtual public X3DScriptEditor
{
public:

	virtual
	~X3DScriptEditorSearch () override;


protected:

	///  @name Construction

	X3DScriptEditorSearch ();

	virtual
	void
	initialize () override;

	virtual
	void
	configure () override;

	///  @name Destruction

	virtual
	void
	store () override;


private:

	///  @name Event handlers

	void
	on_size_allocate (const Gtk::Allocation &);

	virtual
	bool
	on_key_press_event (GdkEventKey*) final override;

	virtual
	bool
	on_key_release_event (GdkEventKey*) final override;

	virtual
	bool
	on_entry_focus_in_event (GdkEventFocus*) final override;

	virtual
	bool
	on_entry_focus_out_event (GdkEventFocus*) final override;

	///  @name Search

	void
	on_enable_search ();

	virtual
	void
	on_replace_toggled () final override;

	virtual
	void
	on_search_menu_icon_released (Gtk::EntryIconPosition, const GdkEventButton*) final override;

	void
	on_build_search_menu ();

	void
	on_search_activate (const Glib::ustring & search, const Glib::ustring & replace);

	void
	on_add_recent_search ();

	virtual
	void
	on_search_case_sensitve_toggled () final override;

	virtual
	void
	on_search_at_word_boundaries_toggled () final override;

	virtual
	void
	on_search_regex_toggled () final override;

	virtual
	void
	on_search_within_selection_toggled () final override;

	virtual
	void
	on_search_wrap_around_toggled () final override;

	void
	on_search_entry_changed ();

	static
	void
	   on_occurences_changed (GObject*, GParamSpec*, gpointer);

	virtual
	void
	on_search_backward_clicked () final override;

	static
	void
	on_search_backward_callback (GObject* const, GAsyncResult* const, const gpointer);

	void
	on_search_backward (GAsyncResult* const);

	virtual
	void
	on_search_forward_clicked () final override;

	void
	on_search_forward_clicked (const Glib::RefPtr <Gsv::Buffer::Mark> &);

	static
	void
	on_search_forward_callback (GObject* const, GAsyncResult* const, const gpointer);

	void
	on_search_forward (GAsyncResult* const);

	virtual
	void
	on_hide_search_clicked () final override;

	///  @name Replace

	virtual
	void
	on_replace_forward_clicked () final override;

	bool
	on_button_press_event (GdkEventButton*);

	virtual
	void
	on_replace_all_clicked () final override;

	///  @name Go to line

	void
	on_enable_go_to_line ();

	virtual
	void
	on_go_to_line_insert_text (const Glib::ustring &, int*) final override;

	virtual
	void
	on_go_to_line_button_clicked () final override;

	virtual
	bool
	on_go_to_line_key_press_event (GdkEventKey*) final override;

	///  @name Members

	GtkSourceSearchSettings* searchSettings;
	GtkSourceSearchContext* searchContext;

	X3D::Keys                        keys;
	Glib::RefPtr <Gsv::Buffer::Mark> searchMark;
	sigc::connection                 searchConnection;
	std::deque <Glib::ustring>       recentSearches;
	std::deque <Glib::ustring>       recentReplaces;
	bool replace;

};

} // puck
} // titania

#endif
