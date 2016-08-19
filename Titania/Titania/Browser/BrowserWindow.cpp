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

#include "BrowserWindow.h"

#include "../Dialogs/FileImportDialog/FileImportDialog.h"
#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveACopyDialog.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Dialogs/NodeIndex/NodeIndex.h"
#include "../Dialogs/OpenLocationDialog/OpenLocationDialog.h"

#include "../Editors/GridEditor/X3DGridTool.h"
#include "../Editors/PrototypeEditor/PrototypeEditor.h"

#include "../Widgets/Footer/Footer.h"
#include "../Widgets/Sidebar/Sidebar.h"

#include "../Browser/BrowserSelection.h"
#include "../Browser/BrowserUserData.h"
#include "../Configuration/config.h"
#include "../Editors/GeometryEditor/GeometryEditor.h"

#include <Titania/X3D/Browser/BrowserOptions.h>
#include <Titania/X3D/Browser/RenderingProperties.h>
#include <Titania/X3D/Browser/Navigation/X3DViewer.h>
#include <Titania/X3D/Browser/Tools/TransformToolOptions.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/EnvironmentalEffects/X3DBackgroundNode.h>
#include <Titania/X3D/Components/EnvironmentalSensor/TransformSensor.h>
#include <Titania/X3D/Components/EnvironmentalSensor/VisibilitySensor.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/LOD.h>
#include <Titania/X3D/Components/Sound/Sound.h>
#include <Titania/X3D/Editing/Combine.h>
#include <Titania/X3D/Tools/Grids/X3DGridTool.h>
#include <Titania/X3D/Types/MatrixStack.h>

#include <Titania/OS.h>
#include <Titania/String.h>

namespace titania {
namespace puck {


const std::set <X3D::X3DConstants::NodeType> BrowserWindow::proximitySensors = {
	X3D::X3DConstants::ProximitySensor,
	X3D::X3DConstants::GeoProximitySensor,
	X3D::X3DConstants::ViewpointGroup
};

BrowserWindow::BrowserWindow (const X3D::BrowserPtr & browser) :
	         X3DBaseInterface (this, browser),
	X3DBrowserWindowInterface (get_ui ("BrowserWindow.glade")),
	         X3DBrowserWindow (browser),
	              cssProvider (Gtk::CssProvider::create ()),
	       environmentActions (),
	           shadingActions (),
	  primitiveQualityActions (),
	    textureQualityActions (),
	                   viewer (X3D::X3DConstants::NoneViewer),
	                 changing (false)
{
	environmentActions = {
		getEditorAction (),
		getBrowserAction ()
	};

	shadingActions = {
		getPhongAction (),
		getGouraudAction (),
		getFlatAction (),
		getWireframeAction (),
		getPointsetAction ()
	};

	primitiveQualityActions = {
		getPrimitiveQualityHighAction (),
		getPrimitiveQualityMediumAction (),
		getPrimitiveQualityLowAction (),
	};

	textureQualityActions = {
		getTextureQualityHighAction (),
		getTextureQualityMediumAction (),
		getTextureQualityLowAction (),
	};

	//if (getConfig () -> getBoolean ("transparent"))
	//	setTransparent (true);
	//else
		browser -> setAntialiasing (4);

	//if (not getConfig () -> hasItem ("maximized"))
	//	getWindow () .maximize ();

	setup ();
}

void
BrowserWindow::configure ()
{
	X3DBrowserWindow::configure ();

	getTransformToolModeAction () -> set_active (getConfig () -> getInteger ("transformToolMode"));
}

void
BrowserWindow::store ()
{
	getConfig () -> setItem ("transformToolMode", (int32_t) getTransformToolModeAction () -> get_active ());

	X3DBrowserWindow::store ();
}

void
BrowserWindow::initialize ()
{
	X3DBrowserWindow::initialize ();

	loadStyles ();

	// Drag & drop targets
	std::vector <Gtk::TargetEntry> targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list")
	};

	getToolbar ()         .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
	getBrowserNotebook () .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	// Browser events
	getBrowsers ()       .addInterest (this, &BrowserWindow::set_browsers);
	getCurrentScene ()   .addInterest (this, &BrowserWindow::set_scene);
	getCurrentContext () .addInterest (this, &BrowserWindow::set_executionContext);

	getViewerButton () .set_menu (getViewerTypeMenu ());

	// Window
	getWindow () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));
	getWidget () .grab_focus ();
}

void
BrowserWindow::loadStyles () 
{
	try
	{
		Glib::RefPtr <Gtk::CssProvider> fileCssProvider = Gtk::CssProvider::create ();
		fileCssProvider -> load_from_path (get_ui ("style.css"));
		Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), fileCssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

		Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		on_style_updated ();
	}
	catch (const Glib::Error & error)
	{
	   __LOG__ << error .what () << std::endl;
	}
}

void
BrowserWindow::setBrowser (const X3D::BrowserPtr & value)
{
	// Disconnect

	getCurrentBrowser () -> getViewer ()           .removeInterest (this, &BrowserWindow::set_viewer);
	getCurrentBrowser () -> getActiveLayer ()      .removeInterest (this, &BrowserWindow::set_activeLayer);
	getCurrentBrowser () -> getViewerType ()       .removeInterest (this, &BrowserWindow::set_viewer);
	getCurrentBrowser () -> getPrivateViewer ()    .removeInterest (this, &BrowserWindow::set_viewer);
	getCurrentBrowser () -> getAvailableViewers () .removeInterest (this, &BrowserWindow::set_available_viewers);

	getCurrentBrowser () -> getBrowserOptions () -> Dashboard ()        .removeInterest (this, &BrowserWindow::set_dashboard);
	getCurrentBrowser () -> getBrowserOptions () -> Shading ()          .removeInterest (this, &BrowserWindow::set_shading);
	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .removeInterest (this, &BrowserWindow::set_primitiveQuality);

	getUserData (getCurrentBrowser ()) -> browserHistory .removeInterest (this, &BrowserWindow::set_browserHistory);

	// Set browser

	X3DBrowserWindow::setBrowser (value);

	// Connect

	getCurrentBrowser () -> getActiveLayer ()      .addInterest (this, &BrowserWindow::set_activeLayer);
	getCurrentBrowser () -> getViewerType ()       .addInterest (this, &BrowserWindow::set_viewer);
	getCurrentBrowser () -> getPrivateViewer ()    .addInterest (this, &BrowserWindow::set_viewer);
	getCurrentBrowser () -> getAvailableViewers () .addInterest (this, &BrowserWindow::set_available_viewers);

	getCurrentBrowser () -> getBrowserOptions () -> Dashboard ()        .addInterest (this, &BrowserWindow::set_dashboard);
	getCurrentBrowser () -> getBrowserOptions () -> Shading ()          .addInterest (this, &BrowserWindow::set_shading);
	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .addInterest (this, &BrowserWindow::set_primitiveQuality);

	getUserData (getCurrentBrowser ()) -> browserHistory .addInterest (this, &BrowserWindow::set_browserHistory);

	// Initialize

	set_activeLayer ();
	set_viewer ();
	set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());

	set_dashboard (getCurrentBrowser () -> getBrowserOptions () -> Dashboard ());
	set_shading (getCurrentBrowser () -> getBrowserOptions () -> Shading ());
	set_primitiveQuality (getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality ());
	set_textureQuality (getCurrentBrowser () -> getBrowserOptions () -> TextureQuality ());

	set_browserHistory ();

	getCurrentBrowser () -> getBrowserOptions () -> RubberBand ()   = getRubberbandAction () -> get_active ();
	getCurrentBrowser () -> getRenderingProperties () -> Enabled () = getRenderingPropertiesAction () -> get_active ();

	on_transform_tool_mode_toggled ();
}

