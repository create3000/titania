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

#include "BackgroundImageEditor.h"

#include "BackgroundImage.h"

#include "../../Bits/String.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Texturing/ImageTexture.h>

namespace titania {
namespace puck {

BackgroundImageEditor::BackgroundImageEditor (X3DBrowserWindow* const browserWindow) :
	                 X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DBackgroundImageEditorInterface (get_ui ("Editors/BackgroundImageEditor.glade")),
	                             page (),
	                         changing (false)
{
	setup ();
}

void
BackgroundImageEditor::initialize ()
{
	X3DBackgroundImageEditorInterface::initialize ();

	getCurrentScene () .addInterest (&BackgroundImageEditor::set_scene, this);

	set_scene ();
}

void
BackgroundImageEditor::set_scene ()
{
	if (page)
	{
		page -> getBackgroundImage () -> removeInterest (&BackgroundImageEditor::set_background_image, this);
		page -> getBackgroundImage () -> getTexture () -> checkLoadState () .removeInterest (&BackgroundImageEditor::set_loadState, this);
	}

	page = getBrowserWindow () -> getCurrentPage ();

	page -> getBackgroundImage () -> addInterest (&BackgroundImageEditor::set_background_image, this);
	page -> getBackgroundImage () -> getTexture () -> checkLoadState () .addInterest (&BackgroundImageEditor::set_loadState, this);

	set_background_image ();
	set_loadState ();
}

void
BackgroundImageEditor::set_background_image ()
{
	changing = true;

	if (page -> getBackgroundImage () -> getUrl () .size () < 2)
	{
		getImageChooserButton () .set_uri ("");
		getImageChooserButton () .set_current_folder (Glib::get_home_dir ());
		getImageReloadButton () .set_sensitive (false);
	}
	else
	{
		getImageChooserButton () .set_uri (page -> getBackgroundImage () -> getUrl () [1]);
		getImageReloadButton () .set_sensitive (true);
	}

	getTransparencyAdjustment () -> set_value (page -> getBackgroundImage () -> getTransparency ());

	changing = false;
}

void
BackgroundImageEditor::set_loadState ()
{
	const auto loadState = page -> getBackgroundImage () -> getTexture () -> checkLoadState ();

	getLoadStateLabel () .set_text (loadStateInfo (loadState));

	switch (loadState)
	{
		case X3D::NOT_STARTED_STATE:
		{
			getImageChooserButton () .set_sensitive (true);
			getImageReloadButton ()  .set_sensitive (not getImageChooserButton ()  .get_file () -> get_path () .empty ());
			getImageRemoveButton ()  .set_sensitive (true);
			break;
		}
		case X3D::IN_PROGRESS_STATE:
		{
			getImageChooserButton () .set_sensitive (false);
			getImageReloadButton ()  .set_sensitive (false);
			getImageRemoveButton ()  .set_sensitive (false);
			break;
		}
		case X3D::COMPLETE_STATE:
		{
			getImageChooserButton () .set_sensitive (true);
			getImageReloadButton ()  .set_sensitive (true);
			getImageRemoveButton ()  .set_sensitive (true);
			break;
		}
		case X3D::FAILED_STATE:
		{
			getImageChooserButton () .set_sensitive (true);
			getImageReloadButton ()  .set_sensitive (false);
			getImageRemoveButton ()  .set_sensitive (true);
			break;
		}
	}
}

void
BackgroundImageEditor::on_image_set ()
{
	if (changing)
		return;

	const basic::uri path        = getImageChooserButton () .get_file () -> get_path ();
	const basic::uri URL         = path .add_file_scheme ();
	const basic::uri relativeURL = getCurrentContext () -> getWorldURL () .relative_path (URL);
	const auto       url         = X3D::MFString ({ relativeURL .str (), URL .str () });

	if (path .empty ())
		return;

	page -> getBackgroundImage () -> setUrl (url);
}

void
BackgroundImageEditor::on_image_reload_clicked ()
{
	on_image_set ();
}

void
BackgroundImageEditor::on_image_remove_clicked ()
{
	// Widgets

	getImageChooserButton () .set_uri ("");
	getImageChooserButton () .set_current_folder (Glib::get_home_dir ());
	getImageReloadButton () .set_sensitive (false);

	// Image

	page -> getBackgroundImage () -> setUrl ({});
}

void
BackgroundImageEditor::on_transparency_changed ()
{
	if (changing)
		return;

	page -> getBackgroundImage () -> setTransparency (getTransparencyAdjustment () -> get_value ());
}

BackgroundImageEditor::~BackgroundImageEditor ()
{
	dispose ();
}

} // puck
} // titania
