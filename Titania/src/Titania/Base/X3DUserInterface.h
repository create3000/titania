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

#ifndef __TITANIA_BASE_X3DUSER_INTERFACE_H__
#define __TITANIA_BASE_X3DUSER_INTERFACE_H__

#include "../Base/X3DBaseInterface.h"
#include "../Configuration/Configuration.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class DialogFactory;

class X3DUserInterface :
	virtual public X3DBaseInterface
{
public:

	/// @name Construction

	virtual
	void
	setup () override;

	///  @name Member access

	virtual
	void
	setName (const std::string &) final override; // must be final, called in constructor

	virtual
	Gtk::Window &
	getWindow () const = 0;

	virtual
	Gtk::Widget &
	getWidget () const = 0;

	template <class Type>
	static
	std::vector <Type*>
	getWidgets (Gtk::Widget & parent);

	void
	setTransparent (const bool value);

	///  @name Operations

	void
	setTitleBar (Gtk::Window & window, Gtk::Widget & titlebar);

	void
	unparent (Gtk::Widget & widget) const;

	void
	reparent (Gtk::Box & box, Gtk::Window & window);

	void
	reparent (Gtk::Overlay & overlay, Gtk::Window & window);

	virtual
	void
	present ();

	/// @name Dialog handling
	
	bool
	hasDialog (const std::string & name) const;

	std::shared_ptr <X3DUserInterface>
	addDialog (const std::string & name, const bool present = true);

	template <class Type>
	std::shared_ptr <Type>
	addDialog (const std::string & name, const bool present = true);

	std::shared_ptr <X3DUserInterface>
	createDialog (const std::string & name) const;

	template <class Type>
	std::shared_ptr <Type>
	createDialog (const std::string & name) const;

	///  @name Configuration handling

	const std::shared_ptr <Configuration> &
	getConfig () const
	{ return config; }

	/// @name Destruction

	virtual
	bool
	quit ();

	virtual
	void
	dispose () override;

	virtual
	~X3DUserInterface () override;


protected:

	/// @name Construction

	X3DUserInterface ();

	virtual
	void
	configure ()
	{ }

	virtual
	void
	initialize ()
	{ }
	
	virtual
	void
	set_selection (const X3D::MFNode & selection)
	{ }

	/// @name Member access

	bool
	isInitialized () const
	{ return initialized; }
	
	bool
	isMaximized () const
	{ return getConfig () -> getItem <bool> ("maximized"); }

	bool
	isFullscreen () const
	{ return getConfig () -> getItem <bool> ("fullscreen"); }

	/// @name Event handlers

	virtual
	void
	on_maximized (const bool value);

	virtual
	void
	on_fullscreen (const bool value);

	/// @name Destruction

	virtual
	void
	store ();


private:

	using UserInterfaceArray = std::list <X3DUserInterface*>;
	using DialogIndex        = std::map <std::string, std::shared_ptr <X3DUserInterface>>;

	///  @name Construction

	X3DUserInterface (const X3DUserInterface &) = delete;

	void
	connectFocusEvent (Gtk::Widget & parent);

	///  @name Event handlers

	void
	on_initialize ();

	void
	on_map ();

	void
	on_unmap ();

	void
	set_selection_and_check (const X3D::MFNode & selection);

	bool
	on_window_state_event (GdkEventWindowState* event);

	bool
	on_delete_event (GdkEventAny* event);

	bool
	on_focus_in_event (GdkEventFocus* event);

	bool
	on_focus_out_event (GdkEventFocus* event);

	///  @name Operations

	void
	restoreDialogs ();

	void
	removeDialog (const std::string & name);

	void
	removeDialogImpl (const std::string & name);

	void
	restoreInterface ();

	void
	saveInterfaces ();

	void
	saveInterface ();

	template <class Type>
	static
	void
	getWidgets (Gtk::Widget* parent, std::vector <Type*> & types);

	///  @name Static members

	static const std::unique_ptr <DialogFactory> dialogFactory;
	static const std::set <std::string>          restorableDialogs;
	static UserInterfaceArray                    userInterfaces;

	///  @name Members

	std::shared_ptr <Configuration> config;
	sigc::connection                windowStateConnection;
	sigc::connection                mapConnection;
	UserInterfaceArray::iterator    userInterface;
	std::shared_ptr <DialogIndex>   dialogs;
	bool                            initialized;

};

template <class Type>
std::shared_ptr <Type>
X3DUserInterface::addDialog (const std::string & name, const bool present)
{
	return std::dynamic_pointer_cast <Type> (addDialog (name, present));
}

template <class Type>
std::shared_ptr <Type>
X3DUserInterface::createDialog (const std::string & name) const
{
	return std::dynamic_pointer_cast <Type> (createDialog (name));
}

template <class Type>
std::vector <Type*>
X3DUserInterface::getWidgets (Gtk::Widget & parent)
{
	std::vector <Type*> types;

	getWidgets (&parent, types);

	return types;
}

template <class Type>
void
X3DUserInterface::getWidgets (Gtk::Widget* parent, std::vector <Type*> & types)
{
	Type* const type = dynamic_cast <Type*> (parent);
	
	if (type)
		types .emplace_back (type);

	const auto container = dynamic_cast <Gtk::Container*> (parent);
	
	if (container)
	{
		for (auto & child : container -> get_children ())
			getWidgets (child, types);

		return;
	}	
}

} // puck
} // titania

#endif
