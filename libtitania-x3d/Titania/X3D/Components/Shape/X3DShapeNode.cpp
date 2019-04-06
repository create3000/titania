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

#include "X3DShapeNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"
#include "../Shape/Appearance.h"
#include "../Shape/FillProperties.h"
#include "../Shape/LineProperties.h"
#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

X3DShapeNode::Fields::Fields () :
	appearance (new SFNode ()),
	  geometry (new SFNode ())
{ }

X3DShapeNode::X3DShapeNode () :
	         X3DChildNode (),
	     X3DBoundedObject (),
	               fields (),
	               hidden (false),
	       appearanceNode (),
	         geometryNode (),
	          transparent (false),
	glBindProgramPipeline ()
{
	addType (X3DConstants::X3DShapeNode);

	addChildObjects (appearanceNode,
	                 geometryNode);
}

void
X3DShapeNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	if (getBrowser () -> getExtension ("GL_ARB_separate_shader_objects"))
		glBindProgramPipeline = ::glBindProgramPipeline;
	else
		glBindProgramPipeline = [ ] (GLuint) { };

	appearance () .addInterest (&X3DShapeNode::set_appearance, this);
	geometry ()   .addInterest (&X3DShapeNode::set_geometry,   this);

	set_appearance ();
	set_geometry ();
}

void
X3DShapeNode::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DChildNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_appearance ();
}

void
X3DShapeNode::isHidden (const bool value)
{
	if (value not_eq hidden)
	{
		hidden = value;

		set_geometry ();

		getBrowser () -> addEvent ();
	}
}

void
X3DShapeNode::set_appearance ()
{
	if (appearanceNode)
		appearanceNode -> isTransparent () .removeInterest (&X3DShapeNode::set_transparent, this);

	appearanceNode = x3d_cast <X3DAppearanceNode*> (appearance ());

	if (appearanceNode)
		appearanceNode -> isTransparent () .addInterest (&X3DShapeNode::set_transparent, this);
	else
		appearanceNode .set (getBrowser () -> getDefaultAppearance ());

	set_transparent ();
}

void
X3DShapeNode::set_geometry ()
{
	if (geometryNode)
	{
		geometryNode -> isCameraObject () .removeInterest (&X3DShapeNode::setCameraObject, static_cast <X3DChildNode*> (this));
		geometryNode -> isTransparent ()  .removeInterest (&X3DShapeNode::set_transparent, this);
	}

	geometryNode .set (hidden ? nullptr : x3d_cast <X3DGeometryNode*> (geometry ()));

	if (geometryNode)
	{
		geometryNode -> isCameraObject () .addInterest (&X3DShapeNode::setCameraObject, static_cast <X3DChildNode*> (this));
		geometryNode -> isTransparent ()  .addInterest (&X3DShapeNode::set_transparent, this);
	}

	set_transparent ();

	setCameraObject (geometryNode and geometryNode -> isCameraObject ());
}

void
X3DShapeNode::set_transparent ()
{
	transparent = (appearanceNode and appearanceNode -> isTransparent ()) or
		           (geometryNode and geometryNode -> isTransparent ());
}

void
X3DShapeNode::display (ShapeContainer* const context)
{
	const auto browser      = context -> getBrowser ();
	const auto renderObject = context -> getRenderer ();
	const auto geometryType = getGeometryType ();

	appearanceNode -> enable (context);

	if (geometryType < 2)
	{
		appearanceNode -> getLineProperties () -> enable (renderObject);

		glDisable (GL_LIGHTING);
		draw (context);

		appearanceNode -> getLineProperties () -> disable (renderObject);
	}
	else
	{
		// Draw geometry.

		if (appearanceNode -> getFillProperties () -> filled ())
			draw (context);

		// Draw hatch on top of whatever appearance is specified.

		GLint polygonMode [2]; // Front and back value.
		glGetIntegerv (GL_POLYGON_MODE, polygonMode);

		if (polygonMode [0] == GL_FILL)
		{
			if (appearanceNode -> getFillProperties () -> hatched ())
			{
				appearanceNode -> getFillProperties () -> enable (renderObject);
				draw (context);
				appearanceNode -> getFillProperties () -> disable (renderObject);
			}
		}

		// Draw line geometry on top of whatever appearance is specified.

		if (appearanceNode -> getLineProperties () -> applied ())
		{
			if (polygonMode [0] == GL_FILL)
				glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

			appearanceNode -> getLineProperties () -> enable (renderObject);
			draw (context);
			appearanceNode -> getLineProperties () -> disable (renderObject);

			glPolygonMode (GL_FRONT, polygonMode [0]);
			glPolygonMode (GL_BACK,  polygonMode [1]);
		}
	}

	appearanceNode -> disable (context);

	glDisable (GL_FOG);
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
