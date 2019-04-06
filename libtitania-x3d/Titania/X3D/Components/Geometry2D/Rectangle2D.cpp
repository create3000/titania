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

#include "Rectangle2D.h"

#include "../../Browser/Geometry2D/Rectangle2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Execution/X3DExecutionContext.h"


namespace titania {
namespace X3D {

const Component   Rectangle2D::component      = Component ("Geometry2D", 1);
const std::string Rectangle2D::typeName       = "Rectangle2D";
const std::string Rectangle2D::containerField = "geometry";

Rectangle2D::Fields::Fields () :
	 size (new SFVec2f (2, 2)),
	solid (new SFBool ())
{ }

Rectangle2D::Rectangle2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Rectangle2D);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "size",     size ());
	addField (initializeOnly, "solid",    solid ());

	size () .setUnit (UnitCategory::LENGTH);

	setGeometryType (2);
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
}

Box3d
Rectangle2D::createBBox () const
{
	return Box3d (Vector3d (size () .getX (), size () .getY (), 0), Vector3d ());
}

void
Rectangle2D::build ()
{
	const auto & options = getBrowser () -> getRectangle2DOptions ();

	getMultiTexCoords () .emplace_back ();

	getTexCoords () = options -> getTexCoords ();
	getNormals   () = options -> getNormals  ();

	if (size () == Vector2f (2, 2))
	{
		getVertices () = options -> getVertices ();
	}
	else
	{
		const auto size1_2 = Vector3d (size () .getX (), size () .getY (), 0) / 2.0;

		for (const auto & vertex : options -> getVertices ())
			getVertices () .emplace_back (vertex * size1_2);
	}

	addElements (options -> getVertexMode (), getVertices () .size ());
	setSolid (solid ());
}

SFNode
Rectangle2D::toPrimitive () const
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Rectangle2D::toPrimitive");

	const auto & options  = getBrowser () -> getRectangle2DOptions ();
	const auto   geometry = options -> toPrimitive (getExecutionContext ());

	geometry -> getField <SFBool> ("solid") = solid ();

	if (size () not_eq Vector2f (2, 2))
	{
		const auto size1_2 = Vector3f (size () .getX (), size () .getY (), 0) / 2.0f;

		for (MFVec3f::reference point : geometry -> getField <SFNode> ("coord") -> getField <MFVec3f> ("point"))
			point = point .get () * size1_2;
	}

	return geometry;
}

} // X3D
} // titania
