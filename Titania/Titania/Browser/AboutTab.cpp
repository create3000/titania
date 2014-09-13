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

#include "AboutTab.h"

#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"

#include <Titania/OS/home.h>
#include <Titania/Stream/Base64.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

static constexpr size_t  ITEMS           = 9;
static constexpr size_t  PREVIEW_SIZE    = 192;
static constexpr size_t  PREVIEW_QUALITY = 90;
static const std::string PREVIEW_TYPE    = "JPG";

AboutTab::AboutTab (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	         history ()
{
	// Don't use browserWindow here.
	setup ();
}

void
AboutTab::initialize ()
{
	getScene () .addInterest (this, &AboutTab::set_scene);
}

basic::uri
AboutTab::getURL () const
{
	return get_page ("about/tab.x3dv");
}

void
AboutTab::open ()
{
	getBrowserWindow () -> append (X3D::createBrowser (getBrowser ()), getURL (), false);
	getBrowserWindow () -> getBrowserNotebook () .set_current_page (getBrowserWindow () -> getBrowsers () .size () - 1);
}

void
AboutTab::loadPreview (const X3D::BrowserPtr & browser)
{
	try
	{
		const auto image = getBrowser () -> getSnapshot (PREVIEW_SIZE, PREVIEW_SIZE, false, std::min <size_t> (16, getBrowser () -> getMaxSamples ()));

		image -> quality (PREVIEW_QUALITY);
		image -> magick (PREVIEW_TYPE);

		Magick::Blob blob;
		image -> write (&blob);

		history .setPreview (getBrowser () -> getExecutionContext () -> getWorldURL (), std::string ((char*) blob .data (), blob .length ()));
	}
	catch (const std::exception & error)
	{ }
}

void
AboutTab::set_scene ()
{
	const auto & scene = getScene ();

	if (scene -> getWorldURL () not_eq getURL ())
		return;

	getBrowser () -> getSelection () -> isEnabled (false);
	getBrowser () -> beginUpdate ();

	try
	{
		const auto   currentPage          = scene -> getNamedNode ("CurrentPage");
		const auto   previousPage         = scene -> getNamedNode ("PreviousPage");
		const auto   nextPage             = scene -> getNamedNode ("NextPage");
		const auto & currentPageValue     = currentPage -> getField <X3D::SFInt32> ("keyValue");
		const auto & previousPage_changed = previousPage -> getField <X3D::SFInt32> ("value_changed");
		const auto & nextPage_changed     = nextPage -> getField <X3D::SFInt32> ("value_changed");

		previousPage_changed .addInterest (this, &AboutTab::set_page, scene .getValue (), std::cref (previousPage_changed));
		nextPage_changed .addInterest (this, &AboutTab::set_page, scene .getValue (), std::cref (nextPage_changed));

		set_page (scene, currentPageValue);
	}
	catch (...)
	{ }
}

void
AboutTab::set_page (X3D::X3DExecutionContext* const scene, const X3D::SFInt32 & page)
{
	if (page < 0)
		return;

	try
	{
		const auto currentPage  = scene -> getNamedNode ("CurrentPage");
		const auto previousPage = scene -> getNamedNode ("PreviousPage");
		const auto nextPage     = scene -> getNamedNode ("NextPage");

		currentPage -> getField <X3D::SFInt32> ("keyValue")  = page;
		previousPage -> getField <X3D::SFInt32> ("keyValue") = page - 1;
		nextPage -> getField <X3D::SFInt32> ("keyValue")     = page + 1;

		scene -> getNamedNode <X3D::Switch> ("PreviousSwitch") -> whichChoice () = (page - 1) < 0 ? -1 : 0;
		scene -> getNamedNode <X3D::Switch> ("NextSwitch") -> whichChoice ()     = (page + 1) * ITEMS < history .getSize () ? 0 : -1;
	}
	catch (...)
	{ }

	size_t i = 0;

	for (const auto & item : history .getItems (page * 9, ITEMS))
	{
		try
		{
			const auto number = basic::to_string (i);
			const auto image  = basic::base64_encode (history .getPreview (item .at ("id")));

			const auto switchNode = scene -> getNamedNode <X3D::Switch> ("Switch" + number);
			const auto texture    = scene -> getNamedNode <X3D::ImageTexture> ("Texture" + number);
			const auto text       = scene -> getNamedNode <X3D::Text> ("Text" + number);
			const auto URL        = scene -> getNamedNode ("URL" + number);

			switchNode -> whichChoice ()                 = 0;
			texture -> url ()                            = { "data:image/jpeg;base64," + image, "library/dot-clear.png" };
			text -> string ()                            = { item .at ("title") };
			URL -> getField <X3D::SFString> ("keyValue") = item .at ("worldURL");
			URL -> getField <X3D::SFString> ("value_changed") .addInterest (this, &AboutTab::set_url);

			++ i;
		}
		catch (...)
		{ }
	}

	for (; i < ITEMS; ++ i)
	{
		try
		{
			const auto number     = basic::to_string (i);
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
AboutTab::set_url (const X3D::SFString & URL)
{
	getBrowserWindow () -> open (URL .str ());
}

AboutTab::~AboutTab ()
{
	dispose ();
}

} // puck
} // titania
