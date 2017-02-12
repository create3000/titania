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

#include "History.h"

#include "../../Configuration/config.h"

#include <Titania/OS.h>
#include <Titania/String.h>

#include <iostream>

namespace titania {
namespace puck {

History::History () :
	X3D::X3DOutput (),
	      filename (config_dir ("history.db")),
	      database (),
	   fileMonitor ()
{
	try
	{
		const bool have_history = os::file_exists (filename);
	
		os::system ("mkdir", "-p", config_dir ());
	
		database .open (filename);
	
		database .query ("CREATE TABLE IF NOT EXISTS History ("
		                 "id           INTEGER,"
		                 "icon         BLOB,"
		                 "title        TEXT,"
		                 "worldURL     TEXT,"
		                 "visited      INTEGER DEFAULT 1,"
		                 "lastAccess   REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
		                 "creationTime REAL    DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')),"
		                 "PRIMARY KEY (id ASC))");
	
		database .try_query ("ALTER TABLE History ADD preview BLOB DEFAULT NULL");
	
		if (not have_history)
		{
			setItem ("Gears",                 "https://cdn.rawgit.com/create3000/Library/master/Examples/Gears/gears.x3d",  "");
			setItem ("Schacht",               "https://cdn.rawgit.com/create3000/Library/master/Examples/Schacht/index.wrl",  "");
			setItem ("Appartment",            "https://cdn.rawgit.com/create3000/Library/master/Examples/Appartment/index.wrl",  "");
			setItem ("LogoPieces",            "https://cdn.rawgit.com/create3000/Library/master/Examples/LogoPieces/index.x3d",  "");
	
			setItem ("Anatomy",               "https://cdn.rawgit.com/create3000/Library/master/Examples/Compel/anatomy/anatomy.wrl",  "");
			setItem ("MovieTruck",            "https://cdn.rawgit.com/create3000/Library/master/Examples/MovieTruck/stage/index.wrl",  "");
			setItem ("ICTS",                  "https://cdn.rawgit.com/create3000/Library/master/Examples/ICTS/index.wrl",  "");
			setItem ("Dolphin",               "https://cdn.rawgit.com/create3000/Library/master/Examples/Dolphin/index.wrl",  "");
			setItem ("BIC Info",              "https://cdn.rawgit.com/create3000/Library/master/Examples/BIC/info.wrl",  "");
			setItem ("BIC Buero",             "https://cdn.rawgit.com/create3000/Library/master/Examples/BIC/buero.wrl",  "");
			setItem ("BIC Building",          "https://cdn.rawgit.com/create3000/Library/master/Examples/BIC/building.wrl",  "");
			setItem ("BIC Man",               "https://cdn.rawgit.com/create3000/Library/master/Examples/BIC/index.wrl",  "");
			setItem ("Jungle",                "https://cdn.rawgit.com/create3000/Library/master/Examples/Compel/jungle/jungle.wrl",  "");
			setItem ("Ripple",                "https://cdn.rawgit.com/create3000/Library/master/Examples/Compel/ripple.wrl",  "");
			setItem ("VRML Banner",           "https://cdn.rawgit.com/create3000/Library/master/Examples/VRMLBanner/vrmlbanner.wrl",  "");
			setItem ("NIST-Logo",             "https://cdn.rawgit.com/create3000/Library/master/Examples/NIST-Logo/index.wrl",  "");
	
			setItem ("Magic Mushrooms",       "https://cdn.rawgit.com/create3000/Library/master/Examples/Mushrooms/index.wrl",  "");
			setItem ("Old Leipzig Rathaus",   "https://cdn.rawgit.com/create3000/Library/master/Examples/Rathaus/stage/index.x3d",  "");
			setItem ("Terrain",               "https://cdn.rawgit.com/create3000/Library/master/Examples/Terrain/index.wrl",  "");
			setItem ("Approach",              "https://cdn.rawgit.com/create3000/Library/master/Examples/Approach/stage/index.wrl",  "");
			setItem ("Huey Huey",             "https://cdn.rawgit.com/create3000/Library/master/Examples/Compel/huey.wrl",  "");
			setItem ("Lost Cathedral",        "https://cdn.rawgit.com/create3000/Library/master/Examples/Cathedral/stage/index.wrl",  "");
			setItem ("Lust For Life",         "https://cdn.rawgit.com/create3000/Library/master/Examples/LustForLife/stage/index.wrl",  "");
			setItem ("Mediafacade Berlin",    "https://cdn.rawgit.com/create3000/Library/master/Examples/Vattenfall/index.wrl",  "");
			setItem ("Foldup",                "https://cdn.rawgit.com/create3000/Library/master/Examples/Compel/foldup.wrl",  "");
			setItem ("4 Pong",                "https://cdn.rawgit.com/create3000/Library/master/Examples/Pong/index.wrl",  "");
			setItem ("Arcadia",               "https://cdn.rawgit.com/create3000/Library/master/Examples/Arcadia/stage/index.wrl",  "");
			setItem ("Astronomy",             "https://cdn.rawgit.com/create3000/Library/master/Examples/Astronomy/stage/astrnmy.wrl",  "");
			setItem ("Backyard Soccer",       "https://cdn.rawgit.com/create3000/Library/master/Examples/ICTS/soccer.wrl",  "");
			setItem ("The Secret Labyrinth",  "https://cdn.rawgit.com/create3000/Library/master/Examples/ICTS/labyrinth.wrl",  "");
			setItem ("Smashing Boxes",        "https://cdn.rawgit.com/create3000/Library/master/Examples/Boxes/boxes.x3d",  "");
			setItem ("Chomp!",                "https://cdn.rawgit.com/create3000/Library/master/Examples/Chomp/chomp.x3d",  "");
	
			setItem ("about:date",  get_page ("about/date.x3dv"),  "");
			setItem ("about:gears", get_page ("about/gears.x3dv"), "");
			setItem ("about:home",  get_page ("about/home.x3dv"),  "");
		}
	
		// Watch for changes
	
		connect ();
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
History::connect ()
{
	// Use:
	// http://www.sqlite.org/c3ref/update_hook.html
	//
	//	void *sqlite3_update_hook (
	//		sqlite3*, 
	//		void(*)(void *,int ,char const *,char const *,sqlite3_int64),
	//		void*
	//	);

	fileMonitor = Gio::File::create_for_path (filename) -> monitor_file ();
	fileMonitor -> signal_changed () .connect (sigc::mem_fun (*this, &History::on_history_changed));
}

void
History::disconnect ()
{
	fileMonitor = Glib::RefPtr <Gio::FileMonitor> ();
}

void
History::on_history_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event)
{
	if (event == Gio::FILE_MONITOR_EVENT_CHANGED)
		processInterests ();
}

void
History::constrainSize (const int32_t months)
{
	try
	{
		if (months > 0)
			database .query ("DELETE FROM History WHERE lastAccess < date ('now','-" + basic::to_string (months, std::locale::classic ()) + " month')");
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

const std::string &
History::getIndex (const std::string & worldURL) const
throw (std::out_of_range,
	    std::invalid_argument)
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

std::string
History::getIcon (const std::string & id) const
throw (std::invalid_argument)
{
	std::string value;

	database .read_blob ("SELECT icon FROM History WHERE id = " + id, value);

	return value;
}

void
History::setPreview (const std::string & worldURL, const std::string & image)
{
	try
	{
		database .write_blob ("UPDATE History SET preview = ? WHERE id = " + getId (worldURL), image);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

std::string
History::getPreview (const std::string & id) const
throw (std::invalid_argument)
{
	std::string value;

	database .read_blob ("SELECT preview FROM History WHERE id = " + id, value);

	return value;
}

void
History::setItem (const std::string & title, const std::string & worldURL, const std::string & image)
{
	try
	{
		try
		{
			update (getId (worldURL), title);
		}
		catch (const std::out_of_range &)
		{
			insert (title, worldURL);
		}
	
		database .write_blob ("UPDATE History SET icon = ? WHERE id = " + getId (worldURL), image);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
History::removeItem (const std::string & id)
{
	try
	{
		database .query ("DELETE FROM History WHERE id = " + id);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

const sql::sqlite3::assoc_row_type &
History::getItemFromIndex (const std::string & index, const Columns column, const SortOrder sortOrder, const std::string & search) const
{
	try
	{
		const std::string where = getWhere (search);
		const std::string order = getOrder (column, sortOrder);

		return database .query_assoc ("SELECT title, worldURL FROM History " +
		                              where + " " +
		                              order + " " +
		                              "LIMIT " + index + ", 1") .at (0);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;

		static const sql::sqlite3::assoc_row_type empty;

		return empty;
	}
}

const sql::sqlite3::assoc_row_type &
History::getItemFromURL (const std::string & worldURL) const
{
	try
	{
		return database .query_assoc ("SELECT title, worldURL FROM History "
		                              "WHERE worldURL = " + database .quote (worldURL) + " " +
		                              "ORDER BY lastAccess DESC") .at (0);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;

		static const sql::sqlite3::assoc_row_type empty;

		return empty;
	}
}

const sql::sqlite3::assoc_type &
History::getItems (const size_t offset, const size_t size, const Columns column, const SortOrder sortOrder, const std::string & search) const
{
	try
	{
		const std::string where = getWhere (search);
		const std::string order = getOrder (column, sortOrder);
		const std::string limit = getLimit (offset, size);
	
		return database .query_assoc ("SELECT id, title, worldURL FROM History " + where + " " + order + " " + limit);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;

		static const sql::sqlite3::assoc_type empty;

		return empty;
	}
}

const std::string &
History::getId (const std::string & worldURL) const
throw (std::out_of_range,
	    std::invalid_argument)
{
	const auto & result = database .query_array ("SELECT id FROM History WHERE "
	                                             "worldURL = " + database .quote (worldURL));

	return result .at (0) .at (0);
}

size_t
History::getSize () const
{
	try
	{
		size_t size = 0;

		std::istringstream isstream (database .query_array ("SELECT COUNT (*) FROM History") .at (0) .at (0));
		
		isstream >> size;

		return size;
	}
	catch (...)
	{
		return 0;
	}
}

std::string
History::getWhere (const std::string & search) const
{
	if (search .empty ())
		return "";

	auto words = std::vector <std::string> ();
	auto terms = std::vector <std::string> ();

	basic::split (std::back_inserter (words), search, " ");

	for (const auto & word : words)
	{
		if (word .empty ())
			continue;

		terms .emplace_back ("title LIKE " + database .quote ("%" + word + "%") + " OR worldURL LIKE " + database .quote ("%" + word + "%"));
	}

	return "WHERE " + basic::join (terms, " OR ");
}

std::string
History::getOrder (const Columns column, const SortOrder sortOrder) const
{
	std::string order;
	
	switch (column)
	{
		case TITLE:
			order = "ORDER BY title";
			break;
		case LAST_ACCESS:
			order = "ORDER BY lastAccess";
			break;
	}

	switch (sortOrder)
	{
		case ASC:
			order += " ASC";
			break;
		case DESC:
			order += " DESC";
			break;
	}

	return order;
}

std::string
History::getLimit (const size_t offset, const size_t size) const
{
	std::string limit;

	if (size)
		limit = "LIMIT " + basic::to_string (size, std::locale::classic ()) + " OFFSET " + basic::to_string (offset, std::locale::classic ());

	return limit;
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
	                 "title = " + database .quote (title) + ","
	                 "visited    = (visited + 1), "
	                 "lastAccess = strftime('%Y-%m-%d %H:%M:%f', 'now') "
	                 "WHERE id = " + id);
}

} // puck
} // titania
