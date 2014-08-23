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

#include "Shader.h"

#include "../InputOutput/Loader.h"
#include <pcrecpp.h>

namespace titania {
namespace X3D {

std::string
preProcessShaderSource (X3DBaseNode* const node, const std::string & string, const basic::uri & worldURL, const size_t level, std::set <basic::uri> & files)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (not files .insert (worldURL) .second)
		return "";

	if (level > 1024)
		throw Error <INVALID_URL> ("Header inclusion depth limit reached, might be caused by cyclic header inclusion.");

	static const pcrecpp::RE include ("\\A#pragma\\s+X3D\\s+include\\s+\"(.*?)\"$");

	std::istringstream input (string);
	std::ostringstream output;

	input  .imbue (std::locale::classic ());
	output .imbue (std::locale::classic ());

	size_t      lineNumber = 1;
	std::string line;
	
	output << "#line "<< lineNumber << " \"" << worldURL << "#" << node -> getName () << "\""  << std::endl;

	while (std::getline (input, line))
	{
		std::string filename;

		if (include .FullMatch (line, &filename))
		{
			Loader loader (node -> getExecutionContext ());
			output << preProcessShaderSource (node, loader .loadDocument (worldURL .transform (filename)), loader .getWorldURL (), level + 1, files) << std::endl;
			output << "#line "<< lineNumber + 1 << " \"" << worldURL << "\""  << std::endl;
		}
		else
		{
			output << line << std::endl;
		}

		++ lineNumber;
	}

	return output .str ();
}

std::string
preProcessShaderSource (X3DBaseNode* const node, const std::string & string, const basic::uri & worldURL, const size_t level)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	std::set <basic::uri> files;
	
	return preProcessShaderSource (node, string, worldURL, level, files);
}

} // X3D
} // titania
