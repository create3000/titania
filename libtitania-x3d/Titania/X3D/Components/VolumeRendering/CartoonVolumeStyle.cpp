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

#include "CartoonVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Texturing3D/X3DTexture3DNode.h"

namespace titania {
namespace X3D {

const Component   CartoonVolumeStyle::component      = Component ("VolumeRendering", 3);
const std::string CartoonVolumeStyle::typeName       = "CartoonVolumeStyle";
const std::string CartoonVolumeStyle::containerField = "renderStyle";

CartoonVolumeStyle::Fields::Fields () :
	     colorSteps (new SFInt32 (4)),
	orthogonalColor (new SFColorRGBA (1, 1, 1, 1)),
	  parallelColor (new SFColorRGBA (0, 0, 0, 1)),
	 surfaceNormals (new SFNode ())
{ }

CartoonVolumeStyle::CartoonVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	                surfaceNormalsNode ()
{
	addType (X3DConstants::CartoonVolumeStyle);

	addField (inputOutput, "metadata",        metadata ());
	addField (inputOutput, "enabled",         enabled ());
	addField (inputOutput, "colorSteps",      colorSteps ());
	addField (inputOutput, "orthogonalColor", orthogonalColor ());
	addField (inputOutput, "parallelColor",   parallelColor ());
	addField (inputOutput, "surfaceNormals",  surfaceNormals ());

	addChildObjects (surfaceNormalsNode);
}

X3DBaseNode*
CartoonVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new CartoonVolumeStyle (executionContext);
}

void
CartoonVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	surfaceNormals () .addInterest (&CartoonVolumeStyle::set_surfaceNormals, this);

	set_surfaceNormals ();
}

void
CartoonVolumeStyle::set_surfaceNormals ()
{
	surfaceNormalsNode = x3d_cast <X3DTexture3DNode*> (surfaceNormals ());
}

void
CartoonVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "colorSteps_"      + getStyleId (), colorSteps () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "orthogonalColor_" + getStyleId (), orthogonalColor () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "parallelColor_"   + getStyleId (), parallelColor () .copy (CopyType::FLAT_COPY));

	if (surfaceNormalsNode)
		shaderNode -> addUserDefinedField (inputOutput, "surfaceNormals_" + getStyleId (), new SFNode (surfaceNormalsNode));
}

