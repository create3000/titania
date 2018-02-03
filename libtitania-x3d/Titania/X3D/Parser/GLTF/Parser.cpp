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
#include "../../Components/Shape/Shape.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Parser/Filter.h"

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
	__LOG__ << this << " " << std::endl;

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

	if (integerValue (jobj, mesh))
	{
		const auto & shapeNode = meshes .get1Value (mesh);

		if (shapeNode)
			transformNode -> children () .emplace_back (shapeNode);
		else
			getBrowser () -> getConsole () -> warn ("Mesh with index '", mesh, "' not found.\n");
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

}

void
Parser::bufferViewsObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Buffer View

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto bufferView = std::make_shared <BufferView> ();

		if (bufferViewValue (json_object_array_get_idx (jobj, i), bufferView))
		{
			bufferViews .emplace_back (std::move (bufferView));
		}
		else
		{
			getBrowser () -> getConsole () -> warn ("No valid buffer view found at index '", i, "'.\n");

			bufferViews .emplace_back ();
		}
	}
}

void
Parser::buffersObject (json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Buffer

	const int32_t size = json_object_array_length (jobj);

	for (int32_t i = 0; i < size; ++ i)
	{
		auto buffer = std::make_shared <std::string> ();

		if (bufferValue (json_object_array_get_idx (jobj, i), buffer))
		{
			buffers .emplace_back (std::move (buffer));
		}
		else
		{
			getBrowser () -> getConsole () -> warn ("No valid buffer found at index '", i, "'.\n");
		
			buffers .emplace_back ();
		}
	}
}

bool
Parser::bufferViewValue (json_object* const jobj, const std::shared_ptr <BufferView> & bufferView)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return false;

	// Uri

	int32_t buffer = -1;

	if (integerValue (json_object_object_get (jobj, "buffer"), buffer))
	{
		int32_t byteLength = 0;

		if (integerValue (json_object_object_get (jobj, "byteLength"), byteLength))
		{
			int32_t byteOffset = 0;

			if (integerValue (json_object_object_get (jobj, "byteOffset"), byteOffset))
			{
				int32_t byteStride = 0;

				integerValue (json_object_object_get (jobj, "byteStride"), byteStride);

				try
				{
					bufferView -> buffer     = buffers .at (buffer);
					bufferView -> byteLength = byteLength;
					bufferView -> byteOffset = byteOffset;
					bufferView -> byteStride = byteStride;
					return true;
				}
				catch (const std::out_of_range & error)
				{
					return false;
				}
			}
		}
	}

	return false;
}

bool
Parser::bufferValue (json_object* const jobj, const std::shared_ptr <std::string> & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return false;

	// Uri

	std::string uriCharacters;

	if (stringValue (json_object_object_get (jobj, "uri"), uriCharacters))
	{
		*value = FileLoader (scene) .loadDocument (uriCharacters);
		return true;
	}

	return false;
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
Parser::json_object_object_get (struct json_object* obj, const char *key)
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
