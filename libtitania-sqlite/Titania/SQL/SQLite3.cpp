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

#include "SQLite3.h"

#include <iostream>

namespace titania {
namespace sql {

sqlite3::sqlite3 (const std::string & filename) :
	 database (NULL), 
	    array (),     
	array_map ()
{
	open (filename);
}

void
sqlite3::open (const std::string & filename)
throw (std::invalid_argument)
{
	if (c::sqlite3_open (filename .c_str (), &database))
		error ("Can't open database: ", c::sqlite3_errmsg (database));
}

void
sqlite3::query (const std::string & statement)
throw (std::invalid_argument)
{
	exec (statement, NULL);
}

sqlite3::array_type
sqlite3::query_array (const std::string & statement)
throw (std::invalid_argument)
{
	array .clear ();

	exec (statement, &array_callback);

	return array;
}

sqlite3::assoc_type
sqlite3::query_assoc (const std::string & statement)
throw (std::invalid_argument)
{
	array_map .clear ();

	exec (statement, &map_callback);

	return array_map;
}

std::string
sqlite3::last_insert_rowid ()
throw (std::out_of_range)
{
	return query_array ("SELECT LAST_INSERT_ROWID ()")  .at (0) .at (0);
}

std::string
sqlite3::quote (const std::string & value) const
{
	return "'" + value + "'";
}

void
sqlite3::exec (const std::string & statement, int (* callback) (void*, int, char**, char**))
throw (std::invalid_argument)
{
	char* errorMessage = NULL;

	if (c::sqlite3_exec (database, statement .c_str (), callback, this, &errorMessage) not_eq SQLITE_OK)
	{
		error ("Can't exec query: ", errorMessage);

		c::sqlite3_free (errorMessage);
	}
}

int
sqlite3::pass (void* notUsed, int argc, char** argv, char** columns)
{
	print (argc, argv, columns);

	return 0;
}

int
sqlite3::array_callback (void* object, int argc, char** argv, char** columns)
{
	sqlite3* self = static_cast <sqlite3*> (object);

	self -> array .emplace_back (argv, argv + argc);

	//print (argc, argv, columns);

	return 0;
}

int
sqlite3::map_callback (void* object, int argc, char** argv, char** columns)
{
	sqlite3* self = static_cast <sqlite3*> (object);

	std::map <std::string, std::string> map;

	for (int i = 0; i < argc; i ++)
		map .insert (std::make_pair (columns [i], argv [i]));

	self -> array_map .emplace_back (map);

	//print (argc, argv, columns);

	return 0;
}

void
sqlite3::print (int argc, char** argv, char** columns)
{
	for (int i = 0; i < argc; ++ i)
	{
		std::cout << columns [i] << " = " << argv [i] << std::endl;
	}
}

void
sqlite3::error (const std::string & what, const std::string & message) const
throw (std::invalid_argument)
{
	throw std::invalid_argument ("SQL error: " + what + message + '.');
}

sqlite3::~sqlite3 ()
{
	c::sqlite3_close (database);
}

} // sql
} // titania
