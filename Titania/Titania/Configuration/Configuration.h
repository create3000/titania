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

#ifndef __TITANIA_CONFIGURATION_CONFIGURATION_H__
#define __TITANIA_CONFIGURATION_CONFIGURATION_H__

#include <glibmm/keyfile.h>
#include <sstream>
#include <string>

namespace titania {
namespace puck {

class Configuration
{
public:

	typedef std::vector <Configuration> Array;

	Configuration (const std::string & = "");

	/// @name Key lockup

	bool
	hasItem (const std::string & key) const;

	/// @name Set configuration value

	void
	setItem (const std::string & key, const bool value);

	void
	setItem (const std::string & key, const int value);

	void
	setItem (const std::string & key, const double value);

	void
	setItem (const std::string & key, const char* value);

	void
	setItem (const std::string & key, const std::string & value);

	template <class Type>
	void
	set (const std::string & key, const Type & value);

	/// @name Get configuration value

	bool
	getBoolean (const std::string & key) const;

	int
	getInteger (const std::string & key) const;

	double
	getDouble (const std::string & key) const;

	Glib::ustring
	getString (const std::string & key) const;

	template <class Type>
	Type
	get (const std::string & key) const;

	void
	remove (const std::string & key) const;

	~Configuration ();


private:

	class KeyFile
	{
	public:

		KeyFile (const std::string & basename);

		Glib::KeyFile*
		operator -> ()
		{ return &keyfile; }

		const Glib::KeyFile*
		operator -> () const
		{ return &keyfile; }

		~KeyFile ();


	private:

	   const std::string filename;
		Glib::KeyFile     keyfile;

	};

	///  @name Static members

	static KeyFile keyfile;

	///  @name Members

	const std::string group;

};

template <class Type>
void
Configuration::set (const std::string & key, const Type & value)
{
	std::ostringstream osstream;

	osstream .imbue (std::locale::classic ());

	osstream << value;

	setItem (key, osstream .str ());
}

template <class Type>
Type
Configuration::get (const std::string & key) const
{
	std::istringstream isstream (getString (key));

	isstream .imbue (std::locale::classic ());

	Type value = Type ();

	isstream >> value;

	return value;
}

} // puck
} // titania

#endif
