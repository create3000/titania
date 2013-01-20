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

#include "BrowserWidget.h"

namespace titania {
namespace puck {

BrowserWidget::BrowserWidget (const std::string & sessionKey, X3DBrowserInterface* const browserWidget) :
	X3DBrowserWidget (sessionKey, browserWidget) 
{ }

// Init

void
BrowserWidget::initialize ()
{
	X3DBrowserWidget::initialize ();
}

void
BrowserWidget::on_map ()
{
	std::clog << "Signal map received for '" << getExecutionContext () -> getWorldURL () << "'." << std::endl;

	getBrowser () -> beginUpdate ();
}

void
BrowserWidget::on_unmap ()
{
	std::clog << "Signal unmap received for '" << getExecutionContext () -> getWorldURL () << "'." << std::endl;

	getBrowser () -> endUpdate ();
}

// Dialog response handling

void
BrowserWidget::messageDialogResponse (int response_id)
{
	getMessageDialog () .hide ();
}

// Bar view handling

void
BrowserWidget::on_footer_toggled ()
{ }

void
BrowserWidget::on_sideBar_toggled ()
{ }

// Toolbar handling

void
BrowserWidget::on_home ()
{
	home ();
}

void
BrowserWidget::on_reload ()
{
	reload ();
}

// Browser toolbar handling

void
BrowserWidget::on_arrow_button_toggled ()
{
	if (getArrowButton () .get_active ())
	{
		std::clog << "Arrow button clicked." << std::endl;
	}

	//	getBrowser () -> setEditMode (true);
	//	getStatusBar () .push ("Edit mode");
}

void
BrowserWidget::on_hand_button_toggled ()
{
	if (getHandButton () .get_active ())
	{
		std::clog << "Hand button clicked." << std::endl;
	}

	//	getBrowser () -> setEditMode (false);
	//	getStatusBar () .push ("Browser mode");
}

void
BrowserWidget::on_look_at_all_clicked ()
{
	getBrowser () -> getExecutionContext () -> getActiveLayer () -> lookAt ();
}

void
BrowserWidget::on_look_at_toggled ()
{
	__LOG__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_locationEntry_activate ()
{
	basic::uri worldURL = getLocationEntry () .get_text () .raw ();

	if (worldURL .is_absolute () and worldURL .scheme () .empty ())
		worldURL = basic::uri ("file://", worldURL);

	loadURL ({ worldURL .str () });
}

void
BrowserWidget::on_locationEntryIcon_activate (Gtk::EntryIconPosition icon_position, const GdkEventButton* event)
{
	switch (icon_position)
	{
		case ENTRY_ICON_PRIMARY:
		{
			std::clog << "Primary WorldURL Button: " << event -> button << std::endl;
			break;
		}
		case ENTRY_ICON_SECONDARY:
		{
			std::clog << "Secondary WorldURL Button: " << event -> button << std::endl;

			switch (event -> button)
			{
				case 1: // Left Button
				{
					//getFileOpenDialog () .present ();
					break;
				}
			}

			break;
		}
	}
}

// Destructor

BrowserWidget::~BrowserWidget ()
{ }

} // puck
} // titania
