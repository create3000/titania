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

#include "BrowserWindow.h"

#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../Dialogs/FileSaveDialog/FileExportImageDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveDialog.h"
#include "../Dialogs/FileSaveDialog/FileSaveACopyDialog.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Dialogs/OpenLocationDialog/OpenLocationDialog.h"

#include "../Editors/ExternalToolsEditor/ExternalToolsEditor.h"
#include "../Editors/GridEditor/X3DGridTool.h"
#include "../Editors/NodeIndex/NodeIndex.h"
#include "../Editors/PrototypeEditor/PrototypeEditor.h"

#include "../Widgets/Footer/Footer.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"
#include "../Widgets/Sidebar/Sidebar.h"

#include "../Browser/BrowserSelection.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../Configuration/config.h"
#include "../Revealer/GeometryEditor/GeometryEditor.h"

#include <Titania/X3D/Browser/Core/BrowserOptions.h>
#include <Titania/X3D/Browser/Core/RenderingProperties.h>
#include <Titania/X3D/Browser/Navigation/X3DViewer.h>
#include <Titania/X3D/Browser/Tools/TransformToolOptions.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/LOD.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>
#include <Titania/X3D/Editing/Combine.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>
#include <Titania/X3D/Parser/Filter.h>
#include <Titania/X3D/Tools/Grids/X3DGridTool.h>
#include <Titania/X3D/Tools/SnapTool/SnapTargetTool.h>
#include <Titania/X3D/Tools/SnapTool/SnapSourceTool.h>
#include <Titania/X3D/Types/MatrixStack.h>

#include <Titania/String.h>

#include <regex>

namespace titania {
namespace puck {

BrowserWindow::BrowserWindow (const X3D::BrowserPtr & defaultBrowser) :
	         X3DBaseInterface (this, defaultBrowser),
	X3DBrowserWindowInterface ({ get_ui ("icons/IconFactory.glade"), get_ui ("BrowserWindow.glade") }),
	         X3DBrowserWindow (defaultBrowser),
	              cssProvider (Gtk::CssProvider::create ()),
	  primitiveQualityActions (),
	    textureQualityActions (),
	                     hand (true),
	                   viewer (X3D::X3DConstants::NoneViewer),
	                 changing (false)
{
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

	//if (getConfig () -> getItem <bool> ("transparent"))
	//	setTransparent (true);

	//if (not getConfig () -> hasItem ("maximized"))
	//	getWindow () .maximize ();

	getX_ITECompatibilityMenuItem () .set_use_underline (false);
	getBrowserX_ITECompatibilityMenuItem () .set_use_underline (false);

	// Load Styles.

	try
	{
		Glib::RefPtr <Gtk::CssProvider> fileCssProvider = Gtk::CssProvider::create ();

		fileCssProvider -> load_from_path (get_ui ("style.css"));

		Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), fileCssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider,     GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

		on_style_updated ();
	}
	catch (const Glib::Error & error)
	{
	   __LOG__ << error .what () << std::endl;
	}

	// Parse accel map.

	//Gtk::AccelMap::load (get_ui ("Accelerators.rc"));

	std::ifstream accelerators (get_ui ("Accelerators.rc"));
	std::string line (512, '\0');

	while (accelerators .getline (&line [0], line .size ()))
	{
		static const std::regex accelerator (R"(\(gtk_accel_path\s+\"(.*?)\"\s+\"(.*?)\"\))");

		std::smatch smatch;

		if (std::regex_search (line, smatch, accelerator))
		{
			const auto accelPath = smatch .str (1);
			const auto accelKey  = smatch .str (2);

			guint key;
			Gdk::ModifierType modifiers;

			Gtk::AccelGroup::parse (accelKey, key, modifiers);
			Gtk::AccelMap::add_entry (accelPath, key, modifiers);
		}
	}

	//Gtk::AccelMap::save ("/home/holger/Accelerators.rc");
	//Gtk::AccelMap::save (config_dir ("Accelerators.rc"));

	// Drag & drop targets

	std::vector <Gtk::TargetEntry> targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain"),
		Gtk::TargetEntry ("text/uri-list")
	};

	getToolbar ()         .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
	getBrowserNotebook () .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	// Debug

	#ifndef TITANIA_DEBUG
	getSeparatorMenuItem38 ()                            .set_visible (false);
	getActivateSnapTargetMenuItem ()                     .set_visible (false);
	getActivateSnapSourceMenuItem ()                     .set_visible (false);
	getCenterSnapTargetInSelectionMenuItem ()            .set_visible (false);
	getMoveSelectionToSnapTargetMenuItem ()              .set_visible (false);
	getMoveSelectionCenterToSnapTargetMenuItem ()        .set_visible (false);
	getBrowserSeparatorMenuItem18 ()                     .set_visible (false);
	getBrowserActivateSnapTargetMenuItem ()              .set_visible (false);
	getBrowserActivateSnapSourceMenuItem ()              .set_visible (false);
	getBrowserCenterSnapTargetInSelectionMenuItem ()     .set_visible (false);
	getBrowserMoveSelectionToSnapTargetMenuItem ()       .set_visible (false);
	getBrowserMoveSelectionCenterToSnapTargetMenuItem () .set_visible (false);
	#endif

	// Setup.

	setup ();
}

void
BrowserWindow::initialize ()
{
	X3DBrowserWindow::initialize ();

	getMasterBrowser () -> initialized () .addInterest (&BrowserWindow::set_masterBrowser, this);

	// Browser events

	getCurrentContext () .addInterest (&BrowserWindow::set_executionContext, this);
	getSelection () -> getHierarchy () .addInterest (&BrowserWindow::set_hierarchy, this);

	// Layout Menu

	getGridTool ()            -> getVisible () .addInterest (&BrowserWindow::set_grid_visible,             this);
	getAngleGridTool ()       -> getVisible () .addInterest (&BrowserWindow::set_angle_grid_visible,       this);
	getAxonometricGridTool () -> getVisible () .addInterest (&BrowserWindow::set_axonometric_grid_visible, this);

	// Window

	getWindow () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));
	getWidget () .grab_focus ();

	// External Tools menu

	ExternalToolsEditor::createMenu (this, getExternalToolsMenuItem ());
	ExternalToolsEditor::createMenu (this, getBrowserExternalToolsMenuItem ());
}

