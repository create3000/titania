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

#include "BlendedVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Texturing/X3DTexture2DNode.h"
#include "../Texturing3D/X3DTexture3DNode.h"
#include "../VolumeRendering/OpacityMapVolumeStyle.h"
#include "../VolumeRendering/X3DVolumeDataNode.h"

#include <regex>

namespace titania {
namespace X3D {

const Component   BlendedVolumeStyle::component      = Component ("VolumeRendering", 3);
const std::string BlendedVolumeStyle::typeName       = "BlendedVolumeStyle";
const std::string BlendedVolumeStyle::containerField = "renderStyle";

const std::map <std::string, BlendedVolumeStyle::WeightFunctionsType> BlendedVolumeStyle::weightFunctions = {
	std::pair ("CONSTANT",         WeightFunctionsType::CONSTANT),
	std::pair ("ALPHA0",           WeightFunctionsType::ALPHA0),
	std::pair ("ALPHA1",           WeightFunctionsType::ALPHA1),
	std::pair ("ONE_MINUS_ALPHA0", WeightFunctionsType::ONE_MINUS_ALPHA0),
	std::pair ("ONE_MINUS_ALPHA1", WeightFunctionsType::ONE_MINUS_ALPHA1),
	std::pair ("TABLE",            WeightFunctionsType::TABLE),
};

BlendedVolumeStyle::Fields::Fields () :
	        weightConstant1 (new SFFloat (0.5)),
	        weightConstant2 (new SFFloat (0.5)),
	        weightFunction1 (new SFString ("CONSTANT")),
	        weightFunction2 (new SFString ("CONSTANT")),
	weightTransferFunction1 (new SFNode ()),
	weightTransferFunction2 (new SFNode ()),
	            renderStyle (new SFNode ()),
	                 voxels (new SFNode ())
{ }

BlendedVolumeStyle::BlendedVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	               weightFunction1Type (WeightFunctionsType::CONSTANT),
	               weightFunction2Type (WeightFunctionsType::CONSTANT),
	       weightTransferFunction1Node (),
	       weightTransferFunction2Node (),
	                   renderStyleNode (),
	                        voxelsNode ()
{
	addType (X3DConstants::BlendedVolumeStyle);

	addField (inputOutput, "metadata",                metadata ());
	addField (inputOutput, "enabled",                 enabled ());
	addField (inputOutput, "weightConstant1",         weightConstant1 ());
	addField (inputOutput, "weightConstant2",         weightConstant2 ());
	addField (inputOutput, "weightFunction1",         weightFunction1 ());
	addField (inputOutput, "weightFunction2",         weightFunction2 ());
	addField (inputOutput, "weightTransferFunction1", weightTransferFunction1 ());
	addField (inputOutput, "weightTransferFunction2", weightTransferFunction2 ());
	addField (inputOutput, "renderStyle",             renderStyle ());
	addField (inputOutput, "voxels",                  voxels ());

	addChildObjects (weightTransferFunction1Node,
	                 weightTransferFunction2Node,
	                 renderStyleNode,
	                 voxelsNode);
}

X3DBaseNode*
BlendedVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new BlendedVolumeStyle (executionContext);
}

void
BlendedVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	weightFunction1 ()         .addInterest (&BlendedVolumeStyle::set_weightFunction1,         this);
	weightFunction2 ()         .addInterest (&BlendedVolumeStyle::set_weightFunction2,         this);
	weightTransferFunction1 () .addInterest (&BlendedVolumeStyle::set_weightTransferFunction1, this);
	weightTransferFunction2 () .addInterest (&BlendedVolumeStyle::set_weightTransferFunction2, this);
	renderStyle ()             .addInterest (&BlendedVolumeStyle::set_renderStyle,             this);
	voxels ()                  .addInterest (&BlendedVolumeStyle::set_voxels,                  this);

	set_weightFunction1 ();
	set_weightFunction2 ();
	set_weightTransferFunction1 ();
	set_weightTransferFunction2 ();
	set_renderStyle ();
	set_voxels ();
}

void
BlendedVolumeStyle::set_weightFunction1 ()
{
	try
	{
		weightFunction1Type = weightFunctions .at (weightFunction1 ());
	}
	catch(const std::out_of_range & error)
	{
		weightFunction1Type = WeightFunctionsType::CONSTANT;
	}
}

void
BlendedVolumeStyle::set_weightFunction2 ()
{
	try
	{
		weightFunction2Type = weightFunctions .at (weightFunction2 ());
	}
	catch(const std::out_of_range & error)
	{
		weightFunction2Type = WeightFunctionsType::CONSTANT;
	}
}

