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

#include "X3DNurbsSurfaceGeometryNode.h"

#include "NurbsTextureCoordinate.h"
#include "../../Bits/Cast.h"

#include <cassert>

namespace titania {
namespace X3D {

X3DNurbsSurfaceGeometryNode::Fields::Fields () :
	        solid (new SFBool (true)),
	uTessellation (new SFInt32 ()),
	vTessellation (new SFInt32 ()),
	      uClosed (new SFBool ()),
	      vClosed (new SFBool ()),
	       uOrder (new SFInt32 (3)),
	       vOrder (new SFInt32 (3)),
	   uDimension (new SFInt32 ()),
	   vDimension (new SFInt32 ()),
	        uKnot (new MFDouble ()),
	        vKnot (new MFDouble ()),
	       weight (new MFDouble ()),
	     texCoord (new SFNode ()),
	 controlPoint (new SFNode ())
{ }

X3DNurbsSurfaceGeometryNode::X3DNurbsSurfaceGeometryNode () :
	X3DParametricGeometryNode (),
	                   fields (),
	             texCoordNode (),
	        nurbsTexCoordNode (),
	         controlPointNode ()
{
	addType (X3DConstants::X3DNurbsSurfaceGeometryNode);

	addChildObjects (texCoordNode,
	                 nurbsTexCoordNode,
	                 controlPointNode);
}

void
X3DNurbsSurfaceGeometryNode::initialize ()
{
	X3DParametricGeometryNode::initialize ();

	texCoord ()     .addInterest (&X3DNurbsSurfaceGeometryNode::set_texCoord, this);
	controlPoint () .addInterest (&X3DNurbsSurfaceGeometryNode::set_controlPoint, this);
	
	set_texCoord ();
	set_controlPoint ();
}

void
X3DNurbsSurfaceGeometryNode::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (this);

	if (nurbsTexCoordNode)
		nurbsTexCoordNode -> removeInterest (this);

	texCoordNode      .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));
	nurbsTexCoordNode .set (x3d_cast <NurbsTextureCoordinate*>   (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (this);

	if (nurbsTexCoordNode)
		nurbsTexCoordNode -> addInterest (this);
}

void
X3DNurbsSurfaceGeometryNode::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (this);
}

size_t
X3DNurbsSurfaceGeometryNode::getUTessellation (const size_t uDimension) const
{
	// Return a tessellation one less to the standard to get the right tessellation.

	if (uTessellation () > 0)
		return uTessellation ();
		
	if (uTessellation () < 0)
		return -uTessellation () * uDimension;

	return 2 * uDimension;
}

size_t
X3DNurbsSurfaceGeometryNode::getVTessellation (const size_t vDimension) const
{
	// Return a tessellation one less to the standard to get the right tessellation.

	if (vTessellation () > 0)
		return vTessellation ();
		
	if (vTessellation () < 0)
		return -vTessellation () * vDimension;

	return 2 * vDimension;
}

bool
X3DNurbsSurfaceGeometryNode::getUClosed (const size_t uOrder,
                                         const size_t uDimension,
                                         const size_t vDimension,
                                         const std::vector <double> & uKnot,
                                         const std::vector <double> & weight) const
{
	if (not uClosed ())
		return false;

	const auto haveWeights = weight .size () == controlPointNode -> getSize ();

	for (size_t v = 0, size = vDimension; v < size; ++ v)
	{
		const auto first = v * uDimension;
		const auto last  = v * uDimension + uDimension - 1;

		if (haveWeights)
		{
			if (weight [first] not_eq weight [last])
				return false;
		}

		if (controlPointNode -> get1Point (first) not_eq controlPointNode -> get1Point (last))
			return false;
	}

	// Check if knots are periodic.

	if (uKnot .size () == uDimension + uOrder)
	{
		{
			size_t count = 1;
	
			for (size_t i = 1, size = uOrder; i < size; ++ i)
			{
				count += uKnot [i] == uKnot .front ();
			}
	
			if (count == uOrder)
				return false;
		}

		{
			size_t count = 1;
	
			for (size_t i = uKnot .size () - uOrder, size = uKnot .size () - 1; i < size; ++ i)
			{
				count += uKnot [i] == uKnot .back ();
			}

			if (count == uOrder)
				return false;
		}
	}

	return true;
}

