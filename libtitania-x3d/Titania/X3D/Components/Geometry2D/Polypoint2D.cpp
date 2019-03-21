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

#include "Polypoint2D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/PointSet.h"
#include "../Shaders/ComposedShader.h"


namespace titania {
namespace X3D {

const Component   Polypoint2D::component      = Component ("Geometry2D", 1);
const std::string Polypoint2D::typeName       = "Polypoint2D";
const std::string Polypoint2D::containerField = "geometry";

Polypoint2D::Fields::Fields () :
	point (new MFVec2f ())
{ }

Polypoint2D::Polypoint2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields ()
{
	addType (X3DConstants::Polypoint2D);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "point",    point ());

	point () .setUnit (UnitCategory::LENGTH);

	setGeometryType (0);
	setTransparent (true);
}

X3DBaseNode*
Polypoint2D::create (X3DExecutionContext* const executionContext) const
{
	return new Polypoint2D (executionContext);
}

void
Polypoint2D::initialize ()
{
	X3DLineGeometryNode::initialize ();
}

const X3DPtr <ComposedShader> &
Polypoint2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getPointShader ();
}

void
Polypoint2D::build ()
{
	for (const auto & vertex : basic::make_const_range (point ()))
		getVertices () .emplace_back (vertex .x (), vertex .y (), 0);

	addElements (GL_POINTS, getVertices () .size ());
	setSolid (false);
}

SFNode
Polypoint2D::toPrimitive () const
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Polypoint2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <PointSet> ();

	geometry -> coord () = coord;

	coord -> point () .assign (getVertices () .cbegin (), getVertices () .cend ());

	return geometry;
}

} // X3D
} // titania
