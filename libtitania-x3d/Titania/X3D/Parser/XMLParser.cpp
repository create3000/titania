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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <libxml++/libxml++.h>
#pragma GCC diagnostic pop

#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Networking/Inline.h"
#include "../Parser/Filter.h"
#include "../Parser/Parser.h"
#include "../Prototype/ProtoDeclaration.h"
#include "../Prototype/ExternProtoDeclaration.h"

namespace titania {
namespace X3D {

class XMLGrammar
{
public:

	static const io::sequence WhiteSpaces;

	static const std::map <std::string, AccessType> AccessTypes;

	static const io::number <int32_t> IntegerValue;

};

const io::sequence XMLGrammar::WhiteSpaces ("\r\n \t,");

const std::map <std::string, AccessType> XMLGrammar::AccessTypes = {
	std::pair ("initializeOnly", initializeOnly),
	std::pair ("inputOnly",      inputOnly),
	std::pair ("outputOnly",     outputOnly),
	std::pair ("inputOutput",    inputOutput),
};

const io::number <int32_t>     XMLGrammar::IntegerValue;

// XMLParser

const std::map <std::string, XMLParser::ElementsFunction> XMLParser::xmlElementIndex = {
	std::pair ("X3D",                std::mem_fn (&XMLParser::x3dElement)),
	std::pair ("Scene",              std::mem_fn (&XMLParser::sceneElement)),
	std::pair ("ExternProtoDeclare", std::mem_fn (&XMLParser::externProtoDeclareElement)),
	std::pair ("ProtoDeclare",       std::mem_fn (&XMLParser::protoDeclareElement)),
	std::pair ("ProtoInstance",      std::mem_fn (&XMLParser::protoInstanceElement)),
};

const std::map <std::string, XMLParser::ElementsFunction> XMLParser::x3dElementChildIndex = {
	std::pair ("head",  std::mem_fn (&XMLParser::headElement)),
	std::pair ("Scene", std::mem_fn (&XMLParser::sceneElement)),
};

const std::map <std::string, XMLParser::ElementsFunction> XMLParser::headElementsIndex = {
	std::pair ("component", std::mem_fn (&XMLParser::componentElement)),
	std::pair ("unit",      std::mem_fn (&XMLParser::unitElement)),
	std::pair ("meta",      std::mem_fn (&XMLParser::metaElement)),
};

const std::map <std::string, XMLParser::ElementsFunction> XMLParser::childElementIndex = {
	std::pair ("ExternProtoDeclare", std::mem_fn (&XMLParser::externProtoDeclareElement)),
	std::pair ("ProtoDeclare",       std::mem_fn (&XMLParser::protoDeclareElement)),
	std::pair ("IS",                 std::mem_fn (&XMLParser::isElement)),
	std::pair ("ProtoInstance",      std::mem_fn (&XMLParser::protoInstanceElement)),
	std::pair ("fieldValue",         std::mem_fn (&XMLParser::fieldValueElement)),
	std::pair ("field",              std::mem_fn (&XMLParser::fieldElement)),
	std::pair ("ROUTE",              std::mem_fn (&XMLParser::routeElement)),
	std::pair ("IMPORT",             std::mem_fn (&XMLParser::importElement)),
	std::pair ("EXPORT",             std::mem_fn (&XMLParser::exportElement)),
};

XMLParser::XMLParser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	            X3DParser (),
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	            xmlParser (new xmlpp::DomParser ()),
	              parents ()
{
	xmlParser -> set_throw_messages (true);
	xmlParser -> set_validate (false);
	xmlParser -> set_include_default_attributes (false);

	pushExecutionContext (scene);
}

void
XMLParser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	try
	{
		scene -> setWorldURL (uri);
		scene -> setEncoding (EncodingType::XML);
		scene -> setProfile (getBrowser () -> getProfile ("Full"));

		xmlParser -> parse_stream (istream);

		const auto xmlDocument = xmlParser -> get_document ();

		if (xmlDocument)
			xmlElement (xmlDocument -> get_root_node ());
	}
	catch (const X3DError & error)
	{
		throw;
	}
	catch (const std::exception & error)
	{
		throw Error <X3D::INVALID_X3D> (error .what ()); 
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown parser error.");
	}
}

void
XMLParser::xmlElement (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	const auto iter = xmlElementIndex .find (xmlElement -> get_name ());

	if (iter == xmlElementIndex .end ())
		nodeElement (xmlElement);
	else
		iter -> second (this, xmlElement);
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
		catch (const X3D::X3DError & error)
		{
			getBrowser () -> getConsole () -> warn (error .what (), "\n");
		}
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
		x3dElementChild (dynamic_cast <xmlpp::Element*> (xmlNode));

	popExecutionContext ();
}

void
XMLParser::x3dElementChild (xmlpp::Element* const xmlElement)
{
	try
	{
		if (not xmlElement)
			return;

		x3dElementChildIndex .at (xmlElement -> get_name ()) (this, xmlElement);
	}
	catch (const std::out_of_range &)
	{ }
}

void
XMLParser::headElement (xmlpp::Element* const xmlElement)
{
	for (const auto & xmlNode : xmlElement -> get_children ())
		headElementChild (dynamic_cast <xmlpp::Element*> (xmlNode));

	try
	{
		scene -> setWorldURL (scene -> getMetaData ("titania-identifier"));
	}
	catch (const X3DError &)
	{ }

	try
	{
		setUnits (scene -> getMetaData ("generator"));
	}
	catch (const X3DError &)
	{ }
}

void
XMLParser::headElementChild (xmlpp::Element* const xmlElement)
{
	try
	{
		if (not xmlElement)
			return;

		headElementsIndex .at (xmlElement -> get_name ()) (this, xmlElement);
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
				getBrowser () -> getConsole () -> warn (error .what (), "\n");
				return;
			}
		}
		else
			getBrowser () -> getConsole () -> warn ("Expected a component support level.\n");
	}
	else
		getBrowser () -> getConsole () -> warn ("Expected a component name.\n");
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
			long double unitConversionFactor;

			if (longDoubleAttribute (xmlElement -> get_attribute ("conversionFactor"), unitConversionFactor))
			{
				try
				{
					scene -> updateUnit (categoryNameCharacters, unitNameCharacters, unitConversionFactor);
					return;
				}
				catch (const X3DError & error)
				{
					getBrowser () -> getConsole () -> error (error .what (), "\n");
					return;
				}
			}
			else
				getBrowser () -> getConsole () -> warn ("Expected unit conversion factor.\n");
		}
		else
			getBrowser () -> getConsole () -> warn ("Expected unit name identificator.\n");
	}
	else
		getBrowser () -> getConsole () -> warn ("Expected category name identificator after UNIT statement.\n");
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
			getBrowser () -> getConsole () -> warn ("Expected metadata value.\n");
	}
	else
		getBrowser () -> getConsole () -> warn ("Expected metadata key.\n");
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
	{
		if (childElement (dynamic_cast <xmlpp::Element*> (xmlNode)))
			continue;

		if (cdataNode (dynamic_cast <xmlpp::CdataNode*> (xmlNode)))
			continue;
	}
}

