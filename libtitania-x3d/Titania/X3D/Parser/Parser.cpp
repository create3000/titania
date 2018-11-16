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

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ExportedNode.h"
#include "../Execution/ImportedNode.h"
#include "../Parser/Filter.h"
#include "../Parser/Grammar.h"
#include "../Prototype/ProtoDeclaration.h"
#include "../Prototype/ExternProtoDeclaration.h"

namespace titania {
namespace X3D {

Parser::AccessTypes::AccessTypes ()
{
	(*this) ["field"]        = initializeOnly;
	(*this) ["eventIn"]      = inputOnly;
	(*this) ["eventOut"]     = outputOnly;
	(*this) ["exposedField"] = inputOutput;

	(*this) ["initializeOnly"] = initializeOnly;
	(*this) ["inputOnly"]      = inputOnly;
	(*this) ["outputOnly"]     = outputOnly;
	(*this) ["inputOutput"]    = inputOutput;
}

Parser::AccessTypes Parser::accessTypes;

Parser::Parser (std::istream & istream, X3DScene* scene) :
	            X3DParser (),
	              istream (istream),
	                scene (scene),
	           lineNumber (1),
	          whiteSpaces (),
	      currentComments (),
	    commentCharacters ()
{ }


///  throws Error <INVALID_X3D>
void
Parser::parseIntoScene ()
{
	//std::clog << "Parsing into scene: " << scene -> getWorldURL () << "." << std::endl;

	try
	{
		scene -> setEncoding (EncodingType::VRML);
		scene -> setProfile (getBrowser () -> getProfile ("Full"));

		istream .imbue (std::locale::classic ());

		x3dScene ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_X3D> (getMessageFromError (error));
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown parser error.");
	}
}

std::string
Parser::getMessageFromError (const X3DError & error)
{
	//__LOG__ << this << " " << std::endl;
	//__LOG__ << this << " " << istream .peek () << std::endl;

	istream .clear ();

	std::string string = error .what ();

	FilterControlCharacters (string);
	FilterBadUTF8Characters (string);

	std::ostringstream stringstream;

	stringstream .imbue (std::locale::classic ());

	try
	{
		std::string rest    = getline ();
		std::string line    = rgetline ();
		std::string preLine = rgetline ();
		size_t      linePos = line .size () - rest .size ();

		FilterControlCharacters (line);
		FilterControlCharacters (preLine);

		FilterBadUTF8Characters (line);
		FilterBadUTF8Characters (preLine);

		if (line .size () > 80)
		{
			line    = line .substr (linePos - 40, 80);
			preLine = std::string ();
			linePos = 40;
		}

		// Format error

		stringstream
			<< std::string (80, '*') << std::endl
			<< "Parser error at line " << lineNumber << ':' << linePos + 1 << std::endl
			<< "in '" << scene -> getWorldURL () << '\'' << std::endl
			<< std::endl
			<< preLine << std::endl
			<< line << std::endl
			<< std::string (linePos, ' ') << '^' << std::endl
			<< string << std::endl
			<< std::string (80, '*')
			<< std::endl;
	}
	catch (...)
	{
		stringstream
			<< std::string (80, '*') << std::endl
			<< "Parser error at line " << lineNumber << std::endl
			<< "in '" << scene -> getWorldURL () << '\'' << std::endl
			<< std::endl
			<< string << std::endl
			<< std::string (80, '*')
			<< std::endl;
	}

	return stringstream .str ();
}

std::string
Parser::getline ()
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for (; ;)
	{
		char c = istream .get ();

		if (static_cast <bool> (istream))
		{
			if (c == '\n' or c == '\r')
			{
				istream .unget ();
				break;
			}

			else
				string .push_back (c);
		}
		else
			break;
	}

	istream .clear ();

	return string;
}

///  throws std::out_of_range
std::string
Parser::rgetline ()
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for (; ;)
	{
		istream .unget ();

		char c = istream .peek ();

		if (static_cast <bool> (istream))
		{
			if (c == '\n' or c == '\r')
				break;

			else
				string .push_back (c);
		}
		else
			throw std::out_of_range ("rgetline");
	}

	istream .clear ();

	return std::string (string .crbegin (), string .crend ());
}

void
Parser::exception (const std::string & string)
{
	if (getBrowser () -> isStrict ())
		throw Error <INVALID_X3D> (string);

	getBrowser () -> getConsole () -> error (string);
}


void
Parser::x3dScene ()
{
	//__LOG__ << this << " " << std::endl;

	pushExecutionContext (scene);

	std::string encoding, specificationVersion, characterEncoding, comment;

	if (headerStatement (encoding, specificationVersion, characterEncoding, comment))
	{
		scene -> setSpecificationVersion (specificationVersion);
		scene -> setCharacterEncoding (characterEncoding);
		scene -> setComment (comment);
	}

	profileStatement ();
	componentStatements ();

	unitStatements ();
	metaStatements ();

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

	statements ();

	scene -> addInnerComments (getComments ());

	popExecutionContext ();

	if (istream .peek () not_eq std::char_traits <char>::eof ())
	{
		if (getBrowser () -> isStrict ())
			throw Error <INVALID_X3D> ("Unknown statement.");
	}
}

void
Parser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	while (comment ())
		;
}

bool
Parser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaces);

	lines (whiteSpaces);

	whiteSpaces .clear ();

	if (Grammar::Comment (istream, commentCharacters))
	{
		FilterControlCharacters (commentCharacters);
		FilterBadUTF8Characters (commentCharacters);
		currentComments .emplace_back (std::move (commentCharacters));
		return true;
	}

	return false;
}

void
Parser::lines (const std::string & string)
{
	//__LOG__ << this << " " << std::endl;

	lineNumber += std::count (string .cbegin (), string .cend (), '\n');
}

bool
Parser::headerStatement (std::string & _encoding, std::string & _specificationVersion, std::string & _characterEncoding, std::string & _comment)
{
	//__LOG__ << this << " " << std::endl;

	std::string _header;

	if (Grammar::Comment (istream, _header))
	{
		if (Grammar::Header (_header, _encoding, _specificationVersion, _characterEncoding, _comment))
			return true;
	}

	return false;
}

