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

#include "ShadedVolumeStyle.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ComposedShader.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Texturing3D/X3DTexture3DNode.h"

namespace titania {
namespace X3D {

const Component   ShadedVolumeStyle::component      = Component ("VolumeRendering", 3);
const std::string ShadedVolumeStyle::typeName       = "ShadedVolumeStyle";
const std::string ShadedVolumeStyle::containerField = "renderStyle";

ShadedVolumeStyle::Fields::Fields () :
	      lighting (new SFBool ()),
	       shadows (new SFBool ()),
	 phaseFunction (new SFString ("Henyey-Greenstein")),
	      material (new SFNode ()),
	surfaceNormals (new SFNode ())
{ }

ShadedVolumeStyle::ShadedVolumeStyle (X3DExecutionContext* const executionContext) :
	                       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposableVolumeRenderStyleNode (),
	                            fields (),
	                surfaceNormalsNode ()
{
	addType (X3DConstants::ShadedVolumeStyle);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "enabled",        enabled ());
	addField (inputOutput,    "lighting",       lighting ());
	addField (inputOutput,    "shadows",        shadows ());
	addField (initializeOnly, "phaseFunction",  phaseFunction ());
	addField (inputOutput,    "material",       material ());
	addField (inputOutput,    "surfaceNormals", surfaceNormals ());

	addChildObjects (materialNode,
	                 surfaceNormalsNode);
}

X3DBaseNode*
ShadedVolumeStyle::create (X3DExecutionContext* const executionContext) const
{
	return new ShadedVolumeStyle (executionContext);
}

void
ShadedVolumeStyle::initialize ()
{
	X3DComposableVolumeRenderStyleNode::initialize ();

	material ()       .addInterest (&ShadedVolumeStyle::set_material,       this);
	surfaceNormals () .addInterest (&ShadedVolumeStyle::set_surfaceNormals, this);

	set_material ();
	set_surfaceNormals ();
}

void
ShadedVolumeStyle::set_material ()
{
	if (materialNode)
		materialNode -> removeInterest (&ShadedVolumeStyle::addEvent, this);

	materialNode = x3d_cast <X3DMaterialNode*> (material ());

	if (materialNode)
		materialNode -> addInterest (&ShadedVolumeStyle::addEvent, this);
}

void
ShadedVolumeStyle::set_surfaceNormals ()
{
	surfaceNormalsNode = x3d_cast <X3DTexture3DNode*> (surfaceNormals ());
}

void
ShadedVolumeStyle::addShaderFields (const X3DPtr <ComposedShader> & shaderNode) const
{
	if (not enabled ())
		return;

	if (materialNode)
	{
		shaderNode -> addUserDefinedField (inputOutput, "ambientIntensity_" + getStyleId (), materialNode -> getField ("ambientIntensity") -> copy (CopyType::FLAT_COPY));
		shaderNode -> addUserDefinedField (inputOutput, "diffuseColor_"     + getStyleId (), materialNode -> getField ("diffuseColor")     -> copy (CopyType::FLAT_COPY));
		shaderNode -> addUserDefinedField (inputOutput, "specularColor_"    + getStyleId (), materialNode -> getField ("specularColor")    -> copy (CopyType::FLAT_COPY));
		shaderNode -> addUserDefinedField (inputOutput, "emissiveColor_"    + getStyleId (), materialNode -> getField ("emissiveColor")    -> copy (CopyType::FLAT_COPY));
		shaderNode -> addUserDefinedField (inputOutput, "shininess_"        + getStyleId (), materialNode -> getField ("shininess")        -> copy (CopyType::FLAT_COPY));
		shaderNode -> addUserDefinedField (inputOutput, "transparency_"     + getStyleId (), materialNode -> getField ("transparency")     -> copy (CopyType::FLAT_COPY));
	}

	if (surfaceNormalsNode)
		shaderNode -> addUserDefinedField (inputOutput, "surfaceNormals_" + getStyleId (), new SFNode (surfaceNormalsNode));
}

std::string
ShadedVolumeStyle::getUniformsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "// ShadedVolumeStyle\n";
	string += "\n";
	string += "uniform float ambientIntensity_" + getStyleId () + ";\n";
	string += "uniform vec3  diffuseColor_" + getStyleId () + ";\n";
	string += "uniform vec3  specularColor_" + getStyleId () + ";\n";
	string += "uniform vec3  emissiveColor_" + getStyleId () + ";\n";
	string += "uniform float shininess_" + getStyleId () + ";\n";
	string += "uniform float transparency_" + getStyleId () + ";\n";

	string += getNormalText (surfaceNormalsNode);

	string += "\n";
	string += "float\n";
	string += "getSpotFactor_" + getStyleId () + " (const in float cutOffAngle, const in float beamWidth, const in vec3 L, const in vec3 d)\n";
	string += "{\n";
	string += "	float spotAngle = acos (clamp (dot (-L, d), -1.0, 1.0));\n";
	string += "\n";
	string += "	if (spotAngle >= cutOffAngle)\n";
	string += "		return 0.0;\n";
	string += "	else if (spotAngle <= beamWidth)\n";
	string += "		return 1.0;\n";
	string += "\n";
	string += "	return (spotAngle - cutOffAngle) / (beamWidth - cutOffAngle);\n";
	string += "}\n";

