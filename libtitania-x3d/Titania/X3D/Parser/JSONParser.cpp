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
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Parser/Filter.h"

extern "C" {

#include <json-c/json.h>

}

namespace titania {
namespace X3D {

JSONParser::JSONParser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	executionContextStack ()
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

	pushExecutionContext (scene);

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

	popExecutionContext ();
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
		int32_t componentSupportLevel = 0;

		if (componentSupportLevelNumber (json_object_object_get (jobj, "@level"), componentSupportLevel))
		{
			const auto component = getBrowser () -> getComponent (componentNameCharacters, componentSupportLevel);

			scene -> updateComponent (component);
			return;
		}

		getBrowser () -> println ("Expected a component support level.");
	}

	getBrowser () -> println ("Expected a component name.");
}

void
JSONParser::unitArray (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
		unitObject (json_object_array_get_idx (jobj, i));
}

void
JSONParser::unitObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	std::string categoryNameCharacters;

	if (categoryNameString (json_object_object_get (jobj, "@category"), categoryNameCharacters))
	{
		std::string unitNameCharacters;

		if (unitNameString (json_object_object_get (jobj, "@name"), unitNameCharacters))
		{
			double unitConversionFactor;

			if (unitConversionFactorNumber (json_object_object_get (jobj, "@conversionFactor"), unitConversionFactor))
			{
				try
				{
					scene -> updateUnit (categoryNameCharacters, unitNameCharacters, unitConversionFactor);
					return;
				}
				catch (const X3DError & error)
				{
					getBrowser () -> println (error .what ());
					return;
				}
			}

			getBrowser () -> println ("Expected unit conversion factor.");
		}

		getBrowser () -> println ("Expected unit name identificator.");
	}

	getBrowser () -> println ("Expected category name identificator after UNIT statement.");
}

void
JSONParser::metaArray (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
		metaObject (json_object_array_get_idx (jobj, i));
}

void
JSONParser::metaObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	std::string metakeyCharacters;

	if (metakey (json_object_object_get (jobj, "@name"), metakeyCharacters))
	{
		std::string metavalueCharacters;

		if (metavalue (json_object_object_get (jobj, "@content"), metavalueCharacters))
		{
			scene -> addMetaData (metakeyCharacters, metavalueCharacters);
			return;
		}

		getBrowser () -> println ("Expected metadata value.");
	}

	getBrowser () -> println ("Expected metadata key.");
}

void
JSONParser::sceneObject (json_object* const jobj)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

	childrenArray (json_object_object_get (jobj, "-children"), scene -> getRootNodes ());
}

void
JSONParser::childrenArray (json_object* const jobj, MFNode & field)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	SFNode node;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (childObject (json_object_array_get_idx (jobj, i), node))
		{
			field .emplace_back (std::move (node));
		}
	}	
}

bool
JSONParser::childObject (json_object* const jobj, SFNode & node)
{
	__LOG__ << this << " " << jobj << std::endl;

	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return false;

	// Parse child nodes.

	json_object_object_foreach (jobj, k, value)
	{
		if (json_object_get_type (value) == json_type_null)
		{
			node = nullptr;
			return true;
		}

		if (json_object_get_type (value) not_eq json_type_object)
			return false;

		const std::string key = k;

		__LOG__ << key << std::endl;

		if (externProtoDeclareObject (key, value))
			return false;

		if (protoDeclareObject (key, value))
			return false;

		if (importObject (key, value))
			return false;

		if (routeObject (key, value))
			return false;

		if (exportObject (key, value))
			return false;

		if (nodeObject (key, value, node))
			return true;
	}

	return false;
}

bool
JSONParser::externProtoDeclareObject (const std::string & key, json_object* const jobj)
{
	static const std::string ExternProtoDeclare = "ExternProtoDeclare";

	if (key not_eq ExternProtoDeclare)
		return false;

	__LOG__ << this << " " << jobj << std::endl;



	return true;
}

bool
JSONParser::protoDeclareObject (const std::string & key, json_object* const jobj)
{
	static const std::string ProtoDeclare = "ProtoDeclare";

	if (key not_eq ProtoDeclare)
		return false;

	__LOG__ << this << " " << jobj << std::endl;



	return true;
}

