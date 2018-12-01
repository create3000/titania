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

#include "X3DBrowserNotebook.h"

#include "../Browser/IconFactory.h"
#include "../Browser/RecentView.h"
#include "../Browser/UserData.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../Configuration/config.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Editors/HistoryEditor/History.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Browser/Core/RenderingProperties.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>

#include <Titania/String.h>
#include <Titania/Stream/GZStream.h>
#include <fstream>

// DEBUG rooted objects
//#include <Titania/X3D/Debug.h>

namespace titania {
namespace puck {

X3DBrowserNotebook::X3DBrowserNotebook (const X3D::BrowserPtr & defaultBrowser) :
	X3DBrowserWindowInterface (),
	                     page (),
	                    pages (),
	              recentPages (),
	            masterBrowser (defaultBrowser),
	                  browser (X3D::createBrowser ()),
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
	                 scene,
	                 executionContext);

	setTitleBar (getWindow (), getHeaderBar ());
}

void
X3DBrowserNotebook::initialize ()
{
	X3DBrowserWindowInterface::initialize ();

	// Master browser

	masterBrowser -> initialized () .addInterest (&X3DBrowserNotebook::set_masterBrowser, this);
	masterBrowser -> setAntialiasing (4);
	masterBrowser -> show ();
	getLogoBox () .pack_start (*getMasterBrowser (), true, true, 0);

	// History

	if (not getConfig () -> hasItem ("rememberHistory"))
		getConfig () -> setItem ("rememberHistory", -1);

	getHistory () -> setSize (getConfig () -> getItem <int32_t> ("rememberHistory"));

	recentView -> initialize ();

	// Collect already open pages.

	std::set <basic::uri> urlIndex;

	for (const auto & page : pages)
		urlIndex .emplace (page -> getWorldURL ());

	// Restore pages.

	const auto files_exists = [ ] (const std::vector <std::string> & files)
	{
		std::vector <std::string> filtered;

		for (const auto & file : files)
		{
			const auto uri = basic::uri (file);

			if (uri .is_local () and not Glib::file_test (uri .path (), Glib::FILE_TEST_EXISTS))
				continue;

			filtered .emplace_back (file);
		}

		return filtered;
	};

	const auto empty     = pages .empty ();
	auto       worldURLs = std::vector <std::string> ();
	auto       recent    = std::vector <std::string> ();

	basic::split (std::back_inserter (worldURLs), getConfig () -> getItem <std::string> ("worldURL"), "\n");
	basic::split (std::back_inserter (recent),    getConfig () -> getItem <std::string> ("recent"),   "\n");

	worldURLs = files_exists (worldURLs);
	recent    = files_exists (recent);

	if (worldURLs .empty () and empty)
		worldURLs .emplace_back (get_page ("about/home.x3dv"));

	for (size_t i = 0; i < worldURLs .size (); ++ i)
	{
		if (urlIndex .count (worldURLs [i]))
			continue;

		const auto page = append (worldURLs [i]);
	}

	if (empty)
	{
		const size_t pageNumber = getConfig () -> getItem <int32_t> ("currentPage");

		if (pageNumber < pages .size ())
			getBrowserNotebook () .set_current_page (pageNumber);
	}
	else
		getBrowserNotebook () .set_current_page (0);

	for (const auto & URL : recent)
	{
		try
		{
			recentPages .emplace_back (getPage (URL));
		}
		catch (const std::out_of_range &)
		{ }
	}

	// 

	getCurrentScene ()  .addInterest (&X3DBrowserNotebook::set_history, this);
	worldURL_changed () .addInterest (&X3DBrowserNotebook::set_history, this);

	getBrowserNotebook () .set_visible (true);

	// Initialize IconFactory

	getIconFactory () -> initialize ();
}

void
X3DBrowserNotebook::configure ()
{
	X3DBrowserWindowInterface::configure ();

	// Restore Menu Configuration from Config


	// Meta data

	if (not getConfig () -> hasItem ("addStandardMetaData"))
		getConfig () -> setItem <bool> ("addStandardMetaData", true);
	
	// RenderingProperties

	getRenderingPropertiesAction () -> set_active (getConfig () -> getItem <bool> ("renderingProperties"));

	
	// Rubberband

	getRubberbandAction () -> set_active (getConfig () -> getItem <bool> ("rubberBand", true));

	if (not getConfig () -> hasItem ("isLive"))
		getConfig () -> setItem <bool> ("isLive", true);

	
	// isLive

	isLive (isLive ());


	// VPaned

	if (getConfig () -> hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () -> getItem <int32_t> ("vPaned"));

	// HPaned

	if (getConfig () -> hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () -> getItem <int32_t> ("hPaned"));


	getStraightenHorizonButton () .set_active (getConfig () -> getItem <bool> ("straightenHorizon"));
}

