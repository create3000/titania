/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "X3DTransformNode.h"

#include "../../Execution/X3DScene.h"

#include <Titania/Math/Utility/almost_equal.h>
#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

X3DTransformNode::Fields::Fields () :
	     translation (new SFVec3f ()),
	        rotation (new SFRotation ()),
	           scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	          center (new SFVec3f ())
{ }

X3DTransformNode::X3DTransformNode () :
	X3DTransformMatrix3DNode (),
	                  fields ()
{
	addType (X3DConstants::X3DTransformNode);

	translation () .setUnit (UnitCategory::LENGTH);
	center ()      .setUnit (UnitCategory::LENGTH);
}

void
X3DTransformNode::initialize ()
{
	X3DTransformMatrix3DNode::initialize ();

	addInterest (this, &X3DTransformNode::eventsProcessed);

	eventsProcessed ();
}

void
X3DTransformNode::eventsProcessed ()
{
	isHidden (scale () .getX () == 0 or
	          scale () .getY () == 0 or
	          scale () .getZ () == 0);

	X3DTransformMatrix3DNode::setMatrix (translation ()      .getValue (),
	                                     rotation ()         .getValue (),
	                                     scale ()            .getValue (),
	                                     scaleOrientation () .getValue (),
	                                     center ()           .getValue ());
}

Matrix4d
X3DTransformNode::getCurrentMatrix () const
{
	Matrix4d matrix;

	matrix .set (translation ()      .getValue (),
	             rotation ()         .getValue (),
	             scale ()            .getValue (),
	             scaleOrientation () .getValue (),
	             center ()           .getValue ());

	return matrix;
}

void
X3DTransformNode::setMatrix (const Matrix4d & matrix)
{
	setMatrixWithCenter (matrix, center () .getValue ());
}

void
X3DTransformNode::setMatrixWithCenter (const Matrix4d & matrix, const Vector3d & c)
{
	Vector3d   t, s;
	Rotation4d r, so;

	matrix .get (t, r, s, so, c);

	if (not almost_equal <float> (t, translation () .getValue (), 17))
		translation () = t;

	if (not almost_equal <float> (r, rotation () .getValue (), 17))
		rotation () = r;

	if (not almost_equal <float> (s, scale () .getValue (), 17))
		scale () = s;

	if (not almost_equal <float> (so, scaleOrientation () .getValue (), 17))
		scaleOrientation () = so;

	if (not almost_equal <float> (c, center () .getValue (), 17))
		center () = c;

	addEvent ();

	X3DTransformMatrix3DNode::setMatrix (matrix);
}

void
X3DTransformNode::setMatrixKeepCenter (const Matrix4d & matrix)
{
	try
	{
		auto centerOffset = Vector3d (center () .getValue ()) + Vector3d (translation () .getValue ()) - matrix .origin ();

		centerOffset = (~matrix) .mult_dir_matrix (centerOffset);

		setMatrixWithCenter (matrix, centerOffset);
	}
	catch (const std::domain_error &)
	{
		// Catch matrix inverse.
	}
}

} // X3D
} // titania
