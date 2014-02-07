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

#include "BrowserWindow.h"

#include "../Configuration/config.h"
#include "../OutlineEditor/OutlineTreeModel.h"
#include "../MaterialEditor/MaterialEditor.h"
#include "../TextureEditor/TextureEditor.h"
#include "../NodePropertiesEditor/NodePropertiesEditor.h"
#include "BrowserSelection.h"

#include <Titania/OS.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

BrowserWindow::BrowserWindow (const X3D::X3DSFNode <X3D::Browser> & browserSurface, int argc, char** argv) :
	X3DBaseInterface (this, browserSurface),
	X3DBrowserEditor (argc, argv),
	  browserSurface (browserSurface),
	motionBlurEditor (this),
	     libraryView (this),
	   viewpointList (this),
	   historyEditor (this),
	   outlineEditor (this),
	            keys (),
	       shortcuts (true),
	          viewer (X3D::ViewerType::NONE)
{
	if (getConfig () .getBoolean ("transparent"))
		setTransparent (true);
	else
		getBrowserSurface () -> set_antialiasing (4); // 4 x Antialiasing
}

void
BrowserWindow::initialize ()
{
	X3DBrowserEditor::initialize ();

	// User interface

	Gtk::Settings::get_default () -> property_gtk_menu_images ()       = true;
	Gtk::Settings::get_default () -> property_gtk_button_images ()     = true;
	Gtk::Settings::get_default () -> property_gtk_toolbar_style ()     = Gtk::TOOLBAR_ICONS;
	Gtk::Settings::get_default () -> property_gtk_toolbar_icon_size () = Gtk::ICON_SIZE_SMALL_TOOLBAR;

	// FileFilter
	getFileFilterX3D   () -> set_name (_ ("X3D"));
	getFileFilterImage () -> set_name (_ ("Images"));
	getFileFilterAudio () -> set_name (_ ("Audio"));
	getFileFilterVideo () -> set_name (_ ("Videos"));

	// Dialogs
	getFileOpenDialog ()        .set_transient_for (getWindow ());
	getOpenLocationDialog ()    .set_transient_for (getWindow ());
	getFileImportDialog ()      .set_transient_for (getWindow ());
	getFileSaveWarningDialog () .set_transient_for (getWindow ());
	getFileSaveDialog ()        .set_transient_for (getWindow ());
	getMessageDialog ()         .set_transient_for (getWindow ());

	// Sidebar
	getViewpointList () .reparent (getViewpointListBox (), getWindow ());
	getHistoryEditor () .reparent (getHistoryEditorBox (), getWindow ());
	getLibraryView ()   .reparent (getLibraryViewBox (),   getWindow ());
	getOutlineEditor () .reparent (getOutlineEditorBox (), getWindow ());

	// CSS
	Glib::RefPtr <Gtk::CssProvider> cssProvider1 = Gtk::CssProvider::create ();
	cssProvider1 -> load_from_path (get_ui ("style.css"));
	Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider1, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	Glib::RefPtr <Gtk::CssProvider> cssProvider2 = Gtk::CssProvider::create ();
	cssProvider2 -> load_from_data (getStyles ());
	Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider2, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// Drag & drop targets
	std::vector <Gtk::TargetEntry> targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list")
	};

	getToolBar ()    .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
	getSurfaceBox () .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	// Selection
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &BrowserWindow::set_selection);
	set_selection (getBrowser () -> getSelection () -> getChildren ());

	// Clipboard
	// Gtk::Clipboard::get () -> set_can_store (); // not needed
	Gtk::Clipboard::get () -> signal_owner_change ().connect (sigc::mem_fun (*this, &BrowserWindow::on_clipboard_owner_change));
	updatePasteStatus ();

	// Dashboard
	getBrowser () -> getBrowserOptions () -> dashboard () .addInterest (this, &BrowserWindow::set_dashboard);
	getBrowser () -> getViewer ()        .addInterest (this, &BrowserWindow::set_viewer);
	getBrowser () -> getExamineViewer () .addInterest (this, &BrowserWindow::set_examine_viewer);
	getBrowser () -> getWalkViewer ()    .addInterest (this, &BrowserWindow::set_walk_viewer);
	getBrowser () -> getFlyViewer ()     .addInterest (this, &BrowserWindow::set_fly_viewer);
	getBrowser () -> getPlaneViewer ()   .addInterest (this, &BrowserWindow::set_plane_viewer);
	getBrowser () -> getNoneViewer ()    .addInterest (this, &BrowserWindow::set_none_viewer);
	getBrowser () -> getLookAt ()        .addInterest (this, &BrowserWindow::set_look_at);
	getViewerButton () .set_menu (getViewerTypeMenu ());

	// Window
	getWindow () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	getWidget () .grab_focus ();
}

