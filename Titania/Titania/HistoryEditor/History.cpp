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

#include "History.h"

#include "../Configuration/config.h"
#include <Titania/OS.h>
#include <iostream>

namespace titania {
namespace puck {

History::History () :
	database ()
{
	std::string filename     = config_dir ("history.db");
	bool        have_history = os::file_exists (filename);

	os::system ("mkdir", "-p", config_dir ());

	database .open (filename);

	database .query ("CREATE TABLE IF NOT EXISTS History ("
	                 "id           INTEGER, "
	                 "icon         BLOB,"
	                 "title        TEXT,"
	                 "worldURL     TEXT,"
	                 "visited      INTEGER DEFAULT 1,"
	                 "lastAccess   REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
	                 "creationTime REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
	                 "PRIMARY KEY (id ASC))");

	if (not have_history)
	{
		setItem ("about:date",  get_page ("about/date.wrl"));
		setItem ("about:gears", get_page ("about/gears.wrl"));
		setItem ("about:home",  get_page ("about/home.wrl"));
	}
}

void
History::setItem (const std::string & title, const std::string & worldURL)
{
	try
	{
		update (getId (worldURL), title);
	}
	catch (const std::out_of_range &)
	{
		insert (title, worldURL);
	}
}

const sql::sqlite3::assoc_row_type &
History::getItemFromIndex (const std::string & index) const
throw (std::out_of_range)
{
	return database .query_assoc ("SELECT title, worldURL FROM History "
	                              "ORDER BY lastAccess DESC "
	                              "LIMIT " + index + ", 1") .at (0);
}

const sql::sqlite3::assoc_row_type &
History::getItemFromURL (const std::string & worldURL) const
throw (std::out_of_range)
{
	return database .query_assoc ("SELECT title, worldURL FROM History "
	                              "WHERE worldURL = " + database .quote (worldURL) + " " +
	                              "ORDER BY lastAccess DESC") .at (0);
}

const sql::sqlite3::assoc_type &
History::getItems () const
{
	return database .query_assoc ("SELECT title, worldURL FROM History ORDER BY lastAccess DESC");
}

const std::string &
History::getIndex (const std::string & worldURL) const
throw (std::out_of_range)
{
	const auto & result = database .query_array ("SELECT "
	                                             "(SELECT COUNT(0) - 1 FROM History h1 WHERE h1 .lastAccess >= h2 .lastAccess) AS 'rowid' "
	                                             "FROM History h2 "
	                                             "WHERE worldURL = " + database .quote (worldURL) + " " +
	                                             "ORDER BY lastAccess DESC");

	const std::string & index = result .at (0) .at (0);

	if (index == "-1")
		throw std::out_of_range ("URL '" + worldURL + "' not found.");

	return index;
}

void
History::insert (const std::string & title, const std::string & worldURL)
{
	database .query ("INSERT INTO History "
	                 "(title, worldURL)"
	                 "VALUES ("
	                 + database .quote (title) + ","
	                 + database .quote (worldURL)
	                 + ")");
}

void
History::update (const std::string & id, const std::string & title)
{
	database .query ("UPDATE History "
	                 "SET "
	                 "title      = " + database .quote (title) + ","
	                                                             "visited    = (visited + 1), "
	                                                             "lastAccess = strftime('%Y-%m-%d %H:%M:%f', 'now') "
	                                                             "WHERE id = " + id);
}

const std::string &
History::getId (const std::string & worldURL) const
throw (std::out_of_range)
{
	const auto & result = database .query_array ("SELECT id FROM History WHERE "
	                                             "worldURL = " + database .quote (worldURL));

	return result .at (0) .at (0);
}

} // puck
} // titania
