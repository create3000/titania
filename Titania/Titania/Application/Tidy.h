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

#ifndef __TITANIA_TIDY_H__
#define __TITANIA_TIDY_H__

#include <Titania/OS.h>
#include <Titania/String/to_string.h>
#include <Titania/X3D.h>
#include <Titania/Stream/GZStream.h>

#include <iostream>

namespace titania {
namespace puck {

///  It never hurts to be tidy.
class Tidy
{
public:

	static
	int
	main (const ApplicationOptions & options)
	{
		if (options .filenames .empty ())
			throw std::runtime_error ("Expected a filename.");

		basic::uri inputFilename (options .filenames .front ());
		basic::uri outputFilename (options .exportFilename);

		if (inputFilename .is_relative ())
			inputFilename = basic::uri (os::cwd ()) .transform (inputFilename);

		const auto browser = X3D::getBrowser ();
		const auto scene   = browser -> createX3DFromURL ({ inputFilename .str () });

		if (outputFilename == "-" or outputFilename == outputFilename .suffix ())
		{
			const auto suffix = outputFilename == "-" ? inputFilename .suffix () : outputFilename .suffix ();

			X3D::Generator::Style (std::cout, options .exportStyle);
	
			if (suffix == ".x3d")
				std::cout << X3D::XMLEncode (scene);

			else if (suffix == ".json")
				std::cout << X3D::JSONEncode (scene);

			else
			{
				if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
					scene -> setSpecificationVersion (X3D::LATEST_VERSION);

				std::cout << scene;
			}
		}
		else
		{
			if (outputFilename .is_relative ())
				outputFilename = basic::uri (os::cwd ()) .transform (outputFilename);

			std::string tmpFilename = "/tmp/titania-XXXXXX" + outputFilename .suffix ();

			try
			{
				auto ofstream = os::mkstemps (tmpFilename, outputFilename .suffix () .size ());

				X3D::Generator::Style (ofstream, options .exportStyle);
	
				if (not ofstream)
					throw std::runtime_error ("Couldn't save file.");

				// Create temp file

				if (outputFilename .suffix () == ".x3dz")
				{
					basic::ogzstream ogzstream (tmpFilename);

					X3D::Generator::Style (ogzstream, options .exportStyle);
	
					ogzstream << X3D::XMLEncode (scene);
				}
				else if (outputFilename .suffix () == ".x3d")
					ofstream << X3D::XMLEncode (scene);

				else if (outputFilename .suffix () == ".json")
					ofstream << X3D::JSONEncode (scene);

				else if (outputFilename .suffix () == ".x3dvz")
				{
					if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
						scene -> setSpecificationVersion (X3D::LATEST_VERSION);

					basic::ogzstream ogzstream (tmpFilename);

					X3D::Generator::Style (ogzstream, options .exportStyle);
	
					ogzstream << scene;
				}
				else
				{
					if (scene -> getSpecificationVersion () == X3D::VRML_V2_0)
						scene -> setSpecificationVersion (X3D::LATEST_VERSION);

					ofstream << scene;
				}

				// Replace original file.

				os::rename (tmpFilename, outputFilename .path ());
			}
			catch (...)
			{
				os::unlink (tmpFilename);
				throw;
			}
		}

		return 0;
	}

};

} // puck
} // titania

#endif
