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

#include "XMLParser.h"

#include <libxml++/libxml++.h>

#include "../Browser/X3DBrowser.h"
#include "../Parser/Filter.h"
#include "../Parser/Parser.h"

namespace titania {
namespace X3D {

class XMLGrammar
{
public:

	static const io::sequence WhiteSpaces;

	static const io::string falseValue;
	static const io::string trueValue;

	static const io::number <double>  DoubleValue;
	static const io::number <int32_t> IntegerValue;

};

const io::sequence XMLGrammar::WhiteSpaces ("\r\n \t,");

const io::string XMLGrammar::falseValue ("false");
const io::string XMLGrammar::trueValue ("true");

const io::number <double>  XMLGrammar::DoubleValue;
const io::number <int32_t> XMLGrammar::IntegerValue;

XMLParser::XMLParser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	            X3DParser (),
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	            xmlParser (new xmlpp::DomParser ()),
	executionContextStack (),
	              parents ()
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (true);
}

void
XMLParser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	try
	{
		scene -> setWorldURL (uri);
		scene -> setEncoding (EncodingType::XML);

		xmlParser -> parse_stream (istream);

		const auto xmlDocument = xmlParser -> get_document ();

		if (xmlDocument)
			xmlElement (xmlDocument -> get_root_node ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		throw;
	}
	catch (const std::exception & error)
	{
		throw Error <X3D::INVALID_X3D> (error .what ()); 
	}
}

void
XMLParser::xmlElement (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("X3D", std::mem_fn (&XMLParser::x3dElement)),
	};

	try
	{
		if (not xmlElement)
			return;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
XMLParser::x3dElement (xmlpp::Element* const xmlElement)
{
	pushExecutionContext (scene);

	std::string profileCharacters;
	std::string specificationVersionCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("profile"), profileCharacters))
	{
		try
		{
			scene -> setProfile (getBrowser () -> getProfile (profileCharacters));
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (stringAttribute (xmlElement -> get_attribute ("version"), specificationVersionCharacters))
	{
		try
		{
			scene -> setSpecificationVersion (specificationVersionCharacters);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Parse children.

	for (const auto & xmlNode : xmlElement -> get_children ())
		x3dChild (dynamic_cast <xmlpp::Element*> (xmlNode));

	popExecutionContext ();
}

void
XMLParser::x3dChild (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("head",  std::mem_fn (&XMLParser::headElement)),
		std::make_pair ("Scene", std::mem_fn (&XMLParser::sceneElement)),
	};

	try
	{
		if (not xmlElement)
			return;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
XMLParser::headElement (xmlpp::Element* const xmlElement)
{
	for (const auto & xmlNode : xmlElement -> get_children ())
		headChild (dynamic_cast <xmlpp::Element*> (xmlNode));
}

void
XMLParser::headChild (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("component", std::mem_fn (&XMLParser::componentElement)),
		std::make_pair ("unit",      std::mem_fn (&XMLParser::unitElement)),
		std::make_pair ("meta",      std::mem_fn (&XMLParser::metaElement)),
	};

	try
	{
		if (not xmlElement)
			return;

		elementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
XMLParser::componentElement (xmlpp::Element* const xmlElement)
{
	std::string componentNameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), componentNameCharacters))
	{
		int32_t componentSupportLevel;

		if (integerAttribute (xmlElement -> get_attribute ("level"), componentSupportLevel))
		{
			try
			{
				const auto component = getBrowser () -> getComponent (componentNameCharacters, componentSupportLevel);

				scene -> updateComponent (component);
				return;
			}
			catch (const X3D::X3DError & error)
			{
				getBrowser () -> println (error .what ());
				return;
			}
		}
		else
			getBrowser () -> println ("Expected a component support level.");
	}
	else
		getBrowser () -> println ("Expected a component name.");
}

void
XMLParser::unitElement (xmlpp::Element* const xmlElement)
{
	std::string categoryNameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("category"), categoryNameCharacters))
	{
		std::string unitNameCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("name"), unitNameCharacters))
		{
			double unitConversionFactor;

			if (doubleAttribute (xmlElement -> get_attribute ("conversionFactor"), unitConversionFactor))
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
			else
				getBrowser () -> println ("Expected unit conversion factor.");
		}
		else
			getBrowser () -> println ("Expected unit name identificator.");
	}
	else
		getBrowser () -> println ("Expected category name identificator after UNIT statement.");
}

