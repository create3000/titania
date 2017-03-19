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

#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Widgets/HistoryView/History.h"
#include "../Browser/RecentView.h"
#include "../Browser/BrowserUserData.h"
#include "../Browser/Image.h"
#include "../Browser/UserData.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"

#include <Titania/X3D/Basic/Traverse.h>
#include <Titania/X3D/Browser/RenderingProperties.h>
#include <Titania/X3D/Components/EnvironmentalEffects/Background.h>

#include <Titania/OS/cwd.h>
#include <Titania/String.h>
#include <Titania/Stream/GZStream.h>
#include <fstream>

// DEBUG rooted objects
//#include <Titania/X3D/Debug.h>

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (const X3D::BrowserPtr & masterBrowser_) :
	X3DBrowserWindowInterface (),
	                  history (new History ()),
	            masterBrowser (masterBrowser_),
	                  browser (X3D::createBrowser ()),
	                 browsers (),
	           recentBrowsers (),
	                    scene (browser -> getExecutionContext ()),
	         executionContext (browser -> getExecutionContext ()),
	           worldURLOutput (),
	               recentView (new RecentView (getBrowserWindow ()))
{
	// Allways added fields, otherwise there is a Xlib error on destruction.

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

	getMasterBrowser () -> initialized () .addInterest (&X3DBrowserWidget::set_initialized, this);
	getMasterBrowser () -> setAntialiasing (4);
	getMasterBrowser () -> show ();
	getLogoBox () .pack_start (*getMasterBrowser (), true, true, 0);

	getHistory () -> constrainSize (getConfig () -> getInteger ("rememberHistory"));
}

void
X3DBrowserWidget::set_initialized ()
{
	getMasterBrowser () -> initialized () .removeInterest (&X3DBrowserWidget::set_initialized, this);

	recentView -> initialize ();

	// Restore browsers.

	std::set <basic::uri> urlIndex;

	for (const auto & browser : browsers)
		urlIndex .emplace (getWorldURL (browser));

	const auto empty     = browsers .empty ();
	auto       worldURLs = std::vector <std::string> ();
	auto       histories = std::vector <std::string> ();

	basic::split (std::back_inserter (worldURLs), getConfig () -> getString ("worldURL"), "\n");
	basic::split (std::back_inserter (histories), getConfig () -> getString ("history"),  "\n");

	if (worldURLs .empty () and empty)
		worldURLs .emplace_back (get_page ("about/home.x3dv"));

	for (size_t i = 0; i < worldURLs .size (); ++ i)
	{
		if (urlIndex .count (worldURLs [i]))
			continue;

		const auto browser = X3D::createBrowser (getMasterBrowser ());

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
	const auto userData = getUserData (getCurrentBrowser ());
	const bool modified = getModified (getCurrentBrowser ());
	const auto title    = getTitle (getCurrentBrowser ());

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
		append (X3D::createBrowser (getMasterBrowser ()), URL);
		getBrowserNotebook () .set_current_page (browsers .size () - 1);
	}
}

void
X3DBrowserWidget::append (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	browsers .emplace_back (browser);

	if (not URL .empty ())
		browser -> initialized () .addInterest (&X3DBrowserWidget::set_browser, this, browser, URL);

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
	browser -> initialized () .addInterest (&X3DBrowserWidget::set_url, this, browser, URL);
	browser -> loadURL ({ get_page ("about/splash.x3dv") .str () }, { });
}

void
X3DBrowserWidget::set_url (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_url, this);

	load (browser, URL);
}

