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

#include "SegmentedVolumeData.h"

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

const Component   SegmentedVolumeData::component      = Component ("VolumeRendering", 2);
const std::string SegmentedVolumeData::typeName       = "SegmentedVolumeData";
const std::string SegmentedVolumeData::containerField = "children";

SegmentedVolumeData::Fields::Fields () :
	    segmentEnabled (new MFBool ()),
	segmentIdentifiers (new SFNode ()),
	       renderStyle (new MFNode ()),
	            voxels (new SFNode ())
{ }

SegmentedVolumeData::SegmentedVolumeData (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      X3DVolumeDataNode (),
	                 fields (),
	 segmentIdentifiersNode (),
	       renderStyleNodes (),
	             voxelsNode (),
	          blendModeNode (new BlendMode (executionContext))
{
	addType (X3DConstants::SegmentedVolumeData);

	addField (inputOutput,    "metadata",           metadata ());
	addField (inputOutput,    "dimensions",         dimensions ());
	addField (inputOutput,    "segmentEnabled",     segmentEnabled ());
	addField (inputOutput,    "segmentIdentifiers", segmentIdentifiers ());
	addField (inputOutput,    "renderStyle",        renderStyle ());
	addField (inputOutput,    "voxels",             voxels ());
	addField (initializeOnly, "bboxCenter",         bboxCenter ());
	addField (initializeOnly, "bboxSize",           bboxSize ());

	addChildObjects (segmentIdentifiersNode,
	                 renderStyleNodes,
	                 voxelsNode,
	                 blendModeNode);
}

X3DBaseNode*
SegmentedVolumeData::create (X3DExecutionContext* const executionContext) const
{
	return new SegmentedVolumeData (executionContext);
}

void
SegmentedVolumeData::initialize ()
{
	X3DVolumeDataNode::initialize ();

	segmentIdentifiers () .addInterest (&SegmentedVolumeData::set_segmentIdentifiers, this);
	renderStyle ()        .addInterest (&SegmentedVolumeData::set_renderStyle,        this);
	voxels ()             .addInterest (&SegmentedVolumeData::set_voxels,             this);
	voxels ()             .addInterest (getAppearance () -> texture ());

	segmentEnabled ()     .addInterest (&SegmentedVolumeData::update,                 this);
	segmentIdentifiers () .addInterest (&SegmentedVolumeData::update,                 this);
	renderStyle ()        .addInterest (&SegmentedVolumeData::update,                 this);

	blendModeNode -> setup ();

	getAppearance () -> texture ()   = voxels ();
	getAppearance () -> blendMode () = blendModeNode;

	set_segmentIdentifiers ();
	set_renderStyle ();
	set_voxels ();

	update ();
}

void
SegmentedVolumeData::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DVolumeDataNode::setExecutionContext (executionContext);

	blendModeNode -> setExecutionContext (executionContext);
}

bool
SegmentedVolumeData::getSegmentEnabled (const size_t index) const
{
	return index < segmentEnabled () .size () ? segmentEnabled () [index] : true;
}

void
SegmentedVolumeData::set_renderStyle ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
	{
		renderStyleNode -> removeInterest (&SegmentedVolumeData::update, this);
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
		renderStyleNode -> addInterest (&SegmentedVolumeData::update, this);
		renderStyleNode -> addVolumeData (this);
	}
}

void
SegmentedVolumeData::set_segmentIdentifiers ()
{
	segmentIdentifiersNode = x3d_cast <X3DTexture3DNode*> (segmentIdentifiers ());
}

void
SegmentedVolumeData::set_voxels ()
{
	if (voxelsNode)
		voxelsNode -> removeInterest (&SegmentedVolumeData::set_textureSize, this);

	voxelsNode = x3d_cast <X3DTexture3DNode*> (voxels ());

	if (voxelsNode)
	{
		voxelsNode -> addInterest (&SegmentedVolumeData::set_textureSize, this);

		set_textureSize ();
	}
}

void
SegmentedVolumeData::set_textureSize ()
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
SegmentedVolumeData::update ()
{
	setShader (createShader ());
}