void
XMLParser::metaElement (xmlpp::Element* const xmlElement)
{
	std::string metakeyCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), metakeyCharacters))
	{
		std::string metavalueCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("content"), metavalueCharacters))
		{
			scene -> addMetaData (metakeyCharacters, metavalueCharacters);
			return;
		}
		else
			getBrowser () -> println ("Expected metadata value.");
	}
	else
		getBrowser () -> println ("Expected metadata key.");
}

//                   .;;;.   .;;;.
//                .;;;.  `:::'  .;;;.
//              .;',:.`;.`;;;'.;'.:.`;.
//            .;' .:|:. `;;.;;' .:|:. `;.
//           .;'  `:|:'  .;;;.  `:|:'  `;.
//           ;;    `:' .;;o o;;. `:'    ;;
//           `;;     .;;'_.-._`;;.     ;;'
//             `;;..;':. '.-.' .:`;..;;'
//                `'   `:.   .:'   `'
//            MJP        `;";'
//                 .;;;..;: :;..;;;.
//               .;'               `;.
//              .;'  .;.       .;.  `;.
//   .;`;.     .;'  .;' `;   ;' `;.  `;.     .;';.
//   ;  ;`;. .;' .;'    .;   ;.    `;. `;. .;';  ;
//    `'`; `;..;'       ;;   ;;       `;..;' ;'`'
//     .;'   `;         ;;. .;;         ;'   `;.
//   .;' ;'.; `;.       ;;;;;;;       .;' ;.`; `;.
//  ;' .-.   `. `;     .;;' ';;.     ;' .'   .-. `;
//  `:_).'    ( :'    ;;;'   `:::    `: )    `.(_:'
//                    `;;     ;;'
//                     (;     ;)
//                     ;;.    ;;
//                     ;;;   ;;;
//                     ;;'   `;;
//                     ;'     `;
//               _____.;       ;._____
//               )_.-' /       \ `-._(
//                 ).-'         `-.(

void
XMLParser::sceneElement (xmlpp::Element* const xmlElement)
{
	childrenElements (xmlElement);
}

void
XMLParser::childrenElements (xmlpp::Element* const xmlElement)
{
	for (const auto & xmlNode : xmlElement -> get_children ())
		childElement (dynamic_cast <xmlpp::Element*> (xmlNode));
}

void
XMLParser::childElement (xmlpp::Element* const xmlElement)
{
	using ElementsFunction = std::function <void (XMLParser*, xmlpp::Element* const)>;

	static const std::map <std::string, ElementsFunction> elementsIndex = {
		std::make_pair ("ExternProtoDeclare", std::mem_fn (&XMLParser::externProtoDeclareElement)),
		std::make_pair ("ProtoDeclare",       std::mem_fn (&XMLParser::protoDeclareElement)),
		std::make_pair ("ProtoInstance",      std::mem_fn (&XMLParser::protoInstanceElement)),
		std::make_pair ("ROUTE",              std::mem_fn (&XMLParser::routeElement)),
		std::make_pair ("IMPORT",             std::mem_fn (&XMLParser::importElement)),
		std::make_pair ("EXPORT",             std::mem_fn (&XMLParser::exportElement)),
	};

	if (not xmlElement)
		return;

	const auto iter = elementsIndex .find (xmlElement -> get_name ());

	if (iter == elementsIndex .end ())
		nodeElement (xmlElement);
	else
		iter -> second (this, xmlElement);
}

void
XMLParser::externProtoDeclareElement (xmlpp::Element* const xmlElement)
{
}

void
XMLParser::protoDeclareElement (xmlpp::Element* const xmlElement)
{
}

void
XMLParser::protoInstanceElement (xmlpp::Element* const xmlElement)
{
}

