/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#include "Parser.h"

#include "../../Browser/Networking/config.h"
#include "../../Components/Core/WorldInfo.h"
#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Components/Grouping/Group.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Grouping/Switch.h"
#include "../../Components/Interpolation/PositionInterpolator.h"
#include "../../Components/Interpolation/OrientationInterpolator.h"
#include "../../Components/Interpolation/CoordinateInterpolator.h"
#include "../../Components/Interpolation/NormalInterpolator.h"
#include "../../Components/Interpolation/SplinePositionInterpolator.h"
#include "../../Components/Interpolation/SquadOrientationInterpolator.h"
#include "../../Components/Rendering/Color.h"
#include "../../Components/Rendering/ColorRGBA.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedTriangleSet.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Rendering/TriangleSet.h"
#include "../../Components/Shaders/FloatVertexAttribute.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h"
#include "../../Components/Texturing/MultiTextureCoordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Components/Texturing3D/TextureCoordinate3D.h"
#include "../../Components/Texturing3D/TextureCoordinate4D.h"
#include "../../Components/Time/TimeSensor.h"
#include "../../Editing/X3DEditor.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Parser/Filter.h"
#include "../../Prototype/ExternProtoDeclaration.h"

#include <Titania/String.h>

extern "C" {

#include <json-c/json.h>

}

#include <regex>

namespace titania {
namespace X3D {
namespace glTF {

const bool   Parser::USE_PROTOTYPES = false;
const double Parser::EPSILON        = 0.000001;

const std::map <Parser::ComponentType, size_t> Parser::componentSizes = {
	std::pair (ComponentType::BYTE,           sizeof (int8_t)),
	std::pair (ComponentType::UNSIGNED_BYTE,  sizeof (uint8_t)),
	std::pair (ComponentType::SHORT,          sizeof (int16_t)),
	std::pair (ComponentType::UNSIGNED_SHORT, sizeof (uint16_t)),
	std::pair (ComponentType::UNSIGNED_INT,   sizeof (uint32_t)),
	std::pair (ComponentType::FLOAT,          sizeof (float)),
};

const std::map <Parser::ComponentType, std::tuple <double, double, double, double>> Parser::normalizedRanges = {
	std::pair (ComponentType::BYTE,           std::tuple (std::numeric_limits <int8_t>::min (),   std::numeric_limits <int8_t>::max (),  -1, 1)),
	std::pair (ComponentType::UNSIGNED_BYTE,  std::tuple (std::numeric_limits <uint8_t>::min (),  std::numeric_limits <uint8_t>::max (),  0, 1)),
	std::pair (ComponentType::SHORT,          std::tuple (std::numeric_limits <int16_t>::min (),  std::numeric_limits <int16_t>::max (), -1, 1)),
	std::pair (ComponentType::UNSIGNED_SHORT, std::tuple (std::numeric_limits <uint16_t>::min (), std::numeric_limits <uint16_t>::max (), 0, 1)),
	std::pair (ComponentType::UNSIGNED_INT,   std::tuple (std::numeric_limits <uint32_t>::min (), std::numeric_limits <uint32_t>::max (), 0, 1)),
};

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3D::X3DParser (),
	         scene (scene),
	           uri (uri),
	       istream (istream),
	        scenes (),
	         nodes (),
	      samplers (),
	        images (),
	      textures (),
	     materials (),
	        meshes (),
	   bufferViews (),
	       buffers (),
	    extensions ()
{ }

void
Parser::parseIntoScene ()
{
	pushExecutionContext (scene);

	scene -> setWorldURL (uri);
	scene -> setEncoding (EncodingType::XML);
	scene -> setProfile (getBrowser () -> getProfile ("Full"));

	std::ostringstream osstream;

	osstream << istream .rdbuf ();

	const auto string = osstream .str ();

	// Parse JSON.

	const auto jtokener = json_tokener_new_ex (100'000);
	const auto jobj     = json_tokener_parse_ex (jtokener, string .c_str (), string .size ());
	const auto jerror   = json_tokener_get_error (jtokener);

	const std::string jerrorString = json_tokener_error_desc (jerror);

	// Parse scene.

	rootObject (jobj);

	// Delete the JSON object and tokener.

	json_object_put (jobj);
	json_tokener_free (jtokener);

	if (jerror not_eq json_tokener_success)
		throw Error <INVALID_X3D> ("Invalid X3D: " + jerrorString);

	popExecutionContext ();
}

void
Parser::importProto (const std::string & name)
{
	static const std::regex version (R"/(/titania/(?:[\d\.]+|alpha)/)/");

	if (not extensions .emplace (name) .second)
		return;
	
	const auto externprotos = scene -> getExternProtoDeclarations () .size ();
	const auto rootNodes    = scene -> getRootNodes () .size ();
	const auto filename     = get_shader ((USE_PROTOTYPES ? "/glTF/Prototypes/" : "/glTF/") + name + ".x3d");

	FileLoader (scene) .parseIntoScene (scene, { filename .str () });

	scene -> setWorldURL (uri);

	// Update externproto's url field.

	const auto importedExternprotos = X3D::ExternProtoDeclarationArray (scene -> getExternProtoDeclarations () .cbegin () + externprotos,
	                                                                    scene -> getExternProtoDeclarations () .cend ());

	for (const auto & externproto : importedExternprotos)
	{
		externproto -> url () = {
			"http://code.create3000.de/titania/" + getBrowser () -> getVersion () + "/shaders/glTF/Prototypes/" + name + ".x3d#" + name,
			"https://cdn.rawgit.com/create3000/titania/" + getBrowser () -> getVersion () + "/libtitania-x3d/share/titania/shaders/glTF/Prototypes/" + name + ".x3d#" + name,
			"https://rawgit.com/create3000/titania/" + getBrowser () -> getVersion () + "/libtitania-x3d/share/titania/shaders/glTF/Prototypes/" + name + ".x3d#" + name,
			"file:///usr/share/titania/shaders/glTF/Prototypes/" + name + ".x3d#" + name,
			"Prototypes/" + name + ".x3d#" + name,
		};

		externproto -> transform (externproto -> url (), filename, uri);
	}

	// Remove unneccessary nodes.

	const auto importedRootNodes = X3D::MFNode (scene -> getRootNodes () .cbegin () + rootNodes,
	                                            scene -> getRootNodes () .cend ());

	X3D::X3DEditor::removeNodesFromScene (scene, importedRootNodes, true, std::make_shared <X3D::UndoStep> ());
}

void
Parser::rootObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	assetObject       (json_object_object_get (jobj, "asset"));
	buffersObject     (json_object_object_get (jobj, "buffers"));
	bufferViewsObject (json_object_object_get (jobj, "bufferViews"));
	asseccorsObject   (json_object_object_get (jobj, "accessors"));
	samplersObject    (json_object_object_get (jobj, "samplers"));
	imagesObject      (json_object_object_get (jobj, "images"));
	texturesObject    (json_object_object_get (jobj, "textures"));
	materialsObject   (json_object_object_get (jobj, "materials"));
	meshesObject      (json_object_object_get (jobj, "meshes"));
	nodesObject       (json_object_object_get (jobj, "nodes"));
	scenesObject      (json_object_object_get (jobj, "scenes"));
	sceneNumber       (json_object_object_get (jobj, "scene"));
	animationsObject  (json_object_object_get (jobj, "animations"));
	//skinsObject       (json_object_object_get (jobj, "skins"));

	// Refine defines.

	for (const auto & appearanceNode : materials)
	{
		auto & defines = appearanceNode -> getField <X3D::MFString> ("defines");

		std::sort (defines .begin (), defines .end ());

		defines .erase (std::unique (defines .begin (), defines .end ()), defines .end ());
	}
}

void
Parser::assetObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// Create WorldInfo.

	const auto worldInfoNode = scene -> createNode <X3D::WorldInfo> ();

	worldInfoNode -> title () = scene -> getWorldURL () .name ();

	scene -> getRootNodes () .emplace_front (worldInfoNode);

	// Get values.

	std::string versionCharacters;
	std::string generatorCharacters;
	std::string copyrightCharacters;

	if (stringValue (json_object_object_get (jobj, "version"), versionCharacters))
	{
		worldInfoNode -> info () .emplace_back ("version: " + versionCharacters);
	}

	if (stringValue (json_object_object_get (jobj, "generator"), generatorCharacters))
	{
		worldInfoNode -> info () .emplace_back ("generator: " + generatorCharacters);
	}

	if (stringValue (json_object_object_get (jobj, "copyright"), copyrightCharacters))
	{
		worldInfoNode -> info () .emplace_back ("copyright: " + copyrightCharacters);
	}
}

void
Parser::scenesObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Create Swtich.

	scenes = scene -> createNode <X3D::Switch> ();

	scene -> getRootNodes () .emplace_back (scenes);
	scene -> addNamedNode (scene -> getUniqueName ("Scenes"), scenes);

	// Scenes.

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
		sceneObject (json_object_array_get_idx (jobj, i));
}

void
Parser::sceneNumber (json_object* const jobj)
{
	int32_t value = 0;

	if (not integerValue (jobj, value))
		return;

	scenes -> whichChoice () = value;
}

void
Parser::sceneObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// Create groupNode.

	const auto groupNode = scene -> createNode <X3D::Group> ();

