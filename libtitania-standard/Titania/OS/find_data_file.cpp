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

#include "find_data_file.h"

#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <glibmm/refptr.h>

namespace titania {
namespace os {

using File      = Glib::RefPtr <Gio::File>;
using FileArray = std::deque <File>;

static
File
find_file (const std::string & file, const FileArray & directories);

static
const FileArray &
get_data_directories ();

static
FileArray
create_data_directories ();

///  throws std::ios_base::failure
std::string
find_data_file (const std::string & file)
{
	return find_file (file, get_data_directories ()) -> get_path ();
}

///  throws std::ios_base::failure
static
File
find_file (const std::string & file, const FileArray & directories)
{
	for (const auto & directory : directories)
	{
		File fullPath = directory -> resolve_relative_path (file);

		if (fullPath -> query_exists ())
			return fullPath;
	}

	std::string dirs;

	for (auto d : directories)
		dirs += "  " + d -> get_path () + "\n";

	throw std::ios_base::failure ("Error: Couldn't find '" + file + "' in specified directories:\n" + dirs);
}

static
const FileArray &
get_data_directories ()
{
	static const FileArray directories = create_data_directories ();

	return directories;
}

static
FileArray
create_data_directories ()
{
	FileArray directories;

	directories .emplace_back (Gio::File::create_for_path (Glib::get_user_data_dir ()));

	for (int i = 0; g_get_system_data_dirs () [i]; ++ i)
		directories .emplace_back (Gio::File::create_for_path (g_get_system_data_dirs () [i]));

	return directories;
}

} // os
} // titania