void
BrowserWindow::set_browsers ()
{
}

void
BrowserWindow::set_scene ()
{
	changing = true;

	// View Menu

	getBackgroundsAction ()       -> set_active (true);
	getFogsAction ()              -> set_active (true);
	getLightsAction ()            -> set_active (false);
	getProximitySensorsAction ()  -> set_active (false);
	getSoundsAction ()            -> set_active (false);
	getTransformSensorsAction ()  -> set_active (false);
	getVisibilitySensorsAction () -> set_active (false);
	getViewpointsAction ()        -> set_active (false);

	changing = false;
}

void
BrowserWindow::set_activeLayer ()
{
	changing = true;

	// Layout Menu

	getGridLayoutToolAction () -> set_active (getGridTool () -> isEnabled ());
	getGridTool () -> update ();

	getAngleLayoutToolAction () -> set_active (getAngleTool () -> isEnabled ());
	getAngleTool () -> update ();

	changing = false;
}

void
BrowserWindow::set_executionContext ()
{
	const bool inScene = not inPrototypeInstance ();

	// Window menu

	getImportMenuItem ()                 .set_sensitive (inScene);
	getSaveMenuItem ()                   .set_sensitive (inScene);
	getSaveAsMenuItem ()                 .set_sensitive (inScene);
	getRemoveUnusedPrototypesMenuItem () .set_sensitive (inScene);
	getEditMenuItem ()                   .set_sensitive (inScene);

	// Browser menu

	getBrowserImportMenuItem ()                 .set_sensitive (inScene);
	getBrowserSaveMenuItem ()                   .set_sensitive (inScene);
	getBrowserSaveAsMenuItem ()                 .set_sensitive (inScene);
	getBrowserRemoveUnusedPrototypesMenuItem () .set_sensitive (inScene);
	getBrowserEditMenuItem ()                   .set_sensitive (inScene);

	// Toolbar

	getSaveButton ()                    .set_sensitive (inScene);
	getImportButton ()                  .set_sensitive (inScene);
	getCreatePrototypeInstanceButton () .set_sensitive (inScene);

	changing = true;

	getLocationEntry () .set_text (getCurrentContext () -> getWorldURL () .str ());
	getLocationEntry () .set_icon_from_stock (Gtk::StockID (getCurrentContext () -> getMasterScene () -> getWorldURL () .filename () .str ()), Gtk::ENTRY_ICON_PRIMARY);

	changing = false;

	// Selection

	set_selection (getSelection () -> getChildren ());
}

void
BrowserWindow::set_browserHistory ()
{
	const auto & browserHistory = getUserData (getCurrentBrowser ()) -> browserHistory;

	getPreviousButton () .set_sensitive (browserHistory .hasPreviousPage ());
	getNextButton ()     .set_sensitive (browserHistory .hasNextPage ());
}

// Selection

void
BrowserWindow::set_touchTime ()
{
	if (getSelection () -> getChildren () .empty ())
		return;

	expandNodes ({ getSelection () -> getChildren () .back () });
}

void
BrowserWindow::set_selection (const X3D::MFNode & selection)
{
	X3DBrowserWindow::set_selection (selection);

	const bool inScene        = not inPrototypeInstance ();
	const bool haveSelection  = inScene and selection .size ();
	const bool haveSelections = inScene and selection .size () > 1;

	// Window menu

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

	getGeometryMenuItem () .set_sensitive (haveSelection);

	// Browser menu

	getBrowserCutMenuItem ()    .set_sensitive (haveSelection);
	getBrowserCopyMenuItem ()   .set_sensitive (haveSelection);
	getBrowserDeleteMenuItem () .set_sensitive (haveSelection);

	getBrowserCloneMenuItem ()       .set_sensitive (haveSelection);
	getBrowserCreateCloneMenuItem () .set_sensitive (haveSelections);
	getBrowserUnlinkCloneMenuItem () .set_sensitive (haveSelection);

	getBrowserGroupSelectedNodesMenuItem () .set_sensitive (haveSelection);
	getBrowserUngroupMenuItem ()            .set_sensitive (haveSelection);
	getBrowserAddToGroupMenuItem ()         .set_sensitive (haveSelections);
	getBrowserDetachFromGroupMenuItem ()    .set_sensitive (haveSelection);
	getBrowserCreateParentMenuItem ()       .set_sensitive (haveSelection);

	getBrowserDeselectAllMenuItem ()           .set_sensitive (selection .size ());
	getBrowserHideSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);
	getBrowserHideUnselectedObjectsMenuItem () .set_sensitive (haveSelection);
	getBrowserShowSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);

	getBrowserGeometryMenuItem () .set_sensitive (haveSelection);

	// Dashboard

	getSelectParentButton ()    .set_sensitive (haveSelection);
	getSelectChildrenButton ()  .set_sensitive (haveSelection);
	getLookAtSelectionButton () .set_sensitive (haveSelection);

	// Show/Hide Object Icons.

	changing = true;

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::X3DLightNode*> (node))
		{
			getLightsAction () -> set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		try
		{
			if (node -> getInnerNode () -> isType (proximitySensors))
			{
				getProximitySensorsAction () -> set_active (true);
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::Sound*> (node))
		{
			getSoundsAction () -> set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::TransformSensor*> (node))
		{
			getTransformSensorsAction () -> set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::VisibilitySensor*> (node))
		{
			getVisibilitySensorsAction () -> set_active (true);
			break;
		}
	}

	for (const auto & node : selection)
	{
		if (X3D::x3d_cast <X3D::X3DViewpointNode*> (node))
		{
			getViewpointsAction () -> set_active (true);
			break;
		}
	}

	changing = false;
}

// Style

void
BrowserWindow::on_style_updated ()
{
//	try
//	{
//		const auto styleContext = getWidget () .get_style_context ();
//		const auto fg_normal    = styleContext -> get_color (Gtk::STATE_FLAG_NORMAL);
//		const auto fg_selected  = styleContext -> get_color (Gtk::STATE_FLAG_SELECTED);
//		const auto bg_normal    = styleContext -> get_background_color (Gtk::STATE_FLAG_NORMAL);
//		const auto bg_selected  = styleContext -> get_background_color (Gtk::STATE_FLAG_SELECTED);
//
//		std::string string;
//
//		string += "#OutlineTreeViewEditor .titania-textview-editable GtkTextView {";
//		string += "  background-color: mix (" + bg_selected .to_string () + ", " + bg_normal .to_string () + ", 0.9);";
//		string += "}";
//		string += "";
//		string += "#OutlineTreeViewEditor .titania-textview-editable GtkTextView:selected {";
//		string += "  color: " + fg_selected .to_string () + ";";
//		string += "  background-color: " + bg_selected .to_string () + ";";
//		string += "}";
//		string += "GtkExpander > GtkLabel {";
//		string += "	border-bottom: 1px solid " + fg_normal .to_string () + ";";
//		string += "}";
//
//		cssProvider -> load_from_data (string);
//	}
//	catch (const Glib::Error & error)
//	{
//	   __LOG__ << error .what () << std::endl;
//	}
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
	getKeys () .press (event);

	if (getCurrentBrowser () -> on_external_key_press_event (event))
		return false;

	if (not getCurrentBrowser () -> has_focus ())
	   return false;

	if (not hasAccelerators ())
		return false;

	getSelection () -> setMode (getKeys () .shift () and not getKeys () .control () ? X3D::Selection::MULTIPLE : X3D::Selection::SINGLE);

	// Nudge selection.

	const bool editMode = getConfig () -> getBoolean ("arrow");

	if (editMode)
	{
		if (not inPrototypeInstance () and not getSelection () -> getChildren () .empty ())
		{
			static constexpr float NUDGE_STEP   = 0.001;
			static constexpr float NUDGE_FACTOR = 10;

			const float nudge           = NUDGE_STEP * (getKeys () .shift () ? NUDGE_FACTOR : 1);
			const bool  alongFrontPlane = false;

			switch (event -> keyval)
			{
				case GDK_KEY_Up:
				{
					if (getKeys () .control ())
						translateSelection (X3D::Vector3f (0, 0, -nudge), alongFrontPlane, NUDGE_BACK);
					else
						translateSelection (X3D::Vector3f (0, nudge, 0), alongFrontPlane, NUDGE_UP);

					return true;
				}
				case GDK_KEY_Down:
				{
					if (getKeys () .control ())
						translateSelection (X3D::Vector3f (0, 0, nudge), alongFrontPlane, NUDGE_FRONT);
					else
						translateSelection (X3D::Vector3f (0, -nudge, 0), alongFrontPlane, NUDGE_DOWN);

					return true;
				}
				case GDK_KEY_Left:
				{
					translateSelection (X3D::Vector3f (-nudge, 0, 0), alongFrontPlane, NUDGE_LEFT);
					return true;
				}
				case GDK_KEY_Right:
				{
					translateSelection (X3D::Vector3f (nudge, 0, 0), alongFrontPlane, NUDGE_RIGHT);
					return true;
				}
				default:
					break;
			}
		}
	}

	// Change viewpoint.

	switch (event -> keyval)
	{
		case GDK_KEY_Home:
		{
			getCurrentBrowser () -> firstViewpoint ();
			return true;
		}
		case GDK_KEY_Page_Up:
		{
			getCurrentBrowser () -> previousViewpoint ();
			return true;
		}
		case GDK_KEY_Page_Down:
		{
			getCurrentBrowser () -> nextViewpoint ();
			return true;
		}
		case GDK_KEY_End:
		{
			getCurrentBrowser () -> lastViewpoint ();
			return true;
		}
		default:
			break;
	}

	return false;
}