void
BlendedVolumeStyle::set_weightTransferFunction1 ()
{
	weightTransferFunction1Node = x3d_cast <X3DTexture2DNode*> (weightTransferFunction1 ());
}

void
BlendedVolumeStyle::set_weightTransferFunction2 ()
{
	weightTransferFunction2Node = x3d_cast <X3DTexture2DNode*> (weightTransferFunction2 ());
}

void
BlendedVolumeStyle::set_renderStyle ()
{
	if (renderStyleNode)
	{
		renderStyleNode -> removeInterest (&BlendedVolumeStyle::addEvent, this);

		for (const auto & volumeDataNode : getVolumeData ())
			renderStyleNode -> removeVolumeData (volumeDataNode);
	}

	renderStyleNode = x3d_cast <X3DComposableVolumeRenderStyleNode*> (renderStyle ());

	if (renderStyleNode)
	{
		renderStyleNode -> addInterest (&BlendedVolumeStyle::addEvent, this);

		for (const auto & volumeDataNode : getVolumeData ())
			renderStyleNode -> addVolumeData (volumeDataNode);
	}
}

void
BlendedVolumeStyle::set_voxels ()
{
	if (voxelsNode)
		voxelsNode -> removeInterest (&BlendedVolumeStyle::set_textureSize, this);

	voxelsNode = x3d_cast <X3DTexture3DNode*> (voxels ());

	if (voxelsNode)
	{
		voxelsNode -> addInterest (&BlendedVolumeStyle::set_textureSize, this);

		set_textureSize ();
	}
}

void
BlendedVolumeStyle::set_textureSize ()
{
	for (const auto & volumeDataNode : getVolumeData ())
	{
		try
		{
			const auto textureSize = Vector3f (voxelsNode -> getWidth (),
			                                   voxelsNode -> getHeight (),
			                                   voxelsNode -> getDepth ());

			volumeDataNode -> getShader () -> setField <SFVec3f> ("textureSize_" + getStyleId (), textureSize);
		}
		catch (const X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
}

void
BlendedVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "weightConstant1_" + getStyleId (), weightConstant1 () .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "weightConstant2_" + getStyleId (), weightConstant2 () .copy (CopyType::FLAT_COPY));

	if (weightTransferFunction1Node)
		shaderNode -> addUserDefinedField (inputOutput, "weightTransferFunction1_" + getStyleId (), new SFNode (weightTransferFunction1Node));

	if (weightTransferFunction2Node)
		shaderNode -> addUserDefinedField (inputOutput, "weightTransferFunction2_" + getStyleId (), new SFNode (weightTransferFunction2Node));

	if (voxelsNode)
	{
		const auto textureSize = new SFVec3f (voxelsNode -> getWidth (), voxelsNode -> getHeight (), voxelsNode -> getDepth ());

		shaderNode -> addUserDefinedField (inputOutput, "voxels_"      + getStyleId (), new SFNode (voxelsNode));
		shaderNode -> addUserDefinedField (inputOutput, "textureSize_" + getStyleId (), textureSize);
	}
	else
	{
		shaderNode -> addUserDefinedField (inputOutput, "textureSize_" + getStyleId (), new SFVec3f ());
	}

	getBrowser () -> getDefaultBlendedVolumeStyle () -> addShaderFields (shaderNode);

	if (renderStyleNode)
		renderStyleNode -> addShaderFields (shaderNode);
}