bool
JSONParser::importObject (const std::string & key, json_object* const jobj)
{
	static const std::string IMPORT = "IMPORT";

	if (key not_eq IMPORT)
		return false;

	__LOG__ << this << " " << jobj << std::endl;



	return true;
}

bool
JSONParser::routeObject (const std::string & key, json_object* const jobj)
{
	static const std::string ROUTE = "ROUTE";

	if (key not_eq ROUTE)
		return false;

	__LOG__ << this << " " << jobj << std::endl;



	return true;
}

bool
JSONParser::exportObject (const std::string & key, json_object* const jobj)
{
	static const std::string EXPORT = "EXPORT";

	if (key not_eq EXPORT)
		return false;

	__LOG__ << this << " " << jobj << std::endl;



	return true;
}

bool
JSONParser::nodeObject (const std::string & nodeType, json_object* const jobj, SFNode & node)
{
	__LOG__ << this << " " << jobj << " " << nodeType << std::endl;

	// USE property

	try
	{
		std::string nodeNameCharacters;
	
		if (nodeNameString (json_object_object_get (jobj, "@USE"), nodeNameCharacters))
		{
			filter_bad_utf8_characters (nodeNameCharacters);
	
			node = getExecutionContext () -> getNamedNode (nodeNameCharacters);
	
			return true;
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> println (error .what ());
		return false;
	}

	// Node object

	bool prototypeInstance = false;

	try
	{
		node = getExecutionContext () -> createNode (nodeType);
	}
	catch (const X3DError & error1)
	{
		// //__LOG__ << this << " " << error .what () << std::endl;

		try
		{
			node = getExecutionContext () -> createPrototypeInstance (nodeType) .getValue ();

			prototypeInstance = true;
		}
		catch (const X3DError & error2)
		{
			getBrowser () -> println (error1 .what () + std::string ("\n") + error2 .what ());
			return false;
		}
	}

	// Node name

	std::string nodeNameCharacters;

	if (nodeNameString (json_object_object_get (jobj, "@DEF"), nodeNameCharacters))
	{
		if (not nodeNameCharacters .empty ())
		{
			try
			{
				const SFNode namedNode = getExecutionContext () -> getNamedNode (nodeNameCharacters); // Create copy!

				getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (nodeNameCharacters), namedNode);
			}
			catch (const X3DError &)
			{ }

			getExecutionContext () -> updateNamedNode (nodeNameCharacters, node);
		}
	}

	// Fields

	if (prototypeInstance)
	{
		// metadata
		fieldValueArray (json_object_object_get (jobj, "fieldValue"), node);
	}
	else
		// Predefined fields
		nodeFieldsObject (jobj, node);

	if (node -> canUserDefinedFields ())
		fieldArray (json_object_object_get (jobj, "field"), node);

	isObject (json_object_object_get (jobj, "IS"), node);

	// After fields are parsed add node to execution context for initialisation.

	getExecutionContext () -> addUninitializedNode (node);

	return true;
}

void
JSONParser::nodeFieldsObject (json_object* const jobj, const SFNode & node)
{
	for (const auto & field : node -> getPreDefinedFields ())
	{
		switch (field -> getType ())
		{
			case X3DConstants::SFNode:
			case X3DConstants::MFNode:
				fieldTypeObject (json_object_object_get (jobj, ("-" + field -> getName ()) .c_str ()), field);
				break;

			default:
				fieldTypeObject (json_object_object_get (jobj, ("@" + field -> getName ()) .c_str ()), field);
				break;
		}
	}
}

void
JSONParser::fieldValueArray (json_object* const jobj, const SFNode & node)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

}

void
JSONParser::fieldArray (json_object* const jobj, const SFNode & node)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

}

void
JSONParser::isObject (json_object* const jobj, const SFNode & node)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) not_eq json_type_object)
		return;

}