void
XMLParser::nodeElement (xmlpp::Element* const xmlElement)
{
	// USE property

	if (useAttribute (xmlElement))
		return;

	// Node object

	try
	{
		const auto node = scene -> createNode (xmlElement -> get_name ());

		defAttribute (xmlElement, node);

		addNode (xmlElement, node);

		parents .emplace_back (node);

		nodeAttributes (xmlElement, node);

		childrenElements (xmlElement);

		getExecutionContext () -> addUninitializedNode (node);

		parents .pop_back ();
	}
	catch (const X3DError & error)
	{
		getBrowser () -> println (error .what ());
	}
}

void
XMLParser::routeElement (xmlpp::Element* const xmlElement)
{
	try
	{
		std::string sourceNodeName;
		std::string sourceField;
		std::string destinationNodeName;
		std::string destinationField;

		if (stringAttribute (xmlElement -> get_attribute ("fromNode"), sourceNodeName))
		{
			if (stringAttribute (xmlElement -> get_attribute ("fromField"), sourceField))
			{
				if (stringAttribute (xmlElement -> get_attribute ("toNode"), destinationNodeName))
				{
					if (stringAttribute (xmlElement -> get_attribute ("toField"), destinationField))
					{
						const auto sourceNode       = getExecutionContext () -> getLocalNode (sourceNodeName);
						const auto destinationNode  = getExecutionContext () -> getLocalNode (destinationNodeName);

						getExecutionContext () -> addRoute (sourceNode, sourceField, destinationNode, destinationField);
					}
					else
						getBrowser () -> println ("Bad ROUTE statement: Expected toField attribute.");
				}
				else
					getBrowser () -> println ("Bad ROUTE statement: Expected toNode attribute.");
			}
			else
				getBrowser () -> println ("Bad ROUTE statement: Expected fromField attribute.");
		}
		else
			getBrowser () -> println ("Bad ROUTE statement: Expected fromNode attribute.");
	}
	catch (const X3DError & error)
	{
		getBrowser () -> println ("XML Parser Error: " + std::string (error .what ()));
	}
}

void
XMLParser::importElement (xmlpp::Element* const xmlElement)
{
}

void
XMLParser::exportElement (xmlpp::Element* const xmlElement)
{
}

bool
XMLParser::doubleAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return XMLGrammar::DoubleValue (istream, value);
}

bool
XMLParser::integerAttribute (xmlpp::Attribute* const xmlAttribute, int32_t & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return XMLGrammar::IntegerValue (istream, value);
}

bool
XMLParser::stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim? but not meta!

	value = xmlAttribute -> get_value ();

	return true;
}

