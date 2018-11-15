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

#ifndef __TITANIA_SQL_SQLITE3_H__
#define __TITANIA_SQL_SQLITE3_H__

#include <deque>
#include <map>
#include <stdexcept>
#include <string>

extern "C"
{
#include <sqlite3.h>
}

namespace titania {
namespace sql {

class sqlite3
{
public:

	typedef std::deque <std::string>    array_row_type;
	typedef std::deque <array_row_type> array_type;

	typedef std::map <std::string, std::string> assoc_row_type;
	typedef std::deque <assoc_row_type>         assoc_type;

	///  @name Constructors

	sqlite3 ();

	sqlite3 (const std::string &);

	///  @name Database handling

	///  throws std::invalid_argument
	void
	open (const std::string &);

	/// @name Query handling

	///  throws std::invalid_argument
	void
	query (const std::string &);

	bool
	try_query (const std::string &);

	///  throws std::invalid_argument
	const array_type &
	query_array (const std::string &) const;

	///  throws std::invalid_argument
	const assoc_type &
	query_assoc (const std::string &) const;

	///  throws std::invalid_argument
	void
	write_blob (const std::string &, const std::string &) const;

	///  throws std::invalid_argument
	void
	read_blob (const std::string &, std::string &) const;

	///  throws std::out_of_range
	const std::string &
	last_insert_rowid () const;

	const std::string &
	last_query () const
	{ return lastQuery; }

	///  @name Utility funtions

	std::string
	quote (const std::string &) const;

	///  @name Destructor

	~sqlite3 ();


private:

	///  throws std::invalid_argument
	void
	exec (const std::string & statement, int (* callback) (void*, int, char**, char**)) const;

	static
	int
	pass (void*, int, char**, char**);

	static
	int
	array_callback (void*, int, char**, char**);

	static
	int
	map_callback (void*, int, char**, char**);

	static
	void
	print (int, char**, char**);

	///  throws std::invalid_argument
	void
	error (const std::string &, const std::string &, const std::string & statement) const;


private:

	::sqlite3* database;

	mutable array_type array;
	mutable assoc_type array_map;
	mutable std::string lastQuery;

};

} // sql
} // titania

#endif