void
JSONParser::fieldTypeObject (json_object* const jobj, X3DFieldDefinition* const field)
{
	if (not jobj)
		return;

	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return sfboolValue (jobj, static_cast <SFBool*> (field));

		case X3DConstants::SFColor:
			return sfcolorValue (jobj, static_cast <SFColor*> (field));

		case X3DConstants::SFColorRGBA:
			return sfcolorRGBAValue (jobj, static_cast <SFColorRGBA*> (field));

		case X3DConstants::SFDouble:
			return sfdoubleValue (jobj, static_cast <SFDouble*> (field));

		case X3DConstants::SFFloat:
			return sffloatValue (jobj, static_cast <SFFloat*> (field));

//		case X3DConstants::SFImage:
//			return sfimageValue (static_cast <SFImage*> (field));

		case X3DConstants::SFInt32:
			return sfint32Value (jobj, static_cast <SFInt32*> (field));

		case X3DConstants::SFMatrix3d:
			return sfmatrix3dValue (jobj, static_cast <SFMatrix3d*> (field));

		case X3DConstants::SFMatrix3f:
			return sfmatrix3fValue (jobj, static_cast <SFMatrix3f*> (field));

		case X3DConstants::SFMatrix4d:
			return sfmatrix4dValue (jobj, static_cast <SFMatrix4d*> (field));

		case X3DConstants::SFMatrix4f:
			return sfmatrix4fValue (jobj, static_cast <SFMatrix4f*> (field));

		case X3DConstants::SFNode:
			return sfnodeValue (jobj, static_cast <SFNode*> (field));

		case X3DConstants::SFRotation:
			return sfrotationValue (jobj, static_cast <SFRotation*> (field));

		case X3DConstants::SFString:
			return sfstringValue (jobj, static_cast <SFString*> (field));

		case X3DConstants::SFTime:
			return sftimeValue (jobj, static_cast <SFTime*> (field));

		case X3DConstants::SFVec2d:
			return sfvec2dValue (jobj, static_cast <SFVec2d*> (field));

		case X3DConstants::SFVec2f:
			return sfvec2fValue (jobj, static_cast <SFVec2f*> (field));

		case X3DConstants::SFVec3d:
			return sfvec3dValue (jobj, static_cast <SFVec3d*> (field));

		case X3DConstants::SFVec3f:
			return sfvec3fValue (jobj, static_cast <SFVec3f*> (field));

		case X3DConstants::SFVec4d:
			return sfvec4dValue (jobj, static_cast <SFVec4d*> (field));

		case X3DConstants::SFVec4f:
			return sfvec4fValue (jobj, static_cast <SFVec4f*> (field));

		case X3DConstants::MFBool:
			return mfboolValue (jobj, static_cast <MFBool*> (field));

		case X3DConstants::MFColor:
			return mfcolorValue (jobj, static_cast <MFColor*> (field));

		case X3DConstants::MFColorRGBA:
			return mfcolorRGBAValue (jobj, static_cast <MFColorRGBA*> (field));

		case X3DConstants::MFDouble:
			return mfdoubleValue (jobj, static_cast <MFDouble*> (field));

		case X3DConstants::MFFloat:
			return mffloatValue (jobj, static_cast <MFFloat*> (field));

//		case X3DConstants::MFImage:
//			return mfimageValue (static_cast <MFImage*> (field));

		case X3DConstants::MFInt32:
			return mfint32Value (jobj, static_cast <MFInt32*> (field));

		case X3DConstants::MFMatrix3d:
			return mfmatrix3dValue (jobj, static_cast <MFMatrix3d*> (field));

		case X3DConstants::MFMatrix3f:
			return mfmatrix3fValue (jobj, static_cast <MFMatrix3f*> (field));

		case X3DConstants::MFMatrix4d:
			return mfmatrix4dValue (jobj, static_cast <MFMatrix4d*> (field));

		case X3DConstants::MFMatrix4f:
			return mfmatrix4fValue (jobj, static_cast <MFMatrix4f*> (field));

		case X3DConstants::MFNode:
			return mfnodeValue (jobj, static_cast <MFNode*> (field));

		case X3DConstants::MFRotation:
			return mfrotationValue (jobj, static_cast <MFRotation*> (field));

		case X3DConstants::MFString:
			return mfstringValue (jobj, static_cast <MFString*> (field));

		case X3DConstants::MFTime:
			return mftimeValue (jobj, static_cast <MFTime*> (field));

		case X3DConstants::MFVec2d:
			return mfvec2dValue (jobj, static_cast <MFVec2d*> (field));

		case X3DConstants::MFVec2f:
			return mfvec2fValue (jobj, static_cast <MFVec2f*> (field));

		case X3DConstants::MFVec3d:
			return mfvec3dValue (jobj, static_cast <MFVec3d*> (field));

		case X3DConstants::MFVec3f:
			return mfvec3fValue (jobj, static_cast <MFVec3f*> (field));

		case X3DConstants::MFVec4d:
			return mfvec4dValue (jobj, static_cast <MFVec4d*> (field));

		case X3DConstants::MFVec4f:
			return mfvec4fValue (jobj, static_cast <MFVec4f*> (field));

		default:
			break;
	}
}