	scenes -> children () .emplace_back (groupNode);

	// Name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		if (not nameCharacters .empty ())
			scene -> addNamedNode (scene -> getUniqueName (X3D::GetNameFromString (nameCharacters)), groupNode);
	}

	// Add nodes.

	sceneNodesObject (json_object_object_get (jobj, "nodes"), groupNode);
}

void
Parser::sceneNodesObject (json_object* const jobj, const X3D::X3DPtr <X3D::Group> & groupNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Nodes.

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		int32_t index = 0;

		if (integerValue (json_object_array_get_idx (jobj, i), index))
		{
			try
			{
				const auto node = nodes .at (index);

				if (node)
					groupNode -> children () .emplace_back (node -> transformNode);
			}
			catch (const std::out_of_range & error)
			{
				getBrowser () -> getConsole () -> error ("Node with index '", index, "' not found.\n");
			}
		}
	}
}

void
Parser::nodesObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Nodes.

	// 1st Pass.
	{
		const int32_t size = json_object_array_length (jobj);
	
		for (int32_t i = 0; i < size; ++ i)
		{
			auto node = node1Value (json_object_array_get_idx (jobj, i));

			if (not node)
			{
				getBrowser () -> getConsole () -> warn ("No valid node found at index '", i, "'.\n");
			}

			nodes .emplace_back (std::move (node));
		}
	}

	// 2nd Pass.
	{
		const int32_t size = json_object_array_length (jobj);

		for (int32_t i = 0; i < size; ++ i)
			node2Object (json_object_array_get_idx (jobj, i), nodes [i]);
	}
}

Parser::NodePtr
Parser::node1Value (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Node.

	const auto node = std::make_shared <Node> ();

	// Create Transform.

	const auto transformNode = scene -> createNode <X3D::Transform> ();

	node -> transformNode = transformNode;

	// Name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		if (not nameCharacters .empty ())
			scene -> addNamedNode (scene -> getUniqueName (X3D::GetNameFromString (nameCharacters)), transformNode);
	}

	// Transformation Matrix

	Matrix4d matrix;

	if (not matrix4dValue (json_object_object_get (jobj, "matrix"), matrix))
	{
		Vector3d   translation;
		Rotation4d rotation;
		Vector3d   scale;
	
		if (vector3dValue (json_object_object_get (jobj, "scale"), scale))
		{
			matrix .scale (scale);
		}
	
		if (rotation4dValue (json_object_object_get (jobj, "rotation"), rotation))
		{
			matrix .rotate (rotation);
		}
	
		if (vector3dValue (json_object_object_get (jobj, "translation"), translation))
		{
			matrix .translate (translation);
		}
	}

	transformNode -> setMatrix (matrix);

	// Add mesh.

	int32_t mesh = 0;

	if (integerValue (json_object_object_get (jobj, "mesh"), mesh))
	{
		try
		{
			node -> mesh = meshes .at (mesh);

			if (node -> mesh)
			{
				for (const auto & primitive : node -> mesh -> primitives)
				{
//					if (primitive -> jointGeometryNodes .empty ())
//					{
						transformNode -> children () .emplace_back (primitive -> shapeNode);
//					}
//					else
//					{
//						const auto switchNode = scene -> createNode <X3D::Switch> ();
//
//						switchNode    -> children () .emplace_back (primitive -> shapeNode);
//						transformNode -> children () .emplace_back (switchNode);
//					}
				}
			}
			else
			{
				getBrowser () -> getConsole () -> warn ("No mesh at index '", mesh, "'.\n");
			}
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Mesh with index '", mesh, "' not found.\n");
		}
	}

	// skin

	int32_t skin = -1;

	if (integerValue (json_object_object_get (jobj, "skin"), skin))
	{
		node -> skin = skin;
	}
	else
	{
		node -> skin = -1;
	}

	return node;
}

void
Parser::node2Object (json_object* const jobj, const NodePtr & node)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	if (not node)
		return;

	nodeChildrenObject (json_object_object_get (jobj, "children"), node -> transformNode);
}

void
Parser::nodeChildrenObject (json_object* const jobj, const X3D::X3DPtr <X3D::Transform> & transformNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Children

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		int32_t index = 0;

		if (integerValue (json_object_array_get_idx (jobj, i), index))
		{
			try
			{
				const auto node = nodes .at (index);

				if (node)
					transformNode -> children () .emplace_back (node -> transformNode);
			}
			catch (const std::out_of_range &)
			{
				getBrowser () -> getConsole () -> error ("Node with index '", index, "' not found.\n");
			}
		}
	}
}

void
Parser::meshesObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Meshes

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		const auto mesh = meshArray (json_object_array_get_idx (jobj, i));

		if (not mesh)
		{
			getBrowser () -> getConsole () -> warn ("No valid mesh object found at index '", i, "'.\n");
		}

		meshes .emplace_back (std::move (mesh));
	}
}

Parser::MeshPtr
Parser::meshArray (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Mesh

	const auto mesh = std::make_shared <Mesh> ();

	// Mesh

	mesh -> primitives = primitivesArray (json_object_object_get (jobj, "primitives"));

	return mesh;
}

Parser::PrimitiveArray
Parser::primitivesArray (json_object* const jobj)
{
	PrimitiveArray primitives;

	if (not jobj)
		return primitives;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return primitives;

	// Primitives

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto primitive = primitiveValue (json_object_array_get_idx (jobj, i));

		if (primitive)
		{
			primitives .emplace_back (std::move (primitive));
		}
		else
		{
			getBrowser () -> getConsole () -> warn ("No valid primitive object found at index '", i, "'.\n");
		}
	}

	return primitives;
}

Parser::PrimitivePtr
Parser::primitiveValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// attributes

	auto attributes = attributesValue (json_object_object_get (jobj, "attributes"));

	if (not attributes)
		return nullptr;

	const auto primitive = std::make_shared <Primitive> ();

	primitive -> attributes = std::move (attributes);
	
	// indices

	int32_t indices = -1;

	integerValue (json_object_object_get (jobj, "indices"), indices);

	if (indices > -1 and size_t (indices) < accessors .size ())
	{
		const auto & asseccor = accessors [indices];

		if (not asseccor)
			return nullptr;

		primitive -> indices = asseccor;
	}
	
	// material

	int32_t material = -1;

	if (integerValue (json_object_object_get (jobj, "material"), material))
	{
		try
		{
			primitive -> material = materials .at (material);
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Material with index '", material, "' not found.\n");
		}
	}

	// mode

	int32_t mode = 4;

	integerValue (json_object_object_get (jobj, "mode"), mode);

	primitive -> mode = mode;

	// targets

	primitive -> targets = targetsValue (json_object_object_get (jobj, "targets"));

	// shapeNode

	primitive -> shapeNode = createShape (primitive);

	return primitive;
}

Parser::AttributesPtr
Parser::attributesValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	const auto attribute = std::make_shared <Attributes> ();

	// NORMAL

	int32_t normal = -1;

	if (integerValue (json_object_object_get (jobj, "NORMAL"), normal))
	{
		try
		{
			attribute -> normal = accessors .at (normal);
		}
		catch (const std::out_of_range & error)
		{ }
	}

	// TANGENT

	int32_t tangent = -1;

	if (integerValue (json_object_object_get (jobj, "TANGENT"), tangent))
	{
		try
		{
			attribute -> tangent = accessors .at (tangent);
		}
		catch (const std::out_of_range & error)
		{ }
	}

	// TEXCOORD

	int32_t texCoord = -1;

	for (size_t i = 0; integerValue (json_object_object_get (jobj, "TEXCOORD_" + basic::to_string (i, std::locale::classic ())), texCoord); ++ i, texCoord = -1)
	{
		try
		{
			attribute -> texCoord .emplace_back (accessors .at (texCoord));
		}
		catch (const std::out_of_range & error)
		{
			attribute -> texCoord .emplace_back ();
		}
	}

	// COLOR

	int32_t color = -1;

	for (size_t i = 0; integerValue (json_object_object_get (jobj, "COLOR_" + basic::to_string (i, std::locale::classic ())), color); ++ i, color = -1)
	{
		try
		{
			attribute -> color .emplace_back (accessors .at (color));
		}
		catch (const std::out_of_range & error)
		{
			attribute -> color .emplace_back ();
		}
	}

	// JOINTS

	int32_t joints = -1;

	for (size_t i = 0; integerValue (json_object_object_get (jobj, "JOINTS_" + basic::to_string (i, std::locale::classic ())), joints); ++ i, joints = -1)
	{
		try
		{
			attribute -> joints .emplace_back (accessors .at (joints));
		}
		catch (const std::out_of_range & error)
		{
			attribute -> joints .emplace_back ();
		}
	}

	// WEIGHTS

	int32_t weights = -1;

	for (size_t i = 0; integerValue (json_object_object_get (jobj, "WEIGHTS_" + basic::to_string (i, std::locale::classic ())), weights); ++ i, weights = -1)
	{
		try
		{
			attribute -> weights .emplace_back (accessors .at (weights));
		}
		catch (const std::out_of_range & error)
		{
			attribute -> weights .emplace_back ();
		}
	}

	// POSITION

	int32_t position = -1;

	if (integerValue (json_object_object_get (jobj, "POSITION"), position))
	{
		try
		{
			attribute -> position = accessors .at (position);

			return attribute;
		}
		catch (const std::out_of_range & error)
		{
			return nullptr;
		}
	}

	return nullptr;
}