void
Parser::profileStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::PROFILE (istream))
	{
		std::string _profileNameId;

		if (profileNameId (_profileNameId))
		{
			try
			{
				scene -> addComments (getComments ());
				scene -> setProfile (getBrowser () -> getProfile (_profileNameId));
				return;
			}
			catch (const X3DError & error)
			{
				if (getBrowser () -> isStrict ())
					throw;

				getBrowser () -> getConsole () -> warn (error .what (), "\n");
				return;
			}
		}

		throw Error <INVALID_X3D> ("Expected a profile name.");
	}
}

void
Parser::componentStatements ()
{
	//__LOG__ << this << " " << std::endl;

	ComponentInfoPtr _componentStatement = componentStatement ();

	while (_componentStatement)
	{
		scene -> updateComponent (_componentStatement);
		_componentStatement = componentStatement ();
	}

}

ComponentInfoPtr
Parser::componentStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::COMPONENT (istream))
	{
		std::string _componentNameId;

		if (componentNameId (_componentNameId))
		{
			comments ();

			if (Grammar::Colon (istream))
			{
				int32_t _componentSupportLevel;

				if (componentSupportLevel (_componentSupportLevel))
				{
					scene -> addComments (getComments ());

					return getBrowser () -> getComponent (_componentNameId, _componentSupportLevel);
				}

				throw Error <INVALID_X3D> ("Expected a component support level.");
			}

			throw Error <INVALID_X3D> ("Expected a ':' after component name.");
		}

		throw Error <INVALID_X3D> ("Expected a component name.");
	}

	return nullptr;
}

bool
Parser::componentNameId (std::string & id)
{
	comments ();

	return Grammar::ComponentName (istream, id);
}

bool
Parser::componentSupportLevel (int32_t & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	return static_cast <bool> (istream >> _value);
}

void
Parser::unitStatements ()
{
	//__LOG__ << this << " " << std::endl;

	while (unitStatement ())
		;
}

bool
Parser::unitStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::UNIT (istream))
	{
		std::string _categoryNameId;

		if (categoryNameId (_categoryNameId))
		{
			std::string _unitNameId;

			if (unitNameId (_unitNameId))
			{
				long double _unitConversionFactor;

				if (unitConversionFactor (_unitConversionFactor))
				{
				   try
				   {
						scene -> updateUnit (_categoryNameId, _unitNameId, _unitConversionFactor);
						return true;
					}
					catch (const X3DError & error)
					{
						getBrowser () -> getConsole () -> warn (error .what (), "\n");
						return true;
					}
				}

				throw Error <INVALID_X3D> ("Expected unit conversion factor.");
			}

			throw Error <INVALID_X3D> ("Expected unit name identificator.");
		}

		throw Error <INVALID_X3D> ("Expected category name identificator after UNIT statement.");
	}

	return false;
}

bool
Parser::exportStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::EXPORT (istream))
	{
		std::string _localNodeNameId;

		if (nodeNameId (_localNodeNameId))
		{
			std::string _exportedNodeNameId;

			comments ();

			const SFNode node = scene -> getLocalNode (_localNodeNameId);

			if (Grammar::AS (istream))
			{
				if (not exportedNodeNameId (_exportedNodeNameId))
					throw Error <INVALID_X3D> ("No name given after AS.");
			}
			else
				_exportedNodeNameId = _localNodeNameId;

			const ExportedNodePtr & _exportedNode = scene -> updateExportedNode (_exportedNodeNameId, node);

			_exportedNode -> addComments (getComments ());

			return true;
		}

		throw Error <INVALID_X3D> ("No name given after EXPORT.");
	}

	return false;
}

bool
Parser::importStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::IMPORT (istream))
	{
		std::string _inlineNodeNameId;

		if (inlineNodeNameId (_inlineNodeNameId))
		{
			const auto _inlineNode = getExecutionContext () -> getNamedNode <Inline> (_inlineNodeNameId);

			if (_inlineNode)
			{
				comments ();

				if (Grammar::Period (istream))
				{
					std::string _exportedNodeNameId;

					if (exportedNodeNameId (_exportedNodeNameId))
					{
						std::string _nodeNameId;

						comments ();

						if (Grammar::AS (istream))
						{
							if (not nodeNameId (_nodeNameId))
								throw Error <INVALID_X3D> ("No name given after AS.");
						}

						const ImportedNodePtr & _importedNode = getExecutionContext () -> updateImportedNode (_inlineNode, _exportedNodeNameId, _nodeNameId);

						_importedNode -> addComments (getComments ());

						return true;
					}

					throw Error <INVALID_X3D> ("Expected exported node name.");
				}

				throw Error <INVALID_X3D> ("Expected a '.' after exported node name.");
			}

			throw Error <INVALID_X3D> ("Node named '" + _inlineNodeNameId + "' is not an Inline node.");
		}

		throw Error <INVALID_X3D> ("No name given after IMPORT statement.");
	}

	return false;
}

void
Parser::metaStatements ()
{
	//__LOG__ << this << " " << std::endl;

	while (metaStatement ())
		;
}

bool
Parser::metaStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::META (istream))
	{
		std::string _metakey;

		if (metakey (_metakey))
		{
			std::string _metavalue;

			if (metavalue (_metavalue))
			{
				scene -> addComments (getComments ());
				scene -> addMetaData (_metakey, _metavalue);
				return true;
			}

			throw Error <INVALID_X3D> ("Expected metadata value.");
		}

		throw Error <INVALID_X3D> ("Expected metadata key.");
	}

	return false;
}

void
Parser::statements ()
{
	//__LOG__ << this << " " << std::endl;

	while (statement ())
		;

	//__LOG__ << this << " " << std::endl;
}

bool
Parser::statement ()
{
	//__LOG__ << this << " " << std::endl;

	if (protoStatement ())
		return true;

	if (routeStatement ())
		return true;

	if (importStatement ())
		return true;

	if (exportStatement ())
		return true;

	SFNode _node;

	if (nodeStatement (_node))
	{
		addRootNode (std::move (_node));
		return true;
	}

	return false;
}

bool
Parser::nodeStatement (SFNode & _node)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::DEF (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			FilterBadUTF8Characters (_nodeNameId);

			if (node (_node, _nodeNameId))
				return true;

			throw Error <INVALID_X3D> ("Expected node name after DEF.");
		}

		throw Error <INVALID_X3D> ("No name given after DEF.");
	}

	if (Grammar::USE (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			FilterBadUTF8Characters (_nodeNameId);

			_node = getExecutionContext () -> getNamedNode (_nodeNameId);

			return true;
		}

		throw Error <INVALID_X3D> ("No name given after USE.");
	}

	if (Grammar::NULL_ (istream))
	{
		_node = nullptr;

		return true;
	}

	if (node (_node))
		return true;

	return false;
}