bool
JSONParser::booleanValue (json_object* const jobj, bool & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_boolean)
		return false;

	value = json_object_get_boolean (jobj);

	return true;
}

bool
JSONParser::doubleValue (json_object* const jobj, double & value)
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
			break;
	}

	return false;
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

void
JSONParser::sfboolValue (json_object* const jobj, SFBool* const field)
{
	bool value = false;

	booleanValue (jobj, value);

	*field = value;
}

void
JSONParser::mfboolValue (json_object* const jobj, MFBool* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	bool value = false;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (booleanValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sfcolorValue (json_object* const jobj, SFColor* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Color3f value;

		const int size = json_object_array_length (jobj);

		if (size == 3)
		{
			if (color3fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Color3f ());
}

void
JSONParser::mfcolorValue (json_object* const jobj, MFColor* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Color3f value;

	int size = json_object_array_length (jobj);

	size -= size % 3;

	for (int i = 0; i < size; i += 3)
	{
		if (color3fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::color3fValue (json_object* const jobj, const int i, Color3f & value)
{
	double r, g, b;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), r))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), g))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), b))
			{
				value = Color3f (r, g, b);
				return true;
			}
		}
	}

	return false;
}

void
JSONParser::sfcolorRGBAValue (json_object* const jobj, SFColorRGBA* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Color4f value;

		const int size = json_object_array_length (jobj);

		if (size == 4)
		{
			if (color4fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Color4f ());
}

void
JSONParser::mfcolorRGBAValue (json_object* const jobj, MFColorRGBA* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Color4f value;

	int size = json_object_array_length (jobj);

	size -= size % 4;

	for (int i = 0; i < size; i += 4)
	{
		if (color4fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::color4fValue (json_object* const jobj, const int i, Color4f & value)
{
	double r, g, b, a;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), r))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), g))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), b))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), a))
				{
					value = Color4f (r, g, b, a);
					return true;
				}
			}
		}
	}

	return false;
}

void
JSONParser::sfdoubleValue (json_object* const jobj, SFDouble* const field)
{
	double value = 0;

	doubleValue (jobj, value);

	*field = value;
}

