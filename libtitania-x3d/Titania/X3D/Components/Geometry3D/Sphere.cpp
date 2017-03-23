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

#include "../../Browser/Geometry3D/QuadSphereProperties.h"
#include "../../Browser/Geometry3D/SphereOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"

namespace titania {
namespace X3D {

const ComponentType Sphere::component      = ComponentType::GEOMETRY_3D;
const std::string   Sphere::typeName       = "Sphere";
const std::string   Sphere::containerField = "geometry";

Sphere::Fields::Fields () :
	          radius (new SFFloat (1)),
	           solid (new SFBool (true)),
	useGlobalOptions (new SFBool (true)),
	      properties (new SFNode ())
{ }

Sphere::Sphere (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	 propertiesNode ()
{
	addType (X3DConstants::Sphere);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "radius",   radius ());
	addField (initializeOnly, "solid",    solid ());

	addField (initializeOnly, "useGlobalOptions", useGlobalOptions ());
	addField (initializeOnly, "properties",       properties ());

	addChildObjects (propertiesNode);

	radius () .setUnit (UnitCategory::LENGTH);

	useGlobalOptions () .isHidden (true);
	properties ()       .isHidden (true);
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

	useGlobalOptions () .addInterest (&Sphere::set_useGlobalOptions, this);
	properties ()       .addInterest (&Sphere::set_properties,       this);

	useGlobalOptions () .set (getMetaData <bool> ("/Sphere/useGlobalOptions", true));

__LOG__ << std::endl;
__LOG__ << properties () << std::endl;

	if (useGlobalOptions ())
		getBrowser () -> getSphereOptions () .addInterest (&Sphere::addEvent, this);

	else
	{
		const auto type = getMetaData <std::string> ("/Sphere/propertiesType", "QuadSphereProperties");

		if (type == "IcoSphereProperties" and false)
			; // TODO
		else // QuadSphereProperties
			propertiesNode .set (MakePtr <QuadSphereProperties> (getExecutionContext ()));

		propertiesNode -> addInterest (&Sphere::set_properties_node, this);
		propertiesNode -> fromMetaData (createMetadataSet ("/Sphere/properties"));
		propertiesNode -> setup ();

		properties () .set (propertiesNode);
	}
}

void
Sphere::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (isInitialized ())
		getBrowser () -> getSphereOptions () .removeInterest (&Sphere::addEvent, this);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		if (useGlobalOptions ())
			getBrowser () -> getSphereOptions () .addInterest (&Sphere::addEvent, this);
	}
}

Box3d
Sphere::createBBox () const
{
	const double diameter = 2 * std::abs (radius ());

	return Box3d (Vector3d (diameter, diameter, diameter), Vector3d ());
}

void
Sphere::set_useGlobalOptions ()
{
	if (useGlobalOptions ())
	{
		getBrowser () -> getSphereOptions () .addInterest (&Sphere::addEvent, this);

		removeMetaData ("/Sphere");
	}
	else
	{
		getBrowser () -> getSphereOptions () .removeInterest (&Sphere::addEvent, this);

		setMetaData ("/Sphere/useGlobalOptions", false);

		set_properties ();
	}
}

void
Sphere::set_properties ()
{
	removeMetaData ("/Sphere/properties");

	if (propertiesNode)
		propertiesNode -> removeInterest (&Sphere::set_properties_node, this);

	propertiesNode .set (properties ());

	if (not propertiesNode)
		propertiesNode .set (MakePtr <QuadSphereProperties> (getExecutionContext ()));

	propertiesNode -> addInterest (&Sphere::set_properties_node, this);

	set_properties_node ();
}

void
Sphere::set_properties_node ()
{
	if (useGlobalOptions ())
		return;

	setMetaData ("/Sphere/propertiesType", propertiesNode -> getTypeName ());

	propertiesNode -> toMetaData (createMetadataSet ("/Sphere/properties"));

	addEvent ();
}

void
Sphere::build ()
{
	if (useGlobalOptions ())
	{
		const auto & options = getBrowser () -> getSphereOptions ();
	
		getTexCoords () .emplace_back (options -> getTexCoords ());

		getNormals ()  = options -> getNormals  ();
		getVertices () = options -> getVertices ();

		addElements (options -> getVertexMode (), getVertices () .size ());
	}
	else
	{
		getTexCoords () .emplace_back (propertiesNode -> createTexCoords ());

		getVertices () = propertiesNode -> createVertices ();

		for (const auto & vertex : getVertices ())
			getNormals () .emplace_back (vertex);

		addElements (propertiesNode -> getVertexMode (), getVertices () .size ());
	}

	if (radius () not_eq 1.0f)
	{
		for (auto & vertex : getVertices ())
			vertex *= double (radius () .getValue ());
	}

	setSolid (solid ());
}

SFNode
Sphere::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto & options  = getBrowser () -> getSphereOptions ();
	const auto   geometry = useGlobalOptions ()
	                        ? options -> toPrimitive (getExecutionContext ())
	                        : propertiesNode -> toPrimitive (getExecutionContext ());

	geometry -> getField <SFBool> ("solid") = solid ();

	if (radius () not_eq 1.0f)
	{
		for (auto & point : geometry -> getField <SFNode> ("coord") -> getField <MFVec3f> ("point"))
			point *= radius () .getValue ();
	}

	return geometry;
}

Sphere::~Sphere ()
{ }

} // X3D
} // titania
