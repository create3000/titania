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
 ******************************************************************************/

#include "X3DBrowserWindow.h"

#include "../Configuration/config.h"
#include <Titania/X3D/Components/Networking/X3DUrlObject.h>

#include <iostream>

namespace titania {
namespace puck {

X3DBrowserWindow::X3DBrowserWindow (int & argc, char** & argv) :
	   Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN), 
	 X3DBrowserWindowUI (get_ui ("BrowserWindow.ui"), gconf_dir ()),                          
	X3DBrowserInterface (X3D::getBrowser ())                                                  
{
	setTransparent (true);

	parseOptions (argc, argv);

	// Add window to application at first. The order is important here.
	// add_window (getWindow ());

	// Add URNs to X3D::X3DUrlObject
	X3D::X3DUrlObject::addURN ("about:icon",        get_ui   ("icons/icon-bw.svg"));
	X3D::X3DUrlObject::addURN ("about:blank",       get_page ("about/blank.x3d"));
	X3D::X3DUrlObject::addURN ("about:home",        get_page ("about/home.x3d"));
	X3D::X3DUrlObject::addURN ("about:info",        get_page ("about/info.x3d"));
	X3D::X3DUrlObject::addURN ("about:splash",      get_page ("about/splash.x3d"));
	X3D::X3DUrlObject::addURN ("about:url_error",   get_page ("about/url_error.x3d"));
	X3D::X3DUrlObject::addURN ("about:hello_world", "http://titania.create3000.de/documentation/examples/hello_world.x3d");
}

// Application Handling

void
X3DBrowserWindow::run ()
{
	Gtk::Application::run (getWindow ());
}

void
X3DBrowserWindow::parseOptions (int & argc, char** & argv)
{
	// Create and intialize option parser.

	// Add Remaining options to option group.

	Glib::OptionGroup mainGroup ("example_group", "description of example group", "help description of example group");

	Glib::OptionEntry remaining;

	remaining .set_long_name (G_OPTION_REMAINING);
	remaining .set_arg_description (G_OPTION_REMAINING);

	mainGroup .add_entry (remaining, remainingOptions);

	// Intialize OptionContext.

	Glib::OptionContext optionContext;

	optionContext .set_main_group (mainGroup);

	// Parse options.

	try
	{
		optionContext .parse (argc, argv);
	}
	catch (const Glib::Error & error)
	{
		std::clog << "Exception: " << error .what () << std::endl;
	}
}

void
X3DBrowserWindow::initialize ()
{
	X3DBrowserWindowUI::initialize ();

	// AddTabLabel

	getAddTabButton () .set_image (*Gtk::manage (new Gtk::Image (Gtk::StockID ("gtk-add"),
	                                                             Gtk::IconSize (Gtk::ICON_SIZE_SMALL_TOOLBAR))));

	// Start with file from options or home page.

	if (remainingOptions .size ())
		loadURL ({ remainingOptions [0] });

	else
		restoreSession ();
}

void
X3DBrowserWindow::restoreSession ()
{
	Configuration sessions = getConfig () .getDirectory ("Sessions");

	// Start with home page when no sessions exists.

	if (not getConfig () .integer ("sessions"))
	{
		insertPage (0);
		getNotebook () .set_current_page (0);
		return;
	}

	// Restore session.

	size_t numSessions = sessions .getDirectories () .size ();

	for (size_t position = 0; position < numSessions; ++ position)
		insertPage (position);

	getNotebook () .set_current_page (getConfig () .integer ("currentPage"));
}

void
X3DBrowserWindow::close ()
{
	// Remove sessions.

	Configuration sessions    = getConfig () .getDirectory ("Sessions");
	size_t        numSessions = sessions .getDirectories () .size ();

	for (size_t i = browserWidgets .size (); i < numSessions; ++ i)
		sessions .getDirectory (std::to_string (i)) .remove ();

	// Save sessions.

	getConfig () .setItem ("sessions",    browserWidgets .size ());
	getConfig () .setItem ("currentPage", getNotebook () .get_current_page ());

	for (size_t i = 0; i < browserWidgets .size (); ++ i)
		browserWidgets [i] -> saveSession (getConfig () .getSession (i) .getKey ());

	quit ();
}

// X3DX3DBrowserWindowUserInterace

const std::shared_ptr <BrowserWidget> &
X3DBrowserWindow::getBrowserWidget () const
{
	return browserWidgets [getNotebook () .get_current_page ()];
}

// X3DBaseNode

const X3D::SFNode <X3D::X3DBrowser> &
X3DBrowserWindow::getBrowser () const
{
	return getBrowserWidget () -> getBrowser ();
}

void
X3DBrowserWindow::setDescription (const std::string & value)
throw (X3D::Error <X3D::INVALID_OPERATION_TIMING>,
       X3D::Error <X3D::DISPOSED>)
{
	Gtk::Widget* child = getNotebook () .get_children () [getNotebook () .get_current_page ()];

	Gtk::HBox*  tab_label = setTabLabel (*child);
	Gtk::Label* label     = new Gtk::Label (value);
	Gtk::Image* icon      = new Gtk::Image (Gtk::StockID (getExecutionContext () -> getWorldURL () .str ()),
	                                        Gtk::IconSize (Gtk::ICON_SIZE_SMALL_TOOLBAR));

	//	Gtk::IconTheme::get_default () -> load_icon (getExecutionContext () -> getWorldURL () .str (),
	//	                                             12,
	//	                                             ICON_LOOKUP_FORCE_SIZE);

	tab_label -> pack_start (*Gtk::manage (icon), false, true, 0);
	tab_label -> pack_start (*Gtk::manage (label), true, true, 0);
	tab_label -> set_spacing (4);
	tab_label -> show_all ();

	getNotebook () .set_tab_label (*child,
	                               *Gtk::manage (tab_label));

	if (value .empty ())
		getWindow () .set_title ("Titania");

	else
		getWindow () .set_title (value);
}

// X3DURLObject

void
X3DBrowserWindow::loadURL (const X3D::MFString & url, const X3D::MFString & parameter)
throw (X3D::Error <X3D::INVALID_URL>,
       X3D::Error <X3D::URL_UNAVAILABLE>,
       X3D::Error <X3D::INVALID_X3D>)
{
	size_t position = getNotebook () .get_n_pages () - 1;

	insertPage (position);
	browserWidgets [position] -> loadURL (url);

	getNotebook () .set_current_page (position);
}

void
X3DBrowserWindow::loadURL (const X3D::MFString & url)
throw (X3D::Error <X3D::INVALID_URL>,
       X3D::Error <X3D::URL_UNAVAILABLE>,
       X3D::Error <X3D::INVALID_X3D>)
{
	getBrowserWidget () -> loadURL (url);
}

void
X3DBrowserWindow::insertPage (size_t position)
{
	// Insert content.

	Gtk::HBox* box = new Gtk::HBox ();

	std::string sessionKey = getConfig () .getSession (browserWidgets .size ()) .getKey ();

	std::shared_ptr <BrowserWidget> browserWidget (new BrowserWidget (sessionKey, this));
	browserWidget -> reparent (*box, getWindow ());
	browserWidget -> getWidget () .show ();
	box -> show ();

	browserWidgets .insert (browserWidgets .begin () + position, browserWidget);

	getNotebook () .insert_page (*Gtk::manage (box), "", position);

	// Set tab label.

	setTabLabel (*box);
}

void
X3DBrowserWindow::removePage (Gtk::Widget & child)
{
	size_t position = getNotebook () .page_num (child);

	__LOG__ << browserWidgets .size () << std::endl;

	browserWidgets [position] -> dispose ();
	getNotebook () .remove_page (child);
	browserWidgets .erase (browserWidgets .begin () + position);

	__LOG__ << browserWidgets .size () << std::endl;
}

Gtk::HBox*
X3DBrowserWindow::setTabLabel (Gtk::Widget & child)
{
	Gtk::HBox* tab_label = new Gtk::HBox ();

	Gtk::Image*  closeImage = new Gtk::Image (Gtk::StockID ("gtk-close"), Gtk::IconSize (Gtk::ICON_SIZE_SMALL_TOOLBAR));
	Gtk::Button* close      = new Gtk::Button ();

	close -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (*this, &X3DBrowserWindow::on_close_tab), sigc::ref (child)));

	close     -> set_image (*Gtk::manage (closeImage));
	tab_label -> pack_end (*Gtk::manage (close), true, true, 0);
	tab_label -> get_style_context () -> add_class ("TabLabel");
	tab_label -> show_all ();

	getNotebook () .set_tab_label (child, *Gtk::manage (tab_label));

	return tab_label;
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

void
X3DBrowserWindow::dispose ()
{ }

} // puck
} // titania
