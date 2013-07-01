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

#include "X3DBrowserWindow.h"

#include "../Configuration/config.h"
#include "../Browser/Image.h"

#include <Titania/gzstream.h>
#include <Titania/String.h>
#include <fstream>
#include <iostream>

namespace titania {
namespace puck {

X3DBrowserWindow::X3DBrowserWindow (const basic::uri & worldURL) :
	 X3DBrowserWindowUI (get_ui ("BrowserWindow.ui"), gconf_dir ())
{
	if (worldURL .size ())
		getConfig () .setItem ("worldURL", worldURL);

	// User interface

	setTransparent (true);

	// Add URNs to X3D::X3DUrlObject
	X3D::X3DUrlObject::addURN ("about:icon",      get_ui   ("icons/icon-bw.svg"));
	X3D::X3DUrlObject::addURN ("about:blank",     get_page ("about/blank.wrl"));
	X3D::X3DUrlObject::addURN ("about:date",      get_page ("about/date.wrl"));
	X3D::X3DUrlObject::addURN ("about:gears",     get_page ("about/gears.wrl"));
	X3D::X3DUrlObject::addURN ("about:home",      get_page ("about/home.wrl"));
	X3D::X3DUrlObject::addURN ("about:info",      get_page ("about/info.wrl"));
	X3D::X3DUrlObject::addURN ("about:url_error", get_page ("about/url_error.wrl"));
	X3D::X3DUrlObject::addURN ("about:hello_world", "http://titania.create3000.de/examples/hello_world.wrl");

	// Browser
	X3D::getBrowser () -> getBrowserOptions () -> splashScreen () = true;
	setBrowser (X3D::createBrowser ());
}

void
X3DBrowserWindow::initialize ()
{
	X3DBrowserWindowUI::initialize ();
	
	__LOG__ << std::endl;

	// Start with file from options or home page.

	// Console
	getBrowser () -> getConsole () -> string_changed () .addInterest (this, &X3DBrowserWindow::set_console);
	
	// UrlError
	getBrowser () -> urlError () .addInterest (this, &X3DBrowserWindow::set_urlError);

	// Initialized
	getBrowser () -> initialized .addInterest (this, &X3DBrowserWindow::set_initialized);

	// Insert Surface, this will initialize the Browser.
	getSurfaceBox () .pack_start (**getBrowser (), true, true, 0);

	// Show Surface and start the X3D Main Loop.
	getBrowser () -> show ();

	restoreSession ();
}

void
X3DBrowserWindow::set_initialized ()
{
	// Initialized
	getBrowser () -> initialized .removeInterest (this, &X3DBrowserWindow::set_initialized);
	getBrowser () -> initialized .addInterest (this, &X3DBrowserWindow::set_world);
	
	if (getConfig () .hasItem ("worldURL"))
		open (getConfig () .string ("worldURL") .raw ());
}

//void
//X3DBrowserWindow::parseOptions (int & argc, char** & argv)
//{
//	// Create and intialize option parser.
//
//	// Add Remaining options to option group.
//
//	Glib::OptionGroup mainGroup ("example_group", "description of example group", "help description of example group");
//
//	Glib::OptionEntry remaining;
//
//	remaining .set_long_name (G_OPTION_REMAINING);
//	remaining .set_arg_description (G_OPTION_REMAINING);
//
//	mainGroup .add_entry (remaining, remainingOptions);
//
//	// Intialize OptionContext.
//
//	Glib::OptionContext optionContext;
//
//	optionContext .set_main_group (mainGroup);
//
//	// Parse options.
//
//	try
//	{
//		optionContext .parse (argc, argv);
//	}
//	catch (const Glib::Error & error)
//	{
//		std::clog << "Exception: " << error .what () << std::endl;
//	}
//}

void
X3DBrowserWindow::restoreSession ()
{
	// Restore Menu Configuration
	// from Config

	// ToolBar
	if (getConfig () .hasItem ("toolBar"))
		getToolBarMenuItem () .set_active (getConfig () .boolean ("toolBar"));

	// SideBar
	if (getConfig () .hasItem ("sideBar"))
		getSideBarMenuItem () .set_active (getConfig () .boolean ("sideBar"));

	// Footer
	if (getConfig () .hasItem ("footer"))
		getFooterMenuItem () .set_active (getConfig () .boolean ("footer"));

	// Shading
	if (getConfig () .string ("shading") == "PHONG")
		getPhongMenuItem () .activate ();
		
	else if (getConfig () .string ("shading") == "GOURAUD")
		getGouraudMenuItem () .activate ();
		
	else if (getConfig () .string ("shading") == "FLAT")
		getFlatMenuItem () .activate ();
		
	else if (getConfig () .string ("shading") == "WIREFRAME")
		getWireFrameMenuItem () .activate ();
		
	else if (getConfig () .string ("shading") == "POINTSET")
		getPointSetMenuItem () .activate ();

	// PrimitiveQuality
	if (getConfig () .string ("primitiveQuality") == "HIGH")
		getHighQualityMenuItem () .activate ();
		
	else if (getConfig () .string ("primitiveQuality") == "MEDIUM")
		getMediumQualityMenuItem () .activate ();
		
	else if (getConfig () .string ("primitiveQuality") == "LOW")
		getLowQualityMenuItem () .activate ();

	// RenderingProperties
	if (getConfig () .hasItem ("renderingProperties"))
		getRenderingPropertiesMenuItem () .set_active (getConfig () .boolean ("renderingProperties"));

	// Rubberband
	if (getConfig () .hasItem ("rubberBand"))
		getRubberbandMenuItem () .set_active (getConfig () .boolean ("rubberBand"));

	// EnableInlineViewpoints
	if (getConfig () .hasItem ("enableInlineViewpoints"))
		getEnableInlineViewpointsMenuItem () .set_active (getConfig () .boolean ("enableInlineViewpoints"));

	// VPaned
	if (getConfig () .hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () .integer ("vPaned"));

	// HPaned
	if (getConfig () .hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () .integer ("hPaned"));

	getSideBarNotebook () .set_current_page (getConfig () .integer ("sidebarCurrentPage"));
	getFooterNotebook ()  .set_current_page (getConfig () .integer ("footerCurrentPage"));
}

void
X3DBrowserWindow::saveSession ()
{
	getConfig () .setItem ("toolBar", getToolBarMenuItem () .get_active ());
	getConfig () .setItem ("sideBar", getSideBarMenuItem () .get_active ());
	getConfig () .setItem ("footer",  getFooterMenuItem  () .get_active ());

	getConfig () .setItem ("shading",          getBrowser () -> getBrowserOptions () -> shading ());
	getConfig () .setItem ("primitiveQuality", getBrowser () -> getBrowserOptions () -> primitiveQuality ());
	getConfig () .setItem ("renderingProperties", getRenderingPropertiesMenuItem () .get_active ());

	getConfig () .setItem ("rubberBand",             getRubberbandMenuItem ()             .get_active ());
	getConfig () .setItem ("enableInlineViewpoints", getEnableInlineViewpointsMenuItem () .get_active ());

	getConfig () .setItem ("vPaned", getVPaned () .get_position ());
	getConfig () .setItem ("hPaned", getHPaned () .get_position ());

	getConfig () .setItem ("sidebarCurrentPage", getSideBarNotebook () .get_current_page ());
	getConfig () .setItem ("footerCurrentPage",  getFooterNotebook  () .get_current_page ());

	if (getBrowser () -> getExecutionContext () -> getWorldURL () .size ())
		getConfig () .setItem ("worldURL", getBrowser () -> getExecutionContext () -> getWorldURL ());
}

void
X3DBrowserWindow::blank ()
{
	if (not getBrowser () -> makeCurrent ())
		return;

	getBrowser () -> replaceWorld (getBrowser () -> createScene ());
}

void
X3DBrowserWindow::open ()
{
	const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	if (worldURL .length () and worldURL .is_local ())
		getFileOpenDialog () .set_current_folder_uri (worldURL .base () .str ());

	getFileOpenDialog () .present ();
}

void
X3DBrowserWindow::open (const basic::uri & worldURL)
{
	try
	{
		loadTime = chrono::now ();

		getBrowser () -> loadURL ({ worldURL .str () });
	}
	catch (...)
	{ }
}

void
X3DBrowserWindow::save (const basic::uri & worldURL)
{
	if (getSaveCompressedButton () .get_active ())
	{
		ogzstream file (worldURL .path ());
		file << X3D::CleanStyle << getBrowser () -> getExecutionContext () << std::flush;
		file .close ();
	}
	else
	{
		std::ofstream file (worldURL .path ());
		file << X3D::CompactStyle << getBrowser () -> getExecutionContext () << std::flush;
		file .close ();
	}

	//update_location ();
}

void
X3DBrowserWindow::reload ()
{
	open (getBrowser () -> getExecutionContext () -> getWorldURL ());
}

bool
X3DBrowserWindow::close ()
{
	X3DBrowserWindowUI::close ();
	
	return false;
}

void
X3DBrowserWindow::set_world ()
{
	loadTime = chrono::now () - loadTime;

	timeout .disconnect ();
	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DBrowserWindow::statistics), 10 * 1000);