std::string
BrowserWindow::getStyles () const
{
	std::string string;

	auto styleContext = getWidget () .get_style_context ();

	auto fg_selected = styleContext -> get_color (Gtk::STATE_FLAG_SELECTED); 
	auto bg_normal   = styleContext -> get_background_color (Gtk::STATE_FLAG_NORMAL); 
	auto bg_selected = styleContext -> get_background_color (Gtk::STATE_FLAG_SELECTED); 

	string += "#OutlineTreeViewEditor .textview-editable {";
	string += "  background-color: mix (" + bg_selected .to_string () + ", " + bg_normal .to_string () + ", 0.9);";
	string += "}";
	string += "";
	string += "#OutlineTreeViewEditor .textview-editable:selected {";
	string += "  color: " + fg_selected .to_string () + ";";
	string += "  background-color: " + bg_selected .to_string () + ";";
	string += "}";

	return string;
}

// Menu

void
BrowserWindow::hasShortcuts (bool value)
{
	shortcuts = value;

	for (const auto & child : getMenuBar () .get_children ())
	{
		auto menuItem = dynamic_cast <Gtk::MenuItem*> (child);

		if (menuItem and menuItem -> get_visible ())
		{
			auto menu = menuItem -> get_submenu ();

			if (menu)
				menu -> set_sensitive (shortcuts);
		}
	}
}

// Selection

void
BrowserWindow::set_selection (const X3D::MFNode & children)
{
	bool haveSelection  = children .size ();
	bool haveSelections = children .size () > 1;

	getCutMenuItem ()    .set_sensitive (haveSelection);
	getCopyMenuItem ()   .set_sensitive (haveSelection);
	getDeleteMenuItem () .set_sensitive (haveSelection);

	getGroupSelectedNodesMenuItem () .set_sensitive (haveSelection);
	getUngroupMenuItem ()            .set_sensitive (haveSelection);
	getAddToGroupMenuItem ()         .set_sensitive (haveSelections);
	getDetachFromGroupMenuItem ()    .set_sensitive (haveSelection);
	getCreateParentGroupMenuItem ()  .set_sensitive (haveSelection);

	getDeselectAllMenuItem () .set_sensitive (haveSelection);

	getNodePropertiesEditorButton () .set_sensitive (haveSelection);
	getMaterialEditorButton ()       .set_sensitive (haveSelection);
	getTextureEditorButton ()        .set_sensitive (haveSelection);
}

// Keys