void
X3DBrowserWidget::load (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	if (browser == getCurrentBrowser ())
		recentView -> loadPreview (getCurrentBrowser ());

	loadTime = chrono::now ();

	if (URL .empty ())
		return;

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
	// Known file type must be also added to BrowserWindow::on_save_activated.

	const auto suffix   = worldURL .suffix ();
	const auto undoStep = std::make_shared <X3D::UndoStep> ("");

	if (getConfig () -> getBoolean ("addStandardMetaData"))
		scene -> addStandardMetaData ();

	// Save

	if (suffix == ".x3d")
	{
		if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
		{
			scene -> setEncoding (X3D::EncodingType::XML);
			scene -> setSpecificationVersion (X3D::LATEST_VERSION);
		}

		std::ofstream file (worldURL .path ());

		if (file)
		{
			setWorldURL (scene, worldURL, undoStep);

			setOutputStyle (scene, file, outputStyle);

			file << X3D::XMLEncode (scene);

			if (copy)
				undoStep -> undo ();

			if (file)
				return true;
		}
	}
	if (suffix == ".x3dz")
	{
		if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
		{
			scene -> setEncoding (X3D::EncodingType::XML);
			scene -> setSpecificationVersion (X3D::LATEST_VERSION);
		}

		basic::ogzstream file (worldURL .path ());

		if (file)
		{
			setWorldURL (scene, worldURL, undoStep);

			setOutputStyle (scene, file, outputStyle);

			file << X3D::XMLEncode (scene);

			if (copy)
				undoStep -> undo ();
			
			if (file)
				return true;
		}
	}
	else if (suffix == ".json")
	{
		if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
		{
			scene -> setEncoding (X3D::EncodingType::JSON);
			scene -> setSpecificationVersion (X3D::LATEST_VERSION);
		}

		std::ofstream file (worldURL .path ());

		if (file)
		{
			setWorldURL (scene, worldURL, undoStep);

			setOutputStyle (scene, file, outputStyle);

			file << X3D::JSONEncode (scene);
			
			if (copy)
				undoStep -> undo ();

			if (file)
				return true;
		}
	}
	else
	{
		if (suffix == ".wrl" or suffix == ".vrml" or suffix == ".vrm" or suffix == ".wrz")
		{
			if (scene -> getSpecificationVersion () not_eq X3D::VRML_V2_0)
			{
				scene -> setEncoding (X3D::EncodingType::VRML);
				scene -> setSpecificationVersion (X3D::VRML_V2_0);
			}
		}
		else if (suffix == ".x3dv" or suffix == ".x3dvz")
		{
			if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
			{
				scene -> setEncoding (X3D::EncodingType::VRML);
				scene -> setSpecificationVersion (X3D::LATEST_VERSION);
			}
		}
		else
		{
			const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
		
			dialog -> setType (Gtk::MESSAGE_ERROR);
			dialog -> setMessage (_ ("Couldn't save file!"));
			dialog -> setText (_ ("The given filename does not have any known file extension. Please enter a known file extension like .x3d or .x3dv."));
			dialog -> run ();

			return false;
		}

		if (suffix == ".x3dvz" or suffix == ".wrz")
		{
			basic::ogzstream file (worldURL .path ());

			if (file)
			{
				setWorldURL (scene, worldURL, undoStep);

				setOutputStyle (scene, file, outputStyle);

				file << scene;
				
				if (copy)
					undoStep -> undo ();

				if (file)
					return true;
			}
		}
		else
		{
			std::ofstream file (worldURL .path ());

			if (file)
			{
				setWorldURL (scene, worldURL, undoStep);

				setOutputStyle (scene, file, outputStyle);

				file << scene;
				
				if (copy)
					undoStep -> undo ();

				if (file)
					return true;
			}
		}
	}

	const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));

	dialog -> setType (Gtk::MESSAGE_ERROR);
	dialog -> setMessage (_ ("Couldn't save file!"));
	dialog -> setText (_ ("Tip: check file and folder permissions."));
	dialog -> run ();

	return false;
}

void
X3DBrowserWidget::setOutputStyle (const X3D::X3DScenePtr & scene, std::ostream & file, const std::string & outputStyle)
{
	if (getConfig () -> getBoolean ("addStandardMetaData"))
		scene -> setMetaData ("outputStyle", outputStyle);

	X3D::Generator::Style (file, outputStyle);
}

std::string
X3DBrowserWidget::getOutputStyle (const X3D::X3DScenePtr & scene) const
{
	try
	{
		return scene -> getMetaData ("outputStyle");
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return "nicest";
	}
}