bool
X3DNurbsSurfaceGeometryNode::getVClosed (const size_t vOrder,
                                         const size_t uDimension,
                                         const size_t vDimension,
                                         const std::vector <double> & vKnot,
                                         const std::vector <double> & weight) const
{
	if (not vClosed ())
		return false;

	const auto haveWeights = weight .size () == controlPointNode -> getSize ();

	for (size_t u = 0, size = uDimension; u < size; ++ u)
	{
		const auto first = u;
		const auto last  = (vDimension - 1) * uDimension + u;

		// Check if first and last weights are unitary.

		if (haveWeights)
		{
			if (weight [first] not_eq weight [last])
				return false;
		}

		// Check if first and last point are coincident.

		if (controlPointNode -> get1Point (first) not_eq controlPointNode -> get1Point (last))
			return false;
	}

	// Check if knots are periodic.

	if (vKnot .size () == vDimension + vOrder)
	{
		{
			size_t count = 1;
	
			for (size_t i = 1, size = vOrder; i < size; ++ i)
			{
				count += vKnot [i] == vKnot .front ();
			}
	
			if (count == vOrder)
				return false;
		}

		{
			size_t count = 1;
	
			for (size_t i = vKnot .size () - vOrder, size = vKnot .size () - 1; i < size; ++ i)
			{
				count += vKnot [i] == vKnot .back ();
			}

			if (count == vOrder)
				return false;
		}
	}

	return true;
}

std::vector <float>
X3DNurbsSurfaceGeometryNode::getKnots (const std::vector <double> & knot, const bool closed, const size_t order, const size_t dimension) const
{
	std::vector <float> knots (knot .cbegin (), knot .cend ());

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.

	bool generateUniform = true;

	if (knots .size () == size_t (dimension + order))
	{
		generateUniform = false;

		size_t consecutiveKnots = 0;

		for (size_t i = 1; i < knots .size (); ++ i)
		{
			if (knots [i] == knots [i - 1])
				++ consecutiveKnots;
			else
				consecutiveKnots = 0;

			if (consecutiveKnots > order - 1)
				generateUniform = true;

			if (knots [i - 1] > knots [i])
				generateUniform = true;
		}
	}

	if (generateUniform)
	{
		knots .resize (dimension + order);

		for (size_t i = 0, size = knots .size (); i < size; ++ i)
			knots [i] = float (i) / (size - 1);
	}

	if (closed)
	{
		for (size_t i = 1, size = order - 1; i < size; ++ i)
			knots .emplace_back (knots .back () + (knots [i] - knots [i - 1]));
	}

	return knots;
}

