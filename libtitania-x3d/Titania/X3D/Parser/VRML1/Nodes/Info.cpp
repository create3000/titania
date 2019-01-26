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

#include "Info.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Core/MetadataString.h"
#include "../../../Components/Core/WorldInfo.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const Component   Info::component      = Component ("Titania", 1);
const std::string Info::typeName       = "Info";
const std::string Info::containerField = "children";

Info::Fields::Fields () :
	string (new X3D::SFString (""))
{ }

Info::Info (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1Info);

	addField (initializeOnly, "string",   string ());
	addField (initializeOnly, "children", children ());
}

X3D::X3DBaseNode*
Info::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new Info (executionContext);
}

void
Info::convert (Converter* const converter)
{
	// NO USE!
	//if (use (converter))
	//	return;

	// Create nodes.

	if (converter -> transforms .empty ())
	{
		// Create nodes.

		const auto infoNode = converter -> scene -> createNode <X3D::WorldInfo> ();

		// Set name.

		// Assign values.

		infoNode -> title () = "Info";
		infoNode -> info ()  = { string () };

		converter -> scene -> getRootNodes () .emplace_back (infoNode);
	}
	else
	{
		const auto metadataNode = converter -> scene -> createNode <X3D::MetadataString> ();

		// Set name.

		if (not getName () .empty ())
			converter -> scene -> updateNamedNode (getName (), metadataNode);

		// Assign values.

		metadataNode -> name ()      = "Info";
		metadataNode -> reference () = getBrowser () -> getProviderUrl ();
		metadataNode -> value ()     = { string () };

		converter -> groups .back () -> metadata () = metadataNode;
	}
}

Info::~Info ()
{ }

} // VRML1
} // X3D
} // titania