bool
BrowserWindow::on_key_release_event (GdkEventKey* event)
{
	getKeys () .release (event);

	getSelection () -> setMode (getKeys () .shift () and not getKeys () .control ()
	                            ? X3D::Selection::MULTIPLE
	                            : X3D::Selection::SINGLE);

	if (getCurrentBrowser () -> on_external_key_release_event (event))
		return false;

	return false;
}

bool
BrowserWindow::on_menubar_button_press_event (GdkEventButton* event)
{
	getWidget () .grab_focus ();
	return false;
}

bool
BrowserWindow::on_notebook_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
		getBrowserMenu () .popup (event -> button, event -> time);
		return true;
	}

	return false;
}

// File menu

void
BrowserWindow::on_new_activated ()
{
	blank ();
}

void
BrowserWindow::on_open_activated ()
{
	std::dynamic_pointer_cast <FileOpenDialog> (addDialog ("FileOpenDialog", false)) -> loadURL ();
}

void
BrowserWindow::on_open_recent_activated ()
{
	openRecent ();
}

void
BrowserWindow::on_open_location_activated ()
{
	std::dynamic_pointer_cast <OpenLocationDialog> (addDialog ("OpenLocationDialog", false)) -> run ();
}

void
BrowserWindow::on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	on_drag_data_received (context, selection_data, time, true);
}

void
BrowserWindow::on_import_activated ()
{
	std::dynamic_pointer_cast <FileImportDialog> (addDialog ("FileImportDialog", false)) -> run ();
}

void
BrowserWindow::on_browser_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	on_drag_data_received (context, selection_data, time, getBrowserAction () -> get_active ());
}

void
BrowserWindow::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
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
			{
			   const auto file = Gio::File::create_for_uri (Glib::uri_unescape_string (string));

				uris .emplace_back ("file://" + file -> get_path ());
			}
		}

		if (not uris .empty ())
		{
			if (do_open)
			{
				open (uris [0]);
			}
			else
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import"));
				const auto nodes    = import (uris, undoStep);
	
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
BrowserWindow::on_save_activated ()
{
	const basic::uri worldURL = getCurrentScene () -> getWorldURL ();

	if (worldURL .empty () or worldURL .is_network ())
		on_save_as_activated ();

	else
		save (worldURL, getCurrentScene () -> isCompressed (), false);
}

void
BrowserWindow::on_save_as_activated ()
{
	std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false)) -> saveScene (false);
}

void
BrowserWindow::on_save_a_copy_activated ()
{
	std::dynamic_pointer_cast <FileSaveACopyDialog> (addDialog ("FileSaveACopyDialog", false)) -> saveScene (true);
}

void
BrowserWindow::on_export_activated ()
{
	std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false)) -> exportImage ();
}

