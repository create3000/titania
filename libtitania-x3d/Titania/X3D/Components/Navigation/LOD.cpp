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
 ******************************************************************************/

#include "LOD.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

LOD::LOD (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	 X3DGroupingNode (),                                                    
	   level_changed (),                                                    // SFInt32 [out] level_changed
	          center (),                                                    // SFVec3f [ ]   center            0 0 0        (-∞,∞)
	forceTransitions (),                                                    // SFBool  [ ]   forceTransitions  FALSE
	           range ()                                                     // MFFloat [ ]   range             [ ]           [0,∞) or -1
{
	setComponent ("Navigation");
	setTypeName ("LOD");

	appendField (inputOutput,    "metadata",         metadata);
	appendField (initializeOnly, "forceTransitions", forceTransitions);
	appendField (initializeOnly, "center",           center);
	appendField (initializeOnly, "range",            range);
	appendField (outputOnly,     "level_changed",    level_changed);
	appendField (initializeOnly, "bboxSize",         bboxSize);
	appendField (initializeOnly, "bboxCenter",       bboxCenter);
	appendField (inputOnly,      "addChildren",      addChildren);
	appendField (inputOnly,      "removeChildren",   removeChildren);
	appendField (inputOutput,    "children",         children);
	addFieldAlias ("level", "children");
}

X3DBasicNode*
LOD::create (X3DExecutionContext* const executionContext) const
{
	return new LOD (executionContext);
}

int32_t
LOD::getLevel ()
{
	Matrix4f::array_type matrix;

	float cx, cy, cz;

	center .getValue (cx, cy, cz);

	glPushMatrix ();
	glTranslatef (cx, cy, cz);
	glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix ();

	float distance = -matrix [14];

	int32_t level = -1;

	if (range .size ())
	{
		int32_t n = std::min (range .size (), children .size () - 1);

		if (distance < range [0])
			level = 0;
		else
		{
			int32_t i;

			for (i = 0; i < n - 1; ++ i)
			{
				if (range [i] <= distance and distance < range [i + 1])
					level = i + 1;
			}

			if (level == -1)
				level = n;
		}
	}
	else
	{
		level = 0;
	}

	return level;
}

void
LOD::intersect ()
{
	if (not children .size ())
		return;

	int32_t level = getLevel ();

	if (children [level])
		children [level] -> select ();
}

void
LOD::display ()
{
	if (not children .size ())
		return;

	int32_t level = getLevel ();

	if (level_changed not_eq level)
		level_changed = level;

	if (children [level])
		children [level] -> display ();
}

} // X3D
} // titania