bool
Parser::rootNodeStatement (SFNode & _node)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::DEF (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			if (node (_node, _nodeNameId))
				return true;

			throw Error <INVALID_X3D> ("Expected node type name after DEF.");
		}

		throw Error <INVALID_X3D> ("No name given after DEF.");
	}

	if (node (_node))
		return true;

	return false;
}

bool
Parser::protoStatement ()
{
	//__LOG__ << this << " " << std::endl;

	if (proto ())
		return true;

	if (externproto ())
		return true;

	return false;
}

void
Parser::protoStatements ()
{
	//__LOG__ << this << " " << std::endl;

	while (protoStatement ())
		;
}

bool
Parser::proto ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::PROTO (istream))
	{
		std::string _nodeTypeId;

		if (nodeTypeId (_nodeTypeId))
		{
			FilterBadUTF8Characters (_nodeTypeId);

			comments ();

			if (Grammar::OpenBracket (istream))
			{
				auto _comments = std::move (getComments ());

				FieldDefinitionArray _interfaceDeclarations = interfaceDeclarations ();

				comments ();

				if (Grammar::CloseBracket (istream))
				{
					auto _interfaceComments = std::move (getComments ());

					comments ();

					if (Grammar::OpenBrace (istream))
					{
						const ProtoDeclarationPtr _proto = getExecutionContext () -> createProtoDeclaration (_nodeTypeId, _interfaceDeclarations);

						pushExecutionContext (_proto);

						protoBody ();

						popExecutionContext ();

						comments ();

						if (Grammar::CloseBrace (istream))
						{
							//__LOG__ << this << " " << (void*) _proto .getValue () << " " << _nodeTypeId << std::endl;

							getExecutionContext () -> updateProtoDeclaration (_nodeTypeId, _proto);

							_proto -> addInterfaceComments (_interfaceComments);
							_proto -> addComments (_comments);
							_proto -> addInnerComments (getComments ());

							return true;
						}

						throw Error <INVALID_X3D> ("Expected a '}' at the end of PROTO body.");
					}

					throw Error <INVALID_X3D> ("Expected a '{' at the beginning of PROTO body.");
				}

				throw Error <INVALID_X3D> ("Expected a ']' at the end of PROTO interface declaration.");
			}

			throw Error <INVALID_X3D> ("Expected a '[' at the beginning of PROTO interface declaration.");
		}

		throw Error <INVALID_X3D> ("Invalid PROTO definition name.");
	}

	return false;
}

void
Parser::protoBody ()
{
	//__LOG__ << this << " " << std::endl;

	protoStatements ();

	SFNode _rootNodeStatement;

	if (rootNodeStatement (_rootNodeStatement))
		addRootNode (std::move (_rootNodeStatement));

	statements ();
}

FieldDefinitionArray
Parser::interfaceDeclarations ()
{
	//__LOG__ << this << " " << std::endl;

	FieldDefinitionArray _interfaceDeclarations;
	X3DFieldDefinition*  _field = interfaceDeclaration ();

	while (_field)
	{
		_interfaceDeclarations .emplace_back (_field);
		_field = interfaceDeclaration ();
	}

	return _interfaceDeclarations;
}

X3DFieldDefinition*
Parser::restrictedInterfaceDeclaration ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::inputOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::outputOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::initializeOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();

				if (fieldValue (_field))
				{
					FilterBadUTF8Characters (_fieldId);

					_field -> setAccessType (initializeOnly);
					_field -> setName (_fieldId);
					_field -> addComments (getComments ());
					return _field;
				}

				delete _field;

				throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	return nullptr;
}

X3DFieldDefinition*
Parser::interfaceDeclaration ()
{
	//__LOG__ << this << " " << std::endl;

	X3DFieldDefinition* _field = restrictedInterfaceDeclaration ();

	if (_field)
		return _field;

	comments ();

	if (Grammar::inputOutput (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();

				if (fieldValue (_field))
				{
					FilterBadUTF8Characters (_fieldId);

					_field -> setAccessType (inputOutput);
					_field -> setName (_fieldId);
					_field -> addComments (getComments ());
					return _field;
				}

				delete _field;

				throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	return nullptr;
}

bool
Parser::externproto ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::EXTERNPROTO (istream))
	{
		std::string _nodeTypeId;

		if (nodeTypeId (_nodeTypeId))
		{
			FilterBadUTF8Characters (_nodeTypeId);

			comments ();

			if (Grammar::OpenBracket (istream))
			{
				auto _comments = std::move (getComments ());

				FieldDefinitionArray _externInterfaceDeclarations = std::move (externInterfaceDeclarations ());

				comments ();

				if (Grammar::CloseBracket (istream))
				{
					auto _interfaceComments = std::move (getComments ());

					MFString _URLList;

					if (URLList (&_URLList))
					{
						const ExternProtoDeclarationPtr _externProto = getExecutionContext () -> createExternProtoDeclaration (_nodeTypeId, _externInterfaceDeclarations, _URLList);

						getExecutionContext () -> updateExternProtoDeclaration (_nodeTypeId, _externProto);

						_externProto -> addInterfaceComments (_interfaceComments);
						_externProto -> addComments (_comments);
						_externProto -> addInnerComments (getComments ());

						return true;
					}

					throw Error <INVALID_X3D> ("Expected a URL list after EXTERNPROTO interface declaration '" + _nodeTypeId + "'.");
				}

				throw Error <INVALID_X3D> ("Expected a ']' at the end of EXTERNPROTO interface declaration.");
			}

			throw Error <INVALID_X3D> ("Expected a '[' at the beginning of EXTERNPROTO interface declaration.");
		}

		throw Error <INVALID_X3D> ("Invalid EXTERNPROTO definition name.");
	}

	return false;
}

FieldDefinitionArray
Parser::externInterfaceDeclarations ()
{
	//__LOG__ << this << " " << std::endl;

	FieldDefinitionArray _externInterfaceDeclarations;
	X3DFieldDefinition*  _field = externInterfaceDeclaration ();

	while (_field)
	{
		_externInterfaceDeclarations .emplace_back (_field);
		_field = externInterfaceDeclaration ();
	}

	return _externInterfaceDeclarations;
}

