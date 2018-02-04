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

#include "../../Components/Core/WorldInfo.h"
#include "../../Components/Grouping/Group.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Grouping/Switch.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedTriangleSet.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Rendering/TriangleSet.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/MultiTextureCoordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing3D/TextureCoordinate3D.h"
#include "../../Components/Texturing3D/TextureCoordinate4D.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Parser/Filter.h"

#include <Titania/String.h>

extern "C" {

#include <json-c/json.h>

}

namespace titania {
namespace X3D {
namespace GLTF {

//accessors
//asset
//bufferViews
//buffers
//images
//materials
//meshes
//nodes
//samplers
//scene
//scenes
//textures
////animation

const std::map <Parser::ComponentType, size_t> Parser::componentSizes = {
	std::make_pair (ComponentType::BYTE,           sizeof (int8_t)),
	std::make_pair (ComponentType::UNSIGNED_BYTE,  sizeof (uint8_t)),
	std::make_pair (ComponentType::SHORT,          sizeof (int16_t)),
	std::make_pair (ComponentType::UNSIGNED_SHORT, sizeof (uint16_t)),
	std::make_pair (ComponentType::UNSIGNED_INT,   sizeof (uint32_t)),
	std::make_pair (ComponentType::FLOAT,          sizeof (float)),
};

const std::map <Parser::ComponentType, std::tuple <double, double, double, double>> Parser::normalizedRanges = {
	std::make_pair (ComponentType::BYTE,           std::make_tuple (std::numeric_limits <int8_t>::min (),   std::numeric_limits <int8_t>::max (),  -1, 1)),
	std::make_pair (ComponentType::UNSIGNED_BYTE,  std::make_tuple (std::numeric_limits <uint8_t>::min (),  std::numeric_limits <uint8_t>::max (),  0, 1)),
	std::make_pair (ComponentType::SHORT,          std::make_tuple (std::numeric_limits <int16_t>::min (),  std::numeric_limits <int16_t>::max (), -1, 1)),
	std::make_pair (ComponentType::UNSIGNED_SHORT, std::make_tuple (std::numeric_limits <uint16_t>::min (), std::numeric_limits <uint16_t>::max (), 0, 1)),
	std::make_pair (ComponentType::UNSIGNED_INT,   std::make_tuple (std::numeric_limits <uint32_t>::min (), std::numeric_limits <uint32_t>::max (), 0, 1)),
};

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	X3D::X3DParser (),
	         scene (scene),
	           uri (uri),
	       istream (istream),
	        scenes (),
	         nodes (),
	        meshes (),
	   bufferViews (),
	       buffers ()
{ }

void
Parser::parseIntoScene ()
{
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
	meshesObject      (json_object_object_get (jobj, "meshes"));
	nodesObject       (json_object_object_get (jobj, "nodes"));
	scenesObject      (json_object_object_get (jobj, "scenes"));
	sceneNumber       (json_object_object_get (jobj, "scene"));
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

	scene -> getRootNodes () .emplace_back (worldInfoNode);

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
				groupNode -> children () .emplace_back (nodes .at (index));
			}
			catch (const std::out_of_range & error)
			{
				getBrowser () -> getConsole () -> warn ("Node with index '", index, "' not found.\n");
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
			node1Object (json_object_array_get_idx (jobj, i));
	}

	// 2nd Pass.
	{
		const int32_t size = json_object_array_length (jobj);

		for (int32_t i = 0; i < size; ++ i)
			node2Object (json_object_array_get_idx (jobj, i), nodes .get1Value (i));
	}
}

void
Parser::node1Object (json_object* const jobj)
{
	if (not jobj)
	{
		nodes .emplace_back ();
		return;
	}

	if (json_object_get_type (jobj) not_eq json_type_object)
	{
		nodes .emplace_back ();
		return;
	}

	// Create Transform.

	const auto transformNode = scene -> createNode <X3D::Transform> ();

	nodes .emplace_back (transformNode);

	// Name

	std::string nameCharacters;

	if (stringValue (json_object_object_get (jobj, "name"), nameCharacters))
	{
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
			transformNode -> children () = meshes .at (mesh);
		}
		catch (const std::out_of_range & error)
		{
			getBrowser () -> getConsole () -> warn ("Mesh with index '", mesh, "' not found.\n");
		}
	}
}

void
Parser::node2Object (json_object* const jobj, const X3D::X3DPtr <X3D::Transform> & transformNode)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	if (not transformNode)
		return;