bool
XMLParser::useAttribute (xmlpp::Element* const xmlElement)
{
	try
	{
		std::string nodeNameCharacters;
	
		if (stringAttribute (xmlElement -> get_attribute ("USE"), nodeNameCharacters))
		{
			filter_bad_utf8_characters (nodeNameCharacters);

			addNode (xmlElement, getExecutionContext () -> getNamedNode (nodeNameCharacters));
			return true;
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> println ("Invalid USE name: " + std::string (error .what ()));
	}

	return false;
}	
	
void
XMLParser::defAttribute (xmlpp::Element* const xmlElement, const SFNode & node)
{
	try
	{
		std::string nodeNameCharacters;
	
		if (stringAttribute (xmlElement -> get_attribute ("DEF"), nodeNameCharacters))
		{
			try
			{
				filter_bad_utf8_characters (nodeNameCharacters);

				const auto namedNode = getExecutionContext () -> getNamedNode (nodeNameCharacters);

				getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (nodeNameCharacters), namedNode);
			}
			catch (const X3DError & error)
			{ }

			getExecutionContext () -> updateNamedNode (nodeNameCharacters, node);
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> println ("Invalid DEF name: " + std::string (error .what ()));
	}
}

void
XMLParser::nodeAttributes (xmlpp::Element* const xmlElement, const SFNode & node)
{
	for (const auto & xmlAttribute : xmlElement -> get_attributes ())
		nodeAttribute (xmlAttribute, node);
}

void
XMLParser::nodeAttribute (xmlpp::Attribute* const xmlAttribute, const SFNode & node)
{
	try
	{
		const auto field = node -> getField (xmlAttribute -> get_name ());

		fieldValue (field, xmlAttribute -> get_value ());
	}
	catch (const X3DError & error)
	{
		//getBrowser () -> println (error .what ());
	}
}

void
XMLParser::fieldValue (X3DFieldDefinition* const field, const std::string & value)
{
	field -> isSet (true);

	std::istringstream istream (value);

	istream .imbue (std::locale::classic ());

	Parser parser (istream, scene);

	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			sfboolValue (istream, static_cast <SFBool*> (field));
			return;

		case X3DConstants::SFColor:
			parser .sfcolorValue (static_cast <SFColor*> (field));
			return;

		case X3DConstants::SFColorRGBA:
			parser .sfcolorRGBAValue (static_cast <SFColorRGBA*> (field));
			return;

		case X3DConstants::SFDouble:
			parser .sfdoubleValue (static_cast <SFDouble*> (field));
			return;

		case X3DConstants::SFFloat:
			parser .sffloatValue (static_cast <SFFloat*> (field));
			return;

		case X3DConstants::SFImage:
			parser .sfimageValue (static_cast <SFImage*> (field));
			return;

		case X3DConstants::SFInt32:
			parser .sfint32Value (static_cast <SFInt32*> (field));
			return;

		case X3DConstants::SFMatrix3d:
			parser .sfmatrix3dValue (static_cast <SFMatrix3d*> (field));
			return;

		case X3DConstants::SFMatrix3f:
			parser .sfmatrix3fValue (static_cast <SFMatrix3f*> (field));
			return;

		case X3DConstants::SFMatrix4d:
			parser .sfmatrix4dValue (static_cast <SFMatrix4d*> (field));
			return;

		case X3DConstants::SFMatrix4f:
			parser .sfmatrix4fValue (static_cast <SFMatrix4f*> (field));
			return;

		case X3DConstants::SFNode:
			static_cast <SFNode*> (field) -> setValue (nullptr);
			return;

		case X3DConstants::SFRotation:
			parser .sfrotationValue (static_cast <SFRotation*> (field));
			return;

		case X3DConstants::SFString:
			static_cast <SFString*> (field) -> setValue (value);
			return;

		case X3DConstants::SFTime:
			parser .sftimeValue (static_cast <SFTime*> (field));
			return;

		case X3DConstants::SFVec2d:
			parser .sfvec2dValue (static_cast <SFVec2d*> (field));
			return;

		case X3DConstants::SFVec2f:
			parser .sfvec2fValue (static_cast <SFVec2f*> (field));
			return;

		case X3DConstants::SFVec3d:
			parser .sfvec3dValue (static_cast <SFVec3d*> (field));
			return;

		case X3DConstants::SFVec3f:
			parser .sfvec3fValue (static_cast <SFVec3f*> (field));
			return;

		case X3DConstants::SFVec4d:
			parser .sfvec4dValue (static_cast <SFVec4d*> (field));
			return;

		case X3DConstants::SFVec4f:
			parser .sfvec4fValue (static_cast <SFVec4f*> (field));
			return;

		case X3DConstants::MFBool:
			sfboolValues (istream, static_cast <MFBool*> (field));
			return;

		case X3DConstants::MFColor:
			parser .sfcolorValues (static_cast <MFColor*> (field));
			return;

		case X3DConstants::MFColorRGBA:
			parser .sfcolorRGBAValues (static_cast <MFColorRGBA*> (field));
			return;

		case X3DConstants::MFDouble:
			parser .sfdoubleValues (static_cast <MFDouble*> (field));
			return;

		case X3DConstants::MFFloat:
			parser .sffloatValues (static_cast <MFFloat*> (field));
			return;

		case X3DConstants::MFImage:
			parser .sfimageValues (static_cast <MFImage*> (field));
			return;

		case X3DConstants::MFInt32:
			parser .sfint32Values (static_cast <MFInt32*> (field));
			return;

		case X3DConstants::MFMatrix3d:
			parser .sfmatrix3dValues (static_cast <MFMatrix3d*> (field));
			return;

		case X3DConstants::MFMatrix3f:
			parser .sfmatrix3fValues (static_cast <MFMatrix3f*> (field));
			return;

		case X3DConstants::MFMatrix4d:
			parser .sfmatrix4dValues (static_cast <MFMatrix4d*> (field));
			return;

		case X3DConstants::MFMatrix4f:
			parser .sfmatrix4fValues (static_cast <MFMatrix4f*> (field));
			return;

		case X3DConstants::MFNode:
			return;

		case X3DConstants::MFRotation:
			parser .sfrotationValues (static_cast <MFRotation*> (field));
			return;

		case X3DConstants::MFString:
			parser .sfstringValues (static_cast <MFString*> (field));
			return;

		case X3DConstants::MFTime:
			parser .sftimeValues (static_cast <MFTime*> (field));
			return;

		case X3DConstants::MFVec2d:
			parser .sfvec2dValues (static_cast <MFVec2d*> (field));
			return;

		case X3DConstants::MFVec2f:
			parser .sfvec2fValues (static_cast <MFVec2f*> (field));
			return;

		case X3DConstants::MFVec3d:
			parser .sfvec3dValues (static_cast <MFVec3d*> (field));
			return;

		case X3DConstants::MFVec3f:
			parser .sfvec3fValues (static_cast <MFVec3f*> (field));
			return;

		case X3DConstants::MFVec4d:
			parser .sfvec4dValues (static_cast <MFVec4d*> (field));
			return;

		case X3DConstants::MFVec4f:
			parser .sfvec4fValues (static_cast <MFVec4f*> (field));
			return;
	}
}

