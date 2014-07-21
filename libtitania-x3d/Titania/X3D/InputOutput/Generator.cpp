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

Generator::NodeTypeIndex::NodeTypeIndex () :
	array ({
	          // Base node type

	          "X3DBaseNode",

	          // Basic node types

	          "Anchor",
	          "Appearance",
	          "Arc2D",
	          "ArcClose2D",
	          "AudioClip",
	          "Background",
	          "BallJoint",
	          "Billboard",
	          "BooleanFilter",
	          "BooleanSequencer",
	          "BooleanToggle",
	          "BooleanTrigger",
	          "BoundedPhysicsModel",
	          "Box",
	          "CADAssembly",
	          "CADFace",
	          "CADLayer",
	          "CADPart",
	          "Circle2D",
	          "ClipPlane",
	          "CollidableOffset",
	          "CollidableShape",
	          "Collision",
	          "CollisionCollection",
	          "CollisionSensor",
	          "CollisionSpace",
	          "Color",
	          "ColorChaser",
	          "ColorDamper",
	          "ColorInterpolator",
	          "ColorRGBA",
	          "ComposedCubeMapTexture",
	          "ComposedShader",
	          "ComposedTexture3D",
	          "Cone",
	          "ConeEmitter",
	          "Contact",
	          "Contour2D",
	          "ContourPolyline2D",
	          "Coordinate",
	          "CoordinateChaser",
	          "CoordinateDamper",
	          "CoordinateDouble",
	          "CoordinateInterpolator",
	          "CoordinateInterpolator2D",
	          "Cylinder",
	          "CylinderSensor",
	          "DISEntityManager",
	          "DISEntityTypeMapping",
	          "DirectionalLight",
	          "Disk2D",
	          "DoubleAxisHingeJoint",
	          "EaseInEaseOut",
	          "ElevationGrid",
	          "EspduTransform",
	          "ExplosionEmitter",
	          "Extrusion",
	          "FillProperties",
	          "FloatVertexAttribute",
	          "Fog",
	          "FogCoordinate",
	          "FontStyle",
	          "ForcePhysicsModel",
	          "GeneratedCubeMapTexture",
	          "GeoCoordinate",
	          "GeoElevationGrid",
	          "GeoLOD",
	          "GeoLocation",
	          "GeoMetadata",
	          "GeoOrigin",
	          "GeoPositionInterpolator",
	          "GeoProximitySensor",
	          "GeoTouchSensor",
	          "GeoTransform",
	          "GeoViewpoint",
	          "Group",
	          "HAnimDisplacer",
	          "HAnimHumanoid",
	          "HAnimJoint",
	          "HAnimSegment",
	          "HAnimSite",
	          "ImageCubeMapTexture",
	          "ImageTexture",
	          "ImageTexture3D",
	          "IndexedFaceSet",
	          "IndexedLineSet",
	          "IndexedQuadSet",
	          "IndexedTriangleFanSet",
	          "IndexedTriangleSet",
	          "IndexedTriangleStripSet",
	          "Inline",
	          "IntegerSequencer",
	          "IntegerTrigger",
	          "KeySensor",
	          "LOD",
	          "Layer",
	          "LayerSet",
	          "Layout",
	          "LayoutGroup",
	          "LayoutLayer",
	          "LinePickSensor",
	          "LineProperties",
	          "LineSet",
	          "LoadSensor",
	          "LocalFog",
	          "Material",
	          "Matrix3VertexAttribute",
	          "Matrix4VertexAttribute",
	          "MetadataBoolean",
	          "MetadataDouble",
	          "MetadataFloat",
	          "MetadataInteger",
	          "MetadataSet",
	          "MetadataString",
	          "MotorJoint",
	          "MovieTexture",
	          "MultiTexture",
	          "MultiTextureCoordinate",
	          "MultiTextureTransform",
	          "NavigationInfo",
	          "Normal",
	          "NormalInterpolator",
	          "NurbsCurve",
	          "NurbsCurve2D",
	          "NurbsOrientationInterpolator",
	          "NurbsPatchSurface",
	          "NurbsPositionInterpolator",
	          "NurbsSet",
	          "NurbsSurfaceInterpolator",
	          "NurbsSweptSurface",
	          "NurbsSwungSurface",
	          "NurbsTextureCoordinate",
	          "NurbsTrimmedSurface",
	          "OrientationChaser",
	          "OrientationDamper",
	          "OrientationInterpolator",
	          "OrthoViewpoint",
	          "PackagedShader",
	          "ParticleSystem",
	          "PickableGroup",
	          "PixelTexture",
	          "PixelTexture3D",
	          "PlaneSensor",
	          "PointEmitter",
	          "PointLight",
	          "PointPickSensor",
	          "PointSet",
	          "Polyline2D",
	          "PolylineEmitter",
	          "Polypoint2D",
	          "PositionChaser",
	          "PositionChaser2D",
	          "PositionDamper",
	          "PositionDamper2D",
	          "PositionInterpolator",
	          "PositionInterpolator2D",
	          "PrimitivePickSensor",
	          "ProgramShader",
	          "ProximitySensor",
	          "QuadSet",
	          "ReceiverPdu",
	          "Rectangle2D",
	          "RigidBody",
	          "RigidBodyCollection",
	          "ScalarChaser",
	          "ScalarDamper",
	          "ScalarInterpolator",
	          "ScreenFontStyle",
	          "ScreenGroup",
	          "Script",
	          "ShaderPart",
	          "ShaderProgram",
	          "Shape",
	          "SignalPdu",
	          "SingleAxisHingeJoint",
	          "SliderJoint",
	          "Sound",
	          "Sphere",
	          "SphereSensor",
	          "SplinePositionInterpolator",
	          "SplinePositionInterpolator2D",
	          "SplineScalarInterpolator",
	          "SpotLight",
	          "SquadOrientationInterpolator",
	          "StaticGroup",
	          "StringSensor",
	          "SurfaceEmitter",
	          "Switch",
	          "TexCoordChaser2D",
	          "TexCoordDamper2D",
	          "Text",
	          "TextureBackground",
	          "TextureCoordinate",
	          "TextureCoordinate3D",
	          "TextureCoordinate4D",
	          "TextureCoordinateGenerator",
	          "TextureProperties",
	          "TextureTransform",
	          "TextureTransform3D",
	          "TextureTransformMatrix3D",
	          "TimeSensor",
	          "TimeTrigger",
	          "TouchSensor",
	          "Transform",
	          "TransformSensor",
	          "TransmitterPdu",
	          "TriangleFanSet",
	          "TriangleSet",
	          "TriangleSet2D",
	          "TriangleStripSet",
	          "TwoSidedMaterial",
	          "UniversalJoint",
	          "Viewpoint",
	          "ViewpointGroup",
	          "Viewport",
	          "VisibilitySensor",
	          "VolumeEmitter",
	          "VolumePickSensor",
	          "WindPhysicsModel",
	          "WorldInfo",

	          // Abstract node types

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
	          "X3DEnvironmentTextureNode",
	          "X3DEnvironmentalSensorNode",
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
	          "X3DPickSensorNode",
	          "X3DPickableObject",
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
	          "X3DTransformMatrix4DNode",
	          "X3DTransformNode",
	          "X3DTriggerNode",
	          "X3DUrlObject",
	          "X3DVertexAttributeNode",
	          "X3DViewpointNode",
	          "X3DViewpointObject",
	          "X3DViewportNode"
			 })
{ }

