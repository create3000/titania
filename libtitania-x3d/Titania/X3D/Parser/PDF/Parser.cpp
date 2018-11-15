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

#include "Parser.h"

#include "../SVG/Parser.h"

#include <fstream>

namespace titania {
namespace X3D {
namespace PDF {

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3D::X3DParser (),
	         scene (scene),
	           uri (uri),
	       istream (istream)
{ }

void
Parser::parseIntoScene ()
{
	try
	{
		const auto  pdfFilename = save (istream, ".pdf");
		std::string svgFilename = "/tmp/titania-XXXXXX.svg";
	
		::close (Glib::mkstemp (svgFilename));

		const auto inkscape = Glib::find_program_in_path ("inkscape");

		const auto inkscape_command_line = std::vector <std::string> ({
			inkscape, pdfFilename, "--export-plain-svg=" + svgFilename
		});

		int exit_status = 0;

		Glib::spawn_sync (Glib::get_home_dir (), inkscape_command_line, Glib::SPAWN_DEFAULT, Glib::SlotSpawnChildSetup (), nullptr, nullptr, &exit_status);

		if (exit_status == 0)
		{
			std::ifstream ifstream (svgFilename);
	
			SVG::Parser (scene, uri, ifstream) .parseIntoScene ();
		}

		Gio::File::create_for_path (svgFilename) -> remove ();
		Gio::File::create_for_path (pdfFilename) -> remove ();
	}
	catch (const Glib::Error & error)
	{
		throw Error <X3D::NOT_SUPPORTED> (error .what ());
	}
}

Parser::~Parser ()
{ }

} // PDF
} // X3D
} // titania
