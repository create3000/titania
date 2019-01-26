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

#include "Texture2Transform.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Texturing/TextureTransform.h"
#include "../../../Execution/X3DExecutionContext.h"
#include "../Converter.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const Component   Texture2Transform::component      = Component ("Titania", 1);
const std::string Texture2Transform::typeName       = "Texture2Transform";
const std::string Texture2Transform::containerField = "children";

Texture2Transform::Fields::Fields () :
	translation (new X3D::SFVec2f (0, 0)),
	   rotation (new X3D::SFFloat (0)),
	scaleFactor (new X3D::SFVec2f (1, 1)),
	     center (new X3D::SFVec2f (0, 0))
{ }

Texture2Transform::Texture2Transform (X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       VRML1Node (),
	          fields ()
{
	//addType (X3D::X3DConstants::VRML1Texture2Transform);

	addField (initializeOnly, "translation", translation ());
	addField (initializeOnly, "rotation",    rotation ());
	addField (initializeOnly, "scaleFactor", scaleFactor ());
	addField (initializeOnly, "center",      center ());
	addField (initializeOnly, "children",    children ());
}

X3D::X3DBaseNode*
Texture2Transform::create (X3D::X3DExecutionContext* const executionContext) const
{
	return new Texture2Transform (executionContext);
}

bool
Texture2Transform::use (Converter* const converter)
{
	try
	{
		if (getName () .empty ())
			return false;

		converter -> textureTransforms .emplace_back (converter -> scene -> getNamedNode (getName ()));

		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

void
Texture2Transform::convert (Converter* const converter)
{
	if (use (converter))
		return;

	// Create nodes.

	const auto textureTransformNode = converter -> scene -> createNode <X3D::TextureTransform> ();

	// Set name.

	if (not getName () .empty ())
		converter -> scene -> updateNamedNode (getName (), textureTransformNode);

	// Assign values.

	textureTransformNode -> translation () = translation ();
	textureTransformNode -> rotation ()    = rotation ();
	textureTransformNode -> scale ()       = scaleFactor ();
	textureTransformNode -> center ()      = center ();

	converter -> textureTransforms .emplace_back (textureTransformNode);
}

Texture2Transform::~Texture2Transform ()
{ }

} // VRML1
} // X3D
} // titania
