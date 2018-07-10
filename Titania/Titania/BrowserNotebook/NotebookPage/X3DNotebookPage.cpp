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
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"
#include "../../Editors/BackgroundImageEditor/BackgroundImage.h"

#include "../BrowserPanel/BrowserPanel.h"

#include <Titania/String.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>

namespace titania {
namespace puck {

X3DNotebookPage::X3DNotebookPage (const basic::uri & startUrl) :
	X3DNotebookPageInterface (get_ui ("Widgets/NotebookPage.glade")),
	             mainBrowser (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser ())),
	             masterScene (mainBrowser -> getExecutionContext ()),
	                   scene (mainBrowser -> getExecutionContext ()),
	        executionContext (mainBrowser -> getExecutionContext ()),
	                     url (startUrl),
	             undoHistory (),
	                modified (false),
	           saveConfirmed (false),
	               savedTime (-1),
	       focusInConnection (),
	    switchPageConnection (),
	         backgroundImage (new BackgroundImage (this)),
	                changing (false)
{
	addChildObjects (mainBrowser, masterScene, scene, executionContext);

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

	mainBrowser -> getSoundSources () .addInterest (&X3DNotebookPage::set_soundSources, this);
	mainBrowser -> getMute ()         .addInterest (&X3DNotebookPage::set_mute,         this);

	getScene ()            .addInterest (this, &X3DNotebookPage::set_scene);
	getExecutionContext () .addInterest (this, &X3DNotebookPage::set_executionContext);

	getBox1 () .remove ();
	getBox1 () .add (*mainBrowser);
	getBox1 () .show_all ();

	set_soundSources ();
	set_mute ();
}

int32_t
X3DNotebookPage::getPageNumber () const
{
	return getBrowserWindow () -> getBrowserNotebook () .page_num (getWidget ());
}

const basic::uri &
X3DNotebookPage::getMasterSceneURL () const
{
	const auto & worldURL = getMasterScene () -> getWorldURL ();

	if (mainBrowser -> isInitialized () and not worldURL .empty ())
	   return worldURL;

	return url;
}

const basic::uri &
X3DNotebookPage::getWorldURL () const
{
	const auto & worldURL = mainBrowser -> getWorldURL ();

	if (mainBrowser -> isInitialized () and not worldURL .empty ())
	   return worldURL;

	return url;
}

void
X3DNotebookPage::setModified (const bool value)
{
	modified = value;

	if (value)
	{
		setSaveConfirmed (false);
	}
	else
	{
		setSavedTime (getScene () -> getWorldURL ());
		getUndoHistory () .setSaved ();
	}

	updateTitle ();
}

bool
X3DNotebookPage::getModified () const
{
	return undoHistory .getModified () or modified;
}

void
X3DNotebookPage::setSavedTime (const basic::uri & url)
{
	try
	{
		const auto file     = Gio::File::create_for_uri (getScene () -> getWorldURL ());
		const auto fileInfo = file -> query_info ();
	
		savedTime = fileInfo -> modification_time () .as_double ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
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

		const auto responseId = createDialog <FileSaveWarningDialog> ("FileSaveWarningDialog") -> run ();

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
	const auto title     = getTitle ();
	const auto protoPath = getProtoPath (mainBrowser -> getExecutionContext ());

	getBrowserWindow () -> getBrowserNotebook () .set_menu_label_text (getWidget (), title);

	getTabImage () .set (Gtk::StockID (getMasterSceneURL () .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	getTabLabel () .set_text (title);
	getTabLabel () .set_tooltip_text (getMasterSceneURL () .str ());

	if (mainBrowser -> getExecutionContext () == getCurrentContext ())
	{
		getBrowserWindow () -> getHeaderBar () .set_title (title
		                                                   + (protoPath .empty () ? "" : " · " + basic::join (protoPath .begin (), protoPath .end (), "."))
		                                                   + " · "
		                                                   + mainBrowser -> getName ());

		getBrowserWindow () -> getHeaderBar () .set_subtitle (mainBrowser -> getExecutionContext () -> getWorldURL () .filename () .str ());
	}
}

std::string
X3DNotebookPage::getTitle () const
{
	auto title = getWorldURL () .basename ();

	if (title .empty () or getScene () -> getWorldURL () .empty ())
		title = _ ("New Scene");

	if (getModified ())
		title += "*";

	return title;
}

void
X3DNotebookPage::loaded ()
{ }

void
X3DNotebookPage::reset ()
{
	// Reset.

	undoHistory .clear ();

	setModified (false);
	setSaveConfirmed (false);
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
	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_loaded,          this);
	mainBrowser -> initialized () .addInterest (&X3DNotebookPage::set_initialized,     this);

	mainBrowser -> set_opacity (1);
	mainBrowser -> setNotifyOnLoad (true);
	mainBrowser -> setMonitorFiles (true);
	mainBrowser -> loadURL ({ url .str () }, { });
}

void
X3DNotebookPage::set_loaded ()
{
	mainBrowser -> initialized () .removeInterest (&X3DNotebookPage::set_loaded, this);

	getBox1 () .set_visible (false);
	getBox1 () .remove ();

	loaded ();
}

void
X3DNotebookPage::set_initialized ()
{
	const auto &     currentMaster  = mainBrowser -> getExecutionContext () -> getMasterScene ();
	X3D::X3DScenePtr currentScene   = nullptr;
	const auto &     currentContext = mainBrowser -> getExecutionContext ();

	// Update scene.

	if (currentContext -> isType ({ X3D::X3DConstants::X3DScene }))
		currentScene = currentContext;
	else
		currentScene = currentContext -> getScene ();

	if (currentScene not_eq scene)
	{
		// Scene of this page has changed.

		if (isSaved ())
		{
			// Scene is now saved to file or changes are discarded.

			scene -> file_changed () .removeInterest (&X3DNotebookPage::set_file_changed, this);

			scene = currentScene;

			scene -> file_changed () .addInterest (&X3DNotebookPage::set_file_changed, this);

			reset ();
		}
		else
		{
			// Restore old execution context.
			mainBrowser -> replaceWorld (executionContext);
			return;
		}
	}

	// Update masterScene.

	if (currentMaster not_eq masterScene)
		masterScene = currentMaster;

	// Update executionContext.

	if (currentContext not_eq executionContext)
		executionContext = currentContext;
}

void
X3DNotebookPage::set_scene ()
{
	getBrowserWindow () -> getIconFactory () -> createIcon (getScene ());

	if (getScene () -> getWorldURL () == get_page ("about/new.x3dv"))
	{
		url = "";

		getScene () -> setWorldURL ("");
		getScene () -> setEncoding (X3D::EncodingType::XML);
		getScene () -> setSpecificationVersion (X3D::LATEST_VERSION);
		getScene () -> removeMetaData ("comment");
		getScene () -> removeMetaData ("created");
		getScene () -> removeMetaData ("creator");
		getScene () -> removeMetaData ("generator");
		getScene () -> removeMetaData ("identifier");
		getScene () -> removeMetaData ("modified");
	}

	if (url .scheme () == "data" and
	    getScene () -> getWorldURL () == get_page ("about/splash.x3dv"))
	{
		url = "";

		getScene () -> setWorldURL ("");
	}
}

void
X3DNotebookPage::set_executionContext ()
{
	updateTitle ();
}

void
X3DNotebookPage::set_file_changed (const X3D::time_type & modificationTime)
{
	if (modificationTime == getSavedTime ())
		return;

	if (getBrowserWindow () -> getBrowserNotebook () .get_current_page () not_eq getPageNumber ())
	{
		switchPageConnection .disconnect ();
		switchPageConnection = getBrowserWindow () -> getBrowserNotebook () .signal_switch_page () .connect (sigc::mem_fun (this, &X3DNotebookPage::on_notebook_switch_page));
	}
	else if (getModified () and not getBrowserWindow () -> getWindow () .has_focus ())
	{
		focusInConnection .disconnect ();
		focusInConnection = getBrowserWindow () -> getWindow () .signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DNotebookPage::on_window_focus_in_event));
	}
	else
	{
		on_file_changed ();
	}
}

bool
X3DNotebookPage::on_window_focus_in_event (GdkEventFocus* event)
{
	on_file_changed ();
	return false;
}

void
X3DNotebookPage::on_notebook_switch_page (Gtk::Widget*, guint pageNumber)
{
	if (int32_t (pageNumber) == getPageNumber ())
		on_file_changed ();
}

void
X3DNotebookPage::on_file_changed ()
{
	focusInConnection    .disconnect ();
	switchPageConnection .disconnect ();

	if (getModified ())
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");
	
		dialog -> setType (Gtk::MESSAGE_QUESTION);
		dialog -> setMessage (_ ("File modified externally!"));
		dialog -> setText (_ (basic::sprintf ("»%s« has been modified by another program. Reload and discard changes?", getScene () -> getWorldURL () .basename () .c_str ())));
		dialog -> getOkButton () .set_label ("gtk-refresh");
	
		if (dialog -> run () not_eq Gtk::RESPONSE_OK)
			return;
	}

	// Reload

	reset ();

	mainBrowser -> loadURL ({ getScene () -> getWorldURL () .str () }, { });
}

void
X3DNotebookPage::set_soundSources ()
{
	getMuteButton () .set_visible (mainBrowser -> getSoundSources () .size ());
}

void
X3DNotebookPage::set_mute ()
{
	changing = true;

	getMuteButton () .set_active (mainBrowser -> getMute ());

	if (mainBrowser -> getMute ())
		getMuteImage () .set (Gtk::StockID ("audio-volume-muted"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	else
		getMuteImage () .set (Gtk::StockID ("audio-volume-high"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	changing = false;
}

void
X3DNotebookPage::on_mute_toggled ()
{
	if (changing)
		return;

	mainBrowser -> setMute (getMuteButton () .get_active ());
}

void
X3DNotebookPage::dispose ()
{
	backgroundImage .reset ();

	X3DNotebookPageInterface::dispose ();
}

X3DNotebookPage::~X3DNotebookPage ()
{ }

} // puck
} // titania
