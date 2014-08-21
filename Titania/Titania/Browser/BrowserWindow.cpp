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

#include "../Dialogs/FileImportDialog/FileImportDialog.h"
#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../Dialogs/OpenLocationDialog/OpenLocationDialog.h"
#include "../Dialogs/NodeIndex/NodeIndex.h"
#include "../Editors/PrototypeInstanceDialog/PrototypeInstanceDialog.h"

#include "../Widgets/Console/Console.h"
#include "../Widgets/HistoryView/HistoryView.h"
#include "../Widgets/LibraryView/LibraryView.h"
#include "../Widgets/OutlineEditor/OutlineEditor.h"
#include "../Widgets/OutlineEditor/OutlineTreeModel.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"
#include "../Widgets/ViewpointList/ViewpointList.h"

#include "../Browser/BrowserSelection.h"
#include "../Configuration/config.h"

#include <Titania/X3D/Tools/EnvironmentalSensor/ProximitySensorTool.h>
#include <Titania/X3D/Tools/EnvironmentalSensor/VisibilitySensorTool.h>

#include <Titania/OS.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

BrowserWindow::BrowserWindow (const X3D::BrowserPtr & browserSurface, int argc, char** argv) :
	X3DBaseInterface (this, browserSurface),
	X3DBrowserEditor (argc, argv),
	   viewpointList (new ViewpointList (this, true)),
	   historyEditor (new HistoryView (this)),
	     libraryView (new LibraryView (this)),
	   outlineEditor (new OutlineEditor (this)),
	         console (new Console (this)),
	            keys (),
	       shortcuts (true),
	          toggle (true),
	        changing (false),
	          viewer (X3D::ViewerType::NONE),
	    saveAsOutput ()
{
	if (getConfig () .getBoolean ("transparent"))
		setTransparent (true);
	else
		getBrowser () -> set_antialiasing (4);

	setup ();
}

void
BrowserWindow::initialize ()
{
	X3DBrowserEditor::initialize ();

	// Sidebar
	viewpointList -> reparent (getViewpointListBox (), getWindow ());
	historyEditor -> reparent (getHistoryViewBox (),   getWindow ());
	libraryView   -> reparent (getLibraryViewBox (),   getWindow ());
	outlineEditor -> reparent (getOutlineEditorBox (), getWindow ());
	console       -> reparent (getConsoleBox (),       getWindow ());

	loadStyles ();

	// Drag & drop targets
	std::vector <Gtk::TargetEntry> targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list")
	};

	getToolBar ()    .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
	getSurfaceBox () .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	// Clipboard
	// Gtk::Clipboard::get () -> set_can_store (); // not needed
	Gtk::Clipboard::get () -> signal_owner_change () .connect (sigc::mem_fun (*this, &BrowserWindow::on_clipboard_owner_change));
	updatePasteStatus ();

	// Browser Events
	getScene ()            .addInterest (this, &BrowserWindow::set_scene);
	getExecutionContext () .addInterest (this, &BrowserWindow::set_executionContext);

	getBrowser () -> getViewer ()               .addInterest (this, &BrowserWindow::set_viewer);
	getBrowser () -> getAvailableViewers ()     .addInterest (this, &BrowserWindow::set_available_viewers);

	getBrowser () -> getBrowserOptions () -> dashboard ()        .addInterest (this, &BrowserWindow::set_dashboard);
	getBrowser () -> getBrowserOptions () -> shading ()          .addInterest (this, &BrowserWindow::set_shading);
	getBrowser () -> getBrowserOptions () -> primitiveQuality () .addInterest (this, &BrowserWindow::set_primitiveQuality);
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &BrowserWindow::set_selection);

	set_selection (getBrowser () -> getSelection () -> getChildren ());
	getViewerButton () .set_menu (getViewerTypeMenu ());

	// Window
	getWindow () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	getWidget () .grab_focus ();
}

const std::shared_ptr <OutlineTreeViewEditor> &
BrowserWindow::getOutlineTreeView () const
{
	return outlineEditor -> getTreeView ();
}

