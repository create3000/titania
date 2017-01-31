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

#ifndef __TITANIA_APPLICATION_OPTIONS_H__
#define __TITANIA_APPLICATION_OPTIONS_H__

#include <vector>
#include <glibmm.h>

namespace titania {
namespace puck {

class ApplicationOptions :
	public Glib::OptionContext
{
public:

	ApplicationOptions (int & argc, char** & argv) :
		Glib::OptionContext ("- Titania X3D Browser"),
		          filenames (),
		     exportFilename (),
		               list (),
		               help (false)
	{
		Glib::OptionEntry optionExportFilename;
		Glib::OptionEntry optionExportStyle;
		Glib::OptionEntry optionList;
		Glib::OptionEntry optionHelp;

		set_summary ("Titania is a X3D/VRML editor and browser for Ubuntu.");

		Glib::OptionGroup mainGroup ("common", "Common options", "General options");
		Glib::OptionGroup exportGroup ("export", "Export options", "Export options");
		Glib::OptionGroup listGroup ("list", "Listings options", "Listings options");

		optionExportFilename .set_short_name ('e');
		optionExportFilename .set_long_name ("export");
		optionExportFilename .set_arg_description ("FILENAME");
		optionExportFilename .set_description ("Set export filename, if '-' is given the output goes to STDOUT.");
		//optionExportFilename .set_flags (Glib::OptionEntry::FLAG_OPTIONAL_ARG | Glib::OptionEntry::FLAG_FILENAME);

		optionExportStyle .set_short_name ('s');
		optionExportStyle .set_long_name ("style");
		optionExportStyle .set_arg_description ("nicest|compact|small|smallest");
		optionExportStyle .set_description ("Set output style for export.");
		//optionExportStyle .set_flags (Glib::OptionEntry::FLAG_OPTIONAL_ARG);

		optionList .set_short_name ('l');
		optionList .set_long_name ("list");
		optionList .set_arg_description ("profiles|components|nodes|fields");
		optionList .set_description ("Get a list of all supported profiles, components, nodes or fields.");
		//optionList .set_flags (Glib::OptionEntry::FLAG_OPTIONAL_ARG);

		set_description ("Copyright 2010 Holger Seelig. License GPLv3+");

		mainGroup   .add_entry (optionHelp,           help);
		exportGroup .add_entry (optionExportFilename, exportFilename);
		exportGroup .add_entry (optionExportStyle,    exportStyle);
		listGroup   .add_entry (optionList,           list);

		set_main_group (mainGroup);
		add_group (exportGroup);
		add_group (listGroup);

		parse (argc, argv);

		for (int i = 1; i < argc; ++ i) 
			filenames .emplace_back (argv [i]);
	}

	std::vector <Glib::ustring> filenames;
	Glib::ustring               exportFilename;
	Glib::ustring               exportStyle;
	Glib::ustring               list;
	bool                        help;

};

} // puck
} // titania

#endif