X3DFieldDefinition*
Parser::externInterfaceDeclaration ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::inputOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::outputOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::initializeOnly (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (initializeOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::inputOutput (istream) .second)
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				FilterBadUTF8Characters (_fieldId);

				X3DFieldDefinition* _field = getBrowser () -> getSupportedField (_fieldType) -> create ();
				_field -> setAccessType (inputOutput);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}

			throw Error <INVALID_X3D> ("Expected a name for field.");
		}

		throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	return nullptr;
}

bool
Parser::routeStatement ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::ROUTE (istream))
	{
		std::string _fromNodeId;

		if (nodeNameId (_fromNodeId))
		{
			FilterBadUTF8Characters (_fromNodeId);

			const SFNode _fromNode = getExecutionContext () -> getLocalNode (_fromNodeId);

			comments ();

			if (Grammar::Period (istream))
			{
				std::string _eventOutId;

				if (outputOnlyId (_eventOutId))
				{
					FilterBadUTF8Characters (_eventOutId);

					comments ();

					if (Grammar::TO (istream))
					{
						std::string _toNodeId;

						if (nodeNameId (_toNodeId))
						{
							FilterBadUTF8Characters (_toNodeId);

							const SFNode _toNode = getExecutionContext () -> getLocalNode (_toNodeId);

							comments ();

							if (Grammar::Period (istream))
							{
								std::string _eventInId;

								if (inputOnlyId (_eventInId))
								{
									FilterBadUTF8Characters (_eventInId);

									try
									{
										const RoutePtr & _route = getExecutionContext () -> addRoute (_fromNode, _eventOutId, _toNode, _eventInId);
	
										_route -> addComments (getComments ());
	
										return true;
									}
									catch (const Error <IMPORTED_NODE> & error)
									{
										// Imported nodes
										return true;
									}
									catch (const X3DError & error)
									{
										exception (error .what ());

										return true;
									}
								}

								throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a field name.");
							}

							throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a '.' after node name.");
						}

						throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a node name.");
					}

					throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a 'TO'.");
				}

				throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a field name.");
			}

			throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a '.' after node name.");
		}

		throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a node name.");
	}

	return false;
}

bool
Parser::URLList (MFString* _value)
{
	//__LOG__ << this << " " << std::endl;

	return mfstringValue (_value);
}

//Nodes

bool
Parser::node (SFNode & _node, const std::string & _nodeNameId)
{
	//__LOG__ << this << " " << _nodeNameId << std::endl;

	std::string _nodeTypeId;

	if (nodeTypeId (_nodeTypeId))
	{
		// //__LOG__ << this << " " << _nodeTypeId << std::endl;

		try
		{
			_node = getExecutionContext () -> createNode (_nodeTypeId);
		}
		catch (const X3DError & error1)
		{
			// //__LOG__ << this << " " << error .what () << std::endl;

			try
			{
				_node = getExecutionContext () -> createProto (_nodeTypeId);
			}
			catch (const X3DError & error2)
			{
				throw Error <INVALID_X3D> (error1 .what () + std::string ("\n") + error2 .what ());
			}
		}

		//__LOG__ << this << " " << _nodeTypeId << " " << (void*) _node << std::endl;

		if (not _nodeNameId .empty ())
		{
			try
			{
				const SFNode namedNode = getExecutionContext () -> getNamedNode (_nodeNameId); // Create copy!

				getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (_nodeNameId), namedNode);
			}
			catch (const X3DError &)
			{ }

			getExecutionContext () -> updateNamedNode (_nodeNameId, _node);
		}

		comments ();

		if (Grammar::OpenBrace (istream))
		{
			const auto _baseNode = _node .getValue ();

			_baseNode -> addComments (getComments ());

			if (_baseNode -> canUserDefinedFields ())
				scriptBody (_baseNode);

			else
				nodeBody (_baseNode);

			comments ();

			if (Grammar::CloseBrace (istream))
			{
				//__LOG__ << this << " " << _nodeTypeId << std::endl;

				_baseNode -> addInnerComments (getComments ());

				addUninitializedNode (_baseNode);

				//__LOG__ << this << " " << _nodeTypeId << std::endl;
				return true;
			}

			throw Error <INVALID_X3D> ("Expected '}' at the end of node body.");
		}

		throw Error <INVALID_X3D> ("Expected '{' at the beginning of node body.");
	}

	//__LOG__ << this << " " << std::endl;
	return false;
}

void
Parser::scriptBody (X3DBaseNode* const _baseNode)
{
	//__LOG__ << this << " " << std::endl;

	while (scriptBodyElement (_baseNode))
		;
}

