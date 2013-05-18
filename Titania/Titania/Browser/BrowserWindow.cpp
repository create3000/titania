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

namespace titania {
namespace puck {

BrowserWindow::BrowserWindow (const basic::uri & worldURL) :
	X3DBrowserWindow (worldURL),    
	motionBlurEditor (getBrowser ()), 
	 viewpointEditor (getBrowser ()), 
	   historyEditor (getBrowser ()), 
	   outlineEditor (getBrowser ())  
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
	getFileOpenDialog () .add_button ("gtk-open", Gtk::RESPONSE_OK);
	getFileOpenDialog () .add_button ("gtk-cancel", Gtk::RESPONSE_CANCEL);

	// FileSaveDialog
	getFileSaveDialog () .set_default_response (Gtk::RESPONSE_OK);
	getFileSaveDialog () .add_button ("gtk-save", Gtk::RESPONSE_OK);
	getFileSaveDialog () .add_button ("gtk-cancel", Gtk::RESPONSE_CANCEL);

	// MotionBlurEditor
	getMotionBlurEditor () .getWindow () .set_transient_for (getWindow ());

	// ViewpointEditor
	getViewpointEditor () .reparent (getViewpointEditorBox (), getWindow ());

	// HistoryEditor
	getHistoryEditor () .reparent (getHistoryEditorBox (), getWindow ());

	// OutlineEditor
	getOutlineEditor () .reparent (getOutlineEditorBox (), getWindow ());
	
	// Console
	Pango::FontDescription font;
	font .set_family ("monospace");
	getConsole () .override_font (font);

	getWindow () .grab_focus ();
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
BrowserWindow::on_save ()
{
	const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	if (worldURL .empty () or worldURL .is_network ())
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
		getFileOpenDialog () .set_current_folder_uri (getBrowser () -> getExecutionContext () -> getWorldURL () .base () .str ());
}

void
BrowserWindow::on_fileSaveDialog_response (int response_id)
{
	getFileSaveDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		save (Glib::uri_unescape_string (getFileSaveDialog () .get_filename ()));

	else
		getFileSaveDialog () .set_current_folder_uri (getBrowser () -> getExecutionContext () -> getWorldURL () .base () .str ());
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

void
BrowserWindow::on_statusBar_toggled ()
{
	toggleWidget (getStatusBar (), getStatusBarMenuItem () .get_active ());
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
	const X3D::SFNode <X3D::NavigationInfo> & navigationInfo = getBrowser () -> getActiveNavigationInfo ();

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
	getBrowser () -> getExecutionContext () -> getActiveLayer () -> lookAt ();
}

void
BrowserWindow::on_enableInlineViewpoints_toggled ()
{
	getBrowser () -> getBrowserOptions () -> enableInlineViewpoints () = getEnableInlineViewpointsMenuItem () .get_active ();
}

// Editor handling

void
BrowserWindow::on_outline_editor_activate ()
{
	//getOutlineEditor () .getWindow () .present ();
}

void
BrowserWindow::on_viewpoint_editor_activate ()
{
	//getViewpointEditor () .getWindow () .present ();
}

void
BrowserWindow::on_motion_blur_editor_activate ()
{
	getMotionBlurEditor () .getWindow () .present ();
}

// Help menu

void
BrowserWindow::on_info ()
{
	open ("about:info");
}

void
BrowserWindow::on_standard_size ()
{
	getWindow () .resize (960, 600);
}

// Browser toolbar handling

void
BrowserWindow::on_hand_button_toggled ()
{
	if (getHandButton () .get_active ())
	{
		std::clog << "Hand button clicked." << std::endl;

		getBrowser () -> select = false;
	}
}

void
BrowserWindow::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		std::clog << "Arrow button clicked." << std::endl;

		getBrowser () -> select = true;
	}
}

void
BrowserWindow::on_look_at_all_clicked ()
{
	getBrowser () -> getExecutionContext () -> getActiveLayer () -> lookAt ();
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

} // puck
} // titania
