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

#include "X3DBrowserWidget.h"

#include "../Browser/IconFactory.h"
#include "../Browser/RecentView.h"
#include "../Browser/BrowserUserData.h"
#include "../Browser/UserData.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Editors/HistoryEditor/History.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Browser/RenderingProperties.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>

#include <Titania/OS/cwd.h>
#include <Titania/String.h>
#include <Titania/Stream/GZStream.h>
#include <fstream>

// DEBUG rooted objects
//#include <Titania/X3D/Debug.h>

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (const X3D::BrowserPtr & defaultBrowser) :
	X3DBrowserWindowInterface (),
	            masterBrowser (defaultBrowser),
	                  browser (X3D::createBrowser (defaultBrowser)),
	                 browsers (),
	           recentBrowsers (),
	                    scene (browser -> getExecutionContext ()),
	         executionContext (browser -> getExecutionContext ()),
	           worldURLOutput (),
	              iconFactory (new IconFactory (getBrowserWindow ())),
	               recentView (new RecentView (getBrowserWindow ())),
	                  history (new History ())
{
	// Allways add fields, otherwise there is a Xlib error in destruction.

	addChildObjects (masterBrowser,
	                 browser,
	                 browsers,
	                 recentBrowsers,
	                 scene,
	                 executionContext);
}

void
X3DBrowserWidget::initialize ()
{
	X3DBrowserWindowInterface::initialize ();

	// Master browser

	masterBrowser -> initialized () .addInterest (&X3DBrowserWidget::set_initialized, this);
	masterBrowser -> setAntialiasing (4);
	masterBrowser -> show ();
	getLogoBox () .pack_start (*getMasterBrowser (), true, true, 0);

	// History

	if (not getConfig () -> hasItem ("rememberHistory"))
		getConfig () -> setItem ("rememberHistory", -1);

	getHistory () -> setSize (getConfig () -> getInteger ("rememberHistory"));
}

void
X3DBrowserWidget::set_initialized ()
{
	masterBrowser -> initialized () .removeInterest (&X3DBrowserWidget::set_initialized, this);

	recentView -> initialize ();

	// Restore browsers.

	std::set <basic::uri> urlIndex;

	for (const auto & browser : browsers)
		urlIndex .emplace (getWorldURL (browser));

	const auto empty     = browsers .empty ();
	auto       worldURLs = std::vector <std::string> ();
	auto       histories = std::vector <std::string> ();
	auto       recent    = std::vector <std::string> ();

	basic::split (std::back_inserter (worldURLs), getConfig () -> getString ("worldURL"), "\n");
	basic::split (std::back_inserter (histories), getConfig () -> getString ("history"),  "\n");
	basic::split (std::back_inserter (recent), getConfig () -> getString ("recent"),  "\n");

	if (worldURLs .empty () and empty)
		worldURLs .emplace_back (get_page ("about/home.x3dv"));

	for (size_t i = 0; i < worldURLs .size (); ++ i)
	{
		if (urlIndex .count (worldURLs [i]))
			continue;

		const auto browser = X3D::createBrowser (masterBrowser);

		if (i < histories .size ())
			getUserData (browser) -> browserHistory .fromString (histories [i]);

		append (browser, worldURLs [i]);
	}

	if (empty)
	{
		const size_t pageNumber = getConfig () -> getInteger ("currentPage");

		if (pageNumber < browsers .size ())
			getBrowserNotebook () .set_current_page (pageNumber);
	}
	else
		getBrowserNotebook () .set_current_page (0);

	for (const auto & URL : recent)
	{
		const auto iter = getBrowser (URL);

		if (iter not_eq getBrowsers () .end ())
			recentBrowsers .emplace_back (*iter);
	}

	// 

	getCurrentScene ()  .addInterest (&X3DBrowserWidget::set_scene, this);
	getCurrentScene ()  .addInterest (&X3DBrowserWidget::set_history, this);
	worldURL_changed () .addInterest (&X3DBrowserWidget::set_history, this);

	getBrowserNotebook () .set_visible (true);
}

