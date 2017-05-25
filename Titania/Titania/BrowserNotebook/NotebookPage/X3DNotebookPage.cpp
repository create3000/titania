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

#include "X3DNotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include "../BrowserView/BrowserView.h"

namespace titania {
namespace puck {

X3DNotebookPage::X3DNotebookPage (const basic::uri & startUrl) :
	X3DNotebookPageInterface (get_ui ("Widgets/NotebookPage.glade")),
	             mainBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser ())),
	                     url (startUrl),
	          browserHistory (mainBrowser),
	             undoHistory (),
	                modified (false),
	           saveConfirmed (false),
	            fileMonitors ()
{
	addChildObjects (mainBrowser);

	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_initialized, this);
	mainBrowser -> shutdown ()    .addInterest (&X3DNotebookPage::set_shutdown,    this);
	mainBrowser -> setNotifyOnLoad (true);
	mainBrowser -> isStrict (false);

	if (not url .empty ())
	{
		mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_browser, this);
		mainBrowser -> set_opacity (0);
	}
}

void
X3DNotebookPage::initialize ()
{
	X3DNotebookPageInterface::initialize ();
}

int32_t
X3DNotebookPage::getPageNumber () const
{
	return getBrowserWindow () -> getBrowserNotebook () .page_num (getWidget ());
}

const basic::uri &
X3DNotebookPage::getSceneURL () const
{
	if (mainBrowser -> isInitialized ())
	   return mainBrowser -> getExecutionContext () -> getMasterScene () -> getWorldURL ();

	return url;
}

const basic::uri &
X3DNotebookPage::getWorldURL () const
{
	if (mainBrowser -> isInitialized ())
	   return mainBrowser -> getWorldURL ();

	return url;
}

void
X3DNotebookPage::setModified (const bool value)
{
	modified = value;
}

bool
X3DNotebookPage::getModified () const
{
	return undoHistory .getModified () or modified;
}

void
X3DNotebookPage::addFileMonitor (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::FileMonitor> & fileMonitor)
{
	fileMonitors .emplace_back (file, fileMonitor);
}

void
X3DNotebookPage::reset ()
{
	// Reset.

	undoHistory .clear ();

	modified      = false;
	saveConfirmed = false;

	for (const auto & fileMonitor : fileMonitors)
	{
		fileMonitor .second -> cancel ();
		fileMonitor .first -> remove ();
	}

	fileMonitors .clear ();
}

void
X3DNotebookPage::shutdown ()
{
	// Reset.

	reset ();
}

void
X3DNotebookPage::set_browser ()
{
	mainBrowser -> initialized () .removeInterest (&X3DNotebookPage::set_browser, this);
	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_splashScreen, this);
	mainBrowser -> loadURL ({ get_page ("about/splash.x3dv") .str () }, { });
}

void
X3DNotebookPage::set_splashScreen ()
{
	mainBrowser -> initialized () .removeInterest (&X3DNotebookPage::set_splashScreen, this);
	mainBrowser -> set_opacity (1);
	mainBrowser -> loadURL ({ url .str () }, { });
}

void
X3DNotebookPage::set_initialized ()
{
	initialized ();
}

void
X3DNotebookPage::set_shutdown ()
{
	shutdown ();
}

void
X3DNotebookPage::on_map ()
{
	mainBrowser -> beginUpdate ();
}

void
X3DNotebookPage::on_unmap ()
{
	mainBrowser -> endUpdate ();
}

X3DNotebookPage::~X3DNotebookPage ()
{ }

} // puck
} // titania
