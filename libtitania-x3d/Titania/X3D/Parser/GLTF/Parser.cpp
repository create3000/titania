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
#include "../../Components/Grouping/Switch.h"

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
	        istream (istream)
{
}

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

	assetObject  (jobj, json_object_object_get (jobj, "asset"));
	scenesObject (jobj, json_object_object_get (jobj, "scenes"));
}

void
Parser::assetObject (json_object* const root, json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	// Create WorldInfo.

	const auto worldInfo = scene -> createNode <X3D::WorldInfo> ();

	worldInfo -> title () = scene -> getWorldURL () .name ();

	scene -> getRootNodes () .emplace_back (worldInfo);

	// Get values.

	std::string versionCharacters;
	std::string generatorCharacters;
	std::string copyrightCharacters;

	if (stringValue (json_object_object_get (jobj, "version"), versionCharacters))
	{
		worldInfo -> info () .emplace_back ("version: " + versionCharacters);
	}

	if (stringValue (json_object_object_get (jobj, "generator"), generatorCharacters))
	{
		worldInfo -> info () .emplace_back ("generator: " + generatorCharacters);
	}

	if (stringValue (json_object_object_get (jobj, "copyright"), copyrightCharacters))
	{
		worldInfo -> info () .emplace_back ("copyright: " + copyrightCharacters);
	}
}

void
Parser::scenesObject (json_object* const root, json_object* const jobj)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	// Create Swtich.

	const auto scenes = scene -> createNode <X3D::Switch> ();

	scene -> addNamedNode (scene -> getUniqueName ("Scenes"), scenes);

	scene -> getRootNodes () .emplace_back (scenes);

	// Set whitchChoice.

	sceneObject (root, json_object_object_get (root, "scene"), scenes);
}

void
Parser::sceneObject (json_object* const root, json_object* const jobj, const X3D::X3DPtr <X3D::Switch> & scenes)
{
	int32_t value = 0;

	if (not integerValue (jobj, value))
		return;

	scenes -> whichChoice () = value;
}

bool
Parser::integerValue (json_object* const jobj, int32_t & value)
{
__LOG__ << jobj << std::endl;
__LOG__ << json_object_get_type (jobj) << std::endl;

	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_int)
	{
		value = 0;
		return true;
	}

	value = json_object_get_int (jobj);

	return true;
}

bool
Parser::stringValue (json_object* const jobj, std::string & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_string)
	{
		value .clear ();
		return true;
	}

	value = json_object_get_string (jobj);

	return true;
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
