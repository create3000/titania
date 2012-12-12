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
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DAppearanceNode (),                                                    
	   lineProperties (),                                                    // SFNode [in,out] lineProperties    NULL        [LineProperties]
	   fillProperties (),                                                    // SFNode [in,out] fillProperties    NULL        [FillProperties]
	         material (),                                                    // SFNode [in,out] material          NULL        [X3DMaterialNode]
	          texture (),                                                    // SFNode [in,out] texture           NULL        [X3DTextureNode]
	 textureTransform (),                                                    // SFNode [in,out] textureTransform  NULL        [X3DTextureTransformNode]
	          shaders ()                                                     // MFNode[in,out]  shaders           [ ]         [X3DShaderNode]
{
	setComponent ("Shape");
	setTypeName ("Appearance");

	addField (inputOutput, "metadata",         metadata);
	addField (inputOutput, "fillProperties",   fillProperties);
	addField (inputOutput, "lineProperties",   lineProperties);
	addField (inputOutput, "material",         material);
	addField (inputOutput, "texture",          texture);
	addField (inputOutput, "textureTransform", textureTransform);
	addField (inputOutput, "shaders",          shaders);
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
}

bool
Appearance::isTransparent ()
{
	if (material and material -> isTransparent ())
		return true;

	if (texture and texture -> isTransparent ())
		return true;

	return false;
}

void
Appearance::display ()
{
	if (lineProperties)
		lineProperties -> display ();

	if (fillProperties)
		fillProperties -> display ();

	if (material)
		material -> display ();

	if (texture)
		texture -> display ();

	if (textureTransform)
		textureTransform -> display ();

	for (const auto & shader : shaders)
		shader -> display ();
}

} // X3D
} // titania
