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

#include "NurbsSwungSurface.h"

#include "../Geometry3D/Extrusion.h"
#include "../NURBS/X3DNurbsControlCurveNode.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string NurbsSwungSurface::componentName  = "NURBS";
const std::string NurbsSwungSurface::typeName       = "NurbsSwungSurface";
const std::string NurbsSwungSurface::containerField = "geometry";

NurbsSwungSurface::Fields::Fields () :
	          solid (new SFBool (true)),
	            ccw (new SFBool (true)),
	   profileCurve (new SFNode ()),
	trajectoryCurve (new SFNode ())
{ }

NurbsSwungSurface::NurbsSwungSurface (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParametricGeometryNode (),
	                   fields (),
	         profileCurveNode (),
	      trajectoryCurveNode (),
	            extrusionNode (new Extrusion (executionContext))
{
	addType (X3DConstants::NurbsSwungSurface);

	addField (inputOutput,    "metadata",        metadata ());
	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (inputOutput,    "profileCurve",    profileCurve ());
	addField (inputOutput,    "trajectoryCurve", trajectoryCurve ());

	addChildObjects (profileCurveNode,
	                 trajectoryCurveNode,
	                 extrusionNode);
}

X3DBaseNode*
NurbsSwungSurface::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsSwungSurface (executionContext);
}

void
NurbsSwungSurface::initialize ()
{
	X3DParametricGeometryNode::initialize ();

	profileCurve ()    .addInterest (&NurbsSwungSurface::set_profileCurve, this);
	trajectoryCurve () .addInterest (&NurbsSwungSurface::set_trajectoryCurve,   this);

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

	set_profileCurve ();
	set_trajectoryCurve ();
}

void
NurbsSwungSurface::set_profileCurve ()
{
	if (profileCurveNode)
		profileCurveNode -> removeInterest (&NurbsSwungSurface::requestRebuild, this);

	profileCurveNode .set (x3d_cast <X3DNurbsControlCurveNode*> (profileCurve ()));

	if (profileCurveNode)
		profileCurveNode -> addInterest (&NurbsSwungSurface::requestRebuild, this);
}

void
NurbsSwungSurface::set_trajectoryCurve ()
{
	if (trajectoryCurveNode)
		trajectoryCurveNode -> removeInterest (&NurbsSwungSurface::requestRebuild, this);

	trajectoryCurveNode .set (x3d_cast <X3DNurbsControlCurveNode*> (trajectoryCurve ()));

	if (trajectoryCurveNode)
		trajectoryCurveNode -> addInterest (&NurbsSwungSurface::requestRebuild, this);
}

void
NurbsSwungSurface::build ()
{
	if (not profileCurveNode)
		return;

	if (not trajectoryCurveNode)
		return;

	const auto extrusion       = MakePtr <Extrusion> (getExecutionContext ());
	const auto trajectoryCurve = trajectoryCurveNode -> tessellate ();
	auto       spine           = std::vector <Vector3f> ();

	for (const auto & point : trajectoryCurve)
		spine .emplace_back (point .x (), 0, point .y ());

	extrusionNode -> crossSection () = profileCurveNode -> tessellate ();
	extrusionNode -> spine ()        = std::move (spine);

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

NurbsSwungSurface::~NurbsSwungSurface ()
{ }

} // X3D
} // titania
