/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
 ******************************************************************************/

#include "Generator.h"

#include "../Browser/Browser.h"

#include <cassert>

namespace titania {
namespace X3D {

template std::ostream & CleanStyle   (std::ostream &);
template std::ostream & CompactStyle (std::ostream &);
template std::ostream & TidyStyle    (std::ostream &);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Generator::AccessTypesIndex::AccessTypesIndex () :
	std::vector <std::string> ({ "initializeOnly", "inputOnly", "outputOnly", "inputOutput" })
{ }

const std::string &
Generator::AccessTypesIndex::operator [ ] (const X3DFieldDefinition* const fieldDefinition) const
{
	if (X3DAccessTypes ())
	{
		switch (fieldDefinition -> getAccessType ())
		{
			case initializeOnly: return data () [0];
			case inputOnly:      return data () [1];
			case outputOnly:     return data () [2];
			case inputOutput:    return data () [3];
		}

		assert (false);
	}

	return VrmlAccessTypes [fieldDefinition];
}

Generator::VrmlAccessTypesIndex::VrmlAccessTypesIndex () :
	std::vector <std::string> ({ "field", "eventIn", "eventOut", "exposedField" })
{ }

const std::string &
Generator::VrmlAccessTypesIndex::operator [ ] (const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getAccessType ())
	{
		case initializeOnly: return data () [0];
		case inputOnly:      return data () [1];
		case outputOnly:     return data () [2];
		case inputOutput:    return data () [3];
	}