Parser::AttributesPtrArray
Parser::targetsValue (json_object* const jobj)
{
	if (not jobj)
		return { };

	if (json_object_get_type (jobj) not_eq json_type_array)
		return { };

	// Create Targets

	AttributesPtrArray targets;

	// Targets

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto target = attributesValue (json_object_array_get_idx (jobj, i));

		if (not target)
		{
			getBrowser () -> getConsole () -> warn ("No valid morph target view found at index '", i, "'.\n");
		}

		targets .emplace_back (std::move (target));
	}

	return targets;
}

void
Parser::asseccorsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Asseccors

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto accessor = accessorValue (json_object_array_get_idx (jobj, i));

		if (not accessor)
		{
			getBrowser () -> getConsole () -> warn ("No valid accessor view found at index '", i, "'.\n");
		}

		accessors .emplace_back (std::move (accessor));
	}
}

Parser::AccessorPtr
Parser::accessorValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	static const std::map <std::string, AccessorType> types = {
		std::pair ("SCALAR", AccessorType::SCALAR),
		std::pair ("VEC2",   AccessorType::VEC2),
		std::pair ("VEC3",   AccessorType::VEC3),
		std::pair ("VEC4",   AccessorType::VEC4),
		std::pair ("MAT2",   AccessorType::MAT2),
		std::pair ("MAT3",   AccessorType::MAT3),
		std::pair ("MAT4",   AccessorType::MAT4),
	};

	static const std::map <int32_t, ComponentType> componentTypes = {
		std::pair (5120, ComponentType::BYTE),
		std::pair (5121, ComponentType::UNSIGNED_BYTE),
		std::pair (5122, ComponentType::SHORT),
		std::pair (5123, ComponentType::UNSIGNED_SHORT),
		std::pair (5125, ComponentType::UNSIGNED_INT),
		std::pair (5126, ComponentType::FLOAT),
	};

	int32_t bufferView = -1;

	if (integerValue (json_object_object_get (jobj, "bufferView"), bufferView))
	{
		std::string type;

		if (stringValue (json_object_object_get (jobj, "type"), type))
		{
			int32_t componentType = -1;

			if (integerValue (json_object_object_get (jobj, "componentType"), componentType))
			{
				int32_t byteOffset = 0;

				integerValue (json_object_object_get (jobj, "byteOffset"), byteOffset);

				int32_t count = -1;

				if (integerValue (json_object_object_get (jobj, "count"), count))
				{
					bool normalized = false;

					booleanValue (json_object_object_get (jobj, "normalized"), normalized);

					try
					{
						const auto accessor = std::make_shared <Accessor> ();

						accessor -> bufferView    = bufferViews .at (bufferView);
						accessor -> type          = types .at (type);
						accessor -> componentType = componentTypes .at (componentType);
						accessor -> byteOffset    = byteOffset;
						accessor -> count         = count;
						accessor -> normalized    = normalized;

						return accessor;
					}
					catch (const std::out_of_range & error)
					{
						return nullptr;
					}
				}
			}
		}
	}

	return nullptr;
}

void
Parser::bufferViewsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Buffer Views

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto bufferView = bufferViewValue (json_object_array_get_idx (jobj, i));

		if (not bufferView)
		{
			getBrowser () -> getConsole () -> warn ("No valid buffer view found at index '", i, "'.\n");
		}

		bufferViews .emplace_back (std::move (bufferView));
	}
}

Parser::BufferViewPtr
Parser::bufferViewValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	int32_t buffer = -1;

	if (integerValue (json_object_object_get (jobj, "buffer"), buffer))
	{
		int32_t byteLength = 0;

		if (integerValue (json_object_object_get (jobj, "byteLength"), byteLength))
		{
			int32_t byteOffset = 0;

			integerValue (json_object_object_get (jobj, "byteOffset"), byteOffset);

			int32_t byteStride = 0;

			integerValue (json_object_object_get (jobj, "byteStride"), byteStride);

			try
			{
				const auto bufferView = std::make_shared <BufferView> ();

				bufferView -> buffer     = buffers .at (buffer);
				bufferView -> byteLength = byteLength;
				bufferView -> byteOffset = byteOffset;
				bufferView -> byteStride = byteStride;

				return bufferView;
			}
			catch (const std::out_of_range & error)
			{
				return nullptr;
			}
		}
	}

	return nullptr;
}

void
Parser::buffersObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Buffers

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto buffer = bufferValue (json_object_array_get_idx (jobj, i));

		if (not buffer)
		{
			getBrowser () -> getConsole () -> warn ("No valid buffer found at index '", i, "'.\n");
		}

		buffers .emplace_back (std::move (buffer));
	}
}

Parser::BufferPtr
Parser::bufferValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Uri

	std::string uriCharacters;

	if (stringValue (json_object_object_get (jobj, "uri"), uriCharacters))
	{
		const auto buffer = std::make_shared <Buffer> ();

		buffer -> contents = FileLoader (scene) .loadDocument (uriCharacters);

		return buffer;
	}

	return nullptr;
}

void
Parser::samplersObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Samplers

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto sampler = samplerValue (json_object_array_get_idx (jobj, i));

		if (not sampler)
		{
			getBrowser () -> getConsole () -> warn ("No valid sampler found at index '", i, "'.\n");
		}

		samplers .emplace_back (std::move (sampler));
	}
}

X3D::X3DPtr <X3D::TextureProperties>
Parser::samplerValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Sampler

	const auto texturePropertiesNode = scene -> createNode <X3D::TextureProperties> ();

	texturePropertiesNode -> textureCompression () = "DEFAULT";

	// name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		if (not nameCharacters .empty ())
			scene -> addNamedNode (scene -> getUniqueName (X3D::GetNameFromString (nameCharacters)), texturePropertiesNode);
	}

	// minFilter

	static const std::map <int32_t, std::pair <std::string, bool>> minificationFilters = {
		std::pair (9728, std::pair ("NEAREST_PIXEL",                false)),
		std::pair (9729, std::pair ("AVG_PIXEL",                    false)),
		std::pair (9984, std::pair ("NEAREST_PIXEL_NEAREST_MIPMAP", true)),
		std::pair (9985, std::pair ("AVG_PIXEL_NEAREST_MIPMAP",     true)),
		std::pair (9986, std::pair ("NEAREST_PIXEL_AVG_MIPMAP",     true)),
		std::pair (9987, std::pair ("AVG_PIXEL_AVG_MIPMAP",         true)),
	};

	int32_t minFilter = -1;

	integerValue (json_object_object_get (jobj, "minFilter"), minFilter);

	try
	{
		const auto & minificationFilter = minificationFilters .at (minFilter);

		texturePropertiesNode -> minificationFilter () = minificationFilter .first;
		texturePropertiesNode -> generateMipMaps ()    = minificationFilter .second;
	}
	catch (const std::out_of_range & error)
	{
		texturePropertiesNode -> minificationFilter () = "AVG_PIXEL";
		texturePropertiesNode -> generateMipMaps ()    = false;
	}

	// magFilter

	static const std::map <int32_t, std::string> magnificationFilters = {
		std::pair (9728, "NEAREST_PIXEL"),
		std::pair (9729, "AVG_PIXEL"),
	};

	int32_t magFilter = -1;

	integerValue (json_object_object_get (jobj, "magFilter"), magFilter);

	try
	{
		const auto & magnificationFilter = magnificationFilters .at (magFilter);

		texturePropertiesNode -> magnificationFilter () = magnificationFilter;
	}
	catch (const std::out_of_range & error)
	{
		texturePropertiesNode -> magnificationFilter () = "AVG_PIXEL";
	}

	// boundaryMode

	static const std::map <int32_t, std::string> boundaryModes = {
		std::pair (33071, "CLAMP_TO_EDGE"),
		std::pair (33648, "MIRRORED_REPEAT"),
		std::pair (10497, "REPEAT"),
	};

	// wrapS

	int32_t wrapS = -1;

	integerValue (json_object_object_get (jobj, "wrapS"), wrapS);

	try
	{
		texturePropertiesNode -> boundaryModeS () = boundaryModes .at (wrapS);
	}
	catch (const std::out_of_range & error)
	{
		texturePropertiesNode -> boundaryModeS () = "REPEAT";
	}

	// wrapT

	int32_t wrapT = -1;

	integerValue (json_object_object_get (jobj, "wrapT"), wrapT);

	try
	{
		texturePropertiesNode -> boundaryModeT () = boundaryModes .at (wrapT);
	}
	catch (const std::out_of_range & error)
	{
		texturePropertiesNode -> boundaryModeS () = "REPEAT";
	}

	return texturePropertiesNode;
}

void
Parser::imagesObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Images

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto image = imageValue (json_object_array_get_idx (jobj, i));

		if (not image)
		{
			getBrowser () -> getConsole () -> warn ("No valid image found at index '", i, "'.\n");
		}

		images .emplace_back (std::move (image));
	}
}