bool
BrowserWindow::on_key_press_event (GdkEventKey* event)
{
	if (shortcuts)
	{
		keys .press (event);

		if (not getBrowser () -> getSelection () -> getChildren () .empty ())
		{
			static constexpr float NUDGE_STEP   = 0.001;
			static constexpr float NUDGE_FACTOR = 10;

			float nudge           = NUDGE_STEP * (keys .shift () ? NUDGE_FACTOR : 1);
			bool  alongFrontPlane = false;

			switch (event -> keyval)
			{
				case GDK_KEY_Up:
				case GDK_KEY_KP_Up:
				{
					if (keys .control ())
						translateSelection (X3D::Vector3f (0, 0, -nudge), alongFrontPlane);
					else
						translateSelection (X3D::Vector3f (0, nudge, 0), alongFrontPlane);
					return true;
				}
				case GDK_KEY_Down:
				case GDK_KEY_KP_Down:
				{
					if (keys .control ())
						translateSelection (X3D::Vector3f (0, 0, nudge), alongFrontPlane);
					else
						translateSelection (X3D::Vector3f (0, -nudge, 0), alongFrontPlane);
					return true;
				}
				case GDK_KEY_Left:
				case GDK_KEY_KP_Left:
				{
					translateSelection (X3D::Vector3f (-nudge, 0, 0), alongFrontPlane);
					return true;
				}
				case GDK_KEY_Right:
				case GDK_KEY_KP_Right:
				{
					translateSelection (X3D::Vector3f (nudge, 0, 0), alongFrontPlane);
					return true;
				}
				default:
					break;
			}
		}
	}

	return false;
}

bool
BrowserWindow::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	return false;
}

// File menu

void
BrowserWindow::on_new ()
{
	if (isSaved ())
		blank ();
}

void
BrowserWindow::on_open ()
{
	if (isSaved ())
	{
		updateWidget ("FileOpenDialog");
		auto fileOpenDialog = getWidget <Gtk::FileChooserDialog> ("FileOpenDialog");

		fileOpenDialog -> add_filter (getFileFilterX3D ());
		fileOpenDialog -> add_filter (getFileFilterImage ());
		fileOpenDialog -> add_filter (getFileFilterAudio ());
		fileOpenDialog -> add_filter (getFileFilterVideo ());
		fileOpenDialog -> set_filter (getFileFilterX3D ());
	
		if (getFileOpenDialog () .get_uri () .empty ())
			fileOpenDialog -> set_current_folder (os::home ());
		else
			fileOpenDialog -> set_uri (getFileOpenDialog () .get_uri ());

		auto response_id = fileOpenDialog -> run ();

		if (response_id == Gtk::RESPONSE_OK)
		{
			open (Glib::uri_unescape_string (fileOpenDialog -> get_uri ()));
		}

		delete fileOpenDialog;
	}
}

void
BrowserWindow::on_open_location ()
{
	if (isSaved ())
	{
		Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

		if (clipboard -> wait_is_text_available ())
		{
			static const pcrecpp::RE scheme ("\\A(file|http|https|ftp|smb)$");

			basic::uri uri (clipboard -> wait_for_text ());

			if (scheme .FullMatch (uri .scheme ()))
				getOpenLocationEntry () .set_text (uri .str ());
		}

		getOpenLocationDialog () .set_response_sensitive (Gtk::RESPONSE_OK, getOpenLocationEntry () .get_text () .size ());

		auto response_id = getOpenLocationDialog () .run ();

		getOpenLocationDialog () .hide ();

		if (response_id == Gtk::RESPONSE_OK)
		{
			open (Glib::uri_unescape_string (getOpenLocationEntry () .get_text ()));
		}
	}
}

void
BrowserWindow::on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	dragDataHandling (context, selection_data, time, true);
}

void
BrowserWindow::on_import ()
{
	updateWidget ("FileImportDialog");
	auto fileImportDialog = getWidget <Gtk::FileChooserDialog> ("FileImportDialog");

	fileImportDialog -> add_filter (getFileFilterX3D ());
	fileImportDialog -> add_filter (getFileFilterImage ());
	fileImportDialog -> add_filter (getFileFilterAudio ());
	fileImportDialog -> add_filter (getFileFilterVideo ());
	fileImportDialog -> set_filter (getFileFilterX3D ());

	if (getFileImportDialog () .get_uri () .empty ())
	{
		if (getFileOpenDialog () .get_uri () .empty ())
			fileImportDialog -> set_current_folder (os::home ());
		else
			fileImportDialog -> set_uri (getFileOpenDialog () .get_uri ());
	}
	else
		fileImportDialog -> set_uri (getFileImportDialog () .get_uri ());

	auto response_id = fileImportDialog -> run ();

	if (response_id == Gtk::RESPONSE_OK)
	{
		auto uri = Glib::uri_unescape_string (fileImportDialog -> get_uri ());

		import ({ uri }, getImportAsInlineMenuItem () .get_active ());
	}

	if (not fileImportDialog -> get_uri () .empty ())
		getFileImportDialog () .set_uri (fileImportDialog -> get_uri ());

	delete fileImportDialog;
}

