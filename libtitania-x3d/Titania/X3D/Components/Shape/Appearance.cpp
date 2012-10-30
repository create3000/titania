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
 ******************************************************************************/

#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DShaderNode.h"
#include "../Shape/FillProperties.h"
#include "../Shape/LineProperties.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Texturing/X3DTextureNode.h"
#include "../Texturing/X3DTextureTransformNode.h"
#include "Appearance.h"

namespace titania {
namespace X3D {

Appearance::Appearance (X3DExecutionContext* const executionContext) :
	     X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DAppearanceNode (),                                                    
	   fillProperties (),                                                    // SFNode [in,out] fillProperties    NULL        [FillProperties]
	   lineProperties (),                                                    // SFNode [in,out] lineProperties    NULL        [LineProperties]
	         material (),                                                    // SFNode [in,out] material          NULL        [X3DMaterialNode]
	          shaders (),                                                    // MFNode[in,out] shaders           [ ]         [X3DShaderNode]
	          texture (),                                                    // SFNode [in,out] texture           NULL        [X3DTextureNode]
	 textureTransform (),                                                    // SFNode [in,out] textureTransform  NULL        [X3DTextureTransformNode]
	  _fillProperties (nullptr),                                             
	  _lineProperties (nullptr),                                             
	        _material (nullptr),                                             
	         _texture (nullptr),                                             
	_textureTransform (nullptr)                                              
{
	setComponent ("Shape");
	setTypeName ("Appearance");

	appendField (inputOutput, "metadata",         metadata);
	appendField (inputOutput, "fillProperties",   fillProperties);
	appendField (inputOutput, "lineProperties",   lineProperties);
	appendField (inputOutput, "material",         material);
	appendField (inputOutput, "texture",          texture);
	appendField (inputOutput, "textureTransform", textureTransform);
	appendField (inputOutput, "shaders",          shaders);
}

X3DBasicNode*
Appearance::create (X3DExecutionContext* const executionContext) const
{
	return new Appearance (executionContext);
}

void
Appearance::initialize ()
{
	X3DAppearanceNode::initialize ();

	fillProperties   .addInterest (this, &Appearance::set_fillProperties);
	lineProperties   .addInterest (this, &Appearance::set_lineProperties);
	material         .addInterest (this, &Appearance::set_material);
	texture          .addInterest (this, &Appearance::set_texture);
	textureTransform .addInterest (this, &Appearance::set_textureTransform);
	shaders          .addInterest (this, &Appearance::set_shaders);

	set_fillProperties   ();
	set_lineProperties   ();
	set_material         ();
	set_texture          ();
	set_textureTransform ();
	set_shaders          ();
}

void
Appearance::set_fillProperties ()
{
	_fillProperties = *SFNode <FillProperties> (fillProperties);
}

void
Appearance::set_lineProperties ()
{
	_lineProperties = *SFNode <LineProperties> (lineProperties);
}

void
Appearance::set_material ()
{
	_material = *SFNode <X3DMaterialNode> (material);
}

void
Appearance::set_texture ()
{
	_texture = *SFNode <X3DTextureNode> (texture);
}

void
Appearance::set_textureTransform ()
{
	_textureTransform = *SFNode <X3DTextureTransformNode> (textureTransform);
}

void
Appearance::set_shaders ()
{
	_shaders .clear ();

	for (const auto & shader : shaders)
	{
		SFNode <X3DShaderNode> _shader = shader;

		if (_shader)
			_shaders .push_back (*_shader);
	}
}

bool
Appearance::isTransparent ()
{
	if (_material and _material -> isTransparent ())
		return true;

	if (_texture and _texture -> isTransparent ())
		return true;

	return false;
}

void
Appearance::draw ()
{
	if (_fillProperties)
		_fillProperties -> display ();

	if (_lineProperties)
		_lineProperties -> display ();

	if (_material)
		_material -> display ();

	if (_texture)
		_texture -> display ();

	if (_textureTransform)
		_textureTransform -> display ();

	for (const auto & _shader : _shaders)
	{
		_shader -> display ();
	}
}

} // X3D
} // titania
