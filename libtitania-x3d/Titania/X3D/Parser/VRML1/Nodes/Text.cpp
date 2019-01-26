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

#include "Text.h"

#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const Component   Text::component      = Component ("Titania", 1);
const std::string Text::typeName       = "Text";
const std::string Text::containerField = "children";

Text::Fields::Fields () :
	       string (new X3D::MFString ({ "" })),
	    direction (new X3D::SFString ("LR")),
	justification (new X3D::SFString ("BEGIN")),
	      spacing (new X3D::SFFloat (1)),
	        width (new X3D::MFFloat ({ 0 }))
{ }

Text::Text (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1Text);

	addField (initializeOnly, "string", string ());
	addField (initializeOnly, "direction", direction ());
	addField (initializeOnly, "justification", justification ());
	addField (initializeOnly, "spacing", spacing ());
	addField (initializeOnly, "width", width ());
	addField (initializeOnly, "children", children ());
}

X3D::X3DBaseNode*
Text::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new Text (executionContext);
}

void
Text::convert (Converter* const converter)
{ }

Text::~Text ()
{ }

} // VRML1
} // X3D
} // titania
