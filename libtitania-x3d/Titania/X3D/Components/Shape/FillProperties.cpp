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

#include "FillProperties.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DProgrammableShaderObject.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const Component   FillProperties::component      = Component ("Shape", 3);
const std::string FillProperties::typeName       = "FillProperties";
const std::string FillProperties::containerField = "fillProperties";

FillProperties::Fields::Fields () :
	    filled (new SFBool (true)),
	   hatched (new SFBool (true)),
	hatchColor (new SFColor (1, 1, 1)),
	hatchStyle (new SFInt32 (1))
{ }

FillProperties::FillProperties (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DAppearanceChildNode (),
	                fields (),
	           transparent ()
{
	addType (X3DConstants::FillProperties);

	addField (inputOutput, "metadata",   metadata ());
	addField (inputOutput, "filled",     filled ());
	addField (inputOutput, "hatched",    hatched ());
	addField (inputOutput, "hatchColor", hatchColor ());
	addField (inputOutput, "hatchStyle", hatchStyle ());

	addChildObjects (transparent);
}

X3DBaseNode*
FillProperties::create (X3DExecutionContext* const executionContext) const
{
	return new FillProperties (executionContext);
}

void
FillProperties::initialize ()
{
	X3DAppearanceChildNode::initialize ();

	filled () .addInterest (&FillProperties::set_transparent, this);

	set_transparent ();
}

void
FillProperties::set_transparent ()
{
	const auto value = not filled ();

	if (value not_eq transparent)
		transparent = value;
}

void
FillProperties::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject) const
{
	glUniform1i (shaderObject -> getFillPropertiesFilledUniformLocation (),  filled ());
	glUniform1i (shaderObject -> getFillPropertiesHatchedUniformLocation (), hatched ());

	if (hatched ())
	{
		const auto textureNode = getBrowser () -> getHatchStyle (hatchStyle ());

		glUniform3fv (shaderObject -> getFillPropertiesHatchColorUniformLocation (), 1, hatchColor () .getValue () .data ());

		glActiveTexture (GL_TEXTURE0 + getBrowser () -> getHatchStyleUnit ());
		glBindTexture (GL_TEXTURE_2D, textureNode -> getTextureId ());
	}
}

FillProperties::~FillProperties ()
{ }

} // X3D
} // titania
