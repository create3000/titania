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

#include "X3DBrowserWidget.h"

#include "../Browser/AboutTab.h"
#include "../Browser/BrowserUserData.h"
#include "../Browser/Image.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"

#include <Titania/String.h>
#include <Titania/gzstream.h>
#include <fstream>

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (const X3D::BrowserPtr & masterBrowser) :
	X3DBrowserWindowInterface (),
	            masterBrowser (masterBrowser),
	                  browser (masterBrowser),
	                 browsers (),
	                    scene (browser -> getExecutionContext ()),
	         executionContext (scene),
	                 aboutTab (new AboutTab (getBrowserWindow ()))
{
	addChildren (browsers, browser, scene, executionContext);
}

void
X3DBrowserWidget::initialize ()
{
	X3DBrowserWindowInterface::initialize ();
	
	aboutTab -> initialize ();

	getBrowser () -> initialized () .addInterest (this, &X3DBrowserWidget::set_initialized);
	getBrowser () -> getBrowserOptions () -> splashScreen ()    = true;
	getBrowser () -> getBrowserOptions () -> splashScreenURL () = { get_ui ("BrowserWidget.x3dv") };
	getBrowser () -> show ();

	getSplashBox () .pack_start (*getBrowser (), true, true, 0);
}

void
X3DBrowserWidget::set_initialized ()
{
	// Restore browsers.

	std::set <basic::uri> urlIndex;

	for (const auto & browser : browsers)
		urlIndex .emplace (getUserData (browser) -> URL);

	const auto empty     = browsers .empty ();
	auto       worldURLs = basic::split (getConfig () .getString ("worldURL"), "\n");
	auto       histories = basic::split (getConfig () .getString ("history"), "\n");

	if (worldURLs .empty () and empty)
		worldURLs .emplace_back (get_page ("about/home.x3dv"));

	for (size_t i = 0; i < worldURLs .size (); ++ i)
	{
		if (urlIndex .count (worldURLs [i]))
			continue;

		const auto browser = X3D::createBrowser (getBrowser ());

		if (i < histories .size ())
			getUserData (browser) -> browserHistory .fromString (histories [i]);

		append (browser, worldURLs [i]);
	}

	if (empty)
	{
		const size_t pageNumber = getConfig () .getInteger ("currentPage"); // Use size_t!!!

		if (pageNumber < browsers .size ())
			getBrowserNotebook () .set_current_page (pageNumber);
	}
	else
		getBrowserNotebook () .set_current_page (0);

	// Show notebook.

	getSplashBox () .set_visible (false);
	getBrowserNotebook () .set_visible (true);

	getScene () .addInterest (this, &X3DBrowserWidget::set_scene);
}

void
X3DBrowserWidget::restoreSession ()
{
	X3DBrowserWindowInterface::restoreSession ();

	// Restore Menu Configuration from Config

	// ToolBar
	if (not getConfig () .hasItem ("toolBar"))
		getConfig () .setItem ("toolBar", true);

	getToolBarMenuItem () .set_active (getConfig () .getBoolean ("toolBar"));

	// SideBar
	if (getConfig () .hasItem ("sideBar"))
		getSideBarMenuItem () .set_active (getConfig () .getBoolean ("sideBar"));

	// Footer
	if (getConfig () .hasItem ("footer"))
		getFooterMenuItem () .set_active (getConfig () .getBoolean ("footer"));

	// RenderingProperties
	if (getConfig () .hasItem ("renderingProperties"))
		getRenderingPropertiesMenuItem () .set_active (getConfig () .getBoolean ("renderingProperties"));

	// Rubberband
	if (getConfig () .hasItem ("rubberBand"))
		getRubberbandMenuItem () .set_active (getConfig () .getBoolean ("rubberBand"));

	if (not getConfig () .hasItem ("isLive"))
		getConfig () .setItem ("isLive", true);

	isLive (isLive ());

	// VPaned
	if (getConfig () .hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () .getInteger ("vPaned"));

	// HPaned
	if (getConfig () .hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () .getInteger ("hPaned"));

	getSideBarNotebook () .set_current_page (getConfig () .getInteger ("sideBarCurrentPage")); // XXX Produces a width/height -1 warning
	getFooterNotebook ()  .set_current_page (getConfig () .getInteger ("footerCurrentPage"));
}

