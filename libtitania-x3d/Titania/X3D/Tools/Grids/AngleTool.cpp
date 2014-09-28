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

#include "AngleTool.h"

#include "../../Bits/config.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string AngleTool::componentName  = "Grids";
const std::string AngleTool::typeName       = "AngleTool";
const std::string AngleTool::containerField = "grid";

AngleTool::Fields::Fields () :
	      dimension (new MFInt32 ({ 5, 16 })),
	 majorLineEvery (new MFInt32 ({ 5, 2 })),
	majorLineOffset (new MFInt32 ({ 0, 0 }))
{ }

AngleTool::AngleTool (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGridTool (),
	     fields ()
{
	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "translation",     translation ());
	addField (inputOutput, "rotation",        rotation ());
	addField (inputOutput, "scale",           scale ());
	addField (inputOutput, "dimension",       dimension ());
	addField (inputOutput, "majorLineEvery",  majorLineEvery ());
	addField (inputOutput, "majorLineOffset", majorLineOffset ());
	addField (inputOutput, "color",           color ());
	addField (inputOutput, "lineColor",       lineColor ());
	addField (inputOutput, "majorLineColor",  majorLineColor ());
}

X3DBaseNode*
AngleTool::create (X3DExecutionContext* const executionContext) const
{
	return new AngleTool (executionContext);
}

void
AngleTool::initialize ()
{
	X3DGridTool::initialize ();

	requestAsyncLoad ({ get_tool ("AngleTool.x3dv") .str () });
}

void
AngleTool::realize ()
{
	X3DGridTool::realize ();

	try
	{
		auto & set_dimension = getToolNode () -> getField <MFInt32> ("set_dimension");
		dimension ()  .addInterest (set_dimension);
		set_dimension .addInterest (dimension ());
		set_dimension .addEvent (dimension ());

		auto & set_majorLineEvery = getToolNode () -> getField <MFInt32> ("set_majorLineEvery");
		majorLineEvery ()  .addInterest (set_majorLineEvery);
		set_majorLineEvery .addInterest (majorLineEvery ());
		set_majorLineEvery .addEvent (majorLineEvery ());

		auto & set_majorLineOffset = getToolNode () -> getField <MFInt32> ("set_majorLineOffset");
		majorLineOffset ()  .addInterest (set_majorLineOffset);
		set_majorLineOffset .addInterest (majorLineOffset ());
		set_majorLineOffset .addEvent (majorLineOffset ());
	}
	catch (const X3DError & error)
	{ }
}

} // X3D
} // titania
