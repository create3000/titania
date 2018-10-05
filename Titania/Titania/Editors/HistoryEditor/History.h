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

#ifndef __TITANIA_EDITORS_HISTORY_EDITOR_HISTORY_H__
#define __TITANIA_EDITORS_HISTORY_EDITOR_HISTORY_H__

#include <Titania/SQL/SQLite3.h>
#include <Titania/X3D/Base/Output.h>
#include <giomm.h>

namespace titania {
namespace puck {

class History :
	public X3D::X3DOutput
{
public:

	///  @name Member types

	enum Columns
	{
		TITLE,
		LAST_ACCESS
	};

	enum SortOrder
	{
		DESC,
		ASC
	};

	///  @name Construction

	History ();

	///  @name Operations

	std::string
	getId (const std::string & worldURL) const
	throw (std::out_of_range,
	       std::invalid_argument);

	void
	setPreview (const std::string & worldURL, const std::string & image);

	std::string
	getPreview (const std::string & id) const
	throw (std::invalid_argument);

	void
	setContextPath (const std::string & worldURL, const std::string & contextPath);

	std::string
	getContextPath (const std::string & worldURL) const;

	void
	setItem (const std::string & title, const std::string & worldURL);

	sql::sqlite3::assoc_row_type
	getItem (const std::string & id) const;

	sql::sqlite3::assoc_type
	getItems (const size_t offset, const size_t limit, const std::string & search = "") const;

	void
	removeItem (const std::string & id);

	void
	setSize (const int32_t months);

	size_t
	getSize () const;

	~History ();


private:

	///  @name FileMonitor

	void
	connect ();

	void
	disconnect ();

	void
	on_history_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event);

	///  @name Operations

	std::string
	getOrder (const Columns column, const SortOrder sortOrder) const;

	std::string
	getWhere (const std::string & search) const;

	std::string
	getLimit (const size_t offset, const size_t size) const;

	void
	insert (const std::string & title, const std::string & worldURL);

	void
	update (const std::string & id, const std::string & title);

	///  @name Members

	std::string filename;
	sql::sqlite3 database;
	Glib::RefPtr <Gio::FileMonitor> fileMonitor;

};

} // puck
} // titania

#endif
