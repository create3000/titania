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

#ifndef __TITANIA_COMMANDS_COMMAND_OPTIONS_H__
#define __TITANIA_COMMANDS_COMMAND_OPTIONS_H__

#include <glibmm.h>
#include <vector>

namespace titania {
namespace puck {

class CommandOptions :
	public Glib::OptionContext
{
public:

	CommandOptions (int & argc, char** & argv) :
		Glib::OptionContext ("- Titania X3D Editor"),
		           filenames (),
		       imageFilename (),
		               width (300),
		              height (100),
		        alphaChannel (false),
		        antialiasing (4),
		      exportFilename (),
		         exportStyle (),
		         exportInfer (false),
		exportRemoveMetadata (false),
		                list (),
		                help (false)
	{
		Glib::OptionEntry optionExportImage;
		Glib::OptionEntry optionWidth;
		Glib::OptionEntry optionHeight;
		Glib::OptionEntry optionAlphaChannel;
		Glib::OptionEntry optionAntialiasing;
		Glib::OptionEntry optionExportFilename;
		Glib::OptionEntry optionExportStyle;
		Glib::OptionEntry optionExportInfer;
		Glib::OptionEntry optionExportRemoveMetadata;
		Glib::OptionEntry optionList;
		Glib::OptionEntry optionHelp;

		set_summary ("Titania is a X3D/VRML editor and browser for the Linux operating system.");

		Glib::OptionGroup mainGroup   ("common", "Common options",   "General options");
		Glib::OptionGroup imageGroup  ("image",  "Image options",    "Image options");
		Glib::OptionGroup exportGroup ("export", "Export options",   "Export options");
		Glib::OptionGroup listGroup   ("list",   "Listings options", "Listings options");

		optionExportImage .set_short_name ('i');
		optionExportImage .set_long_name ("export-image");
		optionExportImage .set_arg_description ("FILENAME");
		optionExportImage .set_description ("Set image filename.");

		optionWidth .set_short_name ('w');
		optionWidth .set_long_name ("width");
		optionWidth .set_arg_description ("WIDTH");
		optionWidth .set_description ("Set image width.");

		optionHeight .set_short_name ('h');
		optionHeight .set_long_name ("height");
		optionHeight .set_arg_description ("HEIGHT");
		optionHeight .set_description ("Set image height.");

		optionAlphaChannel .set_short_name ('c');
		optionAlphaChannel .set_long_name ("alpha-channel");
		optionAlphaChannel .set_description ("Set whether the image has an alpha channel.");

		optionAntialiasing .set_short_name ('a');
		optionAntialiasing .set_long_name ("antialiasing");
		optionAntialiasing .set_arg_description ("SAMPLES");
		optionAntialiasing .set_description ("Set image antialiasing samples.");

		optionExportFilename .set_short_name ('e');
		optionExportFilename .set_long_name ("export");
		optionExportFilename .set_arg_description ("FILENAME");
		optionExportFilename .set_description ("Set export filename, if '-' is given the output goes to STDOUT.");

		optionExportStyle .set_short_name ('s');
		optionExportStyle .set_long_name ("style");
		optionExportStyle .set_arg_description ("nicest|compact|small|smallest");
		optionExportStyle .set_description ("Set output style for export.");

		optionExportInfer .set_short_name ('n');
		optionExportInfer .set_long_name ("infer");
		optionExportInfer .set_description ("Infer profile and components.");

		optionExportRemoveMetadata .set_short_name ('m');
		optionExportRemoveMetadata .set_long_name ("remove-metadata");
		optionExportRemoveMetadata .set_description ("Set whether to export metadata.");

		optionList .set_short_name ('l');
		optionList .set_long_name ("list");
		optionList .set_arg_description ("profiles|components|nodes|fields");
		optionList .set_description ("Get a list of all supported profiles, components, nodes or fields.");

		set_description ("Copyright 2010 Holger Seelig <holger.seelig@yahoo.de>. License GPLv3+");

		mainGroup   .add_entry (optionHelp,                 help);
		imageGroup  .add_entry (optionExportImage,          imageFilename);
		imageGroup  .add_entry (optionWidth,                width);
		imageGroup  .add_entry (optionHeight,               height);
		imageGroup  .add_entry (optionAlphaChannel,         alphaChannel);
		imageGroup  .add_entry (optionAntialiasing,         antialiasing);
		exportGroup .add_entry (optionExportFilename,       exportFilename);
		exportGroup .add_entry (optionExportStyle,          exportStyle);
		exportGroup .add_entry (optionExportInfer,          exportInfer);
		exportGroup .add_entry (optionExportRemoveMetadata, exportRemoveMetadata);
		listGroup   .add_entry (optionList,                 list);

		set_main_group (mainGroup);
		add_group (imageGroup);
		add_group (exportGroup);
		add_group (listGroup);

		parse (argc, argv);

		for (int i = 1; i < argc; ++ i)
			filenames .emplace_back (argv [i]);
	}

	std::vector <Glib::ustring> filenames;
	Glib::ustring               imageFilename;
	int32_t                     width;
	int32_t                     height;
	bool                        alphaChannel;
	int32_t                     antialiasing;
	Glib::ustring               exportFilename;
	Glib::ustring               exportStyle;
	bool                        exportInfer;
	bool                        exportRemoveMetadata;
	Glib::ustring               list;
	bool                        help;

};

} // puck
} // titania

#endif
