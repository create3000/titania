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
//		case X3DConstants::SFBool:
//			return sfboolValue (static_cast <SFBool*> (field));
//
//		case X3DConstants::SFColor:
//			return sfcolorValue (static_cast <SFColor*> (field));
//
//		case X3DConstants::SFColorRGBA:
//			return sfcolorRGBAValue (static_cast <SFColorRGBA*> (field));
//
//		case X3DConstants::SFDouble:
//			return sfdoubleValue (static_cast <SFDouble*> (field));
//
//		case X3DConstants::SFFloat:
//			return sffloatValue (static_cast <SFFloat*> (field));
//
//		case X3DConstants::SFImage:
//			return sfimageValue (static_cast <SFImage*> (field));
//
//		case X3DConstants::SFInt32:
//			return sfint32Value (static_cast <SFInt32*> (field));
//
//		case X3DConstants::SFMatrix3d:
//			return sfmatrix3dValue (static_cast <SFMatrix3d*> (field));
//
//		case X3DConstants::SFMatrix3f:
//			return sfmatrix3fValue (static_cast <SFMatrix3f*> (field));
//
//		case X3DConstants::SFMatrix4d:
//			return sfmatrix4dValue (static_cast <SFMatrix4d*> (field));
//
//		case X3DConstants::SFMatrix4f:
//			return sfmatrix4fValue (static_cast <SFMatrix4f*> (field));

		case X3DConstants::SFNode:
			return sfnodeValue (jobj, static_cast <SFNode*> (field));

//		case X3DConstants::SFRotation:
//			return sfrotationValue (static_cast <SFRotation*> (field));
//
//		case X3DConstants::SFString:
//			return sfstringValue (static_cast <SFString*> (field));
//
//		case X3DConstants::SFTime:
//			return sftimeValue (static_cast <SFTime*> (field));
//
//		case X3DConstants::SFVec2d:
//			return sfvec2dValue (static_cast <SFVec2d*> (field));
//
//		case X3DConstants::SFVec2f:
//			return sfvec2fValue (static_cast <SFVec2f*> (field));
//
//		case X3DConstants::SFVec3d:
//			return sfvec3dValue (static_cast <SFVec3d*> (field));
//
//		case X3DConstants::SFVec3f:
//			return sfvec3fValue (static_cast <SFVec3f*> (field));
//
//		case X3DConstants::SFVec4d:
//			return sfvec4dValue (static_cast <SFVec4d*> (field));
//
//		case X3DConstants::SFVec4f:
//			return sfvec4fValue (static_cast <SFVec4f*> (field));
//
//		case X3DConstants::MFBool:
//			return mfboolValue (static_cast <MFBool*> (field));
//
//		case X3DConstants::MFColor:
//			return mfcolorValue (static_cast <MFColor*> (field));
//
//		case X3DConstants::MFColorRGBA:
//			return mfcolorRGBAValue (static_cast <MFColorRGBA*> (field));
//
//		case X3DConstants::MFDouble:
//			return mfdoubleValue (static_cast <MFDouble*> (field));
//
//		case X3DConstants::MFFloat:
//			return mffloatValue (static_cast <MFFloat*> (field));
//
//		case X3DConstants::MFImage:
//			return mfimageValue (static_cast <MFImage*> (field));
//
//		case X3DConstants::MFInt32:
//			return mfint32Value (static_cast <MFInt32*> (field));
//
//		case X3DConstants::MFMatrix3d:
//			return mfmatrix3dValue (static_cast <MFMatrix3d*> (field));
//
//		case X3DConstants::MFMatrix3f:
//			return mfmatrix3fValue (static_cast <MFMatrix3f*> (field));
//
//		case X3DConstants::MFMatrix4d:
//			return mfmatrix4dValue (static_cast <MFMatrix4d*> (field));
//
//		case X3DConstants::MFMatrix4f:
//			return mfmatrix4fValue (static_cast <MFMatrix4f*> (field));

		case X3DConstants::MFNode:
			return mfnodeValue (jobj, static_cast <MFNode*> (field));

//		case X3DConstants::MFRotation:
//			return mfrotationValue (static_cast <MFRotation*> (field));
//
//		case X3DConstants::MFString:
//			return mfstringValue (static_cast <MFString*> (field));
//
//		case X3DConstants::MFTime:
//			return mftimeValue (static_cast <MFTime*> (field));
//
//		case X3DConstants::MFVec2d:
//			return mfvec2dValue (static_cast <MFVec2d*> (field));
//
//		case X3DConstants::MFVec2f:
//			return mfvec2fValue (static_cast <MFVec2f*> (field));
//
//		case X3DConstants::MFVec3d:
//			return mfvec3dValue (static_cast <MFVec3d*> (field));
//
//		case X3DConstants::MFVec3f:
//			return mfvec3fValue (static_cast <MFVec3f*> (field));
//
//		case X3DConstants::MFVec4d:
//			return mfvec4dValue (static_cast <MFVec4d*> (field));
//
//		case X3DConstants::MFVec4f:
//			return mfvec4fValue (static_cast <MFVec4f*> (field));

		default:
			break;
	}
}

bool
JSONParser::doubleValue (json_object* const jobj, double & value)
{
	if (not jobj)
		return false;

	if (json_object_get_type (jobj) not_eq json_type_double)
		return false;

	value = json_object_get_double (jobj);

	return true;
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
JSONParser::sfnodeValue (json_object* const jobj, SFNode* const field)
{
	if (childObject (jobj, *field))
		return;

	*field = nullptr;
}

void
JSONParser::mfnodeValue (json_object* const jobj, MFNode* const field)
{
	childrenArray (jobj, *field);
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