void
BrowserWindow::configure ()
{
	X3DBrowserWindow::configure ();

	const auto primitiveQuality = getConfig () -> getItem <std::string> ("primitiveQuality", "MEDIUM");

	if (primitiveQuality == "LOW")
		getPrimitiveQualityLowAction () -> set_active (true);
	else if (primitiveQuality == "MEDIUM")
		getPrimitiveQualityMediumAction () -> set_active (true);
	else if (primitiveQuality == "HIGH")
		getPrimitiveQualityHighAction () -> set_active (true);

	const auto textureQuality = getConfig () -> getItem <std::string> ("textureQuality", "MEDIUM");

	if (textureQuality == "LOW")
		getTextureQualityLowAction () -> set_active (true);
	else if (textureQuality == "MEDIUM")
		getTextureQualityMediumAction () -> set_active (true);
	else if (textureQuality == "HIGH")
		getTextureQualityHighAction () -> set_active (true);

	getTransformToolModeAction ()  -> set_active (getConfig () -> getItem <int32_t> ("transformToolMode", 0));
	getX_ITECompatibilityAction () -> set_active (getConfig () -> getItem <bool> ("cobwebCompatibility", true));

	if (getConfig () -> getItem <bool> ("hand", true))
		getHandButton () .set_active (true);
	else
		getArrowButton () .set_active (true);
}

void
BrowserWindow::setPage (const NotebookPagePtr & value)
{
	// Disconnect

	if (getCurrentPage ())
	{
		getCurrentBrowser () -> getViewer ()            .removeInterest (&BrowserWindow::set_viewer,             this);
		getCurrentBrowser () -> getActiveLayer ()       .removeInterest (&BrowserWindow::set_activeLayer,        this);
		getCurrentBrowser () -> getViewerType ()        .removeInterest (&BrowserWindow::set_viewer,             this);
		getCurrentBrowser () -> getPrivateViewer ()     .removeInterest (&BrowserWindow::set_viewer,             this);
		getCurrentBrowser () -> getAvailableViewers ()  .removeInterest (&BrowserWindow::set_available_viewers,  this);
		getCurrentBrowser () -> getStraightenHorizon () .removeInterest (&BrowserWindow::set_straighten_horizon, this);

		getCurrentBrowser () -> getSnapTarget () -> enabled () .removeInterest (&BrowserWindow::set_snapTarget, this);
		getCurrentBrowser () -> getSnapSource () -> enabled () .removeInterest (&BrowserWindow::set_snapSource, this);

		getCurrentBrowser () -> getBrowserOptions () -> Dashboard ()        .removeInterest (&BrowserWindow::set_dashboard,        this);
		getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .removeInterest (&BrowserWindow::set_primitiveQuality, this);
		getCurrentBrowser () -> getBrowserOptions () -> TextureQuality ()   .removeInterest (&BrowserWindow::set_textureQuality,   this);
	}

	// Set page.

	X3DBrowserWindow::setPage (value);

	// Connect.

	if (getCurrentPage ())
	{
		getCurrentBrowser () -> getActiveLayer ()       .addInterest (&BrowserWindow::set_activeLayer,        this);
		getCurrentBrowser () -> getViewerType ()        .addInterest (&BrowserWindow::set_viewer,             this);
		getCurrentBrowser () -> getPrivateViewer ()     .addInterest (&BrowserWindow::set_viewer,             this);
		getCurrentBrowser () -> getAvailableViewers ()  .addInterest (&BrowserWindow::set_available_viewers,  this);
		getCurrentBrowser () -> getStraightenHorizon () .addInterest (&BrowserWindow::set_straighten_horizon, this);

		getCurrentBrowser () -> getSnapTarget () -> enabled () .addInterest (&BrowserWindow::set_snapTarget, this);
		getCurrentBrowser () -> getSnapSource () -> enabled () .addInterest (&BrowserWindow::set_snapSource, this);
	
		getCurrentBrowser () -> getBrowserOptions () -> Dashboard ()        .addInterest (&BrowserWindow::set_dashboard,        this);
		getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () .addInterest (&BrowserWindow::set_primitiveQuality, this);
		getCurrentBrowser () -> getBrowserOptions () -> TextureQuality ()   .addInterest (&BrowserWindow::set_textureQuality,   this);

		// Initialize

		set_activeLayer ();
		set_viewer ();
		set_dashboard        (getCurrentBrowser () -> getBrowserOptions () -> Dashboard ());
		set_primitiveQuality (getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality ());
		set_textureQuality   (getCurrentBrowser () -> getBrowserOptions () -> TextureQuality ());
		set_snapTarget ();
		set_snapSource ();
		set_straighten_horizon ();
	
		getCurrentBrowser () -> getBrowserOptions () -> RubberBand ()   = getRubberbandAction () -> get_active ();
		getCurrentBrowser () -> getRenderingProperties () -> Enabled () = getRenderingPropertiesAction () -> get_active ();
	
		getCurrentBrowser () -> setFixedPipeline (not getX_ITECompatibilityAction () -> get_active ());

		on_transform_tool_mode_toggled ();
	}
}

void
BrowserWindow::set_masterBrowser ()
{
	getMasterBrowser () -> initialized () .removeInterest (&BrowserWindow::set_masterBrowser, this);

	getLogarithmicDepthBufferAction () -> set_active (getConfig () -> getItem <bool> ("logarithmicDepthBuffer"));
}

void
BrowserWindow::set_activeLayer ()
{
	changing = true;

	// Select Menu

	getSelectAllMenuItem () .set_sensitive (getCurrentBrowser () -> getActiveLayer ());

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

	getSaveButton ()            .set_sensitive (inScene);
	getImportButton ()          .set_sensitive (inScene);
	getPrototypeEditorButton () .set_sensitive (inScene);
}

// Selection

void
BrowserWindow::set_touchTime ()
{
	if (getSelection () -> getNodes () .empty ())
		return;

	expandNodes ({ getSelection () -> getNodes () .back () });
}