bool
Parser::scriptBodyElement (X3DBaseNode* const _baseNode)
{
	//__LOG__ << this << " " << std::endl;

	const auto state = istream .rdstate ();
	const auto pos   = istream .tellg ();
	const auto ln    = lineNumber;
	const auto com   = currentComments .size ();

	std::string _accessTypeId;

	if (Id (_accessTypeId))
	{
		auto _accessType = accessTypes .find (_accessTypeId);

		if (_accessType not_eq accessTypes .end ())
		{
			std::string _fieldType;

			if (fieldType (_fieldType))
			{
				std::string _fieldId;

				if (Id (_fieldId))
				{
					comments ();

					if (Grammar::IS (istream))
					{
						if (isInsideProtoDefinition ())
						{
							std::string _isId;

							if (Id (_isId))
							{
								X3DFieldDefinition* _reference = nullptr;

								try
								{
									_reference = getExecutionContext () -> getField (_isId);
								}
								catch (const Error <INVALID_NAME> &)
								{
									exception ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName () + " interface declaration.");
									
									return true;
								}

								const X3DFieldDefinition* _supportedField = getBrowser () -> getSupportedField (_fieldType);

								if (_supportedField -> getType () == _reference -> getType ())
								{
									if (_reference -> isReference (_accessType -> second))
									{
										X3DFieldDefinition* _field = nullptr;

										try
										{
											_field = _baseNode -> getField (_fieldId);

											//if (_field -> getAccessType () not_eq _accessType -> second)
											//	throw Error <INVALID_X3D> ("Field '" + _fieldId + "' must have access type " + toString (_field -> getAccessType ()) + ".");

											// experimental see events test and vrml logo test
											{
												if (_reference -> getType () == _field -> getType ())
												{
													if (_accessType -> second == _field -> getAccessType ())
														;
													else if (_field -> getAccessType () == inputOutput)
													{
														if (_accessType -> second not_eq _field -> getAccessType ())
															_field = createUserDefinedField (_baseNode, _accessType -> second, _fieldId, _supportedField);
													}
													else
													{
														exception ("Field '" + _fieldId + "' must have access type " + to_string (_field -> getAccessType ()) + ".");

														return true;
													}
												}
												else
													_field = createUserDefinedField (_baseNode, _accessType -> second, _fieldId, _supportedField);
											}
										}
										catch (const Error <INVALID_NAME> &)
										{
											_field = createUserDefinedField (_baseNode, _accessType -> second, _fieldId, _supportedField);
										}

										_field -> addReference (_reference);
										_field -> addComments (getComments ());

										return true;
									}

									throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' are incompatible as an IS mapping.");
								}

								throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' have different types.");
							}

							throw Error <INVALID_X3D> ("No name give after IS statement.");
						}

						throw Error <INVALID_X3D> ("IS statement outside PROTO definition.");
					}
				}
			}
		}

		// Reset stream position.

		lineNumber = ln;
		currentComments .resize (com);

		istream .clear (state);

		for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
			istream .unget ();
	}

	X3DFieldDefinition* _field = interfaceDeclaration ();

	if (_field)
	{
		try
		{
			if (_field -> getAccessType () == inputOutput)
			{
				X3DFieldDefinition* _existingField = _baseNode -> getField (_field -> getName ());

				if (_existingField -> getAccessType () == inputOutput)
				{
					if (_field -> getType () == _existingField -> getType ())
					{
						_existingField -> set (*_field);
						_existingField -> isSet (true);

						_field -> dispose ();

						_field -> addDisposedObject (_field);

						return true;
					}
				}
			}
		}
		catch (const Error <INVALID_NAME> &)
		{ }

		_baseNode -> addUserDefinedField (_field -> getAccessType (),
		                                  _field -> getName (),
		                                  _field);

		return true;
	}

	return nodeBodyElement (_baseNode);
}

X3DFieldDefinition*
Parser::createUserDefinedField (X3DBaseNode* const _baseNode, const AccessType _accessType, const std::string & _fieldId, const X3DFieldDefinition* const _supportedField)
{
	X3DFieldDefinition* const _field = _supportedField -> create ();

	_baseNode -> addUserDefinedField (_accessType, _fieldId, _field);

	return _field;
}

void
Parser::nodeBody (X3DBaseNode* const _baseNode)
{
	//__LOG__ << this << " " << std::endl;

	while (nodeBodyElement (_baseNode))
		;
}

bool
Parser::nodeBodyElement (X3DBaseNode* const _baseNode)
{
	//__LOG__ << this << " " << std::endl;

	if (protoStatement ())
		return true;

	if (routeStatement ())
		return true;

	std::string _fieldId;

	if (Id (_fieldId))
	{
		X3DFieldDefinition* _field = nullptr;

		try
		{
			_field = _baseNode -> getField (_fieldId);
		}
		catch (const Error <INVALID_NAME> &)
		{
			throw Error <INVALID_X3D> ("Unknown field '" + _fieldId + "' in class '" + _baseNode -> getTypeName () + "'.");
		}

		comments ();

		if (Grammar::IS (istream))
		{
			if (isInsideProtoDefinition ())
			{
				std::string _isId;

				if (Id (_isId))
				{
					X3DFieldDefinition* _reference = nullptr;

					try
					{
						_reference = getExecutionContext () -> getField (_isId);
					}
					catch (const Error <INVALID_NAME> &)
					{
						exception ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName ());

						return true;
					}

					if (_field -> getType () == _reference -> getType ())
					{
						if (_reference -> isReference (_field -> getAccessType ()))
						{
							_field -> addReference (_reference);
							_field -> addComments (getComments ());
							return true;
						}

						throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " are incompatible as an IS mapping.");
					}

					throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " have different types.");
				}

				throw Error <INVALID_X3D> ("No name give after IS statement.");
			}

			throw Error <INVALID_X3D> ("IS statement outside PROTO definition.");
		}

		if (_field -> isInitializable ())
		{
			if (fieldValue (_field))
			{
				_field -> addComments (getComments ());
				return true;
			}

			throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
		}

		throw Error <INVALID_X3D> ("Couldn't assign value to " + to_string (_field -> getAccessType ()) + " field '" + _fieldId + "'.");
	}

	return false;
}

//Ids

bool
Parser::Id (std::string & _Id)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (static_cast <bool> (istream))
	{
		std::istream::int_type c = istream .peek ();

		switch (c)
		{
			case -1:
			case '\x22':
			case '\x23':
			case '\x27':
			case '\x2b':
			case '\x2c':
			case '\x2d':
			case '\x2e':
			case '\x5b':
			case '\x5c':
			case '\x5d':
			case '\x7b':
			case '\x7d':
			case '\x7f':
			{
				return false;
			}
			default:
			{
				if ((c >= '\x00' and c <= '\x20')or (c >= '\x30' and c <= '\x39'))
				{
					return false;
				}

				_Id .push_back (istream .get ());
			}
		}
	}
	else
		return false;

	for (; static_cast <bool> (istream);)
	{
		if (static_cast <bool> (istream)) // XXX: Tested already in the for loop, delete this line!!!
		{
			std::istream::int_type c = istream .peek ();

			switch (c)
			{
				case -1:
				case '\x22':
				case '\x23':
				case '\x27':
				case '\x2c':
				case '\x2e':
				case '\x5b':
				case '\x5c':
				case '\x5d':
				case '\x7b':
				case '\x7d':
				case '\x7f':
				{
					return true;
				}
				default:
				{
					if ((c >= '\x00' and c <= '\x20'))
					{
						return true;
					}

					_Id .push_back (istream .get ());
				}
			}
		}
		else
			return true;
	}

	return true;
}

bool
Parser::fieldType (std::string & _fieldType)
{
	//__LOG__ << this << " " << std::endl;

	Id (_fieldType);

	return Grammar::SupportedFields .count (_fieldType);
}

