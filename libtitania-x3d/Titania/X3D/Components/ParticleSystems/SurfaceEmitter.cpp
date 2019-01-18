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

#include "SurfaceEmitter.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Networking/config.h"
#include "../../Browser/ParticleSystems/Random.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

const std::string SurfaceEmitter::componentName  = "ParticleSystems";
const std::string SurfaceEmitter::typeName       = "SurfaceEmitter";
const std::string SurfaceEmitter::containerField = "emitter";

SurfaceEmitter::Fields::Fields () :
	surface (new SFNode ())
{ }

SurfaceEmitter::SurfaceEmitter (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticleEmitterNode (),
	                fields (),
	           normalMapId (0),
	        normalBufferId (0),
	          surfaceMapId (0),
	       surfaceBufferId (0),
	      surfaceAreaMapId (0),
	   surfaceAreaBufferId (0),
	           surfaceNode (),
	          pointEmitter (false),
	                 solid (true),
	        areaSoFarArray (1),
	               normals (),
	              vertices (),
	             direction ()
{
	addType (X3DConstants::SurfaceEmitter);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "speed",       speed ());
	addField (inputOutput,    "variation",   variation ());
	addField (initializeOnly, "mass",        mass ());
	addField (initializeOnly, "surfaceArea", surfaceArea ());
	addField (initializeOnly, "surface",     surface ());

	addChildObjects (surfaceNode);

	speed ()       .setUnit (UnitCategory::SPEED);
	mass ()        .setUnit (UnitCategory::MASS);
	surfaceArea () .setUnit (UnitCategory::AREA);
}

X3DBaseNode*
SurfaceEmitter::create (X3DExecutionContext* const executionContext) const
{
	return new SurfaceEmitter (executionContext);
}

void
SurfaceEmitter::initialize ()
{
	X3DParticleEmitterNode::initialize ();

	if (isSoftSystem ())
	{ }
	else
	{
		// Surface map
	
		glGenTextures (1, &normalMapId);
		glGenBuffers  (1, &normalBufferId);
	
		glGenTextures (1, &surfaceMapId);
		glGenBuffers  (1, &surfaceBufferId);
	
		glGenTextures (1, &surfaceAreaMapId);
		glGenBuffers  (1, &surfaceAreaBufferId);
	}

	// Setup

	surface () .addInterest (&SurfaceEmitter::set_surface, this);

	set_surface ();
}

Box3d
SurfaceEmitter::getBBox ()
{
	if (surfaceNode)
		return surfaceNode -> getBBox ();

	return Box3d ();
}

MFString
SurfaceEmitter::getShaderUrl () const
{
	return { get_shader ("ParticleSystems/SurfaceEmitter.vs") .str () };
}

void
SurfaceEmitter::addShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::addShaderFields (shader);

	shader -> addUserDefinedField (inputOutput, "pointEmitter", new SFBool (pointEmitter));
	shader -> addUserDefinedField (inputOutput, "solid",        new SFBool (solid));
}

void
SurfaceEmitter::setTextureBuffer (const X3DPtr <ComposedShader> & shader) const
{
	shader -> enable ();
	shader -> setTextureBuffer ("normalMap",      GL_TEXTURE_BUFFER, normalMapId);
	shader -> setTextureBuffer ("surfaceMap",     GL_TEXTURE_BUFFER, surfaceMapId);
	shader -> setTextureBuffer ("surfaceAreaMap", GL_TEXTURE_BUFFER, surfaceAreaMapId);
	shader -> disable ();
}

