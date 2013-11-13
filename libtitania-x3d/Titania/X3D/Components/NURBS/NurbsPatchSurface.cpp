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

#include "NurbsPatchSurface.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/X3DCoordinateNode.h"

#include "../Rendering/Coordinate.h"

namespace titania {
namespace X3D {

const std::string NurbsPatchSurface::componentName  = "NURBS";
const std::string NurbsPatchSurface::typeName       = "NurbsPatchSurface";
const std::string NurbsPatchSurface::containerField = "geometry";

NurbsPatchSurface::NurbsPatchSurface (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DNurbsSurfaceGeometryNode (),
	                     listId (0)
{
	addField (inputOutput,    "metadata",      metadata ());
	addField (inputOutput,    "uTessellation", uTessellation ());
	addField (inputOutput,    "vTessellation", vTessellation ());
	addField (initializeOnly, "uClosed",       uClosed ());
	addField (initializeOnly, "vClosed",       vClosed ());
	addField (initializeOnly, "solid",         solid ());
	addField (initializeOnly, "uOrder",        uOrder ());
	addField (initializeOnly, "vOrder",        vOrder ());
	addField (initializeOnly, "uDimension",    uDimension ());
	addField (initializeOnly, "vDimension",    vDimension ());
	addField (initializeOnly, "uKnot",         uKnot ());
	addField (initializeOnly, "vKnot",         vKnot ());
	addField (inputOutput,    "weight",        weight ());
	addField (inputOutput,    "texCoord",      texCoord ());
	addField (inputOutput,    "controlPoint",  controlPoint ());
}

X3DBaseNode*
NurbsPatchSurface::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsPatchSurface (executionContext);
}

void
NurbsPatchSurface::initialize ()
{
	X3DNurbsSurfaceGeometryNode::initialize ();

	listId = glGenLists (1);
}

size_t
NurbsPatchSurface::getUTessellationPoints () const
{
	if (uTessellation () > 0)
		return uTessellation () + 1;

	else if (uTessellation () < 0)
		return -uTessellation () * uDimension () + 1;

	else
		return 2 * uDimension () + 1;
}

size_t
NurbsPatchSurface::getVTessellationPoints () const
{
	if (vTessellation () > 0)
		return vTessellation () + 1;

	else if (vTessellation () < 0)
		return -vTessellation () * vDimension () + 1;

	else
		return 2 * vDimension () + 1;
}

std::vector <float>
NurbsPatchSurface::getKnots (const MFDouble & knot, const SFInt32 & order, const SFInt32 & dimension) const
{
	std::vector <float> knots (knot .begin (), knot .end ());

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.

	bool generateUniform = true;

	if (knot .size () == size_t (dimension + order))
	{
		generateUniform = false;

		size_t consecutiveKnots = 0;

		for (size_t i = 1; i < knots .size (); ++ i)
		{
			if (knots [i] == knots [i - 1])
				++ consecutiveKnots;
			else
				consecutiveKnots = 0;

			if (consecutiveKnots > size_t (order - 1))
				generateUniform = true;

			if (knots [i - 1] > knots [i])
				generateUniform = true;
		}
	}

	if (generateUniform)
	{
		knots .resize (dimension + order);

		for (size_t i = 0, size = knots .size (); i < size; ++ i)
			knots [i] = (float) i / (size - 1);
	}

	return knots;
}

void
NurbsPatchSurface::build ()
{
	if (uOrder () < 2)
		return;

	if (vOrder () < 2)
		return;

	if (uDimension () < uOrder ())
		return;

	if (vDimension () < vOrder ())
		return;

	auto coordinate = x3d_cast <Coordinate*> (controlPoint ());

	if (not coordinate)
		return;

	if (coordinate -> getSize () not_eq size_t (uDimension () * vDimension ()))
		return;

	std::vector <float> uKnots (uKnot () .size ());
	std::vector <float> vKnots (vKnot () .size ());

	if (weight () .size () not_eq size_t (uDimension () * vDimension ()))
		return;

	const MFVec3f & point = coordinate -> point ();

	std::vector <float> weights;
	
	GLenum map2Vertex3Or4;
	size_t sizeOfVertex;

	if (weight () .size () < point .size ())
	{
		sizeOfVertex   = 3;
		map2Vertex3Or4 = GL_MAP2_VERTEX_3;
		weights .reserve (point .size () * sizeOfVertex);

		for (unsigned int i = 0; i < point .size (); i ++)
		{
			weights .emplace_back (point [i] .getX ());
			weights .emplace_back (point [i] .getY ());
			weights .emplace_back (point [i] .getZ ());
		}
	}
	else
	{
		sizeOfVertex   = 4;
		map2Vertex3Or4 = GL_MAP2_VERTEX_4;
		weights .reserve (point .size () * sizeOfVertex);

		for (unsigned int i = 0; i < point .size (); i ++)
		{
			weights .emplace_back (point [i] .getX ());
			weights .emplace_back (point [i] .getY ());
			weights .emplace_back (point [i] .getZ ());
			weights .emplace_back (weight () [i]);
		}
	}

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.
	bool generateUniform = true;

	if (uKnot () .size () == (unsigned int) (uDimension () + uOrder ()))
	{
		generateUniform = false;
		size_t consecutiveKnots = 0;

		for (unsigned int i = 0; i < uKnot () .size (); i ++)
		{
			uKnots [i] = (GLfloat) uKnot () [i];

			if (i > 0)
			{
				if (uKnots [i] == uKnots [ i - 1 ])
					consecutiveKnots ++;
				else
					consecutiveKnots = 0;

				if (consecutiveKnots > size_t (uOrder () - 1))
					generateUniform = true;

				if (uKnots [ i - 1 ] > uKnots [i])
					generateUniform = true;
			}
		}
	}

	size_t uSizeToUse = uKnot () .size ();

	if (generateUniform)
	{
		uSizeToUse = uDimension () + uOrder ();

		uKnots .resize (uSizeToUse);

		for (size_t i = 0; i < uSizeToUse; i ++)
			uKnots [i] = (GLfloat) ((double) i / (uSizeToUse - 1));
	}

	generateUniform = true;

	if (vKnot ().size () == (unsigned int) (vDimension () + vOrder ()))
	{
		generateUniform = false;
		size_t consecutiveKnots = 0;

		for (unsigned int i = 0; i < vKnot () .size (); i ++)
		{
			vKnots [i] = (GLfloat) vKnot () [i];

			if (i > 0)
			{
				if (vKnots [i] == vKnots [ i - 1 ])
					consecutiveKnots ++;
				else
					consecutiveKnots = 0;

				if (consecutiveKnots > size_t (vOrder () - 1))
					generateUniform = true;

				if (vKnots [ i - 1 ] > vKnots [i])
					generateUniform = true;
			}
		}
	}

	size_t vSizeToUse = vKnot () .size ();

	if (generateUniform)
	{
		vSizeToUse = vDimension () + vOrder ();

		vKnots .resize (vSizeToUse);

		for (size_t i = 0; i < vSizeToUse; i ++)
			vKnots [i] = (GLfloat) ((double) i / (vSizeToUse - 1));
	}

	glNewList (listId, GL_COMPILE);

	GLUnurbs* nurbsRenderer = gluNewNurbsRenderer ();

	// Tesselation

	gluNurbsProperty (nurbsRenderer, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	gluNurbsProperty (nurbsRenderer, GLU_U_STEP, getUTessellationPoints ());
	gluNurbsProperty (nurbsRenderer, GLU_V_STEP, getVTessellationPoints ());

	glEnable (GL_AUTO_NORMAL);

	gluNurbsProperty (nurbsRenderer, GLU_DISPLAY_MODE, GLU_FILL);
	gluBeginSurface (nurbsRenderer);

	gluNurbsSurface( nurbsRenderer, 
		uSizeToUse, uKnots .data (), 
		vSizeToUse, vKnots .data (),
		sizeOfVertex, sizeOfVertex * uDimension (),
		weights .data (),
		uOrder (), vOrder (),
		map2Vertex3Or4);

	gluEndSurface (nurbsRenderer);
	glDisable (GL_AUTO_NORMAL);
	
	gluDeleteNurbsRenderer (nurbsRenderer);

	glEndList ();
}

void
NurbsPatchSurface::draw ()
{
	if (solid ())
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	glFrontFace (GL_CCW);

	glCallList (listId);
}

void
NurbsPatchSurface::dispose ()
{
	if (listId)
		glDeleteLists (listId, 1);

	X3DNurbsSurfaceGeometryNode::dispose ();
}

}  // X3D
}  // titania