void
BrowserWindow::on_import_as_inline_toggled ()
{
	getConfig () .setItem ("importAsInline", getImportAsInlineMenuItem () .get_active ());
}

void
BrowserWindow::on_surface_box_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                                  int x, int y,
                                                  const Gtk::SelectionData & selection_data,
                                                  guint info,
                                                  guint time)
{
	dragDataHandling (context, selection_data, time, getBrowserMenuItem () .get_active ());
}

void
BrowserWindow::dragDataHandling (const Glib::RefPtr <Gdk::DragContext> & context,
                                 const Gtk::SelectionData & selection_data,
                                 guint time,
                                 bool do_open)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		if (selection_data .get_data_type () == "text/uri-list")
		{
			std::vector <basic::uri> uris;
			auto strings = selection_data .get_uris ();

			for (auto & string : strings)
				uris .emplace_back (Glib::uri_unescape_string (string)); // ???

			if (uris .size ())
			{
				if (do_open)
				{
					if (isSaved ())
						open (uris [0]);
				}
				else
					import (uris, getConfig () .getBoolean ("importAsInline"));

				context -> drag_finish (true, false, time);
				return;
			}
		}

		if (selection_data .get_data_type () == "STRING")
		{
			std::vector <basic::uri> uris;
			auto strings = basic::split (basic::trim (selection_data .get_data_as_string ()), "\r\n");

			for (auto & string : strings)
				uris .emplace_back (Glib::uri_unescape_string (string));

			if (uris .size ())
			{
				if (do_open)
				{
					if (isSaved ())
						open (uris [0]);
				}
				else
					import (uris, getConfig () .getBoolean ("importAsInline"));

				context -> drag_finish (true, false, time);
				return;
			}
		}
	}

	context -> drag_finish (false, false, time);
}

void
BrowserWindow::on_save ()
{
	basic::uri worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	if (worldURL .empty () or worldURL .is_network ())
		on_save_as ();

	else
		save (worldURL, getBrowser () -> getExecutionContext () -> isCompressed ());
}

void
BrowserWindow::on_save_as ()
{
	updateWidget ("FileSaveDialog");

	auto fileSaveDialog = getWidget <Gtk::FileChooserDialog> ("FileSaveDialog");

	fileSaveDialog -> add_filter (getFileFilterX3D ());
	fileSaveDialog -> set_filter (getFileFilterX3D ());

	if (getFileSaveDialog () .get_uri () .empty ())
	{
		if (getFileOpenDialog () .get_uri () .empty ())
		{
			fileSaveDialog -> set_current_folder (os::home ());
			fileSaveDialog -> set_current_name (getBrowser () -> getExecutionContext () -> getWorldURL () .basename (false) + ".x3dv");
		}
		else
			fileSaveDialog -> set_uri (getFileOpenDialog () .get_uri ());
	}
	else
		fileSaveDialog -> set_uri (getFileSaveDialog () .get_uri ());

	auto saveCompressedButton = getWidget <Gtk::CheckButton> ("SaveCompressedButton");

	saveCompressedButton -> set_active (getBrowser () -> getExecutionContext () -> isCompressed ());

	auto response_id = fileSaveDialog -> run ();

	if (response_id == Gtk::RESPONSE_OK)
	{
		save (Glib::uri_unescape_string (fileSaveDialog -> get_uri ()), saveCompressedButton -> get_active ());
	}

	if (not fileSaveDialog -> get_uri () .empty ())
		getFileSaveDialog () .set_uri (fileSaveDialog -> get_uri ());

	delete fileSaveDialog;
}