void
X3DBrowserWidget::saveSession ()
{
	getConfig () .setItem ("vPaned", getVPaned () .get_position ());
	getConfig () .setItem ("hPaned", getHPaned () .get_position ());

	getConfig () .setItem ("sideBarCurrentPage", getSideBarNotebook () .get_current_page ());
	getConfig () .setItem ("footerCurrentPage",  getFooterNotebook ()  .get_current_page ());

	X3DBrowserWindowInterface::saveSession ();
}

std::shared_ptr <BrowserUserData>
X3DBrowserWidget::getUserData (const X3D::BrowserPtr & browser)
{
	if (not browser -> getUserData ())
		browser -> setUserData (X3D::UserDataPtr (new BrowserUserData (browser)));

	return std::static_pointer_cast <BrowserUserData> (browser -> getUserData ());
}

void
X3DBrowserWidget::setBrowser (const X3D::BrowserPtr & value)
{
	browser -> initialized () .removeInterest (this, &X3DBrowserWidget::set_executionContext);

	X3DBrowserWindowInterface::setBrowser (value);

	browser          = value;
	scene            = getBrowser () -> getExecutionContext () -> getMasterContext ();
	executionContext = getBrowser () -> getExecutionContext ();

	browser -> initialized () .addInterest (this, &X3DBrowserWidget::set_executionContext);
	isLive (isLive ());
}

X3D::X3DPtrArray <X3D::Browser>::const_iterator
X3DBrowserWidget::getBrowser (const basic::uri & URL) const
{
	return std::find_if (browsers .cbegin (), browsers .cend (), [&URL] (const X3D::BrowserPtr & browser)
	                     {
	                        auto worldURL = browser -> getExecutionContext () -> getMasterContext () -> getWorldURL ();

	                        if (worldURL .empty ())
										worldURL = X3DBrowserWindow::getUserData (browser) -> URL;

	                        return worldURL == URL;
								});
}

