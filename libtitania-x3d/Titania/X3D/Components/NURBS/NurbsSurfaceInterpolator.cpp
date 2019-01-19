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

#include "NurbsSurfaceInterpolator.h"

#include "../../Bits/Cast.h"
#include "../../Browser/NURBS/NURBS.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/X3DCoordinateNode.h"

#include <Titania/Math/Algorithms/Barycentric.h>
#include <Titania/Math/Geometry/Triangle2.h>
#include <cassert>

namespace titania {
namespace X3D {

const std::string NurbsSurfaceInterpolator::componentName  = "NURBS";
const std::string NurbsSurfaceInterpolator::typeName       = "NurbsSurfaceInterpolator";
const std::string NurbsSurfaceInterpolator::containerField = "children";

const size_t NurbsSurfaceInterpolator::U_TESSELLATION = 8;
const size_t NurbsSurfaceInterpolator::V_TESSELLATION = 8;

NurbsSurfaceInterpolator::Fields::Fields () :
	    set_fraction (new SFVec2f ()),
	          uOrder (new SFInt32 (3)),
	          vOrder (new SFInt32 (3)),
	      uDimension (new SFInt32 ()),
	      vDimension (new SFInt32 ()),
	           uKnot (new MFDouble ()),
	           vKnot (new MFDouble ()),
	          weight (new MFDouble ()),
	    controlPoint (new SFNode ()),
	  normal_changed (new SFVec3f ()),
	position_changed (new SFVec3f ())
{ }

NurbsSurfaceInterpolator::NurbsSurfaceInterpolator (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	          fields (),
	controlPointNode (),
	   rebuildOutput (),
	     tessellator ()
{
	addType (X3DConstants::NurbsSurfaceInterpolator);

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOnly,      "set_fraction",     set_fraction ());
	addField (initializeOnly, "uOrder",           uOrder ());
	addField (initializeOnly, "vOrder",           vOrder ());
	addField (initializeOnly, "uDimension",       uDimension ());
	addField (initializeOnly, "vDimension",       vDimension ());
	addField (initializeOnly, "uKnot",            uKnot ());
	addField (initializeOnly, "vKnot",            vKnot ());
	addField (inputOutput,    "weight",           weight ());
	addField (inputOutput,    "controlPoint",     controlPoint ());
	addField (outputOnly,     "normal_changed",   normal_changed ());
	addField (outputOnly,     "position_changed", position_changed ());

	addChildObjects (controlPointNode,
	                 rebuildOutput);
}

X3DBaseNode*
NurbsSurfaceInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsSurfaceInterpolator (executionContext);
}

void
NurbsSurfaceInterpolator::initialize ()
{
	X3DChildNode::initialize ();

	set_fraction () .addInterest (&NurbsSurfaceInterpolator::set_fraction_,    this);
	uOrder ()       .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	vOrder ()       .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	uDimension ()   .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	vDimension ()   .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	uKnot ()        .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	vKnot ()        .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	weight ()       .addInterest (&NurbsSurfaceInterpolator::requestRebuild,   this);
	controlPoint () .addInterest (&NurbsSurfaceInterpolator::set_controlPoint, this);

	rebuildOutput .addInterest (&NurbsSurfaceInterpolator::build, this);

	set_controlPoint ();
}

bool
NurbsSurfaceInterpolator::getUClosed (const size_t uOrder,
                                      const size_t uDimension,
                                      const size_t vDimension,
                                      const std::vector <double> & uKnot,
                                      const std::vector <double> & weight,
                                      const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getUClosed (uOrder, uDimension, vDimension, uKnot, weight, controlPointNode);
}

bool
NurbsSurfaceInterpolator::getVClosed (const size_t vOrder,
                                      const size_t uDimension,
                                      const size_t vDimension,
                                      const std::vector <double> & vKnot,
                                      const std::vector <double> & weight,
                                      const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getVClosed (vOrder, uDimension, vDimension, vKnot, weight, controlPointNode);
}

std::vector <float>
NurbsSurfaceInterpolator::getKnots (const bool closed,
                                    const size_t order,
                                    const size_t dimension,
                                    const std::vector <double> & knot) const
{
	return NURBS::getKnots (closed, order, dimension, knot);
}

std::vector <Vector4f>
NurbsSurfaceInterpolator::getControlPoints (const bool uClosed,
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
NurbsSurfaceInterpolator::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (&NurbsSurfaceInterpolator::requestRebuild, this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (&NurbsSurfaceInterpolator::requestRebuild, this);

	build ();
}

void
NurbsSurfaceInterpolator::requestRebuild ()
{
	rebuildOutput .addEvent ();
}

void
NurbsSurfaceInterpolator::build ()
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

	{
		// Default unit square

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

	tessellator = std::make_unique <nurbs_tessellator> ();

	tessellator -> only_triangles (true);

	tessellator -> property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator -> property (GLU_U_STEP, U_TESSELLATION * (uKnots .size () - uOrder ()) / uScale);
	tessellator -> property (GLU_V_STEP, V_TESSELLATION * (vKnots .size () - vOrder ()) / vScale);

	// Tessellate

	tessellator -> begin_surface ();

	tessellator -> nurbs_surface (texUKnots .size (), texUKnots .data (),
	                            texVKnots .size (), texVKnots .data (),
	                            4, 4 * texVStride,
	                            texControlPoints [0] .data (),
	                            texUOrder, texVOrder,
	                            GL_MAP2_TEXTURE_COORD_4);

	tessellator -> nurbs_surface (uKnots .size (), uKnots .data (),
	                            vKnots .size (), vKnots .data (),
	                            4, 4 * vStride,
	                            controlPoints [0] .data (),
	                            uOrder (), vOrder (),
	                            GL_MAP2_VERTEX_4);

	tessellator -> end_surface ();

	// End tessellation
}

std::tuple <size_t, Vector3f, bool>
NurbsSurfaceInterpolator::getTriangle (const Vector2f & point, const std::vector <Vector4f> & texCoords) const
{
	//TODO: Use a BVH (QuadTree) for this query.

	for (size_t i = 0, size = texCoords .size (); i < size; i += 3)
	{
		const Triangle2f triangle2 (Vector2f (texCoords [i + 0] .x (), texCoords [i + 0] .y ()),
		                            Vector2f (texCoords [i + 1] .x (), texCoords [i + 1] .y ()),
		                            Vector2f (texCoords [i + 2] .x (), texCoords [i + 2] .y ()));

		if (triangle2 .contains (point))
		{
			const Line3f line (Vector3f (point .x (), point .y (), 0), Vector3f (0, 0, 1));

			const auto & [intersection, intersected] = line .intersects (Vector3f (texCoords [i + 0] .x (), texCoords [i + 0] .y (), 0),
			                                                             Vector3f (texCoords [i + 1] .x (), texCoords [i + 1] .y (), 0),
			                                                             Vector3f (texCoords [i + 2] .x (), texCoords [i + 2] .y (), 0));

			return std::tuple (i, intersection, intersected);
		}
	}

	return std::tuple (0, Vector3f (), false);
}

void
NurbsSurfaceInterpolator::set_fraction_ ()
{
	if (not tessellator)
		return;

	const auto & triangles = tessellator -> triangles ();
	const auto & texCoords = triangles .tex_coords ();
	const auto & normals   = triangles .normals ();
	const auto & vertices  = triangles .vertices ();

	const auto & [index, uvt, intersected] = getTriangle (set_fraction (), texCoords);

	if (not intersected)
		return;

	normal_changed ()   = normalize (from_barycentric <float> (uvt, normals [index], normals [index + 1], normals [index + 2]));
	position_changed () = from_barycentric (uvt, vertices [index], vertices [index + 1], vertices [index + 2]);
}

NurbsSurfaceInterpolator::~NurbsSurfaceInterpolator ()
{ }

} // X3D
} // titania
