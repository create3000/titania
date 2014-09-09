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
#include "../Widgets/HistoryView/History.h"

#include <Titania/OS/home.h>
#include <Titania/OS/system.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

AboutTab::AboutTab (X3DBrowserWindow* const browserWindow) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ())
{
	// Don't use browserWindow here.
}

void
AboutTab::open ()
{
	os::system ("mkdir", "-p", "/tmp/titania/tab/");

	History                   historyDB;
	std::vector <std::string> history;
	size_t                    i = 0;

	for (const auto & item : historyDB .getItems (9))
	{
		try
		{
			const auto preview = historyDB .getPreview (item .at ("id"));

			history .emplace_back (item .at ("title") + "\t" + item .at ("worldURL"));

			std::ofstream image ("/tmp/titania/tab/image" + basic::to_string (i) + ".png");

			image << preview;

			++ i;
		}
		catch (...)
		{ }
	}

	std::ostringstream URL;

	URL
		<< get_page ("about/tab.x3dv")
		<< "?history=" << Glib::uri_escape_string (basic::join (history, "\n"));

	getBrowserWindow () -> append (X3D::createBrowser (getBrowser ()), URL .str (), false);
	getBrowserWindow () -> getBrowserNotebook () .set_current_page (getBrowserWindow () -> getBrowsers () .size () - 1);
}

void
AboutTab::loadPreview (const X3D::BrowserPtr & browser)
{
	try
	{
		const auto image = getBrowser () -> getSnapshot (256, 256, false, std::min <size_t> (16, getBrowser () -> getMaxSamples ()));

		image -> quality (85);
		image -> magick ("JPG");

		Magick::Blob blob;
		image -> write (&blob);

		History () .setPreview (getBrowser () -> getExecutionContext () -> getWorldURL (), std::string ((char*) blob .data (), blob .length ()));
	}
	catch (const std::exception & error)
	{ }
}

AboutTab::~AboutTab ()
{ }

} // puck
} // titania
