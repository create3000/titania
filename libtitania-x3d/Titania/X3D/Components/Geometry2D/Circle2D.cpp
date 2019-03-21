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

#include "Circle2D.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry2D/Circle2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/IndexedLineSet.h"

namespace titania {
namespace X3D {

const Component   Circle2D::component      = Component ("Geometry2D", 2);
const std::string Circle2D::typeName       = "Circle2D";
const std::string Circle2D::containerField = "geometry";

Circle2D::Fields::Fields () :
	 radius (new SFFloat (1)),
	options (new SFNode ())
{ }

Circle2D::Circle2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields (),
	        optionsNode ()
{
	addType (X3DConstants::Circle2D);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "radius",   radius ());
	addField (initializeOnly, "options",  options ());

	addChildObjects (optionsNode);

	radius () .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);

	setGeometryType (1);
}

X3DBaseNode*
Circle2D::create (X3DExecutionContext* const executionContext) const
{
	return new Circle2D (executionContext);
}

void
Circle2D::initialize ()
{
	X3DLineGeometryNode::initialize ();

	options () .addInterest (&Circle2D::set_options, this);

	try
	{
		const auto metaOptions = getMetadataSet ("/Circle2D/options");
	
		optionsNode .set (MakePtr <Circle2DOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&Circle2D::requestRebuild, this);
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
Circle2D::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getCircle2DOptions () .removeInterest (&Circle2D::requestRebuild, this);

	X3DLineGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

const X3DPtr <ComposedShader> &
Circle2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

Box3d
Circle2D::createBBox () const
{
	const double diameter = std::abs (radius ()) * 2;

	return Box3d (Vector3d (diameter, diameter, 0), Vector3d ());
}

void
Circle2D::set_options ()
{
	removeMetaData ("/Circle2D/options");

	if (optionsNode)
		optionsNode -> removeInterest (&Circle2D::requestRebuild, this);

	optionsNode .set (x3d_cast <Circle2DOptions*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getCircle2DOptions ());

	optionsNode -> addInterest (&Circle2D::requestRebuild, this);
}

void
Circle2D::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/Circle2D/options"));

	if (std::abs (radius ()) == 1.0f)
		getVertices () = optionsNode -> getVertices ();

	else
	{
		const double r = std::abs (radius () .getValue ());

		getVertices () .reserve (optionsNode -> getVertices () .size ());

		for (const auto & vertex : optionsNode -> getVertices ())
			getVertices () .emplace_back (vertex * r);
	}

	addElements (optionsNode -> getVertexMode (), getVertices () .size ());
	setSolid (false);
}

SFNode
Circle2D::toPrimitive () const
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Circle2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> coord () = coord;

	coord -> point () .assign (getVertices () .cbegin (), getVertices () .cend ());

	for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
		geometry -> coordIndex () .emplace_back (i);

	geometry -> coordIndex () .emplace_back (0);
	geometry -> coordIndex () .emplace_back (-1);

	return geometry;
}

Circle2D::~Circle2D ()
{ }

} // X3D
} // titania