void
BrowserWindow::loadStyles () const
{
	Glib::RefPtr <Gtk::CssProvider> cssProvider1 = Gtk::CssProvider::create ();
	cssProvider1 -> load_from_path (get_ui ("style.css"));
	Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider1, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	std::string string;

	const auto styleContext = getWidget () .get_style_context ();

	const auto fg_selected = styleContext -> get_color (Gtk::STATE_FLAG_SELECTED);
	const auto bg_normal   = styleContext -> get_background_color (Gtk::STATE_FLAG_NORMAL);
	const auto bg_selected = styleContext -> get_background_color (Gtk::STATE_FLAG_SELECTED);

	string += "#OutlineTreeViewEditor .textview-editable GtkTextView {";
	string += "  background-color: mix (" + bg_selected .to_string () + ", " + bg_normal .to_string () + ", 0.9);";
	string += "}";
	string += "";
	string += "#OutlineTreeViewEditor .textview-editable GtkTextView:selected {";
	string += "  color: " + fg_selected .to_string () + ";";
	string += "  background-color: " + bg_selected .to_string () + ";";
	string += "}";

	Glib::RefPtr <Gtk::CssProvider> cssProvider2 = Gtk::CssProvider::create ();
	cssProvider2 -> load_from_data (string);
	Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider2, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Menu

void
BrowserWindow::hasShortcuts (bool value)
{
	shortcuts = value;

	for (const auto & child : getMenuBar () .get_children ())
	{
		const auto menuItem = dynamic_cast <Gtk::MenuItem*> (child);

		if (menuItem and menuItem -> get_visible ())
		{
			const auto menu = menuItem -> get_submenu ();

			if (menu)
				menu -> set_sensitive (shortcuts);
		}
	}
}

void
BrowserWindow::expandNodes (const X3D::MFNode & nodes)
{
	if (getConfig () .getBoolean ("followPrimarySelection"))
		getBrowser () -> finished () .addInterest (this, &BrowserWindow::expandNodesImpl, nodes);
}

void
BrowserWindow::expandNodesImpl (const X3D::MFNode & nodes)
{
	getBrowser () -> finished () .removeInterest (this, &BrowserWindow::expandNodesImpl);

	for (const auto & node : nodes)
	{
		getOutlineTreeView () -> expand_to (node);

		for (const auto & iter : getOutlineTreeView () -> get_iters (node))
		{
			const auto path = getOutlineTreeView () -> get_model () -> get_path (iter);
			
			getOutlineTreeView () -> expand_row (path, false);
			getOutlineTreeView () -> scroll_to_row (path);
		}
	}
}

void
BrowserWindow::set_scene ()
{
	toggle = false;
	getBackgroundsMenuItem () .set_active (true);

	toggle = false;
	getFogsMenuItem () .set_active (true);

	toggle = false;
	getLightsMenuItem () .set_active (false);

	toggle = false;
	getProximitySensorsMenuItem () .set_active (false);

	toggle = false;
	getVisibilitySensorsMenuItem () .set_active (false);

	toggle = false;
	getViewpointsMenuItem () .set_active (false);

	toggle = true;
}

void
BrowserWindow::set_executionContext ()
{
	const bool inScene = not inPrototypeInstance ();

	getImportMenuItem ()                 .set_sensitive (inScene);
	getSaveMenuItem ()                   .set_sensitive (inScene);
	getSaveAsMenuItem ()                 .set_sensitive (inScene);
	getRemoveUnusedPrototypesMenuItem () .set_sensitive (inScene);

	getEditMenuItem () .set_sensitive (inScene);

	getSaveButton ()                    .set_sensitive (inScene);
	getImportButton ()                  .set_sensitive (inScene);
	getCreatePrototypeInstanceButton () .set_sensitive (inScene);

	set_selection (getBrowser () -> getSelection () -> getChildren ());
}

// Selection

void
BrowserWindow::set_selection (const X3D::MFNode & selection)
{
	const bool inScene        = not inPrototypeInstance ();
	const bool haveSelection  = inScene and selection .size ();
	const bool haveSelections = inScene and selection .size () > 1;

	getCutMenuItem ()    .set_sensitive (haveSelection);
	getCopyMenuItem ()   .set_sensitive (haveSelection);
	getDeleteMenuItem () .set_sensitive (haveSelection);

	getCloneMenuItem ()       .set_sensitive (haveSelection);
	getCreateCloneMenuItem () .set_sensitive (haveSelections);
	getUnlinkCloneMenuItem () .set_sensitive (haveSelection);

	getGroupSelectedNodesMenuItem () .set_sensitive (haveSelection);
	getUngroupMenuItem ()            .set_sensitive (haveSelection);
	getAddToGroupMenuItem ()         .set_sensitive (haveSelections);
	getDetachFromGroupMenuItem ()    .set_sensitive (haveSelection);
	getCreateParentMenuItem ()       .set_sensitive (haveSelection);

	getDeselectAllMenuItem ()           .set_sensitive (selection .size ());
	getHideSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);
	getHideUnselectedObjectsMenuItem () .set_sensitive (haveSelection);
	getShowSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);

	getSelectParentButton ()    .set_sensitive (haveSelection);
	getSelectChildrenButton ()  .set_sensitive (haveSelection);
	getLookAtSelectionButton () .set_sensitive (haveSelection);

	// Show/Hide Object Icons.

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::X3DLightNode*> (node))
		{
			toggle = false;
			getLightsMenuItem () .set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::ProximitySensor*> (node))
		{
			toggle = false;
			getProximitySensorsMenuItem () .set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::VisibilitySensor*> (node))
		{
			toggle = false;
			getVisibilitySensorsMenuItem () .set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::X3DViewpointNode*> (node))
		{
			toggle = false;
			getViewpointsMenuItem () .set_active (true);
			break;
		}
	}

	toggle = true;
}

// Keys

bool
BrowserWindow::on_focus_out_event (GdkEventFocus*)
{
	getKeys () .clear ();

	return false;
}

