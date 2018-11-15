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

#include "SQLite3.h"

#include <iostream>
#include <regex>

namespace titania {
namespace sql {

sqlite3::sqlite3 () :
	 database (nullptr),
	    array (),
	array_map (),
	lastQuery ()
{ }

sqlite3::sqlite3 (const std::string & filename) :
	sqlite3 ()
{
	open (filename);
}

///  throws std::invalid_argument
void
sqlite3::open (const std::string & filename)
{
	if (::sqlite3_open (filename .c_str (), &database))
		error ("Can't open database: ", ::sqlite3_errmsg (database), "");

	query ("PRAGMA database_list");
}

///  throws std::invalid_argument
void
sqlite3::query (const std::string & statement)
{
	exec (statement, nullptr);
}

bool
sqlite3::try_query (const std::string & statement)
{
	try
	{
		exec (statement, nullptr);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

///  throws std::invalid_argument
const sqlite3::array_type &
sqlite3::query_array (const std::string & statement) const
{
	array .clear ();

	exec (statement, &array_callback);

	return array;
}

///  throws std::invalid_argument
const sqlite3::assoc_type &
sqlite3::query_assoc (const std::string & statement) const
{
	array_map .clear ();

	exec (statement, &map_callback);

	return array_map;
}

// http://www.sqlite.org/cvstrac/wiki?p=BlobExample

///  throws std::invalid_argument
void
sqlite3::write_blob (const std::string & statement, const std::string & value) const
{
	::sqlite3_stmt* prepared_statement;
	int returnCode;

	do
	{
		returnCode = ::sqlite3_prepare (database, statement .c_str (), statement .size (), &prepared_statement, 0);

		if (returnCode not_eq SQLITE_OK)
			throw std::invalid_argument ("sqlite3::write_blob in prepare: " + std::string (::sqlite3_errmsg (database)));

		::sqlite3_bind_blob (prepared_statement, 1, value .c_str (), value .size (), SQLITE_STATIC);

		returnCode = ::sqlite3_step (prepared_statement);

		if (returnCode == SQLITE_ROW)
			throw std::invalid_argument ("sqlite3::write_blob in step: " + std::string (::sqlite3_errmsg (database)));

		returnCode = ::sqlite3_finalize (prepared_statement);
	}
	while (returnCode == SQLITE_SCHEMA);
}

///  throws std::invalid_argument
void
sqlite3::read_blob (const std::string & statement, std::string & value) const
{
	::sqlite3_stmt* prepared_statement;
	int             returnCode;

	do
	{
		returnCode = ::sqlite3_prepare (database, statement .c_str (), statement .size (), &prepared_statement, 0);

		if (returnCode not_eq SQLITE_OK)
			throw std::invalid_argument ("sqlite3::write_blob in prepare: " + std::string (::sqlite3_errmsg (database)));

		returnCode = ::sqlite3_step (prepared_statement);

		if (returnCode == SQLITE_ROW)
			value = std::string ((const char*) ::sqlite3_column_blob (prepared_statement, 0), ::sqlite3_column_bytes (prepared_statement, 0));

		returnCode = ::sqlite3_finalize (prepared_statement);
	}
	while (returnCode == SQLITE_SCHEMA);
}

///  throws std::out_of_range
const std::string &
sqlite3::last_insert_rowid () const
{
	try
	{
		return query_array ("SELECT LAST_INSERT_ROWID ()")  .at (0) .at (0);
	}
	catch (const std::invalid_argument &)
	{
		throw std::out_of_range ("sqlite3::last_insert_rowid");
	}
}

std::string
sqlite3::quote (const std::string & value) const
{
	static const std::regex SingleQuote ("'");

	return "'" + std::regex_replace (value, SingleQuote, "''") + "'";
}

///  throws std::invalid_argument
void
sqlite3::exec (const std::string & statement, int (* callback) (void*, int, char**, char**)) const
{
	lastQuery = statement;

	char* errorMessage = nullptr;

	if (::sqlite3_exec (database, statement .c_str (), callback, const_cast <sqlite3*> (this), &errorMessage) not_eq SQLITE_OK)
	{
		std::string string = errorMessage ? errorMessage : "unknown error";

		::sqlite3_free (errorMessage);

		if (database)
			error ("Can't exec query: ", string, statement);

		else
			error ("Can't exec query: ", "database not open", "");
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

	array_row_type array;

	for (int i = 0; i < argc; i ++)
	{
		if (argv [i])
			array .emplace_back (argv [i] ? argv [i] : "");
	}

	if (array .size ())
		self -> array .emplace_back (std::move (array));

	//print (argc, argv, columns);

	return 0;
}

int
sqlite3::map_callback (void* object, int argc, char** argv, char** columns)
{
	sqlite3* self = static_cast <sqlite3*> (object);

	assoc_row_type map;

	for (int i = 0; i < argc; i ++)
	{
		map .emplace (columns [i], argv [i] ? argv [i] : "");
	}

	self -> array_map .emplace_back (std::move (map));

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

///  throws std::invalid_argument
void
sqlite3::error (const std::string & what, const std::string & message, const std::string & statement) const
{
	throw std::invalid_argument ("SQL error: " + what + message + (statement .empty () ? "." : ":\n" + statement));
}

sqlite3::~sqlite3 ()
{
	if (database)
		::sqlite3_close (database);
}

} // sql
} // titania
