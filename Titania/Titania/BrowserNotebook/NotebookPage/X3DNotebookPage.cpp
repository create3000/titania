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

#include "../../Browser/IconFactory.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"

#include "../BrowserView/BrowserView.h"

#include <Titania/String.h>

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

	mainBrowser -> isStrict (false);

	if (not url .empty ())
	{
		mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_browser, this);
		mainBrowser -> set_opacity (0);
	}

	undoHistory .addInterest (&X3DNotebookPage::updateTitle, this);
}

void
X3DNotebookPage::initialize ()
{
	X3DNotebookPageInterface::initialize ();

	getBox1 () .add (*getMainBrowser ());
	getBox1 () .show_all ();
}

int32_t
X3DNotebookPage::getPageNumber () const
{
	return getBrowserWindow () -> getBrowserNotebook () .page_num (getWidget ());
}

X3D::X3DScenePtr
X3DNotebookPage::getMasterScene () const
{
	return X3D::X3DScenePtr (mainBrowser -> getExecutionContext () -> getMasterScene ());
}

X3D::X3DScenePtr
X3DNotebookPage::getScene () const
{
	if (mainBrowser -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DScene }))
		return X3D::X3DScenePtr (mainBrowser -> getExecutionContext ());

	return X3D::X3DScenePtr (mainBrowser -> getExecutionContext () -> getScene ());
}

const basic::uri &
X3DNotebookPage::getMasterSceneURL () const
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

	setSaveConfirmed (false);

	if (not value)
		getUndoHistory () .setSaved ();

	updateTitle ();
}

bool
X3DNotebookPage::getModified () const
{
	return undoHistory .getModified () or modified;
}

bool
X3DNotebookPage::isSaved ()
{
	if (getSaveConfirmed ())
		return true;

	if (getModified ())
	{
		const auto pageNumber = getPageNumber ();

		if (pageNumber < 0)
			return true;

		getBrowserWindow () -> getBrowserNotebook () .set_current_page (pageNumber);

		const auto responseId = std::dynamic_pointer_cast <FileSaveWarningDialog> (createDialog ("FileSaveWarningDialog")) -> run ();

		switch (responseId)
		{
			case Gtk::RESPONSE_OK:
			{
				getBrowserWindow () -> on_save_activated ();
				setSaveConfirmed (not getModified ());
				return getSaveConfirmed ();
			}
			case Gtk::RESPONSE_CANCEL:
			case Gtk::RESPONSE_DELETE_EVENT:
			{
				return false;
			}
			default:
			{
				setSaveConfirmed (true);
				return true;
			}
		}
	}

	return true;
}

void
X3DNotebookPage::updateTitle ()
{
	const bool modified  = getModified ();
	const auto title     = getTitle ();
	const auto protoPath = getProtoPath (mainBrowser -> getExecutionContext ());

	getBrowserWindow () -> getBrowserNotebook () .set_menu_label_text (getWidget (), title);

	getTabImage () .set (Gtk::StockID (getMasterSceneURL () .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	getTabLabel () .set_text (title);
	getTabLabel () .set_tooltip_text (title);

	if (mainBrowser -> getExecutionContext () == getCurrentContext ())
	{
		getWindow () .set_title (mainBrowser -> getExecutionContext () -> getTitle ()
		                         + " · "
		                         + mainBrowser -> getExecutionContext () -> getWorldURL () .filename ()
		                         + (modified ? "*" : "")
		                         + (protoPath .empty () ? "" : " · " + basic::join (protoPath .begin (), protoPath .end (), "."))
		                         + " · "
		                         + mainBrowser -> getName ());
	}
}

std::string
X3DNotebookPage::getTitle () const
{
	const bool modified = getModified ();

	auto title = mainBrowser -> getExecutionContext () -> getTitle ();

	if (title .empty ())
		title = getWorldURL () .basename ();

	if (title .empty ())
		title = _ ("New Scene");

	if (modified)
		title += "*";

	return title;
}

void
X3DNotebookPage::addFileMonitor (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::FileMonitor> & fileMonitor)
{
	fileMonitors .emplace_back (file, fileMonitor);
}

void
X3DNotebookPage::loaded ()
{ }

void
X3DNotebookPage::initialized ()
{
	getBrowserWindow () -> getIconFactory () -> createIcon (getScene ());

	if (getMasterSceneURL () == get_page ("about/new.x3dv"))
	{
	   getCurrentScene () -> setWorldURL ("");
		getCurrentScene () -> setEncoding (X3D::EncodingType::XML);
		getCurrentScene () -> setSpecificationVersion (X3D::LATEST_VERSION);
		getCurrentScene () -> removeMetaData ("comment");
		getCurrentScene () -> removeMetaData ("created");
		getCurrentScene () -> removeMetaData ("creator");
		getCurrentScene () -> removeMetaData ("generator");
		getCurrentScene () -> removeMetaData ("identifier");
		getCurrentScene () -> removeMetaData ("modified");
	}

	updateTitle ();
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
__LOG__ << std::endl;
	mainBrowser -> initialized () .removeInterest (&X3DNotebookPage::set_browser, this);
	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_splashScreen, this);
	mainBrowser -> loadURL ({ get_page ("about/splash.x3dv") .str () }, { });
}

void
X3DNotebookPage::set_splashScreen ()
{
__LOG__ << std::endl;
	mainBrowser -> initialized ()     .removeInterest (&X3DNotebookPage::set_splashScreen, this);
	mainBrowser -> initialized ()     .addInterest (&X3DNotebookPage::set_loaded,          this);
	mainBrowser -> shutdown ()        .addInterest (&X3DNotebookPage::set_shutdown,        this);
	mainBrowser -> getLoadingTotal () .addInterest (&X3DNotebookPage::set_loadCount,       this);
	mainBrowser -> getLoadCount ()    .addInterest (&X3DNotebookPage::set_loadCount,       this);

	mainBrowser -> set_opacity (1);
	mainBrowser -> setNotifyOnLoad (true);
	mainBrowser -> loadURL ({ url .str () }, { });
}

void
X3DNotebookPage::set_loaded ()
{
__LOG__ << std::endl;
	mainBrowser -> initialized () .removeInterest (&X3DNotebookPage::set_loaded, this);
	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_initialized, this);

	loaded ();
	initialized ();
}

void
X3DNotebookPage::set_initialized ()
{
__LOG__ << std::endl;
	initialized ();
}

void
X3DNotebookPage::set_loadCount ()
{
	const auto fraction = double (mainBrowser -> getLoadingTotal () - mainBrowser -> getLoadCount ()) / double (mainBrowser -> getLoadingTotal ());

	getBrowserWindow () -> getLocationEntry () .set_progress_fraction (fraction);
}

void
X3DNotebookPage::set_shutdown ()
{
	shutdown ();
}

X3DNotebookPage::~X3DNotebookPage ()
{ }

} // puck
} // titania