void
BrowserWindow::on_close ()
{
	close ();
}

void
BrowserWindow::on_revert_to_saved ()
{
	if (isSaved ())
		reload ();
}

// Dialog response handling

void
BrowserWindow::on_open_location_entry_changed ()
{
	getOpenLocationDialog () .set_response_sensitive (Gtk::RESPONSE_OK, getOpenLocationEntry () .get_text () .size ());
}

bool
BrowserWindow::on_open_location_entry_key_release_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
	{
		getOpenLocationDialog () .response (Gtk::RESPONSE_OK);
		return true;
	}

	return false;
}

// Undo/Redo

void
BrowserWindow::on_undo ()
{
	undo ();
}

void
BrowserWindow::on_redo ()
{
	redo ();
}

// Clipboard

void
BrowserWindow::on_clipboard_owner_change (GdkEventOwnerChange*)
{
	updatePasteStatus ();
}

void
BrowserWindow::on_cut_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Cut"));

	getSelection () -> clear (undoStep);

	cutNodes (selection, undoStep);

	getSelection () -> undoRestoreSelection (undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_copy_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	copyNodes (selection);
}

void
BrowserWindow::on_paste_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	auto undoStep = std::make_shared <UndoStep> (_ ("Paste"));

	pasteNodes (selection, undoStep);

	addUndoStep (undoStep);
}

// Edit menu

