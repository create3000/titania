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

#include "ToneMappedVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Texturing3D/X3DTexture3DNode.h"

namespace titania {
namespace X3D {

const Component   ToneMappedVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string ToneMappedVolumeStyle::typeName       = "ToneMappedVolumeStyle";
const std::string ToneMappedVolumeStyle::containerField = "renderStyle";

ToneMappedVolumeStyle::Fields::Fields () :
	     coolColor (new SFColorRGBA (0, 0, 1, 0)),
	     warmColor (new SFColorRGBA (1, 1, 0, 0)),
	surfaceNormals (new SFNode ())
{ }

ToneMappedVolumeStyle::ToneMappedVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	                surfaceNormalsNode ()
{
	addType (X3DConstants::ToneMappedVolumeStyle);

	addField (inputOutput, "metadata",       metadata ());
	addField (inputOutput, "enabled",        enabled ());
	addField (inputOutput, "coolColor",      coolColor ());
	addField (inputOutput, "warmColor",      warmColor ());
	addField (inputOutput, "surfaceNormals", surfaceNormals ());

	addChildObjects (surfaceNormalsNode);
}

X3DBaseNode*
ToneMappedVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new ToneMappedVolumeStyle (executionContext);
}

void
ToneMappedVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	surfaceNormals () .addInterest (&ToneMappedVolumeStyle::set_surfaceNormals, this);

	set_surfaceNormals ();
}

void
ToneMappedVolumeStyle::set_surfaceNormals ()
{
	surfaceNormalsNode = x3d_cast <X3DTexture3DNode*> (surfaceNormals ());
}

void
ToneMappedVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "coolColor_" + getStyleId (), coolColor () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "warmColor_" + getStyleId (), warmColor () .copy (CopyType::FLAT_COPY));

	if (surfaceNormalsNode)
		shaderNode -> addUserDefinedField (inputOutput, "surfaceNormals_" + getStyleId (), new SFNode (surfaceNormalsNode));
}

std::string
ToneMappedVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// ToneMappedVolumeStyle\n";
	string += "\n";
	string += "uniform vec4 coolColor_" + getStyleId () + ";\n";
	string += "uniform vec4 warmColor_" + getStyleId () + ";\n";

	string += getNormalText (surfaceNormalsNode);

	string += "\n";
	string += "vec3\n";
	string += "getToneMappedStyle_" + getStyleId () + " (in vec4 coolColor, in vec4 warmColor, in vec4 surfaceNormal, in vec3 lightDir)\n";
	string += "{\n";
	string += "	float colorFactor = (1.0 + dot (lightDir, surfaceNormal .xyz)) * 0.5;\n";
	string += "	return mix (warmColor .rgb, coolColor .rgb, colorFactor);\n";
	string += "}\n";

	return string;
}

std::string
ToneMappedVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// ToneMappedVolumeStyle\n";
	string += "\n";
	string += "	{\n";

	string += "		vec4 surfaceNormal = getNormal_" + getStyleId () + " (texCoord);\n";
	string += "		vec3 toneColor     = vec3 (0.0);\n";
	string += "\n";
	string += "		if (surfaceNormal .w < surfaceTolerance)\n";
	string += "		{\n";
	string += "			textureColor = vec4 (0.0);\n";
	string += "		}\n";
	string += "		else\n";
	string += "		{\n";
	string += "			for (int i = 0; i < x3d_MaxLights; ++ i)\n";
	string += "			{\n";
	string += "				if (i == x3d_NumLights)\n";
	string += "					break;\n";
	string += "\n";
	string += "				x3d_LightSourceParameters light = x3d_LightSource [i];\n";
	string += "				vec3 L = light .type == x3d_DirectionalLight ? -light .direction : normalize (light .location - vertex);\n";
	string += "\n";
	string += "				toneColor += getToneMappedStyle_" + getStyleId () + " (coolColor_" + getStyleId () + ", warmColor_" + getStyleId () + ", surfaceNormal, L);\n";
	string += "			}\n";
	string += "\n";
	string += "			textureColor .rgb = toneColor;\n";
	string += "		}\n";

	string += "	}\n";

	return string;
}

ToneMappedVolumeStyle::~ToneMappedVolumeStyle ()
{ }

} // X3D
} // titania
