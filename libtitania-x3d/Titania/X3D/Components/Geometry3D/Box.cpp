/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "Box.h"

#include "../../Browser/Geometry3D/BoxOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType Box::component      = ComponentType::GEOMETRY_3D;
const std::string   Box::typeName       = "Box";
const std::string   Box::containerField = "geometry";

Box::Fields::Fields () :
	 size (new SFVec3f (2, 2, 2)),
	solid (new SFBool (true))
{ }

Box::Box (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Box);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "size",     size ());
	addField (initializeOnly, "solid",    solid ());

	size () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
Box::create (X3DExecutionContext* const executionContext) const
{
	return new Box (executionContext);
}

void
Box::initialize ()
{
	X3DGeometryNode::initialize ();

	//getBrowser () -> getBoxOptions () .addInterest (this, &Box::update);
}

//void
//Box::setExecutionContext (X3DExecutionContext* const executionContext)
//throw (Error <INVALID_OPERATION_TIMING>,
//       Error <DISPOSED>)
//{
//	getBrowser () -> getBoxOptions () .removeInterest (this, &Box::update);
//
//	X3DGeometryNode::setExecutionContext (executionContext);
//
//	if (isInitialized ())
//		getBrowser () -> getBoxOptions () .addInterest (this, &Box::update);
//}

Box3d
Box::createBBox ()
{
	return Box3d (size () .getValue (), Vector3d ());
}

void
Box::build ()
{
	const auto & options = getBrowser () -> getBoxOptions ();

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (options -> getTexCoords () .size ());
	getTexCoords () [0] = options -> getTexCoords ();

	getNormals () .reserve (options -> getNormals () .size ());
	getNormals  () = options -> getNormals  ();

	if (size () == Vector3f (2, 2, 2))
		getVertices () = options -> getVertices ();

	else
	{
		getVertices () .reserve (options -> getVertices () .size ());

		const auto size1_2 = Vector3d (size () .getValue ()) / 2.0;

		for (const auto & vertex : options -> getVertices ())
			getVertices () .emplace_back (vertex * size1_2);
	}

	addElements (options -> getVertexMode (), getVertices () .size ());
	setSolid (solid ());
	setTextureCoordinate (nullptr);
}

SFNode
Box::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto & options  = getBrowser () -> getBoxOptions ();
	const auto   geometry = options -> toPrimitive (getExecutionContext ());

	geometry -> getField <SFNode> ("metadata") = metadata ();
	geometry -> getField <SFBool> ("solid")    = solid ();

	if (size () == Vector3f (2, 2, 2))
		return geometry;

	const auto size1_2 = size () / 2.0f;

	for (auto & point : geometry -> getField <SFNode> ("coord") -> getField <MFVec3f> ("point"))
		point *= size1_2;

	return geometry;
}

} // X3D
} // titania
