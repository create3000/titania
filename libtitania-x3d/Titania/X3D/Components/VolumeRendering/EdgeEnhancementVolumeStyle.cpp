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

#include "EdgeEnhancementVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Texturing3D/X3DTexture3DNode.h"

namespace titania {
namespace X3D {

const Component   EdgeEnhancementVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string EdgeEnhancementVolumeStyle::typeName       = "EdgeEnhancementVolumeStyle";
const std::string EdgeEnhancementVolumeStyle::containerField = "renderStyle";

EdgeEnhancementVolumeStyle::Fields::Fields () :
	        edgeColor (new SFColorRGBA (0, 0, 0, 1)),
	gradientThreshold (new SFFloat (0.4)),
	   surfaceNormals (new SFNode ())
{ }

EdgeEnhancementVolumeStyle::EdgeEnhancementVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	                surfaceNormalsNode ()
{
	addType (X3DConstants::EdgeEnhancementVolumeStyle);

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOutput, "enabled",           enabled ());
	addField (inputOutput, "edgeColor",         edgeColor ());
	addField (inputOutput, "gradientThreshold", gradientThreshold ());
	addField (inputOutput, "surfaceNormals",    surfaceNormals ());

	addChildObjects (surfaceNormalsNode);
}

X3DBaseNode*
EdgeEnhancementVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new EdgeEnhancementVolumeStyle (executionContext);
}

void
EdgeEnhancementVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	surfaceNormals () .addInterest (&EdgeEnhancementVolumeStyle::set_surfaceNormals, this);

	set_surfaceNormals ();
}

void
EdgeEnhancementVolumeStyle::set_surfaceNormals ()
{
	surfaceNormalsNode = x3d_cast <X3DTexture3DNode*> (surfaceNormals ());
}

void
EdgeEnhancementVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "edgeColor_"         + getStyleId (), edgeColor ()         .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "gradientThreshold_" + getStyleId (), gradientThreshold () .copy (CopyType::FLAT_COPY));

	if (surfaceNormalsNode)
		shaderNode -> addUserDefinedField (inputOutput, "surfaceNormals_" + getStyleId (), new SFNode (surfaceNormalsNode));
}

std::string
EdgeEnhancementVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// EdgeEnhancementVolumeStyle\n";
	string += "\n";
	string += "uniform vec4  edgeColor_" + getStyleId () + ";\n";
	string += "uniform float gradientThreshold_" + getStyleId () + ";\n";

	string += getNormalText (surfaceNormalsNode);

	string += "\n";
	string += "vec4\n";
	string += "getEdgeEnhacementStyle_" + getStyleId () + " (in vec4 originalColor, in vec4 edgeColor, in float gradientThreshold, in vec4 surfaceNormal, in vec3 vertex)\n";
	string += "{\n";
	string += "	if (surfaceNormal .w < surfaceTolerance)\n";
	string += "		return originalColor;\n";
	string += "\n";
	string += "	float angle = abs (dot (surfaceNormal .xyz, normalize (vertex)));\n";
	string += "\n";
	string += "	if (angle >= cos (gradientThreshold))\n";
	string += "		return originalColor;\n";
	string += "	else\n";
	string += "		return vec4 (mix (edgeColor .rgb, originalColor.rgb, angle), originalColor .a);\n";
	string += "}\n";

	return string;
}

std::string
EdgeEnhancementVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// EdgeEnhancementVolumeStyle\n";
	string += "\n";
	string += "	{\n";

	string += "		vec4 surfaceNormal = getNormal_" + getStyleId () + " (texCoord);\n";
	string += "		vec4 edgeColor     = getEdgeEnhacementStyle_" + getStyleId () + " (textureColor, edgeColor_" + getStyleId () + ", gradientThreshold_" + getStyleId () + ", surfaceNormal, vertex);\n";
	string += "\n";
	string += "		textureColor = edgeColor;\n";

	string += "	}\n";

	return string;
}

EdgeEnhancementVolumeStyle::~EdgeEnhancementVolumeStyle ()
{ }

} // X3D
} // titania
