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

#include "Cylinder.h"

#include "../../Browser/Geometry3D/CylinderOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const ComponentType Cylinder::component      = ComponentType::GEOMETRY_3D;
const std::string   Cylinder::typeName       = "Cylinder";
const std::string   Cylinder::containerField = "geometry";

Cylinder::Fields::Fields () :
	bottom (new SFBool (true)),
	height (new SFFloat (2)),
	radius (new SFFloat (1)),
	  side (new SFBool (true)),
	   top (new SFBool (true)),
	 solid (new SFBool (true))
{ }

Cylinder::Cylinder (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Cylinder);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "top",      top ());
	addField (initializeOnly, "bottom",   bottom ());
	addField (initializeOnly, "side",     side ());
	addField (initializeOnly, "height",   height ());
	addField (initializeOnly, "radius",   radius ());
	addField (initializeOnly, "solid",    solid ());
}

X3DBaseNode*
Cylinder::create (X3DExecutionContext* const executionContext) const
{
	return new Cylinder (executionContext);
}

void
Cylinder::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getCylinderOptions () .addInterest (this, &Cylinder::update);
}

void
Cylinder::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> getCylinderOptions () .removeInterest (this, &Cylinder::update);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getCylinderOptions () .addInterest (this, &Cylinder::update);
}

Box3f
Cylinder::createBBox ()
{
	const float diameter = radius () * 2;

	if (not top () and not side () and not bottom ())
		return Box3f ();

	else if (not top () and not side ())
		return Box3f (Vector3f (diameter, 0, diameter), Vector3f (0, -height () / 2, 0));

	else if (not bottom () and not side ())
		return Box3f (Vector3f (diameter, 0, diameter), Vector3f (0, height () / 2, 0));

	else
		return Box3f (Vector3f (diameter, height (), diameter), Vector3f ());
}

void
Cylinder::build ()
{
	const auto & options    = getBrowser () -> getCylinderOptions ();
	const float  uDimension = options -> uDimension ();

	getTexCoords () .emplace_back ();

	const float y1 = height () / 2;
	const float y2 = -y1;

	if (side ())
	{
		for (int32_t i = 0; i < uDimension; ++ i)
		{
			const float u1     = i / uDimension;
			const float theta1 = 2 * M_PI * u1;
			const float x1     = -std::sin (theta1);
			const float z1     = -std::cos (theta1);

			const float u2     = (i + 1) / uDimension;
			const float theta2 = 2 * M_PI * u2;
			const float x2     = -std::sin (theta2);
			const float z2     = -std::cos (theta2);

			// p1 - p4
			//  |   |
			// p2 - p3

			// p1
			getTexCoords () [0] .emplace_back (u1, 1, 0, 1);
			getNormals  () .emplace_back (x1, 0, z1);
			getVertices () .emplace_back (x1 * radius (), y1, z1 * radius ());

			// p2
			getTexCoords () [0] .emplace_back (u1, 0, 0, 1);
			getNormals  () .emplace_back (x1, 0, z1);
			getVertices () .emplace_back (x1 * radius (), y2, z1 * radius ());

			// p3
			getTexCoords () [0] .emplace_back (u2, 0, 0, 1);
			getNormals  () .emplace_back (x2, 0, z2);
			getVertices () .emplace_back (x2 * radius (), y2, z2 * radius ());

			// p4
			getTexCoords () [0] .emplace_back (u2, 1, 0, 1);
			getNormals  () .emplace_back (x2, 0, z2);
			getVertices () .emplace_back (x2 * radius (), y1, z2 * radius ());
		}

		addElements (GL_QUADS, uDimension * 4);
	}

	if (top ())
	{
		for (int32_t i = 0; i < uDimension; ++ i)
		{
			const float u     = i / uDimension;
			const float theta = 2 * M_PI * u;
			const float x     = -std::sin (theta);
			const float z     = -std::cos (theta);

			getTexCoords () [0] .emplace_back ((x + 1) / 2, -(z - 1) / 2, 0, 1);
			getNormals  () .emplace_back (0, 1, 0);
			getVertices () .emplace_back (x * radius (), y1, z * radius ());
		}

		addElements (GL_POLYGON, uDimension);
	}

	if (bottom ())
	{
		for (int32_t i = uDimension - 1; i > -1; -- i)
		{
			const float u     = i / uDimension;
			const float theta = 2 * M_PI * u;
			const float x     = -std::sin (theta);
			const float z     = -std::cos (theta);

			getTexCoords () [0] .emplace_back ((x + 1) / 2, (z + 1) / 2, 0, 1);
			getNormals  () .emplace_back (0, -1, 0);
			getVertices () .emplace_back (x * radius (), y2, z * radius ());
		}

		addElements (GL_POLYGON, uDimension);
	}

	setSolid (solid ());
	setTextureCoordinate (nullptr);
}

SFNode
Cylinder::toPolygonObject () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto & options    = getBrowser () -> getCylinderOptions ();
	const float  uDimension = options -> uDimension ();

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;

	geometry -> getField <SFNode> ("metadata") = metadata ();
	geometry -> getField <SFBool> ("solid")    = solid ();

	const float y1 = height () / 2;
	const float y2 = -y1;

	for (int32_t i = 0; i < uDimension; ++ i)
	{
		const float u     = i / uDimension;
		const float theta = 2 * M_PI * u;
		const float x     = -std::sin (theta);
		const float z     = -std::cos (theta);

		texCoord -> point () .emplace_back ((x + 1) / 2, -(z - 1) / 2);
		coord -> point ()    .emplace_back (x * radius (), y1, z * radius ());
	}

	for (int32_t i = 0; i < uDimension; ++ i)
	{
		const float u     = i / uDimension;
		const float theta = 2 * M_PI * u;
		const float x     = -std::sin (theta);
		const float z     = -std::cos (theta);

		texCoord -> point () .emplace_back ((x + 1) / 2, (z + 1) / 2);
		coord -> point ()    .emplace_back (x * radius (), y2, z * radius ());
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