void
BrowserWindow::on_remove_unused_prototypes_activated ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Unused Prototypes"));

	removeUnusedPrototypes (getCurrentContext (), undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_scene_properties_activated ()
{
	addDialog ("ScenePropertiesEditor");
}

void
BrowserWindow::on_close_activated ()
{
	close (X3D::BrowserPtr (getCurrentBrowser ()));
}

void
BrowserWindow::on_quit_activated ()
{
	quit ();
}

void
BrowserWindow::on_revert_to_saved_activated ()
{
	reload ();
}

// Undo/Redo

void
BrowserWindow::on_undo_activated ()
{
	undo ();
}

void
BrowserWindow::on_redo_activated ()
{
	redo ();
}

void
BrowserWindow::on_cut_activated ()
{
	if (getGeometryEditor () -> on_cut ())
	   return;

	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cut"));

	getSelection () -> clear (undoStep);

	cutNodes (getCurrentContext (), selection, undoStep);

	getSelection () -> undoRestoreSelection (undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_copy_activated ()
{
	if (getGeometryEditor () -> on_copy ())
		return;

	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	copyNodes (getCurrentContext (), selection);
}

void
BrowserWindow::on_paste_activated ()
{
	if (getGeometryEditor () -> on_paste ())
		return;

	auto selection = getSelection () -> getChildren ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Paste"));

	pasteNodes (getCurrentContext (), selection, undoStep);

	addUndoStep (undoStep);
}

// Edit menu

void
BrowserWindow::on_delete_activated ()
{
	if (getGeometryEditor () -> on_delete ())
		return;

	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Delete Node From Scene"));

	removeNodesFromScene (getCurrentContext (), selection, true, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_clone_activated ()
{
	auto selection = getSelection () -> getChildren ();

	if (selection .size () < 2)
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create Clone"));

	const auto clone = selection .back ();
	selection .pop_back ();

	createClone (getCurrentContext (), clone, selection, undoStep);

	getSelection () -> setChildren ({ clone }, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_unlink_clone_activated ()
{
	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Unlink Clone"));

	auto nodes = unlinkClone (getCurrentContext (), selection, undoStep);

	getSelection () -> setChildren (nodes, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_group_selected_nodes_activated ()
{
	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Group"));
	const auto group    = groupNodes (getCurrentContext (), "Transform", selection, undoStep);

	getSelection () -> setChildren ({ group }, undoStep);
	addUndoStep (undoStep);

	expandNodes (X3D::MFNode ({ group }));
}

void
BrowserWindow::on_ungroup_activated ()
{
	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Ungroup"));

	getSelection () -> clear (undoStep);

	const auto nodes = ungroupNodes (getCurrentContext (), selection, undoStep);

	getSelection () -> setChildren (nodes, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_add_to_group_activated ()
{
	auto selection = getSelection () -> getChildren ();

	if (selection .size () < 2)
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Add To Group"));

	getSelection () -> undoRestoreSelection (undoStep);

	const auto group = selection .back ();
	selection .pop_back ();

	if (addToGroup (getCurrentContext (), group, selection, undoStep))
	{
		getSelection () -> setChildren (selection, undoStep);

		addUndoStep (undoStep);
	}
}

void
BrowserWindow::on_detach_from_group_activated ()
{
	const auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Detach From Group"));

	getSelection () -> undoRestoreSelection (undoStep);
	getSelection () -> redoRestoreSelection (undoStep);

	detachFromGroup (getCurrentContext (), selection, getKeys () .shift (), undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_parent_transform_activated ()
{
	on_create_parent ("Transform");
}

void
BrowserWindow::on_create_parent_group_activated ()
{
	on_create_parent ("Group");
}

void
BrowserWindow::on_create_parent_static_group_activated ()
{
	on_create_parent ("StaticGroup");
}

void
BrowserWindow::on_create_parent_switch_activated ()
{
	on_create_parent ("Switch");
}

void
BrowserWindow::on_create_parent_billboard_activated ()
{
	on_create_parent ("Billboard");
}

void
BrowserWindow::on_create_parent_collision_activated ()
{
	on_create_parent ("Collision");
}

void
BrowserWindow::on_create_parent_lod_activated ()
{
	on_create_parent ("LOD");
}

void
BrowserWindow::on_create_parent_viewpoint_group_activated ()
{
	on_create_parent ("ViewpointGroup");
}

void
BrowserWindow::on_create_parent_anchor_activated ()
{
	on_create_parent ("Anchor");
}

void
BrowserWindow::on_create_parent_screen_group_activated ()
{
	on_create_parent ("ScreenGroup");
}

void
BrowserWindow::on_create_parent_layout_layer_activated ()
{
	on_create_parent ("LayoutLayer");
}

void
BrowserWindow::on_create_parent_layout_group_activated ()
{
	on_create_parent ("LayoutGroup");
}

void
BrowserWindow::on_create_parent_geo_transform_activated ()
{
	on_create_parent ("GeoTransform");
}

void
BrowserWindow::on_create_parent_geo_location_activated ()
{
	on_create_parent ("GeoLocation");
}

void
BrowserWindow::on_create_parent_cad_part_activated ()
{
	on_create_parent ("CADPart");
}

void
BrowserWindow::on_create_parent_cad_assembly_activated ()
{
	on_create_parent ("CADAssembly");
}

void
BrowserWindow::on_create_parent_cad_layer_activated ()
{
	on_create_parent ("CADLayer");
}

void
BrowserWindow::on_create_parent_layer_set_activated ()
{
	on_create_parent ("LayerSet", "layers");
}

void
BrowserWindow::on_create_parent_layer_activated ()
{
	on_create_parent ("Layer");
}

void
BrowserWindow::on_create_parent_viewport_activated ()
{
	on_create_parent ("Viewport");
}

void
BrowserWindow::on_create_parent (const std::string & typeName, const std::string & fieldName)
{
	auto selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Parent %s"), typeName .c_str ()));
	const auto group    = createParentGroup (getCurrentContext (), typeName, fieldName, selection, undoStep);

	getSelection () -> setChildren ({ group }, undoStep);
	addUndoStep (undoStep);

	expandNodes ({ group });
}

// View menu

void
BrowserWindow::on_menubar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem ("menubarFullscreen", getMenubarAction () -> get_active ());
	else
		getConfig () -> setItem ("menubar", getMenubarAction () -> get_active ());
	
	getMenubar () .set_visible (getMenubarAction () -> get_active ());
}

void
BrowserWindow::on_toolbar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem ("toolbarFullscreen", getToolbarAction () -> get_active ());
	else
		getConfig () -> setItem ("toolbar", getToolbarAction () -> get_active ());

	getToolbar () .set_visible (getToolbarAction () -> get_active ());
}

void
BrowserWindow::on_sidebar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem ("sidebarFullscreen", getSidebarAction () -> get_active ());
	else
		getConfig () -> setItem ("sidebar", getSidebarAction () -> get_active ());
	
	getSidebarBox () .set_visible (getSidebarAction () -> get_active ());
}

void
BrowserWindow::on_footer_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem ("footerFullscreen", getFooterAction () -> get_active ());
	else
		getConfig () -> setItem ("footer", getFooterAction () -> get_active ());

	getFooterBox () .set_visible (getFooterAction () -> get_active ());
}

void
BrowserWindow::on_tabs_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem ("tabsFullscreen", getTabsAction () -> get_active ());
	else
		getConfig () -> setItem ("tabs", getTabsAction () -> get_active ());

	getBrowserNotebook () .set_show_tabs (getShowTabs ());
}

void
BrowserWindow::on_browser_toggled ()
{
	if (getBrowserAction () -> get_active ())
	{
		toggleActions (getBrowserAction (), environmentActions);

		isEditor (false);
		isLive (true);

		if (not getBackgroundsAction () -> get_active ())
			getBackgroundsAction () -> set_active (true);

		if (not getFogsAction () -> get_active ())
			getFogsAction () -> set_active (true);

		on_hide_all_object_icons_activated ();
		on_show_all_objects_activated ();
	}
}

void
BrowserWindow::on_editor_toggled ()
{
	if (getEditorAction () -> get_active ())
	{
		toggleActions (getEditorAction (), environmentActions);

		isEditor (true);
	}
}

void
BrowserWindow::isEditor (const bool enabled)
{
	X3DBrowserWindow::isEditor (enabled);

	getOpenRecentMenuItem ()                   .set_visible (enabled);
	getImportMenuItem ()                       .set_visible (enabled);
	getSaveMenuItem ()                         .set_visible (enabled);
	getRemoveUnusedPrototypesMenuItem ()       .set_visible (enabled);
	getEditMenuItem ()                         .set_visible (enabled);
	getBrowserOptionsSeparator ()              .set_visible (enabled);
	getMotionBlurMenuItem ()                   .set_visible (enabled);
	getShadingMenuItem ()                      .set_visible (enabled);
	getShowHideEnvironmentalEffectsMenuItem () .set_visible (enabled);
	getObjectIconsMenuItem ()                  .set_visible (enabled);
	getSelectionMenuItem ()                    .set_visible (enabled);
	getGeometryMenuItem ()                     .set_visible (enabled);
	getLayoutMenuItem ()                       .set_visible (enabled);

	getBrowserOpenRecentMenuItem ()                   .set_visible (enabled);
	getBrowserImportMenuItem ()                       .set_visible (enabled);
	getBrowserSaveMenuItem ()                         .set_visible (enabled);
	getBrowserRemoveUnusedPrototypesMenuItem ()       .set_visible (enabled);
	getBrowserEditMenuItem ()                         .set_visible (enabled);
	getBrowserBrowserOptionsSeparator ()              .set_visible (enabled);
	getBrowserMotionBlurMenuItem ()                   .set_visible (enabled);
	getBrowserShadingMenuItem ()                      .set_visible (enabled);
	getBrowserShowHideEnvironmentalEffectsMenuItem () .set_visible (enabled);
	getBrowserObjectIconsMenuItem ()                  .set_visible (enabled);
	getBrowserSelectionMenuItem ()                    .set_visible (enabled);
	getBrowserGeometryMenuItem ()                     .set_visible (enabled);
	getBrowserLayoutMenuItem ()                       .set_visible (enabled);

	getLocationBar () .set_visible (not enabled);
	getEditToolBar () .set_visible (enabled);

	set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());

	getHandButton ()            .set_visible (enabled);
	getArrowButton ()           .set_visible (enabled);
	getPlayPauseButton ()       .set_visible (enabled);
	getSelectSeparator ()       .set_visible (enabled);
	getSelectParentButton ()    .set_visible (enabled);
	getSelectChildrenButton ()  .set_visible (enabled);
	getViewerSeparator ()       .set_visible (enabled);
	getLookAtSelectionButton () .set_visible (enabled and getArrowButton () .get_active ());

	getBrowserNotebook () .set_tab_pos (enabled ? Gtk::POS_BOTTOM : Gtk::POS_TOP);

	getSidebar () -> getLibraryViewBox ()     .set_visible (enabled);
	getSidebar () -> getOutlineEditorBox ()   .set_visible (enabled);
	getSidebar () -> getNodeEditorBox ()      .set_visible (enabled);
	getFooter ()  -> getScriptEditorBox ()    .set_visible (enabled);
	getFooter ()  -> getAnimationEditorBox () .set_visible (enabled);

	if (enabled and getConfig () -> getBoolean ("arrow"))
		getArrowButton () .set_active (true);
	else
		getHandButton () .set_active (true);
	
	set_dashboard (getCurrentBrowser () -> getBrowserOptions () -> Dashboard ());
}

void
BrowserWindow::on_motion_blur_activated ()
{
	addDialog ("MotionBlurEditor");
}

// Shading menu

void
BrowserWindow::on_phong_toggled ()
{
	if (getPhongAction () -> get_active ())
	{
		toggleActions (getPhongAction (), shadingActions);

		on_shading_changed ("PHONG");
	}
}

void
BrowserWindow::on_gouraud_toggled ()
{
	if (getGouraudAction () -> get_active ())
	{
		toggleActions (getGouraudAction (), shadingActions);

		on_shading_changed ("GOURAUD");
	}
}

void
BrowserWindow::on_flat_toggled ()
{
	if (getFlatAction () -> get_active ())
	{
		toggleActions (getFlatAction (), shadingActions);
	
		on_shading_changed ("FLAT");
	}
}

void
BrowserWindow::on_wireframe_toggled ()
{
	if (getWireframeAction () -> get_active ())
	{
		toggleActions (getWireframeAction (), shadingActions);

		on_shading_changed ("WIREFRAME");
	}
}

void
BrowserWindow::on_pointset_toggled ()
{
	if (getPointsetAction () -> get_active ())
	{
		toggleActions (getPointsetAction (), shadingActions);

		on_shading_changed ("POINTSET");
	}
}

void
BrowserWindow::on_shading_changed (const std::string & value)
{
	if (changing)
		return;

	getCurrentBrowser () -> getBrowserOptions () -> Shading () .removeInterest (this, &BrowserWindow::set_shading);
	getCurrentBrowser () -> getBrowserOptions () -> Shading () .addInterest (this, &BrowserWindow::connectShading);

	getCurrentBrowser () -> getBrowserOptions () -> Shading () = value;
}

void
BrowserWindow::set_shading (const X3D::SFString & value)
{
	changing = true;

	if (value == "PHONG")
		getPhongAction () -> set_active (true);

	else if (value == "FLAT")
		getFlatAction () -> set_active (true);

	else if (value == "WIREFRAME")
		getWireframeAction () -> set_active (true);

	else if (value == "POINTSET")
		getPointsetAction () -> set_active (true);

	else
		getGouraudAction () -> set_active (true);

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
BrowserWindow::on_primitive_quality_high_toggled ()
{
	if (getPrimitiveQualityHighAction () -> get_active ())
	{
		toggleActions (getPrimitiveQualityHighAction (), primitiveQualityActions);

		on_primitive_quality_changed ("HIGH");
	}
}

void
BrowserWindow::on_primitive_quality_medium_toggled ()
{
	if (getPrimitiveQualityMediumAction () -> get_active ())
	{
		toggleActions (getPrimitiveQualityMediumAction (), primitiveQualityActions);

		on_primitive_quality_changed ("MEDIUM");
	}
}

void
BrowserWindow::on_primitive_quality_low_toggled ()
{
	if (getPrimitiveQualityLowAction () -> get_active ())
	{
		toggleActions (getPrimitiveQualityLowAction (), primitiveQualityActions);

		on_primitive_quality_changed ("LOW");
	}
}

void
BrowserWindow::on_primitive_quality_changed (const std::string & value)
{
	if (changing)
		return;

	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .removeInterest (this, &BrowserWindow::set_primitiveQuality);
	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .addInterest (this, &BrowserWindow::connectPrimitiveQuality);

	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () = value;
}

void
BrowserWindow::set_primitiveQuality (const X3D::SFString & value)
{
	changing = true;

	if (value == "HIGH")
		getPrimitiveQualityHighAction () -> set_active (true);

	else if (value == "LOW")
		getPrimitiveQualityLowAction () -> set_active (true);

	else
		getPrimitiveQualityMediumAction () -> set_active (true);

	changing = false;
}

void
BrowserWindow::connectPrimitiveQuality (const X3D::SFString & field)
{
	field .removeInterest (this, &BrowserWindow::connectPrimitiveQuality);
	field .addInterest (this, &BrowserWindow::set_primitiveQuality);
}

// Texture Quality

void
BrowserWindow::on_texture_quality_high_toggled ()
{
	if (getTextureQualityHighAction () -> get_active ())
	{
		toggleActions (getTextureQualityHighAction (), textureQualityActions);

		on_texture_quality_changed ("HIGH");
	}
}

void
BrowserWindow::on_texture_quality_medium_toggled ()
{
	if (getTextureQualityMediumAction () -> get_active ())
	{
		toggleActions (getTextureQualityMediumAction (), textureQualityActions);

		on_texture_quality_changed ("MEDIUM");
	}
}

void
BrowserWindow::on_texture_quality_low_toggled ()
{
	if (getTextureQualityLowAction () -> get_active ())
	{
		toggleActions (getTextureQualityLowAction (), textureQualityActions);

		on_texture_quality_changed ("LOW");
	}
}

void
BrowserWindow::on_texture_quality_changed (const std::string & value)
{
	if (changing)
		return;

	getCurrentBrowser () -> getBrowserOptions () -> TextureQuality () .removeInterest (this, &BrowserWindow::set_textureQuality);
	getCurrentBrowser () -> getBrowserOptions () -> TextureQuality () .addInterest (this, &BrowserWindow::connectTextureQuality);

	getCurrentBrowser () -> getBrowserOptions () -> TextureQuality () = value;
}

void
BrowserWindow::set_textureQuality (const X3D::SFString & value)
{
	changing = true;

	if (value == "HIGH")
		getTextureQualityHighAction () -> set_active (true);

	else if (value == "LOW")
		getTextureQualityLowAction () -> set_active (true);

	else
		getTextureQualityMediumAction () -> set_active (true);

	changing = false;
}

void
BrowserWindow::connectTextureQuality (const X3D::SFString & field)
{
	field .removeInterest (this, &BrowserWindow::connectPrimitiveQuality);
	field .addInterest (this, &BrowserWindow::set_primitiveQuality);
}

// Object Icons

void
BrowserWindow::on_backgrounds_toggled ()
{
	if (changing)
		return;

	const bool hidden = not getBackgroundsAction () -> get_active ();

	X3D::traverse (getCurrentContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
	               {
	                  const auto background = dynamic_cast <X3D::X3DBackgroundNode*> (node .getValue ());

	                  if (background)
								background -> isHidden (hidden);

	                  return true;
						},
	               true,
	               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_fogs_toggled ()
{
	if (changing)
		return;

	const bool hidden = not getFogsAction () -> get_active ();

	X3D::traverse (getCurrentContext () -> getRootNodes (), [&hidden] (X3D::SFNode & node)
	               {
	                  const auto fog = dynamic_cast <X3D::X3DFogObject*> (node .getValue ());

	                  if (fog)
								fog -> isHidden (hidden);

	                  return true;
						},
	               true,
	               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

// Object Icons

void
BrowserWindow::on_lights_toggled ()
{
	if (changing)
		return;

	if (getLightsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::X3DLightNode*> (node .getValue ()))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
				            if (dynamic_cast <X3D::X3DLightNode*> (node .getValue ()))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_proximity_sensors_toggled ()
{
	if (changing)
		return;

	if (getProximitySensorsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (node -> isType (proximitySensors))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
		                  if (node -> isType (proximitySensors))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_sounds_toggled ()
{
	if (changing)
		return;

	if (getSoundsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::Sound*> (node .getValue ()))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::Sound*> (node .getValue ()))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_transform_sensors_toggled ()
{
	if (changing)
		return;

	if (getTransformSensorsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::TransformSensor*> (node .getValue ()))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::TransformSensor*> (node .getValue ()))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_visibility_sensors_toggled ()
{
	if (changing)
		return;

	if (getVisibilitySensorsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::VisibilitySensor*> (node .getValue ()))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::VisibilitySensor*> (node .getValue ()))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_viewpoints_toggled ()
{
	if (changing)
		return;

	if (getViewpointsAction () -> get_active ())
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::X3DViewpointNode*> (node .getValue ()))
									node -> addTool ();

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
	else
	{
		X3D::traverse (getCurrentContext () -> getRootNodes (), [&] (X3D::SFNode & node)
		               {
		                  if (dynamic_cast <X3D::X3DViewpointNode*> (node .getValue ()))
									node -> removeTool (true);

		                  return true;
							},
		               true,
		               X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
	}
}

void
BrowserWindow::on_hide_all_object_icons_activated ()
{
	if (getLightsAction () -> get_active ())
		getLightsAction () -> set_active (false);

	if (getProximitySensorsAction () -> get_active ())
		getProximitySensorsAction () -> set_active (false);

	if (getSoundsAction () -> get_active ())
		getSoundsAction () -> set_active (false);

	if (getTransformSensorsAction () -> get_active ())
		getTransformSensorsAction () -> set_active (false);

	if (getVisibilitySensorsAction () -> get_active ())
		getVisibilitySensorsAction () -> set_active (false);

	if (getViewpointsAction () -> get_active ())
		getViewpointsAction () -> set_active (false);
}

void
BrowserWindow::on_rubberband_toggled ()
{
	getConfig () -> setItem ("rubberBand", getRubberbandAction () -> get_active ());
	getCurrentBrowser () -> getBrowserOptions () -> RubberBand () = getRubberbandAction () -> get_active ();
}

// RenderingProperties

void
BrowserWindow::on_rendering_properties_toggled ()
{
	getConfig () -> setItem ("renderingProperties", getRenderingPropertiesAction () -> get_active ());
	getCurrentBrowser () -> getRenderingProperties () -> Enabled () = getRenderingPropertiesAction () -> get_active ();
}

// Fullscreen

void
BrowserWindow::on_fullscreen_activated ()
{
	getWindow () .fullscreen ();
}

void
BrowserWindow::on_unfullscreen_activated ()
{
	getWindow () .unfullscreen ();
}

void
BrowserWindow::set_fullscreen (const bool value)
{
	X3DBrowserWindow::set_fullscreen (value);

	if (value)
	{
		getFullScreenMenuItem ()          .set_visible (false);
		getUnFullScreenMenuItem ()        .set_visible (true);
		getBrowserFullScreenMenuItem ()   .set_visible (false);
		getBrowserUnFullScreenMenuItem () .set_visible (true);
	}
	else
	{
		getFullScreenMenuItem ()          .set_visible (true);
		getUnFullScreenMenuItem ()        .set_visible (false);
		getBrowserFullScreenMenuItem ()   .set_visible (true);
		getBrowserUnFullScreenMenuItem () .set_visible (false);
	}
}

// Selection menu

void
BrowserWindow::on_select_all_activated ()
{
	if (getGeometryEditor () -> on_select_all ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> ();

	getSelection () -> setChildren (getCurrentContext () -> getRootNodes (), undoStep);
}

void
BrowserWindow::on_deselect_all_activated ()
{
	if (getGeometryEditor () -> on_deselect_all ())
		return;

	const auto undoStep = std::make_shared <X3D::UndoStep> ();

	getSelection () -> clear (undoStep);
}

void
BrowserWindow::on_hide_selected_objects_activated ()
{
	auto selection = getSelection () -> getChildren ();

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
BrowserWindow::on_hide_unselected_objects_activated ()
{
	std::set <X3D::X3DShapeNode*> visibles;

	auto selection = getSelection () -> getChildren ();

	X3D::traverse (selection, [&visibles] (X3D::SFNode & node)
	               {
	                  const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);

	                  if (shape)
								visibles .emplace (shape);

	                  return true;
						},
	               true, X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	X3D::traverse (getCurrentContext () -> getRootNodes (), [&visibles] (X3D::SFNode & node)
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
BrowserWindow::on_show_selected_objects_activated ()
{
	auto selection = getSelection () -> getChildren ();

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
BrowserWindow::on_show_all_objects_activated ()
{
	X3D::traverse (getCurrentContext () -> getRootNodes (), [ ] (X3D::SFNode & node)
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
	getConfig () -> setItem ("selectLowest", getSelectLowestAction () -> get_active ());

	getSelection () -> setSelectLowest (getSelectLowestAction () -> get_active ());
}

void
BrowserWindow::on_follow_primary_selection_toggled ()
{
	getConfig () -> setItem ("followPrimarySelection", getFollowPrimarySelectionAction () -> get_active ());

	if (getFollowPrimarySelectionAction () -> get_active ())
		getSelection () -> getPickedTime () .addInterest (this, &BrowserWindow::set_touchTime);

	else
		getSelection () -> getPickedTime () .removeInterest (this, &BrowserWindow::set_touchTime);
}

void
BrowserWindow::on_transform_tool_mode_toggled ()
{
	getCurrentBrowser () -> getTransformToolOptions () -> toolMode () = getTransformToolModeAction () -> get_active ();
}

// Geometry

void
BrowserWindow::on_union_activated ()
{
	on_boolean_activated (_ ("Boolean Operation »Union«"), X3D::Combine::geometryUnion, false);
}

void
BrowserWindow::on_difference_activated ()
{
	on_boolean_activated (_ ("Boolean Operation »Difference«"), X3D::Combine::geometryDifference, true);
}

void
BrowserWindow::on_intersection_activated ()
{
	on_boolean_activated (_ ("Boolean Operation »Intersection«"), X3D::Combine::geometryIntersection, false);
}

void
BrowserWindow::on_exclusion_activated ()
{
	on_boolean_activated (_ ("Boolean Operation »Exclusion«"), X3D::Combine::geometryExclusion, false);
}

void
BrowserWindow::on_combine_activated ()
{
	on_boolean_activated (_ ("Combine Geometries"), X3D::Combine::combineGeometry, false);
}

void
BrowserWindow::on_boolean_activated (const std::string & description, const BooleanOperation & booleanOperation, const bool front)
{
	try
	{
		const auto undoStep  = std::make_shared <X3D::UndoStep> (description);
		const auto selection = getSelection () -> getChildren ();
		const auto shapes    = X3DEditorObject::getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });
		const auto groups    = X3DEditorObject::getNodes <X3D::X3DGroupingNode> (selection, { X3D::X3DConstants::X3DGroupingNode });

		if (shapes .empty ())
			return;

		if (booleanOperation (getCurrentContext (), shapes, undoStep))
		{
			const auto & masterShape = front ? shapes .front () : shapes .back ();

			X3D::Combine::removeShapes (getCurrentContext (), selection, groups, shapes, masterShape, undoStep);

			// Select target

			getBrowserWindow () -> getSelection () -> setChildren ({ masterShape }, undoStep);
			getBrowserWindow () -> addUndoStep (undoStep);
		}
	}
	catch (const X3D::Error <X3D::INVALID_NODE> & error)
	{
	   __LOG__ << error .what () << std::endl;

		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));

		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't apply Boolean operation to geometries!"));
		dialog -> setText (_ ("The input geometries to Boolean operations must be »solid«, "
		                      "ie. closed (watertight) and non-self-intersecting. "
		                      "Additionally, each edge must share only two faces."));
		dialog -> run ();
	}
	catch (const std::exception & error)
	{
	   __LOG__ << error .what () << std::endl;
	}
}

void
BrowserWindow::on_transform_to_zero_activated ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Transform To Zero"));

	transformToZero (getSelection () -> getChildren (), undoStep);

	addUndoStep (undoStep);
}

// Layout

void
BrowserWindow::on_grid_layout_tool_toggled ()
{
	if (changing)
		return;

	changing = true;

	getAngleTool () -> isEnabled (false);
	getAngleTool () -> update ();
	getAngleLayoutToolAction () -> set_active (false);

	changing = false;

	// Toggle grid.

	getGridTool () -> isEnabled (getGridLayoutToolAction () -> get_active ());
	getGridTool () -> update ();
	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
BrowserWindow::on_angle_layout_tool_toggled ()
{
	if (changing)
		return;

	changing = true;

	getGridTool () -> isEnabled (false);
	getGridTool () -> update ();
	getGridLayoutToolAction () -> set_active (false);

	changing = false;

	// Toggle angle grid.

	getAngleTool () -> isEnabled (getAngleLayoutToolAction () -> get_active ());
	getAngleTool () -> update ();
	getBrowserWindow () -> isModified (getCurrentBrowser (), true);
}

void
BrowserWindow::on_grid_properties_activated ()
{
	addDialog ("GridEditor");
}

// Scenes menu

void
BrowserWindow::on_scenes_activated ()
{
	on_scenes_activated (getScenesMenu ());
}

void
BrowserWindow::on_browser_scenes_activated ()
{
	on_scenes_activated (getBrowserScenesMenu ());
}

void
BrowserWindow::on_scenes_activated (Gtk::Menu & menu)
{
	// Remove all menu items.

	for (auto & widget : menu .get_children ())
	   menu .remove (*widget);
	
	// Rebuild menu.

	size_t pageNumber = 0;

	for (const auto & browser : getBrowsers ())
	{
	   const auto & worldURL = getWorldURL (browser);
	   const bool   modified = isModified (browser);
		const auto   icon     = Gtk::manage (new Gtk::Image (Gtk::StockID (worldURL .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
		auto         menuItem = Gtk::manage (new Gtk::ImageMenuItem ());

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (getBrowserNotebook (), &Gtk::Notebook::set_current_page), pageNumber));

		menuItem -> set_image (*icon);
		menuItem -> set_label (worldURL .basename () + (modified ? "*" : ""));
		menuItem -> set_tooltip_text (worldURL .filename () .str ());
		menuItem -> set_always_show_image (true);

		if (browser == getCurrentBrowser ())
		   menuItem -> get_style_context () -> add_class ("titania-menu-item-selected");

		menu .append (*menuItem);

	   ++ pageNumber;
	}

	menu .show_all ();
}

// Help menu

void
BrowserWindow::on_info_activated ()
{
	X3DBrowserWidget::open (get_page ("about/info.x3dv"));
}

/// Toolbar

void
BrowserWindow::on_home ()
{
	load (getCurrentBrowser (), get_page ("about/home.x3dv"));
}

void
BrowserWindow::on_previous_page ()
{
	getUserData (getCurrentBrowser ()) -> browserHistory .previousPage ();
}

void
BrowserWindow::on_next_page ()
{
	getUserData (getCurrentBrowser ()) -> browserHistory .nextPage ();
}

bool
BrowserWindow::on_previous_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	auto & browserHistory = getUserData (getCurrentBrowser ()) -> browserHistory;

	if (browserHistory .isEmpty ())
		return false;

	for (const auto & widget : getHistoryMenu () .get_children ())
		getHistoryMenu () .remove (*widget);

	const auto index   = browserHistory .getIndex ();
	const auto current = browserHistory .getList () .begin () + index;
	auto       first   = browserHistory .getList () .begin () + std::max (index - 5, 0);
	const auto last    = browserHistory .getList () .begin () + std::min (index + 6, (int) browserHistory .getSize ());

	for (; first not_eq last; ++ first)
	{
		Gtk::MenuItem* menuItem = nullptr;
		const auto &   text     = first -> first;

		if (first == current)
		{
			const auto label         = Gtk::manage (new Gtk::Label ());
			const auto checkMenuItem = Gtk::manage (new Gtk::CheckMenuItem ());

			label -> set_markup ("<b>" + Glib::Markup::escape_text (text) + "</b>");
			label -> set_alignment (0, 0.5);
			checkMenuItem -> add (*label);
			checkMenuItem -> set_draw_as_radio (true);
			checkMenuItem -> set_active (true);

			menuItem = checkMenuItem;
		}
		else
		{
			const auto image         = Gtk::manage (new Gtk::Image (Gtk::StockID (first -> second .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
			const auto imageMenuItem = Gtk::manage (new Gtk::ImageMenuItem (text));

			imageMenuItem -> set_image (*image);
			imageMenuItem -> set_always_show_image (true);

			menuItem = imageMenuItem;
		}

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (browserHistory, &BrowserHistory::setIndex), first - browserHistory .getList () .begin ()));
		getHistoryMenu () .prepend (*menuItem);
	}

	getHistoryMenu () .show_all ();
	getHistoryMenu () .popup (event -> button, event -> time);
	return true;
}

bool
BrowserWindow::on_next_button_press_event (GdkEventButton* event)
{
	return on_previous_button_press_event (event);
}

bool
BrowserWindow::on_location_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
	{
		load (getCurrentBrowser (), Glib::uri_unescape_string (getLocationEntry () .get_text ()));
		return true;
	}

	return false;
}

void
BrowserWindow::on_location_icon_released (Gtk::EntryIconPosition icon_position, const GdkEventButton* event)
{
	switch (icon_position)
	{
		case Gtk::ENTRY_ICON_PRIMARY:
			break;
		case Gtk::ENTRY_ICON_SECONDARY:
			on_open_activated ();
			break;
	}
}

void
BrowserWindow::on_node_editor_clicked ()
{
	addDialog ("NodeEditor");
}

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
BrowserWindow::on_color_editor_clicked ()
{
	addDialog ("ColorEditor");
}

void
BrowserWindow::on_texture_mapping_editor_clicked ()
{
	addDialog ("TextureMappingEditor");
}

void
BrowserWindow::on_layer_editor_clicked ()
{
	addDialog ("LayerEditor");
}

void
BrowserWindow::on_background_editor_clicked ()
{
	addDialog ("BackgroundEditor");
}

void
BrowserWindow::on_navigation_info_editor_clicked ()
{
	addDialog ("NavigationInfoEditor");
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
BrowserWindow::on_precision_placement_panel_clicked ()
{
	addDialog ("PrecisionPlacementPanel");
}

void
BrowserWindow::on_prototype_instance_dialog_clicked ()
{
	addDialog ("PrototypeEditor");
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
	getDashboard () .set_visible (getEditorAction () -> get_active () or value);

	if (getHandButton () .get_active ())
		on_hand_button_toggled ();
	else
		on_arrow_button_toggled ();
}

void
BrowserWindow::on_hand_button_toggled ()
{
	if (getHandButton () .get_active ())
	{
		setViewer (viewer);
		set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());
	}

	const bool enabled = getHandButton () .get_active ();

	getSelection () -> isEnabled (not enabled);

	getPlayPauseButton ()       .set_visible (not enabled);
	getSelectSeparator ()       .set_visible (not enabled);
	getSelectParentButton ()    .set_visible (not enabled);
	getSelectChildrenButton ()  .set_visible (not enabled);
	getLookAtSelectionButton () .set_visible (isEditor () and not enabled);

}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		setViewer (viewer);
		set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());
	}

	getConfig () -> setItem ("arrow", getArrowButton () .get_active ());
}

void
BrowserWindow::set_arrow_button (const bool value)
{
}

void
BrowserWindow::on_play_pause_button_clicked ()
{
	isLive (not isLive ());
}

void
BrowserWindow::on_select_parent_button_clicked ()
{
	const auto selection = getSelection () -> getChildren ();

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

	getSelection () -> setChildren (parents);
	expandNodes (parents);
}

void
BrowserWindow::on_select_children_button_clicked ()
{
	const auto selection = getSelection () -> getChildren ();

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

	getSelection () -> setChildren (children);
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
						continue;

					if (not sharedNodes)
					{
						if (child -> getExecutionContext () not_eq parent -> getExecutionContext ())
							continue;
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
BrowserWindow::set_viewer ()
{
	const auto type = getCurrentBrowser () -> getCurrentViewer ();

	switch (type)
	{
		case X3D::X3DConstants::PlaneViewer:
		case X3D::X3DConstants::NoneViewer:
		{
			getStraightenButton ()        .set_visible (false);
			getStraightenHorizonButton () .set_visible (false);
			break;
		}
		case X3D::X3DConstants::WalkViewer:
		case X3D::X3DConstants::FlyViewer:
		{
			getStraightenButton ()        .set_visible (true);
			getStraightenHorizonButton () .set_visible (false);
			break;
		}
		default:
		{
			// ExamineViewer
			getStraightenButton ()        .set_visible (false);
			getStraightenHorizonButton () .set_visible (true);
			break;
		}
	}

	switch (type)
	{
		case X3D::X3DConstants::LookAtViewer:
		{
			//getHandButton ()  .set_sensitive (false);
			//getArrowButton () .set_sensitive (false);
			break;
		}

		case X3D::X3DConstants::RectangleSelection:
		case X3D::X3DConstants::LassoSelection:
		case X3D::X3DConstants::LightSaber:
		{
			getHandButton ()  .set_sensitive (true);
			getArrowButton () .set_sensitive (true);
			break;
		}

		default:
		{
			viewer = type;

			getHandButton ()  .set_sensitive (true);
			getArrowButton () .set_sensitive (true);

			if (getLookAtButton () .get_active ())
				getArrowButton ()  .set_active (true);

			break;
		}
	}

	switch (type)
	{
		case X3D::X3DConstants::NoneViewer:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("gtk-cancel"));
			break;
		}
		case X3D::X3DConstants::ExamineViewer:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("ExamineViewer"));
			break;
		}
		case X3D::X3DConstants::WalkViewer:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("WalkViewer"));
			break;
		}
		case X3D::X3DConstants::FlyViewer:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("FlyViewer"));
			break;
		}
		case X3D::X3DConstants::PlaneViewer:
		{
			getViewerButton () .set_stock_id (Gtk::StockID ("PlaneViewer"));
			break;
		}
		case X3D::X3DConstants::LookAtViewer:
		{
			getLookAtButton () .set_active (true);
			break;
		}
		default:
			break;
	}
}

void
BrowserWindow::set_available_viewers (const X3D::MFEnum <X3D::X3DConstants::NodeType> & availableViewers)
{
	const bool editor    = isEditor () and getArrowButton () .get_active ();
	const bool dashboard = getCurrentBrowser () -> getBrowserOptions () -> Dashboard ();

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
			case X3D::X3DConstants::ExamineViewer:
				examine = true;
				break;
			case X3D::X3DConstants::WalkViewer:
				walk = true;
				break;
			case X3D::X3DConstants::FlyViewer:
				fly = true;
				break;
			case X3D::X3DConstants::PlaneViewer:
				plane = true;
				break;
			case X3D::X3DConstants::NoneViewer:
				none = true;
				break;
			case X3D::X3DConstants::LookAtViewer:
				lookat = dashboard;
				break;
			default:
				break;
		}
	}

	getViewerSeparator ()       .set_visible (dashboard or editor);
	getViewerButton ()          .set_visible (dashboard or editor);
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
	setViewer (viewer);
}

void
BrowserWindow::on_examine_viewer_activated ()
{
	setViewer (X3D::X3DConstants::ExamineViewer);
}

void
BrowserWindow::on_walk_viewer_activated ()
{
	setViewer (X3D::X3DConstants::WalkViewer);
}

void
BrowserWindow::on_fly_viewer_activated ()
{
	setViewer (X3D::X3DConstants::FlyViewer);
}

void
BrowserWindow::on_plane_viewer_activated ()
{
	setViewer (X3D::X3DConstants::PlaneViewer);
}

void
BrowserWindow::on_none_viewer_activated ()
{
	setViewer (X3D::X3DConstants::NoneViewer);
}

void
BrowserWindow::on_straighten_clicked ()
{
	const auto & activeLayer = getCurrentBrowser () -> getActiveLayer ();

	if (activeLayer)
		activeLayer -> getViewpoint () -> straighten (getCurrentBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
}

void
BrowserWindow::on_straighten_horizon_toggled ()
{
	getCurrentBrowser () -> setStraightenHorizon (getStraightenHorizonButton () .get_active ());

	if (getStraightenHorizonButton () .get_active ())
		on_straighten_clicked ();
}

/*
 *  Look at
 */

void
BrowserWindow::on_look_at_selection_clicked ()
{
	if (not getCurrentBrowser () -> getActiveLayer ())
		return;

	const auto & selection = getSelection () -> getChildren ();

	if (selection .empty ())
		return;

	const auto activeViewpoint = getCurrentBrowser () -> getActiveLayer () -> getViewpoint ();

	X3D::Box3d bbox;

	for (const auto & node : selection)
	{
		const auto boundedObject = X3D::x3d_cast <X3D::X3DBoundedObject*> (node);

		if (boundedObject)
			bbox += boundedObject -> getBBox () * getModelViewMatrix (getCurrentContext (), node);

		const auto geometryNode = X3D::x3d_cast <X3D::X3DGeometryNode*> (node);

		if (geometryNode)
			bbox += geometryNode -> getBBox () * getModelViewMatrix (getCurrentContext (), node);
	}

	activeViewpoint -> lookAt (bbox);
}

void
BrowserWindow::on_look_at_all_clicked ()
{
	if (getCurrentBrowser () -> getActiveLayer ())
		getCurrentBrowser () -> getActiveLayer () -> lookAt ();
}

void
BrowserWindow::on_look_at_toggled ()
{
	if (getLookAtButton () .get_active ())
	{
		if (getCurrentBrowser () -> getCurrentViewer () not_eq X3D::X3DConstants::LookAtViewer)
			setViewer (X3D::X3DConstants::LookAtViewer);
	}
	else
	{
		if (getCurrentBrowser () -> getCurrentViewer () not_eq viewer)
			setViewer (viewer);
	}
}

void
BrowserWindow::toggleActions (const Glib::RefPtr <Gtk::ToggleAction> & current,
	                           const std::vector <Glib::RefPtr <Gtk::ToggleAction>> & actions)
{
	for (const auto & action : actions)
	{
		if (action not_eq current)
			action -> set_active (false);
	}
}

BrowserWindow::~BrowserWindow ()
{
	__LOG__ << std::endl;

	dispose ();

	__LOG__ << std::endl;
}

} // puck
} // titania