void
BrowserWindow::on_delete_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Delete"));

	getSelection () -> clear (undoStep);

	removeNodes (selection, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_group_selected_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Group"));

	getSelection () -> clear (undoStep);
	getSelection () -> setChildren ({ groupNodes (selection, undoStep) }, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_ungroup_node_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Ungroup"));

	getSelection () -> clear (undoStep);
	getSelection () -> setChildren (ungroupNodes (selection, undoStep), undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_add_to_group_activate ()
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .size () < 2)
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Add To Group"));

	getSelection () -> undoRestoreSelection (undoStep);

	auto group = selection .back ();
	selection .pop_back ();

	if (addToGroup (group, selection, undoStep))
	{
		getSelection () -> setChildren (selection, undoStep);

		addUndoStep (undoStep);
	}
}

void
BrowserWindow::on_detach_from_group_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Detach From Group"));

	getSelection () -> redoRestoreSelection (undoStep);

	detachFromGroup (selection, getKeys () .shift (), undoStep);

	getSelection () -> undoRestoreSelection (undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_parent_group_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	auto undoStep = std::make_shared <UndoStep> (_ ("Create Parent Group"));

	getSelection () -> clear (undoStep);

	X3D::MFNode groups = createParentGroup (selection, undoStep);

	getSelection () -> setChildren (groups, undoStep);

	addUndoStep (undoStep);

	// Expand groups

	for (const auto & group : groups)
	{
		for (const auto & iter : getOutlineTreeView () .get_iters (group))
			getOutlineTreeView () .expand_row (getOutlineTreeView () .get_model () -> get_path (iter), false);
	}
}

// View menu

void
BrowserWindow::on_toolBar_toggled ()
{
	getConfig () .setItem ("toolBar", getToolBarMenuItem () .get_active ());
	toggleWidget (getToolBar (), getToolBarMenuItem () .get_active ());
}

void
BrowserWindow::on_sideBar_toggled ()
{
	getConfig () .setItem ("sideBar", getSideBarMenuItem () .get_active ());
	toggleWidget (getSideBar (), getSideBarMenuItem () .get_active ());
}

void
BrowserWindow::on_footer_toggled ()
{
	getConfig () .setItem ("footer", getFooterMenuItem  () .get_active ());
	toggleWidget (getFooter (), getFooterMenuItem () .get_active ());
}

void
BrowserWindow::on_browser_toggled ()
{
	if (getBrowserMenuItem () .get_active ())
	{
		getConfig () .setItem ("workspace", "BROWSER");

		enableEditor (false);
	}
}

void
BrowserWindow::on_editor_toggled ()
{
	if (getEditorMenuItem () .get_active ())
	{
		getConfig () .setItem ("workspace", "EDITOR");

		enableEditor (true);
	}
}

void
BrowserWindow::enableEditor (bool enabled)
{
	getImportMenuItem ()           .set_visible (enabled);
	getImportAsInlineMenuItem ()   .set_visible (enabled);
	getEditMenuItem ()             .set_visible (enabled);
	getBrowserOptionsSeparator ()  .set_visible (enabled);
	getShadingMenuItem ()          .set_visible (enabled);
	getPrimitiveQualityMenuItem () .set_visible (enabled);
	getSelectionMenuItem ()        .set_visible (enabled);

	getImportButton ()               .set_visible (enabled);
	getSeparatorToolItem1 ()         .set_visible (enabled);
	getUndoButton ()                 .set_visible (enabled);
	getRedoButton ()                 .set_visible (enabled);
	getSeparatorToolItem2 ()         .set_visible (enabled);
	getNodePropertiesEditorButton () .set_visible (enabled);
	getMaterialEditorButton ()       .set_visible (enabled);
	getTextureEditorButton ()        .set_visible (false);
	getArrowButton ()                .set_visible (enabled);

	getLibraryViewBox ()   .set_visible (enabled);
	getOutlineEditorBox () .set_visible (enabled);

	if (enabled)
		getArrowButton () .set_active (true);
	else
		getHandButton () .set_active (true);
}

// Shading menu

void
BrowserWindow::phong_activate ()
{
	if (getPhongMenuItem () .get_active ())
	{
		getConfig () .setItem ("shading", "PHONG");
		getBrowser () -> getBrowserOptions () -> shading () = "PHONG";
	}
}

void
BrowserWindow::gouraud_activate ()
{
	if (getGouraudMenuItem () .get_active ())
	{
		getConfig () .setItem ("shading", "GOURAUD");
		getBrowser () -> getBrowserOptions () -> shading () = "GOURAUD";
	}
}

void
BrowserWindow::flat_activate ()
{
	if (getFlatMenuItem () .get_active ())
	{
		getConfig () .setItem ("shading", "FLAT");
		getBrowser () -> getBrowserOptions () -> shading () = "FLAT";
	}
}

void
BrowserWindow::wireframe_activate ()
{
	if (getWireFrameMenuItem () .get_active ())
	{
		getConfig () .setItem ("shading", "WIREFRAME");
		getBrowser () -> getBrowserOptions () -> shading () = "WIREFRAME";
	}
}

void
BrowserWindow::pointset_activate ()
{
	if (getPointSetMenuItem () .get_active ())
	{
		getConfig () .setItem ("shading", "POINTSET");
		getBrowser () -> getBrowserOptions () -> shading () = "POINTSET";
	}
}

// Primitive Quality

void
BrowserWindow::on_low_quality_activate ()
{
	if (getLowQualityMenuItem () .get_active ())
	{
		getConfig () .setItem ("primitiveQuality", "LOW");
		getBrowser () -> getBrowserOptions () -> primitiveQuality () = "LOW";
	}
}

void
BrowserWindow::on_medium_quality_activate ()
{
	if (getMediumQualityMenuItem () .get_active ())
	{
		getConfig () .setItem ("primitiveQuality", "MEDIUM");
		getBrowser () -> getBrowserOptions () -> primitiveQuality () = "MEDIUM";
	}
}

void
BrowserWindow::on_high_quality_activate ()
{
	if (getHighQualityMenuItem () .get_active ())
	{
		getConfig () .setItem ("primitiveQuality", "HIGH");
		getBrowser () -> getBrowserOptions () -> primitiveQuality () = "HIGH";
	}
}

// RenderingProperties

void
BrowserWindow::on_rendering_properties_toggled ()
{
	getConfig () .setItem ("renderingProperties", getRenderingPropertiesMenuItem () .get_active ());
	getBrowser () -> getRenderingProperties () -> enabled () = getRenderingPropertiesMenuItem () .get_active ();
}

// Fullscreen

void
BrowserWindow::on_fullscreen ()
{
	getFullScreenMenuItem ()   .set_visible (false);
	getUnFullScreenMenuItem () .set_visible (true);
	getWindow () .fullscreen ();
}

void
BrowserWindow::on_unfullscreen ()
{
	getFullScreenMenuItem ()   .set_visible (true);
	getUnFullScreenMenuItem () .set_visible (false);
	getWindow () .unfullscreen ();
}

// Selection menu

void
BrowserWindow::on_select_all_activate ()
{
	auto undoStep = std::make_shared <UndoStep> ();

	getSelection () -> setChildren (getBrowser () -> getExecutionContext () -> getRootNodes (), undoStep);
}

void
BrowserWindow::on_deselect_all_activate ()
{
	auto undoStep = std::make_shared <UndoStep> ();

	getSelection () -> clear (undoStep);
}

void
BrowserWindow::on_select_lowest_toggled ()
{
	getConfig () .setItem ("selectLowest", getSelectLowestMenuItem () .get_active ());
}

void
BrowserWindow::on_follow_primary_selection_toggled ()
{
	getConfig () .setItem ("followPrimarySelection", getFollowPrimarySelectionMenuItem () .get_active ());
}

// Navigation menu

void
BrowserWindow::on_rubberband_toggled ()
{
	getConfig () .setItem ("rubberBand", getRubberbandMenuItem () .get_active ());
	getBrowser () -> getBrowserOptions () -> rubberBand () = getRubberbandMenuItem () .get_active ();
}

// Editor handling

void
BrowserWindow::on_motion_blur_editor_activate ()
{
	getMotionBlurEditor () .getWindow () .present ();
}

// Help menu

void
BrowserWindow::on_info ()
{
	if (isSaved ())
		open (get_page ("about/info.wrl"));
}

void
BrowserWindow::on_standard_size ()
{
	getWindow () .resize (960, 600);
}

/// Toolbar

void
BrowserWindow::on_node_properties_editor ()
{
	if (isDialogOpen ("NodePropertiesEditor"))
		return;

	if (not getBrowser () -> getSelection () -> getChildren () .empty ())
		addDialog ("NodePropertiesEditor", std::make_shared <NodePropertiesEditor> (getBrowserWindow (), getBrowser () -> getSelection () -> getChildren () .back ()));
}

void
BrowserWindow::on_material_editor ()
{
	if (isDialogOpen ("MaterialEditor"))
		return;

	if (not getBrowser () -> getSelection () -> getChildren () .empty ())
		addDialog ("MaterialEditor", std::make_shared <MaterialEditor> (getBrowserWindow (), getBrowser () -> getSelection () -> getChildren ()));
}

void
BrowserWindow::on_texture_editor ()
{
	if (isDialogOpen ("TextureEditor"))
		return;

	if (not getBrowser () -> getSelection () -> getChildren () .empty ())
		addDialog ("TextureEditor", std::make_shared <TextureEditor> (getBrowserWindow ()));
}

// Browser dashboard handling

void
BrowserWindow::set_dashboard (bool value)
{
	getDashboard () .set_visible (value);
}

void
BrowserWindow::on_hand_button_toggled ()
{
	if (getHandButton () .get_active ())
	{
		std::clog << "Hand button clicked." << std::endl;

		getSelection () -> setEnabled (false);
	}
}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		std::clog << "Arrow button clicked." << std::endl;

		getSelection () -> setEnabled (true);
	}
}

