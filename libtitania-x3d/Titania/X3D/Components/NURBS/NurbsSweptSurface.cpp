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

#include "NurbsSweptSurface.h"

#include "../Geometry3D/Extrusion.h"
#include "../NURBS/NurbsCurve.h"
#include "../NURBS/X3DNurbsControlCurveNode.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string NurbsSweptSurface::componentName  = "NURBS";
const std::string NurbsSweptSurface::typeName       = "NurbsSweptSurface";
const std::string NurbsSweptSurface::containerField = "geometry";

NurbsSweptSurface::Fields::Fields () :
	            solid (new SFBool (true)),
	              ccw (new SFBool (true)),
	crossSectionCurve (new SFNode ()),
	  trajectoryCurve (new SFNode ())
{ }

NurbsSweptSurface::NurbsSweptSurface (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParametricGeometryNode (),
	                   fields (),
	    crossSectionCurveNode (),
	      trajectoryCurveNode (),
	            extrusionNode (new Extrusion (executionContext))
{
	addType (X3DConstants::NurbsSweptSurface);

	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "solid",             solid ());
	addField (initializeOnly, "ccw",               ccw ());
	addField (inputOutput,    "crossSectionCurve", crossSectionCurve ());
	addField (inputOutput,    "trajectoryCurve",   trajectoryCurve ());

	addChildObjects (crossSectionCurveNode,
	                 trajectoryCurveNode,
	                 extrusionNode);
}

X3DBaseNode*
NurbsSweptSurface::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsSweptSurface (executionContext);
}

void
NurbsSweptSurface::initialize ()
{
	X3DParametricGeometryNode::initialize ();

	crossSectionCurve () .addInterest (&NurbsSweptSurface::set_crossSectionCurve, this);
	trajectoryCurve ()   .addInterest (&NurbsSweptSurface::set_trajectoryCurve,   this);

	extrusionNode -> beginCap ()    = false;
	extrusionNode -> endCap ()      = false;
	extrusionNode -> solid ()       = true;
	extrusionNode -> ccw ()         = true;
	extrusionNode -> convex ()      = true;
	extrusionNode -> creaseAngle () = pi <float>;

	extrusionNode -> setup ();

	// Prevent automatic rebuild when field are set.
	extrusionNode -> crossSection () .setTainted (true);
	extrusionNode -> spine ()        .setTainted (true);

	set_crossSectionCurve ();
	set_trajectoryCurve ();
}

void
NurbsSweptSurface::set_crossSectionCurve ()
{
	if (crossSectionCurveNode)
		crossSectionCurveNode -> removeInterest (&NurbsSweptSurface::requestRebuild, this);

	crossSectionCurveNode .set (x3d_cast <X3DNurbsControlCurveNode*> (crossSectionCurve ()));

	if (crossSectionCurveNode)
		crossSectionCurveNode -> addInterest (&NurbsSweptSurface::requestRebuild, this);
}

void
NurbsSweptSurface::set_trajectoryCurve ()
{
	if (trajectoryCurveNode)
		trajectoryCurveNode -> rebuilded () .removeInterest (&NurbsSweptSurface::requestRebuild, this);

	trajectoryCurveNode .set (x3d_cast <NurbsCurve*> (trajectoryCurve ()));

	if (trajectoryCurveNode)
		trajectoryCurveNode -> rebuilded () .addInterest (&NurbsSweptSurface::requestRebuild, this);
}

void
NurbsSweptSurface::build ()
{
	if (not crossSectionCurveNode)
		return;

	if (not trajectoryCurveNode)
		return;

	extrusionNode -> crossSection () = crossSectionCurveNode -> tessellate ();
	extrusionNode -> spine ()        = trajectoryCurveNode   -> tessellate ();

	extrusionNode -> rebuild ();

	getColors ()    = extrusionNode -> getPolygonColors ();
	getTexCoords () = extrusionNode -> getPolygonTexCoords ();
	getNormals ()   = extrusionNode -> getPolygonNormals ();
	getVertices ()  = extrusionNode -> getPolygonVertices ();

	if (not ccw ())
		std::for_each (getNormals () .begin (), getNormals () .end (), std::mem_fn (&Vector3f::negate));

	for (const auto & element : extrusionNode -> getElements ())
		addElements (element .vertexMode (), element .count ());

	setSolid (solid ());
	setCCW (ccw ());
}

NurbsSweptSurface::~NurbsSweptSurface ()
{ }

} // X3D
} // titania
