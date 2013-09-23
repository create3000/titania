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

#include "TextureTransform.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string TextureTransform::componentName  = "Texturing";
const std::string TextureTransform::typeName       = "TextureTransform";
const std::string TextureTransform::containerField = "textureTransform";

static
const Matrix3f textureMatrix = { 1,  0, 0,
	                              0, -1, 0,
	                              0,  1, 1 };

TextureTransform::Fields::Fields () :
	     center (new SFVec2f ()),
	   rotation (new SFFloat ()),
	      scale (new SFVec2f (1, 1)),
	translation (new SFVec2f ())
{ }

TextureTransform::TextureTransform (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureTransformNode (),
	                 fields ()
{
	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "translation", translation ());
	addField (inputOutput, "rotation",    rotation ());
	addField (inputOutput, "scale",       scale ());
	addField (inputOutput, "center",      center ());
}

X3DBaseNode*
TextureTransform::create (X3DExecutionContext* const executionContext) const
{
	return new TextureTransform (executionContext);
}

void
TextureTransform::initialize ()
{
	X3DTextureTransformNode::initialize ();
	eventsProcessed ();
}

void
TextureTransform::eventsProcessed ()
{
	X3DTextureTransformNode::eventsProcessed ();

	// Tc' = -C × S × R × C × T × Tc

	Matrix3f m = textureMatrix;

	if (center () not_eq Vector2f (0, 0))
		m .translate (-center ());

	if (scale () not_eq Vector2f (1, 1))
		m .scale (scale ());

	if (rotation () not_eq 0.0f)
		m .rotate (rotation ());

	if (center () not_eq Vector2f (0, 0))
		m .translate (center ());

	if (translation () not_eq Vector2f (0, 0))
		m .translate (translation ());

	matrix = m;
}

void
TextureTransform::draw ()
{
	glMatrixMode (GL_TEXTURE);

	glLoadMatrixf (matrix .data ());

	glMatrixMode (GL_MODELVIEW);
}


} // X3D
} // titania
