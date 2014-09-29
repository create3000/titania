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

#include "Configuration.h"

#include <Titania/LOG.h>
#include <Titania/String.h>

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
	try
	{
		const auto item = client -> get_entry (getKey (name));

		return item .get_value () .get_type () not_eq Gnome::Conf::VALUE_INVALID;
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
		return false;
	}
}

void
Configuration::setItem (const std::string & name, const bool value)
{
	try
	{
		client -> set (getKey (name), value);
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Configuration::setItem (const std::string & name, const int value)
{
	try
	{
		client -> set (getKey (name), value);
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Configuration::setItem (const std::string & name, const char* value)
{
	try
	{
		client -> set (getKey (name), std::string (value));
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Configuration::setItem (const std::string & name, const std::string & value)
{
	try
	{
		client -> set (getKey (name), value);
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
Configuration::getBoolean (const std::string & name) const
{
	try
	{
		return client -> get_bool (getKey (name));
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
		
		return false;
	}
}

int
Configuration::getInteger (const std::string & name) const
{
	try
	{
		return client -> get_int (getKey (name));
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
		
		return 0;
	}
}

Glib::ustring
Configuration::getString (const std::string & name) const
{
	try
	{
		return client -> get_string (getKey (name));
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	
		return "";
	}
}

Configuration
Configuration::getDirectory (const std::string & subdir) const
{
	return Configuration (path, name + '/' + subdir);
}

Configuration::Array
Configuration::getDirectories () const
{
	Array directories;

	try
	{
		for (const auto & subdir : client -> all_dirs (key))
		{
			const auto dirName = basic::split (subdir, "/");
			directories .emplace_back (path, name + '/' + dirName .back ());
		}
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	return directories;
}

bool
Configuration::exists () const
{
	try
	{
		return client -> dir_exists (key);
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
		return false;
	}
}

void
Configuration::remove ()
{
	try
	{
		client -> recursive_unset (key);
	}
	catch (const Gnome::Conf::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

} // puck
} // titania
