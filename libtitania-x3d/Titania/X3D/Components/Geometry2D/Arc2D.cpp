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

#include "../../Browser/Geometry2D/Arc2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Execution/X3DExecutionContext.h"


namespace titania {
namespace X3D {

const ComponentType Arc2D::component      = ComponentType::GEOMETRY_2D;
const std::string   Arc2D::typeName       = "Arc2D";
const std::string   Arc2D::containerField = "geometry";

Arc2D::Fields::Fields () :
	startAngle (new SFFloat ()),
	  endAngle (new SFFloat (1.570796)),
	    radius (new SFFloat (1))
{ }

Arc2D::Arc2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields ()
{
	addType (X3DConstants::Arc2D);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "startAngle", startAngle ());
	addField (inputOutput,    "endAngle",   endAngle ());
	addField (initializeOnly, "radius",     radius ());

	startAngle () .setUnit (UnitCategory::ANGLE);
	endAngle ()   .setUnit (UnitCategory::ANGLE);
	radius ()     .setUnit (UnitCategory::LENGTH);

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
	using E = void (X3DBaseNode::*) ();

	X3DLineGeometryNode::initialize ();

	getBrowser () -> getArc2DOptions () .addInterest ((E) &Arc2D::addEvent, this);
}

void
Arc2D::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	using E = void (X3DBaseNode::*) ();

	if (isInitialized ())
		getBrowser () -> getArc2DOptions () .removeInterest ((E) &Arc2D::addEvent, this);

	X3DLineGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getArc2DOptions () .addInterest ((E) &Arc2D::addEvent, this);
}

const X3DPtr <ComposedShader> &
Arc2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

double
Arc2D::getAngle ()
{
	const double start = math::interval <double> (startAngle (), 0, pi2 <double>);
	const double end   = math::interval <double> (endAngle (),   0, pi2 <double>);

	if (start == end)
		return pi2 <double>;

	const double difference = std::abs (end - start);

	if (start > end)
		return pi2 <double> - difference;

	if (not std::isnan (difference))
		return difference;
	
	// We must test for NAN, as NAN to int32_t is undefined.
	return 0;
}

void
Arc2D::build ()
{
	const auto & options = getBrowser () -> getArc2DOptions ();

	const double difference = getAngle ();
	size_t       segments   = std::ceil (difference / options -> minAngle ());
	const double angle      = difference / segments;
	GLenum       vertexMode = GL_LINE_LOOP;

	getVertices () .reserve (segments + 1);

	if (difference < pi2 <double>)
	{
		++ segments;
		vertexMode = GL_LINE_STRIP;
	}
	else
		vertexMode = GL_LINE_LOOP;

	for (size_t n = 0; n < segments; ++ n)
	{
		const double theta = startAngle () + angle * n;
		const auto   point = std::polar <double> (std::abs (radius ()), theta);

		getVertices () .emplace_back (point .real (), point .imag (), 0);
	}

	addElements (vertexMode, getVertices () .size ());
	setSolid (false);
}

SFNode
Arc2D::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Arc2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> metadata () = metadata ();
	geometry -> coord ()    = coord;

	coord -> point () .assign (getVertices () .begin (), getVertices () .end ());

	for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
		geometry -> coordIndex () .emplace_back (i);

	if (getElements () [0] .vertexMode () == GL_LINE_LOOP)
		geometry -> coordIndex () .emplace_back (0);

	geometry -> coordIndex () .emplace_back (-1);

	return geometry;
}

} // X3D
} // titania
