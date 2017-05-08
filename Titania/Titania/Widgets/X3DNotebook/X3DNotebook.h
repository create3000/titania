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

#ifndef __TITANIA_WIDGETS_X3DNOTEBOOK_X3DNOTEBOOK_H__
#define __TITANIA_WIDGETS_X3DNOTEBOOK_X3DNOTEBOOK_H__

#include "../../Base/X3DUserInterface.h"

#include <Titania/Math/Functional.h>

#include <gtkmm.h>

#include <memory>
#include <vector>
#include <map>

namespace titania {
namespace puck {

template <class Interface>
class X3DNotebook :
	virtual public Interface
{
public:

	///  @name Construction

	X3DNotebook ();

	///  @name Operations

	void
	addPage (const std::string &, Gtk::Box &);

	template <class Type>
	std::shared_ptr <Type>
	getPage (const std::string & name) const;

	template <class Type>
	std::shared_ptr <Type>
	getCurrentPage () const;

	///  @name Event handlers

	virtual
	void
	on_switch_page (Gtk::Widget*, guint) override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DNotebook () override;


protected:

	///  @name Construction

	virtual
	void
	configure () override;


private:

	///  @name Members

	using Pages = std::map <std::string, std::shared_ptr <X3DUserInterface>>;

	std::map <std::string, Gtk::Box*> boxes;
	std::vector <std::string>         userInterfaces;
	Pages                             pages;

};

template <class Interface>
X3DNotebook <Interface>::X3DNotebook () :
	Interface ()
{ }

template <class Interface>
void
X3DNotebook <Interface>::configure ()
{
	Interface::configure ();

	this -> getNotebook () .set_current_page (this -> getConfig () -> getInteger ("currentPage"));
}

template <class Interface>
void
X3DNotebook <Interface>::addPage (const std::string & name, Gtk::Box & box)
{
	boxes          .emplace (name, &box);
	userInterfaces .emplace_back (name);
	pages          .emplace (name, nullptr);
}

template <class Interface>
template <class Type>
std::shared_ptr <Type>
X3DNotebook <Interface>::getPage (const std::string & name) const
{
	auto & userInterface = const_cast <Pages &> (pages) .at (name);

	if (not userInterface)
	{
		userInterface = this -> createDialog (name);
		userInterface -> setName (this -> getName () + "." + userInterface -> getName ());
		userInterface -> reparent (*boxes .at (name), this -> getWindow ());
	}

	return std::dynamic_pointer_cast <Type> (userInterface);
}

template <class Interface>
template <class Type>
std::shared_ptr <Type>
X3DNotebook <Interface>::getCurrentPage () const
{
	try
	{
		const size_t currentPage = this -> getConfig () -> getInteger ("currentPage");

		return getPage <Type> (userInterfaces .at (currentPage));
	}
	catch (const std::out_of_range &)
	{
		return getPage <Type> (0);
	}
}

template <class Interface>
void
X3DNotebook <Interface>::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	// Show/hide pages

	getCurrentPage <X3DUserInterface> () -> getWidget () .set_visible (false);

	if (pageNumber >= 0 and pageNumber < userInterfaces .size ())
	{
		const auto page = getPage <X3DUserInterface> (userInterfaces .at (pageNumber));

		page -> getWidget () .set_visible (true);
		this -> getLabel () .set_text (_ (page -> getWidget () .get_name ()));
	}

	this -> getConfig () -> setItem ("currentPage", int (pageNumber));
}

template <class Interface>
void
X3DNotebook <Interface>::dispose ()
{
	pages .clear ();

	Interface::dispose ();
}

template <class Interface>
X3DNotebook <Interface>::~X3DNotebook ()
{ }

} // puck
} // titania

#endif