bool
XMLParser::childElement (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return false;

	const auto iter = childElementIndex .find (xmlElement -> get_name ());

	if (iter == childElementIndex .end ())
		nodeElement (xmlElement);
	else
		iter -> second (this, xmlElement);

	return true;
}

void
XMLParser::externProtoDeclareElement (xmlpp::Element* const xmlElement)
{
	std::string nameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), nameCharacters))
	{
		std::string urlCharacters;
	
		if (stringAttribute (xmlElement -> get_attribute ("url"), urlCharacters))
		{
			std::istringstream istream (urlCharacters);

			istream .imbue (std::locale::classic ());

			MFString URLList;

			Parser (istream, scene) .sfstringValues (&URLList);

			// Create externproto and parse fields.

			const auto externproto = getExecutionContext () -> createExternProtoDeclaration (nameCharacters, { }, URLList);
						
			parents .emplace_back (externproto);

			protoInterfaceElement (xmlElement); // Parse fields.

			parents .pop_back ();
	
			getExecutionContext () -> updateExternProtoDeclaration (nameCharacters, externproto);
		}
		else
			getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad ExternProtoDeclare statement: Expected url attribute.\n");
	}
	else
		getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad ExternProtoDeclare statement: Expected name attribute.\n");
}

void
XMLParser::protoDeclareElement (xmlpp::Element* const xmlElement)
{
	std::string nameCharacters;

	if (stringAttribute (xmlElement -> get_attribute ("name"), nameCharacters))
	{
		const auto proto = getExecutionContext () -> createProtoDeclaration (nameCharacters, { });

		for (const auto & xmlNode : xmlElement -> get_children ())
		{
			const auto xmlElement = dynamic_cast <xmlpp::Element*> (xmlNode);

			if (not xmlElement)
				continue;

			if (xmlElement -> get_name () == "ProtoInterface")
			{
				parents .emplace_back (proto);

				protoInterfaceElement (xmlElement);

				parents .pop_back ();
				break;
			}
		}

		for (const auto & xmlNode : xmlElement -> get_children ())
		{
			const auto xmlElement = dynamic_cast <xmlpp::Element*> (xmlNode);

			if (not xmlElement)
				continue;

			if (xmlElement -> get_name () == "ProtoBody")
			{
				pushExecutionContext (proto);

				parents .emplace_back (proto);

				protoBodyElement (xmlElement);

				parents .pop_back ();

				popExecutionContext ();
				break;
			}
		}

		getExecutionContext () -> updateProtoDeclaration (nameCharacters, proto);
	}
	else
		getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad ProtoDeclare statement, expected name attribute.\n");
}

