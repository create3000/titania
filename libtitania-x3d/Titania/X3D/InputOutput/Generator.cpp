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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "Generator.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ExportedNode.h"
#include "../Execution/ImportedNode.h"
#include "../Parser/RegEx.h"

#include <Titania/String/to_string.h>
#include <cassert>

namespace titania {
namespace X3D {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Generator::AccessTypesIndex::AccessTypesIndex ()
{ }

const std::string &
Generator::AccessTypesIndex::operator [ ] (const X3DFieldDefinition* const fieldDefinition) const
{
	if (version == VRML_V2_0)
		return VrmlAccessTypes [fieldDefinition];

	return X3DAccessTypes [fieldDefinition];
}

Generator::X3DAccessTypesIndex::X3DAccessTypesIndex () :
	array ({ "initializeOnly", "inputOnly", "outputOnly", "inputOutput" })
{ }

const std::string &
Generator::X3DAccessTypesIndex::operator [ ] (const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getAccessType ())
	{
		case initializeOnly: return array [0];
		case inputOnly:      return array [1];
		case outputOnly:     return array [2];
		case inputOutput:    return array [3];
	}

	assert (false);
}

Generator::VrmlAccessTypesIndex::VrmlAccessTypesIndex () :
	array ({ "field", "eventIn", "eventOut", "exposedField" })
{ }

const std::string &
Generator::VrmlAccessTypesIndex::operator [ ] (const X3DFieldDefinition* const fieldDefinition) const
{
	switch (fieldDefinition -> getAccessType ())
	{
		case initializeOnly: return array [0];
		case inputOnly:      return array [1];
		case outputOnly:     return array [2];
		case inputOutput:    return array [3];
	}

	assert (false);
}

Generator::NodeTypesIndex::NodeTypesIndex () :
	array ({
	          "X3DAppearanceChildNode",
	          "X3DAppearanceNode",
	          "X3DBackgroundNode",
	          "X3DBindableNode",
	          "X3DBoundedObject",
	          "X3DChaserNode",
	          "X3DChildNode",
	          "X3DColorNode",
	          "X3DComposedGeometryNode",
	          "X3DCoordinateNode",
	          "X3DDamperNode",
	          "X3DDragSensorNode",
	          "X3DEnvironmentalSensorNode",
	          "X3DEnvironmentTextureNode",
	          "X3DFogObject",
	          "X3DFollowerNode",
	          "X3DFontStyleNode",
	          "X3DGeometricPropertyNode",
	          "X3DGeometryNode",
	          "X3DGeospatialObject",
	          "X3DGroupingNode",
	          "X3DInfoNode",
	          "X3DInterpolatorNode",
	          "X3DKeyDeviceSensorNode",
	          "X3DLayerNode",
	          "X3DLayoutNode",
	          "X3DLightNode",
	          "X3DMaterialNode",
	          "X3DMetadataObject",
	          "X3DNBodyCollidableNode",
	          "X3DNBodyCollisionSpaceNode",
	          "X3DNetworkSensorNode",
	          "X3DNode",
	          "X3DNormalNode",
	          "X3DNurbsControlCurveNode",
	          "X3DNurbsSurfaceGeometryNode",
	          "X3DParametricGeometryNode",
	          "X3DParticleEmitterNode",
	          "X3DParticlePhysicsModelNode",
	          "X3DPickableObject",
	          "X3DPickingNode",
	          "X3DPickSensorNode",
	          "X3DPointingDeviceSensorNode",
	          "X3DProductStructureChildNode",
	          "X3DProgrammableShaderObject",
	          "X3DPrototypeInstance",
	          "X3DRigidJointNode",
	          "X3DScriptNode",
	          "X3DSensorNode",
	          "X3DSequencerNode",
	          "X3DShaderNode",
	          "X3DShapeNode",
	          "X3DSoundNode",
	          "X3DSoundSourceNode",
	          "X3DTexture2DNode",
	          "X3DTexture3DNode",
	          "X3DTextureCoordinateNode",
	          "X3DTextureNode",
	          "X3DTextureTransformNode",
	          "X3DTimeDependentNode",
	          "X3DTouchSensorNode",
	          "X3DTransformNode",
	          "X3DTriggerNode",
	          "X3DUrlObject",
	          "X3DVertexAttributeNode",
	          "X3DViewpointNode",
	          "X3DViewportNode"
			 })
{ }

const std::string &
Generator::NodeTypesIndex::operator [ ] (const X3DBaseNode* const baseNode) const
{
	return operator [ ] (baseNode -> getNodeType () .back ());
}

const Generator::AccessTypesIndex     Generator::AccessTypes;
const Generator::X3DAccessTypesIndex  Generator::X3DAccessTypes;
const Generator::VrmlAccessTypesIndex Generator::VrmlAccessTypes;
const Generator::NodeTypesIndex       Generator::NodeTypes;

Generator::StyleType Generator::style   = NICEST;
VersionType          Generator::version = LATEST_VERSION;

Generator::ExecutionContextStack Generator::executionContextStack;
size_t                           Generator::level = 0;
Generator::NodeSet               Generator::exportedNodesIndex;
Generator::NodeSet               Generator::importedNodesIndex;
Generator::NodeSet               Generator::nodes;
Generator::NameIndex             Generator::names;
Generator::NameIndexByNode       Generator::namesByNode;
size_t                           Generator::newName = 0;
Generator::ImportedNamesIndex    Generator::importedNames;
Generator::FieldStack            Generator::containerFieldStack (1);
static const std::string         emptyName;

void
Generator::Style (const std::string & value)
{
	std::string style = value;

	std::transform (style .begin (), style .end (), style .begin (), ::toupper);

	if (style == "SMALLEST")
		SmallestStyle ();

	else if (style == "SMALL")
		SmallStyle ();

	else if (style == "COMPACT")
		CompactStyle ();

	else
		NicestStyle ();
}

void
Generator::SmallestStyle ()
{
	if (style == SMALLEST)
		return;

	style = SMALLEST;

	space     = " ";
	tidySpace = "";
	endl      = " ";
	tidyBreak = "";
	listBreak = " ";
	comma     = "";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
}

void
Generator::SmallStyle ()
{
	if (style == SMALL)
		return;

	style = SMALL;

	space     = " ";
	tidySpace = "";
	endl      = "\n";
	tidyBreak = "";
	listBreak = " ";
	comma     = "";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
}

void
Generator::CompactStyle ()
{
	if (style == COMPACT)
		return;

	style = COMPACT;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = " ";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = false;
}

void
Generator::NicestStyle ()
{
	if (style == NICEST)
		return;

	style = NICEST;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = "\n";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = true;
}

void
Generator::PushContext ()
{
	if (level == 0)
		newName = 0;

	++ level;
}

void
Generator::PopContext ()
{
	-- level;

	if (level == 0)
	{
		nodes .clear ();
		names .clear ();
		namesByNode .clear ();
		importedNames .clear ();
	}
}
	
void
Generator::setExportedNodes (const ExportedNodeIndex & exportedNodes)
{
	exportedNodesIndex .clear ();

	for (const auto & exportedNode : exportedNodes)
		exportedNodesIndex .emplace (exportedNode .second -> getLocalNode ());
}

void
Generator::setImportedNodes (const ImportedNodeIndex & importedNodes)
{
	importedNodesIndex .clear ();

	for (const auto & importedNode : importedNodes)
		importedNodesIndex .emplace (importedNode .second -> getInlineNode ());
}

bool
Generator::IsSharedNode (const X3DBaseNode* const baseNode)
{
	if (executionContextStack .empty ())
		return false;

	return executionContextStack .back () not_eq baseNode -> getExecutionContext ();
}

bool
Generator::ExistsNode (const X3DBaseNode* const baseNode)
{
	return nodes .find (baseNode) not_eq nodes .end ();
}

void
Generator::AddNode (const X3DBaseNode* const baseNode)
{
	nodes .emplace (baseNode);
}

const std::string &
Generator::GetName (const X3DBaseNode* const baseNode)
{
	// Is the node already in index

	const auto iter = namesByNode .find (baseNode);

	if (iter not_eq namesByNode .end ())
		return iter -> second;

	// The node has no name

	if (baseNode -> getName () .empty ())
	{
		if (needsName (baseNode))
		{
			std::string name = getUniqueName ();

			names [name]           = baseNode;
			namesByNode [baseNode] = name;

			return namesByNode [baseNode];
		}

		// The node doesn't need a name

		return baseNode -> getName ();
	}

	// The node has a name

	std::string name      = baseNode -> getName ();
	const bool  hasNumber = RegEx::_LastNumber .PartialMatch (name);

	RegEx::_LastNumber .Replace ("", &name);

	if (name .empty ())
	{
		if (needsName (baseNode))
			name = getUniqueName ();

		else
			return emptyName;
	}
	else
	{
		size_t      i       = 0;
		std::string newName = hasNumber ? name + '_' + basic::to_string (++ i) : name;

		try
		{
			for ( ; ;)
			{
				names .at (newName);

				newName = name + '_' + basic::to_string (++ i);
			}
		}
		catch (const std::out_of_range &)
		{
			name = newName;
		}
	}

	names [name]           = baseNode;
	namesByNode [baseNode] = name;

	return namesByNode [baseNode];
}

bool
Generator::needsName (const X3DBaseNode* const baseNode)
{
	if (baseNode -> getNumClones () > 1)
		return true;
	
	 if (baseNode -> hasRoutes ())
	   return true;

	 if (exportedNodesIndex .find (baseNode) not_eq exportedNodesIndex .end ())
	   return true;

	 if (importedNodesIndex .find (baseNode) not_eq importedNodesIndex .end ())
	   return true;

	return false;
}

std::string
Generator::getUniqueName ()
{
	std::string name;

	try
	{
		for ( ; ;)
		{
			name = '_' + basic::to_string (++ newName);

			names .at (name);
		}
	}
	catch (const std::out_of_range &)
	{ }

	return name;
}

void
Generator::AddImportedNode (const X3DBaseNode* const exportedNode, const std::string & importedName)
{
	importedNames [exportedNode] = importedName;
}

const std::string &
Generator::GetLocalName (const X3DBaseNode* node)
{
	try
	{
		return importedNames .at (node);
	}
	catch (...)
	{
		if (ExistsNode (node))
			return GetName (node);
	}

	throw Error <INVALID_NODE> ("Couldn't get local name for node '" + node -> getTypeName () + "'.");
}

void
Generator::XMLEncodeToStream (std::ostream & ostream, const std::string & string)
{
	for (const auto & c : string)
	{
		switch (c)
		{
			case '\t' :
				{
					ostream << "&#x9;";
					break;
				}
			case '\n':
			{
				ostream << "&#xA;";
				break;
			}
			case '\r':
			{
				ostream << "&#xD;";
				break;
			}
			case '<':
			{
				ostream << "&lt;";
				break;
			}
			case '&':
			{
				ostream << "&amp;";
				break;
			}
			case '\'':
			{
				ostream << "&apos;";
				break;
			}
			case '"':
			{
				ostream << "\\\"";
				break;
			}
			case '\\':
			{
				ostream << "\\\\";
				break;
			}
			default:
			{
				ostream << c;
				break;
			}
		}
	}
}

} // X3D
} // titania