void
BrowserWindow::set_selection (const X3D::MFNode & selection)
{
	X3DBrowserWindow::set_selection (selection);

	const bool inScene          = not inPrototypeInstance ();
	const bool haveSelection    = inScene and selection .size ();
	const bool haveSelections   = inScene and selection .size () > 1;
	const bool executionContext = getSelectionContext (selection);

	// Window menu

	getCutMenuItem ()    .set_sensitive (haveSelection and executionContext);
	getCopyMenuItem ()   .set_sensitive (haveSelection and executionContext);
	getDeleteMenuItem () .set_sensitive (haveSelection and executionContext);

	getCloneMenuItem () .set_sensitive (haveSelection and executionContext);

	getGroupSelectedNodesMenuItem () .set_sensitive (haveSelection and executionContext);
	getUngroupMenuItem ()            .set_sensitive (haveSelection and executionContext);
	getAddToGroupMenuItem ()         .set_sensitive (haveSelections and executionContext);
	getDetachFromGroupMenuItem ()    .set_sensitive (haveSelection and executionContext);
	getCreateParentMenuItem ()       .set_sensitive (haveSelection and executionContext);

	getDeselectAllMenuItem ()           .set_sensitive (selection .size ());
	getHideSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);
	getHideUnselectedObjectsMenuItem () .set_sensitive (haveSelection);
	getShowSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);

	getGeometryMenuItem () .set_sensitive (haveSelection);

	getCenterSnapTargetInSelectionMenuItem () .set_sensitive (haveSelection and executionContext);

	// Browser menu

	getBrowserCutMenuItem ()    .set_sensitive (haveSelection and executionContext);
	getBrowserCopyMenuItem ()   .set_sensitive (haveSelection and executionContext);
	getBrowserDeleteMenuItem () .set_sensitive (haveSelection and executionContext);

	getBrowserCloneMenuItem () .set_sensitive (haveSelection and executionContext);

	getBrowserGroupSelectedNodesMenuItem () .set_sensitive (haveSelection and executionContext);
	getBrowserUngroupMenuItem ()            .set_sensitive (haveSelection and executionContext);
	getBrowserAddToGroupMenuItem ()         .set_sensitive (haveSelections and executionContext);
	getBrowserDetachFromGroupMenuItem ()    .set_sensitive (haveSelection and executionContext);
	getBrowserCreateParentMenuItem ()       .set_sensitive (haveSelection and executionContext);

	getBrowserDeselectAllMenuItem ()           .set_sensitive (selection .size ());
	getBrowserHideSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);
	getBrowserHideUnselectedObjectsMenuItem () .set_sensitive (haveSelection);
	getBrowserShowSelectedObjectsMenuItem ()   .set_sensitive (haveSelection);

	getBrowserGeometryMenuItem () .set_sensitive (haveSelection);

	getBrowserCenterSnapTargetInSelectionMenuItem () .set_sensitive (haveSelection and executionContext);

	// Dashboard

	getLookAtSelectionButton () .set_sensitive (haveSelection);

	// Snap Target

	set_snapTarget ();
}

// Style

void
BrowserWindow::on_style_updated ()
{
	try
	{
		const auto styleContext = getOutlineTreeView () -> get_style_context ();
		const auto fg_normal    = styleContext -> get_color (Gtk::STATE_FLAG_NORMAL);
		const auto fg_selected  = styleContext -> get_color (Gtk::STATE_FLAG_SELECTED);
		const auto bg_normal    = styleContext -> get_background_color (Gtk::STATE_FLAG_NORMAL);
		const auto bg_selected  = styleContext -> get_background_color (Gtk::STATE_FLAG_SELECTED);

		std::string string;

		string += ".titania-widget-box {\n";
		string += "  border-color: " + bg_normal .to_string () + ";\n";
		string += "}\n";
		string += ".titania-widget-box-selected {\n";
		string += "  border-color: " + bg_selected .to_string () + ";\n";
		string += "}\n";

		cssProvider -> load_from_data (string);

		//__LOG__ << string << std::endl;
	}
	catch (const Glib::Error & error)
	{
	   __LOG__ << error .what () << std::endl;
	}
}

// Keys

bool
BrowserWindow::on_focus_out_event (GdkEventFocus* event)
{
	getKeys () .clear ();

	if (getCurrentBrowser () -> on_external_focus_out_event (event))
		return false;

	return false;
}

