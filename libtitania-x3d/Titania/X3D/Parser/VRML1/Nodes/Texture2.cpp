/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Texture2.h"

#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Texturing/ImageTexture.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const ComponentType Texture2::component      = ComponentType::TITANIA;
const std::string   Texture2::typeName       = "Texture2";
const std::string   Texture2::containerField = "children";

Texture2::Fields::Fields () :
	filename (new X3D::SFString ("")),
	   image (new X3D::SFImage ()),
	   wrapS (new X3D::SFString ("REPEAT")),
	   wrapT (new X3D::SFString ("REPEAT"))
{ }

Texture2::Texture2 (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1Texture2);

	addField (initializeOnly, "filename", filename ());
	addField (initializeOnly, "image",    image ());
	addField (initializeOnly, "wrapS",    wrapS ());
	addField (initializeOnly, "wrapT",    wrapT ());
	addField (initializeOnly, "children", children ());
}

X3D::X3DBaseNode*
Texture2::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new Texture2 (executionContext);
}

bool
Texture2::use (Converter* const converter)
{
	try
	{
		if (getName () .empty ())
			return false;

		converter -> textures .emplace_back (converter -> scene -> getNamedNode (getName ()));

		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

void
Texture2::convert (Converter* const converter)
{
	if (use (converter))
		return;

	// Create nodes.

	const auto textureNode = converter -> scene -> createNode <X3D::ImageTexture> ();

	// Set name.

	if (not getName () .empty ())
		converter -> scene -> updateNamedNode (getName (), textureNode);

	// Assign values.

	textureNode -> url ()     = { filename () };
	textureNode -> repeatS () = wrapS () == "CLAMP" ? false : true;
	textureNode -> repeatT () = wrapT () == "CLAMP" ? false : true;

	converter -> textures .emplace_back (textureNode);
}

Texture2::~Texture2 ()
{ }

} // VRML1
} // X3D
} // titania
