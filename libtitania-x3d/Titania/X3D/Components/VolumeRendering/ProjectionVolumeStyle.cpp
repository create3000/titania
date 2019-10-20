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

#include "ProjectionVolumeStyle.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"

namespace titania {
namespace X3D {

const Component   ProjectionVolumeStyle::component      = Component ("VolumeRendering", 2);
const std::string ProjectionVolumeStyle::typeName       = "ProjectionVolumeStyle";
const std::string ProjectionVolumeStyle::containerField = "renderStyle";

ProjectionVolumeStyle::Fields::Fields () :
	              type (new SFString ("MAX")),
	intensityThreshold (new SFFloat (0))
{ }

ProjectionVolumeStyle::ProjectionVolumeStyle (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DVolumeRenderStyleNode (),
	                  fields (),
	                typeType (TypeType::MAX)
{
	addType (X3DConstants::ProjectionVolumeStyle);

	addField (inputOutput, "metadata",           metadata ());
	addField (inputOutput, "enabled",            enabled ());
	addField (inputOutput, "type",               type ());
	addField (inputOutput, "intensityThreshold", intensityThreshold ());
}

X3DBaseNode*
ProjectionVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new ProjectionVolumeStyle (executionContext);
}

void
ProjectionVolumeStyle::initialize ()
{
	X3DVolumeRenderStyleNode::initialize ();

	type () .addInterest (&ProjectionVolumeStyle::set_type, this);

	set_type ();
}

void
ProjectionVolumeStyle::set_type ()
{
	try
	{
		static const std::map <std::string, TypeType> typeTypes = {
			std::pair ("MAX",     TypeType::MAX),
			std::pair ("MIN",     TypeType::MIN),
			std::pair ("AVERAGE", TypeType::AVERAGE),
		};

		typeType = typeTypes .at (type ());
	}
	catch(const std::out_of_range & error)
	{
		typeType = TypeType::MAX;
	}
}

void
ProjectionVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	shaderNode -> addUserDefinedField (inputOutput, "intensityThreshold_" + getStyleId (), intensityThreshold () .copy (CopyType::FLAT_COPY));
}

std::string
ProjectionVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// ProjectionVolumeStyle\n";
	string += "\n";
	string += "uniform float intensityThreshold_" + getStyleId () + ";\n";

	string += "\n";
	string += "vec4\n";
	string += "getProjectionStyle_" + getStyleId () + "(in vec4 originalColor, in vec3 texCoord)\n";
	string += "{\n";

	switch (typeType)
	{
		default:
		case TypeType::MAX:
		{
			string += "	float projectionColor = 0.0;\n";
			break;
		}
		case TypeType::MIN:
		{
			string += "	float projectionColor = 1.0;\n";
			break;
		}
		case TypeType::AVERAGE:
		{
			string += "	float projectionColor = 0.0;\n";
			break;
		}
	}

	string += "	const int samples     = 32;\n";
	string += "	vec3  step            = normalize (x3d_TextureNormalMatrix * vec3 (0.0, 0.0, 1.0)) / float (samples);\n";
	string += "	vec3  ray             = texCoord - step * float (samples) * 0.5;\n";
	string += "	bool  first           = false;\n";
	string += "\n";
	string += "	for (int i = 0; i < samples; ++ i, ray += step)\n";
	string += "	{\n";
	string += "		if (ray .s < 0.0 || ray .s > 1.0)\n";
	string += "			continue;\n";
	string += "\n";
	string += "		if (ray .t < 0.0 || ray .t > 1.0)\n";
	string += "			continue;\n";
	string += "\n";
	string += "		if (ray .p < 0.0 || ray .p > 1.0)\n";
	string += "			continue;\n";
	string += "\n";
	string += "		float intensity = texture (x3d_Texture3D [0], ray) .r;\n";
	string += "\n";

	switch (typeType)
	{
		default:
		case TypeType::MAX:
		{
			string += "		if (intensity < intensityThreshold_" + getStyleId () + ")\n";
			string += "			continue;\n";
			string += "\n";
			string += "		if (intensityThreshold_" + getStyleId () + " > 0.0 && first)\n";
			string += "			break;\n";
			string += "\n";
			string += "		if (intensity <= projectionColor)\n";
			string += "		{\n";
			string += "			first = true;\n";
			string += "			continue;\n";
			string += "		}\n";
			string += "\n";
			string += "		projectionColor = intensity;\n";
			break;
		}
		case TypeType::MIN:
		{
			string += "		if (intensity < intensityThreshold_" + getStyleId () + ")\n";
			string += "			continue;\n";
			string += "\n";
			string += "		if (intensityThreshold_" + getStyleId () + " > 0.0 && first)\n";
			string += "			break;\n";
			string += "\n";
			string += "		if (intensity >= projectionColor)\n";
			string += "		{\n";
			string += "			first = true;\n";
			string += "			continue;\n";
			string += "		}\n";
			string += "\n";
			string += "		projectionColor = intensity;\n";
			break;
		}
		case TypeType::AVERAGE:
		{
			string += "		projectionColor += intensity;\n";
			break;
		}
	}

	string += "	}\n";
	string += "\n";

	if (typeType == TypeType::AVERAGE)
		string += "	projectionColor /= float (samples);\n";

	string += "	return vec4 (vec3 (projectionColor), originalColor .a);\n";
	string += "}\n";

	return string;
}

std::string
ProjectionVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// ProjectionVolumeStyle\n";
	string += "\n";
	string += "	textureColor = getProjectionStyle_" + getStyleId () + " (textureColor, texCoord);\n";

	return string;
}

ProjectionVolumeStyle::~ProjectionVolumeStyle ()
{ }

} // X3D
} // titania