void
X3DBrowserNotebook::set_masterBrowser ()
{
	masterBrowser -> initialized () .removeInterest (&X3DBrowserNotebook::set_masterBrowser, this);
}

void
X3DBrowserNotebook::on_fullscreen (const bool value)
{
	X3DBrowserWindowInterface::on_fullscreen (value);

	if (value)
	{
		getMenubarAction () -> set_active (getConfig () -> getItem <bool> ("menubarFullscreen"));
		getToolbarAction () -> set_active (getConfig () -> getItem <bool> ("toolbarFullscreen"));
		getSidebarAction () -> set_active (getConfig () -> getItem <bool> ("sidebarFullscreen"));
		getFooterAction ()  -> set_active (getConfig () -> getItem <bool> ("footerFullscreen"));
		getTabsAction ()    -> set_active (getConfig () -> getItem <bool> ("tabsFullscreen"));
	}
	else
	{
		getMenubarAction () -> set_active (getConfig () -> getItem <bool> ("menubar", true));
		getToolbarAction () -> set_active (getConfig () -> getItem <bool> ("toolbar", true));
		getSidebarAction () -> set_active (getConfig () -> getItem <bool> ("sidebar", true));
		getFooterAction ()  -> set_active (getConfig () -> getItem <bool> ("footer",  true));
		getTabsAction ()    -> set_active (getConfig () -> getItem <bool> ("tabs",    true));
	}
}

void
X3DBrowserNotebook::store ()
{
	getConfig () -> setItem <int32_t> ("vPaned",            getVPaned () .get_position ());
	getConfig () -> setItem <int32_t> ("hPaned",            getHPaned () .get_position ());
	getConfig () -> setItem <bool>    ("straightenHorizon", getStraightenHorizonButton () .get_active ());

	X3DBrowserWindowInterface::store ();
}

/***
 *  Return an iterator to a page widget determined by @a URL.
 */
///  throws std::out_of_range
NotebookPagePtr
X3DBrowserNotebook::getPage (const basic::uri & URL) const
{
	const auto iter = std::find_if (pages .begin (), pages .end (), [&] (const NotebookPagePtr & page)
	{
		return page -> getMasterSceneURL () == URL;
	});

	if (iter == pages .end ())
		throw std::out_of_range ("X3DBrowserNotebook::getPage");

	return *iter;
}

void
X3DBrowserNotebook::setPage (const NotebookPagePtr & value)
{
	browser -> shutdown ()    .removeInterest (&X3DBrowserNotebook::set_shutdown,         this);
	browser -> initialized () .removeInterest (&X3DBrowserNotebook::set_executionContext, this);
	browser -> getUrlError () .removeInterest (&X3DBrowserNotebook::set_urlError,         this);

	browser -> endUpdate ();

	page             = value;
	browser          = page -> getMainBrowser ();
	scene            = browser -> getExecutionContext () -> getMasterScene ();
	executionContext = browser -> getExecutionContext ();

	setBrowser (browser);
	
	page -> updateTitle ();

	browser -> shutdown ()    .addInterest (&X3DBrowserNotebook::set_shutdown,         this);
	browser -> initialized () .addInterest (&X3DBrowserNotebook::set_executionContext, this);
	browser -> getUrlError () .addInterest (&X3DBrowserNotebook::set_urlError,         this);

	browser -> setStraightenHorizon (getStraightenHorizonButton () .get_active ());

	isLive (isLive ());
}