// Viewer

void
BrowserWindow::set_viewer (X3D::ViewerType type)
{
	switch (type)
	{
		case X3D::ViewerType::PLANE:
		case X3D::ViewerType::NONE:
		{
			getStraightenButton () .set_visible (false);
			break;
		}
		default:
		{
			getStraightenButton () .set_visible (true);
			break;
		}
	}

	switch (type)
	{
		case X3D::ViewerType::LOOK_AT:
		{
			getHandButton ()  .set_sensitive (false);
			getArrowButton () .set_sensitive (false);

			if (getArrowButton () .get_active ())
				getSelection () -> disconnect ();

			break;
		}
		default:
		{
			viewer = type;
		
			getHandButton ()  .set_sensitive (true);
			getArrowButton () .set_sensitive (true);

			if (getArrowButton () .get_active ())
				getSelection () -> connect ();
				
			if (getLookAtButton () .get_active ())
				getLookAtButton () .set_active (false);

			break;
		}
	}

	switch (type)
	{
		case X3D::ViewerType::NONE:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("gtk-cancel"));
			break;
		}
		case X3D::ViewerType::EXAMINE:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("ExamineViewer"));
			break;
		}
		case X3D::ViewerType::WALK:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("WalkViewer"));
			break;
		}
		case X3D::ViewerType::FLY:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("FlyViewer"));
			break;
		}
		case X3D::ViewerType::PLANE:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("PlaneViewer"));
			break;
		}
		case X3D::ViewerType::LOOK_AT:
		{
			if (not getLookAtButton () .get_active ())
				getLookAtButton () .set_active (true);

			break;
		}
	}
}

