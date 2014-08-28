/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "VolumeEmitter.h"

#include "../../Bits/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Miscellaneous/BVH.h"

namespace titania {
namespace X3D {

const std::string VolumeEmitter::componentName  = "ParticleSystems";
const std::string VolumeEmitter::typeName       = "VolumeEmitter";
const std::string VolumeEmitter::containerField = "emitter";

VolumeEmitter::Fields::Fields () :
	  internal (new SFBool (true)),
	 direction (new SFVec3f (0, 1, 0)),
	coordIndex (new MFInt32 ({ -1 })),
	     coord (new SFNode ())
{ }

VolumeEmitter::VolumeEmitter (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticleEmitterNode (),
	                fields (),
	           normalMapId (0),
	        normalBufferId (0),
	          surfaceMapId (0),
	       surfaceBufferId (0),
	      surfaceAreaMapId (0),
	   surfaceAreaBufferId (0),
	           volumeMapId (0),
	        volumeBufferId (0),
	           surfaceNode (new IndexedFaceSet (executionContext)),
	          pointEmitter (false),
	                 solid (true)
{
	addType (X3DConstants::VolumeEmitter);

	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "internal",       internal ());
	addField (inputOutput,    "direction",      direction ());
	addField (inputOutput,    "speed",          speed ());
	addField (inputOutput,    "variation",      variation ());
	addField (initializeOnly, "mass",           mass ());
	addField (initializeOnly, "surfaceArea",    surfaceArea ());
	addField (inputOutput,    "coordIndex",     coordIndex ());
	addField (inputOutput,    "coord",          coord ());

	addChildren (surfaceNode);
}

X3DBaseNode*
VolumeEmitter::create (X3DExecutionContext* const executionContext) const
{
	return new VolumeEmitter (executionContext);
}

void
VolumeEmitter::initialize ()
{
	X3DParticleEmitterNode::initialize ();

	if (not glXGetCurrentContext ())
		return;

	if (not getBrowser () -> hasExtension ("GL_ARB_texture_buffer_object"))
		return;

	// Surface map

	glGenTextures (1, &normalMapId);
	glGenBuffers  (1, &normalBufferId);

	glGenTextures (1, &surfaceMapId);
	glGenBuffers  (1, &surfaceBufferId);

	glGenTextures (1, &surfaceAreaMapId);
	glGenBuffers  (1, &surfaceAreaBufferId);

	glGenTextures (1, &volumeMapId);
	glGenBuffers  (1, &volumeBufferId);

	// Setup

	coordIndex () .addInterest (surfaceNode -> coordIndex ());
	coord ()      .addInterest (surfaceNode -> coord ());

	surfaceNode -> isPrivate (true);
	surfaceNode -> creaseAngle () = 0;
	surfaceNode -> convex ()      = false;
	surfaceNode -> coordIndex ()  = coordIndex ();
	surfaceNode -> coord ()       = coord ();

	surfaceNode -> setup ();
	surfaceNode -> addInterest (this, &VolumeEmitter::set_geometry);

	set_geometry ();
}

Box3f
VolumeEmitter::getBBox (const ParticleSystem* const)
{
	if (coord ())
		return surfaceNode -> getBBox ();

	return Box3f ();
}

MFString
VolumeEmitter::getShaderUrl () const
{
	return { get_shader ("ParticleSystems/VolumeEmitter.vs") .str () };
}

void
VolumeEmitter::addShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::addShaderFields (shader);

	shader -> addUserDefinedField (inputOutput, "pointEmitter", new SFBool (pointEmitter));
	shader -> addUserDefinedField (inputOutput, "direction",    new SFVec3f (normalize (direction () .getValue ())));
}

void
VolumeEmitter::setTextureBuffer (const X3DPtr <ComposedShader> & shader) const
{
	shader -> setTextureBuffer ("normalMap",       normalMapId);
	shader -> setTextureBuffer ("surfaceMap",      surfaceMapId);
	shader -> setTextureBuffer ("surfaceAreaMap",  surfaceAreaMapId);
	shader -> setTextureBuffer ("volumeMap",       volumeMapId);
}

void
VolumeEmitter::setShaderFields (const X3DPtr <ComposedShader> & shader) const
{
	try
	{
		X3DParticleEmitterNode::setShaderFields (shader);

		shader -> setField <SFBool>  ("pointEmitter", pointEmitter, true);
		shader -> setField <SFVec3f> ("direction",    normalize (direction () .getValue ()), true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
VolumeEmitter::set_geometry ()
{
	// Triangulate

	std::vector <Color4f>  colors;
	TexCoordArray          texCoords (surfaceNode -> getMultiTexCoords ());
	std::vector <Vector3f> normals;
	std::vector <Vector3f> vertices;

	surfaceNode -> triangulate (colors, texCoords, normals, vertices);

	float               surfaceArea = 0;
	std::vector <float> surfaceAreas (1);

	for (size_t i = 0, size = vertices .size (); i < size; i += 3)
	{
		surfaceArea += area (vertices [i], vertices [i + 1], vertices [i + 2]);
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

	// BVH

	const BVH  tree (std::move (vertices));
	const auto treeArray = tree .toArray ();

	glBindBuffer (GL_TEXTURE_BUFFER, volumeBufferId);
	glBufferData (GL_TEXTURE_BUFFER, treeArray .size () * sizeof (BVH::ArrayValue), pointEmitter ? 0 : treeArray .data (), GL_STATIC_COPY);

	// Update textures

	glBindTexture (GL_TEXTURE_BUFFER, normalMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, normalBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, surfaceMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, surfaceBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, surfaceAreaMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, surfaceAreaBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, volumeMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, volumeBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, 0);
	glBindBuffer (GL_TEXTURE_BUFFER, 0);
}

void
VolumeEmitter::dispose ()
{
	// Surface map

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

	if (volumeMapId)
		glDeleteTextures (1, &volumeMapId);

	if (volumeBufferId)
		glDeleteBuffers (1, &volumeBufferId);

	// Dispose base

	X3DParticleEmitterNode::dispose ();
}

} // X3D
} // titania