bool
Parser::fieldValue (X3DFieldDefinition* _field)
{
	//__LOG__ << this << " " << std::endl;
	//__LOG__ << this << " " << _field -> getTypeName () << std::endl;
	//__LOG__ << this << " " << _field -> getName () << std::endl;

	_field -> addComments (getComments ());
	_field -> isSet (true);

	switch (_field -> getType ())
	{
		case X3DConstants::SFBool:
			return sfboolValue (static_cast <SFBool*> (_field));

		case X3DConstants::SFColor:
			return sfcolorValue (static_cast <SFColor*> (_field));

		case X3DConstants::SFColorRGBA:
			return sfcolorRGBAValue (static_cast <SFColorRGBA*> (_field));

		case X3DConstants::SFDouble:
			return sfdoubleValue (static_cast <SFDouble*> (_field));

		case X3DConstants::SFFloat:
			return sffloatValue (static_cast <SFFloat*> (_field));

		case X3DConstants::SFImage:
			return sfimageValue (static_cast <SFImage*> (_field));

		case X3DConstants::SFInt32:
			return sfint32Value (static_cast <SFInt32*> (_field));

		case X3DConstants::SFMatrix3d:
			return sfmatrix3dValue (static_cast <SFMatrix3d*> (_field));

		case X3DConstants::SFMatrix3f:
			return sfmatrix3fValue (static_cast <SFMatrix3f*> (_field));

		case X3DConstants::SFMatrix4d:
			return sfmatrix4dValue (static_cast <SFMatrix4d*> (_field));

		case X3DConstants::SFMatrix4f:
			return sfmatrix4fValue (static_cast <SFMatrix4f*> (_field));

		case X3DConstants::SFNode:
			return sfnodeValue (static_cast <SFNode*> (_field));

		case X3DConstants::SFRotation:
			return sfrotationValue (static_cast <SFRotation*> (_field));

		case X3DConstants::SFString:
			return sfstringValue (static_cast <SFString*> (_field));

		case X3DConstants::SFTime:
			return sftimeValue (static_cast <SFTime*> (_field));

		case X3DConstants::SFVec2d:
			return sfvec2dValue (static_cast <SFVec2d*> (_field));

		case X3DConstants::SFVec2f:
			return sfvec2fValue (static_cast <SFVec2f*> (_field));

		case X3DConstants::SFVec3d:
			return sfvec3dValue (static_cast <SFVec3d*> (_field));

		case X3DConstants::SFVec3f:
			return sfvec3fValue (static_cast <SFVec3f*> (_field));

		case X3DConstants::SFVec4d:
			return sfvec4dValue (static_cast <SFVec4d*> (_field));

		case X3DConstants::SFVec4f:
			return sfvec4fValue (static_cast <SFVec4f*> (_field));

		case X3DConstants::MFBool:
			return mfboolValue (static_cast <MFBool*> (_field));

		case X3DConstants::MFColor:
			return mfcolorValue (static_cast <MFColor*> (_field));

		case X3DConstants::MFColorRGBA:
			return mfcolorRGBAValue (static_cast <MFColorRGBA*> (_field));

		case X3DConstants::MFDouble:
			return mfdoubleValue (static_cast <MFDouble*> (_field));

		case X3DConstants::MFFloat:
			return mffloatValue (static_cast <MFFloat*> (_field));

		case X3DConstants::MFImage:
			return mfimageValue (static_cast <MFImage*> (_field));

		case X3DConstants::MFInt32:
			return mfint32Value (static_cast <MFInt32*> (_field));

		case X3DConstants::MFMatrix3d:
			return mfmatrix3dValue (static_cast <MFMatrix3d*> (_field));

		case X3DConstants::MFMatrix3f:
			return mfmatrix3fValue (static_cast <MFMatrix3f*> (_field));

		case X3DConstants::MFMatrix4d:
			return mfmatrix4dValue (static_cast <MFMatrix4d*> (_field));

		case X3DConstants::MFMatrix4f:
			return mfmatrix4fValue (static_cast <MFMatrix4f*> (_field));

		case X3DConstants::MFNode:
			return mfnodeValue (static_cast <MFNode*> (_field));

		case X3DConstants::MFRotation:
			return mfrotationValue (static_cast <MFRotation*> (_field));

		case X3DConstants::MFString:
			return mfstringValue (static_cast <MFString*> (_field));

		case X3DConstants::MFTime:
			return mftimeValue (static_cast <MFTime*> (_field));

		case X3DConstants::MFVec2d:
			return mfvec2dValue (static_cast <MFVec2d*> (_field));

		case X3DConstants::MFVec2f:
			return mfvec2fValue (static_cast <MFVec2f*> (_field));

		case X3DConstants::MFVec3d:
			return mfvec3dValue (static_cast <MFVec3d*> (_field));

		case X3DConstants::MFVec3f:
			return mfvec3fValue (static_cast <MFVec3f*> (_field));

		case X3DConstants::MFVec4d:
			return mfvec4dValue (static_cast <MFVec4d*> (_field));

		case X3DConstants::MFVec4f:
			return mfvec4fValue (static_cast <MFVec4f*> (_field));
	}

	throw Error <INVALID_X3D> ("Unknown field type.");
}

// fieldValues

bool
Parser::LongDouble (long double & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::LongDouble (istream, _value))
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Double (double & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Double (istream, _value))
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Float (float & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Float (istream, _value))
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Int32 (int32_t & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Int32 (istream, _value))
		return true;

	istream .clear ();

	return false;
}

bool
Parser::String (std::string & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::String (istream, _value))
	{
		FilterBadUTF8Characters (_value);
		lines (_value);
		return true;
	}

	return false;
}

bool
Parser::sfboolValue (SFBool* _field)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::TRUE_ (istream) .second)
	{
		_field -> setValue (true);
		return true;
	}

	if (Grammar::FALSE_ (istream) .second)
	{
		_field -> setValue (false);
		return true;
	}

	return false;
}

