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

#ifndef __TITANIA_BROWSER_BROWSER_WIDGET_H__
#define __TITANIA_BROWSER_BROWSER_WIDGET_H__

#include "../Browser/X3DBrowserWidget.h"

#include <gtkmm.h>
#include <iostream>

namespace titania {
namespace puck {

class BrowserWidget :
	public X3DBrowserWidget
{
public:

	BrowserWidget (const std::string &, X3DBrowserInterface* const);

	virtual
	~BrowserWidget ();


private:

	virtual
	void
	initialize ();

	virtual
	void
	on_map ();

	virtual
	void
	on_unmap ();

	/// @name Message dialog response handling

	virtual
	void
	messageDialogResponse (int);
	
	///  @name Toolbar handling

	virtual
	void
	on_home ();
	
	virtual
	void
	on_reload ();

	///  @name Bar view handling

	virtual
	void
	on_footer_toggled ();

	virtual
	void
	on_sideBar_toggled ();

	virtual
	void
	on_look_at_all_clicked ();

	virtual
	void
	on_look_at_toggled ();

	///  @name Browser toolbar handling

	virtual
	void
	on_arrow_button_toggled ();

	virtual
	void
	on_hand_button_toggled ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual
	void
	on_locationEntry_activate ();

	virtual
	void
	on_locationEntryIcon_activate (Gtk::EntryIconPosition, const GdkEventButton*);

};

} // puck
} // titania

#endif
