/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "../../Applications/BrowserApplication/BrowserApplication.h"
#include "../../Applications/ExportImage/ExportImage.h"
#include "../../Applications/Info/Info.h"
#include "../../Applications/main/CommandOptions.h"
#include "../../Applications/Tidy/Tidy.h"

int
main (int argc, char** argv)
{
	using namespace titania;
	using namespace titania::puck;

	try
	{
		// X3D::Init function must be the first X3D function call in a multi-threaded program, and it must complete before any other X3D call is made.
		X3D::Init (argc, argv);

		Glib::setenv ("UBUNTU_MENUPROXY",  "0");   // Disable global menu. This fixes the bug with images in menu items and with no 'active' event for the scene menu item.

		// Run appropriate application.

		CommandOptions options (argc, argv);

		if (not options .imageFilename .empty ())
			return ExportImage () .main (options);

		if (not options .exportFilename .empty ())
			return Tidy::main (options);

		if (not options .list .empty ())
			return Info::main (options);

		if (options .help)
		{
			std::cout << options .get_help () << std::endl;
			return 0;
		}

		return BrowserApplication::main (argc, argv);
	}
	catch (const Glib::Exception & error)
	{
		std::cerr << error .what () << std::endl;
		return 1;
	}
	catch (const std::exception & error)
	{
		std::cerr << error .what () << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "A strange error occured." << std::endl;
		return 1;
	}
}