bool
XMLParser::sfboolValue (std::istream & istream, SFBool* field)
{
	std::string whiteSpaces;

	XMLGrammar::WhiteSpaces (istream, whiteSpaces);

	if (XMLGrammar::trueValue (istream))
	{
		field -> setValue (true);
		return true;
	}

	if (XMLGrammar::falseValue (istream))
	{
		field -> setValue (false);
		return true;
	}

	return false;
}

void
XMLParser::sfboolValues (std::istream & istream, MFBool* field)
{
	SFBool value;

	while (sfboolValue (istream, &value))
		field -> emplace_back (value);
}

void
XMLParser::addNode (xmlpp::Element* const xmlElement, const SFNode & node)
{
	if (parents .empty ())
	{
		scene -> getRootNodes () .emplace_back (node);
		return;
	}

	const auto baseNode = dynamic_cast <X3DBaseNode*> (parents .back ());

	if (baseNode)
	{
		try
		{
			std::string containerFieldCharacters;
		
			stringAttribute (xmlElement -> get_attribute ("containerField"), containerFieldCharacters);

			if (containerFieldCharacters .empty ())
				containerFieldCharacters = node -> getContainerField ();

			const auto field = baseNode -> getField (containerFieldCharacters);

			switch (field -> getType ())
			{
				case X3DConstants::SFNode:
				{
					const auto sfnode = static_cast <SFNode*> (field);

					sfnode -> setValue (node);
					sfnode -> isSet (true);
					return;
				}
				case X3DConstants::MFNode:
				{
					const auto mfnode = static_cast <MFNode*> (field);

					mfnode -> emplace_back (node);
					mfnode -> isSet (true);
					return;
				}
				default:
					return;
			}
		}
		catch (const X3DError &)
		{ }

		return;
	}
}

void
XMLParser::pushExecutionContext (X3DExecutionContext* const executionContext)
{
	executionContextStack .emplace_back (executionContext);
}

void
XMLParser::popExecutionContext ()
{
	executionContextStack .pop_back ();
}

X3DExecutionContext*
XMLParser::getExecutionContext () const
{
	return executionContextStack .back ();
}

XMLParser::~XMLParser ()
{ }

} // X3D
} // titania
