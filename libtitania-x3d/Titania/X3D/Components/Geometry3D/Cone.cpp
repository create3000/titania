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

#include "Cone.h"

#include "../../Browser/Geometry3D/ConeOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace X3D {

const ComponentType Cone::component      = ComponentType::GEOMETRY_3D;
const std::string   Cone::typeName       = "Cone";
const std::string   Cone::containerField = "geometry";

Cone::Fields::Fields () :
	      bottom (new SFBool (true)),
	bottomRadius (new SFFloat (1)),
	      height (new SFFloat (2)),
	        side (new SFBool (true)),
	       solid (new SFBool (true))
{ }

Cone::Cone (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Cone);

	addField (inputOutput,    "metadata",     metadata ());
	addField (initializeOnly, "side",         side ());
	addField (initializeOnly, "bottom",       bottom ());
	addField (initializeOnly, "height",       height ());
	addField (initializeOnly, "bottomRadius", bottomRadius ());
	addField (initializeOnly, "solid",        solid ());
}

X3DBaseNode*
Cone::create (X3DExecutionContext* const executionContext) const
{
	return new Cone (executionContext);
}

void
Cone::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getCylinderOptions () .addInterest (this, &Cone::update);
}

void
Cone::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> getCylinderOptions () .removeInterest (this, &Cone::update);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getCylinderOptions () .addInterest (this, &Cone::update);
}

Box3f
Cone::createBBox ()
{
	const float diameter = bottomRadius () * 2;

	if (not side () and not bottom ())
		return Box3f ();

	else if (not side ())
		return Box3f (Vector3f (diameter, 0, diameter), Vector3f (0, -height () / 2, 0));

	else
		return Box3f (Vector3f (diameter, height (), diameter), Vector3f ());
}

void
Cone::build ()
{
	const auto & options    = getBrowser () -> getConeOptions ();
	const float  vDimension = options -> vDimension ();

	getTexCoords () .emplace_back ();

	const float y1 = height () / 2;
	const float y2 = -y1;
	const auto  nz = std::polar <float> (1, -M_PI / 2 + std::atan (bottomRadius () / height ()));

	if (side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const float u1     = (i + 0.5f) / vDimension;
			const float theta1 = 2 * M_PI * u1;
			const auto  n1     = std::polar <float> (nz .imag (), theta1);

			const float u2     = i / vDimension;
			const float theta2 = 2 * M_PI * u2;
			const auto  p2     = std::polar <float> (-bottomRadius (), theta2);
			const auto  n2     = std::polar <float> (nz .imag (), theta2);

			const float u3     = (i + 1) / vDimension;
			const float theta3 = 2 * M_PI * u3;
			const auto  p3     = std::polar <float> (-bottomRadius (), theta3);
			const auto  n3     = std::polar <float> (nz .imag (), theta3);

			/*    p1
			 *   /  \
			 *  /    \
			 * p2 -- p3
			 */

			// p1
			getTexCoords () [0] .emplace_back (u1, 1, 0, 1);
			getNormals  () .emplace_back (n1 .imag (), nz .real (), n1 .real ());
			getVertices () .emplace_back (0, y1, 0);

			// p2
			getTexCoords () [0] .emplace_back (u2, 0, 0, 1);
			getNormals  () .emplace_back (n2 .imag (), nz .real (), n2 .real ());
			getVertices () .emplace_back (p2 .imag (), y2, p2 .real ());

			// p3
			getTexCoords () [0] .emplace_back (u3, 0, 0, 1);
			getNormals  () .emplace_back (n3 .imag (), nz .real (), n3 .real ());
			getVertices () .emplace_back (p3 .imag (), y2, p3 .real ());
		}

		addElements (GL_TRIANGLES, getVertices () .size ());
	}

	if (bottom ())
	{
		for (int32_t i = vDimension - 1; i > -1; -- i)
		{
			const float u     = i / vDimension;
			const float theta = 2 * M_PI * u;
			auto        p     = std::polar <float> (-1, theta);

			getTexCoords () [0] .emplace_back ((p .imag () + 1) / 2, (p .real () + 1) / 2, 0, 1);
			getNormals  () .emplace_back (0, -1, 0);

			p *= bottomRadius ();
			getVertices () .emplace_back (p .imag (), y2, p .real ());
		}

		addElements (GL_POLYGON, vDimension);
	}

	setSolid (solid ());
	setTextureCoordinate (nullptr);
}

SFNode
Cone::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto & options    = getBrowser () -> getConeOptions ();
	const float  vDimension = options -> vDimension ();

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;

	geometry -> metadata ()    = metadata ();
	geometry -> solid ()       = solid ();
	geometry -> creaseAngle () = 1;

	const float y1 = height () / 2;
	const float y2 = -y1;

	if (side ())
	{
		coord -> point () .emplace_back (0, y1, 0);

		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const float u1 = (i + 0.5f) / vDimension;
			const float u2 = i / vDimension;

			texCoord -> point () .emplace_back (u1, 1);
			texCoord -> point () .emplace_back (u2, 0);
		}

		texCoord -> point () .emplace_back (1, 0);
	}

	if (bottom () or side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const float u     = i / vDimension;
			const float theta = 2 * M_PI * u;
			auto        p     = std::polar <float> (-1, theta);

			if (bottom ())
				texCoord -> point () .emplace_back ((p .imag () + 1) / 2, (p .real () + 1) / 2);

			p *= bottomRadius ();
			coord -> point () .emplace_back (p .imag (), y2, p .real ());
		}
	}

	int32_t c = 0;
	int32_t t = 0;

	if (side ())
	{
		for (int32_t i = 1; i < vDimension; ++ i)
		{
			const int32_t i2 = i * 2;

			geometry -> texCoordIndex () .emplace_back (i2 - 2);
			geometry -> texCoordIndex () .emplace_back (i2 - 1);
			geometry -> texCoordIndex () .emplace_back (i2 + 1);
			geometry -> texCoordIndex () .emplace_back (-1);

			geometry -> coordIndex () .emplace_back (0);
			geometry -> coordIndex () .emplace_back (i);
			geometry -> coordIndex () .emplace_back (i + 1);
			geometry -> coordIndex () .emplace_back (-1);
		}

		const int32_t vDimension2 = vDimension * 2;

		geometry -> texCoordIndex () .emplace_back (vDimension2 - 2);
		geometry -> texCoordIndex () .emplace_back (vDimension2 - 1);
		geometry -> texCoordIndex () .emplace_back (vDimension2);
		geometry -> texCoordIndex () .emplace_back (-1);

		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (vDimension);
		geometry -> coordIndex () .emplace_back (1);
		geometry -> coordIndex () .emplace_back (-1);

		c += 1;
		t += vDimension2 + 1;
	}

	if (bottom ())
	{
		for (int32_t i = 0, ts = t + vDimension - 1, cs = c + vDimension - 1; i < vDimension; ++ i)
		{
			geometry -> texCoordIndex () .emplace_back (ts - i);
			geometry -> coordIndex ()    .emplace_back (cs - i);
		}

		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
