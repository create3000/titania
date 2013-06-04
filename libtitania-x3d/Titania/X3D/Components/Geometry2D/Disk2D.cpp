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

#include "Disk2D.h"

#include "../../Browser/Geometry2D/Disk2DProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

Disk2D::Fields::Fields () :
	innerRadius (new SFFloat ()),
	outerRadius (new SFFloat (1)),
	solid (new SFBool (true))
{ }

Disk2D::Disk2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	         fields ()                                                     
{
	setComponent ("Geometry2D");
	setTypeName ("Disk2D");

	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "innerRadius", innerRadius ());
	addField (initializeOnly, "outerRadius", outerRadius ());
	addField (initializeOnly, "solid",       solid ());
}

X3DBaseNode*
Disk2D::create (X3DExecutionContext* const executionContext) const
{
	return new Disk2D (executionContext);
}

void
Disk2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getBrowserOptions () -> disc2DProperties () .addInterest (this, &Disk2D::set_properties);
}

Box3f
Disk2D::createBBox ()
{
	auto radius = std::max (innerRadius (), outerRadius ());

	return Box3f (Vector3f (std::abs (radius), std::abs (radius), 0), Vector3f ());
}

void
Disk2D::set_properties ()
{
	update ();
}

void
Disk2D::build ()
{
	const Disk2DProperties* properties = getBrowser () -> getBrowserOptions () -> disc2DProperties () .getValue ();

	if (innerRadius () == outerRadius ())
	{
		auto radius = std::abs (outerRadius ());

		// Circle

		if (radius == 1.0f)
			getVertices () = properties -> getVertices ();

		else
		{
			getVertices () .reserve (properties -> getVertices () .size ());

			for (const auto & vertex : properties -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (GL_LINE_LOOP, getVertices () .size ());
		setSolid (false);

		return;
	}

	if (innerRadius () == 0.0f or outerRadius () == 0.0f)
	{
		auto radius = std::abs (std::max (innerRadius (), outerRadius ()));

		// Disk

		size_t elements = solid () ? 1 : 2;

		getTexCoord () .reserve (elements * properties -> getTexCoord () .size ());
		getNormals  () .reserve (elements * properties -> getNormals  () .size ());
		getVertices () .reserve (elements * properties -> getVertices () .size ());

		getTexCoord () = properties -> getTexCoord ();
		getNormals  () = properties -> getNormals  ();

		if (radius == 1.0f)
			getVertices () = properties -> getVertices ();

		else
		{
			getVertices () .reserve (properties -> getVertices () .size ());

			for (const auto & vertex : properties -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (properties -> getVertexMode (), getVertices () .size ());
		setSolid (true);

		if (not solid ())
			addMirrorVertices (properties -> getVertexMode (), true);

		return;
	}

	// Disk with hole

	size_t elements = solid () ? 1 : 2;

	getTexCoord () .reserve (elements * (properties -> getTexCoord () .size () + 2));
	getNormals  () .reserve (elements * (properties -> getNormals  () .size () + 2));
	getVertices () .reserve (elements * (properties -> getVertices () .size () + 2));

	// Texture Coordinates

	auto maxRadius = std::abs (std::max (innerRadius (), outerRadius ()));
	auto minRadius = std::abs (std::min (innerRadius (), outerRadius ()));
	auto scale     = minRadius / maxRadius;

	for (const auto & texCoord : properties -> getTexCoord ())
	{
		getTexCoord () .emplace_back (texCoord * scale + Vector3f ((1 - scale) / 2, (1 - scale) / 2, 0));
		getTexCoord () .emplace_back (texCoord);
	}

	// Normals

	for (const auto & normal : properties -> getNormals ())
	{
		getNormals () .emplace_back (normal);
		getNormals () .emplace_back (normal);
	}

	// Vertices

	for (const auto & vertex : properties -> getVertices ())
	{
		getVertices () .emplace_back (vertex * minRadius);
		getVertices () .emplace_back (vertex * maxRadius);
	}

	// The last two vertices are the first two.
	getTexCoord () .emplace_back (getTexCoord () [0]);
	getTexCoord () .emplace_back (getTexCoord () [1]);
	getNormals  () .emplace_back (getNormals  () [0]);
	getNormals  () .emplace_back (getNormals  () [1]);
	getVertices () .emplace_back (getVertices () [0]);
	getVertices () .emplace_back (getVertices () [1]);

	addElements (GL_QUAD_STRIP, getVertices () .size ());
	setSolid (true);

	if (not solid ())
		addMirrorVertices (GL_QUAD_STRIP, true);
}

void
Disk2D::draw ()
{
	if (innerRadius () == outerRadius ())
		glDisable (GL_LIGHTING);

	X3DGeometryNode::draw ();
}

void
Disk2D::dispose ()
{
	getBrowser () -> getBrowserOptions () -> disc2DProperties () .removeInterest (this, &Disk2D::set_properties);

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
