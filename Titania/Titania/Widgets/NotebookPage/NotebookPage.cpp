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

#include "NotebookPage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

NotebookPage::NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl) :
	        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
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

	mainBrowser -> setAntialiasing (4);
	mainBrowser -> setNotifyOnLoad (true);
	mainBrowser -> isStrict (false);
	mainBrowser -> show ();

	const auto container = getWidget () .get_parent ();

	if (container)
	   container -> remove (getWidget ());

	getBox2 () .pack_start (*mainBrowser, true, true, 0);
	getBox2 () .set_visible (true);

	setup ();
}

void
NotebookPage::initialize ()
{
	X3DNotebookPageInterface::initialize ();
}

int32_t
NotebookPage::getPageNumber () const
{
	return getBrowserWindow () -> getBrowserNotebook () .page_num (getWidget ());
}

const basic::uri &
NotebookPage::getWorldURL () const
{
	if (mainBrowser -> isInitialized ())
	   return mainBrowser -> getWorldURL ();

	return url;
}

void
NotebookPage::setModified (const bool value)
{
	modified = value;
}

bool
NotebookPage::getModified () const
{
	return undoHistory .getModified () or modified;
}

void
NotebookPage::addFileMonitor (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::FileMonitor> & fileMonitor)
{
	fileMonitors .emplace_back (file, fileMonitor);
}

void
NotebookPage::reset ()
{
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

NotebookPage::~NotebookPage ()
{
	dispose ();
}

} // puck
} // titania