const std::string &
Generator::NodeTypeIndex::operator [ ] (const X3DBaseNode* const baseNode) const
{
	return operator [ ] (baseNode -> getType () .back ());
}

const Generator::AccessTypesIndex     Generator::AccessTypes;
const Generator::X3DAccessTypesIndex  Generator::X3DAccessTypes;
const Generator::VrmlAccessTypesIndex Generator::VrmlAccessTypes;
const Generator::NodeTypeIndex   Generator::NodeTypes;

Generator::StyleType Generator::style   = NICEST;
VersionType          Generator::version = LATEST_VERSION;

Generator::ExecutionContextStack Generator::executionContextStack;
size_t                           Generator::level = 0;
Generator::LocalNodeSet          Generator::exportedNodesIndex;
Generator::LocalNodeSet          Generator::importedNodesIndex;
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
Generator::PushExecutionContext (const X3DExecutionContext* const value)
{
	executionContextStack .emplace_back (value);

	exportedNodesIndex .emplace (value, NodeSet ());
	importedNodesIndex .emplace (value, NodeSet ());
}

void
Generator::PopExecutionContext ()
{
	executionContextStack .pop_back ();

	if (executionContextStack .empty ())
	{
		exportedNodesIndex .clear ();
		importedNodesIndex .clear ();
	}
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
	auto & index = exportedNodesIndex .at (executionContextStack .back ());

	for (const auto & exportedNode : exportedNodes)
	{
		try
		{
			index .emplace (exportedNode .second -> getLocalNode ());
		}
		catch (...)
		{ }
	}
}

void
Generator::setImportedNodes (const ImportedNodeIndex & importedNodes)
{
	auto & index = importedNodesIndex .at (executionContextStack .back ());

	for (const auto & importedNode : importedNodes)
	{
		try
		{
			index .emplace (importedNode .second -> getInlineNode ());
		}
		catch (...)
		{ }
	}
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
	return nodes .count (baseNode);
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
	if (baseNode -> isCloned () > 1)
		return true;

	if (baseNode -> hasRoutes ())
		return true;

	try
	{
		const auto & index = exportedNodesIndex .at (baseNode -> getExecutionContext ());

		if (index .count (baseNode))
			return true;
	}
	catch (...)
	{ }

	try
	{
		const auto & index = importedNodesIndex .at (baseNode -> getExecutionContext ());

		if (index .count (baseNode))
			return true;
	}
	catch (...)
	{ }

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
