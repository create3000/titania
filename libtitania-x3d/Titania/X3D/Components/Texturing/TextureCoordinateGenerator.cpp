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

#include "TextureCoordinateGenerator.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

namespace titania {
namespace X3D {

const Component   TextureCoordinateGenerator::component      = Component ("Texturing", 2);
const std::string TextureCoordinateGenerator::typeName       = "TextureCoordinateGenerator";
const std::string TextureCoordinateGenerator::containerField = "texCoord";

const size_t TextureCoordinateGenerator::MAX_PARAMETER = 6;

TextureCoordinateGenerator::Fields::Fields () :
	     mode (new SFString ("SPHERE")),
	parameter (new MFFloat ())
{ }

TextureCoordinateGenerator::TextureCoordinateGenerator (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureCoordinateNode (),
	                  fields (),
	                modeType (ModeType::SPHERE),
	          parameterArray ()
{
	addType (X3DConstants::TextureCoordinateGenerator);

	addField (inputOutput, "metadata",  metadata ());
	addField (inputOutput, "mode",      mode ());
	addField (inputOutput, "parameter", parameter ());
}

X3DBaseNode*
TextureCoordinateGenerator::create (X3DExecutionContext* const executionContext) const
{
	return new TextureCoordinateGenerator (executionContext);
}

void
TextureCoordinateGenerator::initialize ()
{
	X3DTextureCoordinateNode::initialize ();

	mode ()      .addInterest (&TextureCoordinateGenerator::set_mode,      this);
	parameter () .addInterest (&TextureCoordinateGenerator::set_parameter, this);

	set_mode ();
	set_parameter ();
}

void
TextureCoordinateGenerator::set_mode ()
{
	static const std::map <std::string, ModeType> modes = {
		std::pair ("SPHERE",                      ModeType::SPHERE),
		std::pair ("SPHERE-LOCAL",                ModeType::SPHERE_LOCAL),
		std::pair ("SPHERE-REFLECT",              ModeType::SPHERE_REFLECT),
		std::pair ("SPHERE-REFLECT-LOCAL",        ModeType::SPHERE_REFLECT_LOCAL),
		std::pair ("CAMERASPACENORMAL",           ModeType::CAMERASPACENORMAL),
		std::pair ("CAMERASPACEPOSITION",         ModeType::CAMERASPACEPOSITION),
		std::pair ("CAMERASPACEREFLECTIONVECTOR", ModeType::CAMERASPACEREFLECTIONVECTOR),
		std::pair ("COORD-EYE",                   ModeType::COORD_EYE),
		std::pair ("COORD",                       ModeType::COORD),
		std::pair ("NOISE-EYE",                   ModeType::NOISE_EYE),
		std::pair ("NOISE",                       ModeType::NOISE)
	};

	try
	{
		modeType = modes .at (mode ());
	}
	catch (const std::out_of_range &)
	{
		modeType = ModeType::SPHERE;
	}
}

void
TextureCoordinateGenerator::set_parameter ()
{
	parameterArray .assign (parameter () .begin (), parameter () .end ());
	parameterArray .resize (MAX_PARAMETER);
}

void
TextureCoordinateGenerator::init (MultiTexCoordArray & texCoords) const
{
	texCoords .emplace_back ();
}

void
TextureCoordinateGenerator::addTexCoord (const size_t i, MultiTexCoordArray & texCoords, const size_t index) const
{
	texCoords [i] .emplace_back (0, 0, 0, 1);
}

void
TextureCoordinateGenerator::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i) const
{
	glUniform1i  (shaderObject -> getTextureCoordinateGeneratorModeUniformLocation () [i], int (modeType));
	glUniform1fv (shaderObject -> getTextureCoordinateGeneratorParameterUniformLocation () [i], parameterArray .size (), parameterArray .data ());
}

} // X3D
} // titania