	nodeChildrenObject (json_object_object_get (jobj, "children"), transformNode);
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
				transformNode -> children () .emplace_back (nodes .at (index));
			}
			catch (const std::out_of_range &)
			{
				getBrowser () -> getConsole () -> warn ("Node with index '", index, "' not found.\n");
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

		if (mesh .empty ())
		{
			getBrowser () -> getConsole () -> warn ("No valid mesh object found at index '", i, "'.\n");
		}

		meshes .emplace_back (std::move (mesh));
	}
}

X3D::X3DPtrArray <X3D::Shape>
Parser::meshArray (json_object* const jobj)
{
	X3D::X3DPtrArray <X3D::Shape> shapeNodes;

	if (not jobj)
		return shapeNodes;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return shapeNodes;

	// Mesh

	const auto primitives = primitivesArray (json_object_object_get (jobj, "primitives"));

	for (const auto & primitive : primitives)
	{
		const auto shapeNode = createShape (primitive);

		if (shapeNode)
			shapeNodes .emplace_back (shapeNode);
	}

	return shapeNodes;
}

X3D::X3DPtr <X3D::Shape>
Parser::createShape (const PrimitivePtr & primitive)
{
	const auto shapeNode      = scene -> createNode <X3D::Shape> ();
	const auto appearanceNode = scene -> createNode <X3D::Appearance> ();
	const auto materialNode   = scene -> createNode <X3D::Material> ();

	shapeNode -> appearance ()    = appearanceNode;
	appearanceNode -> material () = materialNode;

	switch (primitive -> mode)
	{
		case 0: // POINTS
		{
			break;
		}
		case 1: // LINES
		{
			break;
		}
		case 2: // LINE_LOOP
		{
			break;
		}
		case 3: // LINE_STRIP
		{
			break;
		}
		case 4: // TRIANGLES
		{
			if (primitive -> indices)
				shapeNode -> geometry () = createIndexedTriangleSet (primitive);
			else
				shapeNode -> geometry () = createTriangleSet (primitive);

			break;
		}
		case 5: // TRIANGLE_STRIP
		{
			break;
		}
		case 6: // TRIANGLE_FAN
		{
			break;
		}
		default:
			return nullptr;
	}

	return shapeNode;
}

X3D::X3DPtr <X3D::IndexedTriangleSet>
Parser::createIndexedTriangleSet (const PrimitivePtr & primitive)
{
	const auto geometryNode = scene -> createNode <X3D::IndexedTriangleSet> ();
	const auto indices      = getScalarArray (primitive -> indices);

	geometryNode -> index () .assign (indices .begin (), indices .end ());

	geometryNode -> coord ()    = createCoordinate (primitive -> attributes -> position);
	geometryNode -> normal ()   = createNormal (primitive -> attributes -> normal);
	geometryNode -> texCoord () = createTextureCoordinate (primitive -> attributes -> texCoord);

	return geometryNode;
}

X3D::X3DPtr <X3D::TriangleSet>
Parser::createTriangleSet (const PrimitivePtr & primitive)
{
	const auto geometryNode = scene -> createNode <X3D::TriangleSet> ();

	geometryNode -> coord ()    = createCoordinate (primitive -> attributes -> position);
	geometryNode -> normal ()   = createNormal (primitive -> attributes -> normal);
	geometryNode -> texCoord () = createTextureCoordinate (primitive -> attributes -> texCoord);

	return geometryNode;
}

