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

#include "ScriptEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/NodeIndex/NodeIndex.h"
#include "../Console/Console.h"

#include <Titania/String.h>
#include <gtksourceviewmm/init.h>
#include <gtksourceviewmm/languagemanager.h>
#include <gtksourceviewmm/styleschememanager.h>

#include <gtksourceview/gtksourcesearchsettings.h>
#include <gtksourceview/gtksourcesearchcontext.h>

namespace titania {
namespace puck {

ScriptEditor::ScriptEditor (X3DBrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DScriptEditorInterface (get_ui ("ScriptEditor.xml"), gconf_dir ()),
				X3DScriptEditor (),
	     X3DShaderPartEditor (),
	                modified (false),
	              textBuffer (Gsv::Buffer::create ()),
	                textView (textBuffer),
	          searchSettings (gtk_source_search_settings_new ()),
	           searchContext (gtk_source_search_context_new (textBuffer -> gobj (), searchSettings)),
	               nodeIndex (new NodeIndex (browserWindow)),
	                nodeName (getBrowserWindow (), getNameEntry (), getRenameButton ()),
	                    node (),
	                   index (0),
	                 console (new Console (browserWindow)),
	                    keys (),
	              searchMark (textBuffer -> get_insert ()),
	        searchConnection ()
{
	Gsv::init ();
	setup ();
}

void
ScriptEditor::initialize ()
{
	X3DScriptEditorInterface::initialize ();
	X3DScriptEditor::initialize ();
	X3DShaderPartEditor::initialize ();

	if (getConfig () .hasItem ("paned"))
		getPaned () .set_position (getConfig () .getInteger ("paned"));

	if (getConfig () .hasItem ("sidePaned"))
		getSidePaned () .set_position (getConfig () .getInteger ("sidePaned"));

	getTextBuffer () -> get_undo_manager () -> signal_can_undo_changed () .connect (sigc::mem_fun (*this, &ScriptEditor::on_can_undo_changed));
	getTextBuffer () -> get_undo_manager () -> signal_can_redo_changed () .connect (sigc::mem_fun (*this, &ScriptEditor::on_can_redo_changed));

	getTextBuffer () -> set_highlight_syntax (true);
	getTextBuffer () -> set_highlight_matching_brackets (true);
	getTextBuffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme ("tango"));

	getTextView () .signal_focus_in_event ()    .connect (sigc::mem_fun (*this, &ScriptEditor::on_focus_in_event));
	getTextView () .signal_focus_out_event ()   .connect (sigc::mem_fun (*this, &ScriptEditor::on_focus_out_event));
	getTextView () .signal_key_press_event ()   .connect (sigc::mem_fun (*this, &ScriptEditor::on_key_press_event),   false);
	getTextView () .signal_key_release_event () .connect (sigc::mem_fun (*this, &ScriptEditor::on_key_release_event), false);

	getTextView () .set_show_right_margin (true);
	getTextView () .set_right_margin_position (100);
	//getTextView () .set_highlight_current_line (true);
	getTextView () .set_show_line_numbers (true);
	getTextView () .set_show_line_marks (true);
	getTextView () .property_tab_width () = 3;
	getTextView () .set_indent_width (3);
	getTextView () .set_insert_spaces_instead_of_tabs (false);
	getTextView () .set_auto_indent (true);
	getTextView () .set_indent_on_tab (true);
	getTextView () .show ();

	getScrolledWindow () .add (getTextView ());

	nodeIndex -> getNode () .addInterest (this, &ScriptEditor::set_node);
	nodeIndex -> getHeaderBox () .set_visible (false);
	nodeIndex -> getFooterBox () .set_visible (false);
	nodeIndex -> getScrolledWindow () .set_size_request (0, 0);
	nodeIndex -> reparent (getNodeIndexBox (), getWindow ());

	nodeIndex -> setTypes ({
	                          X3D::X3DConstants::Script,
	                          X3D::X3DConstants::ShaderPart,
	                          X3D::X3DConstants::ShaderProgram
								  });

	getSaveButton () .add_accelerator ("clicked", getAccelGroup (), GDK_KEY_S, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	console -> reparent (getConsoleBox (), getWindow ());

	// Search

	gtk_source_search_settings_set_wrap_around (searchSettings, true);
	gtk_source_search_context_set_highlight (searchContext, true);

	getSearchBox () .unparent ();
	getSearchOverlay () .add_overlay (getSearchBox ());

	g_signal_connect (searchContext, "notify::occurrences-count", G_CALLBACK (&ScriptEditor::on_occurences_changed), this);
}

bool
ScriptEditor::isModified () const
{
	if (not node)
	   return false;
	
	if (modified)
		return true;

	const auto   cdata   = node -> getCDATA ();
	const auto   text    = getTextBuffer () -> get_text ();
	const auto & current = cdata -> get1Value (index);

	if (text not_eq current)
	   return true;
	
	return false;
}

void
ScriptEditor::on_map ()
{
	set_label ();
}

void
ScriptEditor::set_label ()
{
	if (not getWidget () .get_mapped ())
		return;

	if (node)
		getBrowserWindow () -> getFooterLabel () .set_text (basic::sprintf (_ ("Script Editor »%s«"), node -> getName () .c_str ()));

	else
		getBrowserWindow () -> getFooterLabel () .set_text (_ ("Script Editor"));
}

void
ScriptEditor::set_node (const X3D::SFNode & value)
{
	X3DScriptEditor::set_node (value);
	X3DShaderPartEditor::set_node (value);

	if (node)
	{
		if (isModified ())
			on_apply_clicked ();

		const auto cdata = node -> getCDATA ();

		cdata -> removeInterest (this, &ScriptEditor::set_cdata);
		cdata -> removeInterest (this, &ScriptEditor::connectCDATA);
		
		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);
		
		if (urlObject)
			urlObject -> checkLoadState () .removeInterest (this, &ScriptEditor::set_loadState);
	}

	node = value;

	nodeName .setNode (node);

	set_label ();

	getScriptEditor () .set_sensitive (node);
	
	if (node)
	{
		const auto cdata = node -> getCDATA ();

		cdata -> addInterest (this, &ScriptEditor::set_cdata);

		set_cdata ();
	
		// Load state

		X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);
		
		if (urlObject)
		{
			urlObject -> checkLoadState () .addInterest (this, &ScriptEditor::set_loadState);
			set_loadState (urlObject -> checkLoadState ());
		}
	}
	else
	{
		on_map ();

		getTextBuffer () -> begin_not_undoable_action ();
		getTextBuffer () -> set_text ("");
		getTextBuffer () -> end_not_undoable_action ();
		
		set_loadState (X3D::NOT_STARTED_STATE);
	}

