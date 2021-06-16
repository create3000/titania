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

	///  @name Construction

	Configuration (const std::string & = "");

	/// @name Key lockup

	bool
	hasItem (const std::string & key) const;

	/// @name Set configuration value

	template <class Type>
	void
	setItem (const std::string & key, const Type & value);

	/// @name Get configuration value

	template <class Type>
	Type
	getItem (const std::string & key, const Type & defaultValue = Type ()) const;

	/// @name Remove group or configuration value

	void
	removeItem (const std::string & key);

	void
	remove ();

	///  @name Destruction

	~Configuration ();


private:

	///  @name Member types

	class KeyFile;

	using cstr_type = char*;

	///  @name Member access

	KeyFile &
	getKeyfile () const;

	void
	setBoolean (const std::string & key, const bool value);

	void
	setInteger (const std::string & key, const int value);

	void
	setDouble (const std::string & key, const double value);

	void
	setString (const std::string & key, const Glib::ustring & value);

	bool
	getBoolean (const std::string & key) const;

	int
	getInteger (const std::string & key) const;

	double
	getDouble (const std::string & key) const;

	Glib::ustring
	getString (const std::string & key) const;

	///  @name Members

	const std::string group;

};

template <class Type>
void
Configuration::setItem (const std::string & key, const Type & value)
{
	std::ostringstream osstream;

	osstream .imbue (std::locale::classic ());

	osstream << value;

	setItem (key, osstream .str ());
}

template <>
inline
void
Configuration::setItem (const std::string & key, const bool & value)
{
	setBoolean (key, value);
}

template <>
inline
void
Configuration::setItem (const std::string & key, const int & value)
{
	setInteger (key, value);
}

template <>
inline
void
Configuration::setItem (const std::string & key, const double & value)
{
	setDouble (key, value);
}

template <>
inline
void
Configuration::setItem (const std::string & key, const Glib::ustring & value)
{
	setString (key, value);
}

template <>
inline
void
Configuration::setItem (const std::string & key, const std::string & value)
{
	setString (key, value);
}

template <>
inline
void
Configuration::setItem (const std::string & key, const cstr_type & value)
{
	setString (key, value);
}

template <class Type>
Type
Configuration::getItem (const std::string & key, const Type & defaultValue) const
{
	if (hasItem (key))
	{
		std::istringstream isstream (getString (key));
	
		isstream .imbue (std::locale::classic ());
	
		Type value = Type ();
	
		isstream >> value;
	
		return value;
	}

	return defaultValue;
}

template <>
inline
bool
Configuration::getItem (const std::string & key, const bool & defaultValue) const
{
	if (hasItem (key))
		return getBoolean (key);

	return defaultValue;
}

template <>
inline
int
Configuration::getItem (const std::string & key, const int & defaultValue) const
{
	if (hasItem (key))
		return getInteger (key);

	return defaultValue;
}

template <>
inline
double
Configuration::getItem (const std::string & key, const double & defaultValue) const
{
	if (hasItem (key))
		return getDouble (key);

	return defaultValue;
}

template <>
inline
Glib::ustring
Configuration::getItem (const std::string & key, const Glib::ustring & defaultValue) const
{
	if (hasItem (key))
		return getString (key);

	return defaultValue;
}

template <>
inline
std::string
Configuration::getItem (const std::string & key, const std::string & defaultValue) const
{
	if (hasItem (key))
		return getString (key);

	return defaultValue;
}

} // puck
} // titania

#endif