	return string;
}

std::string
ShadedVolumeStyle::getFunctionsText () const
{
	if (not enabled ())
		return "";

	std::string string;

	string += "\n";
	string += "	// ShadedVolumeStyle\n";
	string += "\n";
	string += "	{\n";

	string += "		vec4 surfaceNormal = getNormal_" + getStyleId () + " (texCoord);\n";
	string += "		vec4 shadedColor   = vec4 (0.0);\n";
	string += "\n";
	string += "		if (surfaceNormal .w < normalTolerance)\n";
	string += "		{\n";
	string += "			textureColor = vec4 (0.0);\n";
	string += "		}\n";
	string += "		else\n";
	string += "		{\n";

	if (lighting ())
	{
		if (materialNode)
		{
			string += "			vec3 diffuseFactor = diffuseColor_" + getStyleId () + ";\n";
			string += "			vec3 ambientTerm   = diffuseFactor * ambientIntensity_" + getStyleId () + ";\n";
			string += "\n";
			string += "			shadedColor .a = textureColor .a * (1.0 - transparency_" + getStyleId () + ");\n";
		}
		else
		{
			string += "			vec3 diffuseFactor = textureColor .rgb;\n";
			string += "			vec3 ambientTerm   = vec3 (0.0);\n";
			string += "\n";
			string += "			shadedColor .a = textureColor .a;\n";
		}

		string += "\n";
		string += "			vec3  N  = surfaceNormal .xyz;\n";
		string += "			vec3  V  = normalize (-vertex); // normalized vector from point on geometry to viewer's position\n";
		string += "			float dV = length (vertex);\n";
		string += "\n";
		string += "			for (int i = 0; i < x3d_MaxLights; ++ i)\n";
		string += "			{\n";
		string += "				if (i == x3d_NumLights)\n";
		string += "					break;\n";
		string += "\n";
		string += "				x3d_LightSourceParameters light = x3d_LightSource [i];\n";
		string += "\n";
		string += "				vec3  vL = light .location - vertex; // Light to fragment\n";
		string += "				float dL = length (light .matrix * vL);\n";
		string += "				bool  di = light .type == x3d_DirectionalLight;\n";
		string += "\n";
		string += "				if (di || dL <= light .radius)\n";
		string += "				{\n";
		string += "					vec3 d = light .direction;\n";
		string += "					vec3 c = light .attenuation;\n";
		string += "					vec3 L = di ? -d : normalize (vL);      // Normalized vector from point on geometry to light source i position.\n";
		string += "					vec3 H = normalize (L + V);             // Specular term\n";
		string += "\n";
		string += "					float lightAngle     = dot (N, L);      // Angle between normal and light ray.\n";
		string += "					vec3  diffuseTerm    = diffuseFactor * clamp (lightAngle, 0.0, 1.0);\n";
		string += "					float specularFactor = shininess_" + getStyleId () + " > 0.0 ? pow (max (dot (N, H), 0.0), shininess_" + getStyleId () + " * 128.0) : 1.0;\n";
		string += "					vec3  specularTerm   = light .intensity * specularColor_" + getStyleId () + " * specularFactor;\n";
		string += "\n";
		string += "					float attenuationFactor     = di ? 1.0 : 1.0 / max (c [0] + c [1] * dL + c [2] * (dL * dL), 1.0);\n";
		string += "					float spotFactor            = light .type == x3d_SpotLight ? getSpotFactor_" + getStyleId () + " (light .cutOffAngle, light .beamWidth, L, d) : 1.0;\n";
		string += "					float attenuationSpotFactor = attenuationFactor * spotFactor;\n";
		string += "					vec3  ambientColor          = light .ambientIntensity * ambientTerm;\n";
		string += "					vec3  diffuseColor          = light .intensity * diffuseTerm * max (dot (N, L), 0.0);\n";
		string += "\n";
		string += "					shadedColor .rgb += attenuationSpotFactor * light .color * (ambientColor + diffuseColor + specularTerm);\n";
		string += "				}\n";
		string += "\n";
		string += "				shadedColor .rgb += emissiveColor_" + getStyleId () + ";\n";
		string += "				shadedColor .rgb  = getFogColor (shadedColor .rgb);\n";
		string += "			}\n";
	}
	else
	{
		if (materialNode)
		{
			string += "			shadedColor .rgb = diffuseColor_" + getStyleId () + ";\n";
			string += "			shadedColor .a   = textureColor .a * (1.0 - transparency_" + getStyleId () + ");\n";
		}
		else
		{
			string += "			shadedColor = textureColor;\n";
		}
	}

	string += "\n";
	string += "			textureColor = shadedColor;\n";
	string += "		}\n";

	string += "	}\n";

	return string;
}

ShadedVolumeStyle::~ShadedVolumeStyle ()
{ }

} // X3D
} // titania
