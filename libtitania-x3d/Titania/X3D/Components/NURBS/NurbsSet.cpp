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

#include "NurbsSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../NURBS/X3DNurbsSurfaceGeometryNode.h"

namespace titania {
namespace X3D {

const Component   NurbsSet::component      = Component ("NURBS", 2);
const std::string NurbsSet::typeName       = "NurbsSet";
const std::string NurbsSet::containerField = "children";

NurbsSet::Fields::Fields () :
	tessellationScale (new SFFloat (1)),
	      addGeometry (new MFNode ()),
	   removeGeometry (new MFNode ()),
	         geometry (new MFNode ())
{ }

NurbsSet::NurbsSet (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	X3DBoundedObject (),
	          fields (),
	   geometryNodes ()
{
	addType (X3DConstants::NurbsSet);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "tessellationScale", tessellationScale ());
	addField (initializeOnly, "bboxSize",          bboxSize ());
	addField (initializeOnly, "bboxCenter",        bboxCenter ());
	addField (inputOnly,      "addGeometry",       addGeometry ());
	addField (inputOnly,      "removeGeometry",    removeGeometry ());
	addField (inputOutput,    "geometry",          geometry ());

	addChildObjects (geometryNodes);
}

X3DBaseNode*
NurbsSet::create (X3DExecutionContext* const executionContext) const
{
	return new NurbsSet (executionContext);
}

void
NurbsSet::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	tessellationScale () .addInterest (&NurbsSet::set_tessellationScale, this);
	addGeometry ()       .addInterest (&NurbsSet::set_addGeometry,       this);
	removeGeometry ()    .addInterest (&NurbsSet::set_removeGeometry,    this);
	geometry ()          .addInterest (&NurbsSet::set_geometry,          this);

	set_geometry ();
}

Box3d
NurbsSet::getBBox () const
{
	auto bbox = Box3d ();

	for (const auto & geometryNode : geometryNodes)
		bbox += geometryNode -> getBBox ();

	return bbox;
}

void
NurbsSet::set_tessellationScale ()
{
	const auto value = std::max <float> (0, tessellationScale ());

	for (const auto & geometryNode : geometryNodes)
		geometryNode -> setTessellationScale (value);
}

void
NurbsSet::set_addGeometry ()
{
	addGeometry () .setTainted (true);
	addGeometry () .set_difference (geometry ());

	geometry () .insert (geometry () .end (), addGeometry () .cbegin (), addGeometry () .cend ());

	addGeometry () .set ({ });
	addGeometry () .setTainted (false);
}

void
NurbsSet::set_removeGeometry ()
{
	geometry () .set_difference (removeGeometry ());

	removeGeometry () .set ({ });
}

void
NurbsSet::set_geometry ()
{
	for (const auto & geometryNode : geometryNodes)
		geometryNode -> setTessellationScale (1);

	geometryNodes .clear ();

	for (const auto & child : geometry ())
	{
		auto geometryNode = x3d_cast <X3DNurbsSurfaceGeometryNode*> (child);

		if (geometryNode)
			geometryNodes .emplace_back (geometryNode);
	}

	set_tessellationScale ();
}

void
NurbsSet::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

NurbsSet::~NurbsSet ()
{ }

} // X3D
} // titania
