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

#include "config.h"

#include <Titania/OS.h>
#include <Titania/LOG.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

// KeyFile

Configuration::KeyFile::KeyFile (const std::string & basename) :
	filename (config_dir (basename)),
	 keyfile ()
{
	os::system ("rm",    "-r", config_dir ("configuration/")); // XXX
	os::system ("mkdir", "-p", config_dir ());

	if (os::file_exists (filename))
		keyfile .load_from_file (filename, Glib::KEY_FILE_KEEP_COMMENTS);
}

Configuration::KeyFile::~KeyFile ()
{
	keyfile .save_to_file (filename);
}

// Configuration

Configuration::KeyFile Configuration::keyfile ("configuration.ini");

Configuration::Configuration (const std::string & group) :
	group (group)
{ }

bool
Configuration::hasItem (const std::string & key) const
{
	return keyfile -> has_group (group) and keyfile -> has_key (group, key);
}

void
Configuration::setItem (const std::string & key, const bool value)
{
	keyfile -> set_boolean (group, key, value);
}

void
Configuration::setItem (const std::string & key, const int value)
{
	 keyfile -> set_integer (group, key, value);
}

void
Configuration::setItem (const std::string & key, const double value)
{
	 keyfile -> set_double (group, key, value);
}

void
Configuration::setItem (const std::string & key, const char* value)
{
	 keyfile -> set_string (group, key, value);
}

void
Configuration::setItem (const std::string & key, const std::string & value)
{
	 keyfile -> set_string (group, key, value);
}

bool
Configuration::getBoolean (const std::string & key) const
{
	if (hasItem (key))
		return keyfile -> get_boolean (group, key);
		
	return false;
}

int
Configuration::getInteger (const std::string & key) const
{
	if (hasItem (key))
		return keyfile -> get_integer (group, key);

	return 0;
}

double
Configuration::getDouble (const std::string & key) const
{
	if (hasItem (key))
		return keyfile -> get_double (group, key);

	return 0;
}

Glib::ustring
Configuration::getString (const std::string & key) const
{
	if (hasItem (key))
		return keyfile -> get_string (group, key);
		
	return "";
}

Configuration::~Configuration ()
{ }

} // puck
} // titania
