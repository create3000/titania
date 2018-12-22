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

#include "X3DLineGeometryNode.h"

#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/X3DShaderNode.h"
#include "../Shaders/X3DVertexAttributeNode.h"

namespace titania {
namespace X3D {

X3DLineGeometryNode::X3DLineGeometryNode () :
	X3DGeometryNode ()
{
	addType (X3DConstants::X3DLineGeometryNode);
}

bool
X3DLineGeometryNode::intersects (Line3d line,
                                 const ClipPlaneContainerArray & clipPlanes,
                                 Matrix4d modelViewMatrix,
                                 std::vector <IntersectionPtr> & intersections) const
{
	return false;
}

bool
X3DLineGeometryNode::intersects (Box3d box,
                                 const ClipPlaneContainerArray & clipPlanes,
                                 Matrix4d modelViewMatrix) const
{
	return false;
}

std::vector <Vector3d>
X3DLineGeometryNode::intersects (X3DRenderObject* const renderObject,
                                 const std::shared_ptr <FrameBuffer> & frameBuffer,
                       	         const std::shared_ptr <FrameBuffer> & depthBuffer,
                       	         std::vector <IntersectionPtr> & intersections)
{
	std::vector <Vector3d> hitPoints;

	return hitPoints;
}

void
X3DLineGeometryNode::depth (const X3DShapeContainer* const context)
{ }

void
X3DLineGeometryNode::draw (ShapeContainer* const context)
{
	try
	{
		const auto browser       = context -> getBrowser ();
		const bool pointShading  = browser -> getRenderingProperties () -> getShading () == ShadingType::POINT;
		auto       shaderNode    = browser -> getShader ();
	
		if (browser -> getFixedPipelineRequired ())
		{
			glDisable (GL_LIGHTING);
			X3DGeometryNode::draw (context);
			return;
		}
	
		if (shaderNode == browser -> getDefaultShader ())
			shaderNode = getShaderNode (browser);
	
		// Setup shader.
	
		context -> setGeometryType  (getGeometryType ());
		context -> setColorMaterial (not getColors () .empty ());
	
		shaderNode -> enable ();
		shaderNode -> setLocalUniforms (context);
	
		// Setup vertex attributes.
	
		for (size_t i = 0, size = getAttribs () .size (); i < size; ++ i)
			getAttribs () [i] -> enable (shaderNode, getAttribBufferIds () [i]);
	
		if (not getColors () .empty ())
			shaderNode -> enableColorAttrib (getColorBufferId (), GL_FLOAT, 0, nullptr);
	
		shaderNode -> enableVertexAttrib (getVertexBufferId (), GL_DOUBLE, 0, nullptr);
	
		// Draw
		// Wireframes are always solid so only one drawing call is needed.
	
		for (const auto & element : getElements ())
		{
			glDrawArrays (pointShading ? GL_POINTS : element .vertexMode (), element .first (), element .count ());
		}
	
		// VertexAttribs
	
		for (size_t i = 0, size = getAttribs () .size (); i < size; ++ i)
			getAttribs () [i] -> disable (shaderNode);
	
		shaderNode -> disableColorAttrib ();
		shaderNode -> disableVertexAttrib ();
		shaderNode -> disable ();
	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DLineGeometryNode::drawParticles (ShapeContainer* const context, const std::vector <SoftParticle> & particles, const size_t numParticles)
{
	try
	{
		const auto browser       = context -> getBrowser ();
		const bool pointShading  = browser -> getRenderingProperties () -> getShading () == ShadingType::POINT;
		auto       shaderNode    = browser -> getShader ();
	
		if (not shaderNode)
			return;
	
		if (shaderNode == browser -> getDefaultShader ())
			shaderNode = getShaderNode (browser);
	
		// Setup shader.
	
		context -> setGeometryType  (getGeometryType ());
		context -> setColorMaterial (not getColors () .empty ());
	
		shaderNode -> enable ();
		shaderNode -> setLocalUniforms (context);
	
		// Setup vertex attributes.
	
		for (size_t i = 0, size = getAttribs () .size (); i < size; ++ i)
			getAttribs () [i] -> enable (shaderNode, getAttribBufferIds () [i]);
	
		if (not getColors () .empty ())
			shaderNode -> enableColorAttrib (getColorBufferId (), GL_FLOAT, 0, nullptr);
	
		shaderNode -> enableVertexAttrib (getVertexBufferId (), GL_DOUBLE, 0, nullptr);
	
		// Draw

		auto       modelViewMatrix = context -> getModelViewMatrix ();
		const auto origin          = modelViewMatrix .origin ();

		for (size_t p = 0; p < numParticles; ++ p)
		{
			const auto & particle = particles [p];

			modelViewMatrix .origin (origin);
			modelViewMatrix .translate (particle .position);

			shaderNode -> setParticle (p, particle, modelViewMatrix);

			// Wireframes are always solid so only one drawing call is needed.
		
			for (const auto & element : getElements ())
			{
				glDrawArrays (pointShading ? GL_POINTS : element .vertexMode (), element .first (), element .count ());
			}
		}
	
		// VertexAttribs
	
		for (size_t i = 0, size = getAttribs () .size (); i < size; ++ i)
			getAttribs () [i] -> disable (shaderNode);
	
		shaderNode -> disableColorAttrib ();
		shaderNode -> disableVertexAttrib ();
		shaderNode -> disable ();
	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

X3DLineGeometryNode::~X3DLineGeometryNode ()
{ }

} // X3D
} // titania
