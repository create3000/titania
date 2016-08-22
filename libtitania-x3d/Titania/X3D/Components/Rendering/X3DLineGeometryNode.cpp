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

#include "X3DLineGeometryNode.h"

#include "../../Browser/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/X3DShaderNode.h"

namespace titania {
namespace X3D {

X3DLineGeometryNode::X3DLineGeometryNode () :
	X3DGeometryNode (),
	     shaderNode ()
{
	//addType (X3DConstants::X3DLineGeometryNode);

	addChildren (shaderNode);
}

void
X3DLineGeometryNode::setShader (const X3DPtr <ComposedShader> & value)
{
	shaderNode .set (value);
}

void
X3DLineGeometryNode::draw (ShapeContainer* const context)
{
	const auto & browser      = getBrowser ();
	const auto & shading      = browser -> getRenderingProperties () -> getShading ();
	const bool   pointShading = shading == ShadingType::POINT;
	auto         shaderNode   = browser -> getShader ();

	#ifndef SHADER_PIPELINE
	if (not browser -> getShaderPipeline () and shading not_eq ShadingType::PHONG)
	{
		glDisable (GL_LIGHTING);
		X3DGeometryNode::draw (context);
		return;
	}
	#endif

	if (shaderNode == browser -> getDefaultShader ())
		shaderNode = this -> shaderNode;

	// Setup shader.

	context -> setColorMaterial (not getColors () .empty ());

	shaderNode -> setGlobalUniforms (context);
	shaderNode -> setLocalUniforms (context);

	// Setup vertex attributes.

	if (not getAttribs () .empty ())
	{
		GLint program = 0;

		glGetIntegerv (GL_CURRENT_PROGRAM, &program);

		if (program)
		{
			for (size_t i = 0, size = getAttribs () .size (); i < size; ++ i)
				getAttribs () [i] -> enable (program, getAttribBufferIds () [i]);
		}
	}

	if (not getColors () .empty ())
		shaderNode -> enableColorAttrib (getColorBufferId ());

	shaderNode -> enableVertexAttrib (getVertexBufferId ());

	// Draw
	// Wireframes are always solid so only one drawing call is needed.

	size_t first = 0;

	for (const auto & element : getElements ())
	{
		glDrawArrays (pointShading ? GL_POINTS : element .vertexMode, first, element .count);
		first += element .count;
	}

	shaderNode -> disableColorAttrib ();
	shaderNode -> disableVertexAttrib ();

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

X3DLineGeometryNode::~X3DLineGeometryNode ()
{ }

} // X3D
} // titania
