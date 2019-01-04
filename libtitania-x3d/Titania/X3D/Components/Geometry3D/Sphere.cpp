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

#include "Sphere.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry3D/IcosahedronOptions.h"
#include "../../Browser/Geometry3D/OctahedronOptions.h"
#include "../../Browser/Geometry3D/QuadSphereOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"

namespace titania {
namespace X3D {

const std::string Sphere::componentName  = "Geometry3D";
const std::string Sphere::typeName       = "Sphere";
const std::string Sphere::containerField = "geometry";

Sphere::Fields::Fields () :
	 radius (new SFFloat (1)),
	  solid (new SFBool (true)),
	options (new SFNode ())
{ }

Sphere::Sphere (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	    optionsNode ()
{
	addType (X3DConstants::Sphere);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "radius",   radius ());
	addField (initializeOnly, "solid",    solid ());
	addField (initializeOnly, "options",  options ());

	addChildObjects (optionsNode);

	radius () .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);
}

X3DBaseNode*
Sphere::create (X3DExecutionContext* const executionContext) const
{
	return new Sphere (executionContext);
}

void
Sphere::initialize ()
{
	X3DGeometryNode::initialize ();

	options () .addInterest (&Sphere::set_options, this);

	try
	{
		const auto typeName    = getMetaData <std::string> ("/Sphere/options/@typeName", "QuadSphereOptions");
		const auto metaOptions = getMetadataSet ("/Sphere/options");
	
		if (typeName == "IcosahedronOptions")
			optionsNode .set (MakePtr <IcosahedronOptions> (getExecutionContext ()));
		else if (typeName == "OctahedronOptions")
			optionsNode .set (MakePtr <OctahedronOptions> (getExecutionContext ()));
		else // QuadSphereProperties
			optionsNode .set (MakePtr <QuadSphereOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&Sphere::addEvent, this);
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
Sphere::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

Box3d
Sphere::createBBox () const
{
	const double diameter = 2 * std::abs (radius ());

	return Box3d (Vector3d (diameter, diameter, diameter), Vector3d ());
}

void
Sphere::set_options ()
{
	removeMetaData ("/Sphere/options");

	if (optionsNode)
		optionsNode -> removeInterest (&Sphere::addEvent, this);

	optionsNode .set (x3d_cast <X3DSphereOptionsNode*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getSphereOptions ());

	optionsNode -> addInterest (&Sphere::addEvent, this);
}

void
Sphere::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/Sphere/options"));

	getTexCoords () .emplace_back (optionsNode -> getTexCoords ());

	getNormals ()  = optionsNode -> getNormals ();
	getVertices () = optionsNode -> getVertices ();

	if (radius () not_eq 1.0f)
	{
		for (auto & vertex : getVertices ())
			vertex *= double (radius () .getValue ());
	}

	addElements (optionsNode -> getVertexMode (), getVertices () .size ());
	setSolid (solid ());
}

SFNode
Sphere::toPrimitive () const
{
	const auto geometry = optionsNode -> toPrimitive (getExecutionContext ());

	geometry -> getField <SFBool> ("solid") = solid ();

	if (radius () not_eq 1.0f)
	{
		for (MFVec3f::reference point : geometry -> getField <SFNode> ("coord") -> getField <MFVec3f> ("point"))
			point = point .get () * radius () .getValue ();
	}

	return geometry;
}

Sphere::~Sphere ()
{ }

} // X3D
} // titania
