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
#include "../../Execution/X3DExecutionContext.h"
#include "../NURBS/NurbsPatchSurface.h"
#include "../Rendering/X3DCoordinateNode.h"

#include <Titania/Math/Algorithms/Barycentric.h>
#include <Titania/Math/Geometry/Triangle2.h>
#include <cassert>

namespace titania {
namespace X3D {

const std::string NurbsSurfaceInterpolator::componentName  = "NURBS";
const std::string NurbsSurfaceInterpolator::typeName       = "NurbsSurfaceInterpolator";
const std::string NurbsSurfaceInterpolator::containerField = "children";

const size_t NurbsSurfaceInterpolator::U_TESSELLATION = 128;
const size_t NurbsSurfaceInterpolator::V_TESSELLATION = 128;

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
	        geometry (new NurbsPatchSurface (executionContext))
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
	                 geometry);
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

	uOrder ()       .addInterest (geometry -> uOrder ());
	vOrder ()       .addInterest (geometry -> vOrder ());
	uDimension ()   .addInterest (geometry -> uDimension ());
	vDimension ()   .addInterest (geometry -> vDimension ());
	uKnot ()        .addInterest (geometry -> uKnot ());
	vKnot ()        .addInterest (geometry -> vKnot ());
	weight ()       .addInterest (geometry -> weight ());
	controlPoint () .addInterest (geometry -> controlPoint ());

	geometry -> uTessellation () = U_TESSELLATION;
	geometry -> vTessellation () = V_TESSELLATION;
	geometry -> uOrder ()        = uOrder ();
	geometry -> vOrder ()        = vOrder ();
	geometry -> uDimension ()    = uDimension ();
	geometry -> vDimension ()    = vDimension ();
	geometry -> uKnot ()         = uKnot ();
	geometry -> vKnot ()         = vKnot ();
	geometry -> weight ()        = weight ();
	geometry -> controlPoint ()  = controlPoint ();

	geometry -> setup ();
}

void
NurbsSurfaceInterpolator::set_fraction_ ()
{
	const auto & fraction       = set_fraction () .getValue ();
	const auto & multiTexCoords = geometry -> getPolygonTexCoords ();
	const auto & normals        = geometry -> getPolygonNormals ();
	const auto & vertices       = geometry -> getPolygonVertices ();

	if (multiTexCoords .empty ())
		return;

	const auto & texCoords = multiTexCoords .front ();

	for (size_t i = 0, size = texCoords .size (); i < size; i += 3)
	{
		const Triangle2f triangle2 (Vector2f (texCoords [i + 0] .x (), texCoords [i + 0] .y ()),
		                            Vector2f (texCoords [i + 1] .x (), texCoords [i + 1] .y ()),
		                            Vector2f (texCoords [i + 2] .x (), texCoords [i + 2] .y ()));

		if (triangle2 .contains (fraction))
		{
			const Line3f line (Vector3f (fraction .x (), fraction .y (), 0), Vector3f (0, 0, 1));

			const auto & [uvt, intersected] = line .intersects (Vector3f (texCoords [i + 0] .x (), texCoords [i + 0] .y (), 0),
			                                                    Vector3f (texCoords [i + 1] .x (), texCoords [i + 1] .y (), 0),
			                                                    Vector3f (texCoords [i + 2] .x (), texCoords [i + 2] .y (), 0));

			if (intersected)
			{
				normal_changed ()   = normalize (from_barycentric <float> (uvt, normals [i], normals [i + 1], normals [i + 2]));
				position_changed () = from_barycentric <double> (uvt, vertices [i], vertices [i + 1], vertices [i + 2]);
			}
		}
	}
}

NurbsSurfaceInterpolator::~NurbsSurfaceInterpolator ()
{ }

} // X3D
} // titania