void
XMLParser::protoInterfaceElement (xmlpp::Element* const xmlElement)
{
	for (const auto & child : xmlElement -> get_children ())
		protoInterfaceElementChild (dynamic_cast <xmlpp::Element*> (child));
}

void
XMLParser::protoInterfaceElementChild (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	if (xmlElement -> get_name () == "field")
		fieldElement (xmlElement);
}

void
XMLParser::fieldElement (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	if (parents .empty ())
		return;

	const auto baseNode = dynamic_cast <X3DBaseNode*> (parents .back ());

	if (not baseNode)
		return;

	if (not baseNode -> canUserDefinedFields ())
		return;

	std::string accessTypeCharacters;
	AccessType  accessType;

	stringAttribute (xmlElement -> get_attribute ("accessType"), accessTypeCharacters);

	try
	{
		accessType = XMLGrammar::AccessTypes .at (accessTypeCharacters);
	}
	catch (const std::out_of_range &)
	{
		accessType = initializeOnly;
	}

	std::string               typeCharacters;
	const X3DFieldDefinition* type;

	stringAttribute (xmlElement -> get_attribute ("type"), typeCharacters);

	try
	{
		type = getBrowser () -> getSupportedField (typeCharacters);
	}
	catch (const X3DError &)
	{
		return;
	}

	try
	{
		std::string nameCharacters;

		stringAttribute (xmlElement -> get_attribute ("name"), nameCharacters);

		if (nameCharacters .empty ())
			return;

		const auto field = type -> create ();

		if (accessType & initializeOnly)
		{
			std::string valueCharacters;

			stringAttribute (xmlElement -> get_attribute ("value"), valueCharacters);

			fieldValue (field, valueCharacters);

			parents .emplace_back (field);

			childrenElements (xmlElement);

			parents .pop_back ();
		}

		baseNode -> addUserDefinedField (accessType, nameCharacters, field);
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error: Couldn't add user-defined field.\n", error .what (), "\n");
	}
}

void
XMLParser::protoBodyElement (xmlpp::Element* const xmlElement)
{
	childrenElements (xmlElement);
}

void
XMLParser::isElement (xmlpp::Element* const xmlElement)
{
	if (dynamic_cast <ProtoDeclaration*> (getExecutionContext ()))
	{
		for (const auto & xmlNode : xmlElement -> get_children ())
			isElementChild (dynamic_cast <xmlpp::Element*> (xmlNode));
	}
}