void
X3DBrowserWidget::setExecutionContext (const X3D::X3DExecutionContextPtr & value)
{
	try
	{
		executionContext = value;

		const X3D::BrowserOptionsPtr browserOptions = new X3D::BrowserOptions (getBrowser ());

		browserOptions -> assign (getBrowser () -> getBrowserOptions ());

		getBrowser () -> replaceWorld (executionContext);
		getBrowser () -> getBrowserOptions () -> assign (browserOptions, true);

		getBrowser () -> isLive () .addInterest (getScene () -> isLive ());
		getScene () -> isLive () = getBrowser () -> isLive ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserWidget::isLive (const bool value)
{
	getConfig () .setItem ("isLive", value);

	getPlayPauseButton () .set_stock_id (Gtk::StockID (value ? "gtk-media-pause" : "gtk-media-play"));

	if (value)
		getBrowser () -> beginUpdate ();
	else
		getBrowser () -> endUpdate ();
}

bool
X3DBrowserWidget::isLive () const
{
	return getConfig () .getBoolean ("isLive");
}

void
X3DBrowserWidget::setTitle () const
{
	const auto userData = getUserData (getBrowser ());
	const bool modified = userData -> modified;

	auto title = getScene () -> getTitle ();

	if (title .empty ())
		title = userData -> URL .basename ();

	if (title .empty ())
		title = _ ("New Scene");

	if (modified)
		title += "*";

	getBrowserNotebook () .set_menu_label_text (*getBrowser (), title);

	if (userData -> icon)
		userData -> icon -> set (Gtk::StockID (getScene () -> getWorldURL () .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (userData -> label)
		userData -> label -> set_text (title);

	getWindow () .set_title (getScene () -> getTitle ()
	                         + " · "
	                         + getScene () -> getWorldURL () .filename ()
	                         + (modified ? "*" : "")
	                         + " · "
	                         + getBrowser () -> getName ());
}

void
X3DBrowserWidget::openRecent ()
{
	aboutTab -> loadPreview (getBrowser ());
	aboutTab -> open ();
}

void
X3DBrowserWidget::open (const basic::uri & URL, const bool splashScreen)
{
	const auto iter = getBrowser (URL);

	if (iter not_eq browsers .cend ())
		getBrowserNotebook () .set_current_page (iter - browsers .cbegin ());

	else
	{
		append (X3D::createBrowser (getBrowser ()), URL, splashScreen);
		getBrowserNotebook () .set_current_page (browsers .size () - 1);
	}
}

void
X3DBrowserWidget::append (const X3D::BrowserPtr & browser, const basic::uri & URL, const bool splashScreen)
{
	browsers .emplace_back (browser);

	if (not splashScreen and not URL .empty ())
		browser -> getBrowserOptions () -> splashScreenURL () = { get_page ("about/blank.x3dv") .str () };

	browser -> initialized () .addInterest (this, &X3DBrowserWidget::set_splashScreen, browser, URL);
	browser -> getBrowserOptions () -> splashScreen () = not URL .empty ();
	browser -> set_antialiasing (4);
	browser -> show ();

	const auto text   = URL .empty () ? _ ("New Scene") : URL .basename ();
	const auto icon   = Gtk::manage (new Gtk::Image (Gtk::StockID (URL .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
	const auto label  = Gtk::manage (new Gtk::Label (text));
	const auto button = Gtk::manage (new Gtk::Button ());
	const auto image  = Gtk::manage (new Gtk::Image (Gtk::StockID ("gtk-close"), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
	const auto box    = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 4));

	button -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (*this, &X3DBrowserWidget::close), browser));
	button -> get_style_context () -> add_class ("titania-tab-close");
	button -> set_image (*image);
	button -> set_tooltip_text (_ ("Close Tab"));

	box -> pack_start (*icon,   false, true, 0);
	box -> pack_start (*label,  true,  true, 0);
	box -> pack_start (*button, false, true, 0);
	box -> show_all ();

	getBrowserNotebook () .append_page (*browser, *box);
	getBrowserNotebook () .set_tab_reorderable (*browser, true);
	getBrowserNotebook () .set_menu_label_text (*browser, text);
	getBrowserNotebook () .set_show_tabs (browsers .size () > 1);

	const auto userData = getUserData (browser);

	userData -> URL   = URL;
	userData -> icon  = icon;
	userData -> label = label;
}

void
X3DBrowserWidget::set_splashScreen (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	loadTime = chrono::now ();

	browser -> initialized () .removeInterest (this, &X3DBrowserWidget::set_splashScreen);

	load (browser, URL);
}

void
X3DBrowserWidget::load (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	if (browser == getBrowser ())
		aboutTab -> loadPreview (getBrowser ());

	try
	{
		if (URL .empty ())
			return;

		browser -> loadURL ({ URL .str () });
	}
	catch (const X3D::X3DError & error)
	{
		try
		{
			std::ostringstream osstream;

			osstream
				<< get_page ("about/url_error.x3dv")
				<< "?type=" << basic::to_string (error .getType ())
				<< ";what=" << Glib::uri_escape_string (error .what ());

			const auto scene = browser -> createX3DFromURL ({ osstream .str () });

			scene -> setWorldURL (URL);

			browser -> replaceWorld (scene);
		}
		catch (const X3D::X3DError &)
		{
			try
			{
				browser -> replaceWorld (nullptr);
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
}

void
X3DBrowserWidget::save (const basic::uri & worldURL, const bool compressed)
{
	const auto suffix           = worldURL .suffix ();
	const auto executionContext = X3D::X3DExecutionContextPtr (getRootContext ());

	using namespace std::placeholders;

	X3D::traverse (executionContext,
	               std::bind (&X3DBrowserWidget::transform, executionContext -> getWorldURL (), worldURL, std::make_shared <UndoStep> (""), _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS |
	               X3D::TRAVERSE_ROOT_NODES);

	executionContext -> setWorldURL (worldURL);
	executionContext -> isCompressed (compressed);

	// Save

	if (suffix == ".x3d" or suffix == ".xml")
	{
		if (executionContext -> getVersion () == X3D::VRML_V2_0)
		{
			executionContext -> setEncoding ("X3D");
			executionContext -> setSpecificationVersion (X3D::XMLEncode (X3D::LATEST_VERSION));
		}

		if (compressed)
		{
			ogzstream file (worldURL .path ());

			file
				<< X3D::SmallestStyle
				<< X3D::XMLEncode (executionContext);
		}
		else
		{
			std::ofstream file (worldURL .path ());

			file
				<< X3D::CompactStyle
				<< X3D::XMLEncode (executionContext);
		}
	}
	else
	{
		if (suffix == ".wrl")
		{
			if (executionContext -> getVersion () not_eq X3D::VRML_V2_0)
			{
				executionContext -> setEncoding ("VRML");
				executionContext -> setSpecificationVersion ("2.0");
			}
		}
		else  // ".x3dv"
		{
			if (executionContext -> getVersion () == X3D::VRML_V2_0)
			{
				executionContext -> setEncoding ("X3D");
				executionContext -> setSpecificationVersion (X3D::XMLEncode (X3D::LATEST_VERSION));
			}
		}

		if (compressed)
		{
			ogzstream file (worldURL .path ());

			file
				<< X3D::SmallestStyle
				<< executionContext;
		}
		else
		{
			std::ofstream file (worldURL .path ());

			file
				<< X3D::NicestStyle
				<< executionContext;
		}
	}
}

bool
X3DBrowserWidget::transform (const basic::uri & oldWorldURL, const basic::uri & newWorldURL, const UndoStepPtr & undoStep, X3D::SFNode & node)
{
	using MFString = X3D::X3DField <X3D::MFString::internal_type>;
	using set      = void (MFString::*) (const MFString &);

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3D::X3DConstants::Background :
				{
					X3D::X3DPtr <X3D::Background> background (node);

					undoStep -> addObjects (background);
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> frontUrl ()),  background -> frontUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> backUrl ()),   background -> backUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> leftUrl ()),   background -> leftUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> rightUrl ()),  background -> rightUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> topUrl ()),    background -> topUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> bottomUrl ()), background -> bottomUrl ());

					X3D::X3DUrlObject::transform (background -> frontUrl (),  oldWorldURL, newWorldURL);
					X3D::X3DUrlObject::transform (background -> backUrl (),   oldWorldURL, newWorldURL);
					X3D::X3DUrlObject::transform (background -> leftUrl (),   oldWorldURL, newWorldURL);
					X3D::X3DUrlObject::transform (background -> rightUrl (),  oldWorldURL, newWorldURL);
					X3D::X3DUrlObject::transform (background -> topUrl (),    oldWorldURL, newWorldURL);
					X3D::X3DUrlObject::transform (background -> bottomUrl (), oldWorldURL, newWorldURL);

					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> frontUrl ()),  background -> frontUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> backUrl ()),   background -> backUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> leftUrl ()),   background -> leftUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> rightUrl ()),  background -> rightUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> topUrl ()),    background -> topUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> bottomUrl ()), background -> bottomUrl ());
					break;
				}
			case X3D::X3DConstants::X3DUrlObject:
			{
				X3D::X3DPtr <X3D::X3DUrlObject> urlObject (node);

				undoStep -> addObjects (urlObject);
				undoStep -> addUndoFunction ((set) & MFString::set, std::ref (urlObject -> url ()), urlObject -> url ());

				X3D::X3DUrlObject::transform (urlObject -> url (), oldWorldURL, newWorldURL);

				undoStep -> addRedoFunction ((set) & MFString::set, std::ref (urlObject -> url ()), urlObject -> url ());
				break;
			}
			default:
				break;
		}
	}

	return true;
}

