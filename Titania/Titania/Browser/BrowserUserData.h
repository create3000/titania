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

#ifndef __TITANIA_BROWSER_BROWSER_USER_DATA_H__
#define __TITANIA_BROWSER_BROWSER_USER_DATA_H__

#include "../Browser/BrowserHistory.h"

#include <Titania/X3D/Base/X3DBase.h>
#include <Titania/X3D/Editing/Undo/UndoHistory.h>
#include <gtkmm.h>

namespace titania {
namespace puck {

class BrowserUserData :
	public X3D::X3DBase
{
public:

	///  @name Construction

	BrowserUserData (X3D::X3DBrowser* const browser) :
		           URL (),
		          icon (nullptr),
		         label (nullptr),
		browserHistory (browser),
		   undoHistory (),
		      modified (false),
		 saveConfirmed (false),
		  fileMonitors ()
	{ }

	///  @name Members

	basic::uri  URL;   // Original URL
	Gtk::Image* icon;  // Favicon
	Gtk::Label* label; // Tab label

	BrowserHistory   browserHistory;
	X3D::UndoHistory undoHistory;
	bool             modified;
	bool             saveConfirmed;

	std::map <Glib::RefPtr <Gio::File>, Glib::RefPtr <Gio::FileMonitor>>  fileMonitors;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~BrowserUserData ()
	{ }

};

inline
void
BrowserUserData::dispose ()
{
	undoHistory  .clear ();

	modified      = false;
	saveConfirmed = false;

	for (const auto & fileMonitor : fileMonitors)
	{
		fileMonitor .second -> cancel ();
		fileMonitor .first -> remove ();
	}

	fileMonitors .clear ();
}

} // puck
} // titania

#endif