bool
BrowserWindow::on_key_press_event (GdkEventKey* event)
{
	if (shortcuts)
	{
		keys .press (event);

		getBrowser () -> getSelection () -> setMode (keys .shift () and not keys .control () ? X3D::Selection::MULTIPLE : X3D::Selection::SINGLE);

		// Nudge selection.

		if (not inPrototypeInstance () and not getBrowser () -> getSelection () -> getChildren () .empty ())
		{
			static constexpr float NUDGE_STEP   = 0.001;
			static constexpr float NUDGE_FACTOR = 10;

			const float nudge           = NUDGE_STEP * (keys .shift () ? NUDGE_FACTOR : 1);
			const bool  alongFrontPlane = false;

			switch (event -> keyval)
			{
				case GDK_KEY_Up:
				{
					if (keys .control ())
						translateSelection (X3D::Vector3f (0, 0, -nudge), alongFrontPlane);
					else
						translateSelection (X3D::Vector3f (0, nudge, 0), alongFrontPlane);

					return true;
				}
				case GDK_KEY_Down:
				{
					if (keys .control ())
						translateSelection (X3D::Vector3f (0, 0, nudge), alongFrontPlane);
					else
						translateSelection (X3D::Vector3f (0, -nudge, 0), alongFrontPlane);

					return true;
				}
				case GDK_KEY_Left:
				{
					translateSelection (X3D::Vector3f (-nudge, 0, 0), alongFrontPlane);
					return true;
				}
				case GDK_KEY_Right:
				{
					translateSelection (X3D::Vector3f (nudge, 0, 0), alongFrontPlane);
					return true;
				}
				default:
					break;
			}
		}

		// Change viewpoint.

		switch (event -> keyval)
		{
			case GDK_KEY_Home:
			{
				getBrowser () -> firstViewpoint ();
				return true;
			}
			case GDK_KEY_Page_Up:
			{
				getBrowser () -> previousViewpoint ();
				return true;
			}
			case GDK_KEY_Page_Down:
			{
				getBrowser () -> nextViewpoint ();
				return true;
			}
			case GDK_KEY_End:
			{
				getBrowser () -> lastViewpoint ();
				return true;
			}
			default:
				break;
		}
	}

	return false;
}

bool
BrowserWindow::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	getBrowser () -> getSelection () -> setMode (keys .shift () and not keys .control () ? X3D::Selection::MULTIPLE : X3D::Selection::SINGLE);

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
		std::dynamic_pointer_cast <FileOpenDialog> (addDialog ("FileOpenDialog", false)) -> loadURL ();
}

void
BrowserWindow::on_open_location ()
{
	if (isSaved ())
		std::dynamic_pointer_cast <OpenLocationDialog> (addDialog ("OpenLocationDialog", false)) -> run ();
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
	std::dynamic_pointer_cast <FileImportDialog> (addDialog ("FileImportDialog", false)) -> run ();
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
                                 const guint time,
                                 const bool do_open)
{
	if (selection_data .get_format () == 8 and selection_data .get_length ()) // 8 bit format
	{
		std::vector <basic::uri> uris;

		if (selection_data .get_data_type () == "text/uri-list")
		{
			const auto strings = selection_data .get_uris ();

			for (const auto & string : strings)
				uris .emplace_back (Glib::uri_unescape_string (string));         // ???
		}

		if (selection_data .get_data_type () == "STRING")
		{
			const auto strings = basic::split (basic::trim (selection_data .get_data_as_string ()), "\r\n");

			for (const auto & string : strings)
				uris .emplace_back (Glib::uri_unescape_string (string));
		}

		if (not uris .empty ())
		{
			if (do_open)
			{
				if (isSaved ())
					open (uris [0]);
			}
			else
			{
				const auto undoStep = getConfig () .getBoolean ("importAsInline")
				                      ? std::make_shared <UndoStep> (_ ("Import As Inline"))
				                      : std::make_shared <UndoStep> (_ ("Import"));

				const auto nodes = importURL (uris, getConfig () .getBoolean ("importAsInline"), undoStep);

				getSelection () -> setChildren (nodes, undoStep);
				addUndoStep (undoStep);
			}

			context -> drag_finish (true, false, time);
			return;
		}
	}

	context -> drag_finish (false, false, time);
}

void
BrowserWindow::on_save ()
{
	const basic::uri worldURL = getRootContext () -> getWorldURL ();

	if (worldURL .empty () or worldURL .is_network ())
		on_save_as ();

	else
		save (worldURL, getRootContext () -> isCompressed ());
}

void
BrowserWindow::on_save_as ()
{
	std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false)) -> saveScene ();
}