bool
Parser::mfboolValue (MFBool* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFBool value;

	if (sfboolValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfboolValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfboolValues (MFBool* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFBool value;

	_field -> clear ();

	while (sfboolValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfcolorValue (SFColor* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColor::value_type r, g, b;

	if (Float (r))
	{
		if (Float (g))
		{
			if (Float (b))
			{
				_field -> setValue (Color3f (r, g, b));
				return true;
			}
		}

		exception ("To less values, must be a multiple of 3.");
	}

	return false;
}

bool
Parser::mfcolorValue (MFColor* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFColor value;

	if (sfcolorValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfcolorValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorValues (MFColor* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColor value;

	_field -> clear ();

	while (sfcolorValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfcolorRGBAValue (SFColorRGBA* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColorRGBA::value_type r, g, b, a;

	if (Float (r))
	{
		if (Float (g))
		{
			if (Float (b))
			{
				if (Float (a))
				{
					_field -> setValue (Color4f (r, g, b, a));
					return true;
				}
			}
		}

		exception ("To less values, must be a multiple of 4.");
	}

	return false;
}

bool
Parser::mfcolorRGBAValue (MFColorRGBA* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFColorRGBA value;

	if (sfcolorRGBAValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfcolorRGBAValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorRGBAValues (MFColorRGBA* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColorRGBA value;

	_field -> clear ();

	while (sfcolorRGBAValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfdoubleValue (SFDouble* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFDouble::value_type value;

	if (Double (value))
	{
		_field -> setValue (fromUnit (_field -> getUnit (), value));
		return true;
	}

	return false;
}

bool
Parser::mfdoubleValue (MFDouble* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFDouble value;

	value .setUnit (_field -> getUnit ());

	if (sfdoubleValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfdoubleValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfdoubleValues (MFDouble* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFDouble value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfdoubleValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sffloatValue (SFFloat* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFFloat::value_type value;

	if (Float (value))
	{
		_field -> setValue (fromUnit (_field -> getUnit (), value));
		return true;
	}

	return false;
}

bool
Parser::mffloatValue (MFFloat* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFFloat value;

	value .setUnit (_field -> getUnit ());

	if (sffloatValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sffloatValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sffloatValues (MFFloat* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFFloat value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sffloatValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfimageValue (SFImage* _field)
{
	//__LOG__ << this << " " << std::endl;

	int32_t width, height, components, pixel;

	if (Int32 (width))
	{
		if (Int32 (height))
		{
			if (Int32 (components))
			{
				_field -> setWidth (width);
				_field -> setHeight (height);
				_field -> setComponents (components);
		
				MFInt32 & array = _field -> getArray ();

				for (size_t i = 0, size = array .size (); i < size; ++ i)
				{
					if (Int32 (pixel))
						array [i] = pixel;
				
					else
						throw Error <INVALID_X3D> ("Expected more pixel values.");
				}

				return true;
			}
		}
	}

	return false;
}

bool
Parser::mfimageValue (MFImage* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFImage value;

	if (sfimageValue (&value))
	{
		_field -> emplace_back (std::move (value));
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfimageValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfimageValues (MFImage* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFImage value;

	_field -> clear ();

	while (sfimageValue (&value))
		_field -> emplace_back (std::move (value));

	_field -> shrink_to_fit ();
}

bool
Parser::sfint32Value (SFInt32* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFInt32::value_type value;

	if (Int32 (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mfint32Value (MFInt32* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFInt32 value;

	if (sfint32Value (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfint32Values (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfint32Values (MFInt32* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFInt32 value;

	_field -> clear ();

	while (sfint32Value (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfmatrix3dValue (SFMatrix3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3d::value_type e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (Double (e11))
	{
		if (Double (e12))
		{
			if (Double (e13))
			{
				if (Double (e21))
				{
					if (Double (e22))
					{
						if (Double (e23))
						{
							if (Double (e31))
							{
								if (Double (e32))
								{
									if (Double (e33))
									{
										_field -> setValue (Matrix3d (e11, e12, e13, e21, e22, e23, e31, e32, e33));
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		exception ("To less values, must be a multiple of 9.");
	}

	return false;
}

bool
Parser::mfmatrix3dValue (MFMatrix3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFMatrix3d value;

	if (sfmatrix3dValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfmatrix3dValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3dValues (MFMatrix3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3d value;

	_field -> clear ();

	while (sfmatrix3dValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfmatrix3fValue (SFMatrix3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3f::value_type e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (Float (e11))
	{
		if (Float (e12))
		{
			if (Float (e13))
			{
				if (Float (e21))
				{
					if (Float (e22))
					{
						if (Float (e23))
						{
							if (Float (e31))
							{
								if (Float (e32))
								{
									if (Float (e33))
									{
										_field -> setValue (Matrix3f (e11, e12, e13, e21, e22, e23, e31, e32, e33));
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		exception ("To less values, must be a multiple of 9.");
	}

	return false;
}

bool
Parser::mfmatrix3fValue (MFMatrix3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFMatrix3f value;

	if (sfmatrix3fValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfmatrix3fValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3fValues (MFMatrix3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3f value;

	_field -> clear ();

	while (sfmatrix3fValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfmatrix4dValue (SFMatrix4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4d::value_type e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (Double (e11))
	{
		if (Double (e12))
		{
			if (Double (e13))
			{
				if (Double (e14))
				{
					if (Double (e21))
					{
						if (Double (e22))
						{
							if (Double (e23))
							{
								if (Double (e24))
								{
									if (Double (e31))
									{
										if (Double (e32))
										{
											if (Double (e33))
											{
												if (Double (e34))
												{
													if (Double (e41))
													{
														if (Double (e42))
														{
															if (Double (e43))
															{
																if (Double (e44))
																{
																	_field -> setValue (Matrix4d (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44));
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

		exception ("To less values, must be a multiple of 16.");
	}

	return false;
}

bool
Parser::mfmatrix4dValue (MFMatrix4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFMatrix4d value;

	if (sfmatrix4dValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfmatrix4dValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4dValues (MFMatrix4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4d value;

	_field -> clear ();

	while (sfmatrix4dValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfmatrix4fValue (SFMatrix4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4f::value_type e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (Float (e11))
	{
		if (Float (e12))
		{
			if (Float (e13))
			{
				if (Float (e14))
				{
					if (Float (e21))
					{
						if (Float (e22))
						{
							if (Float (e23))
							{
								if (Float (e24))
								{
									if (Float (e31))
									{
										if (Float (e32))
										{
											if (Float (e33))
											{
												if (Float (e34))
												{
													if (Float (e41))
													{
														if (Float (e42))
														{
															if (Float (e43))
															{
																if (Float (e44))
																{
																	_field -> setValue (Matrix4f (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44));
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

		exception ("To less values, must be a multiple of 16.");
	}

	return false;
}

bool
Parser::mfmatrix4fValue (MFMatrix4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFMatrix4f value;

	if (sfmatrix4fValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfmatrix4fValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4fValues (MFMatrix4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4f value;

	_field -> clear ();

	while (sfmatrix4fValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfnodeValue (SFNode* _field)
{
	//__LOG__ << this << " " << std::endl;

	return nodeStatement (*_field);
}

bool
Parser::mfnodeValue (MFNode* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFNode value;

	if (nodeStatement (value))
	{
		_field -> emplace_back (std::move (value));
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		nodeStatements (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::nodeStatements (MFNode* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFNode _node;

	while (nodeStatement (_node))
		_field -> emplace_back (std::move (_node));

	_field -> shrink_to_fit ();
}

bool
Parser::sfrotationValue (SFRotation* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFRotation::value_type x, y, z, angle;

	if (Double (x))
	{
		if (Double (y))
		{
			if (Double (z))
			{
				if (Double (angle))
				{
					_field -> setValue (Rotation4d (x, y, z, fromUnit (UnitCategory::ANGLE, angle)));
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::mfrotationValue (MFRotation* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFRotation value;

	if (sfrotationValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfrotationValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfrotationValues (MFRotation* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFRotation value;

	_field -> clear ();

	while (sfrotationValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfstringValue (SFString* _field)
{
	//__LOG__ << this << " " << std::endl;

	std::string value;

	if (String (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mfstringValue (MFString* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFString value;

	if (sfstringValue (&value))
	{
		_field -> emplace_back (value .str ());
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfstringValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfstringValues (MFString* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFString value;

	_field -> clear ();

	while (sfstringValue (&value))
		_field -> emplace_back (value .str ());

	_field -> shrink_to_fit ();
}

bool
Parser::sftimeValue (SFTime* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFTime::value_type value;

	if (Double (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mftimeValue (MFTime* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFTime value;

	if (sftimeValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sftimeValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sftimeValues (MFTime* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFTime value;

	_field -> clear ();

	while (sftimeValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec2dValue (SFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2d::value_type x, y;

	if (Double (x))
	{
		if (Double (y))
		{
			const auto unit = _field -> getUnit ();

			_field -> setValue (Vector2d (fromUnit (unit, x),
			                              fromUnit (unit, y)));
			return true;
		}

		exception ("To less values, must be a multiple of 2.");
	}

	return false;
}

bool
Parser::mfvec2dValue (MFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec2d value;

	value .setUnit (_field -> getUnit ());

	if (sfvec2dValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec2dValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2dValues (MFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2d value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec2dValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec2fValue (SFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2f::value_type x, y;

	if (Float (x))
	{
		if (Float (y))
		{
			const auto unit = _field -> getUnit ();

			_field -> setValue (Vector2f (fromUnit (unit, x),
			                              fromUnit (unit, y)));
			return true;
		}

		exception ("To less values, must be a multiple of 2.");
	}

	return false;
}

bool
Parser::mfvec2fValue (MFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec2f value;

	value .setUnit (_field -> getUnit ());

	if (sfvec2fValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec2fValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2fValues (MFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2f value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec2fValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec3dValue (SFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3d::value_type x, y, z;

	if (Double (x))
	{
		if (Double (y))
		{
			if (Double (z))
			{
				const auto unit = _field -> getUnit ();

				_field -> setValue (Vector3d (fromUnit (unit, x),
				                              fromUnit (unit, y),
				                              fromUnit (unit, z)));
				return true;
			}
		}

		exception ("To less values, must be a multiple of 3.");
	}

	return false;
}

bool
Parser::mfvec3dValue (MFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec3d value;

	value .setUnit (_field -> getUnit ());

	if (sfvec3dValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec3dValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3dValues (MFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3d value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec3dValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec3fValue (SFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3f::value_type x, y, z;

	if (Float (x))
	{
		if (Float (y))
		{
			if (Float (z))
			{
				const auto unit = _field -> getUnit ();

				_field -> setValue (Vector3f (fromUnit (unit, x),
				                              fromUnit (unit, y),
				                              fromUnit (unit, z)));
				return true;
			}
		}

		exception ("To less values, must be a multiple of 3.");
	}

	return false;
}

bool
Parser::mfvec3fValue (MFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec3f value;

	value .setUnit (_field -> getUnit ());

	if (sfvec3fValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec3fValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3fValues (MFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3f value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec3fValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec4dValue (SFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4d::value_type x, y, z, w;

	if (Double (x))
	{
		if (Double (y))
		{
			if (Double (z))
			{
				if (Double (w))
				{
					const auto unit = _field -> getUnit ();

					_field -> setValue (Vector4d (fromUnit (unit, x),
					                              fromUnit (unit, y),
					                              fromUnit (unit, z),
					                              fromUnit (unit, w)));
					return true;
				}
			}
		}

		exception ("To less values, must be a multiple of 4.");
	}

	return false;
}

bool
Parser::mfvec4dValue (MFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec4d value;

	value .setUnit (_field -> getUnit ());

	if (sfvec4dValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec4dValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4dValues (MFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4d value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec4dValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

bool
Parser::sfvec4fValue (SFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4f::value_type x, y, z, w;

	if (Float (x))
	{
		if (Float (y))
		{
			if (Float (z))
			{
				if (Float (w))
				{
					const auto unit = _field -> getUnit ();

					_field -> setValue (Vector4f (fromUnit (unit, x),
					                              fromUnit (unit, y),
					                              fromUnit (unit, z),
					                              fromUnit (unit, w)));
					return true;
				}
			}
		}

		exception ("To less values, must be a multiple of 4.");
	}

	return false;
}

bool
Parser::mfvec4fValue (MFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec4f value;

	value .setUnit (_field -> getUnit ());

	if (sfvec4fValue (&value))
	{
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfvec4fValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4fValues (MFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4f value;

	value .setUnit (_field -> getUnit ());

	_field -> clear ();

	while (sfvec4fValue (&value))
		_field -> emplace_back (value);

	_field -> shrink_to_fit ();
}

Parser::~Parser ()
{ }

} // X3D
} // titania
