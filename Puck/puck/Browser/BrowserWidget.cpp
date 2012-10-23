/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "BrowserWidget.h"

namespace titania {
namespace puck {

BrowserWidget::BrowserWidget (const std::string & sessionKey, X3DBrowserInterface* const browserWidget) :
	X3DBrowserWidget (sessionKey, browserWidget)
{ }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::initialize ()
{
	X3DBrowserWidget::initialize ();
}

void
BrowserWidget::on_map ()
{
	std::clog << "Signal map received for '" << getWorldURL () << "'." << std::endl;

	getBrowser () -> beginUpdate ();
}

void
BrowserWidget::on_unmap ()
{
	std::clog << "Signal unmap received for '" << getWorldURL () << "'." << std::endl;

	getBrowser () -> endUpdate ();
}

// File ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_new ()
{
	blank ();
}

void
BrowserWidget::on_home ()
{
	home ();
}

void
BrowserWidget::on_open ()
{
	getFileOpenDialog () .present ();
}

void
BrowserWidget::on_save ()
{
	if (getWorldURL () .empty ())
		getFileSaveDialog () .present ();
	else
		save (getWorldURL ());
}

void
BrowserWidget::on_save_as ()
{
	getFileSaveDialog () .present ();
}

void
BrowserWidget::on_close ()
{ }


void
BrowserWidget::on_revert_to_saved ()
{
	loadURL ({ getWorldURL () });
}

void
BrowserWidget::on_reload ()
{
	loadURL ({ getWorldURL () });
}

// Dialog response handling ////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_fileOpenDialog_response (int response_id)
{
	getFileOpenDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		loadURL ({ getFileOpenDialog () .get_uri () });

	else
		getFileOpenDialog () .set_current_folder_uri (getWorldURL () .base () .str ());
}

void
BrowserWidget::on_fileSaveDialog_response (int response_id)
{
	getFileSaveDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
		save (getFileSaveDialog () .get_filename ());

	else
		getFileSaveDialog () .set_current_folder_uri (getWorldURL () .base () .str ());
}

void
BrowserWidget::messageDialogResponse (int response_id)
{
	getMessageDialog () .hide ();
}

// View ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_toolBar_toggled ()
{
	toggleWidget (getToolBarMenuItem () .get_active (), getToolbar ());
}

void
BrowserWidget::on_navigationBar_toggled ()
{
	toggleWidget (getNavigationBarMenuItem () .get_active (), getNavigationBar ());
}

void
BrowserWidget::on_sideBar_toggled ()
{
	toggleWidget (getSideBarAction () -> get_active (), getSideBar ());
}

void
BrowserWidget::on_footer_toggled ()
{
	toggleWidget (getFooterAction () -> get_active (), getFooter ());
}

void
BrowserWidget::on_statusBar_toggled ()
{
	toggleWidget (getStatusBarMenuItem () .get_active (), getStatusBar ());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::phong_activate ()
{
//	if (not getPhongMenuItem () .get_active ())
//		return;
//
//	getBrowser () -> getBrowserOptions () -> shading = "PHONG";
}

void
BrowserWidget::gouraud_activate ()
{
	if (not getGouraudMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading = "GOURAUD";
}

void
BrowserWidget::flat_activate ()
{
//	if (not getFlatMenuItem () .get_active ())
//		return;
//
//	getBrowser () -> getBrowserOptions () -> shading = "FLAT";
}

void
BrowserWidget::wireframe_activate ()
{
	if (not getWireFrameMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading = "WIREFRAME";
}

void
BrowserWidget::pointset_activate ()
{
	if (not getPointSetMenuItem () .get_active ())
		return;

	getBrowser () -> getBrowserOptions () -> shading = "POINTSET";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_low_quality_activate ()
{
	if (not getLowQualityMenuItem () .get_active ())
		return;

	std::clog << "Setting render quality to low." << std::endl;

	getBrowser () -> getBrowserOptions () -> quadSphere -> uDimension = 20;
	getBrowser () -> getBrowserOptions () -> quadSphere -> vDimension = 10;
}

void
BrowserWidget::on_medium_quality_activate ()
{
	if (not getMediumQualityMenuItem () .get_active ())
		return;

	std::clog << "Setting render quality to medium." << std::endl;

	getBrowser () -> getBrowserOptions () -> quadSphere -> uDimension = 40;
	getBrowser () -> getBrowserOptions () -> quadSphere -> vDimension = 20;
}

void
BrowserWidget::on_high_quality_activate ()
{
	if (not getHighQualityMenuItem () .get_active ())
		return;

	std::clog << "Setting render quality to high." << std::endl;

	getBrowser () -> getBrowserOptions () -> quadSphere -> uDimension = 60;
	getBrowser () -> getBrowserOptions () -> quadSphere -> vDimension = 30;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_rendering_properties_toggled ()
{
	getBrowser () -> getRenderingProperties () -> enabled = getRenderingPropertiesMenuItem () .get_active ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_fullscreen_toggled ()
{
	if (getFullScreenMenuItem () .get_active ())
		getWindow () .fullscreen ();

	else
		getWindow () .unfullscreen ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_headlight_toggled ()
{
	const X3D::SFNode <X3D::NavigationInfo> & navigationInfo = getBrowser () -> getActiveNavigationInfo ();

	navigationInfo -> headlight = getHeadlightMenuItem () .get_active ();
}

void
BrowserWidget::on_show_all_toggled ()
{
	getBrowser () -> getExecutionContext () -> getActiveLayer () -> showAllObjects ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_outline_editor_activate ()
{
	getOutlineEditor () .getWindow () .present ();
}

void
BrowserWidget::on_viewpoint_editor_activate ()
{
	getViewpointEditor () .getWindow () .present ();
}

void
BrowserWidget::on_motion_blur_editor_activate ()
{
	getMotionBlurEditor () .getWindow () .present ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_info ()
{
	loadURL ({ "about:info" });
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
BrowserWidget::on_locationEntry_activate ()
{
	basic::uri worldURL = getLocationEntry () .get_text () .raw ();

	if (worldURL .is_absolute () and worldURL .scheme () .empty ())
		worldURL = basic::uri ("file://", worldURL);

	loadURL ({ worldURL });
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
					getFileOpenDialog () .present ();
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