void
X3DBrowserWidget::configure ()
{
	X3DBrowserWindowInterface::configure ();

	// Restore Menu Configuration from Config


	// Meta data

	if (not getConfig () -> hasItem ("addStandardMetaData"))
		getConfig () -> setItem ("addStandardMetaData", true);


	// ToolBar

	if (not getConfig () -> hasItem ("menubar"))
		getConfig () -> setItem ("menubar", true);

	if (not getConfig () -> hasItem ("menubarFullscreen"))
		getConfig () -> setItem ("menubarFullscreen", false);
	

	// ToolBar

	if (not getConfig () -> hasItem ("toolbar"))
		getConfig () -> setItem ("toolbar", true);

	if (not getConfig () -> hasItem ("toolbarFullscreen"))
		getConfig () -> setItem ("toolbarFullscreen", false);

	
	// SideBar

	if (not getConfig () -> hasItem ("sidebar"))
		getConfig () -> setItem ("sidebar", true);

	if (not getConfig () -> hasItem ("sidebarFullscreen"))
		getConfig () -> setItem ("sidebarFullscreen", false);


	// Footer

	if (not getConfig () -> hasItem ("footer"))
		getConfig () -> setItem ("footer", true);

	if (not getConfig () -> hasItem ("footerFullscreen"))
		getConfig () -> setItem ("footerFullscreen", false);


	// TAbs

	if (not getConfig () -> hasItem ("tabs"))
		getConfig () -> setItem ("tabs", true);

	if (not getConfig () -> hasItem ("tabsFullscreen"))
		getConfig () -> setItem ("tabsFullscreen", false);

	
	// RenderingProperties

	if (getConfig () -> hasItem ("renderingProperties"))
		getRenderingPropertiesAction () -> set_active (getConfig () -> getBoolean ("renderingProperties"));

	
	// Rubberband

	if (getConfig () -> hasItem ("rubberBand"))
		getRubberbandAction () -> set_active (getConfig () -> getBoolean ("rubberBand"));

	if (not getConfig () -> hasItem ("isLive"))
		getConfig () -> setItem ("isLive", true);

	
	// isLive

	isLive (isLive ());


	// VPaned

	if (getConfig () -> hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () -> getInteger ("vPaned"));

	
	// HPaned

	if (getConfig () -> hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () -> getInteger ("hPaned"));

	getStraightenHorizonButton () .set_active (getConfig () -> getBoolean ("straightenHorizon"));
}

void
X3DBrowserWidget::set_fullscreen (const bool value)
{
	if (value)
	{
		getMenubarAction () -> set_active (getConfig () -> getBoolean ("menubarFullscreen"));
		getToolbarAction () -> set_active (getConfig () -> getBoolean ("toolbarFullscreen"));
		getSidebarAction () -> set_active (getConfig () -> getBoolean ("sidebarFullscreen"));
		getFooterAction ()  -> set_active (getConfig () -> getBoolean ("footerFullscreen"));
		getTabsAction ()    -> set_active (getConfig () -> getBoolean ("tabsFullscreen"));
	}
	else
	{
		getMenubarAction () -> set_active (getConfig () -> getBoolean ("menubar"));
		getToolbarAction () -> set_active (getConfig () -> getBoolean ("toolbar"));
		getSidebarAction () -> set_active (getConfig () -> getBoolean ("sidebar"));
		getFooterAction ()  -> set_active (getConfig () -> getBoolean ("footer"));
		getTabsAction ()    -> set_active (getConfig () -> getBoolean ("tabs"));
	}
}

void
X3DBrowserWidget::store ()
{
	getConfig () -> setItem ("vPaned",            getVPaned () .get_position ());
	getConfig () -> setItem ("hPaned",            getHPaned () .get_position ());
	getConfig () -> setItem ("straightenHorizon", getStraightenHorizonButton () .get_active ());

	X3DBrowserWindowInterface::store ();
}

