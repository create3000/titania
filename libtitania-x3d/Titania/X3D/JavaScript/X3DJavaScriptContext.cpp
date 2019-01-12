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

#include "../JavaScript/X3DJavaScriptContext.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Scripting/Script.h"
#include "../Execution/X3DExecutionContext.h"

#include <Titania/String/to_string.h>
#include <Titania/String/trim.h>

namespace titania {
namespace X3D {

X3DJavaScriptContext::X3DJavaScriptContext (Script* const script_, const std::string & ecmascript) :
	X3DBaseNode (),
	     script (script_),
	 ecmascript (ecmascript)
{
	addType (X3DConstants::X3DJavaScriptContext);

	addChildObjects (const_cast <X3DPtr <Script> &> (script));
}

SFBool &
X3DJavaScriptContext::isLive ()
{
	return script -> isLive ();
}

const SFBool &
X3DJavaScriptContext::isLive () const
{
	return script -> isLive ();
}

void
X3DJavaScriptContext::setError (const std::string & message, const std::string & filename, const int32_t lineNumber, const int32_t startColumn, std::string line) const
{
	if (line .empty ())
	{
		// Find error line.

		line = "Couldn't load file!";

		if (not ecmascript .empty ())
		{
			const char nl = (ecmascript .find ('\n', 0) == std::string::npos ? '\r' : '\n');

			std::string::size_type start = 0;
			std::string::size_type end   = 0;

			for (int32_t i = 0; i < lineNumber - 1; ++ i)
			{
				start = ecmascript .find (nl, start);

				if (start == std::string::npos)
					break;
				else
					++ start;
			}

			if (start not_eq std::string::npos)
			{
				if ((end = ecmascript .find (nl, start)) == std::string::npos)
					end = ecmascript .length ();

				line = ecmascript .substr (start, end - start);
			}
		}
	}

	// Pretty print error.
	
	const auto trimmedLine = basic::trim (line);
	const auto spaces      = line .find (trimmedLine);

	getBrowser () -> getConsole () -> error ("\n",
	                                         "#   JavaScript error at line ", lineNumber, (startColumn > 0 ? ':' + basic::to_string (startColumn, std::locale::classic ()) : ""), ":\n",
	                                         "#   in Script '", script -> getName (), "' url '", filename, "',\n",
	                                         "#   world url is '", script -> getExecutionContext () -> getWorldURL (), "'.\n",
	                                         "#   ", "\n",
	                                         "#   ", message, "\n",
	                                         "#      ", trimmedLine, "\n");

	if (startColumn > 0)
		getBrowser () -> getConsole () -> error ("#      " + std::string (startColumn - spaces, ' ') + "^\n");

	getBrowser () -> getConsole () -> error ("\n");
}

} // X3D
} // titania
