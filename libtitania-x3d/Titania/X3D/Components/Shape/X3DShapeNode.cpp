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

#include "X3DShapeNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Hit.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ViewVolume.h"
#include "../../Types/Geometry.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shape/X3DAppearanceNode.h"

namespace titania {
namespace X3D {

X3DShapeNode::X3DShapeNode () :
	    X3DChildNode (),     
	X3DBoundedObject (),     
	      appearance (),     // SFNode [in,out] appearance  NULL        [X3DAppearanceNode]
	        geometry (),     // SFNode [in,out] geometry    NULL        [X3DGeometryNode]
	     _appearance (NULL), 
	       _geometry (NULL)  
{
	addNodeType (X3DConstants::X3DShapeNode);
}

void
X3DShapeNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	appearance .addInterest (this, &X3DShapeNode::set_appearance);
	geometry   .addInterest (this, &X3DShapeNode::set_geometry);

	set_appearance ();
	set_geometry ();
}

Box3f
X3DShapeNode::getBBox ()
{
	if (bboxSize == Vector3f (-1, -1, -1))
	{
		if (_geometry)
			return _geometry -> getBBox ();

		else
			return Box3f ();
	}

	return Box3f (bboxSize, bboxCenter);
}

bool
X3DShapeNode::isTransparent () const
{
	if (_appearance and _appearance -> isTransparent ())
		return true;

	if (_geometry and _geometry -> isTransparent ())
		return true;

	return false;
}

void
X3DShapeNode::set_appearance ()
{
	_appearance = x3d_cast <X3DAppearanceNode*> (appearance .getValue ());
}

void
X3DShapeNode::set_geometry ()
{
	_geometry = x3d_cast <X3DGeometryNode*> (geometry .getValue ());
}

void
X3DShapeNode::traverse (TraverseType type)
{
	switch (type)
	{
		case TraverseType::PICKING:
		{
			pick ();
			break;
		}
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		{
			if (_geometry)
				getBrowser () -> getRenderers () .top () -> addCollision (this);

			break;
		}
		case TraverseType::COLLECT:
		{
			if (_geometry)
				getBrowser () -> getRenderers () .top () -> addShape (this);

			break;
		}
		default:
			break;
	}
}

void
X3DShapeNode::pick ()
{
	if (_geometry)
	{
		if (ViewVolume () .intersect (getBBox ()))
		{
			Line3f hitRay = getBrowser () -> getHitRay ();

			std::deque <Vector3f> hitPoints;

			if (_geometry -> intersect (hitRay, hitPoints))
			{
				for (auto & hitPoint : hitPoints)
					hitPoint = hitPoint * ModelViewMatrix4f ();
			 
			   // Sort desc
				std::sort (hitPoints .begin (),
				           hitPoints .end (),
				           [ ] (const Vector3f &lhs, const Vector3f &rhs) -> bool
				           {
				              return lhs .z () > rhs .z ();
							  });

				// Find first point that is not greater than near plane;
				auto hitPoint = std::lower_bound (hitPoints .cbegin (),
				                              hitPoints .cend (),
				                              -getCurrentNavigationInfo () -> getNearPlane (),
				                              [ ] (const Vector3f &lhs, const float & rhs) -> bool
				                              {
				                                 return lhs .z () > rhs;
														});

				if (hitPoint not_eq hitPoints .end ())
					getBrowser () -> addHit (*hitPoint, this);
			}
		}
	}
}

bool
X3DShapeNode::intersect (const Matrix4f & matrix, const Sphere3f & sphere) const
{
	if (_geometry)
		return _geometry -> intersect (matrix, sphere);

	return false;
}

void
X3DShapeNode::draw ()
{
	glColor4f (1, 1, 1, 1);

	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glScalef (1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glBindTexture (GL_TEXTURE_2D, 0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

	if (_appearance)
		_appearance -> draw ();

	_geometry -> draw ();

	glUseProgram (0);
	glDisable (GL_FOG);
	glDisable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_TEXTURE_CUBE_MAP);
	glDisable (GL_COLOR_MATERIAL);
}

void
X3DShapeNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