bool
BrowserWindow::on_key_press_event (GdkEventKey* event)
{
	getKeys () .press (event);

	getSelection () -> setSelectMultiple (getKeys () .shift () and not getKeys () .control ());

	if (getCurrentBrowser () -> on_external_key_press_event (event))
		return false;

	if (not setAccelerators ())
		return false;

	// Nudge selection.

	const bool editMode = getConfig () -> getItem <bool> ("arrow");

	if (editMode)
	{
		if (not inPrototypeInstance () and not getSelection () -> getNodes () .empty ())
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

	getSelection () -> setSelectMultiple (getKeys () .shift () and not getKeys () .control ());

	if (getCurrentBrowser () -> on_external_key_release_event (event))
		return false;

	return false;
}

bool
BrowserWindow::on_menubar_button_press_event (GdkEventButton* event)
{
	setAccelerators (true);
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
	addDialog <FileOpenDialog> ("FileOpenDialog", false) -> loadURL ();
}

void
BrowserWindow::on_open_recent_activated ()
{
	openRecent ();
}

void
BrowserWindow::on_open_location_activated ()
{
	addDialog <OpenLocationDialog> ("OpenLocationDialog", false) -> run ();
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
	const auto undoStep = std::make_shared <X3D::UndoStep> ("Import Scene");
	const auto dialog = addDialog <FileOpenDialog> ("FileOpenDialog", false);

	dialog -> getWindow () .set_title (_ ("Import File ..."));
	dialog -> run ();

	import ({ dialog -> getUrl () }, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_browser_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                              int x, int y,
                                              const Gtk::SelectionData & selection_data,
                                              guint info,
                                              guint time)
{
	if (y < getCurrentBrowser () -> get_height ())
		on_drag_data_received (context, selection_data, time, false);

	else
		on_drag_data_received (context, selection_data, time, true);
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
				uris .emplace_back (Glib::uri_unescape_string (string));
		}

		if (selection_data .get_data_type () == "STRING")
		{
			auto strings = std::vector <std::string> ();

			basic::split (std::back_inserter (strings), basic::trim (selection_data .get_data_as_string ()), "\r\n");

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
				for (const auto & uri : uris)
					open (uri);
			}
			else
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import"));
				const auto nodes    = import (uris, undoStep);
	
				getSelection () -> setNodes (nodes, undoStep);
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
	const auto & worldURL = getCurrentScene () -> getWorldURL ();

	if (worldURL .empty () or worldURL .is_network () or not X3D::FileGenerator::getKnownFileTypes () .count (worldURL .suffix ()))
	{
		on_save_as_activated ();
	}
	else
	{
		save (worldURL, getOutputStyle (getCurrentScene ()), false);
	}
}

void
BrowserWindow::on_save_as_activated ()
{
	addDialog <FileSaveDialog> ("FileSaveDialog", false) -> save (false);
}

void
BrowserWindow::on_save_a_copy_activated ()
{
	addDialog <FileSaveACopyDialog> ("FileSaveACopyDialog", false) -> save (true);
}

void
BrowserWindow::on_export_activated ()
{
	addDialog <FileExportImageDialog> ("FileExportImageDialog", false) -> run ();
}

void
BrowserWindow::on_remove_unused_prototypes_activated ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Unused Prototypes"));

	X3D::X3DEditor::removeUnusedPrototypes (getCurrentContext (), undoStep);

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
	close (getCurrentPage ());
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
BrowserWindow::on_undo_history_activated ()
{
	addDialog ("UndoHistoryEditor");
}

void
BrowserWindow::on_cut_activated ()
{
	if (getGeometryEditor () -> on_cut ())
	   return;

	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Cut Nodes"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	getSelection () -> clearNodes (undoStep);

	cutNodes (executionContext, selection, undoStep);

	getSelection () -> undoRestoreNodes (undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_copy_activated ()
{
	if (getGeometryEditor () -> on_copy ())
		return;

	const auto selection        = getSelection () -> getNodes ();
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	if (selection .empty ())
		return;

	copyNodes (executionContext, selection);
}

void
BrowserWindow::on_paste_activated ()
{
	if (getGeometryEditor () -> on_paste ())
		return;

	auto selection = getSelection () -> getNodes ();

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Paste Nodes"));

	pasteNodes (getCurrentContext (), selection, undoStep);

	addUndoStep (undoStep);
}

// Edit menu

void
BrowserWindow::on_delete_activated ()
{
	if (getGeometryEditor () -> on_delete ())
		return;

	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	if (checkForClones (selection .cbegin (), selection .cend ()))
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Delete Node From Scene"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	getSelection () -> clearNodes (undoStep);

	X3D::X3DEditor::removeNodesFromScene (executionContext, selection, true, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_create_clone_activated ()
{
	auto selection = getSelection () -> getNodes ();

	if (selection .size () < 2)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Create Clone"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	const auto clone = selection .back ();
	selection .pop_back ();

	getSelection () -> clearNodes (undoStep);

	X3D::X3DEditor::createClone (executionContext, clone, selection, undoStep);

	getSelection () -> setNodes ({ clone }, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_unlink_clone_activated ()
{
	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Unlink Clone"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	auto nodes = X3D::X3DEditor::unlinkClone (executionContext, selection, undoStep);

	getSelection () -> setNodes (nodes, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_group_selected_nodes_activated ()
{
	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	if (checkForClones (selection .cbegin (), selection .cend ()))
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Group Selection"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));
	const auto group            = X3D::X3DEditor::groupNodes (executionContext, "Transform", selection, undoStep);

	getSelection () -> setNodes ({ group }, undoStep);
	addUndoStep (undoStep);

	expandNodes ({ group });
}

void
BrowserWindow::on_ungroup_activated ()
{
	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	if (checkForClones (selection .cbegin (), selection .cend ()))
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Ungroup Selection"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	getSelection () -> clearNodes (undoStep);

	const auto nodes = X3D::X3DEditor::ungroupNodes (executionContext, selection, undoStep);

	getSelection () -> setNodes (nodes, undoStep);

	addUndoStep (undoStep);
}

void
BrowserWindow::on_add_to_group_activated ()
{
	auto selection = getSelection () -> getNodes ();

	if (selection .size () < 2)
		return;

	if (checkForClones (selection .cbegin (), selection .cend () - 1))
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Add Selection To Group"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	getSelection () -> undoRestoreNodes (undoStep);

	const auto group = selection .back ();
	selection .pop_back ();

	if (X3D::X3DEditor::addToGroup (executionContext, group, selection, undoStep))
	{
		getSelection () -> setNodes (selection, undoStep);

		addUndoStep (undoStep);
	}
}

void
BrowserWindow::on_detach_from_group_activated ()
{
	const auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return;

	if (checkForClones (selection .cbegin (), selection .cend ()))
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Detach Selection From Group"));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));

	getSelection () -> undoRestoreNodes (undoStep);
	getSelection () -> redoRestoreNodes (undoStep);

	X3D::X3DEditor::detachFromGroup (executionContext, selection, getKeys () .shift (), undoStep);

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

X3D::SFNode
BrowserWindow::on_create_parent (const std::string & typeName, const std::string & fieldName)
{
	auto selection = getSelection () -> getNodes ();

	if (selection .empty ())
		return nullptr;

	if (checkForClones (selection .cbegin (), selection .cend ()))
		return nullptr;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create Parent Node »%s«"), typeName .c_str ()));
	const auto executionContext = X3D::MakePtr (getSelectionContext (selection));
	const auto group            = X3D::X3DEditor::createParentGroup (executionContext, typeName, fieldName, selection, undoStep);

	getSelection () -> setNodes ({ group }, undoStep);
	addUndoStep (undoStep);

	expandNodes ({ group });

	return group;
}

// View menu

void
BrowserWindow::on_menubar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem <bool> ("menubarFullscreen", getMenubarAction () -> get_active ());
	else
		getConfig () -> setItem <bool> ("menubar", getMenubarAction () -> get_active ());
	
	getMenubar () .set_visible (getMenubarAction () -> get_active ());
}

void
BrowserWindow::on_toolbar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem <bool> ("toolbarFullscreen", getToolbarAction () -> get_active ());
	else
		getConfig () -> setItem <bool> ("toolbar", getToolbarAction () -> get_active ());

	getToolbar () .set_visible (getToolbarAction () -> get_active ());
}

void
BrowserWindow::on_sidebar_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem <bool> ("sidebarFullscreen", getSidebarAction () -> get_active ());
	else
		getConfig () -> setItem <bool> ("sidebar", getSidebarAction () -> get_active ());
	
	getSidebarBox () .set_visible (getSidebarAction () -> get_active ());
}

void
BrowserWindow::on_footer_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem <bool> ("footerFullscreen", getFooterAction () -> get_active ());
	else
		getConfig () -> setItem <bool> ("footer", getFooterAction () -> get_active ());

	getFooterBox () .set_visible (getFooterAction () -> get_active ());
}

void
BrowserWindow::on_tabs_toggled ()
{
	if (isFullscreen ())
		getConfig () -> setItem <bool> ("tabsFullscreen", getTabsAction () -> get_active ());
	else
		getConfig () -> setItem <bool> ("tabs", getTabsAction () -> get_active ());

	getBrowserNotebook () .set_show_tabs (getShowTabs ());
}

void
BrowserWindow::on_motion_blur_activated ()
{
	addDialog ("MotionBlurEditor");
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
	getConfig () -> setItem <std::string> ("primitiveQuality", value);

	getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () = value;
}

void
BrowserWindow::set_primitiveQuality (const X3D::SFString & value)
{
	const auto primitiveQuality = getConfig () -> getItem <std::string> ("primitiveQuality", "MEDIUM");

	if (value != primitiveQuality)
		getCurrentBrowser () -> getBrowserOptions () -> PrimitiveQuality () = primitiveQuality;
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
	getConfig () -> setItem <std::string> ("textureQuality", value);

	getCurrentBrowser () -> getBrowserOptions () -> TextureQuality () = value;
}

void
BrowserWindow::set_textureQuality (const X3D::SFString & value)
{
	const auto textureQuality = getConfig () -> getItem <std::string> ("textureQuality");

	if (value != textureQuality)
		getCurrentBrowser () -> getBrowserOptions () -> TextureQuality () = textureQuality;
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
BrowserWindow::on_fullscreen (const bool value)
{
	X3DBrowserWindow::on_fullscreen (value);

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

	const auto & activeLayer = getCurrentBrowser () -> getActiveLayer ();

	if (activeLayer)
		getSelection () -> setNodes (activeLayer -> children ());
}

void
BrowserWindow::on_deselect_all_activated ()
{
	if (getGeometryEditor () -> on_deselect_all ())
		return;

	getSelection () -> clearNodes ();
}

void
BrowserWindow::on_hide_selected_objects_activated ()
{
	auto selection = getSelection () -> getNodes ();

	X3D::traverse (selection, [ ] (X3D::SFNode & node)
	{
		const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);
		
		if (shape)
			shape -> isHidden (true);
		
		return true;
	},
	X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	getSelection () -> clearNodes ();
}

void
BrowserWindow::on_hide_unselected_objects_activated ()
{
	std::set <X3D::X3DShapeNode*> visibles;

	auto selection = getSelection () -> getNodes ();

	X3D::traverse (selection, [&visibles] (X3D::SFNode & node)
	{
		const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);
		
		if (shape)
			visibles .emplace (shape);
		
		return true;
	},
	X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

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
	X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
}

void
BrowserWindow::on_show_selected_objects_activated ()
{
	auto selection = getSelection () -> getNodes ();

	X3D::traverse (selection, [ ] (X3D::SFNode & node)
	{
		const auto shape = X3D::x3d_cast <X3D::X3DShapeNode*> (node);
		
		if (shape)
			shape -> isHidden (false);
		
		return true;
	},
	X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
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
	X3D::TRAVERSE_INLINE_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);
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
		getSelection () -> getTouchTime () .addInterest (&BrowserWindow::set_touchTime, this);

	else
		getSelection () -> getTouchTime () .removeInterest (&BrowserWindow::set_touchTime, this);
}

void
BrowserWindow::on_transform_tool_mode_toggled ()
{
	getConfig () -> setItem <int32_t> ("transformToolMode", getTransformToolModeAction () -> get_active ());

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
		const auto selection = getSelection () -> getNodes ();
		const auto shapes    = X3DEditorObject::getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });
		const auto groups    = X3DEditorObject::getNodes <X3D::X3DGroupingNode> (selection, { X3D::X3DConstants::X3DGroupingNode });

		if (shapes .empty ())
			return;

		if (booleanOperation (getCurrentContext (), shapes, undoStep))
		{
			const auto & masterShape = front ? shapes .front () : shapes .back ();

			getBrowserWindow () -> getSelection () -> setNodes ({ masterShape }, undoStep);

			X3D::Combine::removeShapes (getCurrentContext (), selection, groups, shapes, masterShape, undoStep);

			// Select target

			getBrowserWindow () -> addUndoStep (undoStep);
		}
	}
	catch (const X3D::Error <X3D::INVALID_NODE> & error)
	{
	   __LOG__ << error .what () << std::endl;

		const auto dialog = createDialog <MessageDialog> ("MessageDialog");

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

	X3D::X3DEditor::transformToZero (getSelection () -> getNodes (), undoStep);

	addUndoStep (undoStep);
}

// Layout

void
BrowserWindow::on_browser_size_activate ()
{
	addDialog ("BrowserSizeEditor");
}

void
BrowserWindow::on_background_image_activate ()
{
	addDialog ("BackgroundImageEditor");
}

void
BrowserWindow::set_grid_visible ()
{
	changing = true;

	const auto visible = getGridTool () -> getVisible ();

	if (visible)
	{
		getAngleGridTool ()       -> setVisible (false);
		getAxonometricGridTool () -> setVisible (false);
	}

	getGridLayoutToolMenuItem ()        .set_active (visible);
	getBrowserGridLayoutToolMenuItem () .set_active (visible);

	changing = false;
}

void
BrowserWindow::set_angle_grid_visible ()
{
	changing = true;

	const auto visible = getAngleGridTool () -> getVisible ();

	if (visible)
	{
		getGridTool ()            -> setVisible (false);
		getAxonometricGridTool () -> setVisible (false);
	}

	getAngleLayoutToolMenuItem ()        .set_active (visible);
	getBrowserAngleLayoutToolMenuItem () .set_active (visible);

	changing = false;
}

void
BrowserWindow::set_axonometric_grid_visible ()
{
	changing = true;

	const auto visible = getAxonometricGridTool () -> getVisible ();

	if (visible)
	{
		getGridTool ()      -> setVisible (false);
		getAngleGridTool () -> setVisible (false);
	}

	getAxonometricGridLayoutToolMenuItem ()        .set_active (visible);
	getBrowserAxonometricGridLayoutToolMenuItem () .set_active (visible);

	changing = false;
}

void
BrowserWindow::on_grid_layout_tool_toggled ()
{
	if (changing)
		return;

	// Toggle grid.

	getGridTool () -> setVisible (getGridLayoutToolMenuItem () .get_active ());

	getCurrentPage () -> setModified (true);
}

void
BrowserWindow::on_angle_layout_tool_toggled ()
{
	if (changing)
		return;

	// Toggle angle grid.

	getAngleGridTool () -> setVisible (getAngleLayoutToolMenuItem () .get_active ());

	getCurrentPage () -> setModified (true);
}

void
BrowserWindow::on_axonometric_layout_tool_toggled ()
{
	if (changing)
		return;

	// Toggle axonometric grid.

	getAxonometricGridTool () -> setVisible (getAxonometricGridLayoutToolMenuItem () .get_active ());

	getCurrentPage () -> setModified (true);
}

void
BrowserWindow::on_grid_properties_activated ()
{
	addDialog ("GridEditor");
}

// Snap Target
	
void
BrowserWindow::set_snapTarget ()
{
	changing = true;

	const auto & selection        = getSelection () -> getNodes ();
	const bool   inScene          = not inPrototypeInstance ();
	const bool   haveSelection    = inScene and selection .size ();
	const bool   executionContext = getSelectionContext (selection);
	const auto & snapTarget       = getCurrentBrowser () -> getSnapTarget ();

	getActivateSnapTargetAction () -> set_active (snapTarget -> enabled ());

	getMoveSelectionToSnapTargetMenuItem ()       .set_sensitive (haveSelection and executionContext and snapTarget -> enabled ());
	getMoveSelectionCenterToSnapTargetMenuItem () .set_sensitive (haveSelection and executionContext and snapTarget -> enabled ());

	getBrowserMoveSelectionToSnapTargetMenuItem ()       .set_sensitive (haveSelection and executionContext and snapTarget -> enabled ());
	getBrowserMoveSelectionCenterToSnapTargetMenuItem () .set_sensitive (haveSelection and executionContext and snapTarget -> enabled ());

	changing = false;
}
	
void
BrowserWindow::set_snapSource ()
{
	changing = true;

	getActivateSnapSourceAction () -> set_active (getCurrentBrowser () -> getSnapSource () -> enabled ());

	changing = false;
}

void
BrowserWindow::on_activate_snap_target_toggled ()
{
	if (changing)
		return;

	getCurrentBrowser () -> getSnapTarget () -> enabled () = getActivateSnapTargetAction () -> get_active ();
}

void
BrowserWindow::on_activate_snap_source_toggled ()
{
	if (changing)
		return;

	getCurrentBrowser () -> getSnapSource () -> enabled () = getActivateSnapSourceAction () -> get_active ();
}

void
BrowserWindow::on_center_snap_target_in_selection ()
{
	if (getSelection () -> getSelectGeometry ())
		on_center_snap_target_in_selection (getSelection () -> getGeometries ());
	else
		on_center_snap_target_in_selection (getSelection () -> getNodes ());
}

void
BrowserWindow::on_center_snap_target_in_selection (const X3D::MFNode & selection)
{
	if (selection .empty ())
		return;

	const auto   executionContext = X3D::MakePtr (getSelectionContext (selection));
	const auto & snapTarget       = getCurrentBrowser () -> getSnapTarget ();
	const auto   bbox             = X3D::X3DEditor::getBoundingBox (executionContext, selection);

	// Place Snap Target.

	if (not snapTarget -> enabled ())
		snapTarget -> enabled () = true;

	snapTarget -> position () = bbox .center ();
}

void
BrowserWindow::on_move_selection_to_snap_target_activate ()
{
	if (getSelection () -> getSelectGeometry ())
	{
		const auto   undoStep  = std::make_shared <X3D::UndoStep> (_ ("Move Geometry Selection To Snap Target"));
		const auto & selection = getSelection () -> getGeometries ();

		on_move_selection_to_snap_target_activate (selection, undoStep);

		addUndoStep (undoStep);
	}
	else
	{
		const auto   undoStep  = std::make_shared <X3D::UndoStep> (_ ("Move Selection To Snap Target"));
		const auto & selection = getSelection () -> getNodes ();
	
		on_move_selection_to_snap_target_activate (selection, undoStep);
	
		addUndoStep (undoStep);
	}
}

void
BrowserWindow::on_move_selection_to_snap_target_activate (const X3D::MFNode & selection, const X3D::UndoStepPtr & undoStep)
{
	if (selection .empty ())
		return;

	const auto   executionContext = X3D::MakePtr (getSelectionContext (selection));
	const auto & snapTarget       = getCurrentBrowser () -> getSnapTarget ();
	const auto & snapSource       = getCurrentBrowser () -> getSnapSource ();
	const auto   targetPosition   = X3D::Vector3d (snapTarget -> position () .getValue ());
	const auto   targetNormal     = X3D::Vector3d (snapTarget -> normal () .getValue ());

	if (not snapTarget -> enabled ())
		return;

	if (snapSource -> enabled ())
	{
		const auto sourcePosition       = X3D::Vector3d (snapSource -> position () .getValue ());
		const auto sourceNormal         = X3D::Vector3d (snapSource -> normal () .getValue ());
		const auto transformationMatrix = X3D::X3DEditor::moveNodesCenterToTarget (executionContext, selection, targetPosition, targetNormal, sourcePosition, sourceNormal, false, undoStep);
		const auto destinationPosition  = X3D::SFVec3f (sourcePosition * transformationMatrix);
		const auto destinationNormal    = X3D::SFVec3f (snapTarget -> normal () .negate ());

		X3D::X3DEditor::setValue (snapSource, snapSource -> position (), destinationPosition, undoStep);
		X3D::X3DEditor::setValue (snapSource, snapSource -> normal (),   destinationNormal,   undoStep);
	}
	else
	{
		X3D::X3DEditor::moveNodesCenterToTarget (executionContext, selection, targetPosition, targetNormal, X3D::Vector3d (), X3D::Vector3d (), false, undoStep);
	}
}

void
BrowserWindow::on_move_selection_center_to_snap_target_activate ()
{
	if (getSelection () -> getSelectGeometry ())
	{
		const auto   undoStep  = std::make_shared <X3D::UndoStep> (_ ("Move Geometry Selection Center To Snap Target"));
		const auto & selection = getSelection () -> getGeometries ();

		on_move_selection_center_to_snap_target_activate (selection, undoStep);

		addUndoStep (undoStep);
	}
	else
	{
		const auto   undoStep  = std::make_shared <X3D::UndoStep> (_ ("Move Selection Center To Snap Target"));
		const auto & selection = getSelection () -> getNodes ();

		on_move_selection_center_to_snap_target_activate (selection, undoStep);

		addUndoStep (undoStep);
	}
}

void
BrowserWindow::on_move_selection_center_to_snap_target_activate (const X3D::MFNode & selection, const X3D::UndoStepPtr & undoStep)
{
	if (selection .empty ())
		return;

	const auto   executionContext = X3D::MakePtr (getSelectionContext (selection));
	const auto & snapTarget       = getCurrentBrowser () -> getSnapTarget ();
	const auto & snapSource       = getCurrentBrowser () -> getSnapSource ();
	const auto   targetPosition   = X3D::Vector3d (snapTarget -> position () .getValue ());
	const auto   targetNormal     = X3D::Vector3d (snapTarget -> normal () .getValue ());

	if (not snapTarget -> enabled ())
		return;

	if (snapSource -> enabled ())
	{
		const auto sourcePosition       = X3D::Vector3d (snapSource -> position () .getValue ());
		const auto sourceNormal         = X3D::Vector3d (snapSource -> normal () .getValue ());
		const auto transformationMatrix = X3D::X3DEditor::moveNodesCenterToTarget (executionContext, selection, targetPosition, targetNormal, sourcePosition, sourceNormal, true, undoStep);
		const auto destinationPosition  = X3D::SFVec3f (sourcePosition * transformationMatrix);
		const auto destinationNormal    = X3D::SFVec3f (snapTarget -> normal () .negate ());

		X3D::X3DEditor::setValue (snapSource, snapSource -> position (), destinationPosition, undoStep);
		X3D::X3DEditor::setValue (snapSource, snapSource -> normal (),   destinationNormal,   undoStep);
	}
	else
	{
		X3D::X3DEditor::moveNodesCenterToTarget (executionContext, selection, targetPosition, targetNormal, X3D::Vector3d (), X3D::Vector3d (), true, undoStep);
	}
}

// Extenal Tools menu

void
BrowserWindow::on_manage_external_tools_activate ()
{
	addDialog ("ExternalToolsEditor");
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

	for (const auto & page : getPages ())
	{
		const auto & browser  = page -> getMainBrowser ();
		const auto & worldURL = page -> getWorldURL ();
		const bool   modified = page -> getModified ();
		const auto   icon     = Gtk::manage (new Gtk::Image (Gtk::StockID (worldURL .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
		auto         menuItem = Gtk::manage (new Gtk::ImageMenuItem ());

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (getBrowserNotebook (), &Gtk::Notebook::set_current_page), pageNumber));

		menuItem -> set_image (*icon);
		menuItem -> set_always_show_image (true);
		menuItem -> set_label (worldURL .basename () + (modified ? "*" : ""));
		menuItem -> set_tooltip_text (worldURL .filename () .str ());

		if (browser == getCurrentBrowser ())
		   menuItem -> get_style_context () -> add_class ("titania-menu-item-selected");

		menu .append (*menuItem);

	   ++ pageNumber;
	}

	menu .show_all ();
}

// Help menu

void
BrowserWindow::on_cobweb_compatibility_toggled ()
{
	getConfig () -> setItem <bool> ("cobwebCompatibility", getX_ITECompatibilityAction () -> get_active ());

	getCurrentBrowser () -> setFixedPipeline (not getX_ITECompatibilityAction () -> get_active ());
}

void
BrowserWindow::on_logarithmic_depth_buffer_toggled ()
{
	getConfig () -> setItem <bool> ("logarithmicDepthBuffer", getLogarithmicDepthBufferAction () -> get_active ());

	getMasterBrowser () -> getBrowserOptions () -> LogarithmicDepthBuffer () = getLogarithmicDepthBufferAction () -> get_active ();
}

void
BrowserWindow::on_info_activated ()
{
	open (get_page ("about/info.x3dv"));
}

void
BrowserWindow::on_node_index_clicked ()
{
	getBrowserWindow () -> addDialog <NodeIndex> ("NodeIndex") -> setNamedNodes ();
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
BrowserWindow::on_prototype_editor_clicked ()
{
	addDialog ("PrototypeEditor");
}

void
BrowserWindow::on_node_editor_clicked ()
{
	addDialog ("NodeEditor");
}

// Primitive toolbar

void
BrowserWindow::on_arc_close_clicked ()
{
	on_primitive_clicked (_ ("Insert ArcClosed2D."), "Library/Primitives/Geometry2D/ArcClose2D.x3dv");
}

void
BrowserWindow::on_disk_clicked ()
{
	on_primitive_clicked (_ ("Insert Disk2D."), "Library/Primitives/Geometry2D/Disk2D.x3dv");
}

void
BrowserWindow::on_rectangle_clicked ()
{
	on_primitive_clicked (_ ("Insert Rectangle2D."), "Library/Primitives/Geometry2D/Rectangle2D.x3dv");
}

void
BrowserWindow::on_star_clicked ()
{
	on_primitive_clicked (_ ("Insert Star2D."), "Library/Primitives/Geometry2D/Star2D.x3dv");
}

void
BrowserWindow::on_box_clicked ()
{
	on_primitive_clicked (_ ("Insert Box."), "Library/Primitives/Geometry3D/Box.x3dv");
}

void
BrowserWindow::on_cone_clicked ()
{
	on_primitive_clicked (_ ("Insert Cone."), "Library/Primitives/Geometry3D/Cone.x3dv");
}

void
BrowserWindow::on_cylinder_clicked ()
{
	on_primitive_clicked (_ ("Insert Cylinder."), "Library/Primitives/Geometry3D/Cylinder.x3dv");
}

void
BrowserWindow::on_elevation_grid_clicked ()
{
	on_primitive_clicked (_ ("Insert ElevationGrid."), "Library/Primitives/Geometry3D/ElevationGrid.x3dv");
}

void
BrowserWindow::on_pyramid_clicked ()
{
	on_primitive_clicked (_ ("Insert Pyramid."), "Library/Primitives/Geometry3D/Pyramid.x3dv");
}

void
BrowserWindow::on_sphere_clicked ()
{
	on_primitive_clicked (_ ("Insert Sphere."), "Library/Primitives/Geometry3D/Sphere.x3dv");
}

void
BrowserWindow::on_text_clicked ()
{
	on_primitive_clicked (_ ("Insert Text."), "Library/Primitives/Text/Text.x3dv");
}

void
BrowserWindow::on_primitive_clicked (const std::string & description, const std::string & path)
{
	try
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (description);
		const auto nodes    = getBrowserWindow () -> import ({ find_data_file (path) }, undoStep);
	
		getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

// Browser dashboard handling

void
BrowserWindow::set_dashboard (const bool value)
{
	getDashboard () .set_visible (value);

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
		hand = true;

		getSelection () -> setEnabled (false);

		getCurrentBrowser () -> setViewerType (viewer);

		set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());
	}

	getConfig () -> setItem <bool> ("hand", getHandButton () .get_active ());
}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		hand = false;

		getSelection () -> setEnabled (true);

		getCurrentBrowser () -> setViewerType (viewer);

		set_available_viewers (getCurrentBrowser () -> getAvailableViewers ());
	}

	getConfig () -> setItem <bool> ("arrow", getArrowButton () .get_active ());
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
BrowserWindow::set_hierarchy ()
{
	const auto & parents  = getSelection () -> getParents ();
	const auto & children = getSelection () -> getChildren ();

	if (parents .empty ())
	{
		getSelectParentButton () .set_sensitive (false);
		getSelectParentButton () .set_tooltip_text (_("Selects the immediate parent of the current selection."));
	}
	else
	{
		const auto & parent = parents .back ();

		getSelectParentButton () .set_sensitive (true);

		if (parent -> getName () .empty ())
			getSelectParentButton () .set_tooltip_text (Glib::ustring::compose (_("Selects the immediate parent (%1) of the current selection."), parent -> getTypeName ()));
		else
			getSelectParentButton () .set_tooltip_text (Glib::ustring::compose (_("Selects the immediate parent (%1 »%2«) of the current selection."), parent -> getTypeName (), X3D::GetDisplayName (parent)));
	}

	if (children .empty ())
	{
		getSelectChildButton () .set_sensitive (false);
		getSelectChildButton () .set_tooltip_text (_("Selects the next lower child within the selected group in the hierarchy."));
	}
	else
	{
		const auto & child = children .back ();

		getSelectChildButton () .set_sensitive (true);

		if (child -> getName () .empty ())
			getSelectChildButton () .set_tooltip_text (Glib::ustring::compose (_("Selects the next lower child (%1) within the selected group in the hierarchy."), child -> getTypeName ()));
		else
			getSelectChildButton () .set_tooltip_text (Glib::ustring::compose (_("Selects the next lower child (%1 »%2«) within the selected group in the hierarchy."), child -> getTypeName (), X3D::GetDisplayName (child)));
	}
}

void
BrowserWindow::on_select_parent_button_clicked ()
{
	const auto parents = getSelection () -> getParents ();

	if (parents .empty ())
		return;

	getSelection () -> setNodes (parents);

	expandNodes (parents);
}

void
BrowserWindow::on_select_child_button_clicked ()
{
	const auto children = getSelection () -> getChildren ();

	if (children .empty ())
		return;

	getSelection () -> setNodes (children);

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

					const auto nodes = getChildrenInProtoinstance (X3D::SFNode (instance -> getRootNode ()), childIndex, seen);

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
			break;
		}
	}

	changing = true;

	getOtherViewerButton () .set_active (true);

	switch (type)
	{
		case X3D::X3DConstants::NoneViewer:
		{
			getSelectViewerButton () .set_stock_id (Gtk::StockID ("NoneViewer"));
			getNoneViewerButton () .set_active (true);
			break;
		}
		case X3D::X3DConstants::ExamineViewer:
		{
			getSelectViewerButton () .set_stock_id (Gtk::StockID ("ExamineViewer"));
			getExamineViewerButton () .set_active (true);
			break;
		}
		case X3D::X3DConstants::WalkViewer:
		{
			getSelectViewerButton () .set_stock_id (Gtk::StockID ("WalkViewer"));
			getWalkViewerButton () .set_active (true);
			break;
		}
		case X3D::X3DConstants::FlyViewer:
		{
			getSelectViewerButton () .set_stock_id (Gtk::StockID ("FlyViewer"));
			getFlyViewerButton () .set_active (true);
			break;
		}
		case X3D::X3DConstants::PlaneViewer:
		{
			getSelectViewerButton () .set_stock_id (Gtk::StockID ("PlaneViewer"));
			getPlaneViewerButton () .set_active (true);
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

	changing = false;
}

void
BrowserWindow::set_available_viewers (const X3D::MFEnum <X3D::X3DConstants::NodeType> & availableViewers)
{
	const bool editor    = getArrowButton () .get_active ();
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

	getViewerSeparator ()     .set_visible (dashboard or editor);
	getSelectViewerButton ()  .set_visible (dashboard or editor);
	getExamineViewerButton () .set_visible (examine);
	getWalkViewerButton ()    .set_visible (walk);
	getFlyViewerButton ()     .set_visible (fly);
	getPlaneViewerButton ()   .set_visible (plane);
	getNoneViewerButton ()    .set_visible (none);

	getLookAtSeparator () .set_visible (lookat);
	getLookAtAllButton () .set_visible (lookat);
	getLookAtButton ()    .set_visible (lookat);
}

void
BrowserWindow::on_select_viewer_clicked ()
{
	getSelectViewerPopover () .popup ();
}

void
BrowserWindow::on_examine_viewer_toggled ()
{
	if (getExamineViewerButton () .get_active ())
		on_viewer_toggled (X3D::X3DConstants::ExamineViewer);
}

void
BrowserWindow::on_walk_viewer_toggled ()
{
	if (getWalkViewerButton () .get_active ())
		on_viewer_toggled (X3D::X3DConstants::WalkViewer);
}

void
BrowserWindow::on_fly_viewer_toggled ()
{
	if (getFlyViewerButton () .get_active ())
		on_viewer_toggled (X3D::X3DConstants::FlyViewer);
}

void
BrowserWindow::on_plane_viewer_toggled ()
{
	if (getPlaneViewerButton () .get_active ())
		on_viewer_toggled (X3D::X3DConstants::PlaneViewer);
}

void
BrowserWindow::on_none_viewer_toggled ()
{
	if (getNoneViewerButton () .get_active ())
		on_viewer_toggled (X3D::X3DConstants::NoneViewer);
}

void
BrowserWindow::on_viewer_toggled (const X3D::X3DConstants::NodeType viewerType)
{
	getSelectViewerPopover () .popdown ();

	if (changing)
		return;

	if (hand)
		getHandButton () .set_active (true);
	else
		getArrowButton () .set_active (true);

	getCurrentBrowser () -> setViewerType (viewerType);
}

void
BrowserWindow::set_straighten_horizon ()
{
	changing = true;

	getStraightenHorizonButton () .set_active (getCurrentBrowser () -> getStraightenHorizon ());

	changing = false;
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
	getCurrentPage () -> lookAtSelection ();
}

void
BrowserWindow::on_look_at_all_clicked ()
{
	getCurrentPage () -> lookAtAll ();
}

void
BrowserWindow::on_look_at_toggled ()
{
	if (getLookAtButton () .get_active ())
	{
		if (getCurrentBrowser () -> getCurrentViewer () not_eq X3D::X3DConstants::LookAtViewer)
			getCurrentBrowser () -> setViewerType (X3D::X3DConstants::LookAtViewer);
	}
	else
	{
		if (getCurrentBrowser () -> getCurrentViewer () not_eq viewer)
			getCurrentBrowser () -> setViewerType (viewer);
	}
}

bool
BrowserWindow::checkForClones (const X3D::MFNode::const_iterator & first, const X3D::MFNode::const_iterator & last)
{
	static const X3D::NodeTypeSet metaDataType = { X3D::X3DConstants::X3DMetadataObject };

	const auto clones = std::count_if (first, last, [ ] (const X3D::SFNode & node)
	{
		const auto metaCloneCount = node -> isType (metaDataType) ? 0 : node -> getMetaCloneCount ();

		return node -> getCloneCount () - metaCloneCount > 1;
	});

	if (not clones)
		return false;

	const auto dialog = createDialog <MessageDialog> ("MessageDialog");

	dialog -> setType (Gtk::MESSAGE_QUESTION);
	dialog -> setMessage (_ ("This operation is not clone save, because it will affect all clones!"));
	dialog -> setText (_ ("You have selected one ore more clones. Use Outline Editor's context menu or drag & drop facility to have a clone safe operation. Proceed anyway?"));

	return dialog -> run () not_eq Gtk::RESPONSE_OK;
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
