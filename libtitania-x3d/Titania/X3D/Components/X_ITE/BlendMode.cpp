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

#include "BlendMode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const std::string BlendMode::componentName  = "X_ITE";
const std::string BlendMode::typeName       = "BlendMode";
const std::string BlendMode::containerField = "blendMode";

const std::map <std::string, GLenum> BlendMode::blendTypes = {
	// WebGL
   std::pair ("ZERO",                     GL_ZERO ),
   std::pair ("ONE",                      GL_ONE ),
   std::pair ("SRC_COLOR",                GL_SRC_COLOR ),
   std::pair ("ONE_MINUS_SRC_COLOR",      GL_ONE_MINUS_SRC_COLOR ),
   std::pair ("DST_COLOR",                GL_DST_COLOR ),
   std::pair ("ONE_MINUS_DST_COLOR",      GL_ONE_MINUS_DST_COLOR ),
   std::pair ("SRC_ALPHA",                GL_SRC_ALPHA ),
   std::pair ("ONE_MINUS_SRC_ALPHA",      GL_ONE_MINUS_SRC_ALPHA ),
   std::pair ("DST_ALPHA",                GL_DST_ALPHA ),
   std::pair ("ONE_MINUS_DST_ALPHA",      GL_ONE_MINUS_DST_ALPHA ),
   std::pair ("SRC_ALPHA_SATURATE",       GL_SRC_ALPHA_SATURATE ),
   std::pair ("CONSTANT_COLOR",           GL_CONSTANT_COLOR  ),
   std::pair ("ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR ),
   std::pair ("CONSTANT_ALPHA",           GL_CONSTANT_ALPHA ),
   std::pair ("ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA ),
	// OpenGL
   // std::pair ("SRC1_COLOR",               GL_SRC1_COLOR ),
   // std::pair ("SRC1_ALPHA",               GL_SRC1_ALPHA ),
};

const std::map <std::string, GLenum> BlendMode::blendModes = {
   std::pair ("FUNC_ADD",              GL_FUNC_ADD ),
   std::pair ("FUNC_SUBTRACT",         GL_FUNC_SUBTRACT ),
   std::pair ("FUNC_REVERSE_SUBTRACT", GL_FUNC_REVERSE_SUBTRACT ),
   std::pair ("MIN",                   GL_MIN ),
   std::pair ("MAX",                   GL_MAX ),
};

BlendMode::Fields::Fields () :
	            blendColor (new SFColorRGBA ()),
	     sourceColorFactor (new SFString ("SRC_ALPHA")),
	     sourceAlphaFactor (new SFString ("ONE_MINUS_SRC_ALPHA")),
	destinationColorFactor (new SFString ("ONE")),
	destinationAlphaFactor (new SFString ("ONE_MINUS_SRC_ALPHA")),
	         colorEquation (new SFString ("FUNC_ADD")),
	         alphaEquation (new SFString ("FUNC_ADD"))
{ }

BlendMode::BlendMode (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DAppearanceChildNode (),
	     sourceColorFactorType (GL_SRC_ALPHA),
	     sourceAlphaFactorType (GL_ONE_MINUS_SRC_ALPHA),
	destinationColorFactorType (GL_ONE),
	destinationAlphaFactorType (GL_ONE_MINUS_SRC_ALPHA),
	         colorEquationType (GL_FUNC_ADD),
	         alphaEquationType (GL_FUNC_ADD)
{
	addType (X3DConstants::BlendMode);

	addField (inputOutput,    "metadata",                metadata ());

	addField (inputOutput,    "blendColor",              blendColor ());
	addField (inputOutput,    "sourceColorFactor",       sourceColorFactor ());
	addField (inputOutput,    "sourceAlphaFactor",       sourceAlphaFactor ());
	addField (inputOutput,    "destinationColorFactor" , destinationColorFactor ());
	addField (inputOutput,    "destinationAlphaFactor",  destinationAlphaFactor ());
	addField (inputOutput,    "colorEquation",           colorEquation ());
	addField (inputOutput,    "alphaEquation",           alphaEquation ());
}

X3DBaseNode*
BlendMode::create (X3DExecutionContext* const executionContext) const
{
	return new BlendMode (executionContext);
}

void
BlendMode::initialize ()
{
	X3DAppearanceChildNode::initialize ();

	sourceColorFactor ()      .addInterest (&BlendMode::set_sourceColorFactor,      this);
	sourceAlphaFactor ()      .addInterest (&BlendMode::set_sourceAlphaFactor,      this);
	destinationColorFactor () .addInterest (&BlendMode::set_destinationColorFactor, this);
	destinationAlphaFactor () .addInterest (&BlendMode::set_destinationAlphaFactor, this);
	colorEquation ()          .addInterest (&BlendMode::set_colorEquation,        this);
	alphaEquation ()          .addInterest (&BlendMode::set_alphaEquation,        this);

	set_sourceColorFactor ();
	set_sourceAlphaFactor ();
	set_destinationColorFactor ();
	set_destinationAlphaFactor ();
	set_colorEquation ();
	set_alphaEquation ();
}

void
BlendMode::set_sourceColorFactor ()
{
	try
	{
		sourceColorFactorType = blendTypes .at (sourceColorFactor ());
	}
	catch (const X3DError &)
	{
		sourceColorFactorType = GL_SRC_ALPHA;
	}
}

void
BlendMode::set_sourceAlphaFactor ()
{
	try
	{
		sourceAlphaFactorType = blendTypes .at (sourceAlphaFactor ());
	}
	catch (const X3DError &)
	{
		sourceAlphaFactorType = GL_ONE_MINUS_SRC_ALPHA;
	}
}

void
BlendMode::set_destinationColorFactor ()
{
	try
	{
		destinationColorFactorType = blendTypes .at (destinationColorFactor ());
	}
	catch (const X3DError &)
	{
		destinationColorFactorType = GL_ONE;
	}
}

void
BlendMode::set_destinationAlphaFactor ()
{
	try
	{
		destinationAlphaFactorType = blendTypes .at (destinationAlphaFactor ());
	}
	catch (const X3DError &)
	{
		destinationAlphaFactorType = GL_ONE_MINUS_SRC_ALPHA;
	}
}

void
BlendMode::set_colorEquation ()
{
	try
	{
		colorEquationType = blendModes .at (colorEquation ());
	}
	catch (const X3DError &)
	{
		colorEquationType = GL_FUNC_ADD;
	}
}

void
BlendMode::set_alphaEquation ()
{
	try
	{
		alphaEquationType = blendModes .at (alphaEquation ());
	}
	catch (const X3DError &)
	{
		alphaEquationType = GL_FUNC_ADD;
	}
}

void
BlendMode::enable ()
{
	const auto & c = blendColor () .getValue ();

	glBlendColor (c .r (), c .g (), c .b (), c .a ());
	glBlendFuncSeparate (sourceColorFactorType, sourceAlphaFactorType, destinationColorFactorType, destinationAlphaFactorType);
	glBlendEquationSeparate (colorEquationType, alphaEquationType);
}

void
BlendMode::disable ()
{
	glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate (GL_FUNC_ADD, GL_FUNC_ADD);
}


} // X3D
} // titania
