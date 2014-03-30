/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
#include "../Parser/Filter.h"
#include "../Parser/Grammar.h"
#include "../Prototype/ExternProto.h"

namespace titania {
namespace X3D {

const std::string Parser::componentName  = "Browser";
const std::string Parser::typeName       = "Parser";
const std::string Parser::containerField = "parser";

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
	          X3DBaseNode (scene -> getBrowser (), scene),
	            X3DParser (),
	              istream (istream),
	                scene (scene),
	executionContextStack (),
	      currentComments (),
	          whitespaces ()
{ }

void
Parser::parseIntoScene ()
throw (Error <INVALID_X3D>)
{
	std::clog << "Parsing into scene: " << scene -> getWorldURL () << "." << std::endl;

	try
	{
		x3dScene ();
	}
	catch (const X3DError & error)
	{
		std::clog << "Failed parsing into scene: " << scene -> getWorldURL () << "." << std::endl;
		throw Error <INVALID_X3D> (getMessageFromError (error));
	}
	catch (...)
	{
		std::clog << "Failed parsing into scene: " << scene -> getWorldURL () << "." << std::endl;
		throw Error <INVALID_X3D> ("Unkown parser error.");
	}

	std::clog << "Done parsing into scene: " << scene -> getWorldURL () << "." << std::endl;
}

std::string
Parser::getMessageFromError (const X3DError & error)
{
	//__LOG__ << this << " " << std::endl;

	//__LOG__ << this << " " << istream .peek () << std::endl;

	istream .clear ();

	size_t      lineNumber = std::count (whitespaces .begin (), whitespaces .end (), '\n') + 1;
	std::string string     = error .what ();

	filter_control_characters (string);
	filter_bad_utf8_characters (string);

	std::ostringstream stringstream;

	try
	{
		std::string rest    = getline ();
		std::string line    = rgetline ();
		std::string preLine = rgetline ();
		size_t      linePos = line .size () - rest .size ();

		filter_control_characters (line);
		filter_control_characters (preLine);

		filter_bad_utf8_characters (line);
		filter_bad_utf8_characters (preLine);

		if (line .size () > 80)
		{
			line    = line .substr (line .size () - 80, 80);
			preLine = std::string ();
			linePos = 80;
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
			<< std::string (80, '*');
	}
	catch (...)
	{
		stringstream
			<< std::string (80, '*') << std::endl
			<< "Parser error at line " << lineNumber << std::endl
			<< "in '" << scene -> getWorldURL () << '\'' << std::endl
			<< std::endl
			<< string << std::endl
			<< std::string (80, '*');
	}

	return stringstream .str ();
}

std::string
Parser::getline ()
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for ( ; ;)
	{
		char c = istream .get ();

		if (istream)
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

std::string
Parser::rgetline ()
throw (std::out_of_range)
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for ( ; ;)
	{
		istream .unget ();

		char c = istream .peek ();

		if (istream)
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

	return std::string (string .rbegin (), string .rend ());
}

void
Parser::pushExecutionContext (X3DExecutionContext* const executionContext)
{
	//__LOG__ << this << " " << std::endl;

	executionContextStack .emplace_back (executionContext);
}

void
Parser::popExecutionContext ()
{
	//__LOG__ << this << " " << std::endl;

	executionContextStack .pop_back ();
}

X3DExecutionContext*
Parser::getExecutionContext () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//__LOG__ << this << " " << std::endl;

	return executionContextStack .back ();
}

bool
Parser::isInsideProtoDefinition () const
{
	//__LOG__ << this << " " << std::endl;

	return executionContextStack .size () > 1;
}

void
Parser::addRootNode (SFNode && rootNode)
{
	//__LOG__ << this << " " << std::endl;

	getExecutionContext () -> getRootNodes () .emplace_back (std::move (rootNode));
}

void
Parser::x3dScene ()
{
	//__LOG__ << this << " " << std::endl;

	pushExecutionContext (scene);

	std::string encoding, specificationVersion, characterEncoding, comment;

	if (headerStatement (encoding, specificationVersion, characterEncoding, comment))
	{
		scene -> setEncoding (encoding);
		scene -> setSpecificationVersion (specificationVersion);
		scene -> setCharacterEncoding (characterEncoding);
		scene -> setComment (comment);
	}

	profileStatement ();
	componentStatements ();

	unitStatements ();
	metaStatements ();

	statements ();

	scene -> addInnerComments (getComments ());

	popExecutionContext ();

	if (istream .peek () == std::char_traits <char>::eof ())
		return;

	throw Error <INVALID_X3D> ("Unknown statement.");
}

void
Parser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::whitespaces (istream, whitespaces);

	while (comment ())
		Grammar::whitespaces (istream, whitespaces);
}

bool
Parser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	std::string _comment;

