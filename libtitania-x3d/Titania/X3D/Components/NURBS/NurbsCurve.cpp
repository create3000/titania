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

#include "NurbsCurve.h"

#include "../NURBS/CoordinateDouble.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Shaders/ComposedShader.h"

#include "../../Bits/Cast.h"
#include "../../Browser/NURBS/NURBS.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Mesh/NurbsTessellator.h>

#include <cassert>

namespace titania {
namespace X3D {

const std::string NurbsCurve::componentName  = "NURBS";
const std::string NurbsCurve::typeName       = "NurbsCurve";
const std::string NurbsCurve::containerField = "geometry";

NurbsCurve::Fields::Fields () :
	      closed (new SFBool ()),
	tessellation (new SFInt32 ()),
	       order (new SFInt32 (3)),
	        knot (new MFDouble ()),
	      weight (new MFDouble ()),
	controlPoint (new SFNode ())
{ }

NurbsCurve::NurbsCurve (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParametricGeometryNode (),
	      X3DLineGeometryNode (),
	                   fields (),
	         controlPointNode ()
{
	addType (X3DConstants::NurbsCurve);

	addField (inputOutput,    "metadata",     metadata ());
	addField (initializeOnly, "closed",       closed ());
	addField (inputOutput,    "tessellation", tessellation ());
	addField (initializeOnly, "order",        order ());
	addField (initializeOnly, "knot",         knot ());
	addField (inputOutput,    "weight",       weight ());
	addField (inputOutput,    "controlPoint", controlPoint ());

	setGeometryType (GeometryType::GEOMETRY_LINES);

	addChildObjects (controlPointNode);
}

X3DBaseNode*
NurbsCurve::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsCurve (executionContext);
}

void
NurbsCurve::initialize ()
{
	X3DParametricGeometryNode::initialize ();

	controlPoint () .addInterest (&NurbsCurve::set_controlPoint, this);
	
	set_controlPoint ();
}

void
NurbsCurve::set_controlPoint ()
{
	if (controlPointNode)
		controlPointNode -> removeInterest (&NurbsCurve::requestRebuild, this);

	controlPointNode .set (x3d_cast <X3DCoordinateNode*> (controlPoint ()));

	if (controlPointNode)
		controlPointNode -> addInterest (&NurbsCurve::requestRebuild, this);
}

const X3DPtr <ComposedShader> &
NurbsCurve::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

size_t
NurbsCurve::getTessellation (const size_t dimension) const
{
	return NURBS::getTessellation (tessellation (), dimension);
}

bool
NurbsCurve::getClosed (const size_t order,
                       const std::vector <double> & knot,
                       const std::vector <double> & weight,
                       const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	if (not closed ())
		return false;

	return NURBS::getClosed (order, knot, weight, controlPointNode);
}

std::vector <Vector4f>
NurbsCurve::getControlPoints (const bool closed,
                              const size_t order,
                              const std::vector <double> & weight,
                              const X3DPtr <X3DCoordinateNode> & controlPointNode) const
{
	return NURBS::getControlPoints (closed, order, weight, controlPointNode);
}

std::vector <Vector3f>
NurbsCurve::tessellate () const
{
	if (order () < 2)
		return { };

	if (not controlPointNode)
		return { };

	if (controlPointNode -> getSize () < size_t (order ()))
		return { };

	// Order and dimension are now positive numbers.

	const auto   closed = getClosed (order (), knot (), weight (), controlPointNode);
	const auto & lines  = getVertices ();
	auto         curve  = std::vector <Vector3f> ();

	for (size_t i = 0, size = lines .size (); i < size; i += 2)
		curve .emplace_back (lines [i]);

	if (closed)
		curve .emplace_back (lines .front ());
	else
		curve .emplace_back (lines .back ());

	return curve;
}

void
NurbsCurve::build ()
{
	if (order () < 2)
		return;

	if (not controlPointNode)
		return;

	if (controlPointNode -> getSize () < size_t (order ()))
		return;

	// Order and dimension are now positive numbers.

	const auto closed        = getClosed (order (), knot (), weight (), controlPointNode);
	auto       controlPoints = getControlPoints (closed, order (), weight (), controlPointNode);

	// Knots

	auto       knots = getKnots (closed, order (), controlPointNode -> getSize (), knot ());
	const auto scale = knots .back () - knots .front ();

	assert ((knots .size () - order ()) == controlPoints .size ());

	// Tessellate

	nurbs_tessellator tessellator;

	tessellator .property (GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	tessellator .property (GLU_U_STEP, getTessellation (controlPoints .size ()) / scale);

	tessellator .begin_curve ();

	tessellator .nurbs_curve (knots .size (), knots .data (),
	                          4, controlPoints [0] .data (),
	                          order (),
	                          GL_MAP1_VERTEX_4);

	tessellator .end_curve ();

	// End tessellation

	// Lines

	const auto & lines = tessellator .lines ();

	getVertices () .insert (getVertices () .end (), lines .begin (), lines .end ());

	addElements (GL_LINES, lines .size ());
}

NodeType
NurbsCurve::getPrimitiveType () const
{
	return X3DConstants::IndexedLineSet;
}

SFNode
NurbsCurve::toPrimitive () const
{
	const auto geometryNode   = getExecutionContext () -> createNode <IndexedLineSet> ();
	const auto coordinateNode = getExecutionContext () -> createNode <CoordinateDouble> ();
	auto       coordMap       = std::map <Vector3d, size_t> ();

	geometryNode -> coord () = coordinateNode;

	for (const auto & vertex : getVertices ())
		coordMap .emplace (vertex, coordMap .size ());

	for (size_t i = 0, size = getVertices () .size (); i < size; i += 2)
	{
		const auto & vertex1 = getVertices () [i];
		const auto & vertex2 = getVertices () [i + 1];

		geometryNode -> coordIndex () .emplace_back (coordMap [vertex1]);
		geometryNode -> coordIndex () .emplace_back (coordMap [vertex2]);
		geometryNode -> coordIndex () .emplace_back (-1);
	}

	for (const auto & [point, index] : coordMap)
		coordinateNode -> set1Point (index, point);

	return geometryNode;
}

NurbsCurve::~NurbsCurve ()
{ }

} // X3D
} // titania