	assert (false);
}

Generator::NodeTypesIndex::NodeTypesIndex ()
{
	emplace_back ("X3DAppearanceChildNodeType");
	emplace_back ("X3DAppearanceNodeType");
	emplace_back ("X3DBackgroundNodeType");
	emplace_back ("X3DBindableNodeType");
	emplace_back ("X3DBoundedObjectType");
	emplace_back ("X3DChaserNodeType");
	emplace_back ("X3DChildNodeType");
	emplace_back ("X3DColorNodeType");
	emplace_back ("X3DComposedGeometryNodeType");
	emplace_back ("X3DCoordinateNodeType");
	emplace_back ("X3DDamperNodeType");
	emplace_back ("X3DDragSensorNodeType");
	emplace_back ("X3DEnvironmentalSensorNodeType");
	emplace_back ("X3DEnvironmentTextureNodeType");
	emplace_back ("X3DFogObjectType");
	emplace_back ("X3DFollowerNodeType");
	emplace_back ("X3DFontStyleNodeType");
	emplace_back ("X3DGeometricPropertyNodeType");
	emplace_back ("X3DGeometryNodeType");
	emplace_back ("X3DGroupingNodeType");
	emplace_back ("X3DInfoNodeType");
	emplace_back ("X3DInterpolatorNodeType");
	emplace_back ("X3DKeyDeviceSensorNodeType");
	emplace_back ("X3DLayerNodeType");
	emplace_back ("X3DLayoutNodeType");
	emplace_back ("X3DLightNodeType");
	emplace_back ("X3DMaterialNodeType");
	emplace_back ("X3DMetadataObjectType");
	emplace_back ("X3DNBodyCollidableNodeType");
	emplace_back ("X3DNBodyCollisionSpaceNodeType");
	emplace_back ("X3DNetworkSensorNodeType");
	emplace_back ("X3DNodeType");
	emplace_back ("X3DNormalNodeType");
	emplace_back ("X3DNurbsControlCurveNodeType");
	emplace_back ("X3DNurbsSurfaceGeometryNodeType");
	emplace_back ("X3DParametricGeometryNodeType");
	emplace_back ("X3DParticleEmitterNodeType");
	emplace_back ("X3DParticlePhysicsModelNodeType");
	emplace_back ("X3DPickableObjectType");
	emplace_back ("X3DPickingNodeType");
	emplace_back ("X3DPickSensorNodeType");
	emplace_back ("X3DPointingDeviceSensorNodeType");
	emplace_back ("X3DProductStructureChildNodeType");
	emplace_back ("X3DProgrammableShaderObjectType");
	emplace_back ("X3DPropertyNode");
	emplace_back ("X3DPrototypeInstanceType");
	emplace_back ("X3DRigidJointNodeType");
	emplace_back ("X3DScriptNodeType");
	emplace_back ("X3DSensorNodeType");
	emplace_back ("X3DSequencerNodeType");
	emplace_back ("X3DShaderNodeType");
	emplace_back ("X3DShapeNodeType");
	emplace_back ("X3DSoundNodeType");
	emplace_back ("X3DSoundSourceNodeType");
	emplace_back ("X3DTexture2DNodeType");
	emplace_back ("X3DTexture3DNodeType");
	emplace_back ("X3DTextureCoordinateNodeType");
	emplace_back ("X3DTextureNodeType");
	emplace_back ("X3DTextureTransformNodeType");
	emplace_back ("X3DTimeDependentNodeType");
	emplace_back ("X3DTouchSensorNodeType");
	emplace_back ("X3DTriggerNodeType");
	emplace_back ("X3DUrlObjectType");
	emplace_back ("X3DVertexAttributeNodeType");
	emplace_back ("X3DViewpointNodeType");
	emplace_back ("X3DViewportNodeType");
}

const std::string &
Generator::NodeTypesIndex::operator [ ] (const X3DBasicNode* basicNode) const
{
	return operator [ ] (basicNode -> getNodeType () .back ());
}

const Generator::VrmlAccessTypesIndex Generator::VrmlAccessTypes;
const Generator::AccessTypesIndex     Generator::AccessTypes;
const Generator::NodeTypesIndex       Generator::NodeTypes;

size_t                 Generator::level = 0;
Generator::NodesSet    Generator::nodes;
Generator::NewNamesMap Generator::newNames;
size_t                 Generator::newName = 0;

bool        Generator::expandNodes = false;
std::string Generator::style       = "tidy";

bool Generator::x3dFieldNames  = true;
bool Generator::x3dAccessTypes = true;

void
Generator::Style (const std::string & value)
{
	if (value == "clean")
		CleanStyle ();

	else if (value == "compact")
		CompactStyle ();

	else
		TidyStyle ();
}

void
Generator::CleanStyle ()
{
	style = "clean";

	space     = " ";
	tidySpace = "";
	endl      = " ";
	tidyBreak = "";
	listBreak = " ";
	comma     = "";

	indent     = "";
	indentChar = "";

	hasListBreak = false;
}

void
Generator::CompactStyle ()
{
	style = "compact";

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = " ";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	hasListBreak = false;
}

void
Generator::TidyStyle ()
{
	style = "tidy";

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = "\n";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	hasListBreak = true;
}

void
Generator::PushLevel ()
{
	if (level == 0)
		newName = 0;

	++ level;
}

void
Generator::PopLevel ()
{
	-- level;

	if (level == 0)
	{
		nodes .clear ();
		newNames .clear ();
	}
}

bool
Generator::ExistsNode (const X3DBasicNode* basicNode)
{
	return nodes .find (basicNode) not_eq nodes .end ();
}

void
Generator::AddNode (const X3DBasicNode* basicNode)
{
	nodes .insert (basicNode);
}

std::string
Generator::GetName (const X3DBasicNode* basicNode)
{
	if (basicNode -> getName () .empty ())
	{
		NewNamesMap::const_iterator iter = newNames .find (basicNode);

		if (iter not_eq newNames .end ())
			return iter -> second;

		if (basicNode -> getNumClones () > 1)
		{
			std::string name;

			try
			{
				for ( ; ;)
				{
					name = '_' + std::to_string (newName ++);

					basicNode -> getExecutionContext () -> getNamedNode (name);
				}
			}
			catch (const Error <INVALID_NAME> &)
			{ }

			newNames [basicNode] = name;

			return name;
		}
	}

	return basicNode -> getName ();
}

} // X3D
} // titania