void
X3DBrowserNotebook::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	try
	{
		getCurrentBrowser () -> replaceWorld (value);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserNotebook::isLive (const bool value)
{
	getConfig () -> setItem ("isLive", value);

	getPlayPauseButton () .set_stock_id (Gtk::StockID (value ? "gtk-media-pause" : "gtk-media-play"));

	if (value)
		getCurrentBrowser () -> beginUpdate ();
	else
		getCurrentBrowser () -> endUpdate ();
}

bool
X3DBrowserNotebook::isLive () const
{
	return getConfig () -> getItem <bool> ("isLive");
}

void
X3DBrowserNotebook::openRecent ()
{
	recentView -> loadPreview (getCurrentBrowser ());
	recentView -> open ();
}

void
X3DBrowserNotebook::open (const basic::uri & URL_)
{
	auto URL = URL_;

	if (URL .is_relative ())
		URL = basic::uri ("file://" + basic::path (Glib::get_current_dir ()) .escape () + "/") .transform (URL);

	try
	{
		getBrowserNotebook () .set_current_page (getPage (URL) -> getPageNumber ());
	}
	catch (const std::out_of_range &)
	{
		append (URL);

		getBrowserNotebook () .set_current_page (pages .size () - 1);
	}
}

NotebookPagePtr
X3DBrowserNotebook::append (const basic::uri & URL)
{
	const auto page    = std::make_shared <NotebookPage> (getBrowserWindow (), URL);
	const auto browser = page -> getMainBrowser ();

	pages .emplace_back (page);

	page -> getTabCloseButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DBrowserNotebook::on_tab_close_clicked), page .get ()));

	getBrowserNotebook () .append_page (page -> getWidget (), page -> getTabWidget ());
	getBrowserNotebook () .set_tab_reorderable (page -> getWidget (), true);
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	return page;
}

bool
X3DBrowserNotebook::getShowTabs () const
{
	const bool showTabs = isFullscreen () ? getConfig () -> getItem <bool> ("tabsFullscreen") : getConfig () -> getItem <bool> ("tabs", true);

	return pages .size () > 1 and showTabs;
}

void
X3DBrowserNotebook::load (const basic::uri & URL)
{
	if (URL .empty ())
		return;

	getCurrentBrowser () -> loadURL ({ URL .escape () .str () }, { });
}

bool
X3DBrowserNotebook::save (const X3D::X3DScenePtr & scene, const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	try
	{
		// Known file type must be also added to BrowserWindow::on_save_activated.

		const auto undoStep = std::make_shared <X3D::UndoStep> ("Save A Copy Undo Step");

		if (getConfig () -> getItem <bool> ("addStandardMetaData"))
			scene -> addStandardMetaData ();

		setOutputStyle (scene, outputStyle);

		X3D::X3DEditor::setWorldURL (scene, worldURL, undoStep);

		X3D::FileGenerator::write (scene, worldURL, outputStyle, true);

		if (copy)
			undoStep -> undo ();

		worldURL_changed () .processInterests ();

		recentView -> loadPreview (scene -> getBrowser ());

		return true;
	}
	catch (const X3D::Error <X3D::NOT_SUPPORTED> & error)
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("The given filename does not have any known file extension. Please enter a known file extension like .x3d or .x3dv."));
		dialog -> run ();

		return false;
	}
	catch (const X3D::Error <X3D::INVALID_URL> & error)
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("Tip: check file and folder permissions."));
		dialog -> run ();
	
		return false;
	}
}

void
X3DBrowserNotebook::setOutputStyle (const X3D::X3DScenePtr & scene, const std::string & outputStyle)
{
	scene -> removeMetaData ("outputStyle"); // TODO: remove this line.

	if (getConfig () -> getItem <bool> ("addStandardMetaData"))
	{
		if (outputStyle == "nicest")
			scene -> removeMetaData ("titania-output-style");
		else
			scene -> setMetaData ("titania-output-style", outputStyle);
	}
}

std::string
X3DBrowserNotebook::getOutputStyle (const X3D::X3DScenePtr & scene) const
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
X3DBrowserNotebook::reload ()
{
	if (getCurrentScene () -> getWorldURL () .empty ())
		load (getCurrentPage () -> getWorldURL ());
	else
		load (getCurrentScene () -> getWorldURL ());
}

