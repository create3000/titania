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

#include "LineProperties.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

inline
long int
strtol (const char* str, int base)
{
	char* endptr;

	return ::strtol (str, &endptr, base);
}

static std::vector <GLushort> linetypes = {
	strtol ("0000000000000000", 2), // 0 None
	strtol ("1111111111111111", 2), // 1 Solid
	strtol ("1111111110000000", 2), // 2 Dashed
	strtol ("1010101010101010", 2), // 3 Dotted
	strtol ("1111111110001000", 2), // 4 Dashed-dotted
	strtol ("1111100010001000", 2), // 5 Dash-dot-dot

	strtol ("1111111111111111", 2), // 6 (single arrow)
	strtol ("1111111111111111", 2), // 7 (single dot)
	strtol ("1111111111111111", 2), // 8 (double arrow)

	strtol ("1111111100000000", 2), // 9 (stitch line)
	strtol ("1111111000111000", 2), // 10 (chain line)
	strtol ("1111111110011100", 2), // 11 (center line)
	strtol ("1111111111100000", 2), // 12 (hidden line)
	strtol ("1111111011101110", 2)  // 13 (phantom line)

	// 14 (break line - style 1)
	// 15 (break line - style 2)
	// 16 User - specified dash pattern

};

LineProperties::Fields::Fields () :
	             applied (new SFBool (true)),
	            linetype (new SFInt32 (1)),
	linewidthScaleFactor (new SFFloat ())
{ }

LineProperties::LineProperties (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DAppearanceChildNode (),
	                fields ()
{
	setComponent ("Shape");
	setTypeName ("LineProperties");

	addField (inputOutput, "metadata",             metadata ());
	addField (inputOutput, "applied",              applied ());
	addField (inputOutput, "linetype",             linetype ());
	addField (inputOutput, "linewidthScaleFactor", linewidthScaleFactor ());
}

X3DBaseNode*
LineProperties::create (X3DExecutionContext* const executionContext) const
{
	return new LineProperties (executionContext);
}

void
LineProperties::draw ()
{
	if (applied ())
	{
		glEnable (GL_LINE_STIPPLE);

		if (linetype () > 0 and linetype () < (int32_t) linetypes .size ())
			glLineStipple (1, linetypes [linetype ()]);

		else
			glLineStipple (1, linetypes [1]);

		glLineWidth (linewidthScaleFactor ());
	}
	else
	{
		glDisable (GL_LINE_STIPPLE);
		glLineWidth (1);
	}
}

} // X3D
} // titania
