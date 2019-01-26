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

#include "Arc2D.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry2D/Arc2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/IndexedLineSet.h"

#include <complex>

namespace titania {
namespace X3D {

const Component   Arc2D::component      = Component ("Geometry2D", 2);
const std::string Arc2D::typeName       = "Arc2D";
const std::string Arc2D::containerField = "geometry";

Arc2D::Fields::Fields () :
	startAngle (new SFFloat ()),
	  endAngle (new SFFloat (1.570796)),
	    radius (new SFFloat (1)),
	   options (new SFNode ())
{ }

Arc2D::Arc2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields (),
	        optionsNode ()
{
	addType (X3DConstants::Arc2D);

	addField (inputOutput,    "metadata",   metadata ());
	addField (initializeOnly, "startAngle", startAngle ());
	addField (initializeOnly, "endAngle",   endAngle ());
	addField (initializeOnly, "radius",     radius ());
	addField (initializeOnly, "options",    options ());

	addChildObjects (optionsNode);

	startAngle () .setUnit (UnitCategory::ANGLE);
	endAngle ()   .setUnit (UnitCategory::ANGLE);
	radius ()     .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);

	setGeometryType (GeometryType::GEOMETRY_LINES);
}

X3DBaseNode*
Arc2D::create (X3DExecutionContext* const executionContext) const
{
	return new Arc2D (executionContext);
}

void
Arc2D::initialize ()
{
	X3DLineGeometryNode::initialize ();

	options () .addInterest (&Arc2D::set_options, this);

	try
	{
		const auto metaOptions = getMetadataSet ("/Arc2D/options");
	
		optionsNode .set (MakePtr <Arc2DOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&Arc2D::requestRebuild, this);
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
Arc2D::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getArc2DOptions () .removeInterest (&Arc2D::requestRebuild, this);

	X3DLineGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

const X3DPtr <ComposedShader> &
Arc2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

double
Arc2D::getSweepAngle ()
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
Arc2D::set_options ()
{
	removeMetaData ("/Arc2D/options");

	if (optionsNode)
		optionsNode -> removeInterest (&Arc2D::requestRebuild, this);

	optionsNode .set (x3d_cast <Arc2DOptions*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getArc2DOptions ());

	optionsNode -> addInterest (&Arc2D::requestRebuild, this);
}

void
Arc2D::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/Arc2D/options"));

	const double sweepAngle = getSweepAngle ();
	const auto   circle     = sweepAngle == pi2 <double>;
	int32_t      steps      = sweepAngle * optionsNode -> dimension () / pi2 <double>;
	GLenum       vertexMode = GL_LINE_LOOP;

	steps = std::max (3, steps);

	getVertices () .reserve (steps + 1);

	if (not circle)
	{
		++ steps;
		vertexMode = GL_LINE_STRIP;
	}

	const auto steps_1 = circle ? steps : steps - 1;

	for (int32_t n = 0; n < steps; ++ n)
	{
		const auto   t     = double (n) / steps_1;
		const double theta = startAngle () + (sweepAngle * t);
		const auto   point = std::polar <double> (std::abs (radius ()), theta);

		getVertices () .emplace_back (point .real (), point .imag (), 0);
	}

	addElements (vertexMode, getVertices () .size ());
	setSolid (false);
}

SFNode
Arc2D::toPrimitive () const
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Arc2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> coord () = coord;

	coord -> point () .assign (getVertices () .cbegin (), getVertices () .cend ());

	for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
		geometry -> coordIndex () .emplace_back (i);

	if (getElements () [0] .vertexMode () == GL_LINE_LOOP)
		geometry -> coordIndex () .emplace_back (0);

	geometry -> coordIndex () .emplace_back (-1);

	return geometry;
}

Arc2D::~Arc2D ()
{ }

} // X3D
} // titania