void
X3DBrowserWidget::setBrowser (const X3D::BrowserPtr & value)
{
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_executionContext, this);
	browser -> getUrlError () .removeInterest (&X3DBrowserWidget::set_urlError, this);

	X3DBrowserWindowInterface::setBrowser (value);

	browser          = value;
	scene            = browser -> getExecutionContext () -> getMasterScene ();
	executionContext = browser -> getExecutionContext ();
	
	browser -> initialized () .addInterest (&X3DBrowserWidget::set_executionContext, this);
	browser -> getUrlError () .addInterest (&X3DBrowserWidget::set_urlError, this);

	browser -> setStraightenHorizon (getStraightenHorizonButton () .get_active ());

	isLive (isLive ());
}

/***
 *  Return an iterator to a browser widget determined by @a URL.
 */
X3D::X3DPtrArray <X3D::Browser>::const_iterator
X3DBrowserWidget::getBrowser (const basic::uri & URL) const
{
	return std::find_if (browsers .cbegin (), browsers .cend (), [this, &URL] (const X3D::BrowserPtr & browser)
	                     {
	                        auto worldURL = browser -> getExecutionContext () -> getMasterScene () -> getWorldURL ();

	                        if (browser -> isInitialized ())
	                           return worldURL == URL;
										
									return getWorldURL (browser) == URL;
								});
}

void
X3DBrowserWidget::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	try
	{
		getCurrentBrowser () -> replaceWorld (value);
	}
	catch (const X3D::X3DError &)
	{ }
}

const basic::uri &
X3DBrowserWidget::getWorldURL (const X3D::BrowserPtr & browser) const
{
	if (browser -> isInitialized ())
	   return browser -> getWorldURL ();

	return getUserData (browser) -> URL;
}

void
X3DBrowserWidget::isLive (const bool value)
{
	getConfig () -> setItem ("isLive", value);

	getPlayPauseButton () .set_stock_id (Gtk::StockID (value ? "gtk-media-pause" : "gtk-media-play"));

	if (value)
		getCurrentBrowser () -> beginUpdate ();
	else
		getCurrentBrowser () -> endUpdate ();
}

bool
X3DBrowserWidget::isLive () const
{
	return getConfig () -> getBoolean ("isLive");
}

std::shared_ptr <BrowserUserData>
X3DBrowserWidget::getUserData (const X3D::BrowserPtr & browser)
{
	if (not browser -> getUserData ())
		browser -> setUserData (X3D::UserDataPtr (new BrowserUserData (browser)));

	return std::static_pointer_cast <BrowserUserData> (browser -> getUserData ());
}

std::shared_ptr <UserData>
X3DBrowserWidget::getUserData (const X3D::X3DExecutionContextPtr & executionContext)
{
	if (not executionContext -> getUserData ())
		executionContext -> setUserData (X3D::UserDataPtr (new UserData ()));

	return std::static_pointer_cast <UserData> (executionContext -> getUserData ());
}

std::shared_ptr <UserData>
X3DBrowserWidget::getUserData (const X3D::SFNode & node)
{
	if (not node -> getUserData ())
		node -> setUserData (X3D::UserDataPtr (new UserData ()));

	return std::static_pointer_cast <UserData> (node -> getUserData ());
}

