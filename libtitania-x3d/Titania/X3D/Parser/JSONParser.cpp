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

#include "JSONParser.h"

#include "../Browser/X3DBrowser.h"

extern "C" {

#include <json-c/json.h>

}

namespace titania {
namespace X3D {

JSONParser::JSONParser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	  scene (scene),
	    uri (uri),
	istream (istream)
{ }

void
JSONParser::parseIntoScene ()
{
	__LOG__ << this << " " << std::endl;

	scene -> setWorldURL (uri);

	std::ostringstream osstream;

	osstream << istream .rdbuf ();

	// Parse JSON.

	enum json_tokener_error jerror;

	json_object* const jobj = json_tokener_parse_verbose (osstream .str () .c_str (), &jerror);

	if (jerror not_eq json_tokener_success)
	{
		const std::string jerrorString = json_tokener_error_desc (jerror);

		throw Error <INVALID_X3D> ("Invalid X3D: " + jerrorString);
	}

	//

	jsonObject (jobj);

	// Delete the JSON object.

	json_object_put (jobj);
}

void
JSONParser::jsonObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	x3dObject (json_object_object_get (jobj, "X3D"));
}

void
JSONParser::x3dObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	std::string encodingCharacters;
	std::string profileCharacters;
	std::string specificationVersionCharacters;

	if (encodingString (json_object_object_get (jobj, "encoding"), encodingCharacters))
	{
		// TODO
	}

	if (profileString (json_object_object_get (jobj, "@profile"), profileCharacters))
	{
		scene -> setProfile (getBrowser () -> getProfile (profileCharacters));
	}

	if (versionString (json_object_object_get (jobj, "@version"), specificationVersionCharacters))
	{
		scene -> setSpecificationVersion (specificationVersionCharacters);
	}

	headObject (json_object_object_get (jobj, "head"));

	sceneObject (json_object_object_get (jobj, "Scene"));
}

bool
JSONParser::encodingString (json_object* const jobj, std::string & encodingCharacters)
{
	return stringValue (jobj, encodingCharacters);
}

bool
JSONParser::profileString (json_object* const jobj, std::string & profileCharacters)
{
	return stringValue (jobj, profileCharacters);
}

bool
JSONParser::versionString (json_object* const jobj, std::string & specificationVersionCharacters)
{
	return stringValue (jobj, specificationVersionCharacters);
}

void
JSONParser::headObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	componentArray (json_object_object_get (jobj, "component"));
	unitArray      (json_object_object_get (jobj, "unit"));
	metaArray      (json_object_object_get (jobj, "meta"));
}

void
JSONParser::componentArray (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
		componentObject (json_object_array_get_idx (jobj, i));
}

void
JSONParser::componentObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	std::string componentNameCharacters;

	if (componentNameString (json_object_object_get (jobj, "@name"), componentNameCharacters))
	{
		int32_t componentLevel = 0;

		if (componentLevelNumber (json_object_object_get (jobj, "@level"), componentLevel))
		{
			

			return;
		}

		//throw Error <INVALID_X3D> ("Expected a component support level.");
	}	

	//throw Error <INVALID_X3D> ("Expected a component name.");
}

bool
JSONParser::componentNameString (json_object* const jobj, std::string & componentNameCharacters)
{
	return stringValue (jobj, componentNameCharacters);
}

bool
JSONParser::componentLevelNumber (json_object* const jobj, int32_t & componentLevelNumber)
{
	return integerValue (jobj, componentLevelNumber);
}

void
JSONParser::unitArray (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

}

void
JSONParser::metaArray (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

}

void
JSONParser::sceneObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

}

bool
JSONParser::integerValue (json_object* const jobj, int32_t & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_int)
		return false;

	value = json_object_get_int (jobj);

	return true;
}

bool
JSONParser::stringValue (json_object* const jobj, std::string & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_string)
		return false;

	value = json_object_get_string (jobj);

	return true;
}

JSONParser::~JSONParser ()
{ }

} // X3D

} // titania