void
BrowserWindow::set_examine_viewer (bool value)
{
	getExamineViewerMenuItem () .set_visible (value);
}

void
BrowserWindow::set_walk_viewer (bool value)
{
	getWalkViewerMenuItem () .set_visible (value);
}

void
BrowserWindow::set_fly_viewer (bool value)
{
	getFlyViewerMenuItem () .set_visible (value);
}

void
BrowserWindow::set_plane_viewer (bool value)
{
	getPlaneViewerMenuItem () .set_visible (value);
}

void
BrowserWindow::set_none_viewer (bool value)
{
	getNoneViewerMenuItem () .set_visible (value);
}

void
BrowserWindow::on_viewer_clicked ()
{
	if (getLookAtButton () .get_active ())
	{
		getBrowser () -> setViewer (viewer);
	}
}

void
BrowserWindow::on_examine_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::EXAMINE);
}

void
BrowserWindow::on_walk_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::WALK);
}

void
BrowserWindow::on_fly_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::FLY);
}

void
BrowserWindow::on_plane_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::PLANE);
}

void
BrowserWindow::on_none_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::NONE);
}

void
BrowserWindow::on_straighten_clicked ()
{
	if (getBrowser () -> getActiveLayer ())
		getBrowser () -> getActiveLayer () -> getViewpoint () -> straighten ();
}

// Look at

void
BrowserWindow::set_look_at (bool value)
{
	getLookAtAllButton () .set_visible (value);
	getLookAtButton ()    .set_visible (value);
}

void
BrowserWindow::on_look_at_all_clicked ()
{
	if (getBrowser () -> getActiveLayer ())
		getBrowser () -> getActiveLayer () -> lookAt ();
}

void
BrowserWindow::on_look_at_toggled ()
{
	if (getLookAtButton () .get_active ())
	{
		if (getBrowser () -> getViewer () not_eq X3D::ViewerType::LOOK_AT)
			getBrowser () -> setViewer (X3D::ViewerType::LOOK_AT);
	}
	else
	{
		if (getBrowser () -> getViewer () not_eq viewer)
			getBrowser () -> setViewer (viewer);
	}
}

// Dialog response handling

void
BrowserWindow::on_messageDialog_response (int)
{
	getMessageDialog () .hide ();
}

} // puck
} // titania