std::string
CartoonVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// CartoonVolumeStyle\n";
	string += "\n";
	string += "uniform int  colorSteps_" + getStyleId () + ";\n";
	string += "uniform vec4 orthogonalColor_" + getStyleId () + ";\n";
	string += "uniform vec4 parallelColor_" + getStyleId () + ";\n";

	string += getNormalText (surfaceNormalsNode);

	string += "\n";
	string += "vec3\n";
	string += "rgb2hsv_" + getStyleId () + " (in vec3 color)\n";
	string += "{\n";
	string += "	float h = 0.0;\n";
	string += "	float s = 0.0;\n";
	string += "	float v = 0.0;\n";
	string += "\n";
	string += "	float min = min (min (color .r, color .g), color .b);\n";
	string += "	float max = max (max (color .r, color .g), color .b);\n";
	string += "	v = max; // value\n";
	string += "\n";
	string += "	float delta = max - min;\n";
	string += "\n";
	string += "	if (max != 0.0 && delta != 0.0)\n";
	string += "	{\n";
	string += "		s = delta / max; // s\n";
	string += "\n";
	string += "		if (color .r == max)\n";
	string += "			h =     (color .g - color .b) / delta;  // between yellow & magenta\n";
	string += "		else if (color .g == max)\n";
	string += "			h = 2.0 + (color .b - color .r) / delta;  // between cyan & yellow\n";
	string += "		else\n";
	string += "			h = 4.0 + (color .r - color .g) / delta;  // between magenta & cyan\n";
	string += "\n";
	string += "		h *= M_PI / 3.0;  // radiants\n";
	string += "		if (h < 0.0)\n";
	string += "			h += M_PI * 2.0;\n";
	string += "	}\n";
	string += "	else\n";
	string += "		s = h = 0.0;         // s = 0, h is undefined\n";
	string += "\n";
	string += "	return vec3 (h, s, v);\n";
	string += "}\n";

	string += "\n";
	string += "vec3\n";
	string += "hsv2rgb_" + getStyleId () + " (in vec3 hsv)\n";
	string += "{\n";
	string += "	float h = hsv [0];\n";
	string += "	float s = clamp (hsv [1], 0.0, 1.0);\n";
	string += "	float v = clamp (hsv [2], 0.0, 1.0);\n";
	string += "\n";
	string += "	// H is given on [0, 2 * Pi]. S and V are given on [0, 1].\n";
	string += "	// RGB are each returned on [0, 1].\n";
	string += "\n";
	string += "	if (s == 0.0)\n";
	string += "	{\n";
	string += "		// achromatic (grey)\n";
	string += "		return vec3 (v, v, v);\n";
	string += "	}\n";
	string += "	else\n";
	string += "	{\n";
	string += "		float w = (h * (180.0 / M_PI)) / 60.0;     // sector 0 to 5\n";
	string += "\n";
	string += "		float i = floor (w);\n";
	string += "		float f = w - i;                      // factorial part of h\n";
	string += "		float p = v * ( 1.0 - s );\n";
	string += "		float q = v * ( 1.0 - s * f );\n";
	string += "		float t = v * ( 1.0 - s * ( 1.0 - f ) );\n";
	string += "\n";
	string += "		switch (int (i) % 6)\n";
	string += "		{\n";
	string += "			case 0:  return vec3 (v, t, p);\n";
	string += "			case 1:  return vec3 (q, v, p);\n";
	string += "			case 2:  return vec3 (p, v, t);\n";
	string += "			case 3:  return vec3 (p, q, v);\n";
	string += "			case 4:  return vec3 (t, p, v);\n";
	string += "			default: return vec3 (v, p, q);\n";
	string += "		}\n";
	string += "	}\n";
	string += "\n";
	string += "	return vec3 (0.0);\n";
	string += "}\n";

	string += "\n";
	string += "vec3\n";
	string += "mix_hsv_" + getStyleId () + " (in vec3 a, in vec3 b, in float t)\n";
	string += "{\n";
	string += "	// Linearely interpolate in HSV space between source color @a a and destination color @a b by an amount of @a t.\n";
	string += "	// Source and destination color must be in HSV space.\n";
	string += "\n";
	string += "	float ha = a [0];\n";
	string += "	float sa = a [1];\n";
	string += "	float va = a [2];\n";
	string += "\n";
	string += "	float hb = b [0];\n";
	string += "	float sb = b [1];\n";
	string += "	float vb = b [2];\n";
	string += "\n";
	string += "	if (sa == 0.0)\n";
	string += "		ha = hb;\n";
	string += "\n";
	string += "	if (sb == 0.0)\n";
	string += "		hb = ha;\n";
	string += "\n";
	string += "	float range = abs (hb - ha);\n";
	string += "\n";
	string += "	if (range <= M_PI)\n";
	string += "	{\n";
	string += "		float h = ha + t * (hb - ha);\n";
	string += "		float s = sa + t * (sb - sa);\n";
	string += "		float v = va + t * (vb - va);\n";
	string += "		return vec3 (h, s, v);\n";
	string += "	}\n";
	string += "\n";
	string += "	float PI2  = M_PI * 2.0;\n";
	string += "	float step = (PI2 - range) * t;\n";
	string += "	float h    = ha < hb ? ha - step : ha + step;\n";
	string += "\n";
	string += "	if (h < 0.0)\n";
	string += "		h += PI2;\n";
	string += "\n";
	string += "	else if (h > PI2)\n";
	string += "		h -= PI2;\n";
	string += "\n";
	string += "	float s = sa + t * (sb - sa);\n";
	string += "	float v = va + t * (vb - va);\n";
	string += "	return vec3 (h, s, v);\n";
	string += "}\n";

	string += "\n";
	string += "vec4\n";
	string += "getCartoonStyle_" + getStyleId () + " (in vec4 originalColor, in vec3 texCoord)\n";
	string += "{\n";
	string += "	vec4 surfaceNormal = getNormal_" + getStyleId () + " (texCoord);\n";
	string += "\n";
	string += "	if (surfaceNormal .w < normalTolerance)\n";
	string += "		return vec4 (0.0);\n";
	string += "\n";
	string += "	vec4 orthogonalColor = orthogonalColor_" + getStyleId () + ";\n";
	string += "	vec4 parallelColor   = parallelColor_" + getStyleId () + ";\n";
	string += "	int  colorSteps      = colorSteps_" + getStyleId () + ";\n";
	string += "\n";
	string += "	float steps    = clamp (float (colorSteps), 1.0, 64.0);\n";
	string += "	float step     = M_PI / 2.0 / steps;\n";
	string += "	float cosTheta = min (dot (surfaceNormal .xyz, normalize (vertex)), 1.0);\n";
	string += "\n";
	string += "	if (cosTheta < 0.0)\n";
	string += "		return vec4 (0.0);\n";
	string += "\n";
	string += "	float t             = cos (min (floor (acos (cosTheta) / step) * (steps > 1.0 ? steps / (steps - 1.0) : 1.0), steps) * step);\n";
	string += "	vec3  orthogonalHSV = rgb2hsv_" + getStyleId () + " (orthogonalColor .rgb);\n";
	string += "	vec3  parallelHSV   = rgb2hsv_" + getStyleId () + " (parallelColor .rgb);\n";
	string += "\n";
	string += "	return vec4 (hsv2rgb_" + getStyleId () + " (mix_hsv_" + getStyleId () + " (orthogonalHSV, parallelHSV, t)), originalColor .a);\n";
	string += "}\n";

	return string;
}

std::string
CartoonVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// CartoonVolumeStyle\n";
	string += "\n";
	string += "	textureColor = getCartoonStyle_" + getStyleId () + " (textureColor, texCoord);\n";

	return string;
}

CartoonVolumeStyle::~CartoonVolumeStyle ()
{ }

} // X3D
} // titania
