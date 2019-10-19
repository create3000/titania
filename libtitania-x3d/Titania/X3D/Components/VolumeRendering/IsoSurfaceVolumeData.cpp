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

#include "IsoSurfaceVolumeData.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/FileLoader.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/ShaderPart.h"
#include "../Shape/Appearance.h"
#include "../Texturing3D/X3DTexture3DNode.h"
#include "../VolumeRendering/OpacityMapVolumeStyle.h"
#include "../VolumeRendering/X3DVolumeRenderStyleNode.h"
#include "../X_ITE/BlendMode.h"

#include <regex>

namespace titania {
namespace X3D {

const Component   IsoSurfaceVolumeData::component      = Component ("VolumeRendering", 2);
const std::string IsoSurfaceVolumeData::typeName       = "IsoSurfaceVolumeData";
const std::string IsoSurfaceVolumeData::containerField = "children";

IsoSurfaceVolumeData::Fields::Fields () :
	   surfaceValues (new MFFloat ()),
	 contourStepSize (new SFFloat (0)),
	surfaceTolerance (new SFFloat (0)),
	       gradients (new SFNode ()),
	     renderStyle (new MFNode ()),
	          voxels (new SFNode ())
{ }

IsoSurfaceVolumeData::IsoSurfaceVolumeData (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DVolumeDataNode (),
	           fields (),
	    gradientsNode (),
	 renderStyleNodes (),
	       voxelsNode (),
	    blendModeNode (new BlendMode (executionContext))
{
	addType (X3DConstants::IsoSurfaceVolumeData);

	addField (inputOutput,    "metadata",         metadata ());
	addField (inputOutput,    "dimensions",       dimensions ());
	addField (inputOutput,    "surfaceValues",    surfaceValues ());
	addField (inputOutput,    "contourStepSize",  contourStepSize ());
	addField (inputOutput,    "surfaceTolerance", surfaceTolerance ());
	addField (inputOutput,    "gradients",        gradients ());
	addField (inputOutput,    "renderStyle",      renderStyle ());
	addField (inputOutput,    "voxels",           voxels ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (initializeOnly, "bboxSize",         bboxSize ());

	addChildObjects (gradientsNode,
	                 renderStyleNodes,
	                 voxelsNode,
	                 blendModeNode);
}

X3DBaseNode*
IsoSurfaceVolumeData::create (X3DExecutionContext* const executionContext) const
{
	return new IsoSurfaceVolumeData (executionContext);
}

void
IsoSurfaceVolumeData::initialize ()
{
	X3DVolumeDataNode::initialize ();

	gradients ()   .addInterest (&IsoSurfaceVolumeData::set_gradients,   this);
	renderStyle () .addInterest (&IsoSurfaceVolumeData::set_renderStyle, this);
	voxels ()      .addInterest (&IsoSurfaceVolumeData::set_voxels,      this);
	voxels ()      .addInterest (getAppearance () -> texture ());

	surfaceValues ()    .addInterest (&IsoSurfaceVolumeData::update, this);
	contourStepSize ()  .addInterest (&IsoSurfaceVolumeData::update, this);
	surfaceTolerance () .addInterest (&IsoSurfaceVolumeData::update, this);
	renderStyle ()      .addInterest (&IsoSurfaceVolumeData::update, this);

	blendModeNode -> setup ();

	getAppearance () -> texture ()   = voxels ();
	getAppearance () -> blendMode () = blendModeNode;

	set_gradients ();
	set_renderStyle ();
	set_voxels ();

	update ();
}

void
IsoSurfaceVolumeData::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DVolumeDataNode::setExecutionContext (executionContext);

	blendModeNode -> setExecutionContext (executionContext);
}

void
IsoSurfaceVolumeData::set_gradients ()
{
	gradientsNode = x3d_cast <X3DTexture3DNode*> (gradients ());
}

void
IsoSurfaceVolumeData::set_renderStyle ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
	{
		renderStyleNode -> removeInterest (&IsoSurfaceVolumeData::update, this);
		renderStyleNode -> removeVolumeData (this);
	}

	renderStyleNodes .clear ();

	for (const auto & node : renderStyle ())
	{
		const auto renderStyleNode = x3d_cast <X3DVolumeRenderStyleNode*> (node);

		if (renderStyleNode)
			renderStyleNodes .emplace_back (renderStyleNode);
	}

	for (const auto & renderStyleNode : renderStyleNodes)
	{
		renderStyleNode -> addInterest (&IsoSurfaceVolumeData::update, this);
		renderStyleNode -> addVolumeData (this);
	}
}

void
IsoSurfaceVolumeData::set_voxels ()
{
	if (voxelsNode)
		voxelsNode -> removeInterest (&IsoSurfaceVolumeData::set_textureSize, this);

	voxelsNode = x3d_cast <X3DTexture3DNode*> (voxels ());

	if (voxelsNode)
	{
		voxelsNode -> addInterest (&IsoSurfaceVolumeData::set_textureSize, this);

		set_textureSize ();
	}
}

void
IsoSurfaceVolumeData::set_textureSize ()
{
	try
	{
		const auto textureSize = Vector3f (voxelsNode -> getWidth (),
		                                   voxelsNode -> getHeight (),
		                                   voxelsNode -> getDepth ());

		getShader () -> setField <SFVec3f> ("x3d_TextureSize", textureSize);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
IsoSurfaceVolumeData::update ()
{
	setShader (createShader ());
}

X3DPtr <ComposedShader>
IsoSurfaceVolumeData::createShader () const
{
	const auto & opacityMapVolumeStyle = getBrowser () -> getDefaultVolumeStyle ();
	std::string  styleUniforms         = opacityMapVolumeStyle -> getUniformsText ();
	std::string  styleFunctions        = opacityMapVolumeStyle -> getFunctionsText ();

	styleUniforms  += "\n";
	styleUniforms  += "uniform float surfaceValues [" + basic::to_string (surfaceValues () .size (), std::locale::classic ()) + "];\n";
	styleUniforms  += "uniform float surfaceTolerance;\n";

	for (const auto & renderStyleNode : renderStyleNodes)
		styleUniforms  += renderStyleNode -> getUniformsText ();

	styleFunctions += "\n";
	styleFunctions += "	// IsoSurfaceVolumeData\n";
	styleFunctions += "\n";

	if (gradientsNode)
	{
		styleUniforms += "\n";
		styleUniforms += "uniform sampler3D gradients;\n";

		styleFunctions += "	if (length (texture (gradients, texCoord) .rgb) < 0.1)\n";
		styleFunctions += "		return vec4 (0.0);";
	}

	styleFunctions += "\n";
	styleFunctions += "	float intensity = textureColor .r;\n";
	styleFunctions += "\n";

	if (surfaceValues () .size () == 1)
	{
		if (contourStepSize () == 0.0f)
		{
			styleFunctions += "	if (intensity > surfaceValues [0])\n";
			styleFunctions += "	{\n";
			styleFunctions += "		textureColor = vec4 (vec3 (surfaceValues [0]), 1.0);\n";

			if (renderStyleNodes .size ())
			{
				styleFunctions += renderStyleNodes [0] -> getFunctionsText ();
			}

			styleFunctions += "	}\n";
			styleFunctions += "	else\n";
			styleFunctions += "	{\n";
			styleFunctions += "		textureColor = vec4 (0.0);\n";
			styleFunctions += "	}\n";
			styleFunctions += "\n";
		}
		else
		{
			std::deque <float> calcSurfaceValues;

			for (float v = surfaceValues () [0] - contourStepSize (); v > 0; v -= contourStepSize ())
				calcSurfaceValues .emplace_front (v);

			calcSurfaceValues .emplace_back (surfaceValues () [0]);

			for (float v = surfaceValues () [0] + contourStepSize (); v <= 1; v += contourStepSize ())
				calcSurfaceValues .emplace_back (v);

			styleFunctions += "	if (false)\n";
			styleFunctions += "	{ }\n";

			for (const auto & surfaceValue : calcSurfaceValues)
			{
				styleFunctions += "	else if (intensity > " + basic::to_string (surfaceValue, std::locale::classic ()) + ")\n";
				styleFunctions += "	{\n";
				styleFunctions += "		textureColor = vec4 (vec3 (" + basic::to_string (surfaceValue, std::locale::classic ()) + "), 1.0);\n";

				if (renderStyleNodes .size ())
				{
					styleFunctions += renderStyleNodes [0] -> getFunctionsText ();
				}

				styleFunctions += "	}\n";
			}

			styleFunctions += "	else\n";
			styleFunctions += "	{\n";
			styleFunctions += "		textureColor = vec4 (0.0);\n";
			styleFunctions += "	}\n";
			styleFunctions += "\n";
		}
	}
	else
	{
		styleFunctions += "	if (false)\n";
		styleFunctions += "	{ }\n";

		for (size_t i = 0, size = surfaceValues () .size (); i < size; ++ i)
		{
			styleFunctions += "	else if (intensity > surfaceValues [" + basic::to_string (i, std::locale::classic ()) + "])\n";
			styleFunctions += "	{\n";
			styleFunctions += "		textureColor = vec4 (vec3 (surfaceValues [" + basic::to_string (i, std::locale::classic ()) + "]), 1.0);\n";

			if (renderStyleNodes .size ())
			{
				const size_t r = std::min (i, renderStyleNodes .size () - 1);

				styleFunctions += renderStyleNodes [r] -> getFunctionsText ();
			}

			styleFunctions += "	}\n";
		}

		styleFunctions += "	else\n";
		styleFunctions += "	{\n";
		styleFunctions += "		textureColor = vec4 (0.0);\n";
		styleFunctions += "	}\n";
		styleFunctions += "\n";
	}

	static const std::regex CLIP_PLANES             (R"/(#pragma X3D include "include/ClipPlanes.glsl"\n)/");
	static const std::regex FOG                     (R"/(#pragma X3D include "include/Fog.glsl"\n)/");
	static const std::regex VOLUME_STYLES_UNIFORMS  (R"/(// VOLUME_STYLES_UNIFORMS\n)/");
	static const std::regex VOLUME_STYLES_FUNCTIONS (R"/(// VOLUME_STYLES_FUNCTIONS\n)/");

	auto fl   = FileLoader (getExecutionContext ());
	auto clip = fl .loadDocument (get_shader ("Shaders/include/ClipPlanes.glsl"));
	auto fog  = fl .loadDocument (get_shader ("Shaders/include/Fog.glsl"));
	auto vs   = fl .loadDocument (get_shader ("VolumeRendering/VolumeStyle.vs"));
	auto fs   = fl .loadDocument (get_shader ("VolumeRendering/VolumeStyle.fs"));

	fs = std::regex_replace (fs, CLIP_PLANES,             clip);
	fs = std::regex_replace (fs, FOG,                     fog);
	fs = std::regex_replace (fs, VOLUME_STYLES_UNIFORMS,  styleUniforms);
	fs = std::regex_replace (fs, VOLUME_STYLES_FUNCTIONS, styleFunctions);

	const auto vertexPart   = getExecutionContext () -> createNode <ShaderPart> ();
	const auto fragmentPart = getExecutionContext () -> createNode <ShaderPart> ();
	const auto shaderNode   = getExecutionContext () -> createNode <ComposedShader> ();

	fragmentPart -> setName ("VolumeDataFragmentShaderPart");
	vertexPart   -> setName ("VolumeDataVertexShaderPart");
	shaderNode   -> setName ("VolumeDataComposedShader");

	fragmentPart -> type () = "FRAGMENT";
	vertexPart   -> url ()  = { "data:x-shader/x-vertex," + vs };
	fragmentPart -> url ()  = { "data:x-shader/x-fragment," + fs };

	shaderNode -> parts () .emplace_back (vertexPart);
	shaderNode -> parts () .emplace_back (fragmentPart);

	shaderNode -> addUserDefinedField (inputOutput, "surfaceValues",    surfaceValues ()    .copy (CopyType::FLAT_COPY));
	shaderNode -> addUserDefinedField (inputOutput, "surfaceTolerance", surfaceTolerance () .copy (CopyType::FLAT_COPY));

	if (gradientsNode)
		shaderNode -> addUserDefinedField (inputOutput, "grandients", new SFNode (gradientsNode));

	if (voxelsNode)
	{
		auto textureSize = new SFVec3f (voxelsNode -> getWidth (), voxelsNode -> getHeight (), voxelsNode -> getDepth ());

		shaderNode -> addUserDefinedField (inputOutput, "x3d_TextureSize", textureSize);
	}
	else
	{
		shaderNode -> addUserDefinedField (inputOutput, "x3d_TextureSize", new SFVec3f ());
	}

	opacityMapVolumeStyle -> addShaderFields (shaderNode);

	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> addShaderFields (shaderNode);

	__LOG__ << std::endl << fs << std::endl;

	return shaderNode;
}

void
IsoSurfaceVolumeData::shutdown ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> removeVolumeData (this);

	X3DVolumeDataNode::shutdown ();
}

IsoSurfaceVolumeData::~IsoSurfaceVolumeData ()
{ }

} // X3D
} // titania
