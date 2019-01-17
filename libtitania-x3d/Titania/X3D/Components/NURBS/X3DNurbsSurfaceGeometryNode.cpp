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

#include "../Rendering/X3DCoordinateNode.h"
#include "../NURBS/NurbsTextureCoordinate.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/NURBS/NURBS.h"

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
	         controlPointNode (),
	        tessellationScale (1)
{
	addType (X3DConstants::X3DNurbsSurfaceGeometryNode);

	addChildObjects (texCoordNode,
	                 nurbsTexCoordNode,
	                 controlPointNode,
	                 tessellationScale);
}

void
X3DNurbsSurfaceGeometryNode::initialize ()
{
	X3DParametricGeometryNode::initialize ();

	texCoord ()     .addInterest (&X3DNurbsSurfaceGeometryNode::set_texCoord,     this);
	controlPoint () .addInterest (&X3DNurbsSurfaceGeometryNode::set_controlPoint, this);

	set_texCoord ();
	set_controlPoint ();
}

void
X3DNurbsSurfaceGeometryNode::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);

	if (nurbsTexCoordNode)
		nurbsTexCoordNode -> removeInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);

	texCoordNode      .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));
	nurbsTexCoordNode .set (x3d_cast <NurbsTextureCoordinate*>   (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);

	if (nurbsTexCoordNode)
		nurbsTexCoordNode -> addInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);
}

void
X3DNurbsSurfaceGeometryNode::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (&X3DNurbsSurfaceGeometryNode::requestRebuild, this);
}

size_t
X3DNurbsSurfaceGeometryNode::getUTessellation (const size_t uDimension) const
{
	return NURBS::getTessellation (uTessellation (), uDimension);
}

size_t
X3DNurbsSurfaceGeometryNode::getVTessellation (const size_t vDimension) const
{
	return NURBS::getTessellation (vTessellation (), vDimension);
}

bool
X3DNurbsSurfaceGeometryNode::getUClosed (const size_t uOrder,
                                         const size_t uDimension,
                                         const size_t vDimension,
                                         const std::vector <double> & uKnot,
                                         const std::vector <double> & weight,
                                         const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	if (not uClosed ())
		return false;

	return NURBS::getUClosed (uOrder, uDimension, vDimension, uKnot, weight, controlPointNode);
}

bool
X3DNurbsSurfaceGeometryNode::getVClosed (const size_t vOrder,
                                         const size_t uDimension,
                                         const size_t vDimension,
                                         const std::vector <double> & vKnot,
                                         const std::vector <double> & weight,
                                         const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	if (not vClosed ())
		return false;

	return NURBS::getVClosed (vOrder, uDimension, vDimension, vKnot, weight, controlPointNode);
}