void
X3DBrowserWidget::reload ()
{
	load (browser, getScene () -> getWorldURL ());
}

void
X3DBrowserWidget::close (const X3D::BrowserPtr & browser)
{
	if (browser == getBrowser ())
		aboutTab -> loadPreview (browser);

	browser -> initialized () .removeInterest (this, &X3DBrowserWidget::set_splashScreen);

	getUserData (browser) -> dispose ();

	browsers .remove (browser);

	if (browsers .empty ())
		openRecent ();

	getBrowserNotebook () .remove_page (*browser);
	getBrowserNotebook () .set_show_tabs (browsers .size () > 1);
}

bool
X3DBrowserWidget::quit ()
{
	aboutTab -> loadPreview (getBrowser ());

	std::deque <std::string> worldURLs;
	std::deque <std::string> browserHistories;

	for (const auto & browser : browsers)
	{
		const auto userData = getUserData (browser);

		auto URL = browser -> getExecutionContext () -> getMasterContext () -> getWorldURL ();

		if (URL .empty ())
			URL = userData -> URL;

		if (not URL .empty ())
			worldURLs .emplace_back (URL);

		browserHistories .emplace_back (userData -> browserHistory .toString ());

		userData -> dispose ();
	}

	auto currentPage = getBrowserNotebook () .get_current_page ();

	if (browsers [currentPage] -> getExecutionContext () -> getMasterContext () -> getWorldURL () .empty ())
		currentPage = 0;

	getConfig () .setItem ("currentPage", currentPage);
	getConfig () .setItem ("worldURL",    basic::join (worldURLs, "\n"));
	getConfig () .setItem ("history",     basic::join (browserHistories, "\n"));

	X3DBrowserWindowInterface::quit ();
	return false;
}