void
X3DBrowserWidget::setTitle ()
{
	const auto userData  = getUserData (getCurrentBrowser ());
	const bool modified  = getModified (getCurrentBrowser ());
	const auto title     = getTitle (getCurrentBrowser ());
	const auto protoPath = getProtoPath (getCurrentContext ());

	getBrowserNotebook () .set_menu_label_text (*getCurrentBrowser (), title);

	if (userData -> icon)
		userData -> icon -> set (Gtk::StockID (getCurrentScene () -> getWorldURL () .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (userData -> label)
	{
		userData -> label -> set_text (title);
		userData -> label -> set_tooltip_text (title);
	}

	getWindow () .set_title (getCurrentContext () -> getTitle ()
	                         + " · "
	                         + getCurrentContext () -> getWorldURL () .filename ()
	                         + (modified ? "*" : "")
	                         + (protoPath .empty () ? "" : " · " + basic::join (protoPath .begin (), protoPath .end (), "."))
	                         + " · "
	                         + getCurrentBrowser () -> getName ());
}

std::string
X3DBrowserWidget::getTitle (const X3D::BrowserPtr & browser) const
{
	const bool modified = getModified (browser);

	auto title = browser -> getExecutionContext () -> getTitle ();

	if (title .empty ())
		title = getWorldURL (browser) .basename ();

	if (title .empty ())
		title = _ ("New Scene");

	if (modified)
		title += "*";

	return title;
}

void
X3DBrowserWidget::openRecent ()
{
	recentView -> loadPreview (getCurrentBrowser ());
	recentView -> open ();
}

void
X3DBrowserWidget::open (const basic::uri & URL_)
{
	auto URL = URL_;

	if (URL .is_relative ())
		URL = basic::uri (os::cwd ()) .transform (URL);

	const auto iter = getBrowser (URL);

	if (iter not_eq browsers .cend ())
		getBrowserNotebook () .set_current_page (iter - browsers .cbegin ());

	else
	{
		append (X3D::createBrowser (masterBrowser), URL);
		getBrowserNotebook () .set_current_page (browsers .size () - 1);
	}
}

void
X3DBrowserWidget::append (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	browsers .emplace_back (browser);

	if (not URL .empty ())
	{
		browser -> initialized () .addInterest (&X3DBrowserWidget::set_browser, this, browser, URL);
		browser -> set_opacity (0);
	}

	browser -> setAntialiasing (4);
	browser -> setNotifyOnLoad (true);
	browser -> isStrict (false);
	browser -> show ();

	const auto text   = URL .empty () ? _ ("New Scene") : URL .basename ();
	const auto icon   = Gtk::manage (new Gtk::Image (Gtk::StockID (URL .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
	const auto label  = Gtk::manage (new Gtk::Label (text));
	const auto button = Gtk::manage (new Gtk::Button ());
	const auto image  = Gtk::manage (new Gtk::Image (Gtk::StockID ("gtk-close"), Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
	const auto box    = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 4));

	label -> set_ellipsize (Pango::ELLIPSIZE_END);
	label -> set_width_chars (14);
	label -> set_lines (1);
	label -> set_tooltip_text (URL .filename () .str ());

	button -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DBrowserWidget::close), browser));
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
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	const auto userData = getUserData (browser);

	userData -> URL   = URL;
	userData -> icon  = icon;
	userData -> label = label;
}

bool
X3DBrowserWidget::getShowTabs () const
{
	const bool showTabs = isFullscreen () ? getConfig () -> getBoolean ("tabsFullscreen") : getConfig () -> getBoolean ("tabs");

	return browsers .size () > 1 && showTabs;
}

void
X3DBrowserWidget::set_browser (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_browser, this);
	browser -> initialized () .addInterest (&X3DBrowserWidget::set_splashScreen, this, browser, URL);
	browser -> loadURL ({ get_page ("about/splash.x3dv") .str () }, { });
}

void
X3DBrowserWidget::set_splashScreen (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_splashScreen, this);
	browser -> set_opacity (1);

	load (browser, URL);
}

void
X3DBrowserWidget::load (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	if (URL .empty ())
		return;

	recentView -> loadPreview (browser);
	browser -> loadURL ({ URL .str () }, { });
}

bool
X3DBrowserWidget::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	return save (getCurrentScene (), worldURL, outputStyle, copy);
}