void
X3DBrowserNotebook::on_tab_close_clicked (NotebookPage* const page)
{
	try
	{
		close (pages .at (page -> getPageNumber ()));
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserNotebook::close (const NotebookPagePtr page)
{
	const auto browser = page -> getMainBrowser ();

	// Remove browser completely.

	// Open recent browser if browser is the currentBrowser.

	recentPages .erase (std::remove (recentPages .begin (), recentPages .end (), page), recentPages .end ());

	if (browser == getCurrentBrowser ())
	{
		recentView -> loadPreview (browser);

		if (not recentPages .empty ())
			getBrowserNotebook () .set_current_page (recentPages .back () -> getPageNumber ());
	}

	// Remove page.

	page -> shutdown ();
	pages .erase (std::remove (pages .begin (), pages .end (), page), pages .end ());

	if (pages .empty ())
		openRecent ();

	getBrowserNotebook () .remove_page (page -> getWidget ());
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	// DEBUG rooted objects
	//const auto b = browser .getValue ();
	//const_cast <X3D::BrowserPtr &> (browser_) = nullptr;
	//const_cast <X3D::BrowserPtr &> (browser)  = nullptr;
	//X3D::debug_roots (b);
}

bool
X3DBrowserNotebook::quit ()
{
	recentView -> loadPreview (getCurrentBrowser ());

	std::deque <std::string> worldURLs;
	std::deque <std::string> recent;

	for (const auto & page : recentPages)
	{
		const auto URL = page -> getMasterSceneURL ();

		if (not URL .empty ())
			recent .emplace_back (URL);
	}

	for (const auto & page : pages)
	{
		const auto URL = page -> getMasterSceneURL ();

		if (not URL .empty ())
			worldURLs .emplace_back (URL);

		page -> shutdown ();
	}

	auto currentPage = getBrowserNotebook () .get_current_page ();

	try
	{
		// Check if current scene is an empty scene;
		if (pages .at (currentPage) -> getMasterSceneURL () .empty ())
			currentPage = 0;
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
		currentPage = 0;
	}

	getConfig () -> setItem ("currentPage", currentPage);
	getConfig () -> setItem ("worldURL",    basic::join (worldURLs, "\n"));
	getConfig () -> setItem ("recent",      basic::join (recent, "\n"));

	X3DBrowserWindowInterface::quit ();
	return false;
}

void
X3DBrowserNotebook::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	try
	{
		const auto page = pages .at (pageNumber);

		recentPages .erase (std::remove (recentPages .begin (), recentPages .end (), page), recentPages .end ());
		recentPages .emplace_back (page);

		recentView -> loadPreview (getCurrentBrowser ());

		setPage (page);
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserNotebook::on_page_reordered (Gtk::Widget* widget, guint pageNumber)
{
	const auto iter = std::find_if (pages .begin (), pages .end (), [&] (const NotebookPagePtr & page) { return &page -> getWidget () == widget; });

	if (iter == pages .end ())
		return;

	auto page = std::move (*iter);

	pages .erase (iter);
	pages .emplace (pages .begin () + pageNumber, std::move (page));
}

void
X3DBrowserNotebook::set_shutdown ()
{
	recentView -> loadPreview (getCurrentBrowser ());
}

void
X3DBrowserNotebook::set_executionContext ()
{
	X3D::X3DScenePtr currentScene   = nullptr;
	const auto &     currentContext = getCurrentBrowser () -> getExecutionContext ();

	if (currentContext -> isType ({ X3D::X3DConstants::X3DScene }))
		currentScene = currentContext;
	else
		currentScene = currentContext -> getScene ();

	if (currentScene not_eq scene)
		scene = currentScene;

	if (currentContext not_eq executionContext)
		executionContext = currentContext;
}

void
X3DBrowserNotebook::set_history ()
{
	const auto worldURL = getCurrentScene () -> getWorldURL ();

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

	getHistory () -> setItem (worldURL .basename (), worldURL .filename ());
}

void
X3DBrowserNotebook::set_urlError ()
{
	std::ostringstream error;

	for (const auto & string : getCurrentBrowser () -> getUrlError ())
		error << string;

	const auto dialog = createDialog <MessageDialog> ("MessageDialog");

	dialog -> setMessage (_ ("Couldn't open requested file!"));
	dialog -> getTextLabel () .get_style_context () -> add_class ("monospace");
	dialog -> setText (error .str ());
	dialog -> getCancelButton () .set_visible (false);
	dialog -> run ();
}

void
X3DBrowserNotebook::dispose ()
{
	pages       .clear ();
	page        .reset ();
	recentPages .clear ();
	iconFactory .reset ();
	recentView  .reset ();
	history     .reset ();

	X3DBrowserWindowInterface::dispose ();
}

X3DBrowserNotebook::~X3DBrowserNotebook ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