void
BrowserWindow::on_remove_unused_prototypes ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Remove Unused Prototypes"));

	removeUnusedPrototypes (undoStep);

	addUndoStep (undoStep);
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

	const auto undoStep = std::make_shared <UndoStep> (_ ("Cut"));

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
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	const auto undoStep = std::make_shared <UndoStep> (_ ("Paste"));

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

	const auto undoStep = std::make_shared <UndoStep> (_ ("Delete Node From Scene"));

	getSelection () -> clear (undoStep);

	removeNodesFromScene (getExecutionContext (), selection, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_clone_activate ()
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .size () < 2)
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Create Clone"));

	const auto clone = selection .back ();
	selection .pop_back ();

	createClone (clone, selection, undoStep);

	getSelection () -> setChildren ({ clone }, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_unlink_clone_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Unlink Clone"));

	X3D::MFNode nodes = unlinkClone (selection, undoStep);

	getSelection () -> setChildren (nodes, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_group_selected_nodes_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Group"));
	const auto group    = groupNodes ("Transform", selection, undoStep);

	emplaceBack (getExecutionContext () -> getRootNodes (), group, undoStep);

	getSelection () -> setChildren ({ group }, undoStep);
	addUndoStep (undoStep);

	expandNodes (X3D::MFNode ({ group }));
}

void
BrowserWindow::on_ungroup_node_activate ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Ungroup"));

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

	const auto undoStep = std::make_shared <UndoStep> (_ ("Add To Group"));

	getSelection () -> undoRestoreSelection (undoStep);

	const auto group = selection .back ();
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

	const auto undoStep = std::make_shared <UndoStep> (_ ("Detach From Group"));

	getSelection () -> undoRestoreSelection (undoStep);
	getSelection () -> redoRestoreSelection (undoStep);

	detachFromGroup (selection, getKeys () .shift (), undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_parent_transform_activate ()
{
	on_create_parent ("Transform");
}

void
BrowserWindow::on_create_parent_group_activate ()
{
	on_create_parent ("Group");
}

void
BrowserWindow::on_create_parent_switch_activate ()
{
	on_create_parent ("Switch");
}

void
BrowserWindow::on_create_parent_billboard_activate ()
{
	on_create_parent ("Billboard");
}

void
BrowserWindow::on_create_parent_collision_activate ()
{
	on_create_parent ("Collision");
}

void
BrowserWindow::on_create_parent_lod_activate ()
{
	on_create_parent ("LOD");
}

void
BrowserWindow::on_create_parent_anchor_activate ()
{
	on_create_parent ("Anchor");
}

void
BrowserWindow::on_create_parent_screen_group_activate ()
{
	on_create_parent ("ScreenGroup");
}

void
BrowserWindow::on_create_parent_layout_group_activate ()
{
	on_create_parent ("LayoutGroup");
}

void
BrowserWindow::on_create_parent_geo_transform_activate ()
{
	on_create_parent ("GeoTransform");
}

void
BrowserWindow::on_create_parent_geo_location_activate ()
{
	on_create_parent ("GeoLocation");
}

void
BrowserWindow::on_create_parent_cad_part_activate ()
{
	on_create_parent ("CADPart");
}

void
BrowserWindow::on_create_parent_cad_assembly_activate ()
{
	on_create_parent ("CADAssembly");
}

void
BrowserWindow::on_create_parent_cad_layer_activate ()
{
	on_create_parent ("CADLayer");
}

void
BrowserWindow::on_create_parent (const std::string & typeName)
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (basic::sprintf (_ ("Create Parent %s"), typeName .c_str ()));

	getSelection () -> clear (undoStep);

	const auto leader = selection .back ();
	selection .pop_back ();

	const auto group = createParentGroup (typeName, { leader }, undoStep);

	if (not selection .empty ())
	{
		addToGroup (group, selection, undoStep);
		addToGroup (group, { leader }, undoStep);
	}

	getSelection () -> setChildren ({ group }, undoStep);

	addUndoStep (undoStep);

	expandNodes (X3D::MFNode ({ group }));
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

		isLive (true);

		enableEditor (false);

		if (not getBackgroundsMenuItem () .get_active ())
			getBackgroundsMenuItem () .set_active (true);

		if (not getFogsMenuItem () .get_active ())
			getFogsMenuItem () .set_active (true);

		if (getLightsMenuItem () .get_active ())
			getLightsMenuItem () .set_active (false);

		if (getProximitySensorsMenuItem () .get_active ())
			getProximitySensorsMenuItem () .set_active (false);

		if (getVisibilitySensorsMenuItem () .get_active ())
			getVisibilitySensorsMenuItem () .set_active (false);

		if (getViewpointsMenuItem () .get_active ())
			getViewpointsMenuItem () .set_active (false);

		on_show_all_objects_activate ();
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
BrowserWindow::enableEditor (const bool enabled)
{
	getImportMenuItem ()                       .set_visible (enabled);
	getImportAsInlineMenuItem ()               .set_visible (enabled);
	getRemoveUnusedPrototypesMenuItem ()       .set_visible (enabled);
	getEditMenuItem ()                         .set_visible (enabled);
	getBrowserOptionsSeparator ()              .set_visible (enabled);
	getShadingMenuItem ()                      .set_visible (enabled);
	getPrimitiveQualityMenuItem ()             .set_visible (enabled);
	getShowHideEnvironmentalEffectsMenuItem () .set_visible (enabled);
	getObjectIconsMenuItem ()                  .set_visible (enabled);
	getSelectionMenuItem ()                    .set_visible (enabled);
	getLayoutMenuItem ()                       .set_visible (enabled);

	getImportButton () .set_visible (enabled);
	getEditToolBar ()  .set_visible (enabled);

	set_dashboard (getBrowser () -> getBrowserOptions () -> dashboard ());
	set_available_viewers (getBrowser () -> getAvailableViewers ());

	getHandButton ()            .set_visible (enabled);
	getArrowButton ()           .set_visible (enabled);
	getPlayPauseButton ()       .set_visible (enabled);
	getSelectSeparator ()       .set_visible (enabled);
	getSelectParentButton ()    .set_visible (enabled);
	getSelectChildrenButton ()  .set_visible (enabled);
	getViewerSeparator ()       .set_visible (enabled);
	getLookAtSelectionButton () .set_visible (enabled);

	getLibraryViewBox ()   .set_visible (enabled);
	getOutlineEditorBox () .set_visible (enabled);

	if (enabled and getConfig () .getBoolean ("arrow"))
		getArrowButton () .set_active (true);
	else
		getHandButton () .set_active (true);
}

// Shading menu

void
BrowserWindow::on_phong_activate ()
{
	if (getPhongMenuItem () .get_active ())
		on_shading_activate ("PHONG");
}

void
BrowserWindow::on_gouraud_activate ()
{
	if (getGouraudMenuItem () .get_active ())
		on_shading_activate ("GOURAUD");
}

void
BrowserWindow::on_flat_activate ()
{
	if (getFlatMenuItem () .get_active ())
		on_shading_activate ("FLAT");
}

void
BrowserWindow::on_wireframe_activate ()
{
	if (getWireFrameMenuItem () .get_active ())
		on_shading_activate ("WIREFRAME");
}

void
BrowserWindow::on_pointset_activate ()
{
	if (getPointSetMenuItem () .get_active ())
		on_shading_activate ("POINTSET");
}

void
BrowserWindow::on_shading_activate (const std::string & value)
{
	if (changing)
		return;

	getBrowser () -> getBrowserOptions () -> shading () .removeInterest (this, &BrowserWindow::set_shading);
	getBrowser () -> getBrowserOptions () -> shading () .addInterest (this, &BrowserWindow::connectShading);

	getBrowser () -> getBrowserOptions () -> shading () = value;
}

void
BrowserWindow::set_shading (const X3D::SFString & value)
{
	changing = true;

	if (value == "PHONG")
		getPhongMenuItem () .set_active (true);

	else if (value == "FLAT")
		getFlatMenuItem () .set_active (true);

	else if (value == "WIREFRAME")
		getWireFrameMenuItem () .set_active (true);

	else if (value == "POINTSET")
		getPointSetMenuItem () .set_active (true);

	else
		getGouraudMenuItem () .set_active (true);

	changing = false;
}

void
BrowserWindow::connectShading (const X3D::SFString & field)
{
	field .removeInterest (this, &BrowserWindow::connectShading);
	field .addInterest (this, &BrowserWindow::set_shading);
}

// Primitive Quality

void
BrowserWindow::on_high_quality_activate ()
{
	if (getHighQualityMenuItem () .get_active ())
		on_primitiveQuality_activate ("HIGH");
}

void
BrowserWindow::on_medium_quality_activate ()
{
	if (getMediumQualityMenuItem () .get_active ())
		on_primitiveQuality_activate ("MEDIUM");
}

void
BrowserWindow::on_low_quality_activate ()
{
	if (getLowQualityMenuItem () .get_active ())
		on_primitiveQuality_activate ("LOW");
}

void
BrowserWindow::on_primitiveQuality_activate (const std::string & value)
{
	if (changing)
		return;

	getBrowser () -> getBrowserOptions () -> primitiveQuality () .removeInterest (this, &BrowserWindow::set_primitiveQuality);
	getBrowser () -> getBrowserOptions () -> primitiveQuality () .addInterest (this, &BrowserWindow::connectPrimitiveQuality);

	getBrowser () -> getBrowserOptions () -> primitiveQuality () = value;
}

void
BrowserWindow::set_primitiveQuality (const X3D::SFString & value)
{
	changing = true;

	if (value == "HIGH")
		getHighQualityMenuItem () .set_active (true);

	else if (value == "LOW")
		getLowQualityMenuItem () .set_active (true);

	else
		getMediumQualityMenuItem () .set_active (true);

	changing = false;
}

void
BrowserWindow::connectPrimitiveQuality (const X3D::SFString & field)
{
	field .removeInterest (this, &BrowserWindow::connectPrimitiveQuality);
	field .addInterest (this, &BrowserWindow::set_primitiveQuality);
}

// Object Icons

void
BrowserWindow::on_backgrounds_toggled ()
{
	if (toggle)
	{
		const bool hidden = not getBackgroundsMenuItem () .get_active ();

		X3D::traverse (getExecutionContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
		               {
		                  const auto background = dynamic_cast <X3D::X3DBackgroundNode*> (node .getValue ());

		                  if (background)
									background -> isHidden (hidden);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}

	toggle = true;
}

void
BrowserWindow::on_fogs_toggled ()
{
	if (toggle)
	{
		const bool hidden = not getFogsMenuItem () .get_active ();

		X3D::traverse (getExecutionContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
		               {
		                  const auto fog = dynamic_cast <X3D::X3DFogObject*> (node .getValue ());

		                  if (fog)
									fog -> isHidden (hidden);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}

	toggle = true;
}

// Object Icons

void
BrowserWindow::on_lights_toggled ()
{
	if (toggle)
	{
		if (getLightsMenuItem () .get_active ())
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::X3DLightNode*> (node .getValue ());

			                  if (tool)
										tool -> addTool ();

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
		else
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
			               {
			                  if (not node)
										return true;

			                  for (const auto & type: basic::make_reverse_range (node -> getType ()))
			                  {
			                     if (type == X3D::X3DConstants::X3DLightNodeTool)
			                     {
			                        if (not getBrowser () -> getSelection () -> isSelected (node))
												node -> removeTool (true);

			                        break;
										}
									}

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
	}

	toggle = true;
}

void
BrowserWindow::on_proximity_sensors_toggled ()
{
	if (toggle)
	{
		if (getProximitySensorsMenuItem () .get_active ())
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::ProximitySensor*> (node .getValue ());

			                  if (tool)
										tool -> addTool ();

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
		else
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::ProximitySensorTool*> (node .getValue ());

			                  if (tool and not getBrowser () -> getSelection () -> isSelected (node))
										tool -> removeTool (true);

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
	}

	toggle = true;
}

void
BrowserWindow::on_visibility_sensors_toggled ()
{
	if (toggle)
	{
		if (getVisibilitySensorsMenuItem () .get_active ())
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::VisibilitySensor*> (node .getValue ());

			                  if (tool)
										tool -> addTool ();

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
		else
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::VisibilitySensorTool*> (node .getValue ());

			                  if (tool and not getBrowser () -> getSelection () -> isSelected (node))
										tool -> removeTool (true);

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
	}

	toggle = true;
}

void
BrowserWindow::on_viewpoints_toggled ()
{
	if (toggle)
	{
		if (getViewpointsMenuItem () .get_active ())
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
			               {
			                  const auto tool = dynamic_cast <X3D::X3DViewpointNode*> (node .getValue ());

			                  if (tool)
										tool -> addTool ();

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
		else
		{
			X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
			               {
			                  if (not node)
										return true;

			                  for (const auto & type: basic::make_reverse_range (node -> getType ()))
			                  {
			                     if (type == X3D::X3DConstants::X3DViewpointNodeTool)
			                     {
			                        if (not getBrowser () -> getSelection () -> isSelected (node))
												node -> removeTool (true);

			                        break;
										}
									}

			                  return true;
								},
			               true,
			               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
		}
	}

	toggle = true;
}

void
BrowserWindow::on_hide_all_object_icons_activate ()
{
	if (getLightsMenuItem () .get_active ())
		getLightsMenuItem () .set_active (false);

	if (getProximitySensorsMenuItem () .get_active ())
		getProximitySensorsMenuItem () .set_active (false);

	if (getVisibilitySensorsMenuItem () .get_active ())
		getVisibilitySensorsMenuItem () .set_active (false);

	if (getViewpointsMenuItem () .get_active ())
		getViewpointsMenuItem () .set_active (false);
}

void
BrowserWindow::on_rubberband_toggled ()
{
	getConfig () .setItem ("rubberBand", getRubberbandMenuItem () .get_active ());
	getBrowser () -> getBrowserOptions () -> rubberBand () = getRubberbandMenuItem () .get_active ();
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
	const auto undoStep = std::make_shared <UndoStep> ();

	getSelection () -> setChildren (getExecutionContext () -> getRootNodes (), undoStep);
}

void
BrowserWindow::on_deselect_all_activate ()
{
	const auto undoStep = std::make_shared <UndoStep> ();

	getSelection () -> clear (undoStep);
}

void
BrowserWindow::on_hide_selected_objects_activate ()
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	X3D::traverse (selection, [ ] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
								shape -> isHidden (true);

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_hide_unselected_objects_activate ()
{
	std::set <X3D::X3DShapeNode*> visibles;

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	X3D::traverse (selection, [&visibles] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
								visibles .emplace (shape);

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	X3D::traverse (getExecutionContext () -> getRootNodes (), [&visibles] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
	                  {
	                     if (not visibles .count (shape))
									shape -> isHidden (true);
							}

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_show_selected_objects_activate ()
{
	auto selection = getBrowser () -> getSelection () -> getChildren ();

	X3D::traverse (selection, [ ] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
								shape -> isHidden (false);

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_show_all_objects_activate ()
{
	X3D::traverse (getExecutionContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
								shape -> isHidden (false);

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_select_lowest_toggled ()
{
	getConfig () .setItem ("selectLowest", getSelectLowestMenuItem () .get_active ());

	getBrowser () -> getSelection () -> setSelectLowest (getSelectLowestMenuItem () .get_active ());
}

void
BrowserWindow::on_follow_primary_selection_toggled ()
{
	getConfig () .setItem ("followPrimarySelection", getFollowPrimarySelectionMenuItem () .get_active ());

	if (getFollowPrimarySelectionMenuItem () .get_active ())
		getBrowser () -> getSelection () -> getPickedTime () .addInterest (this, &BrowserWindow::set_pickedTime);

	else
		getBrowser () -> getSelection () -> getPickedTime () .removeInterest (this, &BrowserWindow::set_pickedTime);
}

void
BrowserWindow::set_pickedTime ()
{
	if (not getBrowser () -> getSelection () -> getChildren () .empty ())
		getOutlineTreeView () -> expand_to (getBrowser () -> getSelection () -> getChildren () .back ());
}

// Layout

void
BrowserWindow::on_grid_tool_activate ()
{
	__LOG__ << std::endl;
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
	getWindow () .resize (1024, 1024.0 * 9.0 / 16.0);
}

/// Toolbar

void
BrowserWindow::on_node_properties_editor_clicked ()
{
	addDialog ("NodePropertiesEditor");
}

void
BrowserWindow::on_appearance_editor_clicked ()
{
	addDialog ("AppearanceEditor");
}

void
BrowserWindow::on_texture_editor_clicked ()
{
	addDialog ("TextureEditor");
}

void
BrowserWindow::on_text_editor_clicked ()
{
	addDialog ("TextEditor");
}

void
BrowserWindow::on_geometry_properties_editor_clicked ()
{
	addDialog ("GeometryPropertiesEditor");
}

void
BrowserWindow::on_viewpoint_editor_clicked ()
{
	addDialog ("ViewpointEditor");
}

void
BrowserWindow::on_light_editor_clicked ()
{
	addDialog ("LightEditor");
}

void
BrowserWindow::on_lod_editor_clicked ()
{
	addDialog ("LODEditor");
}

void
BrowserWindow::on_inline_editor_clicked ()
{
	addDialog ("InlineEditor");
}

void
BrowserWindow::on_prototype_instance_dialog_clicked ()
{
	std::dynamic_pointer_cast <PrototypeInstanceDialog> (addDialog ("PrototypeInstanceDialog", false)) -> run ();
}

void
BrowserWindow::on_node_index_clicked ()
{
	std::dynamic_pointer_cast <NodeIndex> (getBrowserWindow () -> addDialog ("NodeIndex")) -> setNamedNodes ();
}

// Browser dashboard handling

void
BrowserWindow::set_dashboard (const bool value)
{
	getDashboard () .set_visible (getEditorMenuItem () .get_active () or value);
}

void
BrowserWindow::on_hand_button_toggled ()
{
	if (getHandButton () .get_active ())
	{
		getConfig () .setItem ("arrow", false);
		getBrowser () -> getSelection () -> isEnabled (false);
	}

	set_available_viewers (getBrowser () -> getAvailableViewers ());
}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		getConfig () .setItem ("arrow", true);
		getBrowser () -> getSelection () -> isEnabled (true);
	}

	set_available_viewers (getBrowser () -> getAvailableViewers ());
}

void
BrowserWindow::on_play_pause_button_clicked ()
{
	isLive (not getConfig () .getBoolean ("isLive"));
}

void
BrowserWindow::on_select_parent_button_clicked ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	std::set <X3D::SFNode> parentIndex;

	for (const auto & child : selection)
	{
		bool hasParents = false;

		for (const auto & parent : getParentNodes (child))
		{
			if (parent == child -> getExecutionContext ())
				continue;

			if (parent -> getExecutionContext () not_eq child -> getExecutionContext ())
				continue;

			for (const auto & type : basic::make_reverse_range (parent -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGroupingNode      :
					case X3D::X3DConstants::X3DPrototypeInstance :
						{
							hasParents = true;
							parentIndex .emplace (parent);
							break;
						}
					default:
						break;
				}
			}
		}

		if (not hasParents)
			parentIndex .emplace (child);
	}

	// Get all hierarchies sorted by length.

	std::multimap <size_t, std::vector <X3D::X3DChildObject*>>  hierachies;

	for (const auto & parent : parentIndex)
	{
		const auto hierarchy = X3D::find (parent -> getExecutionContext (), parent);

		if (hierarchy .empty ())
			continue;

		hierachies .emplace (hierarchy .size (), std::move (hierarchy));
	}

	// Sort out parent if parent is a child of another parent.

	std::map <X3D::X3DChildObject*, bool> tree;

	for (auto & pair : hierachies)
	{
		auto & hierarchy = pair .second;

		// Insert parent.

		tree .emplace (hierarchy .back (), true);

		// Insert other parents.

		for (const auto & object : std::make_pair (hierarchy .crbegin () + 1, hierarchy .crend ()))
		{
			if (tree .emplace (object, false) .first -> second)
			{
				hierarchy .back () = nullptr;
				break;
			}
		}
	}

	X3D::MFNode parents;

	for (const auto & hierarchy : hierachies)
	{
		if (hierarchy .second .back ())
			parents .emplace_back (hierarchy .second .back ());
	}

	//	{ // DEBUG
	//		__LOG__ << parents .size () << std::endl;
	//
	//		for (const auto & parent : parents)
	//			__LOG__ << parent -> getTypeName () << std::endl;
	//	} // DEBUG

	if (parents .empty ())
		return;

	// Select and expand.

	getBrowser () -> getSelection () -> setChildren (parents);
	expandNodes (parents);
}

void
BrowserWindow::on_select_children_button_clicked ()
{
	const auto selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	X3D::MFNode children;

	for (const auto & parent : selection)
	{
		const auto nodes = getChildren (parent);

		if (nodes .empty ())
			children .emplace_back (parent);

		else
			children .insert (children .end (), nodes .begin (), nodes .end ());
	}

	getBrowser () -> getSelection () -> setChildren (children);
	expandNodes (children);
}

X3D::MFNode
BrowserWindow::getChildren (const X3D::SFNode & parent, const bool sharedNodes) const
{
	X3D::MFNode children;

	for (const auto & type : basic::make_reverse_range (parent -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::LOD :
				{
					const auto lodNode = dynamic_cast <X3D::LOD*> (parent .getValue ());
					const auto index   = lodNode -> level_changed ();

					if (index >= 0 and index < (int32_t) lodNode -> children () .size ())
					{
						const auto & child = lodNode -> children () [index];

						if (not child)
							return children;

						if (not sharedNodes)
						{
							if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
								return children;
						}

						children .emplace_back (child);
					}

					return children;
				}
			case X3D::X3DConstants::Switch:
			{
				const auto switchNode = dynamic_cast <X3D::Switch*> (parent .getValue ());
				const auto index      = switchNode -> whichChoice ();

				if (index >= 0 and index < (int32_t) switchNode -> children () .size ())
				{
					const auto & child = switchNode -> children () [index];

					if (not child)
						return children;

					if (not sharedNodes)
					{
						if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
							return children;
					}

					children .emplace_back (child);
				}

				return children;
			}
			case X3D::X3DConstants::X3DGroupingNode:
			{
				const auto groupingNode = dynamic_cast <X3D::X3DGroupingNode*> (parent .getValue ());

				for (const auto & child : groupingNode -> children ())
				{
					if (not child)
						return children;

					if (not sharedNodes)
					{
						if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
							return children;
					}

					children .emplace_back (child);
				}

				return children;
			}
			case X3D::X3DConstants::X3DPrototypeInstance:
			{
				const auto instance = dynamic_cast <X3D::X3DPrototypeInstance*> (parent .getValue ());

				// Create child index.

				std::set <X3D::SFNode> childIndex;

				for (const auto & field : instance -> getFieldDefinitions ())
				{
					if (field -> isInitializable ())
					{
						switch (field -> getType ())
						{
							case X3D::X3DConstants::SFNode :
								{
									const auto child = *static_cast <X3D::SFNode*> (field);

									if (not child)
										break;

									if (child == parent)
										break;

									if (not sharedNodes)
									{
										if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
											break;
									}

									childIndex .emplace (child);

									break;
								}
							case X3D::X3DConstants::MFNode:
							{
								const auto mfnode = *static_cast <X3D::MFNode*> (field);

								for (const auto & child : mfnode)
								{
									if (not child)
										continue;

									if (child == parent)
										continue;

									if (not sharedNodes)
									{
										if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
											continue;
									}

									childIndex .emplace (child);
								}

								break;
							}
							default:
								break;
						}
					}
				}

				// Get children.

				try
				{
					std::set <X3D::SFNode> seen;

					const auto nodes = getChildrenInProtoinstance (instance -> getRootNode (), childIndex, seen);

					children .insert (children .end (), nodes .begin (), nodes .end ());
				}
				catch (const X3D::X3DError &)
				{ }

				return children;
			}
			default:
				break;
		}
	}

	return children;
}

X3D::MFNode
BrowserWindow::getChildrenInProtoinstance (const X3D::SFNode & parent, const std::set <X3D::SFNode> & childIndex, std::set <X3D::SFNode> & seen) const
{
	X3D::MFNode children;

	if (not seen .emplace (parent) .second)
		return children;

	for (const auto & child : getChildren (parent, true))
	{
		if (childIndex .count (child))
			children .emplace_back (child);

		else if (child -> getExecutionContext () == parent -> getExecutionContext ())
		{
			const auto nodes = getChildrenInProtoinstance (child, childIndex, seen);

			children .insert (children .end (), nodes .begin (), nodes .end ());
		}
	}

	return children;
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
		case X3D::ViewerType::LOOKAT:
		{
			getHandButton ()  .set_sensitive (false);
			getArrowButton () .set_sensitive (false);

			//if (getArrowButton () .get_active ())
			//getSelection () -> disconnect ();

			break;
		}
		default:
		{
			viewer = type;

			getHandButton ()  .set_sensitive (true);
			getArrowButton () .set_sensitive (true);

			//if (getArrowButton () .get_active ())
			//getSelection () -> connect ();

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
		case X3D::ViewerType::LOOKAT:
		{
			if (not getLookAtButton () .get_active ())
				getLookAtButton () .set_active (true);

			break;
		}
	}
}

void
BrowserWindow::set_available_viewers (const X3D::MFEnum <X3D::ViewerType> & availableViewers)
{
	const bool editor = getEditorMenuItem () .get_active () and getArrowButton () .get_active ();

	bool examine = editor;
	bool walk    = editor;
	bool fly     = editor;
	bool plane   = editor;
	bool none    = editor;
	bool lookat  = editor;

	for (const auto & viewer : availableViewers)
	{
		switch (viewer)
		{
			case X3D::ViewerType::EXAMINE :
				examine = true;
				break;
			case X3D::ViewerType::WALK:
				walk = true;
				break;
			case X3D::ViewerType::FLY:
				fly = true;
				break;
			case X3D::ViewerType::PLANE:
				plane = true;
				break;
			case X3D::ViewerType::NONE:
				none = true;
				break;
			case X3D::ViewerType::LOOKAT:
				lookat = true;
				break;
		}
	}

	getExamineViewerMenuItem () .set_visible (examine);
	getWalkViewerMenuItem ()    .set_visible (walk);
	getFlyViewerMenuItem ()     .set_visible (fly);
	getPlaneViewerMenuItem ()   .set_visible (plane);
	getNoneViewerMenuItem ()    .set_visible (none);

	getLookAtSeparator () .set_visible (lookat);
	getLookAtAllButton () .set_visible (lookat);
	getLookAtButton ()    .set_visible (lookat);
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

/*
 *  Look at
 */

void
BrowserWindow::on_look_at_selection_clicked ()
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	const auto & selection = getBrowser () -> getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto activeViewpoint = getBrowser () -> getActiveLayer () -> getViewpoint ();

	X3D::Box3f bbox;

	for (const auto & node : selection)
	{
		const auto boundedObject = X3D::x3d_cast <X3D::X3DBoundedObject*> (node);

		if (boundedObject)
			bbox += boundedObject -> getBBox () * X3D::Matrix4f (findModelViewMatrix (boundedObject));
	}

	activeViewpoint -> lookAt (bbox);
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
		if (getBrowser () -> getViewer () not_eq X3D::ViewerType::LOOKAT)
			getBrowser () -> setViewer (X3D::ViewerType::LOOKAT);
	}
	else
	{
		if (getBrowser () -> getViewer () not_eq viewer)
			getBrowser () -> setViewer (viewer);
	}
}

BrowserWindow::~BrowserWindow ()
{
	dispose ();
}

} // puck
} // titania