void
X3DBrowserWidget::setWorldURL (const X3D::X3DScenePtr & scene, const basic::uri & worldURL, const X3D::UndoStepPtr & undoStep)
{
	if (worldURL == scene -> getWorldURL ())
		return;

	using namespace std::placeholders;

	X3D::traverse (scene,
	               std::bind (&X3DBrowserWidget::transform, scene -> getWorldURL (), worldURL, undoStep, _1),
	               true,
	               X3D::TRAVERSE_EXTERNPROTO_DECLARATIONS |
	               X3D::TRAVERSE_PROTO_DECLARATIONS |
	               X3D::TRAVERSE_ROOT_NODES);

	undoStep -> addUndoFunction (&X3D::Output::processInterests, std::ref (worldURL_changed ()));
	undoStep -> addUndoFunction (&X3D::X3DScene::setWorldURL, scene, scene -> getWorldURL ());
	undoStep -> addRedoFunction (&X3D::X3DScene::setWorldURL, scene, worldURL);
	undoStep -> addRedoFunction (&X3D::Output::processInterests, std::ref (worldURL_changed ()));
	scene -> setWorldURL (worldURL);

	worldURL_changed () .processInterests ();

	recentView -> loadPreview (scene -> getBrowser ());
}

bool
X3DBrowserWidget::transform (const basic::uri & oldWorldURL, const basic::uri & newWorldURL, const X3D::UndoStepPtr & undoStep, X3D::SFNode & node)
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

	// Remove browser copletely.

	// Important here to remove the interests, because the notebook could make it visible on remove_page.
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_browser, this);
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_url, this);

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

	X3DBrowserWindowInterface::quit ();
	return false;
}

void
X3DBrowserWidget::on_switch_browser (Gtk::Widget*, guint pageNumber)
{
	recentView -> loadPreview (getCurrentBrowser ());

	setBrowser (browsers [pageNumber]);

	recentBrowsers .remove (getCurrentBrowser ());
	recentBrowsers .emplace_back (getCurrentBrowser ());
}

void
X3DBrowserWidget::on_browser_reordered (Gtk::Widget* widget, guint pageNumber)
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
	createIcon ();
	setTitle ();

	#ifdef TITANIA_DEBUG
	loadTime = chrono::now () - loadTime;
	timeout .disconnect ();
	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DBrowserWidget::statistics), 10 * 1000);
	#endif
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

	getHistory () -> setItem (title, worldURL, getIcon (worldURL, Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
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
X3DBrowserWidget::createIcon ()
{
	const basic::uri & worldURL = getCurrentScene () -> getWorldURL ();

	try
	{
		basic::uri uri;

		try
		{
			uri = getCurrentScene () -> getMetaData ("icon");
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			if (worldURL .is_local ())
				throw;

			uri = "/favicon.ico";
		}

		createIcon (worldURL, X3D::Loader (getCurrentScene ()) .loadDocument (uri));
	}
	catch (const std::exception & error)
	{
		createIcon (worldURL, "");
	}
}

void
X3DBrowserWidget::createIcon (const std::string & name, const std::string & document)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	if (not document .empty ())
	{
		try
		{
			const titania::Image icon (document);

			iconSet = Gtk::IconSet::create (icon .getIcon ());
		}
		catch (const std::exception & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
}

void
X3DBrowserWidget::createIcon (const std::string & name, Magick::Image && image)
{
	Glib::RefPtr <Gtk::IconSet> iconSet;

	try
	{
		const titania::Image icon (std::move (image));

		iconSet = Gtk::IconSet::create (icon .getIcon ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	if (not iconSet)
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));

	const Gtk::StockID stockId = Gtk::StockID (name);

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, name));
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
	std::string title = getCurrentScene () -> getWorldURL ();

	try
	{
		title = getCurrentScene () -> getMetaData ("title");
		std::clog << "Statistics for: " << title << std::endl;
	}
	catch (...)
	{ }

	std::clog << "Load Time: " << loadTime << std::endl;
	std::clog << "FPS: " << getCurrentBrowser () -> getRenderingProperties () -> getFPS () << std::endl;

	return false;
}

void
X3DBrowserWidget::dispose ()
{
	history    .reset ();
	recentView .reset ();

	X3DBrowserWindowInterface::dispose ();
}

X3DBrowserWidget::~X3DBrowserWidget ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