void
JSONParser::mfdoubleValue (json_object* const jobj, MFDouble* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	double value = 0;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (doubleValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sffloatValue (json_object* const jobj, SFFloat* const field)
{
	double value = 0;

	doubleValue (jobj, value);

	*field = value;
}

void
JSONParser::mffloatValue (json_object* const jobj, MFFloat* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	double value = 0;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (doubleValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sfint32Value (json_object* const jobj, SFInt32* const field)
{
	int32_t value = 0;

	integerValue (jobj, value);

	*field = value;
}

void
JSONParser::mfint32Value (json_object* const jobj, MFInt32* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	int32_t value = 0;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (integerValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sfmatrix3dValue (json_object* const jobj, SFMatrix3d* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Matrix3d value;

		const int size = json_object_array_length (jobj);

		if (size == 9)
		{
			if (matrix3dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Matrix3d ());
}

void
JSONParser::mfmatrix3dValue (json_object* const jobj, MFMatrix3d* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Matrix3d value;

	int size = json_object_array_length (jobj);

	size -= size % 9;

	for (int i = 0; i < size; i += 9)
	{
		if (matrix3dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::matrix3dValue (json_object* const jobj, const int i, Matrix3d & value)
{
	double e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), e11))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), e12))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), e13))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), e21))
				{
					if (doubleValue (json_object_array_get_idx (jobj, i + 4), e22))
					{
						if (doubleValue (json_object_array_get_idx (jobj, i + 5), e23))
						{
							if (doubleValue (json_object_array_get_idx (jobj, i + 6), e31))
							{
								if (doubleValue (json_object_array_get_idx (jobj, i + 7), e32))
								{
									if (doubleValue (json_object_array_get_idx (jobj, i + 8), e33))
									{
										value = Matrix3d (e11, e12, e13, e21, e22, e23, e31, e32, e33);
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

	return false;
}

void
JSONParser::sfmatrix3fValue (json_object* const jobj, SFMatrix3f* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Matrix3f value;

		const int size = json_object_array_length (jobj);

		if (size == 9)
		{
			if (matrix3fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Matrix3f ());
}

void
JSONParser::mfmatrix3fValue (json_object* const jobj, MFMatrix3f* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Matrix3f value;

	int size = json_object_array_length (jobj);

	size -= size % 9;

	for (int i = 0; i < size; i += 9)
	{
		if (matrix3fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::matrix3fValue (json_object* const jobj, const int i, Matrix3f & value)
{
	double e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), e11))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), e12))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), e13))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), e21))
				{
					if (doubleValue (json_object_array_get_idx (jobj, i + 4), e22))
					{
						if (doubleValue (json_object_array_get_idx (jobj, i + 5), e23))
						{
							if (doubleValue (json_object_array_get_idx (jobj, i + 6), e31))
							{
								if (doubleValue (json_object_array_get_idx (jobj, i + 7), e32))
								{
									if (doubleValue (json_object_array_get_idx (jobj, i + 8), e33))
									{
										value = Matrix3f (e11, e12, e13, e21, e22, e23, e31, e32, e33);
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

	return false;
}

void
JSONParser::sfmatrix4dValue (json_object* const jobj, SFMatrix4d* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Matrix4d value;

		const int size = json_object_array_length (jobj);

		if (size == 16)
		{
			if (matrix4dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Matrix4d ());
}

void
JSONParser::mfmatrix4dValue (json_object* const jobj, MFMatrix4d* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Matrix4d value;

	int size = json_object_array_length (jobj);

	size -= size % 16;

	for (int i = 0; i < size; i += 16)
	{
		if (matrix4dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::matrix4dValue (json_object* const jobj, const int i, Matrix4d & value)
{
	double e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), e11))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), e12))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), e13))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), e14))
				{
					if (doubleValue (json_object_array_get_idx (jobj, i + 4), e21))
					{
						if (doubleValue (json_object_array_get_idx (jobj, i + 5), e22))
						{
							if (doubleValue (json_object_array_get_idx (jobj, i + 6), e23))
							{
								if (doubleValue (json_object_array_get_idx (jobj, i + 7), e24))
								{
									if (doubleValue (json_object_array_get_idx (jobj, i + 8), e31))
									{
										if (doubleValue (json_object_array_get_idx (jobj, i + 9), e32))
										{
											if (doubleValue (json_object_array_get_idx (jobj, i + 10), e33))
											{
												if (doubleValue (json_object_array_get_idx (jobj, i + 11), e34))
												{
													if (doubleValue (json_object_array_get_idx (jobj, i + 12), e41))
													{
														if (doubleValue (json_object_array_get_idx (jobj, i + 13), e42))
														{
															if (doubleValue (json_object_array_get_idx (jobj, i + 14), e43))
															{
																if (doubleValue (json_object_array_get_idx (jobj, i + 15), e44))
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

void
JSONParser::sfmatrix4fValue (json_object* const jobj, SFMatrix4f* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Matrix4f value;

		const int size = json_object_array_length (jobj);

		if (size == 16)
		{
			if (matrix4fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Matrix4f ());
}

void
JSONParser::mfmatrix4fValue (json_object* const jobj, MFMatrix4f* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Matrix4f value;

	int size = json_object_array_length (jobj);

	size -= size % 16;

	for (int i = 0; i < size; i += 16)
	{
		if (matrix4fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::matrix4fValue (json_object* const jobj, const int i, Matrix4f & value)
{
	double e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), e11))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), e12))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), e13))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), e14))
				{
					if (doubleValue (json_object_array_get_idx (jobj, i + 4), e21))
					{
						if (doubleValue (json_object_array_get_idx (jobj, i + 5), e22))
						{
							if (doubleValue (json_object_array_get_idx (jobj, i + 6), e23))
							{
								if (doubleValue (json_object_array_get_idx (jobj, i + 7), e24))
								{
									if (doubleValue (json_object_array_get_idx (jobj, i + 8), e31))
									{
										if (doubleValue (json_object_array_get_idx (jobj, i + 9), e32))
										{
											if (doubleValue (json_object_array_get_idx (jobj, i + 10), e33))
											{
												if (doubleValue (json_object_array_get_idx (jobj, i + 11), e34))
												{
													if (doubleValue (json_object_array_get_idx (jobj, i + 12), e41))
													{
														if (doubleValue (json_object_array_get_idx (jobj, i + 13), e42))
														{
															if (doubleValue (json_object_array_get_idx (jobj, i + 14), e43))
															{
																if (doubleValue (json_object_array_get_idx (jobj, i + 15), e44))
																{
																	value = Matrix4f (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44);
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

void
JSONParser::sfnodeValue (json_object* const jobj, SFNode* const field)
{
	if (childObject (jobj, *field))
		return;

	*field = nullptr;
}

void
JSONParser::mfnodeValue (json_object* const jobj, MFNode* const field)
{
	field -> clear ();

	childrenArray (jobj, *field);
}

void
JSONParser::sfrotationValue (json_object* const jobj, SFRotation* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Rotation4d value;

		const int size = json_object_array_length (jobj);

		if (size == 4)
		{
			if (rotation4dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Rotation4d ());
}

void
JSONParser::mfrotationValue (json_object* const jobj, MFRotation* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Rotation4d value;

	int size = json_object_array_length (jobj);

	size -= size % 4;

	for (int i = 0; i < size; i += 4)
	{
		if (rotation4dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::rotation4dValue (json_object* const jobj, const int i, Rotation4d & value)
{
	double x, y, z, angle;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), z))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), angle))
				{
					value = Rotation4d (x, y, z, angle);
					return true;
				}
			}
		}
	}

	return false;
}

void
JSONParser::sfstringValue (json_object* const jobj, SFString* const field)
{
	std::string stringCharacters;

	stringValue (jobj, stringCharacters);

	*field = stringCharacters;
}

void
JSONParser::mfstringValue (json_object* const jobj, MFString* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	std::string value;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (stringValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (std::move (value));
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sftimeValue (json_object* const jobj, SFTime* const field)
{
	double value = 0;

	doubleValue (jobj, value);

	*field = value;
}

void
JSONParser::mftimeValue (json_object* const jobj, MFTime* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	double value = 0;

	const int size = json_object_array_length (jobj);

	for (int i = 0; i < size; ++ i)
	{
		if (doubleValue (json_object_array_get_idx (jobj, i), value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

void
JSONParser::sfvec2dValue (json_object* const jobj, SFVec2d* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector2d value;

		const int size = json_object_array_length (jobj);

		if (size == 2)
		{
			if (vector2dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector2d ());
}

void
JSONParser::mfvec2dValue (json_object* const jobj, MFVec2d* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector2d value;

	int size = json_object_array_length (jobj);

	size -= size % 2;

	for (int i = 0; i < size; i += 2)
	{
		if (vector2dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector2dValue (json_object* const jobj, const int i, Vector2d & value)
{
	double x, y;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			value = Vector2d (x, y);
			return true;
		}
	}

	return false;
}

void
JSONParser::sfvec2fValue (json_object* const jobj, SFVec2f* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector2f value;

		const int size = json_object_array_length (jobj);

		if (size == 2)
		{
			if (vector2fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector2f ());
}

void
JSONParser::mfvec2fValue (json_object* const jobj, MFVec2f* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector2f value;

	int size = json_object_array_length (jobj);

	size -= size % 2;

	for (int i = 0; i < size; i += 2)
	{
		if (vector2fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector2fValue (json_object* const jobj, const int i, Vector2f & value)
{
	double x, y;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			value = Vector2f (x, y);
			return true;
		}
	}

	return false;
}

void
JSONParser::sfvec3dValue (json_object* const jobj, SFVec3d* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector3d value;

		const int size = json_object_array_length (jobj);

		if (size == 3)
		{
			if (vector3dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector3d ());
}

void
JSONParser::mfvec3dValue (json_object* const jobj, MFVec3d* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector3d value;

	int size = json_object_array_length (jobj);

	size -= size % 3;

	for (int i = 0; i < size; i += 3)
	{
		if (vector3dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector3dValue (json_object* const jobj, const int i, Vector3d & value)
{
	double x, y, z;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), z))
			{
				value = Vector3d (x, y, z);
				return true;
			}
		}
	}

	return false;
}

void
JSONParser::sfvec3fValue (json_object* const jobj, SFVec3f* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector3f value;

		const int size = json_object_array_length (jobj);

		if (size == 3)
		{
			if (vector3fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector3f ());
}

void
JSONParser::mfvec3fValue (json_object* const jobj, MFVec3f* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector3f value;

	int size = json_object_array_length (jobj);

	size -= size % 3;

	for (int i = 0; i < size; i += 3)
	{
		if (vector3fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector3fValue (json_object* const jobj, const int i, Vector3f & value)
{
	double x, y, z;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), z))
			{
				value = Vector3f (x, y, z);
				return true;
			}
		}
	}

	return false;
}

void
JSONParser::sfvec4dValue (json_object* const jobj, SFVec4d* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector4d value;

		const int size = json_object_array_length (jobj);

		if (size == 4)
		{
			if (vector4dValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector4d ());
}

void
JSONParser::mfvec4dValue (json_object* const jobj, MFVec4d* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector4d value;

	int size = json_object_array_length (jobj);

	size -= size % 4;

	for (int i = 0; i < size; i += 4)
	{
		if (vector4dValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector4dValue (json_object* const jobj, const int i, Vector4d & value)
{
	double x, y, z, w;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), z))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), w))
				{
					value = Vector4d (x, y, z, w);
					return true;
				}
			}
		}
	}

	return false;
}

void
JSONParser::sfvec4fValue (json_object* const jobj, SFVec4f* const field)
{
	if (not jobj)
		return;

	if (json_object_get_type (jobj) == json_type_array)
	{
		Vector4f value;

		const int size = json_object_array_length (jobj);

		if (size == 4)
		{
			if (vector4fValue (jobj, 0, value))
			{
				field -> setValue (value);
				return;
			}
		}
	}

	field -> setValue (Vector4f ());
}

void
JSONParser::mfvec4fValue (json_object* const jobj, MFVec4f* const field)
{
	if (not jobj)
		return;

	field -> clear ();

	if (json_object_get_type (jobj) not_eq json_type_array)
		return;

	Vector4f value;

	int size = json_object_array_length (jobj);

	size -= size % 4;

	for (int i = 0; i < size; i += 4)
	{
		if (vector4fValue (jobj, i, value))
			field -> emplace_back (value);
		else
			field -> emplace_back ();
	}
}

bool
JSONParser::vector4fValue (json_object* const jobj, const int i, Vector4f & value)
{
	double x, y, z, w;

	if (doubleValue (json_object_array_get_idx (jobj, i + 0), x))
	{
		if (doubleValue (json_object_array_get_idx (jobj, i + 1), y))
		{
			if (doubleValue (json_object_array_get_idx (jobj, i + 2), z))
			{
				if (doubleValue (json_object_array_get_idx (jobj, i + 3), w))
				{
					value = Vector4f (x, y, z, w);
					return true;
				}
			}
		}
	}

	return false;
}

void
JSONParser::pushExecutionContext (X3DExecutionContext* const executionContext)
{
	//__LOG__ << this << " " << std::endl;

	executionContextStack .emplace_back (executionContext);
}

void
JSONParser::popExecutionContext ()
{
	//__LOG__ << this << " " << std::endl;

	executionContextStack .pop_back ();
}

X3DExecutionContext*
JSONParser::getExecutionContext () const
{
	//__LOG__ << this << " " << std::endl;

	return executionContextStack .back ();
}

bool
JSONParser::isInsideProtoDefinition () const
{
	//__LOG__ << this << " " << std::endl;

	return executionContextStack .size () > 1;
}

JSONParser::~JSONParser ()
{ }

} // X3D

} // titania