Parser::ImagePtr
Parser::imageValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Image

	const auto image = std::make_shared <Image> ();

	// name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		image -> name = nameCharacters;
	}

	// uri

	std::string uriCharacters;

	if (stringValue (json_object_object_get (jobj, "uri"), uriCharacters))
	{
		image -> uri = uriCharacters;
	}

	return image;
}

void
Parser::texturesObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Materials

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto texture = textureValue (json_object_array_get_idx (jobj, i));

		if (not texture)
		{
			getBrowser () -> getConsole () -> warn ("No valid texture found at index '", i, "'.\n");
		}

		textures .emplace_back (std::move (texture));
	}
}

X3D::X3DPtr <X3D::X3DTextureNode>
Parser::textureValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create ImageTexture

	const auto textureNode = scene -> createNode <X3D::ImageTexture> ();

	// sampler

	int32_t sampler = -1;

	if (integerValue (json_object_object_get (jobj, "sampler"), sampler))
	{
		try
		{
			textureNode -> textureProperties () = samplers .at (sampler);
		}
		catch (const std::out_of_range & error)
		{ }
	}

	// source

	int32_t source = -1;

	if (integerValue (json_object_object_get (jobj, "source"), source))
	{
		try
		{
			const auto image = images .at (source);

			if (image)
			{
				if (not image -> name .empty ())
					scene -> addNamedNode (scene -> getUniqueName (X3D::GetNameFromString (image -> name)), textureNode);

				textureNode -> url () = { image -> uri };

				return textureNode;
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}

	return nullptr;
}

void
Parser::materialsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Materials

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto material = materialValue (json_object_array_get_idx (jobj, i));

		if (not material)
		{
			getBrowser () -> getConsole () -> warn ("No valid material found at index '", i, "'.\n");
		}

		materials .emplace_back (std::move (material));
	}
}

X3D::SFNode
Parser::materialValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Material

	const auto appearanceNode = createAppearance ();

	addUninitializedNode (appearanceNode);

	// Name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		if (not nameCharacters .empty ())
			scene -> addNamedNode (scene -> getUniqueName (X3D::GetNameFromString (nameCharacters)), appearanceNode);
	}

	// doubleSided

	bool doubleSided = false;

	if (booleanValue (json_object_object_get (jobj, "doubleSided"), doubleSided))
	{
		appearanceNode -> setMetaData <bool> ("/Titania/doubleSided", doubleSided);
	}

	// alphaMode

	std::string alphaMode = "OPAQUE";

	if (stringValue (json_object_object_get (jobj, "alphaMode"), alphaMode))
	{
		appearanceNode -> setField <X3D::SFString> ("alphaMode", alphaMode);
	}

	// alphaMode

	double alphaCutoff = 0.5;

	if (doubleValue (json_object_object_get (jobj, "alphaCutoff"), alphaCutoff))
	{
		//appearanceNode -> setField <X3D::SFFloat> ("alphaCutoff", float (alphaCutoff));
	}

	// emissiveFactor

	Vector3d emissiveFactor;

	if (vector3dValue (json_object_object_get (jobj, "emissiveFactor"), emissiveFactor))
	{
		appearanceNode -> setField <X3D::SFVec3f> ("emissiveFactor", emissiveFactor);
	}

	// emissiveTextureInfo

	emissiveTextureInfo (json_object_object_get (jobj, "emissiveTexture"), appearanceNode);

	// occlusionTextureInfo

	occlusionTextureInfo (json_object_object_get (jobj, "occlusionTexture"), appearanceNode);

	// normalTextureInfo

	normalTextureInfo (json_object_object_get (jobj, "normalTexture"), appearanceNode);

	// pbrMetallicRoughness

	pbrMetallicRoughness (json_object_object_get (jobj, "pbrMetallicRoughness"), appearanceNode);

	// extensions

	materialExtensions (json_object_object_get (jobj, "extensions"), appearanceNode);

	return appearanceNode;
}