void
XMLParser::isElementChild (xmlpp::Element* const xmlElement)
{
	if (not xmlElement)
		return;

	if (xmlElement -> get_name () == "connect")
		connectElement (xmlElement);
}

void
XMLParser::connectElement (xmlpp::Element* const xmlElement)
{
	try
	{
		if (parents .empty ())
			return;

		const auto baseNode = dynamic_cast <X3DBaseNode*> (parents .back ());

		if (baseNode)
		{
			std::string nodeFieldCharacters;
	
			if (stringAttribute (xmlElement -> get_attribute ("nodeField"), nodeFieldCharacters))
			{
				std::string protoFieldCharacters;
		
				if (stringAttribute (xmlElement -> get_attribute ("protoField"), protoFieldCharacters))
				{
					const auto nodeField  = baseNode -> getField (nodeFieldCharacters);
					const auto protoField = getExecutionContext () -> getField (protoFieldCharacters);

					if (nodeField -> getType () == protoField -> getType ())
					{
						if (protoField -> isReference (nodeField -> getAccessType ()))
							nodeField -> addReference (protoField);
						else
							getBrowser () -> getConsole () -> warn ("Field '", nodeField -> getName (), "' and '", protoField -> getName (), "' in PROTO ", getExecutionContext () -> getName (), " are incompatible as an IS mapping.\n");
					}
					else
						getBrowser () -> getConsole () -> warn ("Field '", nodeField -> getName (), "' and '", protoField -> getName (), "' in PROTO ", getExecutionContext () -> getName (), " have different types.\n");
				}
				else
					getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad connect statement: Expected protoField attribute.\n");
			}
			else
				getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad connect statement: Expected nodeField attribute.\n");
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error: Couldn't create IS reference:\n", error .what (), "\n");
	}
}

void
XMLParser::protoInstanceElement (xmlpp::Element* const xmlElement)
{
	try
	{
		// USE property
	
		if (useAttribute (xmlElement))
			return;

		// Node object

		std::string nameCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("name"), nameCharacters))
		{
			const auto node = getExecutionContext () -> createProto (nameCharacters);

			defAttribute (xmlElement, node);

			addNode (xmlElement, node);

			parents .emplace_back (node);

			childrenElements (xmlElement);

			addUninitializedNode (node);

			parents .pop_back ();
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error:\n", error .what (), "\n");
	}
}