bool
X3DBrowserWidget::save (const X3D::X3DScenePtr & scene, const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	try
	{
		// Known file type must be also added to BrowserWindow::on_save_activated.

		const auto undoStep = std::make_shared <X3D::UndoStep> ("Save A Copy Undo Step");

		recentView -> loadPreview (scene -> getBrowser ());

		if (getConfig () -> getBoolean ("addStandardMetaData"))
			scene -> addStandardMetaData ();

		setOutputStyle (scene, outputStyle);
		X3D::X3DEditor::setWorldURL (scene, worldURL, undoStep);

		X3D::FileGenerator::write (scene, worldURL, outputStyle);

		if (copy)
			undoStep -> undo ();

		worldURL_changed () .processInterests ();

		return true;
	}
	catch (const X3D::Error <X3D::NOT_SUPPORTED> & error)
	{
		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("The given filename does not have any known file extension. Please enter a known file extension like .x3d or .x3dv."));
		dialog -> run ();

		return false;
	}
	catch (const X3D::Error <X3D::INVALID_URL> & error)
	{
		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("Tip: check file and folder permissions."));
		dialog -> run ();
	
		return false;
	}
}

void
X3DBrowserWidget::setOutputStyle (const X3D::X3DScenePtr & scene, const std::string & outputStyle)
{
	scene -> removeMetaData ("outputStyle"); // TODO: remove this line.

	if (getConfig () -> getBoolean ("addStandardMetaData"))
	{
		if (outputStyle == "nicest")
			scene -> removeMetaData ("titania-output-style");
		else
			scene -> setMetaData ("titania-output-style", outputStyle);
	}
}

std::string
X3DBrowserWidget::getOutputStyle (const X3D::X3DScenePtr & scene) const
{
	try
	{
		return basic::tolower (scene -> getMetaData ("outputStyle"), std::locale::classic ());
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{ }

	try
	{
		return basic::tolower (scene -> getMetaData ("titania-output-style"), std::locale::classic ());
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return "nicest";
	}
}

void
X3DBrowserWidget::reload ()
{
	load (browser, getCurrentScene () -> getWorldURL ());
}

void
X3DBrowserWidget::close (const X3D::BrowserPtr & browser_)
{
	const X3D::BrowserPtr browser = browser_;

	// Open recent browser if browser is the currentBrowser.

	if (browser == getCurrentBrowser ())
	{
		recentView -> loadPreview (browser);
		recentBrowsers .remove (browser);

		if (not recentBrowsers .empty ())
		{
			const auto iter = std::find (browsers .cbegin (), browsers .cend (), recentBrowsers .back ());
		
			if (iter not_eq browsers .cend ())
				getBrowserNotebook () .set_current_page (iter - browsers .cbegin ());
		}
	}

	// Remove browser completely.

	// Important here to remove the interests, because the notebook could make it visible on remove_page.
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_browser, this);
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_splashScreen, this);

	getUserData (browser) -> dispose ();

	browsers       .remove (browser);
	recentBrowsers .remove (browser);

	if (browsers .empty ())
		openRecent ();

	getBrowserNotebook () .remove_page (*browser);
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	// DEBUG rooted objects
	//const auto b = browser .getValue ();
	//const_cast <X3D::BrowserPtr &> (browser_) = nullptr;
	//const_cast <X3D::BrowserPtr &> (browser)  = nullptr;
	//X3D::debug_roots (b);
}