X3D::X3DPtr <X3D::Coordinate>
Parser::createCoordinate (const AccessorPtr & accessor)
{
	if (not accessor)
		return nullptr;

	const auto coordinateNode = scene -> createNode <X3D::Coordinate> ();

	auto & points = coordinateNode -> point ();

	switch (accessor -> type)
	{
		case AccessorType::VEC2:
		{
			const auto array = getVec2Array (accessor);

			for (const auto & value : array)
				points .emplace_back (value [0], value [1], 0);

			return coordinateNode;
		}
		case AccessorType::VEC3:
		{
			const auto array = getVec3Array (accessor);

			for (const auto & value : array)
				points .emplace_back (value);

			return coordinateNode;
		}
		case AccessorType::VEC4:
		{
			const auto array = getVec4Array (accessor);

			for (const auto & value : array)
				points .emplace_back (value [0] / value [3], value [1] / value [3], value [2] / value [3]);

			return coordinateNode;
		}
		default:
			return nullptr;
	}

	return nullptr;
}

X3D::X3DPtr <X3D::Normal>
Parser::createNormal (const AccessorPtr & accessor)
{
	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC3:
		{
			const auto normalNode = scene -> createNode <X3D::Normal> ();
			const auto array      = getVec3Array (accessor);
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
Parser::createTextureCoordinate (const AccessorPtrArray & accessors)
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
Parser::createSingleTextureCoordinate (const AccessorPtr & accessor)
{
	if (not accessor)
		return nullptr;

	switch (accessor -> type)
	{
		case AccessorType::VEC2:
		{
			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate> ();
			const auto array                 = getVec2Array (accessor);
			auto &     points                = textureCoordinateNode -> point ();

			for (const auto & value : array)
				points .emplace_back (value);

			return textureCoordinateNode;
		}
		case AccessorType::VEC3:
		{
			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate3D> ();
			const auto array                 = getVec3Array (accessor);
			auto &     points                = textureCoordinateNode -> point ();

			for (const auto & value : array)
				points .emplace_back (value);

			return textureCoordinateNode;
		}
		case AccessorType::VEC4:
		{
			const auto textureCoordinateNode = scene -> createNode <X3D::TextureCoordinate4D> ();
			const auto array                 = getVec4Array (accessor);
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

	// Attributes

	auto attributes = attributesValue (json_object_object_get (jobj, "attributes"));

	if (not attributes)
		return nullptr;

	const auto primitive = std::make_shared <Primitive> ();

	primitive -> attributes = std::move (attributes);
	
	// Indices

	int32_t indices = -1;

	integerValue (json_object_object_get (jobj, "indices"), indices);

	if (indices > -1 and size_t (indices) < accessors .size ())
	{
		const auto & asseccor = accessors [indices];

		if (not asseccor)
			return nullptr;

		primitive -> indices = asseccor;
	}
	
	// Mode

	int32_t mode = 4;

	integerValue (json_object_object_get (jobj, "mode"), mode);

	primitive -> mode = mode;

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
		std::make_pair ("SCALAR", AccessorType::SCALAR),
		std::make_pair ("VEC2",   AccessorType::VEC2),
		std::make_pair ("VEC3",   AccessorType::VEC3),
		std::make_pair ("VEC4",   AccessorType::VEC4),
		std::make_pair ("MAT2",   AccessorType::MAT2),
		std::make_pair ("MAT3",   AccessorType::MAT3),
		std::make_pair ("MAT4",   AccessorType::MAT4),
	};

	static const std::map <int32_t, ComponentType> componentTypes = {
		std::make_pair (5120, ComponentType::BYTE),
		std::make_pair (5121, ComponentType::UNSIGNED_BYTE),
		std::make_pair (5122, ComponentType::SHORT),
		std::make_pair (5123, ComponentType::UNSIGNED_SHORT),
		std::make_pair (5125, ComponentType::UNSIGNED_INT),
		std::make_pair (5126, ComponentType::FLOAT),
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

std::vector <double>
Parser::getScalarArray (const AccessorPtr & accessor)
{
	static constexpr size_t components = 1;

	std::vector <double> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto data = reinterpret_cast <const int8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto data = reinterpret_cast <const uint8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto data = reinterpret_cast <const int16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto data = reinterpret_cast <const uint16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto data = reinterpret_cast <const uint32_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto data = reinterpret_cast <const float*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0]);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range = normalizedRanges .at (componentType);

				for (auto & value : array)
					value = project <double> (value, std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
	
				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

std::vector <Vector2d>
Parser::getVec2Array (const AccessorPtr & accessor)
{
	static constexpr size_t components = 2;

	std::vector <Vector2d> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto data = reinterpret_cast <const int8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto data = reinterpret_cast <const uint8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto data = reinterpret_cast <const int16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto data = reinterpret_cast <const uint16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto data = reinterpret_cast <const uint32_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto data = reinterpret_cast <const float*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1]);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range = normalizedRanges .at (componentType);

				for (auto & value : array)
				{
					value [0] = project <double> (value [0], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [1] = project <double> (value [1], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
				}

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

std::vector <Vector3d>
Parser::getVec3Array (const AccessorPtr & accessor)
{
	static constexpr size_t components = 3;

	std::vector <Vector3d> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto data = reinterpret_cast <const int8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto data = reinterpret_cast <const uint8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto data = reinterpret_cast <const int16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto data = reinterpret_cast <const uint16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto data = reinterpret_cast <const uint32_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto data = reinterpret_cast <const float*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2]);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range = normalizedRanges .at (componentType);

				for (auto & value : array)
				{
					value [0] = project <double> (value [0], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [1] = project <double> (value [1], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [2] = project <double> (value [2], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
				}

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
}

std::vector <Vector4d>
Parser::getVec4Array (const AccessorPtr & accessor)
{
	static constexpr size_t components = 4;

	std::vector <Vector4d> array;

	const auto bufferView    = accessor -> bufferView;
	const auto byteOffset    = accessor -> byteOffset + bufferView -> byteOffset;
	const auto componentType = accessor -> componentType;
	const auto componentSize = componentSizes .at (componentType);
	const auto count         = accessor -> count;
	const auto normalized    = accessor -> normalized;
	const auto buffer        = bufferView -> buffer;
	const auto byteStride    = bufferView -> byteStride;
	const auto stride        = std::max <size_t> (components, byteStride / componentSize);
	const auto first         = buffer -> contents .data () + byteOffset;
	const auto last          = first + byteStride * (count - 1) + componentSize * components;
	const auto bufferFirst   = buffer -> contents .data ();
	const auto bufferLast    = buffer -> contents .data () + buffer -> contents .size ();

	if (first < bufferFirst or first >= bufferLast)
		return array;

	if (last < bufferFirst or last >= bufferLast)
		return array;

	switch (componentType)
	{
		case ComponentType::BYTE:
		{
			auto data = reinterpret_cast <const int8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
		case ComponentType::UNSIGNED_BYTE:
		{
			auto data = reinterpret_cast <const uint8_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
		case ComponentType::SHORT:
		{
			auto data = reinterpret_cast <const int16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
		case ComponentType::UNSIGNED_SHORT:
		{
			auto data = reinterpret_cast <const uint16_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
		case ComponentType::UNSIGNED_INT:
		{
			auto data = reinterpret_cast <const uint32_t*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
		case ComponentType::FLOAT:
		{
			auto data = reinterpret_cast <const float*> (first);

			for (int32_t i = 0; i < count; ++ i, data += stride)
			{
				array .emplace_back (data [0], data [1], data [2], data [3]);
			}

			break;
		}
	}

	if (normalized)
	{
		switch (componentType)
		{
			case ComponentType::BYTE:
			case ComponentType::UNSIGNED_BYTE:
			case ComponentType::SHORT:
			case ComponentType::UNSIGNED_SHORT:
			case ComponentType::UNSIGNED_INT:
			{
				const auto & range = normalizedRanges .at (componentType);

				for (auto & value : array)
				{
					value [0] = project <double> (value [0], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [1] = project <double> (value [1], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [2] = project <double> (value [2], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
					value [4] = project <double> (value [4], std::get <0> (range), std::get <1> (range), std::get <2> (range), std::get <3> (range));
				}

				break;
			}
			case ComponentType::FLOAT:
				break;
		}
	}

	return array;
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

} // GLTF
} // X3D
} // titania
