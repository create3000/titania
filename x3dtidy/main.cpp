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

#include <iostream>

#include <Titania/X3D.h>
#include <gtkmm.h>
#include <unistd.h>

#include "anyoption.h"

using namespace titania;

int
main (int argc, char** argv)
{
	AnyOption options;

	options .addUsage ("NAME");
	options .addUsage ("       x3dtidy - X3D/VRML beautifer");
	options .addUsage ("");
	options .addUsage ("SYNOPSIS");
	options .addUsage ("       x3dtidy [OPTIONS] FILE [OUTFILE]");
	options .addUsage ("");
	options .addUsage ("DESCRIPTION");
	options .addUsage ("       Format FILE to standard output or to OUTFILE.");
	options .addUsage ("");
	options .addUsage ("VRML2 field names and access types are printed when the encoding is VRML,");
	options .addUsage ("otherwise X3D field names and access types are printed.");
	options .addUsage ("When VRML2 field names are printed then this changes:");
	options .addUsage ("Collision .enabled to Collision .collide, LOD .children to LOD .level and");
	options .addUsage ("Switch .children to Switch .level");
	options .addUsage ("");
	options .addUsage ("");
	options .addUsage ("       -c, --compact");
	options .addUsage ("              output in compact style");
	options .addUsage ("");
	options .addUsage ("       -e, --expanded");
	options .addUsage ("              expands nodes, this means all printable fields are printed");
	options .addUsage ("");
	options .addUsage ("       -h, --help");
	options .addUsage ("              prints usage");
	options .addUsage ("");
	options .addUsage ("       -s=mode, --style=mode");
	options .addUsage ("              mode can be 'smallest', 'small ', 'compact', or 'nicest'");
	options .addUsage ("              nicest is the default mode");
	options .addUsage ("");
	options .addUsage ("EXAMPLES");
	options .addUsage ("       x3dtidy -s=compact file.wrl");
	options .addUsage ("              Formats file.wrl's contents in compact style mode to standard");
	options .addUsage ("              output.");
	options .addUsage ("");
	options .addUsage ("       x3dtidy file.wrl beautified.wrl");
	options .addUsage ("              Formats file.wrl's contents and saves the output in");
   options .addUsage ("              beautified.wrl.");
	options .addUsage ("");
	options .addUsage ("COPYRIGHT");
	options .addUsage ("       Copyright \xc2\xa9 2010 Holger Seelig.  License GPLv3+:");
	options .addUsage ("       GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
	options .addUsage ("       This is free software: you are free to change and redistribute it.");
	options .addUsage ("       There is NO WARRANTY, to the extent permitted by law.");

	options .setFlag ("compact",  'c');
	options .setFlag ("expanded", 'e');
	options .setFlag ("help",     'h');

	options .setOption ("style", 's');

	options .processCommandArgs (argc, argv);

	if (options .getFlag ("help") or options .getFlag ('h'))
	{
		options .printUsage ();
		return 0;
	}

	if (options .getFlag ("expanded") or options .getFlag ('e'))
		X3D::Generator::ExpandNodes (true);

	if (options .getFlag ("compact") or options .getFlag ('c'))
		X3D::Generator::CompactStyle ();

	if (options .getValue ('s'))
		X3D::Generator::Style (options .getValue ('s'));

	if (options .getValue ("style"))
		X3D::Generator::Style (options .getValue ("style"));

	try
	{
		auto browser = X3D::getBrowser ();

		if (options .getArgc ())
		{
			X3D::MFString url ({ options .getArgv (0) });
		
			if (options .getArgc () > 1)
			{
				std::string tmpFilename = "/tmp/x3dtidy." + std::to_string (getpid ()) + ".wrl";

				try
				{
					std::ofstream file (tmpFilename);

					// Create temp file

					file << browser -> createX3DFromURL (url);

					// Replace original

					rename (tmpFilename .c_str (), options .getArgv (1));
				}
				catch (...)
				{
					unlink (tmpFilename .c_str ());
					throw;
				}
			}
			else
			{
				std::cout << browser -> createX3DFromURL (url);
			}
		}
		else
			options .printUsage ();
	}
	catch (const X3D::X3DError & exception)
	{
		std::cerr << exception .what () << std::endl;
		return 1;
	}
	catch (const Glib::Error & exception)
	{
		std::cerr << exception .what () << std::endl;
		return 1;
	}
	catch (const std::exception & exception)
	{
		std::cerr << exception .what () << std::endl;
		return 1;
	}

	return 0;
}