bool
X3DBrowserWidget::quit ()
{
	recentView -> loadPreview (getCurrentBrowser ());

	std::deque <std::string> worldURLs;
	std::deque <std::string> browserHistories;
	std::deque <std::string> recent;

	for (const auto & browser : recentBrowsers)
	{
		const auto userData = getUserData (browser);

		auto URL = browser -> getExecutionContext () -> getMasterScene () -> getWorldURL ();

		if (not browser -> isInitialized ())
			URL = userData -> URL;

		if (not URL .empty ())
			recent .emplace_back (URL);
	}

	for (const auto & browser : browsers)
	{
		const auto userData = getUserData (browser);

		auto URL = browser -> getExecutionContext () -> getMasterScene () -> getWorldURL ();

		if (not browser -> isInitialized ())
			URL = userData -> URL;

		if (not URL .empty ())
			worldURLs .emplace_back (URL);

		browserHistories .emplace_back (userData -> browserHistory .toString ());

		userData -> dispose ();
	}

	auto currentPage = getBrowserNotebook () .get_current_page ();

	if (browsers [currentPage] -> getExecutionContext () -> getMasterScene () -> getWorldURL () .empty ())
		currentPage = 0;

	getConfig () -> setItem ("currentPage", currentPage);
	getConfig () -> setItem ("worldURL",    basic::join (worldURLs, "\n"));
	getConfig () -> setItem ("history",     basic::join (browserHistories, "\n"));
	getConfig () -> setItem ("recent",      basic::join (recent, "\n"));

	X3DBrowserWindowInterface::quit ();
	return false;
}

void
X3DBrowserWidget::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	recentView -> loadPreview (getCurrentBrowser ());

	setBrowser (browsers [pageNumber]);

	recentBrowsers .remove (getCurrentBrowser ());
	recentBrowsers .emplace_back (getCurrentBrowser ());
}

void
X3DBrowserWidget::on_page_reordered (Gtk::Widget* widget, guint pageNumber)
{
	const auto iter = std::find (browsers .begin (), browsers .end (), widget);

	if (iter == browsers .end ())
		return;

	auto browser = std::move (*iter);

	browsers .erase (iter);
	browsers .emplace (browsers .begin () + pageNumber, std::move (browser));
}

void
X3DBrowserWidget::set_scene ()
{
	getIconFactory () -> createIcon (getCurrentScene ());

	setTitle ();
}

void
X3DBrowserWidget::set_executionContext ()
{
	if (getCurrentBrowser () -> getExecutionContext () == executionContext)
	   return;

	X3D::X3DScenePtr currentScene (getCurrentBrowser () -> getExecutionContext ());

	if (not currentScene)
		currentScene = getCurrentBrowser () -> getExecutionContext () -> getScene ();

	if (currentScene not_eq scene)
		scene = std::move (currentScene);

	executionContext = getCurrentBrowser () -> getExecutionContext ();

	setTitle ();
}

void
X3DBrowserWidget::set_history ()
{
	const std::string title    = getCurrentScene () -> getTitle ();
	const basic::uri  worldURL = getCurrentScene () -> getWorldURL ();

	if (worldURL .empty ())
		return;

	try
	{
		if (getCurrentScene () -> getMetaData ("titania-history") == "FALSE")
			return;
	}
	catch (const X3D::X3DError &)
	{ }

	// Update history.

	getHistory () -> setItem (title, worldURL, getIconFactory () -> getIcon (worldURL, Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
}

void
X3DBrowserWidget::set_urlError ()
{
	try
	{
		std::ostringstream error;

		for (const auto & string : getCurrentBrowser () -> getUrlError ())
			error << string .str ();

		std::ostringstream osstream;

		osstream
			<< get_page ("about/url_error.x3dv")
			<< "?type=" << basic::to_string (21, std::locale::classic ())
			<< ";what=" << Glib::uri_escape_string (error .str ());

		const auto scene = getCurrentBrowser () -> createX3DFromURL ({ osstream .str () });

		scene -> setWorldURL (getCurrentBrowser () -> getWorldURL ());

		getCurrentBrowser () -> replaceWorld (scene);
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

void
X3DBrowserWidget::dispose ()
{
	iconFactory .reset ();
	recentView  .reset ();
	history     .reset ();

	X3DBrowserWindowInterface::dispose ();
}

X3DBrowserWidget::~X3DBrowserWidget ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
