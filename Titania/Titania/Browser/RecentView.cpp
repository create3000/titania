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

#include "RecentView.h"

#include "../Browser/BrowserSelection.h"
#include "../Browser/IconFactory.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../Configuration/config.h"
#include "../Editors/HistoryEditor/History.h"

#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>
#include <Titania/X3D/Components/Text/Text.h>
#include <Titania/X3D/Components/Texturing/ImageTexture.h>

#include <Titania/String.h>

namespace titania {
namespace puck {

static constexpr size_t  ITEMS           = 9;
static constexpr size_t  PREVIEW_SIZE    = 192;
static constexpr size_t  PREVIEW_QUALITY = 90;
static const std::string PREVIEW_TYPE    = "JPG";

RecentView::RecentView (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	          config (new Configuration ("RecentView"))
{
	// Don't use browserWindow here.
	setup ();
}

void
RecentView::initialize ()
{
	getCurrentScene () .addInterest (&RecentView::set_scene, this);
}

basic::uri
RecentView::getURL () const
{
	return get_page ("about/tab.x3dv");
}

X3D::String
RecentView::getTitle (const X3D::String & string, const size_t max) const
{
	if (string .length () > max)
		return string .substr (0, max) + "…";

	return string;
}

void
RecentView::open ()
{
	getBrowserWindow () -> open (getURL ());
}

void
RecentView::loadPreview (X3D::X3DBrowser* const browser)
{
	try
	{
		if (not browser -> isInitialized ())
			return;

		const auto worldURL = browser -> getWorldURL ();
		auto       image    = browser -> getSnapshot (PREVIEW_SIZE, PREVIEW_SIZE, false, 16);

		image .quality (PREVIEW_QUALITY);
		image .magick (PREVIEW_TYPE);

		Magick::Blob blob;
		image .write (&blob);

		const std::string preview ((char*) blob .data (), blob .length ());

		getBrowserWindow () -> getHistory () -> setPreview (worldURL, preview);
		getBrowserWindow () -> getIconFactory () -> createIcon (worldURL .filename (), preview);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
RecentView::set_scene ()
{
	const auto & scene = getCurrentScene ();

	if (scene -> getWorldURL () not_eq getURL ())
		return;

	// XXX: or extend the use of isPrivate
	getBrowserWindow () -> getSelection () -> setEnabled (false);
	getCurrentBrowser () -> beginUpdate ();

	try
	{
		const auto   beginTouch           = scene -> getNamedNode ("BeginTouch");
		const auto   previousPage         = scene -> getNamedNode ("PreviousPage");
		const auto   nextPage             = scene -> getNamedNode ("NextPage");
		const auto & beginTime            = beginTouch -> getField <X3D::SFTime> ("touchTime");
		const auto & previousPage_changed = previousPage -> getField <X3D::SFInt32> ("value_changed");
		const auto & nextPage_changed     = nextPage -> getField <X3D::SFInt32> ("value_changed");

		beginTime            .addInterest (&RecentView::set_page, this, scene .getValue (), X3D::SFInt32 (0));
		previousPage_changed .addInterest (&RecentView::set_page, this, scene .getValue (), std::cref (previousPage_changed));
		nextPage_changed     .addInterest (&RecentView::set_page, this, scene .getValue (), std::cref (nextPage_changed));

		set_page (scene, X3D::SFInt32 (getConfig () -> getItem <int32_t> ("currentPage")));
	}
	catch (...)
	{ }
}

void
RecentView::set_page (X3D::X3DExecutionContext* const scene, const X3D::SFInt32 & page)
{
	if (page < 0)
		return;

	try
	{
		getConfig () -> setItem ("currentPage", page);
		const auto previousPage = scene -> getNamedNode ("PreviousPage");
		const auto nextPage     = scene -> getNamedNode ("NextPage");
		const auto items        = getBrowserWindow () -> getHistory () -> getItems ((page + 1) * 9, ITEMS);

		previousPage -> getField <X3D::SFInt32> ("keyValue") = page - 1;
		nextPage -> getField <X3D::SFInt32> ("keyValue")     = page + 1;

		scene -> getNamedNode <X3D::Switch> ("PreviousSwitch") -> whichChoice () = (page - 1) < 0 ? -1 : 0;
		scene -> getNamedNode <X3D::Switch> ("NextSwitch") -> whichChoice ()     = items .size () ? 0 : -1;
	}
	catch (...)
	{ }

	size_t i = 0;

	for (const auto & item : getBrowserWindow () -> getHistory () -> getItems (page * 9, ITEMS))
	{
		try
		{
			const auto number = basic::to_string (i, std::locale::classic ());
			const auto image  = Glib::Base64::encode (getBrowserWindow () -> getHistory () -> getPreview (item .at ("id")));

			const auto switchNode  = scene -> getNamedNode <X3D::Switch> ("Switch" + number);
			const auto texture     = scene -> getNamedNode <X3D::ImageTexture> ("Texture" + number);
			const auto text        = scene -> getNamedNode <X3D::Text> ("Text" + number);
			const auto touchSensor = scene -> getNamedNode <X3D::TouchSensor> ("TouchSensor" + number);
			const auto URL         = scene -> getNamedNode ("URL" + number);

			switchNode -> whichChoice ()                 = 0;
			texture -> url ()                            = { "data:image/jpeg;base64," + image, "library/dot-clear.png" };
			text -> string ()                            = { getTitle (item .at ("title"), 20) };
			touchSensor -> description ()                = item .at ("worldURL");
			URL -> getField <X3D::SFString> ("keyValue") = item .at ("worldURL");
			URL -> getField <X3D::SFString> ("value_changed") .addInterest (&RecentView::set_url, this);

			++ i;
		}
		catch (...)
		{ }
	}

	for (; i < ITEMS; ++ i)
	{
		try
		{
			const auto number     = basic::to_string (i, std::locale::classic ());
			const auto switchNode = scene -> getNamedNode <X3D::Switch> ("Switch" + number);
			const auto texture    = scene -> getNamedNode <X3D::ImageTexture> ("Texture" + number);

			switchNode -> whichChoice () = 1;
			texture -> url ()            = { "library/dot-clear.png" };
		}
		catch (...)
		{ }
	}
}

void
RecentView::set_url (const X3D::SFString & url)
{
	basic::uri URL = url .str ();

	if (URL .is_relative ())
		URL = basic::uri (Glib::get_current_dir () + "/") .transform (URL);

	try
	{
		const auto page       = getBrowserWindow () -> getPage (URL);
		const auto recentPage = getBrowserWindow () -> getCurrentPage ();

		getBrowserWindow () -> getBrowserNotebook () .set_current_page (page -> getPageNumber ());

		// Closing this browser must be deferred, as this browser is currently processing events.
		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (getBrowserWindow (), &X3DBrowserNotebook::close), recentPage), Glib::PRIORITY_HIGH);
	}
	catch (const std::out_of_range &)
	{
		if (not getBrowserWindow () -> isLive ())
			getBrowserWindow () -> getCurrentBrowser () -> endUpdate ();

		getBrowserWindow () -> getSelection () -> setEnabled (getBrowserWindow () -> getSelection () -> getEnabled ());
		getBrowserWindow () -> load (URL);
	}
}

RecentView::~RecentView ()
{
	dispose ();
}

} // puck
} // titania
