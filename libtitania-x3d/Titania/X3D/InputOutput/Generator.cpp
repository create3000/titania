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
#include "../Parser/RegEx.h"

#include <cassert>

namespace titania {
namespace X3D {

template std::ostream & SmallestStyle (std::ostream &);
template std::ostream & CompactStyle  (std::ostream &);
template std::ostream & NicestStyle   (std::ostream &);

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
	emplace_back ("X3DAppearanceChildNode");
	emplace_back ("X3DAppearanceNode");
	emplace_back ("X3DBackgroundNode");
	emplace_back ("X3DBindableNode");
	emplace_back ("X3DBoundedObject");
	emplace_back ("X3DChaserNode");
	emplace_back ("X3DChildNode");
	emplace_back ("X3DColorNode");
	emplace_back ("X3DComposedGeometryNode");
	emplace_back ("X3DCoordinateNode");
	emplace_back ("X3DDamperNode");
	emplace_back ("X3DDragSensorNode");
	emplace_back ("X3DEnvironmentalSensorNode");
	emplace_back ("X3DEnvironmentTextureNode");
	emplace_back ("X3DFogObject");
	emplace_back ("X3DFollowerNode");
	emplace_back ("X3DFontStyleNode");
	emplace_back ("X3DGeometricPropertyNode");
	emplace_back ("X3DGeometryNode");
	emplace_back ("X3DGeospatialObject");
	emplace_back ("X3DGroupingNode");
	emplace_back ("X3DInfoNode");
	emplace_back ("X3DInterpolatorNode");
	emplace_back ("X3DKeyDeviceSensorNode");
	emplace_back ("X3DLayerNode");
	emplace_back ("X3DLayoutNode");
	emplace_back ("X3DLightNode");
	emplace_back ("X3DMaterialNode");
	emplace_back ("X3DMetadataObject");
	emplace_back ("X3DNBodyCollidableNode");
	emplace_back ("X3DNBodyCollisionSpaceNode");
	emplace_back ("X3DNetworkSensorNode");
	emplace_back ("X3DNode");
	emplace_back ("X3DNormalNode");
	emplace_back ("X3DNurbsControlCurveNode");
	emplace_back ("X3DNurbsSurfaceGeometryNode");
	emplace_back ("X3DParametricGeometryNode");
	emplace_back ("X3DParticleEmitterNode");
	emplace_back ("X3DParticlePhysicsModelNode");
	emplace_back ("X3DPickableObject");
	emplace_back ("X3DPickingNode");
	emplace_back ("X3DPickSensorNode");
	emplace_back ("X3DPointingDeviceSensorNode");
	emplace_back ("X3DProductStructureChildNode");
	emplace_back ("X3DProgrammableShaderObject");
	emplace_back ("X3DPrototypeInstance");
	emplace_back ("X3DRigidJointNode");
	emplace_back ("X3DScriptNode");
	emplace_back ("X3DSensorNode");
	emplace_back ("X3DSequencerNode");
	emplace_back ("X3DShaderNode");
	emplace_back ("X3DShapeNode");
	emplace_back ("X3DSoundNode");
	emplace_back ("X3DSoundSourceNode");
	emplace_back ("X3DTexture2DNode");
	emplace_back ("X3DTexture3DNode");
	emplace_back ("X3DTextureCoordinateNode");
	emplace_back ("X3DTextureNode");
	emplace_back ("X3DTextureTransformNode");
	emplace_back ("X3DTimeDependentNode");
	emplace_back ("X3DTouchSensorNode");
	emplace_back ("X3DTransformNode");
	emplace_back ("X3DTriggerNode");
	emplace_back ("X3DUrlObject");
	emplace_back ("X3DVertexAttributeNode");
	emplace_back ("X3DViewpointNode");
	emplace_back ("X3DViewportNode");
}

const std::string &
Generator::NodeTypesIndex::operator [ ] (const X3DBaseNode* const basicNode) const
{
	return operator [ ] (basicNode -> getNodeType () .back ());
}

const Generator::VrmlAccessTypesIndex Generator::VrmlAccessTypes;
const Generator::AccessTypesIndex     Generator::AccessTypes;
const Generator::NodeTypesIndex       Generator::NodeTypes;

size_t                        Generator::level = 0;
Generator::NodeSet            Generator::nodes;
Generator::NameIndex          Generator::names;
Generator::NameIndexByNode    Generator::namesByNode;
size_t                        Generator::newName = 0;
Generator::ImportedNamesIndex Generator::importedNames;
static const std::string      emptyName;

bool                 Generator::expandNodes = false;
Generator::StyleType Generator::style       = NICEST;

bool Generator::x3dFieldNames  = true;
bool Generator::x3dAccessTypes = true;

void
Generator::Style (const std::string & value)
{
	std::string style = value;

	std::transform (style .begin (), style .end (), style .begin (), ::toupper);

	if (style == "SMALLEST")
		SmallestStyle ();

	if (style == "SMALL")
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

bool
Generator::ExistsNode (const X3DBaseNode* const basicNode)
{
	return nodes .find (basicNode) not_eq nodes .end ();
}

void
Generator::AddNode (const X3DBaseNode* const basicNode)
{
	nodes .emplace (basicNode);
}

const std::string &
Generator::GetName (const X3DBaseNode* const basicNode)
{
	// Is the node already in index

	auto iter = namesByNode .find (basicNode);

	if (iter not_eq namesByNode .end ())
		return iter -> second;

	// The node has no name

	if (basicNode -> getName () .empty ())
	{
		if (basicNode -> getNumClones () > 1 or basicNode -> hasRoutes ())
		{
			std::string name = getUniqueName ();

			names [name]            = basicNode;
			namesByNode [basicNode] = name;

			return namesByNode [basicNode];
		}

		// The node doesn't need a name

		return basicNode -> getName ();
	}

	// The node has a name

	std::string name      = basicNode -> getName ();
	bool        hasNumber = RegEx::_LastNumber .PartialMatch (name);

	RegEx::_LastNumber .Replace ("", &name);

	if (name .empty ())
	{
		if (basicNode -> getNumClones () > 1 or basicNode -> hasRoutes ())
			name = getUniqueName ();

		else
			return emptyName;
	}
	else
	{
		size_t      i       = 0;
		std::string newName = hasNumber ? name + '_' + std::to_string (++ i) : name;

		try
		{
			for ( ; ;)
			{
				names .at (newName);

				newName = name + '_' + std::to_string (++ i);
			}
		}
		catch (const std::out_of_range &)
		{
			name = newName;
		}
	}

	names [name]            = basicNode;
	namesByNode [basicNode] = name;

	return namesByNode [basicNode];
}

std::string
Generator::getUniqueName ()
{
	std::string name;

	try
	{
		for ( ; ;)
		{
			name = '_' + std::to_string (++ newName);

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

	throw Error <INVALID_NODE> ("Couldn't get local name.");
}

} // X3D
} // titania
