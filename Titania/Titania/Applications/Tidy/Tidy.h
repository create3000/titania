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

#ifndef __TITANIA_COMMANDS_TIDY_H__
#define __TITANIA_COMMANDS_TIDY_H__

#include "../../Applications/main/CommandOptions.h"

#include <Titania/X3D.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>

namespace titania {
namespace puck {

///  It never hurts to be tidy.
class Tidy
{
public:

	static
	int
	main (const CommandOptions & options)
	{
		if (options .filenames .empty ())
			throw std::runtime_error ("Expected a filename.");

		basic::uri inputFilename (options .filenames .front ());
		basic::uri outputFilename (options .exportFilename);

		if (inputFilename .is_relative ())
			inputFilename = basic::uri ("file://" + Glib::get_current_dir () + "/") .transform (inputFilename);

		const auto browser = X3D::getBrowser ();
		const auto scene   = browser -> createX3DFromURL ({ inputFilename .str () });

		if (outputFilename == "-" or outputFilename == outputFilename .extension ())
		{
			const auto extension = outputFilename == "-" ? inputFilename .extension () : outputFilename .extension ();

			X3D::FileGenerator::write (std::cout, scene, extension, options .exportStyle, not options .exportRemoveMetadata);
		}
		else
		{
			if (outputFilename .is_relative ())
				outputFilename = basic::uri ("file://" + Glib::get_current_dir () + "/") .transform (outputFilename);

			X3D::FileGenerator::write (scene, outputFilename, options .exportStyle, not options .exportRemoveMetadata);
		}

		return 0;
	}

};

} // puck
} // titania

#endif
