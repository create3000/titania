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

#include <Titania/OS/cwd.h>
#include <Titania/String/to_string.h>
#include <Titania/X3D.h>

#include <iostream>
#include <unistd.h>

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
		try
		{
			X3D::Generator::Style (options .exportStyle);

			auto browser = X3D::getBrowser ();

			basic::uri uri (options .filename);

			if (uri .is_relative ())
				uri = basic::uri (os::cwd ()) .transform (uri);

			if (options .exportFilename == "-")
			{
				if (uri .suffix () == ".x3d")
					std::cout << X3D::XMLEncode (browser -> createX3DFromURL ({ uri .str () }));

				else if (uri .suffix () == ".json")
					std::cout << X3D::JSONEncode (browser -> createX3DFromURL ({ uri .str () }));

				else
					std::cout << browser -> createX3DFromURL ({ uri .str () });
			}
			else
			{
				basic::uri out (options .exportFilename);

				if (out .is_relative ())
					out = basic::uri (os::cwd ()) .transform (out);

				std::string tmpFilename = "/tmp/x3dtidy." + basic::to_string (getpid (), std::locale::classic ()) + out .suffix ();

				try
				{
					std::ofstream file (tmpFilename);

					// Create temp file

					if (out .suffix () == ".x3d")
						file << X3D::XMLEncode (browser -> createX3DFromURL ({ uri .str () }));

					else if (out .suffix () == ".json")
						file << X3D::JSONEncode (browser -> createX3DFromURL ({ uri .str () }));

					else
						file << browser -> createX3DFromURL ({ uri .str () });

					// Replace original

					rename (tmpFilename .c_str (), out .path () .c_str ());
				}
				catch (...)
				{
					unlink (tmpFilename .c_str ());
					throw;
				}
			}
		}
		catch (const X3D::X3DError & error)
		{
			std::cerr << error .what () << std::endl;
			return 1;
		}

		return 0;
	}

};

} // puck
} // titania

#endif