void
XMLParser::fieldValueElement (xmlpp::Element* const xmlElement)
{
	try
	{
		if (parents .empty ())
			return;
	
		const auto node = dynamic_cast <X3DPrototypeInstance*> (parents .back ());
	
		if (node)
		{
			std::string nameCharacters;
	
			if (stringAttribute (xmlElement -> get_attribute ("name"), nameCharacters))
			{
				const auto field = node -> getField (nameCharacters);

				if (field -> getAccessType () & initializeOnly)
				{
					std::string valueCharacters;

					if (stringAttribute (xmlElement -> get_attribute ("value"), valueCharacters))
						fieldValue (field, valueCharacters);

					parents .emplace_back (field);

					childrenElements (xmlElement);

					parents .pop_back ();
				}
			}
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error: Couldn't assign field value:\n", error .what (), "\n");
	}
}

void
XMLParser::nodeElement (xmlpp::Element* const xmlElement)
{
	try
	{
		// USE property
	
		if (useAttribute (xmlElement))
			return;
	
		// Node object
	
		const auto node = getExecutionContext () -> createNode (xmlElement -> get_name ());

		defAttribute (xmlElement, node);

		addNode (xmlElement, node);

		parents .emplace_back (node);

		nodeAttributes (xmlElement, node);

		childrenElements (xmlElement);

		addUninitializedNode (node);

		parents .pop_back ();
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error (error .what (), "\n");
	}
}

void
XMLParser::routeElement (xmlpp::Element* const xmlElement)
{
	try
	{
		std::string sourceNodeName;

		if (stringAttribute (xmlElement -> get_attribute ("fromNode"), sourceNodeName))
		{
			std::string sourceField;

			if (stringAttribute (xmlElement -> get_attribute ("fromField"), sourceField))
			{
				std::string destinationNodeName;

				if (stringAttribute (xmlElement -> get_attribute ("toNode"), destinationNodeName))
				{
					std::string destinationField;

					if (stringAttribute (xmlElement -> get_attribute ("toField"), destinationField))
					{
						try
						{
							const auto sourceNode       = getExecutionContext () -> getLocalNode (sourceNodeName);
							const auto destinationNode  = getExecutionContext () -> getLocalNode (destinationNodeName);
	
							getExecutionContext () -> addRoute (sourceNode, sourceField, destinationNode, destinationField);
						}
						catch (const Error <IMPORTED_NODE> & error)
						{
							// Imported nodes
						}
					}
					else
						getBrowser () -> getConsole () -> warn ("Bad ROUTE statement: Expected toField attribute.\n");
				}
				else
					getBrowser () -> getConsole () -> warn ("Bad ROUTE statement: Expected toNode attribute.\n");
			}
			else
				getBrowser () -> getConsole () -> warn ("Bad ROUTE statement: Expected fromField attribute.\n");
		}
		else
			getBrowser () -> getConsole () -> warn ("Bad ROUTE statement: Expected fromNode attribute.\n");
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error:\n", error .what (), "\n");
	}
}

void
XMLParser::importElement (xmlpp::Element* const xmlElement)
{
	try
	{
		std::string inlineDEFCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("inlineDEF"), inlineDEFCharacters))
		{
			std::string exportedDEFCharacters;
	
			if (stringAttribute (xmlElement -> get_attribute ("importedDEF"), exportedDEFCharacters) || stringAttribute (xmlElement -> get_attribute ("exportedDEF"), exportedDEFCharacters))
			{
				std::string ASCharacters;

				stringAttribute (xmlElement -> get_attribute ("AS"), ASCharacters);

				if (ASCharacters .empty ())
					ASCharacters = exportedDEFCharacters;

				const auto inlineNode = getExecutionContext () -> getNamedNode <Inline> (inlineDEFCharacters);

				getExecutionContext () -> updateImportedNode (inlineNode, exportedDEFCharacters, ASCharacters);
			}
			else
				getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad IMPORT statement, expected exportedDEF attribute.\n");
		}
		else
			getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad IMPORT statement, expected exportedDEF attribute.\n");
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error:\n", error .what (), "\n");
	}
}

void
XMLParser::exportElement (xmlpp::Element* const xmlElement)
{
	try
	{
		if (scene not_eq getExecutionContext ())
			return;

		std::string localDEFCharacters;

		if (stringAttribute (xmlElement -> get_attribute ("localDEF"), localDEFCharacters))
		{
			std::string ASCharacters;
	
			stringAttribute (xmlElement -> get_attribute ("AS"), ASCharacters);

			if (ASCharacters .empty ())
				ASCharacters = localDEFCharacters;

			const auto localNode = getExecutionContext () -> getLocalNode (localDEFCharacters);
	
			scene -> updateExportedNode (ASCharacters, localNode);
		}
		else
			getBrowser () -> getConsole () -> warn ("XML Parser Error: Bad EXPORT statement, expected localDEF attribute.\n");
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("XML Parser Error:\n", error .what (), "\n");
	}
}

bool
XMLParser::cdataNode (xmlpp::CdataNode* const cdataNode)
{
	if (not cdataNode)
		return false;

	if (parents .empty ())
		return true;

	const auto baseNode = dynamic_cast <X3DBaseNode*> (parents .back ());

	if (baseNode)
	{
		const auto field = baseNode -> getSourceText ();

		if (field)
		{
			field -> emplace_back (cdataNode -> get_content ());
			field -> isSet (true);
		}
	}

	return true;
}

bool
XMLParser::doubleAttribute (xmlpp::Attribute* const xmlAttribute, double & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return Grammar::Double (istream, value);
}