X3DPtr <ComposedShader>
SegmentedVolumeData::createShader () const
{
	const bool segmentEnabled0 = getSegmentEnabled (0);
	const bool segmentEnabled1 = getSegmentEnabled (1);

	const auto & opacityMapVolumeStyle = getBrowser () -> getDefaultVolumeStyle ();
	std::string  styleUniforms         = opacityMapVolumeStyle -> getUniformsText ();
	std::string  styleFunctions        = opacityMapVolumeStyle -> getFunctionsText ();

	styleUniforms  += "\n";
	styleUniforms  += "uniform float surfaceTolerance;\n";

	if (segmentIdentifiersNode)
	{
		styleUniforms  += "\n";
		styleUniforms  += "uniform sampler3D segmentIdentifiers;\n";
		styleFunctions += "\n";

		styleFunctions += "\n";
		styleFunctions += "	float segment = texture (segmentIdentifiers, texCoord) .r;\n";
	}
	else
	{
		styleFunctions += "	float segment = 0.0;\n";
	}

	styleFunctions += "\n";
	styleFunctions += "	if (segment == 0.0)\n";
	styleFunctions += "	{\n";

	if (segmentEnabled0)
	{
		if (renderStyleNodes .size () > 0)
		{
			styleUniforms  += renderStyleNodes [0] -> getUniformsText (),
			styleFunctions += renderStyleNodes [0] -> getFunctionsText ();
		}
	}
	else
	{
		styleFunctions += "	return vec4 (0.0);\n";
	}

	styleFunctions += "	}\n";
	styleFunctions += "	else\n";
	styleFunctions += "	{\n";

	if (segmentEnabled1)
	{
		if (renderStyleNodes .size () > 1)
		{
			styleUniforms  += renderStyleNodes [1] -> getUniformsText (),
			styleFunctions += renderStyleNodes [1] -> getFunctionsText ();
		}
	}
	else
	{
		styleFunctions += "	return vec4 (0.0);\n";
	}

	styleFunctions += "	}\n";

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

	fragmentPart -> setName ("SegmentedVolumeDataFragmentShaderPart");
	vertexPart   -> setName ("SegmentedVolumeDataVertexShaderPart");
	shaderNode   -> setName ("SegmentedVolumeDataComposedShader");

	fragmentPart -> type () = "FRAGMENT";
	vertexPart   -> url ()  = { "data:x-shader/x-vertex," + vs };
	fragmentPart -> url ()  = { "data:x-shader/x-fragment," + fs };

	shaderNode -> parts () .emplace_back (vertexPart);
	shaderNode -> parts () .emplace_back (fragmentPart);

	if (voxelsNode)
	{
		auto textureSize = new SFVec3f (voxelsNode -> getWidth (), voxelsNode -> getHeight (), voxelsNode -> getDepth ());

		shaderNode -> addUserDefinedField (inputOutput, "x3d_TextureSize", textureSize);
	}
	else
	{
		shaderNode -> addUserDefinedField (inputOutput, "x3d_TextureSize", new SFVec3f ());
	}

	if (segmentIdentifiersNode)
		shaderNode -> addUserDefinedField (inputOutput, "segmentIdentifiers", new SFNode (segmentIdentifiersNode));

	opacityMapVolumeStyle -> addShaderFields (shaderNode);

	if (segmentEnabled0)
	{
		if (renderStyleNodes .size () > 0)
			renderStyleNodes [0] -> addShaderFields (shaderNode);
	}

	if (segmentEnabled1)
	{
		if (renderStyleNodes .size () > 1)
			renderStyleNodes [1] -> addShaderFields (shaderNode);
	}

	__LOG__ << std::endl << fs << std::endl;

	return shaderNode;
}

void
SegmentedVolumeData::shutdown ()
{
	for (const auto & renderStyleNode : renderStyleNodes)
		renderStyleNode -> removeVolumeData (this);

	X3DVolumeDataNode::shutdown ();
}

SegmentedVolumeData::~SegmentedVolumeData ()
{ }

} // X3D
} // titania
