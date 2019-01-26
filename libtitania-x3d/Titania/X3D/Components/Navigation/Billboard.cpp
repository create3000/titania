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

#include "Billboard.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   Billboard::component      = Component ("Navigation", 2);
const std::string Billboard::typeName       = "Billboard";
const std::string Billboard::containerField = "children";

// https://bitbucket.org/Coin3D/coin/src/abc9f50968c9/src/vrml97/Billboard.cpp

static constexpr Vector3d xAxis (1, 0, 0);
static constexpr Vector3d yAxis (0, 1, 0);
static constexpr Vector3d zAxis (0, 0, 1);

Billboard::Fields::Fields () :
	axisOfRotation (new SFVec3f (yAxis))
{ }

Billboard::Billboard (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	           X3DGroupingNode (),
	X3DTransformMatrix3DObject (),
	                    fields ()
{
	addType (X3DConstants::Billboard);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "axisOfRotation", axisOfRotation ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
Billboard::create (X3DExecutionContext* const executionContext) const
{
	return new Billboard (executionContext);
}

void
Billboard::initialize ()
{
	X3DGroupingNode::initialize ();
	X3DTransformMatrix3DObject::initialize ();
}

Box3d
Billboard::getBBox () const
{
	return X3DGroupingNode::getBBox () * getMatrix ();
}

///  throws std::domain_error
const Matrix4d &
Billboard::rotate (X3DRenderObject* const renderObject)
{
	const auto invModelViewMatrix = inverse (renderObject -> getModelViewMatrix () .get ());
	const auto billboardToViewer  = normalize (invModelViewMatrix .origin ());       // Normalized to get work with Geo

	if (axisOfRotation () == Vector3f ())
	{
		const auto viewerYAxis = normalize (invModelViewMatrix .mult_dir_matrix (yAxis)); // Normalized to get work with Geo

		auto x = cross (viewerYAxis, billboardToViewer);
		auto y = cross (billboardToViewer, x);
		auto z = billboardToViewer;

		// Compose rotation

		x .normalize ();
		y .normalize ();

		setMatrix (Matrix4d (x [0], x [1], x [2], 0,
		                     y [0], y [1], y [2], 0,
		                     z [0], z [1], z [2], 0,
		                     0,     0,     0,     1));
	}
	else
	{
		const auto N1 = cross <double> (axisOfRotation () .getValue (), billboardToViewer); // Normal vector of plane as in specification
		const auto N2 = cross <double> (axisOfRotation () .getValue (), zAxis);             // Normal vector of plane between axisOfRotation and zAxis

		setMatrix (Matrix4d (Rotation4d (N2, N1)));                                            // Rotate zAxis in plane
	}

	return getMatrix ();
}

void
Billboard::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	renderObject -> getModelViewMatrix () .push ();

	try
	{
		switch (type)
		{
			case TraverseType::CAMERA:
			case TraverseType::DEPTH:
				// No clone support for shadow and generated cube map texture
				renderObject -> getModelViewMatrix () .mult_left (getMatrix ());
				break;
			default:
				renderObject -> getModelViewMatrix () .mult_left (rotate (renderObject));
				break;
		}

		X3DGroupingNode::traverse (type, renderObject);
	}
	catch (const std::domain_error &)
	{ }

	renderObject -> getModelViewMatrix () .pop ();
}

void
Billboard::dispose ()
{
	X3DTransformMatrix3DObject::dispose ();
	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
