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

#include "File.h"

#include <Titania/String.h>
#include <gtkmm.h>

namespace titania {
namespace puck {

basic::uri
File::getUri (const Glib::RefPtr <Gio::File> & file)
{
	if (file -> get_path () .empty ())
		return file -> get_uri ();

	return "file://" + file -> get_path ();
}

std::pair <std::string, bool>
File::getContentType (const std::string & data)
{
	bool resultUncertain = false;

	const auto contentType = Gio::content_type_guess ("", (guchar*) &data [0], data .size (), resultUncertain);

	return std::pair (contentType, resultUncertain);
}

std::string
File::getIconName (const Glib::RefPtr <Gio::FileInfo> & fileInfo)
{
	const auto iconInfo = Gtk::IconTheme::get_default () -> lookup_icon (fileInfo -> get_icon (), 16);
   const auto filename = basic::uri (iconInfo .get_filename ());
	const auto iconName = filename .stem ();

	return iconName;
}

Glib::RefPtr <Gio::File>
File::getContainingFolder (const Glib::RefPtr <Gio::File> & file)
{
	const auto fileInfo = file -> query_info ();
	
	if (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
		return file;

	return file -> get_parent ();
}

std::vector <Glib::RefPtr <Gio::FileInfo>>
File::getChildren (const Glib::RefPtr <Gio::File> & directory, const bool hidden)
{
	try
	{
		auto       fileInfos  = std::vector <Glib::RefPtr <Gio::FileInfo>> ();
		const auto enumerator = directory -> enumerate_children ();
		auto       fileInfo   = enumerator -> next_file ();

		while (fileInfo)
		{
			if (hidden or not (fileInfo -> is_hidden () or fileInfo -> is_backup ()))
				fileInfos .emplace_back (fileInfo);

			fileInfo = enumerator -> next_file ();
		}
	
		std::sort (fileInfos .begin (), fileInfos .end (), [ ] (const Glib::RefPtr <Gio::FileInfo> & lhs, const Glib::RefPtr <Gio::FileInfo> & rhs)
		           {
		              return basic::naturally_compare (lhs -> get_name (), rhs -> get_name ());
					  });
	
		std::stable_sort (fileInfos .begin (), fileInfos .end (), [ ] (const Glib::RefPtr <Gio::FileInfo> & lhs, const Glib::RefPtr <Gio::FileInfo> & rhs)
		                  {
		                     return (lhs -> get_file_type () == Gio::FILE_TYPE_DIRECTORY) > (rhs -> get_file_type () == Gio::FILE_TYPE_DIRECTORY);
								});
	
		return fileInfos;
	}
	catch (...)
	{
		return { };
	}
}

bool
File::isSubfolder (Glib::RefPtr <Gio::File> subfolder, const Glib::RefPtr <Gio::File> & folder)
{
	const auto folderUri = folder -> get_uri ();

	if (subfolder -> get_uri () == folderUri)
		return true;

	while (subfolder -> has_parent ())
	{
		subfolder = subfolder -> get_parent ();

		const auto subfolderUri = subfolder -> get_uri ();

		if (subfolderUri .size () < folderUri .size ())
			return false;

		if (subfolderUri == folderUri)
			return true;
	}

	return false;
}

bool
File::hasChildren (const Glib::RefPtr <Gio::File> & directory, const bool hidden)
{
	try
	{
		const auto enumerator = directory -> enumerate_children ();
		auto       fileInfo   = enumerator -> next_file ();

		while (fileInfo)
		{
			if (hidden or not (fileInfo -> is_hidden () or fileInfo -> is_backup ()))
				return true;

			fileInfo = enumerator -> next_file ();
		}
	
		return false;
	}
	catch (...)
	{
		return false;
	}
}

void
File::copyFile (const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & destination, const Gio::FileCopyFlags flags)
{
	const auto sourceInfo = source -> query_info ();

	if (sourceInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY)
		copyFolder (source, destination, flags);
	else
		source -> copy (destination, flags);
}

void
File::copyFolder (const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & destination, const Gio::FileCopyFlags flags)
{
	destination -> make_directory_with_parents ();
	
	for (const auto fileInfo : getChildren (source, true))
	{
		const auto sourceChild      = source -> get_child (fileInfo -> get_name ());
		const auto destinationChild = destination -> get_child (fileInfo -> get_name ());

		switch (fileInfo -> get_file_type ())
		{
			case Gio::FILE_TYPE_DIRECTORY:
			{
				copyFolder (sourceChild, destinationChild, flags);
				continue;
			}
			case Gio::FILE_TYPE_REGULAR:
			case Gio::FILE_TYPE_SYMBOLIC_LINK:
			{
				sourceChild -> copy (destinationChild, flags);
				continue;
			}
			default:
				continue;
		}
	}
}

void
File::removeFile (const Glib::RefPtr <Gio::File> & file)
{
	for (const auto & fileInfo : File::getChildren (file, true))
		removeFile (file -> get_child (fileInfo -> get_name ()));

	file -> remove ();
}

} // puck
} // titania