std::vector <Vector4f>
X3DNurbsSurfaceGeometryNode::getControlPoints (const bool uClosed,
                                               const bool vClosed,
                                               const size_t uOrder,
                                               const size_t vOrder,
                                               const size_t uDimension,
                                               const size_t vDimension,
                                               const std::vector <double> & weight) const
{
	std::vector <Vector4f> controlPoints;

	if (weight .size () not_eq controlPointNode -> getSize ())
	{
		for (size_t v = 0, i = 0; v < vDimension; ++ v)
		{
			for (size_t u = 0; u < uDimension; ++ u, ++ i)
			{
				const auto p = controlPointNode -> get1Point (i);

				controlPoints .emplace_back (p .x (),
				                             p .y (),
				                             p .z (),
				                             1);
			}

			if (uClosed)
			{
				const auto first = controlPoints .size () - uDimension;

				for (size_t i = 1, size = uOrder - 1; i < size; ++ i)
					controlPoints .emplace_back (controlPoints [first + i]);
			}
		}

		if (vClosed)
		{
			for (size_t i = (uDimension + uClosed * (uOrder - 2)), size = (uDimension + uClosed * (uOrder - 2)) * (vOrder - 1); i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}
	else
	{
		for (size_t v = 0, i = 0; v < vDimension; ++ v)
		{
			for (size_t u = 0; u < uDimension; ++ u, ++ i)
			{
				const auto p = controlPointNode -> get1Point (i);

				controlPoints .emplace_back (p .x (),
				                             p .y (),
				                             p .z (),
				                             weight [i]);
			}

			if (uClosed)
			{
				const auto first = controlPoints .size () - uDimension;

				for (size_t i = 1, size = uOrder - 1; i < size; ++ i)
					controlPoints .emplace_back (controlPoints [first + i]);
			}
		}

		if (vClosed)
		{
			for (size_t i = (uDimension + uClosed * (uOrder - 2)), size = (uDimension + uClosed * (uOrder - 2)) * (vOrder - 1); i < size; ++ i)
				controlPoints .emplace_back (controlPoints [i]);
		}
	}

	return controlPoints;
}

void
X3DNurbsSurfaceGeometryNode::build ()
{
	if (uOrder () < 2)
		return;

	if (vOrder () < 2)
		return;

	if (uDimension () < uOrder ())
		return;

	if (vDimension () < vOrder ())
		return;

	if (not controlPointNode)
		return;

	if (controlPointNode -> getSize () not_eq size_t (uDimension () * vDimension ()))
		return;

	// Order and dimension are now positive numbers.

	// ControlPoints

	const auto uClosed       = getUClosed (uOrder (), uDimension (), vDimension (), uKnot (), weight ());
	const auto vClosed       = getVClosed (vOrder (), uDimension (), vDimension (), vKnot (), weight ());
	auto       controlPoints = getControlPoints (uClosed, vClosed, uOrder (), vOrder (), uDimension (), vDimension (), weight ());

	// Knots

	auto       uKnots = getKnots (uKnot (), uClosed, uOrder (), uDimension ());
	auto       vKnots = getKnots (vKnot (), vClosed, vOrder (), vDimension ());
	const auto uScale = uKnots .back () - uKnots .front ();
	const auto vScale = vKnots .back () - vKnots .front ();

	assert ((uKnots .size () - uOrder ()) * (vKnots .size () - vOrder ()) == controlPoints .size ());

	// TextureCoordinate

	// Default unit square
	std::vector <Vector4f> texControlPoints;
	std::vector <float>    texUKnot;
	std::vector <float>    texVKnot;

//	if (texCoordNode)
//	{
//		texCoordNode -> init (getTexCoords (), 0);
//	}
//	else if (nurbsTexCoordNode)
//	{
//
//	}
//	else
	{
		getTexCoords () .emplace_back ();
	
		texControlPoints .emplace_back (0, 0, 0, 1);
		texControlPoints .emplace_back (1, 0, 0, 1);
		texControlPoints .emplace_back (0, 1, 0, 1);
		texControlPoints .emplace_back (1, 1, 0, 1);
	
		texUKnot .emplace_back (uKnots .front ());
		texUKnot .emplace_back (uKnots .front ());
		texUKnot .emplace_back (uKnots .back ());
		texUKnot .emplace_back (uKnots .back ());
	
		texVKnot .emplace_back (vKnots .front ());
		texVKnot .emplace_back (vKnots .front ());
		texVKnot .emplace_back (vKnots .back ());
		texVKnot .emplace_back (vKnots .back ());
	}

	// Initialize NURBS tesselllator

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, uScale ? getUTessellation (uKnots .size () - uOrder ()) / uScale : 1);
	tessellator .property (GLU_V_STEP, vScale ? getVTessellation (vKnots .size () - vOrder ()) / vScale : 1);

	// Tessellate

	tessellator .begin_surface ();

	tessellator .nurbs_surface (texUKnot .size (), texUKnot .data (),
	                            texVKnot .size (), texVKnot .data (),
	                            4, 4 * 2,
	                            texControlPoints [0] .data (),
	                            2, 2,
	                            GL_MAP2_TEXTURE_COORD_4);

	tessellator .nurbs_surface (uKnots .size (), uKnots .data (),
	                            vKnots .size (), vKnots .data (),
	                            4, 4 * (uDimension () + uClosed * (uOrder () - 2)),
	                            controlPoints [0] .data (),
	                            uOrder (), vOrder (),
	                            GL_MAP2_VERTEX_4);

	trimSurface (tessellator);

	tessellator .end_surface ();

	// End tessellation

	// Triangles

	const auto & triangles = tessellator .triangles ();

	getTexCoords () [0] .insert (getTexCoords () [0] .end (), triangles .tex_coords () .begin (), triangles .tex_coords () .end ());
	getNormals ()  .insert (getNormals ()  .end (), triangles .normals ()  .begin (), triangles .normals ()  .end ());
	getVertices () .insert (getVertices () .end (), triangles .vertices () .begin (), triangles .vertices () .end ());

	addElements (GL_TRIANGLES, triangles .vertices () .size ());

	// Quads

	const auto & quads = tessellator .quads ();

	getTexCoords () [0] .insert (getTexCoords () [0] .end (), quads .tex_coords () .begin (), quads .tex_coords () .end ());
	getNormals ()  .insert (getNormals ()  .end (), quads .normals ()  .begin (), quads .normals ()  .end ());
	getVertices () .insert (getVertices () .end (), quads .vertices () .begin (), quads .vertices () .end ());

	addElements (GL_QUADS, quads .vertices () .size ());

	// Properties

	setSolid (solid ());
	setCCW (true);
}

X3DNurbsSurfaceGeometryNode::~X3DNurbsSurfaceGeometryNode ()
{ }

} // X3D
} // titania
