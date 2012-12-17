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
 ******************************************************************************/

#include "X3DBrowserUserInterface.h"

#include "../Configuration/config.h"

namespace titania {
namespace puck {

X3DBrowserUserInterface::X3DBrowserUserInterface (const std::string & sessionKey, X3DBrowserInterface* const browserWidget) :
	 X3DBrowserWidgetUI (get_ui ("BrowserWidget.ui"), sessionKey), 
	X3DBrowserInterface (X3D::createBrowser ()),                   
	            surface (getBrowser ()),                           
	    viewpointEditor (sessionKey, this),                        
	      historyEditor (sessionKey, this),                        
	      outlineEditor (sessionKey, this)                         
{
	setBrowserWidget (browserWidget);
}

void
X3DBrowserUserInterface::initialize ()
{
	X3DBrowserWidgetUI::initialize ();

	// Add IconFactory to the list of icon factories searched by Gtk::Style::lookup_icon_set ().
	getIconFactory () -> add_default ();

	// ViewpointEditor
	getViewpointEditor () .reparent (getViewpointEditorBox (), getWindow ());

	// HistoryEditor
	getHistoryEditor () .reparent (getHistoryEditorBox (), getWindow ());

	// OutlineEditor
	getOutlineEditor () .reparent (getOutlineEditorBox (), getWindow ());

	// Restore Session.
	restoreSession ();
}

void
X3DBrowserUserInterface::restoreSession ()
{
	// VPaned
	if (getConfig () .integer ("vPaned"))
		getVPaned () .set_position (getConfig () .integer ("vPaned"));

	// HPaned
	if (getConfig () .integer ("hPaned"))
		getHPaned () .set_position (getConfig () .integer ("hPaned"));

	getSideBarNotebook () .set_current_page (getConfig () .integer ("sidebarCurrentPage"));
	getFooterNotebook ()  .set_current_page (getConfig () .integer ("footerCurrentPage"));
}

void
X3DBrowserUserInterface::saveSession ()
{
	// Prevent saving when page not initialized.
	if (not is_initialized ())
		return;

	//	printStatistics ();

	if (getExecutionContext () -> getWorldURL () .size ())
		getConfig () .setItem ("worldURL", getExecutionContext () -> getWorldURL ());

	getConfig () .setItem ("vPaned", getVPaned () .get_position ());
	getConfig () .setItem ("hPaned", getHPaned () .get_position ());

	getConfig () .setItem ("sidebarCurrentPage", getSideBarNotebook () .get_current_page ());
	getConfig () .setItem ("footerCurrentPage",  getFooterNotebook  () .get_current_page ());
}

void
X3DBrowserUserInterface::close ()
{
	//saveSession ()
}

// Destructor

X3DBrowserUserInterface::~X3DBrowserUserInterface ()
{ }

} // puck
} // titania