	loadIcon ();
}

bool
X3DBrowserWindow::statistics ()
{
	std::string title = getBrowser () -> getExecutionContext () -> getWorldURL ();
	
	try
	{
		title = getBrowser () -> getExecutionContext () -> getMetaData ("title");
		std::clog << "Statistics for: " << title << std::endl;
	}
	catch (...)
	{ }

	std::clog << "Load Time: " << loadTime << std::endl;
	std::clog << "FPS: " << getBrowser () -> getRenderingProperties () -> fps () << std::endl;
	
	return false;
}

void
X3DBrowserWindow::set_console ()
{
	auto buffer = getConsole () .get_buffer ();

	for (const auto & string : getBrowser () -> getConsole () -> string_changed ())
		buffer -> insert (buffer -> end (), string .getValue ());

	buffer -> move_mark (buffer -> get_insert (), buffer -> end ());

	getConsole () .scroll_to (buffer -> get_insert ());
}

void
X3DBrowserWindow::set_urlError ()
{
	if (getBrowser () -> urlError () .empty ())
		return;

	getMessageDialog () .set_message ("Invalid X3D");
	getMessageDialog () .set_secondary_text ("<span font_desc=\"mono\">"
	                                         + Glib::Markup::escape_text (basic::join (getBrowser () -> urlError (), "\n"))
	                                         + "</span>",
	                                         true);
	getMessageDialog () .show ();
}

