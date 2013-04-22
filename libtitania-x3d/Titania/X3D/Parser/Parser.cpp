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

#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Networking/Inline.h"
#include "../Components/Scripting/Script.h"
#include "../Components/Shaders/X3DProgrammableShaderObject.h"
#include "../Parser/RegEx.h"
#include "../Prototype/ExternProto.h"

#include <Titania/Stream/IGZFilter.h>

namespace titania {
namespace X3D {

static
void
error (const int line, const std::string & type)
{
	GLenum errorNum = glGetError ();

	if (errorNum not_eq GL_NO_ERROR)
	{
		std::clog << "OpenGL Error at " << line << " " << type << ": " << gluErrorString (errorNum) << std::endl;
		assert (0);
	}
}

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

Parser::Parser (X3DScene* scene) :
	X3DBaseNode (scene -> getBrowser (), scene), 
	  X3DParser (),                              
	      scene (scene),                         
	      input (),                              
	     string ()                               
{
	setComponent ("Browser");
	setTypeName ("Parser");
}

void
Parser::parseIntoScene (std::istream & istream)
throw (Error <INVALID_X3D>)
{
	std::ostringstream ostringstream;

	ostringstream << basic::gunzip (istream) .rdbuf ();

	input  = ostringstream .str ();
	string = pcrecpp::StringPiece (input);

	std::clog << "Parsing into scene: " << scene -> getWorldURL () << "." << std::endl;

	try
	{
		x3dScene ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_X3D> (getMessageFromError (error));
	}

	std::clog << "Done parsing into scene: " << scene -> getWorldURL () << "." << std::endl;
}

std::string
Parser::getMessageFromError (const X3DError & error)
{
	//__LOG__ << std::endl;

	// still unused error message: Premature end of file after DEF

	std::ostringstream stringstream;

	char nl = input .find ('\n', 0) == std::string::npos ? '\r' : '\n';

	std::string::size_type pos = (string .data () - input .data ()) / sizeof (char);

	std::string::size_type start = input .rfind (nl, pos);

	start = (start == std::string::npos) ? 0 : start + 1;
	std::string::size_type end = input .find (nl, start);

	if (end == std::string::npos)
		end = input .length ();

	std::string line = input .substr (start, end - start);

	std::string::size_type linePos = pos > start ? pos - start : 0;

	int                    lineNumber = 0;
	std::string            begin      = input .substr (0, pos);
	std::string::size_type _break     = begin .find (nl, 0);

	while (_break not_eq std::string::npos)
	{
		++ lineNumber;
		_break = begin .find ("\n", _break + 1);
	}

	++ lineNumber;

	std::string preLine;

	if (start)
	{
		std::string::size_type preLineStart = input .rfind (nl, start - 2);
		preLineStart = preLineStart == std::string::npos ? 0 : preLineStart + 1;
		preLine      = input .substr (preLineStart, start - 1 - preLineStart);
	}

	stringstream
		<< std::string (80, '*') << std::endl
		<< "Parser error at - line " << lineNumber << ':' << linePos << std::endl
		<< std::endl
		<< preLine << std::endl
		<< line << std::endl
		<< std::string (linePos, ' ') << '^' << std::endl
		<< error .what () << std::endl
		<< std::string (80, '*') << std::endl
		<< "in '" << scene -> getWorldURL () << '\'' << std::endl;

	std::clog << stringstream .str () << std::endl;

	return stringstream .str ();
}

void
Parser::pushExecutionContext (X3DExecutionContext* const executionContext)
{
	//__LOG__ << std::endl;

	executionContextStack .push_back (executionContext);
}

void
Parser::popExecutionContext ()
{
	//__LOG__ << std::endl;

	executionContextStack .pop_back ();
}

X3DExecutionContext*
Parser::getExecutionContext () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//__LOG__ << std::endl;

	return executionContextStack .back ();
}

bool
Parser::isInsideProtoDefinition () const
{
	//__LOG__ << std::endl;

	return executionContextStack .size () > 1;
}

void
Parser::addRootNode (const SFNode <X3DBaseNode> & rootNode)
{
	//__LOG__ << std::endl;

	getExecutionContext () -> addRootNode (rootNode);
}

void
Parser::comments ()
{
	////__LOG__ << std::endl;

	std::string _comment;

	while (comment (_comment))
		;
}

bool
Parser::comment (std::string & _comment)
{
	////__LOG__ << std::endl;

	return RegEx::Comment .Consume (&string, &_comment);
}

void
Parser::x3dScene ()
{
	//__LOG__ << std::endl;

	pushExecutionContext (scene);

	std::string encoding, specificationVersion, characterEncoding, comment;

	if (headerStatement (encoding, specificationVersion, characterEncoding, comment))
	{
		scene -> setEncoding (encoding);
		scene -> setSpecificationVersion (specificationVersion);
		scene -> setCharacterEncoding (characterEncoding);
		scene -> setComment (comment);
	}
	else
		//__LOG__ << std::endl;

	comments ();
	scene -> setProfile (profileStatement ());

	comments ();
	scene -> addComponents (componentStatements ());

	comments ();
	unitStatements ();

	comments ();
	metaStatements ();

	statements ();

	RegEx::Whitespaces .Consume (&string);

	popExecutionContext ();

	if (string .empty ())
		return;

	throw Error <INVALID_X3D> ("Unknown statement.");
}

bool
Parser::headerStatement (std::string & encoding, std::string & specificationVersion, std::string & characterEncoding, std::string & comment)
{
	//__LOG__ << std::endl;

	if (RegEx::Header .Consume (&string, &encoding, &specificationVersion, &characterEncoding, &comment))
	{
		comments ();
		return true;
	}

	return false;
}

const ProfileInfo*
Parser::profileStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::PROFILE .Consume (&string))
	{
		comments ();

		std::string _profileNameId;

		if (profileNameId (_profileNameId))
		{
			return getBrowser () -> getProfile (_profileNameId);
		}
		else
			throw Error <INVALID_X3D> ("Expected a profile name.");
	}

	return nullptr;
}