bool
XMLParser::longDoubleAttribute (xmlpp::Attribute* const xmlAttribute, long double & value)
{
	if (not xmlAttribute)
		return false;

	// TODO: Trim and check for count?

	std::istringstream istream (xmlAttribute -> get_value ());

	istream .imbue (std::locale::classic ());

	return Grammar::LongDouble (istream, value);
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
			FilterBadUTF8Characters (nodeNameCharacters);

			addNode (xmlElement, getExecutionContext () -> getNamedNode (nodeNameCharacters));
			return true;
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("Invalid USE name:\n", error .what (), "\n");
	}

	return false;
}	
	
void
XMLParser::defAttribute (xmlpp::Element* const xmlElement, X3DBaseNode* const baseNode)
{
	try
	{
		std::string nodeNameCharacters;
	
		if (stringAttribute (xmlElement -> get_attribute ("DEF"), nodeNameCharacters))
		{
			try
			{
				FilterBadUTF8Characters (nodeNameCharacters);

				const auto namedNode = getExecutionContext () -> getNamedNode (nodeNameCharacters);

				getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (nodeNameCharacters), namedNode);
			}
			catch (const X3DError & error)
			{ }

			getExecutionContext () -> updateNamedNode (nodeNameCharacters, SFNode (baseNode));
		}
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error ("Invalid DEF name:\n", error .what (), "\n");
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

	parser .setUnits (getUnits ());
	parser .pushExecutionContext (getExecutionContext ());

	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			parser .sfboolValue (static_cast <SFBool*> (field));
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
			parser .sfboolValues (static_cast <MFBool*> (field));
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
		{
			static const io::inverse_character q ('"');

			const auto mfstring = static_cast <MFString*> (field);

			mfstring -> clear ();

			std::string qs, string;

			if (q (istream, qs))
				istream .unget ();

			while (Grammar::String (istream, string))
			{
				FilterBadUTF8Characters (string);

				mfstring -> emplace_back (std::move (string));

				if (q (istream, qs))
					istream .unget ();
			}

			return;
		}
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

void
XMLParser::addNode (xmlpp::Element* const xmlElement, X3DBaseNode* const childNode)
{
	// ExecutionContext

	if (parents .empty () or dynamic_cast <ProtoDeclaration*> (parents .back ()))
	{
		getExecutionContext () -> getRootNodes () .emplace_back (childNode);
		return;
	}

	// Node

	{
		const auto baseNode = dynamic_cast <X3DBaseNode*> (parents .back ());
	
		if (baseNode)
		{
			try
			{
				std::string containerFieldCharacters;
			
				stringAttribute (xmlElement -> get_attribute ("containerField"), containerFieldCharacters);
	
				if (containerFieldCharacters .empty ())
					containerFieldCharacters = childNode -> getContainerField ();
	
				const auto field = baseNode -> getField (containerFieldCharacters);
	
				switch (field -> getType ())
				{
					case X3DConstants::SFNode:
					{
						const auto sfnode = static_cast <SFNode*> (field);
	
						sfnode -> setValue (childNode);
						sfnode -> isSet (true);
						return;
					}
					case X3DConstants::MFNode:
					{
						const auto mfnode = static_cast <MFNode*> (field);
	
						mfnode -> emplace_back (childNode);
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

	// Field

	{
		const auto field = dynamic_cast <X3DFieldDefinition*> (parents .back ());
	
		if (field)
		{
			switch (field -> getType ())
			{
				case X3DConstants::SFNode:
				{
					const auto sfnode = static_cast <SFNode*> (field);

					sfnode -> setValue (childNode);
					sfnode -> isSet (true);
					return;
				}
				case X3DConstants::MFNode:
				{
					const auto mfnode = static_cast <MFNode*> (field);

					mfnode -> emplace_back (childNode);
					mfnode -> isSet (true);
					return;
				}
				default:
					return;
			}

			return;
		}
	}
}

XMLParser::~XMLParser ()
{ }

} // X3D
} // titania
