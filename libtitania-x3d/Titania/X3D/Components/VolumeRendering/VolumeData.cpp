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

#include "VolumeData.h"

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

const Component   VolumeData::component      = Component ("VolumeRendering", 2);
const std::string VolumeData::typeName       = "VolumeData";
const std::string VolumeData::containerField = "children";

VolumeData::Fields::Fields () :
	renderStyle (new SFNode ()),
	     voxels (new SFNode ())
{ }

VolumeData::VolumeData (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DVolumeDataNode (),
	           fields (),
	  renderStyleNode (),
	       voxelsNode (),
	    blendModeNode (new BlendMode (executionContext))
{
	addType (X3DConstants::VolumeData);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "dimensions",  dimensions ());
	addField (inputOutput,    "renderStyle", renderStyle ());
	addField (inputOutput,    "voxels",      voxels ());
	addField (initializeOnly, "bboxSize",    bboxSize ());
	addField (initializeOnly, "bboxCenter",  bboxCenter ());

	addChildObjects (renderStyleNode,
	                 voxelsNode,
	                 blendModeNode);
}

X3DBaseNode*
VolumeData::create (X3DExecutionContext* const executionContext) const
{
	return new VolumeData (executionContext);
}

void
VolumeData::initialize ()
{
	X3DVolumeDataNode::initialize ();

	renderStyle () .addInterest (&VolumeData::set_renderStyle, this);
	voxels ()      .addInterest (&VolumeData::set_voxels,      this);
	voxels ()      .addInterest (getAppearance () -> texture ());

	renderStyle () .addInterest (&VolumeData::update,          this);

	blendModeNode -> setup ();

	getAppearance () -> texture ()   = voxels ();
	getAppearance () -> blendMode () = blendModeNode;

	set_renderStyle ();
	set_voxels ();

	update ();
}

void
VolumeData::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DVolumeDataNode::setExecutionContext (executionContext);

	blendModeNode -> setExecutionContext (executionContext);
}

void
VolumeData::set_renderStyle ()
{
	if (renderStyleNode)
	{
		renderStyleNode -> removeInterest (&VolumeData::update, this);
		renderStyleNode -> removeVolumeData (this);
	}

	renderStyleNode = x3d_cast <X3DVolumeRenderStyleNode*> (renderStyle ());

	if (renderStyleNode)
	{
		renderStyleNode -> addInterest (&VolumeData::update, this);
		renderStyleNode -> addVolumeData (this);
	}
}

void
VolumeData::set_voxels ()
{
	if (voxelsNode)
		voxelsNode -> removeInterest (&VolumeData::set_textureSize, this);

	voxelsNode = x3d_cast <X3DTexture3DNode*> (voxels ());

	if (voxelsNode)
	{
		voxelsNode -> addInterest (&VolumeData::set_textureSize, this);

		set_textureSize ();
	}
}

void
VolumeData::set_textureSize ()
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
VolumeData::update ()
{
	setShader (createShader ());
}

X3DPtr <ComposedShader>
VolumeData::createShader () const
{
	const auto & opacityMapVolumeStyle = getBrowser () -> getDefaultVolumeStyle ();
	std::string  styleUniforms         = opacityMapVolumeStyle -> getUniformsText ();
	std::string  styleFunctions        = opacityMapVolumeStyle -> getFunctionsText ();

	styleUniforms  += "\n";
	styleUniforms  += "uniform float surfaceTolerance;\n";

	if (renderStyleNode)
	{
		styleUniforms  += renderStyleNode -> getUniformsText (),
		styleFunctions += renderStyleNode -> getFunctionsText ();
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

	if (renderStyleNode)
		renderStyleNode -> addShaderFields (shaderNode);

	__LOG__ << std::endl << fs << std::endl;

	return shaderNode;
}

void
VolumeData::shutdown ()
{
	if (renderStyleNode)
		renderStyleNode -> removeVolumeData (this);

	X3DVolumeDataNode::shutdown ();
}

VolumeData::~VolumeData ()
{ }

} // X3D
} // titania
