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

#ifndef __PUCK_USER_INTERFACES_USER_INTERFACE_H__
#define __PUCK_USER_INTERFACES_USER_INTERFACE_H__

#include "../Configuration/Configuration.h"
#include "../Base/X3DBrowserInterface.h"
#include "../Base/X3DBaseInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DUserInterface :
	virtual public X3DBaseInterface
{
public:

	X3DBrowserInterface*
	getBrowserWidget () const;

	virtual
	const std::string &
	getWidgetName () const = 0;

	virtual
	Gtk::Window &
	getWindow () const = 0;

	virtual
	Gtk::Widget &
	getWidget () const = 0;

	void
	reparent (Gtk::Widget &, Gtk::Window &);

	void
	toggleWidget (bool, Gtk::Widget &);

	void
	saveSession (const std::string &);

	virtual
	void
	saveSession ();

protected:

	X3DUserInterface (const std::string &, const std::string &);

	void
	construct ();

	virtual
	void
	initialize ();

	bool
	is_initialized ();

	virtual
	void
	close ();

	void
	setBrowserWidget (X3DBrowserInterface* const);

	Configuration &
	getConfig ()
	{ return gconf; }


private:

	X3DUserInterface (const X3DUserInterface &) = delete;

	void
	on_map ();

	bool
	on_delete_event (GdkEventAny*);

	void
	restoreInterface ();

	void
	saveInterfaces ();

	void
	saveInterface ();

	static std::vector <X3DUserInterface*> userInterfaces;

	bool initialized;

	Configuration gconf;
	
	X3DBrowserInterface* browserWidget;

};

} // puck
} // titania

#endif
