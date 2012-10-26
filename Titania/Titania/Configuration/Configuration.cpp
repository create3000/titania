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

#include "Configuration.h"

#include <Titania/Bits/String.h>
#include <Titania/LOG.h>

namespace titania {
namespace puck {

Configuration::Configuration (const std::string & path, const std::string & name) :
	      path (path),                    
	      name (name),              
	       key (path + '/' + name)
{
	Gnome::Conf::init ();
	client = Gnome::Conf::Client::get_default_client ();
}

void
Configuration::setPath (const std::string & value)
{
	path = value;
	key  = path + '/' + name;
}

const std::string &
Configuration::getKey () const
{
	return key;
}

std::string
Configuration::getKey (const std::string & name) const
{
	return key + '/' + name;
}

bool
Configuration::hasItem (const std::string & name) const
{
	auto item = client -> get_entry (getKey (name));
	return item .get_value () .get_type () != Gnome::Conf::VALUE_INVALID;
}

void
Configuration::setItem (const std::string & name, const bool value)
{
	client -> set (getKey (name), value);
}

void
Configuration::setItem (const std::string & name, const int value)
{
	client -> set (getKey (name), value);
}

void
Configuration::setItem (const std::string & name, const size_t value)
{
	client -> set (getKey (name), (int) value);
}

void
Configuration::setItem (const std::string & name, const std::string & value)
{
	client -> set (getKey (name), value);
}

bool
Configuration::boolean (const std::string & name) const
{
	return client -> get_bool (getKey (name));
}

int
Configuration::integer (const std::string & name) const
{
	return client -> get_int (getKey (name));
}

Glib::ustring
Configuration::string (const std::string & name) const
{
	return client -> get_string (getKey (name));
}

bool
Configuration::exists ()
{
	return client -> dir_exists (key);
}

void
Configuration::remove ()
{
	client -> remove_dir (key);
}

Configuration
Configuration::getSession (size_t id)
{
	return getDirectory ("Sessions/" + std::to_string (id));
}

Configuration
Configuration::getDirectory (const std::string & subdir)
{
	return Configuration (path, name + '/' + subdir);
}

Configuration::Array
Configuration::getDirectories ()
{
	Array directories;

	for (const auto & subdir : client -> all_dirs (key))
	{
		auto dirName = basic::split (subdir, "/");
		directories .emplace_back (path, name + '/' + dirName .back ());
	}

	return directories;
}

} // puck
} // titania
