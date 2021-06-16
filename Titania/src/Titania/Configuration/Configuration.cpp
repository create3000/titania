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

#include "Configuration.h"

#include "config.h"

#include <Titania/LOG.h>
#include <Titania/String.h>

#include <giomm.h>
#include <glibmm/fileutils.h>

namespace titania {
namespace puck {

// KeyFile

class Configuration::KeyFile
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

Configuration::KeyFile::KeyFile (const std::string & basename) :
	filename (config_dir (basename)),
	 keyfile ()
{
	const auto configdir = Gio::File::create_for_path (config_dir ());

	if (not configdir -> query_exists ())
		configdir -> make_directory_with_parents (); 

	if (Glib::file_test (filename, Glib::FILE_TEST_EXISTS))
		keyfile .load_from_file (filename, Glib::KEY_FILE_KEEP_COMMENTS);
}

Configuration::KeyFile::~KeyFile ()
{
	keyfile .save_to_file (filename);
}

// Configuration

Configuration::Configuration (const std::string & group) :
	group (group)
{ }

Configuration::KeyFile &
Configuration::getKeyfile () const
{
	static KeyFile keyfile ("configuration.ini");

	return keyfile;
}

bool
Configuration::hasItem (const std::string & key) const
{
	return getKeyfile () -> has_group (group) and getKeyfile () -> has_key (group, key);
}

void
Configuration::setBoolean (const std::string & key, const bool value)
{
	getKeyfile () -> set_boolean (group, key, value);
}

void
Configuration::setInteger (const std::string & key, const int value)
{
	 getKeyfile () -> set_integer (group, key, value);
}

void
Configuration::setDouble (const std::string & key, const double value)
{
	 getKeyfile () -> set_double (group, key, value);
}

void
Configuration::setString (const std::string & key, const Glib::ustring & value)
{
	 getKeyfile () -> set_string (group, key, value);
}

bool
Configuration::getBoolean (const std::string & key) const
{
	if (hasItem (key))
		return getKeyfile () -> get_boolean (group, key);
		
	return false;
}

int
Configuration::getInteger (const std::string & key) const
{
	if (hasItem (key))
		return getKeyfile () -> get_integer (group, key);

	return 0;
}

double
Configuration::getDouble (const std::string & key) const
{
	if (hasItem (key))
		return getKeyfile () -> get_double (group, key);

	return 0;
}

Glib::ustring
Configuration::getString (const std::string & key) const
{
	if (hasItem (key))
		return getKeyfile () -> get_string (group, key);
		
	return "";
}

void
Configuration::removeItem (const std::string & key)
{
	if (hasItem (key))
		getKeyfile () -> remove_key (group, key);
}

void
Configuration::remove ()
{
	if (getKeyfile () -> has_group (group))
		getKeyfile () -> remove_group (group);
}

Configuration::~Configuration ()
{ }

} // puck
} // titania