	isModified (false);
}

bool
ScriptEditor::on_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	return false;
}

bool
ScriptEditor::on_focus_out_event (GdkEventFocus*)
{
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> hasAccelerators (true);
	return false;
}

void
ScriptEditor::on_apply_clicked ()
{
	if (not node)
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Apply Script"));

	apply (undoStep);

	getBrowserWindow () -> addUndoStep (X3D::BrowserPtr (node -> getBrowser ()), undoStep);
}

void
ScriptEditor::apply (const UndoStepPtr & undoStep)
{
	const auto cdata = node -> getCDATA ();
	const auto text  = getTextBuffer () -> get_text ();

	cdata -> removeInterest (this, &ScriptEditor::set_cdata);
	cdata -> addInterest (this, &ScriptEditor::connectCDATA);

	undoStep -> addObjects (node);

	undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
	cdata -> set1Value (index, text);
	undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, *cdata);

	getBrowser () -> println (X3D::SFTime (chrono::now ()) .toUTCString (), ": ", basic::sprintf (_ ("%s »%s« is build."), node -> getTypeName () .c_str (), node -> getName () .c_str ()));

	isModified (false);
}

void
ScriptEditor::on_undo_clicked ()
{
	getTextBuffer () -> undo ();
}

void
ScriptEditor::on_redo_clicked ()
{
	getTextBuffer () -> redo ();
}

