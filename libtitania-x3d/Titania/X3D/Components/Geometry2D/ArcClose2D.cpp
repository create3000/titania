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

#include "ArcClose2D.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry2D/ArcClose2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Coordinate.h"
#include "../Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace X3D {

const Component   ArcClose2D::component      = Component ("Geometry2D", 2);
const std::string ArcClose2D::typeName       = "ArcClose2D";
const std::string ArcClose2D::containerField = "geometry";

ArcClose2D::Fields::Fields () :
	closureType (new SFString ("PIE")),
	 startAngle (new SFFloat ()),
	   endAngle (new SFFloat (1.570796)),
	     radius (new SFFloat (1)),
	      solid (new SFBool ()),
	    options (new SFNode ())
{ }

ArcClose2D::ArcClose2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	    optionsNode ()
{
	addType (X3DConstants::ArcClose2D);

	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "closureType", closureType ());
	addField (initializeOnly, "startAngle",  startAngle ());
	addField (initializeOnly, "endAngle",    endAngle ());
	addField (initializeOnly, "radius",      radius ());
	addField (initializeOnly, "solid",       solid ());
	addField (initializeOnly, "options",     options ());

	addChildObjects (optionsNode);

	startAngle () .setUnit (UnitCategory::ANGLE);
	endAngle ()   .setUnit (UnitCategory::ANGLE);
	radius ()     .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);

	setGeometryType (GeometryType::GEOMETRY_2D);
}

X3DBaseNode*
ArcClose2D::create (X3DExecutionContext* const executionContext) const
{
	return new ArcClose2D (executionContext);
}

void
ArcClose2D::initialize ()
{
	X3DGeometryNode::initialize ();

	options () .addInterest (&ArcClose2D::set_options, this);

	try
	{
		const auto metaOptions = getMetadataSet ("/ArcClose2D/options");
	
		optionsNode .set (MakePtr <ArcClose2DOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&ArcClose2D::requestRebuild, this);
		optionsNode -> fromMetaData (metaOptions);
		optionsNode -> setup ();

		options () .set (optionsNode);
	}
	catch (const X3D::X3DError & error)
	{
		set_options ();
	}
}

void
ArcClose2D::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getArcClose2DOptions () .removeInterest (&ArcClose2D::requestRebuild, this);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

double
ArcClose2D::getSweepAngle () const
{
	const double start = math::interval <double> (startAngle (), 0, pi2 <double>);
	const double end   = math::interval <double> (endAngle (),   0, pi2 <double>);

	if (start == end)
		return pi2 <double>;

	const double sweepAngle = std::abs (end - start);

	if (start > end)
		return pi2 <double> - sweepAngle;

	if (not std::isnan (sweepAngle))
		return sweepAngle;
	
	// We must test for NAN, as NAN to int32_t is undefined.
	return 0;
}

void
ArcClose2D::set_options ()
{
	removeMetaData ("/ArcClose2D/options");

	if (optionsNode)
		optionsNode -> removeInterest (&ArcClose2D::requestRebuild, this);

	optionsNode .set (x3d_cast <ArcClose2DOptions*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getArcClose2DOptions ());

	optionsNode -> addInterest (&ArcClose2D::requestRebuild, this);
}

void
ArcClose2D::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/ArcClose2D/options"));

	const double sweepAngle = getSweepAngle ();
	const auto   circle     = sweepAngle == pi2 <double>;
	const auto   steps      = std::max <int32_t> (4, sweepAngle * optionsNode -> dimension () / (2 * pi <double>) + 1);

	const size_t elements = solid () ? 1 : 2;
	const size_t vertices = steps + 2;
	const size_t reserve  = elements * vertices;

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (reserve);

	getNormals  () .reserve (reserve);
	getVertices () .reserve (reserve);

	if (not circle)
	{
		// If it is a arc, add a center point otherwise it is a circle.

		if (closureType () not_eq "CHORD")
		{
			getTexCoords () [0] .emplace_back (0.5, 0.5, 0, 1);
			getNormals  () .emplace_back (0, 0, 1);
			getVertices () .emplace_back (0, 0, 0);
		}
	}

	const auto steps_1 = steps - 1;

	for (int32_t n = 0; n < steps; ++ n)
	{
		const auto   t        = double (n) / steps_1;
		const double theta    = startAngle () + (sweepAngle * t);
		const auto   texCoord = std::polar <double> (0.5, theta) + std::complex <double> (0.5, 0.5);
		const auto   point    = std::polar <double> (std::abs (radius ()), theta);

		getTexCoords () [0] .emplace_back (texCoord .real (), texCoord .imag (), 0, 1);
		getNormals  () .emplace_back (0, 0, 1);
		getVertices () .emplace_back (point .real (), point .imag (), 0);
	}

	addElements (GL_POLYGON, getVertices () .size ());
	setSolid (solid ());
	addMirrorVertices (GL_POLYGON, false);
}

SFNode
ArcClose2D::toPrimitive () const
{
	const double sweepAngle = getSweepAngle ();
	const auto   circle     = sweepAngle == pi2 <double>;
	const auto   steps      = std::max <int32_t> (4, sweepAngle * optionsNode -> dimension () / (2 * pi <double>) + 1);

	const auto texCoords = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord     = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry  = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> solid ()    = solid ();
	geometry -> convex ()   = false;
	geometry -> texCoord () = texCoords;
	geometry -> coord ()    = coord;

	if (not circle)
	{
		// If it is a arc, add a center point otherwise it is a circle.

		if (closureType () not_eq "CHORD")
		{
			texCoords -> point () .emplace_back (0.5, 0.5);
			coord -> point () .emplace_back (0, 0, 0);
		}
	}

	const auto steps_1 = steps - 1;

	for (int32_t n = 0, size = circle ? steps_1 : steps; n < size; ++ n)
	{
		const auto   t        = double (n) / steps_1;
		const double theta    = startAngle () + (sweepAngle * t);
		const auto   texCoord = std::polar <double> (0.5, theta) + std::complex <double> (0.5, 0.5);
		const auto   point    = std::polar <double> (std::abs (radius ()), theta);

		texCoords -> point () .emplace_back (texCoord .real (), texCoord .imag ());
		coord -> point () .emplace_back (point .real (), point .imag (), 0);
	}

	for (int32_t i = 0, size = coord -> point () .size (); i < size; ++ i)
	{
		geometry -> texCoordIndex () .emplace_back (i);
		geometry -> coordIndex ()    .emplace_back (i);
	}

	geometry -> texCoordIndex () .emplace_back (-1);
	geometry -> coordIndex ()    .emplace_back (-1);

	return geometry;
}

ArcClose2D::~ArcClose2D ()
{ }

} // X3D
} // titania