ComponentInfoArray
Parser::componentStatements ()
{
	//__LOG__ << std::endl;

	ComponentInfoArray _componentStatements;

	const ComponentInfo* _componentStatement = componentStatement ();

	while (_componentStatement)
	{
		_componentStatements .push_back (_componentStatement -> getName (), _componentStatement);
		_componentStatement = componentStatement ();
	}

	return _componentStatements;
}

const ComponentInfo*
Parser::componentStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::COMPONENT .Consume (&string))
	{
		comments ();

		std::string _componentNameId;

		if (componentNameId (_componentNameId))
		{
			if (RegEx::Colon .Consume (&string))
			{
				comments ();

				int32_t _componentSupportLevel;

				if (componentSupportLevel (_componentSupportLevel))
				{
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

	return NULL;
}

bool
Parser::componentSupportLevel (int32_t & value)
{
	//__LOG__ << std::endl;

	return _int32 (value);
}

void
Parser::unitStatements ()
{
	//__LOG__ << std::endl;

	while (unitStatement ())
		;
}

bool
Parser::unitStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::UNIT .Consume (&string))
	{
		comments ();

		std::string _categoryNameId;

		if (categoryNameId (_categoryNameId))
		{
			std::string _unitNameId;

			if (unitNameId (_unitNameId))
			{
				double _unitConversionFactor;

				if (unitConversionFactor (_unitConversionFactor))
				{
					// save unit in scene -> updateUnit (_categoryNameId, _unitNameId, _unitConversionFactor);
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
Parser::unitConversionFactor (double & _value)
{
	//__LOG__ << std::endl;

	return _double (_value);
}

bool
Parser::exportStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::EXPORT .Consume (&string))
	{
		comments ();

		std::string _localNodeNameId;

		if (nodeNameId (_localNodeNameId))
		{
			std::string _exportedNodeNameId;

			if (RegEx::AS .Consume (&string))
			{
				comments ();

				if (not exportedNodeNameId (_exportedNodeNameId))
					throw Error <INVALID_X3D> ("No name given after AS.");
			}

			getExecutionContext () -> addExportedNode (_localNodeNameId, _exportedNodeNameId);

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
	//__LOG__ << std::endl;

	if (RegEx::IMPORT .Consume (&string))
	{
		comments ();

		std::string _inlineNodeNameId;

		if (inlineNodeNameId (_inlineNodeNameId))
		{
			const SFNode <Inline> _inlineNode = dynamic_cast <Inline*> (getExecutionContext () -> getNamedNode (_inlineNodeNameId) .getValue ());

			if (_inlineNode)
			{
				if (RegEx::Period .Consume (&string))
				{
					comments ();

					std::string _exportedNodeNameId;

					if (exportedNodeNameId (_exportedNodeNameId))
					{
						std::string _nodeNameId;

						if (RegEx::AS .Consume (&string))
						{
							comments ();

							if (not nodeNameId (_nodeNameId))
								throw Error <INVALID_X3D> ("No name given after AS.");
						}

						getExecutionContext () -> addImportedNode (_inlineNode, _exportedNodeNameId, _nodeNameId);

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
	//__LOG__ << std::endl;

	while (metaStatement ())
		;
}

bool
Parser::metaStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::META .Consume (&string))
	{
		comments ();

		std::string _metakey;

		if (metakey (_metakey))
		{
			std::string _metavalue;

			if (metavalue (_metavalue))
			{
				scene -> setMetaData (_metakey, _metavalue);
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

bool
Parser::metakey (std::string & _value)
{
	//__LOG__ << std::endl;

	return _string (_value);
}

bool
Parser::metavalue (std::string & _value)
{
	//__LOG__ << std::endl;

	return _string (_value);
}

void
Parser::statements ()
{
	//__LOG__ << std::endl;

	while (statement ())
		;

	//__LOG__ << std::endl;
}

bool
Parser::statement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (protoStatement ())
		return true;

	if (routeStatement ())
		return true;

	if (importStatement ())
		return true;

	if (exportStatement ())
		return true;

	SFNode <X3DBaseNode> _node;

	if (nodeStatement (_node))
	{
		addRootNode (_node);
		return true;
	}

	return false;
}

bool
Parser::nodeStatement (X3DFieldDefinition & _node)
{
	//__LOG__ << std::endl;

	if (RegEx::DEF .Consume (&string))
	{
		comments ();

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

	if (RegEx::USE .Consume (&string))
	{
		comments ();

		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			const SFNode <X3DBaseNode> & _namedNode = getExecutionContext () -> getNamedNode (_nodeNameId);

			_node .write (_namedNode);

			return true;
		}
		else
			throw Error <INVALID_X3D> ("No name given after USE.");
	}

	if (RegEx::_null .Consume (&string))
	{
		comments ();

		static SFNode <X3DBaseNode> _null;

		_node .write (_null);

		return true;
	}

	if (node (_node))
		return true;

	return false;
}

bool
Parser::rootNodeStatement (SFNode <X3DBaseNode> & _node)
{
	//__LOG__ << std::endl;

	if (RegEx::DEF .Consume (&string))
	{
		comments ();
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
	//__LOG__ << std::endl;

	if (proto ())
		return true;

	if (externproto ())
		return true;

	return false;
}

void
Parser::protoStatements ()
{
	//__LOG__ << std::endl;

	while (protoStatement ())
		;
}

bool
Parser::proto ()
{
	//__LOG__ << std::endl;

	if (RegEx::PROTO .Consume (&string))
	{
		comments ();

		std::string _nodeTypeId;

		if (nodeTypeId (_nodeTypeId))
		{
			if (RegEx::OpenBracket .Consume (&string))
			{
				comments ();

				FieldDefinitionArray _interfaceDeclarations = interfaceDeclarations ();

				if (RegEx::CloseBracket .Consume (&string))
				{
					comments ();

					if (RegEx::OpenBrace .Consume (&string))
					{
						comments ();

						const SFNode <Proto> & _proto = getExecutionContext () -> addProtoDeclaration (_nodeTypeId, _interfaceDeclarations);

						pushExecutionContext (*_proto);

						protoBody ();

						popExecutionContext ();

						if (RegEx::CloseBrace .Consume (&string))
						{
							//__LOG__ << (void*) _proto .getValue () << " " << _nodeTypeId << std::endl;
							comments ();
							return true;
						}
						else
							throw Error <INVALID_X3D> ("Expected a '}' at the end of PROTO body.");
					}
					else
						throw Error <INVALID_X3D> ("Expected a '{' at the beginning of PROTO body.");
				}
				else
					throw Error <INVALID_X3D> ("Expected a ']' at the end of PROTO head.");
			}
			else
				throw Error <INVALID_X3D> ("Expected a '[' at the beginning of PROTO head.");
		}
		else
			throw Error <INVALID_X3D> ("Invalid PROTO definition name.");
	}

	return false;
}

void
Parser::protoBody ()
{
	//__LOG__ << std::endl;

	protoStatements ();

	SFNode <X3DBaseNode> _rootNodeStatement;

	if (rootNodeStatement (_rootNodeStatement))
		addRootNode (_rootNodeStatement);

	else
		throw Error <INVALID_X3D> ("Expected root node statement inside PROTO body.");

	statements ();
}

FieldDefinitionArray
Parser::interfaceDeclarations ()
{
	//__LOG__ << std::endl;

	FieldDefinitionArray _interfaceDeclarations;
	X3DFieldDefinition*  _field = interfaceDeclaration ();

	while (_field)
	{
		_interfaceDeclarations .push_back (_field);
		_field = interfaceDeclaration ();
	}

	return _interfaceDeclarations;
}

X3DFieldDefinition*
Parser::restrictedInterfaceDeclaration ()
{
	//__LOG__ << std::endl;

	if (RegEx::inputOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	if (RegEx::outputOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	if (RegEx::initializeOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();

				if (fieldValue (_field))
				{
					_field -> setAccessType (initializeOnly);
					_field -> setName (_fieldId);
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
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	return nullptr;
}

X3DFieldDefinition*
Parser::interfaceDeclaration ()
{
	//__LOG__ << std::endl;

	X3DFieldDefinition* _field = restrictedInterfaceDeclaration ();

	if (_field)
		return _field;

	if (RegEx::inputOutput .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();

				if (fieldValue (_field))
				{
					_field -> setAccessType (inputOutput);
					_field -> setName (_fieldId);
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
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	return nullptr;
}

bool
Parser::externproto ()
{
	//__LOG__ << std::endl;

	if (RegEx::EXTERNPROTO .Consume (&string))
	{
		comments ();

		std::string _nodeTypeId;

		if (nodeTypeId (_nodeTypeId))
		{
			if (RegEx::OpenBracket .Consume (&string))
			{
				comments ();

				FieldDefinitionArray _externInterfaceDeclarations = externInterfaceDeclarations ();

				if (RegEx::CloseBracket .Consume (&string))
				{
					comments ();

					MFString _URLList;

					if (URLList (&_URLList))
					{
						getExecutionContext () -> addExternProtoDeclaration (_nodeTypeId, _externInterfaceDeclarations, _URLList);
						return true;
					}
					else
						throw Error <INVALID_X3D> ("Expected a URL list after EXTERNPROTO '" + _nodeTypeId + "'.");
				}
				else
					throw Error <INVALID_X3D> ("Expected a ']' at the end of EXTERNPROTO head.");
			}
			else
				throw Error <INVALID_X3D> ("Expected a '[' at the beginning of EXTERNPROTO head.");
		}
		else
			throw Error <INVALID_X3D> ("Invalid EXTERNPROTO definition name.");
	}

	return false;
}

FieldDefinitionArray
Parser::externInterfaceDeclarations ()
{
	//__LOG__ << std::endl;

	FieldDefinitionArray _externInterfaceDeclarations;
	X3DFieldDefinition*  _field = externInterfaceDeclaration ();

	while (_field)
	{
		_externInterfaceDeclarations .push_back (_field);
		_field = externInterfaceDeclaration ();
	}

	return _externInterfaceDeclarations;
}

X3DFieldDefinition*
Parser::externInterfaceDeclaration ()
{
	//__LOG__ << std::endl;

	if (RegEx::inputOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (inputOnly);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	if (RegEx::outputOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (outputOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (outputOnly);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	if (RegEx::initializeOnly .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (initializeOnlyId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (initializeOnly);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	if (RegEx::inputOutput .Consume (&string))
	{
		comments ();
		std::string _fieldType;

		if (fieldType (_fieldType))
		{
			std::string _fieldId;

			if (inputOutputId (_fieldId))
			{
				X3DFieldDefinition* _field = getBrowser () -> getFieldType (_fieldType) -> clone ();
				_field -> setAccessType (inputOutput);
				_field -> setName (_fieldId);
				return _field;
			}
			else
				throw Error <INVALID_X3D> ("Expected a name for field.");
		}
		else
		{
			std::string _Id;
			Id (_Id);
			throw Error <INVALID_X3D> ("Unknown event or field type: '" + _Id + "'.");
		}
	}

	return nullptr;
}

bool
Parser::routeStatement ()
{
	//__LOG__ << std::endl;

	if (RegEx::ROUTE .Consume (&string))
	{
		comments ();

		std::string _fromNodeId;

		if (nodeNameId (_fromNodeId))
		{
			const SFNode <X3DBaseNode> & _fromNode = getExecutionContext () -> getNode (_fromNodeId);

			if (RegEx::Period .Consume (&string))
			{
				comments ();

				std::string _eventOutId;

				if (outputOnlyId (_eventOutId))
				{
					X3DFieldDefinition* _eventOut = _fromNode .getField (_eventOutId);

					if (_eventOut)
					{
						if (RegEx::TO .Consume (&string))
						{
							comments ();

							std::string _toNodeId;

							if (nodeNameId (_toNodeId))
							{
								const SFNode <X3DBaseNode> & _toNode = getExecutionContext () -> getNode (_toNodeId);

								if (RegEx::Period .Consume (&string))
								{
									comments ();

									std::string _eventInId;

									if (inputOnlyId (_eventInId))
									{
										X3DFieldDefinition* _eventIn = _toNode .getField (_eventInId);

										if (_eventIn)
										{
											if (_eventOut -> getType () == _eventIn -> getType ())
											{
												getExecutionContext () -> addRoute (_fromNode, _eventOutId, _toNode, _eventInId);

												comments ();

												return true;
											}
											else
												throw Error <INVALID_X3D> ("ROUTE types " + _eventOut -> getTypeName () + " and " + _eventIn -> getTypeName () + " do not match.");
										}
										else
											throw Error <INVALID_X3D> ("Bad ROUTE specification: Unknown eventIn '" + _eventInId + "' in node '" + _toNodeId + "' class " + _toNode .getNodeTypeName ());
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
						throw Error <INVALID_X3D> ("Bad ROUTE specification: Unknown eventOut '" + _eventOutId + "' in node '" + _fromNodeId + "' class " + _fromNode .getNodeTypeName ());
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
	//__LOG__ << std::endl;

	return mfstringValue (_value);
}

//Nodes

bool
Parser::node (X3DFieldDefinition & _node, const std::string & _nodeNameId)
{
	//__LOG__ << _nodeNameId << std::endl;

	std::string _nodeTypeId;

	if (nodeTypeId (_nodeTypeId))
	{
		//__LOG__ << _nodeTypeId << std::endl;

		SFNode <X3DBaseNode> _newNode;

		try
		{
			_newNode = getExecutionContext () -> createNode (_nodeTypeId, false);
		}
		catch (const Error <INVALID_NAME> &)
		{
			_newNode = getExecutionContext () -> createProtoInstance (_nodeTypeId, false) .getValue ();
		}

		X3DBaseNode* _basicNode = _newNode .getValue ();

		//__LOG__ << _nodeTypeId << " " << (void*) _newNode << std::endl;

		if (_nodeNameId .length ())
			getExecutionContext () -> updateNamedNode (_nodeNameId, _newNode);

		if (RegEx::OpenBrace .Consume (&string))
		{
			comments ();

			if (dynamic_cast <Script*> (_basicNode))
				scriptBody (_basicNode);

			else if (dynamic_cast <X3DProgrammableShaderObject*> (_basicNode))
				scriptBody (_basicNode);

			else
				nodeBody (_basicNode);

			if (RegEx::CloseBrace .Consume (&string))
			{
				comments ();

				_node .write (_newNode);
				
				_basicNode -> setup ();

				return true;
			}
			else
				throw Error <INVALID_X3D> ("Expected '}' at the end of node body.");
		}
		else
			throw Error <INVALID_X3D> ("Expected '{' at the beginning of node body.");
	}

	//__LOG__ << std::endl;
	return false;
}

void
Parser::scriptBody (X3DBaseNode* const _basicNode)
{
	//__LOG__ << std::endl;

	while (scriptBodyElement (_basicNode))
		;
}

bool
Parser::scriptBodyElement (X3DBaseNode* const _basicNode)
{
	//__LOG__ << std::endl;

	std::string _accessType, _fieldType, _fieldId;

	if (RegEx::ScriptNodeInterfaceIS .Consume (&string, &_accessType, &_fieldType, &_fieldId))
	{
		comments ();

		if (isInsideProtoDefinition ())
		{
			std::string _isId;

			if (Id (_isId))
			{
				X3DFieldDefinition* _reference = getExecutionContext () -> getField (_isId);

				if (_reference)
				{
					const X3DFieldDefinition* _supportedField = getBrowser () -> getFieldType (_fieldType);

					if (_supportedField -> getType () == _reference -> getType ())
					{
						if (accessTypes [_accessType] == _reference -> getAccessType () or accessTypes [_accessType] == inputOutput)
						{
							X3DFieldDefinition* _field = _supportedField -> clone ();

							_field -> setReference (_reference);

							_basicNode -> addUserDefinedField (accessTypes [_accessType],
							                                   _fieldId,
							                                   _field);

							return true;
						}
						else
							throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' are incompatible as an IS mapping.");
					}
					else
						throw Error <INVALID_X3D> ("Field '" + _fieldId + "' and '" + _reference -> getName () + "' in PROTO '" + getExecutionContext () -> getName () + "' have different types.");
				}
				else
					throw Error <INVALID_X3D> ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName () + " head.");
			}
			else
				throw Error <INVALID_X3D> ("No name give after IS statement.");
		}
		else
			throw Error <INVALID_X3D> ("IS statement outside PROTO definition.");
	}

	X3DFieldDefinition* _field = interfaceDeclaration ();

	if (_field)
	{
		_basicNode -> addUserDefinedField (_field -> getAccessType (),
		                                   _field -> getName (),
		                                   _field);

		return true;
	}

	return nodeBodyElement (_basicNode);
}

void
Parser::nodeBody (X3DBaseNode* const _basicNode)
{
	//__LOG__ << std::endl;

	while (nodeBodyElement (_basicNode))
		;
}

bool
Parser::nodeBodyElement (X3DBaseNode* const _basicNode)
{
	//__LOG__ << std::endl;

	if (protoStatement ())
		return true;

	if (routeStatement ())
		return true;

	std::string _fieldId;

	if (initializeOnlyId (_fieldId))
	{
		X3DFieldDefinition* _field = _basicNode -> getField (_fieldId);

		if (_field)
		{
			if (RegEx::IS .Consume (&string))
			{
				comments ();

				if (isInsideProtoDefinition ())
				{
					std::string _isId;

					if (Id (_isId))
					{
						X3DFieldDefinition* _reference = getExecutionContext () -> getField (_isId);

						if (_reference)
						{
							if (_field -> getType () == _reference -> getType ())
							{
								if (_field -> getAccessType () == _reference -> getAccessType () or _field -> getAccessType () == inputOutput)
								{
									_field -> setReference (_reference);
									return true;
								}
								else
									throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " are incompatible as an IS mapping.");
							}
							else
								throw Error <INVALID_X3D> ("Field '" + _field -> getName () + "' and '" + _reference -> getName () + "' in PROTO " + getExecutionContext () -> getName () + " have different types.");
						}
						else
							throw Error <INVALID_X3D> ("No such event or field '" + _isId + "' inside PROTO " + getExecutionContext () -> getName ());
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
					return true;
				}
				else
					throw Error <INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
			}
			else
				throw Error <INVALID_X3D> ("Couldn't assign value for " + Generator::AccessTypes [_field] + " '" + _fieldId + "'.");
		}
		else
			throw Error <INVALID_X3D> ("Unknown field '" + _fieldId + "' in class '" + _basicNode -> getTypeName () + "'.");
	}

	return false;
}

//Ids

bool
Parser::Id (std::string & _Id)
{
	//__LOG__ << std::endl;

	if (RegEx::Id .Consume (&string, &_Id))
	{
		comments ();
		return true;
	}

	return false;
}

bool
Parser::componentNameId (std::string & _Id)
{
	//__LOG__ << std::endl;

	if (RegEx::ComponentNameId .Consume (&string, &_Id))
	{
		comments ();
		return true;
	}

	return false;
}

bool
Parser::fieldType (std::string & _fieldType)
{
	//__LOG__ << std::endl;

	if (RegEx::FieldType .Consume (&string, &_fieldType))
	{
		comments ();
		return true;
	}

	return false;
}

bool
Parser::fieldValue (X3DFieldDefinition* _field)
{
	//__LOG__ << std::endl;

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
			return sfnodeValue (_field);

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

template <class Type>
bool
Parser::from_string (Type & _value, const std::string & _string, std::ios_base & (*_format) (std::ios_base &))
{
	////__LOG__ << std::endl;

	std::istringstream _iss (_string);

	return _iss >> _format >> _value;
}

bool
Parser::_double (double & _value)
{
	////__LOG__ << std::endl;

	std::string _match;

	if (RegEx::_double .Consume (&string, &_match))
	{
		comments ();
		return from_string <double> (_value, _match, std::dec);
	}

	return false;
}

bool
Parser::_float (float & _value)
{
	////__LOG__ << std::endl;

	std::string _match;

	if (RegEx::_float .Consume (&string, &_match))
	{
		comments ();
		return from_string <float> (_value, _match, std::dec);
	}

	return false;
}

bool
Parser::_int32 (int32_t & _value)
{
	////__LOG__ << std::endl;

	if (_hex (_value))
		return true;

	std::string _match;

	if (RegEx::_int32 .Consume (&string, &_match))
	{
		comments ();
		return from_string <int32_t> (_value, _match, std::dec);
	}

	return false;
}

bool
Parser::_hex (int32_t & _value)
{
	////__LOG__ << std::endl;

	std::string _match;

	if (RegEx::_hex .Consume (&string, &_match))
	{
		comments ();
		unsigned long hex;
		bool          res = from_string <unsigned long> (hex, _match, std::hex);

		if (res)
		{
			_value = hex;
			return true;
		}
	}

	return false;
}

bool
Parser::_string (std::string & _value)
{
	////__LOG__ << std::endl;

	if (RegEx::_string .Consume (&string, &_value))
	{
		comments ();
		RegEx::EscapedQuotationMark .GlobalReplace ("\"", &_value);
		return true;
	}

	return false;
}

bool
Parser::sfboolValue (SFBool* _field)
{
	////__LOG__ << std::endl;

	if (RegEx::_true .Consume (&string))
	{
		comments ();
		_field -> setValue (true);
		return true;
	}

	if (RegEx::_false .Consume (&string))
	{
		comments ();
		_field -> setValue (false);
		return true;
	}

	return false;
}

bool
Parser::mfboolValue (MFBool* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFBool value;

	if (sfboolValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfboolValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfboolValues (MFBool* _field)
{
	////__LOG__ << std::endl;

	SFBool value;

	while (sfboolValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfcolorValue (SFColor* _field)
{
	////__LOG__ << std::endl;

	int32_t color;

	if (_hex (color))
	{
		_field -> setValue (((color >> 16) & 0xff) / 255.0f,
		                    ((color >> 8) & 0xff) / 255.0f,
		                    (color & 0xff) / 255.0f);
		return true;
	}

	float r, g, b;

	if (_float (r))
	{
		if (_float (g))
		{
			if (_float (b))
			{
				_field -> setValue (r, g, b);
				return true;
			}
		}
	}

	return false;
}

bool
Parser::mfcolorValue (MFColor* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFColor value;

	if (sfcolorValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfcolorValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorValues (MFColor* _field)
{
	////__LOG__ << std::endl;

	SFColor value;

	while (sfcolorValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfcolorRGBAValue (SFColorRGBA* _field)
{
	////__LOG__ << std::endl;

	float r, g, b, a;

	if (_float (r))
	{
		if (_float (g))
		{
			if (_float (b))
			{
				if (_float (a))
				{
					_field -> setValue (r, g, b, a);
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::mfcolorRGBAValue (MFColorRGBA* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFColorRGBA value;

	if (sfcolorRGBAValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfcolorRGBAValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfcolorRGBAValues (MFColorRGBA* _field)
{
	////__LOG__ << std::endl;

	SFColorRGBA value;

	while (sfcolorRGBAValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfdoubleValue (SFDouble* _field)
{
	////__LOG__ << std::endl;

	double value;

	if (_double (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mfdoubleValue (MFDouble* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFDouble value;

	if (sfdoubleValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfdoubleValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfdoubleValues (MFDouble* _field)
{
	////__LOG__ << std::endl;

	SFDouble value;

	while (sfdoubleValue (&value))
		_field -> push_back (value);
}

bool
Parser::sffloatValue (SFFloat* _field)
{
	////__LOG__ << std::endl;

	float value;

	if (_float (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mffloatValue (MFFloat* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFFloat value;

	if (sffloatValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sffloatValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sffloatValues (MFFloat* _field)
{
	////__LOG__ << std::endl;

	SFFloat value;

	while (sffloatValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfimageValue (SFImage* _field)
{
	////__LOG__ << std::endl;

	int32_t width, height, components, size, pixel;
	MFInt32 array;

	if (_int32 (width))
	{
		if (_int32 (height))
		{
			if (_int32 (components))
			{
				size = height * width;

				for (int32_t i = 0; i < size; ++ i)
				{
					if (_int32 (pixel))
						array .push_back (pixel);
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
	////__LOG__ << std::endl;

	_field -> clear ();

	SFImage value;

	if (sfimageValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfimageValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfimageValues (MFImage* _field)
{
	////__LOG__ << std::endl;

	SFImage value;

	while (sfimageValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfint32Value (SFInt32* _field)
{
	////__LOG__ << std::endl;

	int32_t value;

	if (_int32 (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mfint32Value (MFInt32* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFInt32 value;

	if (sfint32Value (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfint32Values (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfint32Values (MFInt32* _field)
{
	////__LOG__ << std::endl;

	SFInt32 value;

	while (sfint32Value (&value))
		_field -> push_back (value);
}

bool
Parser::sfmatrix3dValue (SFMatrix3d* _field)
{
	////__LOG__ << std::endl;

	double e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (_double (e11))
	{
		if (_double (e12))
		{
			if (_double (e13))
			{
				if (_double (e21))
				{
					if (_double (e22))
					{
						if (_double (e23))
						{
							if (_double (e31))
							{
								if (_double (e32))
								{
									if (_double (e33))
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
	}

	return false;
}

bool
Parser::mfmatrix3dValue (MFMatrix3d* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFMatrix3d value;

	if (sfmatrix3dValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfmatrix3dValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3dValues (MFMatrix3d* _field)
{
	////__LOG__ << std::endl;

	SFMatrix3d value;

	while (sfmatrix3dValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfmatrix3fValue (SFMatrix3f* _field)
{
	////__LOG__ << std::endl;

	float e11, e12, e13, e21, e22, e23, e31, e32, e33;

	if (_float (e11))
	{
		if (_float (e12))
		{
			if (_float (e13))
			{
				if (_float (e21))
				{
					if (_float (e22))
					{
						if (_float (e23))
						{
							if (_float (e31))
							{
								if (_float (e32))
								{
									if (_float (e33))
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
	}

	return false;
}

bool
Parser::mfmatrix3fValue (MFMatrix3f* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFMatrix3f value;

	if (sfmatrix3fValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfmatrix3fValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix3fValues (MFMatrix3f* _field)
{
	////__LOG__ << std::endl;

	SFMatrix3f value;

	while (sfmatrix3fValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfmatrix4dValue (SFMatrix4d* _field)
{
	////__LOG__ << std::endl;

	double e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (_double (e11))
	{
		if (_double (e12))
		{
			if (_double (e13))
			{
				if (_double (e14))
				{
					if (_double (e21))
					{
						if (_double (e22))
						{
							if (_double (e23))
							{
								if (_double (e24))
								{
									if (_double (e31))
									{
										if (_double (e32))
										{
											if (_double (e33))
											{
												if (_double (e34))
												{
													if (_double (e41))
													{
														if (_double (e42))
														{
															if (_double (e43))
															{
																if (_double (e44))
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
	}

	return false;
}

bool
Parser::mfmatrix4dValue (MFMatrix4d* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFMatrix4d value;

	if (sfmatrix4dValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfmatrix4dValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4dValues (MFMatrix4d* _field)
{
	////__LOG__ << std::endl;

	SFMatrix4d value;

	while (sfmatrix4dValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfmatrix4fValue (SFMatrix4f* _field)
{
	////__LOG__ << std::endl;

	float e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;

	if (_float (e11))
	{
		if (_float (e12))
		{
			if (_float (e13))
			{
				if (_float (e14))
				{
					if (_float (e21))
					{
						if (_float (e22))
						{
							if (_float (e23))
							{
								if (_float (e24))
								{
									if (_float (e31))
									{
										if (_float (e32))
										{
											if (_float (e33))
											{
												if (_float (e34))
												{
													if (_float (e41))
													{
														if (_float (e42))
														{
															if (_float (e43))
															{
																if (_float (e44))
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
	}

	return false;
}

bool
Parser::mfmatrix4fValue (MFMatrix4f* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFMatrix4f value;

	if (sfmatrix4fValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfmatrix4fValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfmatrix4fValues (MFMatrix4f* _field)
{
	////__LOG__ << std::endl;

	SFMatrix4f value;

	while (sfmatrix4fValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfnodeValue (X3DFieldDefinition* const _field)
{
	////__LOG__ << std::endl;

	return nodeStatement (*_field);
}

bool
Parser::mfnodeValue (MFNode* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFNode <X3DBaseNode> value;

	if (nodeStatement (value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		nodeStatements (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::nodeStatements (MFNode* _field)
{
	////__LOG__ << std::endl;

	SFNode <X3DBaseNode> _node;

	while (nodeStatement (_node))
		_field -> push_back (_node);
}

bool
Parser::sfrotationValue (SFRotation* _field)
{
	////__LOG__ << std::endl;

	float x, y, z, angle;

	if (_float (x))
	{
		if (_float (y))
		{
			if (_float (z))
			{
				if (_float (angle))
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
	////__LOG__ << std::endl;

	_field -> clear ();

	SFRotation value;

	if (sfrotationValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfrotationValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfrotationValues (MFRotation* _field)
{
	////__LOG__ << std::endl;

	SFRotation value;

	while (sfrotationValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfstringValue (SFString* _field)
{
	////__LOG__ << std::endl;

	std::string value;

	if (_string (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mfstringValue (MFString* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFString value;

	if (sfstringValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfstringValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfstringValues (MFString* _field)
{
	////__LOG__ << std::endl;

	SFString value;

	while (sfstringValue (&value))
		_field -> push_back (value);
}

bool
Parser::sftimeValue (SFTime* _field)
{
	////__LOG__ << std::endl;

	double value;

	if (_double (value))
	{
		_field -> setValue (value);
		return true;
	}

	return false;
}

bool
Parser::mftimeValue (MFTime* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFTime value;

	if (sftimeValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sftimeValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sftimeValues (MFTime* _field)
{
	////__LOG__ << std::endl;

	SFTime value;

	while (sftimeValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec2dValue (SFVec2d* _field)
{
	////__LOG__ << std::endl;

	double x, y;

	if (_double (x))
	{
		if (_double (y))
		{
			_field -> setValue (x, y);
			return true;
		}
	}

	return false;
}

bool
Parser::mfvec2dValue (MFVec2d* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec2d value;

	if (sfvec2dValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec2dValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2dValues (MFVec2d* _field)
{
	////__LOG__ << std::endl;

	SFVec2d value;

	while (sfvec2dValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec2fValue (SFVec2f* _field)
{
	////__LOG__ << std::endl;

	float x, y;

	if (_float (x))
	{
		if (_float (y))
		{
			_field -> setValue (x, y);
			return true;
		}
	}

	return false;
}

bool
Parser::mfvec2fValue (MFVec2f* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec2f value;

	if (sfvec2fValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec2fValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec2fValues (MFVec2f* _field)
{
	////__LOG__ << std::endl;

	SFVec2f value;

	while (sfvec2fValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec3dValue (SFVec3d* _field)
{
	////__LOG__ << std::endl;

	double x, y, z;

	if (_double (x))
	{
		if (_double (y))
		{
			if (_double (z))
			{
				_field -> setValue (x, y, z);
				return true;
			}
		}
	}

	return false;
}

bool
Parser::mfvec3dValue (MFVec3d* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec3d value;

	if (sfvec3dValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec3dValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3dValues (MFVec3d* _field)
{
	////__LOG__ << std::endl;

	SFVec3d value;

	while (sfvec3dValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec3fValue (SFVec3f* _field)
{
	////__LOG__ << std::endl;

	float x, y, z;

	if (_float (x))
	{
		if (_float (y))
		{
			if (_float (z))
			{
				_field -> setValue (x, y, z);
				return true;
			}
		}
	}

	return false;
}

bool
Parser::mfvec3fValue (MFVec3f* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec3f value;

	if (sfvec3fValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec3fValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec3fValues (MFVec3f* _field)
{
	////__LOG__ << std::endl;

	SFVec3f value;

	while (sfvec3fValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec4dValue (SFVec4d* _field)
{
	////__LOG__ << std::endl;

	double x, y, z, w;

	if (_double (x))
	{
		if (_double (y))
		{
			if (_double (z))
			{
				if (_double (w))
				{
					_field -> setValue (x, y, z, w);
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::mfvec4dValue (MFVec4d* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec4d value;

	if (sfvec4dValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec4dValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4dValues (MFVec4d* _field)
{
	////__LOG__ << std::endl;

	SFVec4d value;

	while (sfvec4dValue (&value))
		_field -> push_back (value);
}

bool
Parser::sfvec4fValue (SFVec4f* _field)
{
	////__LOG__ << std::endl;

	float x, y, z, w;

	if (_float (x))
	{
		if (_float (y))
		{
			if (_float (z))
			{
				if (_float (w))
				{
					_field -> setValue (x, y, z, w);
					return true;
				}
			}
		}
	}

	return false;
}

bool
Parser::mfvec4fValue (MFVec4f* _field)
{
	////__LOG__ << std::endl;

	_field -> clear ();

	SFVec4f value;

	if (sfvec4fValue (&value))
	{
		_field -> push_back (value);
		return true;
	}

	if (RegEx::Brackets .Consume (&string))
	{
		comments ();
		return true;
	}

	if (RegEx::OpenBracket .Consume (&string))
	{
		comments ();
		sfvec4fValues (_field);

		if (RegEx::CloseBracket .Consume (&string))
		{
			comments ();
			return true;
		}
		else
			throw Error <INVALID_X3D> ("Expected ']'.");
	}

	return false;
}

void
Parser::sfvec4fValues (MFVec4f* _field)
{
	////__LOG__ << std::endl;

	SFVec4f value;

	while (sfvec4fValue (&value))
		_field -> push_back (value);
}

} // X3D
} // titania