void
ScriptEditor::on_can_undo_changed ()
{
	getUndoButton () .set_sensitive (getTextBuffer () -> can_undo ());

	if (getTextBuffer () -> can_undo ())
	{
		if (node)
		{
			isModified (true);
			getBrowserWindow () -> isModified (X3D::BrowserPtr (node -> getBrowser ()), true);
		}
	}
}

void
ScriptEditor::on_can_redo_changed ()
{
	getRedoButton () .set_sensitive (getTextBuffer () -> can_redo ());
}

void
ScriptEditor::set_cdata ()
{
	getTextBuffer () -> begin_not_undoable_action ();

	const auto cdata = node -> getCDATA ();

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::Script:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "application/javascript"));

				if (index >= cdata -> size () or cdata -> get1Value (index) .empty ())
					getTextBuffer () -> set_text ("javascript:\n");
				else
					getTextBuffer () -> set_text (cdata -> get1Value (index));
				break;
			}
			case X3D::X3DConstants::ShaderPart:
			case X3D::X3DConstants::ShaderProgram:
			{
				getTextBuffer () -> set_language (Gsv::LanguageManager::get_default () -> guess_language ("", "text/x-c"));

				if (index >= cdata -> size () or cdata -> get1Value (index) .empty ())
					getTextBuffer () -> set_text ("data:text/plain,\n");
				else
					getTextBuffer () -> set_text (cdata -> get1Value (index));
				break;
			}
			default:
				continue;
		}

		break;
	}

	getTextBuffer () -> end_not_undoable_action ();
}

void
ScriptEditor::connectCDATA (const X3D::MFString & field)
{
	field .removeInterest (this, &ScriptEditor::connectCDATA);
	field .addInterest (this, &ScriptEditor::set_cdata);
}

void
ScriptEditor::on_loadState_clicked ()
{
	getBrowserWindow () -> getFooterNotebook () .set_current_page (0);
}

void
ScriptEditor::set_loadState (const X3D::LoadState loadState)
{
	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-stop"));
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-refresh"));
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-yes"));
			break;
		}
		case X3D::FAILED_STATE:
		{
			getLoadStateButton () .set_stock_id (Gtk::StockID ("gtk-no"));
			break;
		}
	}
}

/*
 * Search and Replace
 */

bool
ScriptEditor::on_search_entry_focus_in_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (false);

	searchMark = getTextBuffer () -> create_mark (getTextBuffer () -> get_iter_at_mark (getTextBuffer () -> get_insert ()));

	return false;
}

bool
ScriptEditor::on_search_entry_focus_out_event (GdkEventFocus*)
{
	getBrowserWindow () -> hasAccelerators (true);

	getTextBuffer () -> delete_mark (searchMark);

	// Don't leave the mark alone.
	searchMark = getTextBuffer () -> get_insert ();

	return false;
}

bool
ScriptEditor::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);

	switch (event -> keyval)
	{
		case GDK_KEY_f:
		{
			if (keys .control ())
				getToggleReplaceButton () .set_active (false);
		
			break;
		}
		default:
			break;
	}

	switch (event -> keyval)
	{
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
ScriptEditor::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	return false;
}

void
ScriptEditor::on_enable_search ()
{
	Gsv::Buffer::iterator selectionBegin;
	Gsv::Buffer::iterator selectionEnd;

	getTextBuffer () -> get_selection_bounds (selectionBegin, selectionEnd);

	const auto selection = getTextBuffer () -> get_text (selectionBegin, selectionEnd);

	if (selection .size ())
	{
		getSearchEntry () .set_text (selection);
		gtk_source_search_settings_set_search_text (searchSettings, selection .c_str ());
	}

	searchConnection = getSearchEntry () .signal_changed () .connect (sigc::mem_fun (*this, &ScriptEditor::on_search_entry_changed));

	getSearchEntry () .grab_focus ();
	getSearchBox ()   .set_reveal_child (true);

	if (selection .size ())
		getTextBuffer () -> select_range (selectionBegin, selectionEnd);
}

