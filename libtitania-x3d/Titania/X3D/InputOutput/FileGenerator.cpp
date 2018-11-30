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

#include "FileGenerator.h"

#include <Titania/Stream/GZStream.h>

#include <glibmm.h>

#include <iostream>

namespace titania {
namespace X3D {

const std::set <std::string> FileGenerator::knowFileTypes = {
	".x3d",
	".x3dz",
	".x3dj",
	".x3dv",
	".x3dvz",
	".wrl",
	".vrml",
	".vrm",
	".wrz",
};

///  throws Error <INVALID_URL> Error <NOT_SUPPORTED>, Error <DISPOSED> std::exception
void
FileGenerator::write (std::ostream & ostream, const X3DScenePtr & scene, const std::string & extension, const std::string & outputStyle, const bool metadata)
{
	using GeneratorCallback = std::function <bool (std::ostream &, const X3DScenePtr &)>;

	static const std::map <std::string, GeneratorCallback> generators = {
		std::pair (".x3d",   generate_x3d),
		std::pair (".x3dz",  generate_x3d),
		std::pair (".x3dj",  generate_x3dj),
		std::pair (".x3dv",  generate_x3dv),
		std::pair (".x3dvz", generate_x3dv),
		std::pair (".wrl",   generate_wrl),
		std::pair (".vrml",  generate_wrl),
		std::pair (".vrm",   generate_wrl),
		std::pair (".wrz",   generate_wrl),
	};

	Generator::Style    (ostream, outputStyle);
	Generator::MetaData (ostream, metadata);

	bool saved = false;

	try
	{
		if (ostream)
			saved = generators .at (extension) (ostream, scene);
	}
	catch (const std::out_of_range &)
	{
		throw Error <NOT_SUPPORTED> ("Invalid Type: Couldn't save scene as '" + extension + "'.");
	}

	if (not saved)
		throw Error <INVALID_URL> ("Invalid URL: Couldn't save scene to output stream.");
}

///  throws Error <INVALID_URL> Error <NOT_SUPPORTED>, Error <DISPOSED> std::exception
void
FileGenerator::write (const X3DScenePtr & scene, basic::uri worldURL, const std::string & outputStyle, const bool metadata)
{
	static const std::map <std::string, bool> compressed = {
		std::pair (".x3d",   false),
		std::pair (".x3dz",  true),
		std::pair (".x3dj",  false),
		std::pair (".x3dv",  false),
		std::pair (".x3dvz", true),
		std::pair (".wrl",   false),
		std::pair (".vrml",  false),
		std::pair (".vrm",   false),
		std::pair (".wrz",   true),
	};

	if (worldURL .is_relative ())
		worldURL = basic::uri ("file://" + basic::path (Glib::get_current_dir (), "/") .escape () .str () + "/") .transform (worldURL);

	if (not worldURL .is_local ())
		throw Error <INVALID_URL> ("Invalid URL: URL is not local.");

	try
	{
		try
		{
			if (compressed .at (worldURL .extension ()))
			{
				basic::ogzstream ostream (worldURL .path ());
		
				write (ostream, scene, worldURL .extension (), outputStyle, metadata);
			}
			else
			{
				std::ofstream ostream (worldURL .path ());
		
				write (ostream, scene, worldURL .extension (), outputStyle, metadata);
			}
		}
		catch (const std::out_of_range &)
		{
			throw Error <NOT_SUPPORTED> ("Invalid Type: Couldn't save scene as '" + worldURL .extension () + "'.");
		}
	}
	catch (const Error <INVALID_URL> &)
	{
		throw Error <INVALID_URL> ("Invalid URL: Couldn't save scene to '" + worldURL +  "'.");
	}
}

bool
FileGenerator::generate_x3d (std::ostream & ostream, const X3DScenePtr & scene)
{
	if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
	{
		scene -> setEncoding (X3D::EncodingType::XML);
		scene -> setSpecificationVersion (X3D::LATEST_VERSION);
	}

	ostream << XMLEncode (scene);

	return not ostream .fail ();
}

bool
FileGenerator::generate_x3dj (std::ostream & ostream, const X3DScenePtr & scene)
{
	if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
	{
		scene -> setEncoding (X3D::EncodingType::XML);
		scene -> setSpecificationVersion (X3D::LATEST_VERSION);
	}

	ostream << JSONEncode (scene);

	return not ostream .fail ();
}

bool
FileGenerator::generate_x3dv (std::ostream & ostream, const X3DScenePtr & scene)
{
	if (scene -> getSpecificationVersion () == VRML_V2_0)
		scene -> setSpecificationVersion (LATEST_VERSION);

	ostream << scene;

	return not ostream .fail ();
}

bool
FileGenerator::generate_wrl (std::ostream & ostream, const X3DScenePtr & scene)
{
	if (scene -> getSpecificationVersion () not_eq X3D::VRML_V2_0)
	{
		scene -> setEncoding (X3D::EncodingType::VRML);
		scene -> setSpecificationVersion (X3D::VRML_V2_0);
	}

	ostream << scene;

	return not ostream .fail ();
}

} // X3D
} // titania