void
Parser::emissiveTextureInfo (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// index

	int32_t index = -1;

	if (integerValue (json_object_object_get (jobj, "index"), index))
	{
		try
		{
			const auto & texture = textures .at (index);

			if (texture)
			{
				appearanceNode -> setField <X3D::SFNode> ("emissiveTexture", texture);
				appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_EMISSIVE_MAP");
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
Parser::occlusionTextureInfo (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// scale

	double strength = 1;

	if (doubleValue (json_object_object_get (jobj, "strength"), strength))
	{
		appearanceNode -> setField <X3D::SFFloat> ("occlusionStrength", float (strength));
	}

	// index

	int32_t index = -1;

	if (integerValue (json_object_object_get (jobj, "index"), index))
	{
		try
		{
			const auto & texture = textures .at (index);

			if (texture)
			{
				appearanceNode -> setField <X3D::SFNode> ("occlusionTexture", texture);
				appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_OCCLUSION_MAP");
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
Parser::normalTextureInfo (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// scale

	double scale = 1;

	if (doubleValue (json_object_object_get (jobj, "scale"), scale))
	{
		appearanceNode -> setField <X3D::SFFloat> ("normalScale", float (scale));
	}

	// index

	int32_t index = -1;

	if (integerValue (json_object_object_get (jobj, "index"), index))
	{
		try
		{
			const auto & texture = textures .at (index);

			if (texture)
			{
				appearanceNode -> setField <X3D::SFNode> ("normalTexture", texture);
				appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_NORMAL_MAP");
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
Parser::pbrMetallicRoughness (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// Create pbrMetallicRoughness

	const auto metallicRoughness = createMetallicRoughness ();

	appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("KHR_materials_pbrMetallicRoughness");
	appearanceNode -> setField <X3D::SFNode> ("metallicRoughness", metallicRoughness);

	// baseColorFactor

	Vector4d baseColorFactor (1, 1, 1, 1);

	if (vector4dValue (json_object_object_get (jobj, "baseColorFactor"), baseColorFactor))
	{
		metallicRoughness -> setField <X3D::SFVec4f> ("baseColorFactor", Vector4f (baseColorFactor));
	}

	// metallicFactor

	double metallicFactor = 1;

	if (doubleValue (json_object_object_get (jobj, "metallicFactor"), metallicFactor))
	{
		metallicRoughness -> setField <X3D::SFFloat> ("metallicFactor", float (metallicFactor));
	}

	// roughnessFactor

	double roughnessFactor = 1;

	if (doubleValue (json_object_object_get (jobj, "roughnessFactor"), roughnessFactor))
	{
		metallicRoughness -> setField <X3D::SFFloat> ("roughnessFactor", float (roughnessFactor));
	}

	// baseColorTextureInfo

	const auto baseColorTexture = textureInfoValue (json_object_object_get (jobj, "baseColorTexture"));

	if (baseColorTexture)
		appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_BASE_COLOR_MAP");

	metallicRoughness -> setField <X3D::SFNode> ("baseColorTexture", baseColorTexture);

	// metallicRoughnessTextureInfo

	const auto metallicRoughnessTexture = textureInfoValue (json_object_object_get (jobj, "metallicRoughnessTexture"));

	if (metallicRoughnessTexture)
		appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_METAL_ROUGHNESS_MAP");

	metallicRoughness -> setField <X3D::SFNode> ("metallicRoughnessTexture", metallicRoughnessTexture);
}

void
Parser::materialExtensions (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// KHR_materials_pbrSpecularGlossiness

	pbrSpecularGlossiness (json_object_object_get (jobj, "KHR_materials_pbrSpecularGlossiness"), appearanceNode);
}

void
Parser::pbrSpecularGlossiness (json_object* const jobj, const X3D::SFNode & appearanceNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// Create pbrMetallicRoughness

	const auto specularGlossiness = createSpecularGlossiness ();

	appearanceNode -> getField <X3D::MFString> ("defines")  .emplace_back ("KHR_materials_pbrSpecularGlossiness");
	appearanceNode -> getField <X3D::MFNode> ("extensions") .emplace_back (specularGlossiness);

	// baseColorFactor

	Vector4d diffuseFactor (1, 1, 1, 1);

	if (vector4dValue (json_object_object_get (jobj, "diffuseFactor"), diffuseFactor))
	{
		specularGlossiness -> setField <X3D::SFVec4f> ("diffuseFactor", Vector4f (diffuseFactor));
	}

	// baseColorFactor

	Vector3d specularFactor (1, 1, 1);

	if (vector3dValue (json_object_object_get (jobj, "specularFactor"), specularFactor))
	{
		specularGlossiness -> setField <X3D::SFVec3f> ("specularFactor", Vector3f (specularFactor));
	}

	// metallicFactor

	double glossinessFactor = 1;

	if (doubleValue (json_object_object_get (jobj, "glossinessFactor"), glossinessFactor))
	{
		specularGlossiness -> setField <X3D::SFFloat> ("glossinessFactor", float (glossinessFactor));
	}

	// diffuseTextureInfo

	const auto diffuseTexture = textureInfoValue (json_object_object_get (jobj, "diffuseTexture"));

	if (diffuseTexture)
		appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_DIFFUSE_MAP");

	specularGlossiness -> setField <X3D::SFNode> ("diffuseTexture", diffuseTexture);

	// specularGlossinessTexture

	const auto specularGlossinessTexture = textureInfoValue (json_object_object_get (jobj, "specularGlossinessTexture"));

	if (specularGlossinessTexture)
		appearanceNode -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_SPECULAR_GLOSSINESS_MAP");

	specularGlossiness -> setField <X3D::SFNode> ("specularGlossinessTexture", specularGlossinessTexture);
}

X3D::X3DPtr <X3D::X3DTextureNode>
Parser::textureInfoValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// index

	int32_t index = -1;

	if (integerValue (json_object_object_get (jobj, "index"), index))
	{
		try
		{
			return textures .at (index);
		}
		catch (const std::out_of_range & error)
		{
			return nullptr;
		}
	}

	return nullptr;
}

X3D::X3DPtr <X3D::Shape>
Parser::createShape (const PrimitivePtr & primitive) const
{
	const auto shapeNode      = scene -> createNode <X3D::Shape> ();
	const auto appearanceNode = primitive -> material ? primitive -> material : createAppearance ();
	const auto geometryNode   = createGeometry (primitive, appearanceNode);

	appearanceNode -> removeMetaData ("/Titania");

	shapeNode -> appearance () = appearanceNode;
	shapeNode -> geometry ()   = geometryNode;

	return shapeNode;
}

X3D::X3DPtr <X3D::X3DNode>
Parser::createAppearance () const
{
	const_cast <Parser*> (this) -> importProto ("pbrAppearance");

	const auto instance = scene -> createProto ("pbrAppearance");

	const_cast <Parser*> (this) -> addUninitializedNode (instance);

	instance -> getField <X3D::MFString> ("defines") .emplace_back ("MANUAL_SRGB");

	return instance;
}

X3D::SFNode
Parser::createMetallicRoughness () const
{
	const_cast <Parser*> (this) -> importProto ("pbrMetallicRoughness");

	const auto instance = scene -> createProto ("pbrMetallicRoughness");

	const_cast <Parser*> (this) -> addUninitializedNode (instance);

	return instance;
}

X3D::SFNode
Parser::createSpecularGlossiness () const
{
	const_cast <Parser*> (this) -> importProto ("pbrSpecularGlossiness");

	const auto instance = scene -> createProto ("pbrSpecularGlossiness");

	const_cast <Parser*> (this) -> addUninitializedNode (instance);

	return instance;
}

X3D::X3DPtr <X3D::X3DGeometryNode>
Parser::createGeometry (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const
{
	switch (primitive -> mode)
	{
		case 0: // POINTS
		{
			return nullptr;
		}
		case 1: // LINES
		{
			return nullptr;
		}
		case 2: // LINE_LOOP
		{
			return nullptr;
		}
		case 3: // LINE_STRIP
		{
			return nullptr;
		}
		case 4: // TRIANGLES
		{
			if (primitive -> indices)
				return createIndexedTriangleSet (primitive, material);

			return createTriangleSet (primitive, material);
		}
		case 5: // TRIANGLE_STRIP
		{
			return nullptr;
		}
		case 6: // TRIANGLE_FAN
		{
			return nullptr;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::IndexedTriangleSet>
Parser::createIndexedTriangleSet (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const
{
	const auto geometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();
	const auto attributes   = primitive -> attributes;
	const auto indices      = getScalarArray <int32_t> (primitive -> indices);
	const auto tangent      = createTangent (attributes -> tangent);

	geometryNode -> index () .assign (indices .cbegin (), indices .cend ());

	geometryNode -> solid ()    = not material -> getMetaData <bool> ("doubleSided");
	geometryNode -> coord ()    = createCoordinate (attributes -> position);
	geometryNode -> normal ()   = createNormal (attributes -> normal);
	geometryNode -> texCoord () = createTextureCoordinate (attributes -> texCoord);

	if (tangent)
	{
		geometryNode -> attrib () .emplace_back (tangent);
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_TANGENTS");
	}

	material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_NORMALS");

	if (not attributes -> color .empty ())
	{
		geometryNode -> color () = createColor (attributes -> color [0]);
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_COLORS");
	}

	if (geometryNode -> texCoord ())
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_TEXCOORDS");

	geometryNode -> normalPerVertex () = geometryNode -> normal ();

	// Joints

	for (const auto & attribute : attributes -> joints)
	{
		try
		{
			const auto joints = getScalarArray <int32_t> (attribute);
	
			std::map <int32_t, std::vector <int32_t>> map;
	
			for (size_t i = 0, size = indices .size () - indices .size () % 3; i < size; i += 3)
			{
				const auto joint = joints .at (indices [i]);

				map [joint] .emplace_back (indices [i + 0]);
				map [joint] .emplace_back (indices [i + 1]);
				map [joint] .emplace_back (indices [i + 2]);
			}

			for (const auto & pair : map)
			{
				const auto jointGeometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();
		
				for (const auto index : pair .second)
					jointGeometryNode -> index () .emplace_back (index);
	
				jointGeometryNode -> solid ()           = geometryNode -> solid ();
				jointGeometryNode -> coord ()           = geometryNode -> coord ();
				jointGeometryNode -> normal ()          = geometryNode -> normal ();
				jointGeometryNode -> texCoord ()        = geometryNode -> texCoord ();
				jointGeometryNode -> attrib ()          = geometryNode -> attrib ();
				jointGeometryNode -> color ()           = geometryNode -> color ();
				jointGeometryNode -> normalPerVertex () = geometryNode -> normalPerVertex ();
	
				primitive -> jointGeometryNodes .emplace (pair .first, jointGeometryNode);
			}
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> warn ("Invalid joint array.\n");
		}
	}

	return geometryNode;
}

X3D::X3DPtr <X3D::TriangleSet>
Parser::createTriangleSet (const PrimitivePtr & primitive, const X3D::X3DPtr <X3D::X3DNode> & material) const
{
	const auto geometryNode = scene -> createNode <X3D::TriangleSet> ();
	const auto attributes   = primitive -> attributes;
	const auto tangent      = createTangent (attributes -> tangent);

	geometryNode -> solid ()    = not material -> getMetaData <bool> ("doubleSided");
	geometryNode -> coord ()    = createCoordinate (attributes -> position);
	geometryNode -> normal ()   = createNormal (attributes -> normal);
	geometryNode -> texCoord () = createTextureCoordinate (attributes -> texCoord);

	if (tangent)
	{
		geometryNode -> attrib () .emplace_back (tangent);
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_TANGENTS");
	}

	material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_NORMALS");

	if (not attributes -> color .empty ())
	{
		geometryNode -> color () = createColor (attributes -> color [0]);
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_COLORS");
	}

	if (geometryNode -> texCoord ())
		material -> getField <X3D::MFString> ("defines") .emplace_back ("HAS_TEXCOORDS");

	// Joints

	for (const auto & attribute : attributes -> joints)
	{
		const auto joints = getScalarArray <int32_t> (attribute);

		std::map <int32_t, std::vector <int32_t>> map;

		for (size_t index = 0, size = joints .size () - joints .size () % 3; index < size; index += 3)
		{
			const auto joint = joints [index];

			map [joint] .emplace_back (index + 0);
			map [joint] .emplace_back (index + 1);
			map [joint] .emplace_back (index + 2);
		}

		for (const auto & pair : map)
		{
			const auto jointGeometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();
	
			for (const auto index : pair .second)
				jointGeometryNode -> index () .emplace_back (index);

			jointGeometryNode -> solid ()    = geometryNode -> solid ();
			jointGeometryNode -> coord ()    = geometryNode -> coord ();
			jointGeometryNode -> texCoord () = geometryNode -> texCoord ();
			jointGeometryNode -> attrib ()   = geometryNode -> attrib ();
			jointGeometryNode -> color ()    = geometryNode -> color ();

			primitive -> jointGeometryNodes .emplace (pair .first, jointGeometryNode);
		}
	}
		
	return geometryNode;
}

X3D::X3DPtr <X3D::Coordinate>
Parser::createCoordinate (const AccessorPtr & accessor) const
{
	if (not accessor)
		return nullptr;

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	auto & points = coordinateNode -> point ();

	switch (accessor -> type)
	{
		case AccessorType::VEC2:
		{
			const auto array = getVectorArray <Vector2d> (accessor);

			if (array .empty ())
				return nullptr;

			for (const auto & value : array)
				points .emplace_back (value [0], value [1], 0);

			return coordinateNode;
		}
		case AccessorType::VEC3:
		{
			const auto array = getVectorArray <Vector3d> (accessor);

			if (array .empty ())
				return nullptr;

			for (const auto & value : array)
				points .emplace_back (value);

			return coordinateNode;
		}
		case AccessorType::VEC4:
		{
			const auto array = getVectorArray <Vector4d> (accessor);

			if (array .empty ())
				return nullptr;

			for (const auto & value : array)
				points .emplace_back (value [0] / value [3], value [1] / value [3], value [2] / value [3]);

			return coordinateNode;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::FloatVertexAttribute>
Parser::createTangent (const AccessorPtr & accessor) const
{
	// Implementation note: When tangents are not specified, client implementations should calculate tangents using
	// default MikkTSpace algorithms. For best results, the mesh triangles should also be processed using default
	// MikkTSpace algorithms.

	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC4:
		{
			const auto array = getVectorArray <Vector4d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto attribNode = scene -> createNode <X3D::FloatVertexAttribute> ();
			auto &     attrib     = attribNode -> value ();

			attribNode -> name ()          = "tangent";
			attribNode -> numComponents () = 4;

			for (const auto & value : array)
			{
				for (const auto & component : value)
					attrib .emplace_back (component);
			}

			return attribNode;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::Normal>
Parser::createNormal (const AccessorPtr & accessor) const
{
	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC3:
		{
			const auto array = getVectorArray <Vector3d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto normalNode = scene -> createNode <X3D::Normal> ();
			auto &     vector     = normalNode -> vector ();

			for (const auto & value : array)
				vector .emplace_back (value);

			return normalNode;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::X3DTextureCoordinateNode>
Parser::createTextureCoordinate (const AccessorPtrArray & accessors) const
{
	switch (accessors .size ())
	{
		case 0:
		{
			return nullptr;
		}
		case 1:
		{
			return createSingleTextureCoordinate (accessors [0]);
		}
		default:
		{
			const auto textureCoordinateNode = scene -> createNode <X3D::MultiTextureCoordinate> ();

			for (const auto & accessor : accessors)
				textureCoordinateNode -> texCoord () .emplace_back (createSingleTextureCoordinate (accessor));

			return textureCoordinateNode;
		}
	}

	return nullptr;
}

X3D::X3DPtr <X3D::X3DTextureCoordinateNode>
Parser::createSingleTextureCoordinate (const AccessorPtr & accessor) const
{
	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC2:
		{
			const auto array = getVectorArray <Vector2d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate> ();
			auto &     points                = textureCoordinateNode -> point ();

			for (const auto & value : array)
				points .emplace_back (value);

			return textureCoordinateNode;
		}
		case AccessorType::VEC3:
		{
			const auto array = getVectorArray <Vector3d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate3D> ();
			auto &     points                = textureCoordinateNode -> point ();

			for (const auto & value : array)
				points .emplace_back (value);

			return textureCoordinateNode;
		}
		case AccessorType::VEC4:
		{
			const auto array = getVectorArray <Vector4d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate4D> ();
			auto &     points                = textureCoordinateNode -> point ();

			for (const auto & value : array)
				points .emplace_back (value);

			return textureCoordinateNode;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::X3DColorNode>
Parser::createColor (const AccessorPtr & accessor) const
{
	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC3:
		{
			const auto array = getVectorArray <Vector3d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto colorNode = scene -> createNode <X3D::Color> ();
			auto &     color     = colorNode -> color ();

			for (const auto & value : array)
				color .emplace_back (value .x (), value .y (), value .z ());

			return colorNode;
		}
		case AccessorType::VEC4:
		{
			const auto array = getVectorArray <Vector4d> (accessor);

			if (array .empty ())
				return nullptr;

			const auto transparent = std::any_of (array .cbegin (),
			                                      array .cend (),
			                                      [ ] (const Vector4d & value) { return value .w () < 1; });	  

			if (transparent)
			{
				const auto colorNode = scene -> createNode <X3D::ColorRGBA> ();
				auto &     color     = colorNode -> color ();
	
				for (const auto & value : array)
					color .emplace_back (value .x (), value .y (), value .z (), value .w ());
	
				return colorNode;
			}
			else
			{
				const auto colorNode = scene -> createNode <X3D::Color> ();
				auto &     color     = colorNode -> color ();

				for (const auto & value : array)
					color .emplace_back (value .x (), value .y (), value .z ());
	
				return colorNode;
			}
		}
		default:
			return nullptr;
	}

	return nullptr;
}

void
Parser::animationsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Create Animations Group

	const auto animations = scene -> createNode <X3D::Group> ();

	scene -> addNamedNode (scene -> getUniqueName ("Animations"), animations);
	scene -> getRootNodes () .emplace_back (animations);

	// Animations

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto animation = animationValue (json_object_array_get_idx (jobj, i));

		if (not animation)
		{
			getBrowser () -> getConsole () -> warn ("No valid animation found at index '", i, "'.\n");
		}
		else
		{
			animations -> children () .emplace_back (std::move (animation));
		}
	}
}

X3D::X3DPtr <X3D::Group>
Parser::animationValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Animation

	const auto animation = scene -> createNode <X3D::Group> ();

	// name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
		if (not nameCharacters .empty ())
			scene -> addNamedNode (scene -> getUniqueName (nameCharacters), animation);
	}

	// samplers

	const auto samplers = animationSamplersValue (json_object_object_get (jobj, "samplers"));

	// channels

	animationChannelsObject (json_object_object_get (jobj, "channels"), animation, samplers);

	return animation;
}

void
Parser::animationChannelsObject (json_object* const jobj, const X3D::X3DPtr <X3D::Group> & animation, const AnimationSamplerArray & samplers)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Animations

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto animationChannel = animationChannelValue (json_object_array_get_idx (jobj, i), samplers);

		if (not animationChannel)
		{
			getBrowser () -> getConsole () -> warn ("No valid animation channel found at index '", i, "'.\n");
		}
		else
		{
			scene -> addNamedNode (scene -> getUniqueName ("Channel" + basic::to_string (i, std::locale::classic ())), animationChannel);

			animation -> children () .emplace_back (animationChannel);
		}
	}
}

X3D::X3DPtr <X3D::Group>
Parser::animationChannelValue (json_object* const jobj, const AnimationSamplerArray & samplers)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// target

	const auto animationTarget = animationTargetValue (json_object_object_get (jobj, "target"));

	if (not animationTarget)
		return nullptr;

	const auto node = animationTarget -> node;

	// Create Group

	const auto groupNode = scene -> createNode <X3D::Group> ();

	// Create TimeSensor

	const auto timeSensorNode = scene -> createNode <X3D::TimeSensor> ();

	timeSensorNode -> loop () = true;

	groupNode -> children () .emplace_back (timeSensorNode);

	// input

	int32_t sampler = -1;

	if (integerValue (json_object_object_get (jobj, "sampler"), sampler))
	{
		try
		{
			const auto animationSampler = samplers .at (sampler);

			if (not animationSampler)
				return nullptr;

			const auto times = getScalarArray <double> (animationSampler -> input);

			if (times .empty ())
				return nullptr;

			timeSensorNode -> cycleInterval () = times .back ();

			switch (animationTarget -> path)
			{
				case PathType::TRANSLATION:
				{
					const auto translationInterpolator = createTranslationInterpolator (times, animationSampler, timeSensorNode, node -> transformNode);

					if (translationInterpolator)
						groupNode -> children () .emplace_back (translationInterpolator);

					break;
				}
				case PathType::ROTATION:
				{
					const auto rotationInterpolator = createRotationInterpolator (times, animationSampler, timeSensorNode, node -> transformNode);

					if (rotationInterpolator)
						groupNode -> children () .emplace_back (rotationInterpolator);

					break;
				}
				case PathType::SCALE:
				{
					const auto scaleInterpolator = createScaleInterpolator (times, animationSampler, timeSensorNode, node -> transformNode);

					if (scaleInterpolator)
						groupNode -> children () .emplace_back (scaleInterpolator);

					break;
				}
				case PathType::WEIGHTS:
				{
					if (not node -> mesh)
						return nullptr;

					for (const auto & primitive : node -> mesh -> primitives)
					{
						const auto & shapeNode    = primitive -> shapeNode;
						const auto   geometryNode = X3D::X3DPtr <X3D::X3DGeometryNode> (shapeNode -> geometry ());

						if (not geometryNode)
							continue;

						const auto coordinateInterpolatorNode = createCoordinateInterpolator (primitive -> targets, timeSensorNode, geometryNode);

						if (coordinateInterpolatorNode)
							groupNode -> children () .emplace_back (coordinateInterpolatorNode);

						const auto normalInterpolatorNode = createNormalInterpolator (primitive -> targets, timeSensorNode, geometryNode);

						if (normalInterpolatorNode)
							groupNode -> children () .emplace_back (normalInterpolatorNode);
					}

					break;
				}
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}

	return groupNode;
}

Parser::AnimationTargetPtr
Parser::animationTargetValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;
 
	// Create Animation Target

	const auto animationTarget = std::make_shared <AnimationTarget> ();

	// path

	static const std::map <std::string, PathType> pathTypes = {
		std::pair ("translation", PathType::TRANSLATION),
		std::pair ("rotation",    PathType::ROTATION),
		std::pair ("scale",       PathType::SCALE),
		std::pair ("weights",     PathType::WEIGHTS),
	};

	std::string path;

	if (stringValue (json_object_object_get (jobj, "path"), path))
	{
		try
		{
			animationTarget -> path = pathTypes .at (path);
		}
		catch (const std::out_of_range & error)
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	// node

	int32_t node = -1;

	if (integerValue (json_object_object_get (jobj, "node"), node))
	{
		try
		{
			animationTarget -> node = nodes .at (node);

			if (not animationTarget -> node)
				return nullptr;
		}
		catch (const std::out_of_range & error)
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return animationTarget;
}

Parser::AnimationSamplerArray
Parser::animationSamplersValue (json_object* const jobj)
{
	if (not jobj)
		return { };

	if (json_object_get_type (jobj) not_eq json_type_array)
		return { };

	// Create Animation Samplers

	AnimationSamplerArray animationSamplers;

	// Animations

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto animationSampler = animationSamplerValue (json_object_array_get_idx (jobj, i));

		if (not animationSampler)
		{
			getBrowser () -> getConsole () -> warn ("No valid animation sampler found at index '", i, "'.\n");
		}

		animationSamplers .emplace_back (std::move (animationSampler));
	}

	return animationSamplers;
}

Parser::AnimationSamplerPtr
Parser::animationSamplerValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Animation Sampler

	const auto animationSampler = std::make_shared <AnimationSampler> ();

	// interpolation

	static const std::map <std::string, InterpolationType> interpolationTypes = {
		std::pair ("LINEAR",      InterpolationType::LINEAR),
		std::pair ("STEP",        InterpolationType::STEP),
		std::pair ("CUBICSPLINE", InterpolationType::CUBICSPLINE),
	};

	std::string interpolation = "LINEAR";

	if (stringValue (json_object_object_get (jobj, "interpolation"), interpolation))
	{
		try
		{
			animationSampler -> interpolation = interpolationTypes .at (interpolation);
		}
		catch (const std::out_of_range & error)
		{
			animationSampler -> interpolation = InterpolationType::LINEAR;
		}
	}

	// input

	int32_t input = -1;

	if (integerValue (json_object_object_get (jobj, "input"), input))
	{
		try
		{
			animationSampler -> input = accessors .at (input);
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Invalid animation input accessor found.\n");
			return nullptr;
		}
	}
	else
	{
		getBrowser () -> getConsole () -> warn ("Invalid animation input accessor found.\n");
		return nullptr;
	}

	// output

	int32_t output = -1;

	if (integerValue (json_object_object_get (jobj, "output"), output))
	{
		try
		{
			animationSampler -> output = accessors .at (output);
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Invalid animation output accessor found.\n");
			return nullptr;
		}
	}
	else
	{
		getBrowser () -> getConsole () -> warn ("Invalid animation output accessor found.\n");
		return nullptr;
	}

	return animationSampler;
}

X3D::X3DPtr <X3D::X3DInterpolatorNode>
Parser::createTranslationInterpolator (const std::vector <double> & times,
                                       const AnimationSamplerPtr & animationSampler,
                                       const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                       const X3D::X3DPtr <X3D::Transform> & transformNode)
{
	switch (animationSampler -> interpolation)
	{
		case InterpolationType::LINEAR:
		{
			const auto interpolatorNode = scene -> createNode <X3D::PositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);
		
			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (keyValue);
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_translation");
		
			return interpolatorNode;
		}
		case InterpolationType::STEP:
		{
			const auto interpolatorNode = scene -> createNode <X3D::PositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			if (not times .empty ())
			{
				interpolatorNode -> key () .emplace_back (times .front () / cycleInterval);

				for (size_t i = 0, size = times .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval - EPSILON);
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval);
				}

				if (times .size () > 1)
					interpolatorNode -> key () .emplace_back (times .back () / cycleInterval);
			}

			if (not keyValues .empty ())
			{
				interpolatorNode -> keyValue () .emplace_back (keyValues .front ());

				for (size_t i = 0, size = keyValues .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> keyValue () .emplace_back (keyValues [i - 1]);
					interpolatorNode -> keyValue () .emplace_back (keyValues [i]);
				}

				if (keyValues .size () > 1)
					interpolatorNode -> keyValue () .emplace_back (keyValues .back ());
			}
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_translation");
		
			return interpolatorNode;
		}
		case InterpolationType::CUBICSPLINE:
		{
			const auto interpolatorNode = scene -> createNode <X3D::SplinePositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();
		
			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);

			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (keyValue);
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_translation");
		
			return interpolatorNode;
		}
	}

	return nullptr;
}

X3D::X3DPtr <X3D::X3DInterpolatorNode>
Parser::createRotationInterpolator (const std::vector <double> & times,
                                    const AnimationSamplerPtr & animationSampler,
                                    const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                    const X3D::X3DPtr <X3D::Transform> & transformNode)
{
	switch (animationSampler -> interpolation)
	{
		case InterpolationType::LINEAR:
		{
			const auto interpolatorNode = scene -> createNode <X3D::OrientationInterpolator> ();
			const auto keyValues        = getVectorArray <Vector4d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);

			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (getRotation (keyValue));

			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_rotation");
		
			return interpolatorNode;
		}
		case InterpolationType::STEP:
		{
			const auto interpolatorNode = scene -> createNode <X3D::OrientationInterpolator> ();
			const auto keyValues        = getVectorArray <Vector4d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			if (not times .empty ())
			{
				interpolatorNode -> key () .emplace_back (times .front () / cycleInterval);

				for (size_t i = 0, size = times .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval - EPSILON);
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval);
				}

				if (times .size () > 1)
					interpolatorNode -> key () .emplace_back (times .back () / cycleInterval);
			}

			if (not keyValues .empty ())
			{
				interpolatorNode -> keyValue () .emplace_back (getRotation (keyValues .front ()));

				for (size_t i = 0, size = keyValues .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> keyValue () .emplace_back (getRotation (keyValues [i - 1]));
					interpolatorNode -> keyValue () .emplace_back (getRotation (keyValues [i]));
				}

				if (keyValues .size () > 1)
					interpolatorNode -> keyValue () .emplace_back (getRotation (keyValues .back ()));
			}
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_rotation");
		
			return interpolatorNode;
		}
		case InterpolationType::CUBICSPLINE:
		{
			const auto interpolatorNode = scene -> createNode <X3D::SquadOrientationInterpolator> ();
			const auto keyValues        = getVectorArray <Vector4d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();
		
			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);

			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (getRotation (keyValue));
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_rotation");
		
			return interpolatorNode;
		}
	}

	return nullptr;
}

X3D::X3DPtr <X3D::X3DInterpolatorNode>
Parser::createScaleInterpolator (const std::vector <double> & times,
                                 const AnimationSamplerPtr & animationSampler,
                                 const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                 const X3D::X3DPtr <X3D::Transform> & transformNode)
{
	switch (animationSampler -> interpolation)
	{
		case InterpolationType::LINEAR:
		{
			const auto interpolatorNode = scene -> createNode <X3D::PositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);
		
			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (keyValue);
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_scale");
		
			return interpolatorNode;
		}
		case InterpolationType::STEP:
		{
			const auto interpolatorNode = scene -> createNode <X3D::PositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();

			if (not times .empty ())
			{
				interpolatorNode -> key () .emplace_back (times .front () / cycleInterval);

				for (size_t i = 0, size = times .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval - EPSILON);
					interpolatorNode -> key () .emplace_back (times [i] / cycleInterval);
				}

				if (times .size () > 1)
					interpolatorNode -> key () .emplace_back (times .back () / cycleInterval);
			}

			if (not keyValues .empty ())
			{
				interpolatorNode -> keyValue () .emplace_back (keyValues .front ());

				for (size_t i = 0, size = keyValues .size () - 1; i < size; ++ i)
				{
					interpolatorNode -> keyValue () .emplace_back (keyValues [i - 1]);
					interpolatorNode -> keyValue () .emplace_back (keyValues [i]);
				}

				if (keyValues .size () > 1)
					interpolatorNode -> keyValue () .emplace_back (keyValues .back ());
			}
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_scale");
		
			return interpolatorNode;
		}
		case InterpolationType::CUBICSPLINE:
		{
			const auto interpolatorNode = scene -> createNode <X3D::SplinePositionInterpolator> ();
			const auto keyValues        = getVectorArray <Vector3d> (animationSampler -> output);
			const auto cycleInterval    = times .back ();
		
			for (const auto t : times)
				interpolatorNode -> key () .emplace_back (t / cycleInterval);

			for (const auto keyValue : keyValues)
				interpolatorNode -> keyValue () .emplace_back (keyValue);
			
			scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
			scene -> addRoute (interpolatorNode, "value_changed", transformNode, "set_scale");
		
			return interpolatorNode;
		}
	}

	return nullptr;
}

X3D::X3DPtr <X3D::CoordinateInterpolator>
Parser::createCoordinateInterpolator (const AttributesPtrArray & targets,
                                      const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                      const X3D::X3DPtr <X3D::X3DGeometryNode> & geometryNode) const
{
	try
	{
		auto coordinateNode = X3D::X3DPtr <X3D::Coordinate> (geometryNode -> getField <X3D::SFNode> ("coord"));
	
		if (not coordinateNode)
		{
			coordinateNode = scene -> createNode <X3D::Coordinate> ();

			if (not targets .empty ())
			{
				if (not targets .front () -> position)
					return nullptr;
	
				const auto array = getVectorArray <Vector3d> (targets .front () -> position);
	
				for (const auto & value : array)
					coordinateNode -> point () .emplace_back (value);
			}

			geometryNode -> getField <X3D::SFNode> ("coord") = coordinateNode;
		}

		const auto interpolatorNode = scene -> createNode <X3D::CoordinateInterpolator> ();

		for (size_t i = 0, size = targets .size (); i < size; ++ i)
		{
			interpolatorNode -> key () .emplace_back (float (i) / float (size - 1));
		}

		for (const auto & target : targets)
		{
			if (not target -> position)
				return nullptr;

			const auto array = getVectorArray <Vector3d> (target -> position);

			for (const auto & value : array)
				interpolatorNode -> keyValue () .emplace_back (value);
		}
	
		scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
		scene -> addRoute (interpolatorNode, "value_changed", coordinateNode, "set_point");

		return interpolatorNode;
	}
	catch (const X3D::X3DError & error)
	{
		getBrowser () -> getConsole () -> error (error .what (), "\n");
		return nullptr;
	}
}

X3D::X3DPtr <X3D::NormalInterpolator>
Parser::createNormalInterpolator (const AttributesPtrArray & targets,
                                  const X3D::X3DPtr <X3D::TimeSensor> & timeSensorNode,
                                  const X3D::X3DPtr <X3D::X3DGeometryNode> & geometryNode) const
{
	try
	{
		auto normalNode = X3D::X3DPtr <X3D::Normal> (geometryNode -> getField <X3D::SFNode> ("normal"));

		if (not normalNode)
		{
			normalNode = scene -> createNode <X3D::Normal> ();

			if (not targets .empty ())
			{
				if (not targets .front () -> normal)
					return nullptr;
	
				const auto array = getVectorArray <Vector3d> (targets .front () -> normal);
	
				for (const auto & value : array)
					normalNode -> vector () .emplace_back (value);
			}

			geometryNode -> getField <X3D::SFNode> ("normal") = normalNode;
		}

		const auto interpolatorNode = scene -> createNode <X3D::NormalInterpolator> ();

		for (size_t i = 0, size = targets .size (); i < size; ++ i)
		{
			interpolatorNode -> key () .emplace_back (float (i) / float (size - 1));
		}

		for (const auto & target : targets)
		{
			if (not target -> normal)
				return nullptr;

			const auto array = getVectorArray <Vector3d> (target -> normal);

			for (const auto & value : array)
				interpolatorNode -> keyValue () .emplace_back (value);
		}

		scene -> addRoute (timeSensorNode, "fraction_changed", interpolatorNode, "set_fraction");
		scene -> addRoute (interpolatorNode, "value_changed", normalNode, "set_vector");

		return interpolatorNode;
	}
	catch (const X3D::X3DError & error)
	{
		getBrowser () -> getConsole () -> error (error .what (), "\n");
		return nullptr;
	}
}

void
Parser::skinsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Create skin array.

	auto skins = SkinPtrArray ();

	// Skins

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto skin = skinValue (json_object_array_get_idx (jobj, i));

		if (not skin)
		{
			getBrowser () -> getConsole () -> warn ("No valid skin found at index '", i, "'.\n");
		}

		skins .emplace_back (std::move (skin));
	}

	/// Node skins

	for (const auto & node : nodes)
	{
		try
		{	
			if (node -> skin == -1)
				continue;

			if (not node -> mesh)
				continue;

			const auto skin = skins .at (node -> skin);

			if (not skin)
				continue;

			for (const auto & primitive : node -> mesh -> primitives)
			{
				const auto & shapeNode = primitive -> shapeNode;

				for (const auto & pair : primitive -> jointGeometryNodes)
				{
					try
					{
						const auto   joint              = pair .first;
						const auto   jointNode          = skin -> joints .at (joint);
						const auto & jointGeometryNode  = pair .second;
						const auto   jointShapeNode     = scene -> createNode <X3D::Shape> ();
						const auto   jointTransformNode = scene -> createNode <X3D::Transform> ();

						if (joint >= 0 and joint < int32_t (skin -> inverseBindMatrices .size ()))
							jointTransformNode -> setMatrix (skin -> inverseBindMatrices [joint]);

						jointShapeNode -> appearance () = shapeNode -> appearance ();
						jointShapeNode -> geometry ()   = jointGeometryNode;

						jointTransformNode -> children () .emplace_back (jointShapeNode);
						jointNode -> transformNode -> children () .emplace_back (jointTransformNode);
					}
					catch (const std::out_of_range & error)
					{
						getBrowser () -> getConsole () -> error ("No valid joint for mesh found.\n");
					}
				}
			}
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("No valid skin for node found.\n");
		}
	}
}

Parser::SkinPtr
Parser::skinValue (json_object* const jobj)
{
	if (not jobj)
		return nullptr;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return nullptr;

	// Create Animation

	const auto skin = std::make_shared <Skin> ();

	// inverseBindMatrices

	int32_t inverseBindMatrices = -1;

	if (integerValue (json_object_object_get (jobj, "inverseBindMatrices"), inverseBindMatrices))
	{
		try
		{
			skin -> inverseBindMatrices = getMatrixArray <Matrix4d> (accessors .at (inverseBindMatrices));
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Invalid inverseBindMatrices accessor found.\n");
		}
	}

	// joints

	skin -> joints = jointsValue (json_object_object_get (jobj, "joints"));

	// inverseBindMatrices

	int32_t skeleton = -1;

	if (integerValue (json_object_object_get (jobj, "skeleton"), skeleton))
	{
		try
		{
			skin -> skeleton = nodes .at (skeleton);
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Invalid skeleton node found.\n");
		}
	}

	return skin;
}

Parser::NodePtrArray
Parser::jointsValue (json_object* const jobj)
{
	if (not jobj)
		return { };

	if (json_object_get_type (jobj) not_eq json_type_array)
		return { };

	// Create skin array.

	auto joints = NodePtrArray ();

	// Skins

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		try
		{
			int32_t node = -1;
	
			integerValue (json_object_array_get_idx (jobj, i), node);

			joints .emplace_back (nodes .at (node));
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> error ("Invalid joint node found.\n");
			joints .emplace_back ();
		}
	}

	return joints;
}

bool
Parser::booleanValue (json_object* const jobj, bool & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_boolean)
		return false;

	value = json_object_get_boolean (jobj);

	return true;
}

bool
Parser::doubleValue (json_object* const jobj, double & value)
{
	if (not jobj)
		return false;

	switch (json_object_get_type (jobj))
	{
		case json_type_int:
			value = json_object_get_int (jobj);
			return true;

		case json_type_double:
			value = json_object_get_double (jobj);
			return true;

		default:
			return false;
	}

	return false;
}

bool
Parser::integerValue (json_object* const jobj, int32_t & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_int)
		return false;

	value = json_object_get_int (jobj);

	return true;
}

bool
Parser::matrix4dValue (json_object* const jobj, Matrix4d & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return false;

	double e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (doubleValue (json_object_array_get_idx (jobj, 0), e11))
	{
		if (doubleValue (json_object_array_get_idx (jobj, 1), e12))
		{
			if (doubleValue (json_object_array_get_idx (jobj, 2), e13))
			{
				if (doubleValue (json_object_array_get_idx (jobj, 3), e14))
				{
					if (doubleValue (json_object_array_get_idx (jobj, 4), e21))
					{
						if (doubleValue (json_object_array_get_idx (jobj, 5), e22))
						{
							if (doubleValue (json_object_array_get_idx (jobj, 6), e23))
							{
								if (doubleValue (json_object_array_get_idx (jobj, 7), e24))
								{
									if (doubleValue (json_object_array_get_idx (jobj, 8), e31))
									{
										if (doubleValue (json_object_array_get_idx (jobj, 9), e32))
										{
											if (doubleValue (json_object_array_get_idx (jobj, 10), e33))
											{
												if (doubleValue (json_object_array_get_idx (jobj, 11), e34))
												{
													if (doubleValue (json_object_array_get_idx (jobj, 12), e41))
													{
														if (doubleValue (json_object_array_get_idx (jobj, 13), e42))
														{
															if (doubleValue (json_object_array_get_idx (jobj, 14), e43))
															{
																if (doubleValue (json_object_array_get_idx (jobj, 15), e44))
																{
																	value = Matrix4d (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44);
																	return true;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool
Parser::rotation4dValue (json_object* const jobj, Rotation4d & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return false;

	double x, y, z, w;

	if (doubleValue (json_object_array_get_idx (jobj, 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, 2), z))
			{
				if (doubleValue (json_object_array_get_idx (jobj, 3), w))
				{
					value = Rotation4d (Quaternion4d (x, y, z, w));
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::stringValue (json_object* const jobj, std::string & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_string)
		return false;

	value = json_object_get_string (jobj);

	return true;
}

bool
Parser::vector3dValue (json_object* const jobj, Vector3d & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return false;

	double x, y, z;

	if (doubleValue (json_object_array_get_idx (jobj, 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, 2), z))
			{
				value = Vector3d (x, y, z);
				return true;
			}
		}
	}

	return false;
}

bool
Parser::vector4dValue (json_object* const jobj, Vector4d & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return false;

	double x, y, z, w;

	if (doubleValue (json_object_array_get_idx (jobj, 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, 2), z))
			{
				if (doubleValue (json_object_array_get_idx (jobj, 3), w))
				{
					value = Vector4d (x, y, z, w);
					return true;
				}
			}
		}
	}

	return false;
}

Rotation4d
Parser::getRotation (const Vector4d & value)
{
	return Rotation4d (Quaternion4d (value .x (),
	                                 value .y (),
	                                 value .z (),
	                                 value .w ()));
}

struct json_object*
Parser::json_object_object_get (struct json_object* obj, const std::string & key)
{
	return json_object_object_get (obj, key .c_str ());
}

struct json_object*
Parser::json_object_object_get (struct json_object* obj, const char* key)
{
	struct json_object* value;

	if (json_object_object_get_ex (obj, key, &value))
		return value;

	return nullptr;
}

Parser::~Parser ()
{ }

} // glTF
} // X3D
} // titania
