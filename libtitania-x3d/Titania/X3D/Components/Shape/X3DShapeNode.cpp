/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "../../Browser/Browser.h"
#include "../../Browser/Hit.h"
#include "../../Rendering/ViewVolume.h"
#include "../../Types/Geometry.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shape/X3DAppearanceNode.h"
#include "X3DShapeNode.h"

namespace titania {
namespace X3D {

const Matrix4d X3DShapeNode::textureMatrix = { 1,  0, 0, 0,
	                                            0, -1, 0, 0,
	                                            0,  0, 1, 0,
	                                            0,  1, 0, 1 };

X3DShapeNode::X3DShapeNode () :
	    X3DChildNode (), 
	X3DBoundedObject (), 
	      appearance (), // SFNode [in,out] appearance  NULL        [X3DAppearanceNode]
	        geometry ()  // SFNode [in,out] geometry    NULL        [X3DGeometryNode]
{
	addNodeType (X3DShapeNodeType);
}

Box3f
X3DShapeNode::getBBox ()
{
	if (bboxSize == Vector3f (-1, -1, -1))
	{
		if (geometry)
			return geometry -> getBBox ();
		else
			return Box3f ();
	}

	return Box3f (bboxSize, bboxCenter);
}

bool
X3DShapeNode::isTransparent ()
{
	if (appearance and appearance -> isTransparent ())
		return true;

	if (geometry and geometry -> isTransparent ())
		return true;

	return false;
}

void
X3DShapeNode::intersect ()
{
	if (not getBrowser () -> isSensitive ())
		return;

	if (geometry)
	{
		if (ViewVolume () .intersect (getBBox ()))
		{
			Line3f hitRay = getBrowser () -> getHitRay ();
			Hit*   hit    = new Hit ();

			if (geometry -> intersect (hitRay, hit))
			{
				hit -> position = hitRay .point ();
				hit -> nodes    = getBrowser () -> getSensitiveNodes ();
				getBrowser () -> addHit (hit);
			}
			else
				delete hit;
		}
	}
}

void
X3DShapeNode::display ()
{
	if (geometry)
	{
		Box3f bbox = getBBox ();

		Matrix4f centerMatrix = ModelViewMatrix4f ();
		centerMatrix .translate (bbox .center ());

		float radius = abs (centerMatrix .multDirMatrix (bbox .size ())) * 0.5f;

		if (radius > centerMatrix [3] [2])
			getBrowser () -> getCurrentRenderer () -> addShape (this, centerMatrix [3] [2] - radius);
	}
}

void
X3DShapeNode::draw ()
{
	glColor4f (1, 1, 1, 1);
	getBrowser () -> setObjectAlpha (1, 1);

	glMatrixMode (GL_TEXTURE);
	glLoadMatrixd (textureMatrix .data ());

	//glLoadIdentity();
	//glTranslatef(0, 1, 0);
	//glScalef(1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	if (appearance)
		appearance -> display ();

	geometry -> display ();
}

} // X3D
} // titania
