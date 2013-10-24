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

#include "Appearance.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DShaderNode.h"
#include "../Shape/FillProperties.h"
#include "../Shape/LineProperties.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Texturing/X3DTextureNode.h"
#include "../Texturing/TextureTransform.h"

namespace titania {
namespace X3D {

const std::string Appearance::componentName  = "Shape";
const std::string Appearance::typeName       = "Appearance";
const std::string Appearance::containerField = "appearance";

Appearance::Fields::Fields () :
	  fillProperties (new SFNode ()),
	  lineProperties (new SFNode ()),
	        material (new SFNode ()),
	         texture (new SFNode ()),
	textureTransform (new SFNode ()),
	         shaders (new MFNode ())
{ }

Appearance::Appearance (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DAppearanceNode (),
	           fields (),
	  _fillProperties (nullptr),
	  _lineProperties (nullptr),
	        _material (nullptr),
	         _texture (nullptr),
	_textureTransform (nullptr),
	          _shader (nullptr),
glBindProgramPipeline ()
{
	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "fillProperties",   fillProperties ());
	addField (inputOutput, "lineProperties",   lineProperties ());
	addField (inputOutput, "material",         material ());
	addField (inputOutput, "texture",          texture ());
	addField (inputOutput, "textureTransform", textureTransform ());
	addField (inputOutput, "shaders",          shaders ());
}

X3DBaseNode*
Appearance::create (X3DExecutionContext* const executionContext) const
{
	return new Appearance (executionContext);
}

void
Appearance::initialize ()
{
	X3DAppearanceNode::initialize ();

	if (getBrowser () -> getRenderingProperties () -> hasExtension ("GL_ARB_separate_shader_objects"))
		glBindProgramPipeline = ::glBindProgramPipeline;
	else
		glBindProgramPipeline = [ ] (GLuint) { };

	lineProperties ()   .addInterest (this, &Appearance::set_lineProperties);
	fillProperties ()   .addInterest (this, &Appearance::set_fillProperties);
	material ()         .addInterest (this, &Appearance::set_material);
	texture ()          .addInterest (this, &Appearance::set_texture);
	textureTransform () .addInterest (this, &Appearance::set_textureTransform);
	shaders ()          .addInterest (this, &Appearance::set_shaders);

	set_lineProperties ();
	set_fillProperties ();
	set_material ();
	set_texture ();
	set_textureTransform ();
	set_shaders ();
}

bool
Appearance::isTransparent () const
{
	if (_material and _material -> isTransparent ())
		return true;

	if (_texture and _texture -> isTransparent ())
		return true;

	if (_shader)
		return true;

	return false;
}

void
Appearance::set_fillProperties ()
{
	_fillProperties = x3d_cast <FillProperties*> (fillProperties ());

	if (_fillProperties)
		return;

	_fillProperties = getBrowser () -> getBrowserOptions () -> fillProperties ();
}

void
Appearance::set_lineProperties ()
{
	_lineProperties = x3d_cast <LineProperties*> (lineProperties ());

	if (_lineProperties)
		return;

	_lineProperties = getBrowser () -> getBrowserOptions () -> lineProperties ();
}

void
Appearance::set_material ()
{
	_material = x3d_cast <X3DMaterialNode*> (material ());
}

void
Appearance::set_texture ()
{
	_texture = x3d_cast <X3DTextureNode*> (texture ());
}

void
Appearance::set_textureTransform ()
{
	_textureTransform = x3d_cast <X3DTextureTransformNode*> (textureTransform ());

	if (_textureTransform)
		return;

	_textureTransform = getBrowser () -> getBrowserOptions () -> textureTransform ();
}

void
Appearance::set_shaders ()
{
	for (const auto & shader : shaders ())
	{
		auto shaderNode = x3d_cast <X3DShaderNode*> (shader);

		if (shaderNode and shaderNode -> isValid ())
		{
			shaderNode -> isSelected () = true;

			_shader = shaderNode;

			return;
		}
	}

	_shader = nullptr;	
}

void
Appearance::draw ()
{
	// Material

	if (_material)
		_material -> draw ();

	else
	{
		glDisable (GL_LIGHTING);
		glColor4f (1, 1, 1, 1);
	}

	// Texture

	if (_texture)
		_texture -> draw ();

	// TextureTransform

	_textureTransform -> draw ();

	// Shader

	if (_shader)
		_shader -> draw ();

	else
	{
		glUseProgram (0);
		glBindProgramPipeline (0);
	}
}

} // X3D
} // titania
