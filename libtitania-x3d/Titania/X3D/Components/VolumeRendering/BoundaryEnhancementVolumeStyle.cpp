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

#include "BoundaryEnhancementVolumeStyle.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"

namespace titania {
namespace X3D {

const Component   BoundaryEnhancementVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string BoundaryEnhancementVolumeStyle::typeName       = "BoundaryEnhancementVolumeStyle";
const std::string BoundaryEnhancementVolumeStyle::containerField = "renderStyle";

BoundaryEnhancementVolumeStyle::Fields::Fields () :
	retainedOpacity (new SFFloat (0.2)),
	boundaryOpacity (new SFFloat (0.9)),
	  opacityFactor (new SFFloat (2))
{ }

BoundaryEnhancementVolumeStyle::BoundaryEnhancementVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields ()
{
	addType (X3DConstants::BoundaryEnhancementVolumeStyle);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "enabled",         enabled ());
	addField (inputOutput, "retainedOpacity", retainedOpacity ());
	addField (inputOutput, "boundaryOpacity", boundaryOpacity ());
	addField (inputOutput, "opacityFactor",   opacityFactor ());
}

X3DBaseNode*
BoundaryEnhancementVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new BoundaryEnhancementVolumeStyle (executionContext);
}

void
BoundaryEnhancementVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();
}

void
BoundaryEnhancementVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "retainedOpacity_" + getStyleId (), retainedOpacity () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "boundaryOpacity_" + getStyleId (), boundaryOpacity () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "opacityFactor_"   + getStyleId (), opacityFactor ()   .copy (CopyType::FLAT_COPY));
}

std::string
BoundaryEnhancementVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// BoundaryEnhancementVolumeStyle\n";
	string += "\n";
	string += "uniform float retainedOpacity_" + getStyleId () + ";\n";
	string += "uniform float boundaryOpacity_" + getStyleId () + ";\n";
	string += "uniform float opacityFactor_" + getStyleId () + ";\n";

	string += "\n";
	string += "float\n";
	string += "getBoundaryEnhancementStyle_" + getStyleId () + " (in float originalAlpha, in vec3 texCoord)\n";
	string += "{\n";
	string += "	float f0 = texture (x3d_Texture3D [0], texCoord) .r;\n";
	string += "	float f1 = texture (x3d_Texture3D [0], texCoord + vec3 (0.0, 0.0, 1.0 / x3d_TextureSize .z)) .r;\n";
	string += "	float f  = abs (f0 - f1);\n";
	string += "\n";
	string += "	float retainedOpacity = retainedOpacity_" + getStyleId () + ";\n";
	string += "	float boundaryOpacity = boundaryOpacity_" + getStyleId () + ";\n";
	string += "	float opacityFactor   = opacityFactor_" + getStyleId () + ";\n";
	string += "\n";
	string += "	return originalAlpha * (retainedOpacity + pow (boundaryOpacity * f, opacityFactor));\n";
	string += "}\n";

	return string;
}

std::string
BoundaryEnhancementVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// BoundaryEnhancementVolumeStyle\n";
	string += "\n";
	string += "	textureColor .a = getBoundaryEnhancementStyle_" + getStyleId () + " (textureColor .a, texCoord);\n";

	return string;
}

BoundaryEnhancementVolumeStyle::~BoundaryEnhancementVolumeStyle ()
{ }

} // X3D
} // titania
