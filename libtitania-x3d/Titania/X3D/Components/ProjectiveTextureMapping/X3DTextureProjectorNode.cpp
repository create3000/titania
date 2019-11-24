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

#include "X3DTextureProjectorNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ProjectiveTextureContainer.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

X3DTextureProjectorNode::Fields::Fields () :
	 description (new SFString ("")),
	          on (new SFBool (true)),
	      global (new SFBool (true)),
	    location (new SFVec3f (0, 0, 0)),
	   direction (new SFVec3f (0, 0, 1)),
	    upVector (new SFVec3f (0, 0, 1)),
	nearDistance (new SFFloat (1)),
	 farDistance (new SFFloat (10)),
	 aspectRatio (new SFFloat ()),
	     texture (new SFNode ())
{ }

X3DTextureProjectorNode::X3DTextureProjectorNode () :
	X3DChildNode (),
	      fields (),
	 textureNode ()
{
	addType (X3DConstants::X3DTextureProjectorNode);

	location ()     .setUnit (UnitCategory::LENGTH);
	nearDistance () .setUnit (UnitCategory::LENGTH);
	farDistance ()  .setUnit (UnitCategory::LENGTH);

	addChildObjects (textureNode);
}

void
X3DTextureProjectorNode::initialize ()
{
	X3DChildNode::initialize ();

	texture () .addInterest (&X3DTextureProjectorNode::set_texture, this);

	set_texture ();
}

void
X3DTextureProjectorNode::set_texture ()
{
	if (textureNode)
		textureNode -> removeInterest (&X3DTextureProjectorNode::set_aspectRatio, this);

	textureNode = x3d_cast <X3DTexture2DNode*> (texture ());

	if (textureNode)
		textureNode -> addInterest (&X3DTextureProjectorNode::set_aspectRatio, this);

	set_aspectRatio ();
}

void
X3DTextureProjectorNode::set_aspectRatio ()
{
	if (textureNode)
		aspectRatio () = double (textureNode -> getWidth ()) / double (textureNode -> getHeight ());
	else
		aspectRatio () = 0;
}

const Matrix4d &
X3DTextureProjectorNode::getBiasMatrix () const
{
	// Transforms normalized coords from range (-1, 1) to (0, 1).
	static const auto biasMatrix = Matrix4d (0.5, 0.0, 0.0, 0.0,
	                                         0.0, 0.5, 0.0, 0.0,
	                                         0.0, 0.0, 0.5, 0.0,
	                                         0.5, 0.5, 0.5, 1.0);

	return biasMatrix;
}

Rotation4d
X3DTextureProjectorNode::straightenHorizon (const Rotation4d & orientation) const
{
	const auto localXAxis = Vector3d (-1, 0, 0) * orientation;
	const auto localZAxis = Vector3d ( 0, 0, 1) * orientation;
	const auto upVector   = normalize (Vector3d (this -> upVector () .getValue ()));
	const auto vector     = cross (localZAxis, upVector);

	// If viewer looks along the up vector.
	if (std::abs (dot (localZAxis, upVector)) >= 1)
		return orientation;

	if (std::abs (dot (vector, localXAxis)) >= 1)
		return orientation;

	const auto rotation = Rotation4d (localXAxis, vector);

	return orientation * rotation;
}

void
X3DTextureProjectorNode::push (X3DRenderObject* const renderObject)
{
	if (not getBrowser () -> getProjectiveTextureMapping ())
		return;

	if (textureNode and on ())
	{
		if (renderObject -> isIndependent ())
		{
			if (global ())
			{
				const auto projectiveTextureContainer = std::make_shared <ProjectiveTextureContainer> (renderObject -> getBrowser (),
				                                                                                       this,
				                                                                                       renderObject -> getModelViewMatrix () .get ());

				renderObject -> getGlobalObjects ()      .emplace_back (projectiveTextureContainer);
				renderObject -> getProjectiveTextures () .emplace_back (projectiveTextureContainer);
			}
			else
			{
				const auto projectiveTextureContainer = std::make_shared <ProjectiveTextureContainer> (renderObject -> getBrowser (),
				                                                                                       this,
				                                                                                       renderObject -> getModelViewMatrix () .get ());

				renderObject -> getLocalObjects ()       .emplace_back (projectiveTextureContainer);
				renderObject -> getProjectiveTextures () .emplace_back (projectiveTextureContainer);
			}
		}
		else
		{
			const auto & projectiveTextureContainer = renderObject -> getProjectiveTextureContainer ();

			if (global ())
			{
				projectiveTextureContainer -> getModelViewMatrix () .push (renderObject -> getModelViewMatrix () .get ());

				renderObject -> getGlobalObjects ()      .emplace_back (projectiveTextureContainer);
				renderObject -> getProjectiveTextures () .emplace_back (projectiveTextureContainer);
			}
			else
			{
				projectiveTextureContainer -> getModelViewMatrix () .push (renderObject -> getModelViewMatrix () .get ());

				renderObject -> getLocalObjects ()       .emplace_back (projectiveTextureContainer);
				renderObject -> getProjectiveTextures () .emplace_back (projectiveTextureContainer);
			}
		}
	}
}

void
X3DTextureProjectorNode::pop (X3DRenderObject* const renderObject)
{
	if (not getBrowser () -> getProjectiveTextureMapping ())
		return;

	if (textureNode and on () and not global ())
		renderObject -> getLocalObjects () .pop_back ();
}

X3DTextureProjectorNode::~X3DTextureProjectorNode ()
{ }

} // X3D
} // titania
