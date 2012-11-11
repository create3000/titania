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
 ******************************************************************************/

#include "Sphere.h"

#include "../../Browser/Properties/QuadSphereProperties.h"
#include "../../Execution/X3DExecutionContext.h"
#include <Titania/Math/Math.h>
#include <complex>
#include <iostream>

namespace titania {
namespace X3D {

SFInt32 Sphere::uDimension (20);
SFInt32 Sphere::vDimension (10);

MFInt32 Sphere::indices    = createIndices    ();
MFInt32 Sphere::texIndices = createTexIndices ();
MFVec2f Sphere::texCoord   = createTexCoord   ();
MFVec3f Sphere::points     = createPoints     ();

Sphere::Sphere (X3DExecutionContext* const executionContext) :
	   X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	         radius (1)                                                    // SFFloat [ ] radius  1           (0,∞)
{
	setComponent ("Geometry3D");
	setTypeName ("Sphere");

	appendField (inputOutput,    "metadata", metadata);
	appendField (initializeOnly, "radius",   radius);
	appendField (initializeOnly, "solid",    solid);

	setChildren (uDimension, vDimension);
}

X3DBasicNode*
Sphere::create (X3DExecutionContext* const executionContext) const
{
	return new Sphere (executionContext);
}

void
Sphere::setProperties (const SFNode <X3DSpherePropertyNode> & properties)
{
	SFNode <QuadSphereProperties> quadSphereProperties = properties;

	if (quadSphereProperties)
	{
		uDimension = quadSphereProperties -> uDimension;
		vDimension = quadSphereProperties -> vDimension;

		std::clog
			<< "Configuring Sphere" << std::endl
			<< "\tuDimension: " << uDimension << std::endl
			<< "\tvDimension: " << vDimension << std::endl;

		indices    = createIndices    ();
		texIndices = createTexIndices ();
		texCoord   = createTexCoord   ();
		points     = createPoints     ();
	}
}

Box3f
Sphere::createBBox ()
{
	float diameter = 2 * radius;

	return Box3f (Vector3f (diameter, diameter, diameter), Vector3f ());
}

MFInt32
Sphere::createIndices ()
{
	MFInt32 indices;

	for (int32_t p = 0, v = 0; v < vDimension - 1; ++ v, ++ p)
	{
		for (int32_t u = 0; u < uDimension - 2; ++ u, ++ p)
		{
			indices .push_back (p);
			indices .push_back (p + uDimension - 1);
			indices .push_back (p + uDimension);
			indices .push_back (p + 1);
		}

		indices .push_back (p);
		indices .push_back (p + uDimension - 1);
		indices .push_back (p + 1);
		indices .push_back (p - uDimension + 2);
	}

	return indices;
}

MFInt32
Sphere::createTexIndices ()
{
	MFInt32 indices;

	for (int32_t p = 0, v = 0; v < vDimension - 1; ++ v, ++ p)
	{
		for (int32_t u = 0; u < uDimension - 1; ++ u, ++ p)
		{
			indices .push_back (p);
			indices .push_back (p + uDimension);
			indices .push_back (p + uDimension + 1);
			indices .push_back (p + 1);
		}
	}

	return indices;
}

MFVec2f
Sphere::createTexCoord ()
{
	MFVec2f texCoord;

	for (int32_t v = 0; v < vDimension; ++ v)
	{
		float y = v / float (vDimension - 1);

		for (int u = 0; u < uDimension - 1; ++ u)
		{
			float x = u / float (uDimension - 1);
			texCoord .push_back (SFVec2f (x, 1 - y));
		}

		texCoord .push_back (SFVec2f (1, 1 - y));
	}

	return texCoord;
}

MFVec3f
Sphere::createPoints ()
{
	MFVec3f points;

	// north pole
	for (int32_t u = 0; u < uDimension - 1; ++ u)
		points .push_back (SFVec3f (0, 1, 0));

	// sphere segments
	for (int32_t v = 1; v < vDimension - 1; ++ v)
	{
		std::complex <float> zPlane = std::polar <float> (1, -M_PI * (v / float (vDimension - 1)));

		for (int32_t u = 0; u < uDimension - 1; ++ u)
		{
			std::complex <float> yPlane = std::polar <float> (zPlane .imag (), (2 * M_PI) * (u / float (uDimension - 1)));

			points .push_back (SFVec3f (yPlane .imag (), zPlane .real (), yPlane .real ()));
		}
	}

	// south pole
	for (int32_t u = 0; u < uDimension - 1; ++ u)
		points .push_back (SFVec3f (0, -1, 0));

	return points;
}

void
Sphere::build ()
{
	X3DGeometryNode::build ();

	MFInt32::const_iterator index;
	MFInt32::const_iterator texIndex;

	for (index = indices .begin (), texIndex = texIndices .begin (); index not_eq indices .end ();)
	{
		for (int i = 0; i < 4; ++ i, ++ index, ++ texIndex)
		{
			auto point = points [*index] * radius;

			getTexCoord () .emplace_back (texCoord [*texIndex]);
			getNormals  () .emplace_back (points [*index]);
			getVertices () .emplace_back (point);
		}
	}

	setVertexMode (GL_QUADS);
	setNumIndices (indices .size ());
}

void
Sphere::dispose ()
{
	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