void
SurfaceEmitter::setShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	try
	{
		X3DParticleEmitterNode::setShaderFields (shader);

		shader -> setField <SFBool> ("pointEmitter", pointEmitter, true);
		shader -> setField <SFBool> ("solid",        solid,        true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
SurfaceEmitter::set_surface ()
{
	if (surfaceNode)
		surfaceNode -> rebuilded () .removeInterest (&SurfaceEmitter::set_geometry, this);

	surfaceNode .set (x3d_cast <X3DGeometryNode*> (surface ()));

	if (surfaceNode)
		surfaceNode -> rebuilded () .addInterest (&SurfaceEmitter::set_geometry, this);

	set_geometry ();
}

void
SurfaceEmitter::set_geometry ()
{
	if (isSoftSystem ())
	{
		if (surfaceNode)
		{
			// Triangulate
	
			std::vector <Color4f>  colors;
			TexCoordArray          texCoords (surfaceNode -> getPolygonTexCoords () .size ());
			std::vector <Vector3d> verticesAux;
	
			normals  .clear ();
			vertices .clear ();

			surfaceNode -> triangulate (colors, texCoords, normals, verticesAux);
	
			vertices .assign (verticesAux .cbegin (), verticesAux .cend ());

			// Calculate area so far array

			areaSoFarArray .resize (1);

			float areaSoFar = 0;
	
			for (size_t i = 0, size = vertices .size (); i < size; i += 3)
			{
				areaSoFar += Triangle3f (vertices [i], vertices [i + 1], vertices [i + 2]) .area ();
				areaSoFarArray .emplace_back (areaSoFar);
			}
		}
	}
	else
	{
		if (surfaceNode)
		{
			// Triangulate
	
			std::vector <Color4f>  colors;
			TexCoordArray          texCoords (surfaceNode -> getPolygonTexCoords () .size ());
			std::vector <Vector3f> normals;
			std::vector <Vector3d> verticesAux;
	
			surfaceNode -> triangulate (colors, texCoords, normals, verticesAux);
	
			const std::vector <Vector3f> vertices (verticesAux .cbegin (), verticesAux .cend ());
	
			float               surfaceArea = 0;
			std::vector <float> surfaceAreas (1);
	
			for (size_t i = 0, size = vertices .size (); i < size; i += 3)
			{
				surfaceArea += Triangle3f (vertices [i], vertices [i + 1], vertices [i + 2]) .area ();
				surfaceAreas .emplace_back (surfaceArea);
			}
	
			// Upload
	
			pointEmitter = vertices .empty ();
			solid        = surfaceNode -> getSolid ();
	
			glBindBuffer (GL_TEXTURE_BUFFER, normalBufferId);
			glBufferData (GL_TEXTURE_BUFFER, normals .size () * sizeof (Vector3f), pointEmitter ? 0 : normals .data (), GL_STATIC_COPY);
	
			glBindBuffer (GL_TEXTURE_BUFFER, surfaceBufferId);
			glBufferData (GL_TEXTURE_BUFFER, vertices .size () * sizeof (Vector3f), pointEmitter ? 0 : vertices .data (), GL_STATIC_COPY);
	
			glBindBuffer (GL_TEXTURE_BUFFER, surfaceAreaBufferId);
			glBufferData (GL_TEXTURE_BUFFER, surfaceAreas .size () * sizeof (float), pointEmitter ? 0 : surfaceAreas .data (), GL_STATIC_COPY);
		}
		else
		{
			pointEmitter = true;
	
			glBindBuffer (GL_TEXTURE_BUFFER, normalBufferId);
			glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
	
			glBindBuffer (GL_TEXTURE_BUFFER, surfaceBufferId);
			glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
	
			glBindBuffer (GL_TEXTURE_BUFFER, surfaceAreaBufferId);
			glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
		}
	
		// Update textures
	
		glBindTexture (GL_TEXTURE_BUFFER, normalMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, normalBufferId);
	
		glBindTexture (GL_TEXTURE_BUFFER, surfaceMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, surfaceBufferId);
	
		glBindTexture (GL_TEXTURE_BUFFER, surfaceAreaMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, surfaceAreaBufferId);
	
		glBindTexture (GL_TEXTURE_BUFFER, 0);
		glBindBuffer (GL_TEXTURE_BUFFER, 0);
	}
}

Vector3f
SurfaceEmitter::getRandomPosition () const
{
	if (not surfaceNode)
		return Vector3f ();

	// Determine index0.

	const size_t size     = areaSoFarArray .size ();
	const float  fraction = random1 (0, 1) * areaSoFarArray [size - 1];
	size_t       index0   = 0;

	if (size == 1 || fraction <= areaSoFarArray [0])
	{
		index0 = 0;
	}
	else if (fraction >= areaSoFarArray [size - 1])
	{
		index0 = size - 2;
	}
	else
	{
		const auto iter = std::upper_bound (areaSoFarArray .cbegin (), areaSoFarArray .cend (), fraction);

		if (iter not_eq areaSoFarArray .cend ())
		{
			index0 = (iter - areaSoFarArray .cbegin ()) - 1;
		}
		else
		{
			index0 = 0;
		}
	}

	// Random barycentric coordinates.

	auto u = random1 (0, 1);
	auto v = random1 (0, 1);

	if (u + v > 1)
	{
		u = 1 - u;
		v = 1 - v;
	}

	// Interpolate and set position.

	const auto i = index0 * 3;
	const auto t = 1 - u - v;

	const auto position = u * vertices [i + 0] + v * vertices [i + 1] + t * vertices [i + 2];

	// Interpolate and set direction for velocity.

	const_cast <SurfaceEmitter*> (this) -> direction = u * normals [i + 0] + v * normals [i + 1] + t * normals [i + 2];

	return position;
}

Vector3f
SurfaceEmitter::getRandomVelocity () const
{
	if (not surfaceNode)
		return getSphericalRandomVelocity ();

	return direction * getRandomSpeed ();
}

void
SurfaceEmitter::dispose ()
{
	// Surface map

	try
	{
		ContextLock lock (getBrowser ());

		if (normalMapId)
			glDeleteTextures (1, &normalMapId);

		if (normalBufferId)
			glDeleteBuffers (1, &normalBufferId);

		if (surfaceMapId)
			glDeleteTextures (1, &surfaceMapId);

		if (surfaceBufferId)
			glDeleteBuffers (1, &surfaceBufferId);

		if (surfaceAreaMapId)
			glDeleteTextures (1, &surfaceAreaMapId);

		if (surfaceAreaBufferId)
			glDeleteBuffers (1, &surfaceAreaBufferId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	// Dispose base

	X3DParticleEmitterNode::dispose ();
}

} // X3D
} // titania
