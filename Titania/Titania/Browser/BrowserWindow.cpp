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
#include <Titania/String/Trim.h>

namespace titania {
namespace puck {

BrowserWindow::BrowserWindow (const basic::uri & worldURL) :
	X3DBaseInterface (this),
	X3DBrowserWindow (worldURL),
	motionBlurEditor (this),
	   viewpointList (this),
	   historyEditor (this),
	   outlineEditor (this),
	            keys ()
{ }

void
BrowserWindow::initialize ()
{
	X3DBrowserWindow::initialize ();

	// User interface

	Gtk::Settings::get_default () -> property_gtk_menu_images ()       = true;
	Gtk::Settings::get_default () -> property_gtk_button_images ()     = true;
	Gtk::Settings::get_default () -> property_gtk_toolbar_style ()     = Gtk::TOOLBAR_ICONS;
	Gtk::Settings::get_default () -> property_gtk_toolbar_icon_size () = Gtk::ICON_SIZE_SMALL_TOOLBAR;

	// FileOpenDialog
	getFileOpenDialog () .set_default_response (Gtk::RESPONSE_OK);
	getFileOpenDialog () .add_button ("gtk-cancel", Gtk::RESPONSE_CANCEL);
	getFileOpenDialog () .add_button ("gtk-open", Gtk::RESPONSE_OK);

	getFileFilterX3D   () -> set_name ("X3D");
	getFileFilterImage () -> set_name ("Images");
	getFileFilterAudio () -> set_name ("Audio");
	getFileFilterVideo () -> set_name ("Videos");

	getFileOpenDialog () .add_filter (getFileFilterX3D ());
	getFileOpenDialog () .add_filter (getFileFilterImage ());
	getFileOpenDialog () .add_filter (getFileFilterAudio ());
	getFileOpenDialog () .add_filter (getFileFilterVideo ());

	getFileOpenDialog () .set_filter (getFileFilterX3D ());

	// OpenLocationDialog
	getOpenLocationDialog () .set_default_response (Gtk::RESPONSE_OK);
	getOpenLocationDialog () .add_button ("gtk-cancel", Gtk::RESPONSE_CANCEL);
	getOpenLocationDialog () .add_button ("gtk-open", Gtk::RESPONSE_OK);

	// FileSaveDialog
	getFileSaveDialog () .set_default_response (Gtk::RESPONSE_OK);
	getFileSaveDialog () .add_button ("gtk-cancel", Gtk::RESPONSE_CANCEL);
	getFileSaveDialog () .add_button ("gtk-save", Gtk::RESPONSE_OK);

	// MotionBlurEditor
	getMotionBlurEditor () .getWindow () .set_transient_for (getWindow ());

	// ViewpointList
	getViewpointList () .reparent (getViewpointListBox (), getWindow ());

	// HistoryEditor
	getHistoryEditor () .reparent (getHistoryEditorBox (), getWindow ());

	// OutlineEditor
	getOutlineEditor () .reparent (getOutlineEditorBox (), getWindow ());

	// CSS
	Glib::RefPtr <Gtk::CssProvider> cssProvider = Gtk::CssProvider::create ();
	cssProvider -> load_from_path (get_ui ("style.css"));
	Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// Drag & drop targets
	std::vector <Gtk::TargetEntry> targets = {
		Gtk::TargetEntry ("STRING"),
		Gtk::TargetEntry ("text/plain")
	};

	getSurfaceBox () .drag_dest_set (targets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

	// Dashboard
	getBrowser () -> getBrowserOptions () -> dashboard () .addInterest (this, &BrowserWindow::set_dashboard);
	getBrowser () -> getViewer ()        .addInterest (this, &BrowserWindow::set_viewer);
	getBrowser () -> getExamineViewer () .addInterest (this, &BrowserWindow::set_examine_viewer);
	getBrowser () -> getWalkViewer ()    .addInterest (this, &BrowserWindow::set_walk_viewer);
	getBrowser () -> getFlyViewer ()     .addInterest (this, &BrowserWindow::set_fly_viewer);
	getBrowser () -> getNoneViewer ()    .addInterest (this, &BrowserWindow::set_none_viewer);
	getBrowser () -> getLookAt ()        .addInterest (this, &BrowserWindow::set_look_at);
	getViewerButton () .set_menu (getViewerTypeMenu ());

	// Library
	buildLibrary ();

	// Window
	getWindow () .grab_focus ();
}

void
BrowserWindow::buildLibrary ()
{
	auto componentsMenuItem = Gtk::manage (new Gtk::MenuItem ("Components"));
	auto componentsMenu     = Gtk::manage (new Gtk::Menu ());
	componentsMenuItem -> set_submenu (*componentsMenu);

	std::map <std::string, Gtk::Menu*> componentsMenus;

	for (const auto & component : getBrowser () -> getSupportedComponents ())
	{
		auto menuItem = Gtk::manage (new Gtk::MenuItem (component -> getName ()));
		auto menu     = Gtk::manage (new Gtk::Menu ());
		menuItem -> set_submenu (*menu);

		componentsMenu -> append (*menuItem);
		componentsMenus .insert (std::make_pair (component -> getName (), menu));
	}

	for (const auto & node : getBrowser () -> getSupportedNodes ())
	{
		auto menuItem = Gtk::manage (new Gtk::MenuItem (node -> getTypeName ()));
		
		menuItem -> signal_activate () .connect (sigc::bind <std::string> (sigc::mem_fun (*this, &BrowserWindow::on_add_node), node -> getTypeName ()));
		
		componentsMenus [node -> getComponentName ()] -> append (*menuItem);
	}

	getLibraryMenu () .append (*componentsMenuItem);
	getLibraryMenu () .show_all ();
}

// File menu

void
BrowserWindow::on_new ()
{
	blank ();
}

void
BrowserWindow::on_open ()
{
	open ();
}

void
BrowserWindow::on_open_location_dialog ()
{
	Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

	if (clipboard -> wait_is_text_available ())
	{
		basic::uri uri (clipboard -> wait_for_text ());

		if (uri .is_network ())
			getOpenLocationEntry () .set_text (uri .str ());
	}

	getOpenLocationDialog () .set_response_sensitive (Gtk::RESPONSE_OK, getOpenLocationEntry () .get_text () .size ());
	getOpenLocationDialog () .present ();
}

void
BrowserWindow::on_save ()
{
	const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	if (get_save_as () or worldURL .empty () or worldURL .is_network ())
		on_save_as ();

	else
		save (worldURL);
}

void
BrowserWindow::on_save_as ()
{
	const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	if (worldURL .length () and worldURL .is_local ())
		getFileSaveDialog () .set_uri (worldURL .str ());

	getFileSaveDialog () .present ();
}

void
BrowserWindow::on_close ()
{
	close ();
}

void
BrowserWindow::on_revert_to_saved ()
{
	reload ();
}

// Dialog response handling

void
BrowserWindow::on_fileOpenDialog_response (int response_id)
{
	getFileOpenDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		open (Glib::uri_unescape_string (getFileOpenDialog () .get_uri ()));

	else
	{
		const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

		if (worldURL .is_local ())
			getFileOpenDialog () .set_current_folder_uri (worldURL .base () .str ());
	}
}

bool
BrowserWindow::on_openLocationEntry_key_release_event (GdkEventKey* event)
{
	getOpenLocationDialog () .set_response_sensitive (Gtk::RESPONSE_OK, getOpenLocationEntry () .get_text () .size ());

	if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
	{
		getOpenLocationDialog () .response (Gtk::RESPONSE_OK);
		return true;
	}

	return false;
}

void
BrowserWindow::on_openLocationDialog_response (int response_id)
{
	getOpenLocationDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		open (Glib::uri_unescape_string (getOpenLocationEntry () .get_text ()));
}

void
BrowserWindow::on_fileSaveDialog_response (int response_id)
{
	getFileSaveDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		save (Glib::uri_unescape_string (getFileSaveDialog () .get_filename ()));

	else
	{
		const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

		if (worldURL .is_local ())
			getFileOpenDialog () .set_current_folder_uri (worldURL .base () .str ());
	}
}

// View menu

void
BrowserWindow::on_toolBar_toggled ()
{
	toggleWidget (getToolBar (), getToolBarMenuItem () .get_active ());
}

void
BrowserWindow::on_sideBar_toggled ()
{
	toggleWidget (getSideBar (), getSideBarMenuItem () .get_active ());
}

void
BrowserWindow::on_footer_toggled ()
{
	toggleWidget (getFooter (), getFooterMenuItem () .get_active ());
}

// Shading menu

void
BrowserWindow::phong_activate ()
{
	if (not getPhongMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading () = "PHONG";
}

void
BrowserWindow::gouraud_activate ()
{
	if (not getGouraudMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading () = "GOURAUD";
}

void
BrowserWindow::flat_activate ()
{
	if (not getFlatMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading () = "FLAT";
}

void
BrowserWindow::wireframe_activate ()
{
	if (not getWireFrameMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading () = "WIREFRAME";
}

void
BrowserWindow::pointset_activate ()
{
	if (not getPointSetMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading () = "POINTSET";
}

// Primitive Quality

void
BrowserWindow::on_low_quality_activate ()
{
	if (not getLowQualityMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> primitiveQuality () = "LOW";
}

void
BrowserWindow::on_medium_quality_activate ()
{
	if (not getMediumQualityMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> primitiveQuality () = "MEDIUM";
}

void
BrowserWindow::on_high_quality_activate ()
{
	if (not getHighQualityMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> primitiveQuality () = "HIGH";
}

// RenderingProperties

void
BrowserWindow::on_rendering_properties_toggled ()
{
	getBrowser () -> getRenderingProperties () -> enabled () = getRenderingPropertiesMenuItem () .get_active ();
}

// Fullscreen

void
BrowserWindow::on_fullscreen_toggled ()
{
	if (getFullScreenMenuItem () .get_active ())
		getWindow () .fullscreen ();

	else
		getWindow () .unfullscreen ();
}

// Navigation menu

void
BrowserWindow::on_headlight_toggled ()
{
	auto navigationInfo = getBrowser () -> getActiveNavigationInfo ();

	if (navigationInfo)
		navigationInfo -> headlight () = getHeadlightMenuItem () .get_active ();
}

void
BrowserWindow::on_rubberband_toggled ()
{
	getBrowser () -> getBrowserOptions () -> rubberBand () = getRubberbandMenuItem () .get_active ();
}

void
BrowserWindow::on_look_at_all_activate ()
{
	if (getBrowser () -> getActiveLayer ())
		getBrowser () -> getActiveLayer () -> lookAt ();
}

void
BrowserWindow::on_enableInlineViewpoints_toggled ()
{
	getBrowser () -> getBrowserOptions () -> enableInlineViewpoints () = getEnableInlineViewpointsMenuItem () .get_active ();
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
	open (get_page ("about/info.wrl"));
}

void
BrowserWindow::on_standard_size ()
{
	getWindow () .resize (960, 600);
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

		//getBrowser () -> select = false;
	}
}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		std::clog << "Arrow button clicked." << std::endl;

		//getBrowser () -> select = true;
	}
}

// Viewer

void
BrowserWindow::set_viewer (X3D::ViewerType type)
{
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
BrowserWindow::set_none_viewer (bool value)
{
	getNoneViewerMenuItem () .set_visible (value);
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
BrowserWindow::on_none_viewer_activate ()
{
	getBrowser () -> setViewer (X3D::ViewerType::NONE);
}

// Look at

void
BrowserWindow::on_look_at_all_clicked ()
{
	if (getBrowser () -> getActiveLayer ())
		getBrowser () -> getActiveLayer () -> lookAt ();
}

void
BrowserWindow::set_look_at (bool value)
{
	getLookAtAllButton () .set_visible (value);
	getLookAtButton ()    .set_visible (value);
}

void
BrowserWindow::on_look_at_toggled ()
{
	__LOG__ << std::endl;
}

// Dialog response handling

void
BrowserWindow::on_messageDialog_response (int response_id)
{
	getMessageDialog () .hide ();
}

bool
BrowserWindow::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);

	return false;
}

bool
BrowserWindow::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	return false;
}

void
BrowserWindow::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                      int x, int y,
                                      const Gtk::SelectionData & selection_data,
                                      guint info,
                                      guint time)
{
	if (selection_data .get_format () == 8)
	{
		open (Glib::uri_unescape_string (basic::trim (selection_data .get_data_as_string ())));

		context -> drag_finish (true, false, time);
		return;
	}

	context -> drag_finish (false, false, time);
}

// Editing facilities

void
BrowserWindow::on_add_node (const std::string & typeName)
{
	auto scene = getBrowser () -> getExecutionContext ();
	auto node  = scene -> createNode (typeName);

	node -> setup ();
	scene -> getRootNodes () .emplace_back (node);
}

void
BrowserWindow::on_group_selected_nodes_activate ()
{
	__LOG__ << std::endl;
	
	auto scene     = getBrowser () -> getExecutionContext ();
	auto node      = scene -> createNode ("Transform");
	//auto transform = dynamic_cast <X3D::Transform*> (node .getValue ());
		
	node -> setup ();
	scene -> getRootNodes () .emplace_back (node);
}

void
BrowserWindow::on_add_to_group_activate ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