void
X3DBrowserWidget::on_switch_browser (Gtk::Widget*, guint pageNumber)
{
	aboutTab -> loadPreview (getBrowser ());

	setBrowser (browsers [pageNumber]);
}

void
X3DBrowserWidget::on_browser_reordered (Widget* widget, guint pageNumber)
{
	const auto iter = std::find (browsers .begin (), browsers .end (), widget);

	if (iter == browsers .end ())
		return;

	const auto browser = *iter;

	browsers .erase (iter);
	browsers .insert (browsers .begin () + pageNumber, browser);
}

void
X3DBrowserWidget::set_executionContext ()
{
	if (getBrowser () -> getExecutionContext () not_eq executionContext)
	{
		if (X3D::X3DScenePtr (getBrowser () -> getExecutionContext ()))
			scene = getBrowser () -> getExecutionContext ();

		executionContext = getBrowser () -> getExecutionContext ();
	}
}

void
X3DBrowserWidget::set_scene ()
{
	loadTime = chrono::now () - loadTime;

	timeout .disconnect ();
	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DBrowserWidget::statistics), 10 * 1000);

	loadIcon ();
	setTitle ();
}

void
X3DBrowserWidget::loadIcon ()
{
	const basic::uri & worldURL = getScene () -> getWorldURL ();

	try
	{
		basic::uri uri;

		try
		{
			uri = getScene () -> getMetaData ("icon");
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			if (worldURL .is_local ())
				throw;

			uri = "/favicon.ico";
		}

		loadIcon (worldURL, X3D::Loader (getScene ()) .loadDocument (uri));
	}
	catch (const std::exception & error)
	{
		loadIcon (worldURL, "");
	}
}

void
X3DBrowserWidget::loadIcon (const basic::uri & worldURL, const std::string & document)
{
	const Gtk::StockID stockId = Gtk::StockID (worldURL .filename () .str ());

	Glib::RefPtr <Gtk::IconSet> iconSet;

	if (not document .empty ())
	{
		try
		{
			const titania::Image icon (document);

			iconSet = Gtk::IconSet::create (Gdk::Pixbuf::create_from_data (icon .getData (),
			                                                               Gdk::COLORSPACE_RGB,
			                                                               icon .getTransparency (),
			                                                               sizeof (Image::value_type) * 8,
			                                                               icon .getWidth (),
			                                                               icon .getHeight (),
			                                                               icon .getWidth () * icon .getComponents ()) -> copy ());
		}
		catch (const std::exception & error)
		{ }
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, worldURL .filename () .str ()));
}

std::string
X3DBrowserWidget::getIcon (const basic::uri & worldURL, const Gtk::IconSize & iconSize)
{
	std::string image;
	auto        iconSet = Gtk::IconSet::lookup_default (Gtk::StockID (worldURL .filename () .str ()));

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	if (iconSet)
	{
		try
		{
			const auto pixbuf = iconSet -> render_icon_pixbuf (getWidget () .get_style_context (), iconSize);

			if (pixbuf)
			{
				gchar* buffer;
				gsize  bufferSize;

				pixbuf -> save_to_buffer (buffer, bufferSize);

				image = std::string (buffer, bufferSize);

				g_free (buffer);
			}
		}
		catch (const Glib::Exception &)
		{ }
	}

	return image;
}

void
X3DBrowserWidget::setTransparent (const bool value)
{
	if (value)
	{
		Glib::RefPtr <Gdk::Visual> visual = getWindow () .get_screen () -> get_rgba_visual ();

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
}

bool
X3DBrowserWidget::statistics ()
{
	std::string title = getScene () -> getWorldURL ();

	try
	{
		title = getScene () -> getMetaData ("title");
		std::clog << "Statistics for: " << title << std::endl;
	}
	catch (...)
	{ }

	std::clog << "Load Time: " << loadTime << std::endl;
	std::clog << "FPS: " << getBrowser () -> getRenderingProperties () -> getFPS () << std::endl;

	return false;
}

X3DBrowserWidget::~X3DBrowserWidget ()
{ }

} // puck
} // titania