	if (Grammar::comment (istream, _comment))
	{
		currentComments .push_back (_comment);
		return true;
	}

	return false;
}

bool
Parser::headerStatement (std::string & _encoding, std::string & _specificationVersion, std::string & _characterEncoding, std::string & _comment)
{
	//__LOG__ << this << " " << std::endl;

	std::string _header;

	if (Grammar::comment (istream, _header))
	{
		if (Grammar::Header .FullMatch (_header, &_encoding, &_specificationVersion, &_characterEncoding, &_comment))
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
			scene -> addComments (getComments ());
			scene -> setProfile (getBrowser () -> getProfile (_profileNameId));
		}
		else
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
		scene -> addComponent (_componentStatement);
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
				else
					throw Error <INVALID_X3D> ("Expected a component support level.");
			}
			else
				throw Error <INVALID_X3D> ("Expected a ':' after component name.");
		}
		else
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

	return istream >> _value;
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
				double _unitConversionFactor;

				if (unitConversionFactor (_unitConversionFactor))
				{
					scene -> updateUnit (_categoryNameId, _unitNameId, _unitConversionFactor);
					return true;
				}
				else
					throw Error <INVALID_X3D> ("Expected unit conversion factor.");
			}
			else
				throw Error <INVALID_X3D> ("Expected unit name identificator.");
		}
		else
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

			const SFNode node = scene -> getNode (_localNodeNameId);

			if (Grammar::AS (istream))
			{
				if (not exportedNodeNameId (_exportedNodeNameId))
					throw Error <INVALID_X3D> ("No name given after AS.");
			}
			else
				_exportedNodeNameId = _localNodeNameId;

			const X3DPtr <ExportedNode> & _exportedNode = scene -> addExportedNode (_exportedNodeNameId, node);

			_exportedNode -> addComments (getComments ());

			return true;
		}
		else
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
			const SFNode             _namedNode  = getExecutionContext () -> getNamedNode (_inlineNodeNameId);
			const X3DPtr <Inline> _inlineNode = x3d_cast <Inline*> (_namedNode);

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

						const X3DPtr <ImportedNode> & _importedNode = getExecutionContext () -> addImportedNode (_inlineNode, _exportedNodeNameId, _nodeNameId);

						_importedNode -> addComments (getComments ());

						return true;
					}
					else
						throw Error <INVALID_X3D> ("Expected exported node name.");
				}
				else
					throw Error <INVALID_X3D> ("Expected exported a '.' after exported node name.");
			}
			else
				throw Error <INVALID_X3D> ("Node named '" + _inlineNodeNameId + "' is not an Inline node.");
		}
		else
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
			else
				throw Error <INVALID_X3D> ("Expected metadata value.");
		}
		else
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
			if (node (_node, _nodeNameId))
				return true;
			else
				throw Error <INVALID_X3D> ("Expected node name after DEF.");
		}
		else
			throw Error <INVALID_X3D> ("No name given after DEF.");
	}

	if (Grammar::USE (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			_node = getExecutionContext () -> getNamedNode (_nodeNameId);

			return true;
		}
		else
			throw Error <INVALID_X3D> ("No name given after USE.");
	}

	if (Grammar::_null (istream))
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
			else
				throw Error <INVALID_X3D> ("Expected node type name after DEF.");
		}
		else
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
			comments ();

			if (Grammar::OpenBracket (istream))
			{
				auto _comments = std::move (getComments ());

				FieldDefinitionArray _interfaceDeclarations = std::move (interfaceDeclarations ());

				comments ();

				if (Grammar::CloseBracket (istream))
				{
					auto _interfaceComments = std::move (getComments ());

					comments ();

					if (Grammar::OpenBrace (istream))
					{
						const X3DPtr <Proto> & _proto = getExecutionContext () -> addProtoDeclaration (_nodeTypeId, _interfaceDeclarations);

						pushExecutionContext (_proto);

						protoBody ();

						popExecutionContext ();

						comments ();

						if (Grammar::CloseBrace (istream))
						{
							//__LOG__ << this << " " << (void*) _proto .getValue () << " " << _nodeTypeId << std::endl;

							_proto -> addInterfaceComments (_interfaceComments);
							_proto -> addComments (_comments);
							_proto -> addInnerComments (getComments ());

							return true;
						}
						else
							throw Error <INVALID_X3D> ("Expected a '}' at the end of PROTO body.");
					}
					else
						throw Error <INVALID_X3D> ("Expected a '{' at the beginning of PROTO body.");
				}
				else
					throw Error <INVALID_X3D> ("Expected a ']' at the end of PROTO interface declaration.");
			}
			else
				throw Error <INVALID_X3D> ("Expected a '[' at the beginning of PROTO interface declaration.");
		}
		else
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

	else
		throw Error <INVALID_X3D> ("Expected root node statement inside PROTO body.");

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

	if (Grammar::inputOnly (istream) or Grammar::eventIn (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::outputOnly (istream) or Grammar::eventOut (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::initializeOnly (istream) or Grammar::field (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();

				if (fieldValue (_field))
				{
					_field -> setAccessType (initializeOnly);
					_field -> setName (_fieldId);
					_field -> addComments (getComments ());
					return _field;
				}
				else
				{
					delete _field;
					throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
				}
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
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

	if (Grammar::inputOutput (istream) or Grammar::exposedField (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();

				if (fieldValue (_field))
				{
					_field -> setAccessType (inputOutput);
					_field -> setName (_fieldId);
					_field -> addComments (getComments ());
					return _field;
				}
				else
				{
					delete _field;
					throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
				}
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
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
						const X3DPtr <ExternProto> & _externProto = getExecutionContext () -> addExternProtoDeclaration (_nodeTypeId, _externInterfaceDeclarations, _URLList);

						_externProto -> addInterfaceComments (_interfaceComments);
						_externProto -> addComments (_comments);
						_externProto -> addInnerComments (getComments ());

						return true;
					}
					else
						throw Error <INVALID_X3D> ("Expected a URL list after EXTERNPROTO '" + _nodeTypeId + "'.");
				}
				else
					throw Error <INVALID_X3D> ("Expected a ']' at the end of EXTERNPROTO interface declaration.");
			}
			else
				throw Error <INVALID_X3D> ("Expected a '[' at the beginning of EXTERNPROTO interface declaration.");
		}
		else
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

	if (Grammar::inputOnly (istream) or Grammar::eventIn (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::outputOnly (istream) or Grammar::eventOut (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::initializeOnly (istream) or Grammar::field (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (initializeOnly);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _fieldType + "'.");
	}

	if (Grammar::inputOutput (istream) or Grammar::exposedField (istream))
	{
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> create ();
				_field -> setAccessType (inputOutput);
				_field -> setName (_fieldId);
				_field -> addComments (getComments ());
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
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
			const SFNode _fromNode = getExecutionContext () -> getNode (_fromNodeId);

			comments ();

			if (Grammar::Period (istream))
			{
				std::string _eventOutId;

				if (outputOnlyId (_eventOutId))
				{
					X3DFieldDefinition* _eventOut = nullptr;

					try
					{
						_eventOut = _fromNode .getField (_eventOutId);
					}
					catch (const Error <INVALID_NAME> &)
					{
						throw Error <INVALID_X3D> ("Bad ROUTE specification: Unknown eventOut '" + _eventOutId + "' in node '" + _fromNodeId + "' class " + _fromNode .getNodeTypeName ());
					}

					comments ();

					if (Grammar::TO (istream))
					{
						std::string _toNodeId;

						if (nodeNameId (_toNodeId))
						{
							const SFNode _toNode = getExecutionContext () -> getNode (_toNodeId);

							comments ();

							if (Grammar::Period (istream))
							{
								std::string _eventInId;

								if (inputOnlyId (_eventInId))
								{
									X3DFieldDefinition* _eventIn = nullptr;

									try
									{
										_eventIn = _toNode .getField (_eventInId);
									}
									catch (const Error <INVALID_NAME> &)
									{
										throw Error <INVALID_X3D> ("Bad ROUTE specification: Unknown eventIn '" + _eventInId + "' in node '" + _toNodeId + "' class " + _toNode .getNodeTypeName ());
									}

									if (_eventOut -> getType () == _eventIn -> getType ())
									{
										const X3DPtr <Route> & _route = getExecutionContext () -> addRoute (_fromNode, _eventOutId, _toNode, _eventInId);

										_route -> addComments (getComments ());

										return true;
									}
									else
										throw Error <INVALID_X3D> ("ROUTE types " + _eventOut -> getTypeName () + " and " + _eventIn -> getTypeName () + " do not match.");
								}
								else
									throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a field name.");
							}
							else
								throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a '.' after node name.");
						}
						else
							throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a node name.");
					}
					else
						throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a 'TO'.");
				}
				else
					throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a field name.");
			}
			else
				throw Error <INVALID_X3D> ("Bad ROUTE specification: Expected a '.' after node name.");
		}
		else
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
				_node = getExecutionContext () -> createProtoInstance (_nodeTypeId) .getValue ();
			}
			catch (const X3DError & error2)
			{
				throw Error <INVALID_X3D> (error1 .what () + std::string ("\n") + error2 .what ());
			}
		}

		X3DBaseNode* _baseNode = _node .getValue ();

		//__LOG__ << this << " " << _nodeTypeId << " " << (void*) _node << std::endl;

		if (not _nodeNameId .empty ())
		{
			try
			{
				const SFNode namedNode = getExecutionContext () -> getNamedNode (_nodeNameId); // Create copy!

				getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (_nodeNameId, true), namedNode);
			}
			catch (const X3DError &)
			{ }

			getExecutionContext () -> updateNamedNode (_nodeNameId, _node);
		}

		comments ();

		if (Grammar::OpenBrace (istream))
		{
			_baseNode -> addComments (getComments ());

			if (_baseNode -> hasUserDefinedFields ())
				scriptBody (_baseNode);

			else
				nodeBody (_baseNode);

			comments ();

			if (Grammar::CloseBrace (istream))
			{
				//__LOG__ << this << " " << _nodeTypeId << std::endl;

				_baseNode -> addInnerComments (getComments ());

				getExecutionContext () -> addUninitializedNode (_baseNode);

				//__LOG__ << this << " " << _nodeTypeId << std::endl;
				return true;
			}
			else
				throw Error <INVALID_X3D> ("Expected '}' at the end of node body.");
		}
		else
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

	auto state = istream .rdstate ();
	auto pos   = istream .tellg ();
	auto ws    = whitespaces .size ();
	auto com   = currentComments .size ();

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
									throw Error <INVALID_X3D> ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName () + " interface declaration.");
								}

								const X3DFieldDefinition* _supportedField = getBrowser () -> getFieldType (_fieldType);

								if (_supportedField -> getType () == _reference -> getType ())
								{
									if (_accessType -> second == _reference -> getAccessType () or _accessType -> second == inputOutput)
									{
										X3DFieldDefinition* _field = nullptr;

										try
										{
											_field = _baseNode -> getField (_fieldId);

											//if (_field -> getAccessType () not_eq _accessType -> second)
											//	throw Error <INVALID_X3D> ("Field '" + _fieldId + "' must have access type " + Generator::AccessTypes [_field] + ".");

											// experimental see events test and vrml logo test
											{
												if (_supportedField -> getType () == _field -> getType ())
												{
													if (_accessType -> second == _field -> getAccessType () or _field -> getAccessType () == inputOutput)
													{
														if (_accessType -> second not_eq _field -> getAccessType ())
															throw Error <INVALID_NAME> ("");
													}
													else
														throw Error <INVALID_X3D> ("Field '" + _fieldId + "' must have access type " + Generator::AccessTypes [_field] + ".");
												}
												else
													throw Error <INVALID_NAME> ("");
											}
										}
										catch (const Error <INVALID_NAME> &)
										{
											_field = _supportedField -> create ();

											_baseNode -> addUserDefinedField (_accessType -> second,
											                                  _fieldId,
											                                  _field);
										}

										_field -> addReference (_reference);
										_field -> addComments (getComments ());

										return true;
									}
									else
										throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' are incompatible as an IS mapping.");
								}
								else
									throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' have different types.");
							}
							else
								throw Error <INVALID_X3D> ("No name give after IS statement.");
						}
						else
							throw Error <INVALID_X3D> ("IS statement outside PROTO definition.");
					}
				}
			}
		}

		// Reset stream position.

		istream .clear (state);
		istream .seekg (pos - istream .tellg (), std::ios_base::cur);
		whitespaces .resize (ws);
		currentComments .resize (com);
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
					_existingField -> write (*_field);

					getGarbageCollector () .addObject (_field);

					return true;
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
						throw Error <INVALID_X3D> ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName ());
					}

					if (_field -> getType () == _reference -> getType ())
					{
						if (_field -> getAccessType () == _reference -> getAccessType () or _field -> getAccessType () == inputOutput)
						{
							_field -> addReference (_reference);
							_field -> addComments (getComments ());
							return true;
						}
						else
							throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " are incompatible as an IS mapping.");
					}
					else
						throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " have different types.");
				}
				else
					throw Error <INVALID_X3D> ("No name give after IS statement.");
			}
			else
				throw Error <INVALID_X3D> ("IS statement outside PROTO definition.");
		}

		if (_field -> isInitializeable ())
		{
			if (fieldValue (_field))
			{
				_field -> addComments (getComments ());
				return true;
			}
			else
				throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
		}
		else
			throw Error <INVALID_X3D> ("Couldn't assign value to " + Generator::AccessTypes [_field] + " field '" + _fieldId + "'.");
	}

	return false;
}