void
ScriptEditor::on_replace_toggled ()
{
	const bool active = getToggleReplaceButton () .get_active ();

	getReplaceEntry ()       .set_visible (active);
	getReplaceButtonsBox ()  .set_visible (active);
	getToggleReplaceImage () .set (Gtk::StockID (active ? "gtk-yes" : "gtk-no"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
}

void
ScriptEditor::on_search_entry_changed ()
{
	gtk_source_search_settings_set_search_text (searchSettings, getSearchEntry () .get_text () .c_str ());

	on_search_forward_clicked (searchMark);
}

void
ScriptEditor::on_occurences_changed (GObject* gobject, GParamSpec* pspec, gpointer user_data)
{
	const auto self           = static_cast <ScriptEditor*> (user_data);
	const bool hasOccurrences = gtk_source_search_context_get_occurrences_count (self -> searchContext) > 0;

	self -> getSearchBackwardButton () .set_sensitive (hasOccurrences);
	self -> getSearchForwardButton ()  .set_sensitive (hasOccurrences);
	self -> getReplaceButton ()        .set_sensitive (hasOccurrences);
	self -> getReplaceAllButton ()     .set_sensitive (hasOccurrences);
}

void
ScriptEditor::on_search_backward_clicked ()
{
	const auto insertIter = getTextBuffer () -> get_iter_at_mark (getTextBuffer () -> get_insert ());

	gtk_source_search_context_backward_async (searchContext, insertIter .gobj (), nullptr, &ScriptEditor::on_search_backward_callback, this);
}

void
ScriptEditor::on_search_backward_callback (GObject* const source_object, GAsyncResult* const result, const gpointer self)
{
	static_cast <ScriptEditor*> (self) -> on_search_backward (result);
}

void
ScriptEditor::on_search_backward (GAsyncResult* const result)
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
ScriptEditor::on_search_forward_clicked ()
{
	Gsv::Buffer::iterator selectionBegin;
	Gsv::Buffer::iterator selectionEnd;

	if (getTextBuffer () -> get_selection_bounds (selectionBegin, selectionEnd))
		getTextBuffer () -> place_cursor (selectionEnd);

	on_search_forward_clicked (getTextBuffer () -> get_insert ());
}

void
ScriptEditor::on_search_forward_clicked (const Glib::RefPtr <Gsv::Buffer::Mark> & mark)
{
	const auto insertIter = getTextBuffer () -> get_iter_at_mark (mark);

	gtk_source_search_context_forward_async (searchContext, insertIter .gobj (), nullptr, &ScriptEditor::on_search_forward_callback, this);
}

void
ScriptEditor::on_search_forward_callback (GObject* const source_object, GAsyncResult* const result, const gpointer self)
{
	static_cast <ScriptEditor*> (self) -> on_search_forward (result);
}

void
ScriptEditor::on_search_forward (GAsyncResult* const result)
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
ScriptEditor::on_hide_search_clicked ()
{
	searchConnection .disconnect ();

	getSearchBox ()           .set_reveal_child (false);
	getToggleReplaceButton () .set_active (false);
	getTextView ()            .grab_focus ();

	gtk_source_search_settings_set_search_text (searchSettings, nullptr);
}

/*
 * Destructiond
 */

ScriptEditor::~ScriptEditor ()
{
	getConfig () .setItem ("paned",     getPaned ()     .get_position ());
	getConfig () .setItem ("sidePaned", getSidePaned () .get_position ());

	//how to delete searchContext;
	//how to delete searchSettings;

	dispose ();
}

} // puck
} // titania