void
X3DBrowserWindow::loadIcon ()
{
	const auto & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();

	Gtk::StockID stockId = Gtk::StockID (worldURL .str ());

	Glib::RefPtr <Gtk::IconSet> iconSet;

	try
	{
		// Create Stock Icon

		basic::uri uri;

		try
		{
			uri = getBrowser () -> getExecutionContext () -> getMetaData ("icon");
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			uri = "/favicon.ico";
		}

		titania::Image icon (getBrowser () -> loadDocument (uri));

		iconSet = Gtk::IconSet::create (Gdk::Pixbuf::create_from_data (icon .getData (),
		                                                               Gdk::COLORSPACE_RGB,
		                                                               icon .getTransparency (),
		                                                               sizeof (Image::value_type) * 8,
		                                                               icon .getWidth (),
		                                                               icon .getHeight (),
		                                                               icon .getWidth () * icon .getComponents ()) -> copy ());
	}
	catch (const std::exception & error)
	{
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));
	}

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, worldURL .str ()));
}

void
X3DBrowserWindow::setTransparent (bool value)
{
	if (value)
	{
		Glib::RefPtr <Gdk::Visual> visual = getWindow () .get_screen () -> get_rgba_visual ();

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
}

X3DBrowserWindow::~X3DBrowserWindow ()
{
	// Console
	getBrowser () -> getConsole () -> string_changed () .removeInterest (this, &X3DBrowserWindow::set_console);
	
	// UrlError
	getBrowser () -> urlError () .removeInterest (this, &X3DBrowserWindow::set_urlError);

	// Initialized
	getBrowser () -> initialized .removeInterest (this, &X3DBrowserWindow::set_world);
}

} // puck
} // titania