//Ids

bool
Parser::Id (std::string & _Id)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (istream)
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

	for ( ; istream;)
	{
		if (istream)
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

	return Grammar::FieldType .find (_fieldType) not_eq Grammar::FieldType .end ();
}

bool
Parser::fieldValue (X3DFieldDefinition* _field)
{
	//__LOG__ << this << " " << std::endl;
	//__LOG__ << this << " " << _field -> getTypeName () << std::endl;

	_field -> addComments (getComments ());

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
Parser::Double (double & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (istream >> _value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Float (float & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (istream >> _value)
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

	if (Grammar::string (istream, _value))
	{
		whitespaces .append (_value);
		return true;
	}

	istream .clear ();

	return false;
}

bool
Parser::sfboolValue (SFBool* _field)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::_true (istream))
	{
		_field -> setValue (true);
		return true;
	}

	if (Grammar::_false (istream))
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfboolValues (MFBool* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFBool value;

	while (sfboolValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfcolorValue (SFColor* _field)
{
	//__LOG__ << this << " " << std::endl;

	float r, g, b;

	if (Float (r))
	{
		if (Float (g))
		{
			if (Float (b))
			{
				_field -> setValue (r, g, b);
				return true;
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 3.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorValues (MFColor* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColor value;

	while (sfcolorValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfcolorRGBAValue (SFColorRGBA* _field)
{
	//__LOG__ << this << " " << std::endl;

	float r, g, b, a;

	if (Float (r))
	{
		if (Float (g))
		{
			if (Float (b))
			{
				if (Float (a))
				{
					_field -> setValue (r, g, b, a);
					return true;
				}
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 4.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorRGBAValues (MFColorRGBA* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFColorRGBA value;

	while (sfcolorRGBAValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfdoubleValue (SFDouble* _field)
{
	//__LOG__ << this << " " << std::endl;

	double value;

	if (Double (value))
	{
		_field -> setValue (value);
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

	while (sfdoubleValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sffloatValue (SFFloat* _field)
{
	//__LOG__ << this << " " << std::endl;

	float value;

	if (Float (value))
	{
		_field -> setValue (value);
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sffloatValues (MFFloat* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFFloat value;

	while (sffloatValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfimageValue (SFImage* _field)
{
	//__LOG__ << this << " " << std::endl;

	int32_t width, height, components, pixel;
	MFInt32 array;

	if (Int32 (width))
	{
		if (Int32 (height))
		{
			if (Int32 (components))
			{
				int32_t size = height * width;

				array .reserve (size);

				for (int32_t i = 0; i < size; ++ i)
				{
					if (Int32 (pixel))
						array .emplace_back (pixel);

					else
						throw Error <INVALID_X3D> ("Expected more pixel values.");
				}

				_field -> setValue (width, height, components, array);
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
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfimageValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfimageValues (MFImage* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFImage value;

	while (sfimageValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfint32Value (SFInt32* _field)
{
	//__LOG__ << this << " " << std::endl;

	int32_t value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfint32Values (MFInt32* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFInt32 value;

	while (sfint32Value (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfmatrix3dValue (SFMatrix3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	double e11, e12, e13, e21, e22, e23, e31, e32, e33;

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
										_field -> setValue (e11, e12, e13, e21, e22, e23, e31, e32, e33);
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 9.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3dValues (MFMatrix3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3d value;

	while (sfmatrix3dValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfmatrix3fValue (SFMatrix3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	float e11, e12, e13, e21, e22, e23, e31, e32, e33;

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
										_field -> setValue (e11, e12, e13, e21, e22, e23, e31, e32, e33);
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 9.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3fValues (MFMatrix3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix3f value;

	while (sfmatrix3fValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfmatrix4dValue (SFMatrix4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	double e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

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
																	_field -> setValue (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44);
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

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 16.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4dValues (MFMatrix4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4d value;

	while (sfmatrix4dValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfmatrix4fValue (SFMatrix4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	float e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

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
																	_field -> setValue (e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44);
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

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 16.");
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4fValues (MFMatrix4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFMatrix4f value;

	while (sfmatrix4fValue (&value))
		_field -> emplace_back (value);
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

		else
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
}

bool
Parser::sfrotationValue (SFRotation* _field)
{
	//__LOG__ << this << " " << std::endl;

	float x, y, z, angle;

	if (Float (x))
	{
		if (Float (y))
		{
			if (Float (z))
			{
				if (Float (angle))
				{
					_field -> setValue (x, y, z, angle);
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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfrotationValues (MFRotation* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFRotation value;

	while (sfrotationValue (&value))
		_field -> emplace_back (value);
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
		_field -> emplace_back (value);
		return true;
	}

	if (Grammar::OpenBracket (istream))
	{
		sfstringValues (_field);

		comments ();

		if (Grammar::CloseBracket (istream))
			return true;

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfstringValues (MFString* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFString value;

	while (sfstringValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sftimeValue (SFTime* _field)
{
	//__LOG__ << this << " " << std::endl;

	double value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sftimeValues (MFTime* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFTime value;

	while (sftimeValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec2dValue (SFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	double x, y;

	if (Double (x))
	{
		if (Double (y))
		{
			_field -> setValue (x, y);
			return true;
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 2.");
	}

	return false;
}

bool
Parser::mfvec2dValue (MFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec2d value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2dValues (MFVec2d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2d value;

	while (sfvec2dValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec2fValue (SFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	float x, y;

	if (Float (x))
	{
		if (Float (y))
		{
			_field -> setValue (x, y);
			return true;
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 2.");
	}

	return false;
}

bool
Parser::mfvec2fValue (MFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec2f value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2fValues (MFVec2f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec2f value;

	while (sfvec2fValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec3dValue (SFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	double x, y, z;

	if (Double (x))
	{
		if (Double (y))
		{
			if (Double (z))
			{
				_field -> setValue (x, y, z);
				return true;
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 3.");
	}

	return false;
}

bool
Parser::mfvec3dValue (MFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec3d value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3dValues (MFVec3d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3d value;

	while (sfvec3dValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec3fValue (SFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	float x, y, z;

	if (Float (x))
	{
		if (Float (y))
		{
			if (Float (z))
			{
				_field -> setValue (x, y, z);
				return true;
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 3.");
	}

	return false;
}

bool
Parser::mfvec3fValue (MFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec3f value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3fValues (MFVec3f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec3f value;

	while (sfvec3fValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec4dValue (SFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	double x, y, z, w;

	if (Double (x))
	{
		if (Double (y))
		{
			if (Double (z))
			{
				if (Double (w))
				{
					_field -> setValue (x, y, z, w);
					return true;
				}
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 4.");
	}

	return false;
}

bool
Parser::mfvec4dValue (MFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec4d value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4dValues (MFVec4d* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4d value;

	while (sfvec4dValue (&value))
		_field -> emplace_back (value);
}

bool
Parser::sfvec4fValue (SFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	float x, y, z, w;

	if (Float (x))
	{
		if (Float (y))
		{
			if (Float (z))
			{
				if (Float (w))
				{
					_field -> setValue (x, y, z, w);
					return true;
				}
			}
		}

		throw Error <INVALID_X3D> ("To less values, must be a multiple of 4.");
	}

	return false;
}

bool
Parser::mfvec4fValue (MFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	_field -> clear ();

	SFVec4f value;

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

		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4fValues (MFVec4f* _field)
{
	//__LOG__ << this << " " << std::endl;

	SFVec4f value;

	while (sfvec4fValue (&value))
		_field -> emplace_back (value);
}

} // X3D
} // titania
