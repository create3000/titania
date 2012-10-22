/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3DBrowserWidget.h"

#include "../Browser/ViewpointMenuItem.h"

#include <Titania/Bits/String.h>
#include <Titania/Chrono/Now.h>

#include <Titania/gzstream.h>
#include <fstream>

#include "Image.h"

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (const std::string & sessionKey, X3DBrowserInterface* const browserWidget) :
	X3DBrowserUserInterface (sessionKey, browserWidget), 
	               loadTime (0)
{ }

void
X3DBrowserWidget::initialize ()
{
	X3DBrowserUserInterface::initialize ();

	__LOG__ << this << std::endl;

	// Splash Screen.
	
	// Connect to map_event
	map_event = getDrawingArea () .signal_map_event () .connect (sigc::mem_fun (*this, &X3DBrowserWidget::on_map_event));

	// Insert DrawingArea, this will initialize the Browser.
	getDrawingAreaBox () .pack_start (getDrawingArea (), true, true, 0);

	// Show DrawingArea and start the X3D Main Loop.
	getDrawingArea () .show ();
}

bool
X3DBrowserWidget::on_map_event (GdkEventAny* event)
{
	__LOG__ << this << std::endl;
	
	map_event .disconnect ();

	// Register browser interests.
	getBrowser () -> addInterest (this, &X3DBrowserWidget::set_initialized);

	// Load Splash Screen.
	getBrowser () -> loadURL ({ "about:splash" });
	
	return false; // Propagate the event further. 
}

void
X3DBrowserWidget::set_initialized ()
{
	__LOG__ << this << std::endl;

	// The Splash Screen is loaded.

	update_location ();
	update_icon     ();

	getBrowser () -> removeInterest     (this, &X3DBrowserWidget::set_initialized);
	getBrowser () -> world .addInterest (this, &X3DBrowserWidget::set_world);

	loadURL ({ getConfig () .string ("worldURL") });
}

void
X3DBrowserWidget::blank ()
{
	if (not getDrawingArea () .gl ())
		return;

	getBrowser () -> replaceWorld (getBrowser () -> createScene ());
}

void
X3DBrowserWidget::home ()
{
	loadURL ({ "about:home" });
}

void
X3DBrowserWidget::setDescription (const std::string & value)
throw (X3D::Error <X3D::INVALID_OPERATION_TIMING>,
       X3D::Error <X3D::DISPOSED>)
{
	if (value .empty ())
		getWindow () .set_title ("Titania");

	else
		getWindow () .set_title (value + " ✩ " + "Titania");
}

const basic::uri &
X3DBrowserWidget::getWorldURL ()
{
	return getBrowser () -> getWorldURL ();
}

void
X3DBrowserWidget::loadURL (const X3D::MFString & url, const X3D::MFString & parameter)
throw (X3D::Error <X3D::INVALID_URL>,
       X3D::Error <X3D::URL_UNAVAILABLE>,
       X3D::Error <X3D::INVALID_X3D>)
{ }

void
X3DBrowserWidget::loadURL (const X3D::MFString & url)
throw (X3D::Error <X3D::INVALID_URL>,
       X3D::Error <X3D::URL_UNAVAILABLE>,
       X3D::Error <X3D::INVALID_X3D>)
{
	pushStatusBar ("Opening file " + url .toString () + ".");

	__LOG__ << url << std::endl;

	try
	{
		loadTime             = chrono::now <double> ();
		getBrowser () -> url = url;
	}
	catch (const X3D::X3DError & error)
	{
		getMessageDialog () .set_message ("Invalid X3D");
		getMessageDialog () .set_secondary_text ("<span font_desc=\"mono\">"
		                                         + std::string (error .what ())
		                                         + "\n"
		                                         + basic::join (getBrowser () -> URLError .cbegin (),
		                                                        getBrowser () -> URLError .cend (),
		                                                        "\n")
		                                         + "</span>",
		                                         true);
		getMessageDialog () .show ();
	}
}

void
X3DBrowserWidget::save (const basic::uri & worldURL)
{
	if (getSaveCompressedButton () .get_active ())
	{
		ogzstream file (worldURL);
		file << X3D::CleanStyle << getBrowser () -> world << std::flush;
		file .close ();
	}
	else
	{
		std::ofstream file (worldURL);
		file << X3D::CompactStyle << getBrowser () -> world << std::flush;
		file .close ();
	}

	//update_location ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// StatusBar

void
X3DBrowserWidget::pushStatusBar (const std::string & string)
{
	getStatusBar () .push (string);
}

void
X3DBrowserWidget::popStatusBar ()
{
	//	getStatusBar () .pop ();
}

// Statistics

void
X3DBrowserWidget::printStatistics () const
{
//	try
//	{
//		std::string name = getBrowser () -> getExecutionContext () -> getMetaData ("title");
//
//		std::cout << "Rendering statistics for: " << name << std::endl;
//
//		std::cout << "Load Time: " << loadTime << std::endl;
//
//		for (auto const & line : getDrawingArea () .statistics -> string)
//			std::cout << "  " << line .toString () << std::endl;
//	}
//	catch (const X3D::Error <X3D::INVALID_NAME> &)
//	{ }
}

// Callbacks

void
X3DBrowserWidget::set_world (/* X3D::SFNode <X3D::World> & world */)
{
	loadTime = chrono::now <double> () - loadTime;
	std::cout << "Load Time: " << loadTime << std::endl;

	saveSession ();

	// Update browser widget.

	update_location   ();
	update_icon       ();
	update_viewpoints ();

	setDescription (getWorldURL ());

	// Clear statusbar.

	popStatusBar ();
}

void
X3DBrowserWidget::update_location ()
{
	getLocationEntry () .set_text (getWorldURL () .str ());

	if (getWorldURL () .length () and getWorldURL () .is_local ())
	{
		getFileOpenDialog () .set_current_folder_uri (getWorldURL () .base () .str ());
		getFileSaveDialog () .set_current_folder_uri (getWorldURL () .base () .str ());
	}
}

void
X3DBrowserWidget::update_icon ()
{
	__LOG__ << getBrowser () -> getWorldURL () << std::endl;

	Gtk::StockID                stockId = Gtk::StockID (getBrowser () -> getWorldURL () .str ());
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
		__LOG__ << "Couldn't load icon: using blank icon: " << error .what () << std::endl;
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));
	}

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, getBrowser () -> getWorldURL () .str ()));

	getLocationEntry () .property_primary_icon_stock () .set_value (stockId);
}

void
X3DBrowserWidget::update_viewpoints (/* X3D::SFNode <X3D::World> & world */)
{
	Gtk::Menu* submenu = Gtk::manage (new Gtk::Menu ());

	for (auto const & viewpoint : getBrowser () -> getExecutionContext () -> getViewpoints ())
	{
		X3D::X3DViewpointNode* _viewpoint = dynamic_cast <X3D::X3DViewpointNode*> (viewpoint .getValue ());

		if (_viewpoint -> description .length ())
			submenu -> append (*Gtk::manage (new ViewpointMenuItem (viewpoint)));
	}

	submenu -> show_all ();

	getViewpointsMenuItem () .set_submenu (*submenu);
}

// Destructor

X3DBrowserWidget::~X3DBrowserWidget ()
{ }

} // puck
} // titania
