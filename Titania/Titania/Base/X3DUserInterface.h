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

#ifndef __TITANIA_BASE_X3DUSER_INTERFACE_H__
#define __TITANIA_BASE_X3DUSER_INTERFACE_H__

#include "../Base/X3DBaseInterface.h"
#include "../Configuration/Configuration.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DUserInterface :
	virtual public X3DBaseInterface
{
public:

	///  @name Widget members

	virtual
	const std::string &
	getWidgetName () const = 0;

	virtual
	Gtk::Window &
	getWindow () const = 0;

	virtual
	Gtk::Widget &
	getWidget () const = 0;

	///  @name Operations

	void
	reparent (Gtk::Widget &, Gtk::Window &);

	void
	toggleWidget (Gtk::Widget &, bool);

	///  @name Destruction

	void
	saveSession (const std::string &);

	virtual
	void
	saveSession ();

	virtual
	~X3DUserInterface ();


protected:

	/// @name Construction

	X3DUserInterface (const std::string &, const std::string &);

	void
	construct ();

	virtual
	void
	initialize ()
	{ }

	bool
	isInitialized () const
	{ return not constructed_connection .connected (); }

	/// @name Member access
	
	bool
	isMaximized () const
	{ return getConfig () .getBoolean ("maximized"); }

	bool
	isFullscreen () const
	{ return getConfig () .getBoolean ("fullscreen"); }

	Configuration &
	getConfig ()
	{ return gconf; }

	const Configuration &
	getConfig () const
	{ return gconf; }

	/// @name Dialog handling
	
	bool
	isDialogOpen (const std::string &) const;

	void
	addDialog (const std::string &, const std::shared_ptr <X3DUserInterface> &);

	/// @name Destruction

	virtual
	bool
	close ();


private:

	typedef std::list <X3DUserInterface*> UserInterfaceArray;

	///  @name Construction

	X3DUserInterface (const X3DUserInterface &) = delete;

	///  @name Event handlers

	void
	set_constructed ();

	void
	on_map ();
	
	bool
	on_window_state_event (GdkEventWindowState*);

	bool
	on_delete_event (GdkEventAny*);

	///  @name Operations

	void
	removeDialog (const std::string &);

	void
	restoreInterface ();

	void
	saveInterfaces ();

	void
	saveInterface ();

	///  @name Static members

	static UserInterfaceArray userInterfaces;

	///  @name Members

	Configuration                 gconf;
	sigc::connection              constructed_connection;
	UserInterfaceArray::iterator  userInterface;

	std::map <std::string, std::shared_ptr <X3DUserInterface>> dialogs;


};

} // puck
} // titania

#endif
