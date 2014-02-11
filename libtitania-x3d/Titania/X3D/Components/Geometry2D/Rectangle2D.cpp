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

#include "Rectangle2D.h"

#include "../../Browser/Geometry2D/Rectangle2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Rectangle2D::componentName  = "Geometry2D";
const std::string Rectangle2D::typeName       = "Rectangle2D";
const std::string Rectangle2D::containerField = "geometry";

Rectangle2D::Fields::Fields () :
	 size (new SFVec2f (2, 2)),
	solid (new SFBool (true))
{ }

Rectangle2D::Rectangle2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "size",     size ());
	addField (initializeOnly, "solid",    solid ());
}

X3DBaseNode*
Rectangle2D::create (X3DExecutionContext* const executionContext) const
{
	return new Rectangle2D (executionContext);
}

void
Rectangle2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getBrowserOptions () -> rectangle2D () .addInterest (this, &Rectangle2D::set_properties);
}

void
Rectangle2D::set_properties ()
{
	addEvent ();
}

Box3f
Rectangle2D::createBBox ()
{
	return Box3f (Vector3f (size () .getX (), size () .getY (), 0), Vector3f ());
}

void
Rectangle2D::build ()
{
	const Rectangle2DOptions* const properties = getBrowser () -> getBrowserOptions () -> rectangle2D ();

	size_t elements = solid () ? 1 : 2;

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (elements * properties -> getTexCoords () .size ());
	getTexCoords () [0] = properties -> getTexCoords ();

	getNormals () .reserve (elements * properties -> getNormals  () .size ());
	getNormals () = properties -> getNormals  ();

	getVertices () .reserve (elements * properties -> getVertices () .size ());

	if (size () == Vector2f (2, 2))
		getVertices () = properties -> getVertices ();

	else
	{
		getVertices () .reserve (properties -> getVertices () .size ());

		const auto size1_2 = Vector3f (size () .getX (), size () .getY (), 0) / 2.0f;

		for (const auto & vertex : properties -> getVertices ())
			getVertices () .emplace_back (vertex * size1_2);
	}

	addElements (properties -> getVertexMode (), getVertices () .size ());
	setSolid (true);
	setTextureCoordinate (nullptr);

	if (not solid ())
		addMirrorVertices (properties -> getVertexMode (), true);
}

} // X3D
} // titania