std::vector <Vector4f>
X3DNurbsSurfaceGeometryNode::getTexControlPoints (const bool uClosed,
                                                  const bool vClosed,
                                                  const size_t uOrder,
                                                  const size_t vOrder,
                                                  const size_t uDimension,
                                                  const size_t vDimension) const
{
	std::vector <Vector4f> controlPoints;

	for (size_t v = 0, i = 0; v < vDimension; ++ v)
	{
		for (size_t u = 0; u < uDimension; ++ u, ++ i)
			controlPoints .emplace_back (texCoordNode -> get1Point (i));

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

	return controlPoints;
}

std::vector <Vector4f>
X3DNurbsSurfaceGeometryNode::getControlPoints (const bool uClosed,
                                               const bool vClosed,
                                               const size_t uOrder,
                                               const size_t vOrder,
                                               const size_t uDimension,
                                               const size_t vDimension,
                                               const std::vector <double> & weight,
                                               const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getControlPoints (uClosed, vClosed, uOrder, vOrder, uDimension, vDimension, weight, controlPointNode);
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

	const auto   uClosed       = getUClosed (uOrder (), uDimension (), vDimension (), uKnot (), weight (), controlPointNode);
	const auto   vClosed       = getVClosed (vOrder (), uDimension (), vDimension (), vKnot (), weight (), controlPointNode);
	auto         controlPoints = getControlPoints (uClosed, vClosed, uOrder (), vOrder (), uDimension (), vDimension (), weight (), controlPointNode);
	const size_t vStride       = (uDimension () + uClosed * (uOrder () - 2));

	// Knots

	auto       uKnots = getKnots (uClosed, uOrder (), uDimension (), uKnot ());
	auto       vKnots = getKnots (vClosed, vOrder (), vDimension (), vKnot ());
	const auto uScale = uKnots .back () - uKnots .front ();
	const auto vScale = vKnots .back () - vKnots .front ();

	assert ((uKnots .size () - uOrder ()) * (vKnots .size () - vOrder ()) == controlPoints .size ());

	// TextureCoordinate

	size_t                 texUOrder = 0;
	size_t                 texVOrder = 0;
	size_t                 texVStride = 0;
	std::vector <float>    texUKnots;
	std::vector <float>    texVKnots;
	std::vector <Vector4f> texControlPoints;

	if (texCoordNode)
	{
		if (texCoordNode -> getSize () == controlPointNode -> getSize ())
		{
			getTexCoords () .emplace_back ();

			texUOrder        = uOrder ();
			texVOrder        = vOrder ();
			texVStride       = vStride;
			texUKnots        = uKnots;
			texVKnots        = vKnots;
			texControlPoints = getTexControlPoints (uClosed, vClosed, uOrder (), vOrder (), uDimension (), vDimension ());
		}
	}
	else if (nurbsTexCoordNode)
	{
		if (nurbsTexCoordNode -> isValid ())
		{
			getTexCoords () .emplace_back ();

			texUOrder  = nurbsTexCoordNode -> uOrder ();
			texVOrder  = nurbsTexCoordNode -> vOrder ();
			texVStride = nurbsTexCoordNode -> uDimension ();

			uKnots = getKnots (false, nurbsTexCoordNode -> uOrder (), nurbsTexCoordNode -> uDimension (), nurbsTexCoordNode -> uKnot ());
			vKnots = getKnots (false, nurbsTexCoordNode -> vOrder (), nurbsTexCoordNode -> vDimension (), nurbsTexCoordNode -> vKnot ());

			texControlPoints = nurbsTexCoordNode -> getControlPoints ();
		}
	}

	if (getTexCoords () .empty ())
	{
		// Default unit square

		getTexCoords () .emplace_back ();

		texUOrder  = 2;
		texVOrder  = 2;
		texVStride = 2;

		texControlPoints .emplace_back (0, 0, 0, 1);
		texControlPoints .emplace_back (1, 0, 0, 1);
		texControlPoints .emplace_back (0, 1, 0, 1);
		texControlPoints .emplace_back (1, 1, 0, 1);
	
		texUKnots .emplace_back (uKnots .front ());
		texUKnots .emplace_back (uKnots .front ());
		texUKnots .emplace_back (uKnots .back ());
		texUKnots .emplace_back (uKnots .back ());
	
		texVKnots .emplace_back (vKnots .front ());
		texVKnots .emplace_back (vKnots .front ());
		texVKnots .emplace_back (vKnots .back ());
		texVKnots .emplace_back (vKnots .back ());
	}

	// Initialize NURBS tesselllator

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, getUTessellation (uKnots .size () - uOrder ()) / uScale * tessellationScale);
	tessellator .property (GLU_V_STEP, getVTessellation (vKnots .size () - vOrder ()) / vScale * tessellationScale);

	// Tessellate

	tessellator .begin_surface ();

	tessellator .nurbs_surface (texUKnots .size (), texUKnots .data (),
	                            texVKnots .size (), texVKnots .data (),
	                            4, 4 * texVStride,
	                            texControlPoints [0] .data (),
	                            texUOrder, texVOrder,
	                            GL_MAP2_TEXTURE_COORD_4);

	tessellator .nurbs_surface (uKnots .size (), uKnots .data (),
	                            vKnots .size (), vKnots .data (),
	                            4, 4 * vStride,
	                            controlPoints [0] .data (),
	                            uOrder (), vOrder (),
	                            GL_MAP2_VERTEX_4);

	trimSurface (tessellator);

	tessellator .end_surface ();

	// End tessellation

	// Triangles

	const auto & triangles = tessellator .triangles ();

	getTexCoords () [0] .insert (getTexCoords () [0] .end (), triangles .tex_coords () .begin (), triangles .tex_coords () .end ());
	getNormals ()       .insert (getNormals ()       .end (), triangles .normals ()    .begin (), triangles .normals ()    .end ());
	getVertices ()      .insert (getVertices ()      .end (), triangles .vertices ()   .begin (), triangles .vertices ()   .end ());

	addElements (GL_TRIANGLES, triangles .vertices () .size ());

	// Quads

	const auto & quads = tessellator .quads ();

	getTexCoords () [0] .insert (getTexCoords () [0] .end (), quads .tex_coords () .begin (), quads .tex_coords () .end ());
	getNormals ()       .insert (getNormals ()       .end (), quads .normals ()    .begin (), quads .normals ()    .end ());
	getVertices ()      .insert (getVertices ()      .end (), quads .vertices ()   .begin (), quads .vertices ()   .end ());

	addElements (GL_QUADS, quads .vertices () .size ());

	// Properties

	setSolid (solid ());
	setCCW (true);
}

X3DNurbsSurfaceGeometryNode::~X3DNurbsSurfaceGeometryNode ()
{ }

} // X3D
} // titania