std::string
BlendedVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	if (not voxelsNode)
		return "";

	std::string string;

	string += "\n";
	string += "// BlendedVolumeStyle\n";
	string += "\n";
	string += "uniform float     weightConstant1_" + getStyleId () + ";\n";
	string += "uniform float     weightConstant2_" + getStyleId () + ";\n";

	if (weightTransferFunction1Node)
		string += "uniform sampler2D weightTransferFunction1_" + getStyleId () + ";\n";

	if (weightTransferFunction2Node)
		string += "uniform sampler2D weightTransferFunction2_" + getStyleId () + ";\n";

	string += "uniform sampler3D voxels_"      + getStyleId () + ";\n";
	string += "uniform vec3      textureSize_" + getStyleId () + ";\n";

	auto uniformsText = getBrowser () -> getDefaultBlendedVolumeStyle () -> getUniformsText ();

	if (renderStyleNode)
		uniformsText += renderStyleNode -> getUniformsText ();

	static const std::regex x3d_Texture3D   (R"/(x3d_Texture3D \[0\])/");
	static const std::regex x3d_TextureSize (R"/(x3d_TextureSize)/");

	uniformsText = std::regex_replace (uniformsText, x3d_Texture3D,   "voxels_"      + getStyleId ());
	uniformsText = std::regex_replace (uniformsText, x3d_TextureSize, "textureSize_" + getStyleId ());

	string += "\n";
	string += uniformsText;

	string += "\n";
	string += "vec4\n";
	string += "getBlendedStyle_" + getStyleId () + " (in vec4 originalColor, in vec3 texCoord)\n";
	string += "{\n";
	string += "	vec4 blendColor_" + getStyleId () + " = texture (voxels_" + getStyleId () + ", texCoord);";

	auto functionsText = getBrowser () -> getDefaultBlendedVolumeStyle () -> getFunctionsText ();

	if (renderStyleNode)
		functionsText += renderStyleNode -> getFunctionsText ();

	static const std::regex textureColor (R"/(textureColor)/");

	functionsText = std::regex_replace (functionsText, textureColor, "blendColor_" + getStyleId ());

	string += "\n";
	string += functionsText;

	string += "\n";
	string += "	// BlendedVolumeStyle\n";
	string += "\n";

	switch (weightFunction1Type)
	{
		default: // CONSTANT
		{
			string += "	float w1_" + getStyleId () + " = weightConstant1_" + getStyleId () + ";\n";
			break;
		}
		case WeightFunctionsType::ALPHA0:
		{
			string += "	float w1_" + getStyleId () + " = originalColor .a;\n";
			break;
		}
		case WeightFunctionsType::ALPHA1:
		{
			string += "	float w1_" + getStyleId () + " = blendColor_ " + getStyleId () + " .a;\n";
			break;
		}
		case WeightFunctionsType::ONE_MINUS_ALPHA0:
		{
			string += "	float w1_" + getStyleId () + " = 1.0 - originalColor .a;\n";
			break;
		}
		case WeightFunctionsType::ONE_MINUS_ALPHA1:
		{
			string += "	float w1_" + getStyleId () + " = 1.0 - blendColor_ " + getStyleId () + " .a;\n";
			break;
		}
		case WeightFunctionsType::TABLE:
		{
			if (weightTransferFunction1Node)
			{
				string += "	float w1_" + getStyleId () + " = texture (weightTransferFunction1_" + getStyleId () + ", vec2 (originalColor .a, blendColor_" + getStyleId () + " .a)) .r;\n";
			}
			else
			{
				// Use default CONSTANT value.
				string += "	float w1_" + getStyleId () + " = weightConstant1_" + getStyleId () + ";\n";
			}

			break;
		}
	}

	switch (weightFunction2Type)
	{
		default: // CONSTANT
		{
			string += "	float w2_" + getStyleId () + " = weightConstant2_" + getStyleId () + ";\n";
			break;
		}
		case WeightFunctionsType::ALPHA0:
		{
			string += "	float w2_" + getStyleId () + " = originalColor .a;\n";
			break;
		}
		case WeightFunctionsType::ALPHA1:
		{
			string += "	float w2_" + getStyleId () + " = blendColor_ " + getStyleId () + " .a;\n";
			break;
		}
		case WeightFunctionsType::ONE_MINUS_ALPHA0:
		{
			string += "	float w2_" + getStyleId () + " = 1.0 - originalColor .a;\n";
			break;
		}
		case WeightFunctionsType::ONE_MINUS_ALPHA1:
		{
			string += "	float w2_" + getStyleId () + " = 1.0 - blendColor_ " + getStyleId () + " .a;\n";
			break;
		}
		case WeightFunctionsType::TABLE:
		{
			if (weightTransferFunction2Node)
			{
				string += "	float w2_" + getStyleId () + " = texture (weightTransferFunction2_" + getStyleId () + ", vec2 (originalColor .a, blendColor_" + getStyleId () + " .a)) .r;\n";
			}
			else
			{
				// Use default CONSTANT value.
				string += "	float w2_" + getStyleId () + " = weightConstant2_" + getStyleId () + ";\n";
			}

			break;
		}
	}

	string += "\n";
	string += "	return clamp (originalColor * w1_" + getStyleId () + " + blendColor_" + getStyleId () + " * w2_" + getStyleId () + ", 0.0, 1.0);\n";
	string += "}\n";

	return string;
}

std::string
BlendedVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	if (not voxelsNode)
		return "";

	std::string string;

	string += "\n";
	string += "	// BlendedVolumeStyle\n";
	string += "\n";
	string += "	textureColor = getBlendedStyle_" + getStyleId () + " (textureColor, texCoord);\n";

	return string;
}

BlendedVolumeStyle::~BlendedVolumeStyle ()
{ }

} // X3D
} // titania
