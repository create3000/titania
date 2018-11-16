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

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/Transform.h"
#include "../Filter.h"
#include "../Grammar.h"
#include "../Parser.h"

#include "Converter.h"
#include "Nodes.h"

namespace titania {
namespace X3D {
namespace VRML1 {

const io::string Parser::fields ("fields");

const std::map <std::string, std::string> Parser::substitutions = {
	std::pair ("SFBitMask", "SFString"),
	std::pair ("SFEnum",    "SFString"),
	std::pair ("SFLong",    "SFInt32"),
	std::pair ("SFMatrix",  "SFMatrix4f"),
	std::pair ("MFBitMask", "MFString"),
	std::pair ("MFEnum",    "MFString"),
	std::pair ("MFLong",    "MFInt32"),
	std::pair ("MFMatrix",  "MFMatrix4f"),
};

const io::character Parser::VerticalBar ('|');

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	       X3D::X3DParser (),
	                scene (scene),
	                  uri (uri),
	              istream (istream),
	                nodes (),
	                stage (scene -> getBrowser () -> createScene (false)),
	executionContextStack (),
	           lineNumber (1),
	          whiteSpaces (),
	      currentComments (),
	    commentCharacters ()
{
	nodes .emplace ("Background",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new Background         (executionContext); });
	nodes .emplace ("CollideStyle",       [ ] (X3D::X3DExecutionContext* const executionContext) { return new CollideStyle       (executionContext); });
	nodes .emplace ("Cone",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cone               (executionContext); });
	nodes .emplace ("Coordinate3",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Coordinate3        (executionContext); });
	nodes .emplace ("Cube",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cube               (executionContext); });
	nodes .emplace ("Cylinder",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Cylinder           (executionContext); });
	nodes .emplace ("DirectedSound",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new DirectedSound      (executionContext); });
	nodes .emplace ("DirectionalLight",   [ ] (X3D::X3DExecutionContext* const executionContext) { return new DirectionalLight   (executionContext); });
	nodes .emplace ("ElevationGrid",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new ElevationGrid      (executionContext); });
	nodes .emplace ("Environment",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Environment        (executionContext); });
	nodes .emplace ("FontStyle",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new FontStyle          (executionContext); });
	nodes .emplace ("GeneralCylinder",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new GeneralCylinder    (executionContext); });
	nodes .emplace ("Group",              [ ] (X3D::X3DExecutionContext* const executionContext) { return new Separator          (executionContext); });
	nodes .emplace ("IndexedFaceSet",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new IndexedFaceSet     (executionContext); });
	nodes .emplace ("IndexedLineSet",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new IndexedLineSet     (executionContext); });
	nodes .emplace ("Info",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Info               (executionContext); });
	nodes .emplace ("LOD",                [ ] (X3D::X3DExecutionContext* const executionContext) { return new LOD                (executionContext); });
	nodes .emplace ("MaterialBinding",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new MaterialBinding    (executionContext); });
	nodes .emplace ("Material",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Material           (executionContext); });
	nodes .emplace ("MatrixTransform",    [ ] (X3D::X3DExecutionContext* const executionContext) { return new MatrixTransform    (executionContext); });
	nodes .emplace ("NavigationInfo",     [ ] (X3D::X3DExecutionContext* const executionContext) { return new NavigationInfo     (executionContext); });
	nodes .emplace ("NormalBinding",      [ ] (X3D::X3DExecutionContext* const executionContext) { return new NormalBinding      (executionContext); });
	nodes .emplace ("Normal",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Normal             (executionContext); });
	nodes .emplace ("OrthographicCamera", [ ] (X3D::X3DExecutionContext* const executionContext) { return new OrthographicCamera (executionContext); });
	nodes .emplace ("PerspectiveCamera",  [ ] (X3D::X3DExecutionContext* const executionContext) { return new PerspectiveCamera  (executionContext); });
	nodes .emplace ("PointLight",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointLight         (executionContext); });
	nodes .emplace ("PointSet",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointSet           (executionContext); });
	nodes .emplace ("PointSound",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new PointSound         (executionContext); });
	nodes .emplace ("Rotation",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Rotation           (executionContext); });
	nodes .emplace ("Scale",              [ ] (X3D::X3DExecutionContext* const executionContext) { return new Scale              (executionContext); });
	nodes .emplace ("Separator",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new Separator          (executionContext); });
	nodes .emplace ("ShapeHints",         [ ] (X3D::X3DExecutionContext* const executionContext) { return new ShapeHints         (executionContext); });
	nodes .emplace ("Sphere",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Sphere             (executionContext); });
	nodes .emplace ("SpotLight",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new SpotLight          (executionContext); });
	nodes .emplace ("Switch",             [ ] (X3D::X3DExecutionContext* const executionContext) { return new Switch             (executionContext); });
	nodes .emplace ("TextLanguage",       [ ] (X3D::X3DExecutionContext* const executionContext) { return new TextLanguage       (executionContext); });
	nodes .emplace ("Texture2Transform",  [ ] (X3D::X3DExecutionContext* const executionContext) { return new Texture2Transform  (executionContext); });
	nodes .emplace ("Texture2",           [ ] (X3D::X3DExecutionContext* const executionContext) { return new Texture2           (executionContext); });
	nodes .emplace ("TextureCoordinate2", [ ] (X3D::X3DExecutionContext* const executionContext) { return new TextureCoordinate2 (executionContext); });
	nodes .emplace ("Text",               [ ] (X3D::X3DExecutionContext* const executionContext) { return new Text               (executionContext); });
	nodes .emplace ("Transform",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new Transform          (executionContext); });
	nodes .emplace ("TransformSeparator", [ ] (X3D::X3DExecutionContext* const executionContext) { return new Separator          (executionContext); });
	nodes .emplace ("Translation",        [ ] (X3D::X3DExecutionContext* const executionContext) { return new Translation        (executionContext); });
	nodes .emplace ("WorldInfo",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WorldInfo          (executionContext); });
	nodes .emplace ("WWWAnchor",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WWWAnchor          (executionContext); });
	nodes .emplace ("WWWInline",          [ ] (X3D::X3DExecutionContext* const executionContext) { return new WWWInline          (executionContext); });
}

void
Parser::parseIntoScene ()
{
	try
	{
		scene -> setWorldURL (uri);
		scene -> setEncoding (EncodingType::VRML);
		scene -> setProfile (getBrowser () -> getProfile ("Full"));

		istream .imbue (std::locale::classic ());

		vrmlScene ();

		convert ();

		optimize ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		__LOG__ << istream .rdbuf () << std::endl;
		//throw;
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown parser error.");
	}
}

void
Parser::convert ()
{
	Converter converter (scene);

	for (const auto & node : stage -> getRootNodes ())
	{
		const auto vrml1Node = dynamic_cast <VRML1Node*> (node .getValue ());

		if (vrml1Node)
			vrml1Node -> convert (&converter);
	}
}

void
Parser::optimize ()
{
	for (auto & child : scene -> getRootNodes ())
		child = optimize (child);

	scene -> getRootNodes () .remove (nullptr);
}

X3D::SFNode &
Parser::optimize (X3D::SFNode & node)
{
	const auto groupNode = dynamic_cast <X3D::X3DGroupingNode*> (node .getValue ());

	if (groupNode)
	{
		for (auto & child : groupNode -> children ())
			child = optimize (child);
	
		groupNode -> children () .remove (nullptr);
	
		if (groupNode -> children () .size () == 0)
		  return node = nullptr;

		if (not groupNode -> metadata ())
		{
			if (groupNode -> children () .size () == 1)
			{
				const auto transformNode = dynamic_cast <X3D::Transform*> (groupNode);
		
				if (transformNode)
				{
					bool onlyDefaults = true;
		
					onlyDefaults &= transformNode -> translation ()      == X3D::Vector3f ();
					onlyDefaults &= transformNode -> rotation ()         == X3D::Rotation4d ();
					onlyDefaults &= transformNode -> scale ()            == X3D::Vector3f (1, 1, 1);
					onlyDefaults &= transformNode -> scaleOrientation () == X3D::Rotation4d ();
					onlyDefaults &= transformNode -> center ()           == X3D::Vector3f ();

					if (onlyDefaults)
						return node = transformNode -> children () [0];
				}
			}
		}
	}

	return node;
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

	X3D::Grammar::WhiteSpaces (istream, whiteSpaces);

	lines (whiteSpaces);

	whiteSpaces .clear ();

	if (X3D::Grammar::Comment (istream, commentCharacters))
	{
		X3D::FilterControlCharacters (commentCharacters);
		X3D::FilterBadUTF8Characters (commentCharacters);
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

void
Parser::vrmlScene ()
{
	//__LOG__ << this << " " << std::endl;

	pushExecutionContext (stage);

	std::string encoding, specificationVersion, characterEncoding, comment;

	if (headerStatement (encoding, specificationVersion, characterEncoding, comment))
	{
		scene -> setSpecificationVersion (specificationVersion);
		scene -> setCharacterEncoding (characterEncoding);
		scene -> setComment (comment);
	}

	statements ();

	popExecutionContext ();

	if (istream .peek () not_eq std::char_traits <char>::eof ())
	{
		if (getBrowser () -> isStrict ())
			throw Error <X3D::INVALID_X3D> ("Unknown statement.");
	}
}

bool
Parser::headerStatement (std::string & _encoding, std::string & _specificationVersion, std::string & _characterEncoding, std::string & _comment)
{
	//__LOG__ << this << " " << std::endl;

	std::string _header;

	if (X3D::Grammar::Comment (istream, _header))
	{
		if (X3D::Grammar::Header (_header, _encoding, _specificationVersion, _characterEncoding, _comment))
			return true;
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

	//if (protoStatement ())
	//	return true;

	X3D::SFNode _node;

	if (nodeStatement (_node))
	{
		addRootNode (std::move (_node));
		return true;
	}

	return false;
}

bool
Parser::nodeStatement (X3D::SFNode & _node)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (X3D::Grammar::DEF (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			X3D::FilterBadUTF8Characters (_nodeNameId);

			if (node (_node, _nodeNameId))
				return true;

			throw Error <X3D::INVALID_X3D> ("Expected node name after DEF.");
		}

		throw Error <X3D::INVALID_X3D> ("No name given after DEF.");
	}

	if (X3D::Grammar::USE (istream))
	{
		std::string _nodeNameId;

		if (nodeNameId (_nodeNameId))
		{
			X3D::FilterBadUTF8Characters (_nodeNameId);

			_node = getExecutionContext () -> getNamedNode (_nodeNameId);

			return true;
		}

		throw Error <X3D::INVALID_X3D> ("No name given after USE.");
	}

	if (X3D::Grammar::NULL_ (istream))
	{
		_node = nullptr;

		return true;
	}

	if (node (_node))
		return true;

	return false;
}

//Nodes

bool
Parser::node (X3D::SFNode & _node, const std::string & _nodeNameId)
{
	//__LOG__ << this << " " << _nodeNameId << std::endl;

	auto state = istream .rdstate ();
	auto pos   = istream .tellg ();
	auto ln    = lineNumber;
	auto com   = currentComments .size ();

	std::string _nodeTypeId;

	if (nodeTypeId (_nodeTypeId))
	{
		comments ();

		if (X3D::Grammar::OpenBrace (istream))
		{
			//__LOG__ << this << " " << _nodeTypeId << std::endl;
	
			const auto iter = nodes .find (_nodeTypeId);
	
			if (iter == nodes .end ())
			{
				_node = X3D::MakePtr <CustomNode> (getExecutionContext ());

				fieldsElements (_node);
			}
			else
			{
				_node = X3D::SFNode (iter -> second (getExecutionContext ()));
			}
	
	
			//__LOG__ << this << " " << _nodeTypeId << " " << (void*) _node << std::endl;
	
			if (not _nodeNameId .empty ())
			{
				try
				{
					const SFNode namedNode = getExecutionContext () -> getNamedNode (_nodeNameId); // Create copy!
	
					getExecutionContext () -> updateNamedNode (getExecutionContext () -> getUniqueName (_nodeNameId), namedNode);
				}
				catch (const X3D::X3DError &)
				{ }
	
				getExecutionContext () -> updateNamedNode (_nodeNameId, _node);
			}

			nodeBody (_node);

			comments ();

			if (Grammar::CloseBrace (istream))
			{
				//__LOG__ << this << " " << _nodeTypeId << std::endl;

				addUninitializedNode (_node);

				//__LOG__ << this << " " << _nodeTypeId << std::endl;
				return true;
			}

			throw Error <X3D::INVALID_X3D> ("Expected '}' at the end of node body.");
		}
		else
		{
			// Reset stream position.
	
			lineNumber = ln;
			currentComments .resize (com);
	
			istream .clear (state);
	
			for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
				istream .unget ();

			return false;
		}

		throw Error <X3D::INVALID_X3D> ("Expected '{' at the beginning of node body.");
	}

	//__LOG__ << this << " " << std::endl;
	return false;
}

void
Parser::nodeBody (const X3D::SFNode & _node)
{
	//__LOG__ << this << " " << std::endl;

	while (nodeBodyElement (_node))
		;
}

bool
Parser::nodeBodyElement (const X3D::SFNode & _node)
{
	//__LOG__ << this << " " << std::endl;

	X3D::SFNode _child;

	if (nodeStatement (_child))
	{
		_node -> getField <X3D::MFNode> ("children") .emplace_back (_child);
		return true;
	}

	std::string _fieldId;

	if (Id (_fieldId))
	{
		X3D::X3DFieldDefinition* _field = nullptr;

		try
		{
			_field = _node -> getField (_fieldId);
		}
		catch (const Error <X3D::INVALID_NAME> &)
		{
			throw Error <X3D::INVALID_X3D> ("Unknown field '" + _fieldId + "' in class '" + _node -> getTypeName () + "'.");
		}

		if (_field -> isInitializable ())
		{
			if (fieldValue (_field))
				return true;

			throw Error <X3D::INVALID_X3D> ("Couldn't read value for field '" + _fieldId + "'.");
		}

		throw Error <X3D::INVALID_X3D> ("Couldn't assign value to " + to_string (_field -> getAccessType ()) + " field '" + _fieldId + "'.");
	}

	return false;
}

void
Parser::fieldsElements (const X3D::SFNode & _node)
{
	comments ();

	if (fields (istream))
	{
		comments ();

		if (X3D::Grammar::OpenBracket (istream))
		{
			while (fieldElement (_node))
				;

			if (X3D::Grammar::CloseBracket (istream))
				return;
		}

		throw Error <X3D::INVALID_X3D> ("Couldn't parse custom fields.");
	}
}

bool
Parser::fieldElement (const X3D::SFNode & _node)
{
	comments ();

	std::string _fieldType;

	if (Id (_fieldType))
	{
		comments ();

		std::string _fieldName;

		if (Id (_fieldName))
		{
			const auto iter = substitutions .find (_fieldType);

			if (iter not_eq substitutions .end ())
				_fieldType = iter -> second;

			const auto _field = getBrowser () -> getSupportedField (_fieldType) -> create ();

			_node -> addUserDefinedField (initializeOnly, _fieldName, _field);

			return true;
		}
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
Parser::fieldValue (X3D::X3DFieldDefinition* const field)
{
	field -> isSet (true);

	X3D::Parser parser (istream, scene);

	parser .setUnits (false);

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
			return parser .sfboolValue (static_cast <X3D::SFBool*> (field));

		case X3D::X3DConstants::SFColor:
			return parser .sfcolorValue (static_cast <X3D::SFColor*> (field));

		case X3D::X3DConstants::SFColorRGBA:
			return parser .sfcolorRGBAValue (static_cast <X3D::SFColorRGBA*> (field));

		case X3D::X3DConstants::SFDouble:
			return parser .sfdoubleValue (static_cast <X3D::SFDouble*> (field));

		case X3D::X3DConstants::SFFloat:
			return parser .sffloatValue (static_cast <X3D::SFFloat*> (field));

		case X3D::X3DConstants::SFImage:
			return parser .sfimageValue (static_cast <X3D::SFImage*> (field));

		case X3D::X3DConstants::SFInt32:
			return parser .sfint32Value (static_cast <X3D::SFInt32*> (field));

		case X3D::X3DConstants::SFMatrix3d:
			return parser .sfmatrix3dValue (static_cast <X3D::SFMatrix3d*> (field));

		case X3D::X3DConstants::SFMatrix3f:
			return parser .sfmatrix3fValue (static_cast <X3D::SFMatrix3f*> (field));

		case X3D::X3DConstants::SFMatrix4d:
			return parser .sfmatrix4dValue (static_cast <X3D::SFMatrix4d*> (field));

		case X3D::X3DConstants::SFMatrix4f:
			return parser .sfmatrix4fValue (static_cast <X3D::SFMatrix4f*> (field));

		case X3D::X3DConstants::SFNode:
			return sfnodeValue (static_cast <X3D::SFNode*> (field));

		case X3D::X3DConstants::SFRotation:
			return parser .sfrotationValue (static_cast <X3D::SFRotation*> (field));

		case X3D::X3DConstants::SFString:
			return sfstringValue (static_cast <X3D::SFString*> (field));

		case X3D::X3DConstants::SFTime:
			return parser .sftimeValue (static_cast <X3D::SFTime*> (field));

		case X3D::X3DConstants::SFVec2d:
			return parser .sfvec2dValue (static_cast <X3D::SFVec2d*> (field));

		case X3D::X3DConstants::SFVec2f:
			return parser .sfvec2fValue (static_cast <X3D::SFVec2f*> (field));

		case X3D::X3DConstants::SFVec3d:
			return parser .sfvec3dValue (static_cast <X3D::SFVec3d*> (field));

		case X3D::X3DConstants::SFVec3f:
			return parser .sfvec3fValue (static_cast <X3D::SFVec3f*> (field));

		case X3D::X3DConstants::SFVec4d:
			return parser .sfvec4dValue (static_cast <SFVec4d*> (field));

		case X3D::X3DConstants::SFVec4f:
			return parser .sfvec4fValue (static_cast <SFVec4f*> (field));

		case X3D::X3DConstants::MFBool:
			return parser .mfboolValue (static_cast <X3D::MFBool*> (field));

		case X3D::X3DConstants::MFColor:
			return parser .mfcolorValue (static_cast <X3D::MFColor*> (field));

		case X3D::X3DConstants::MFColorRGBA:
			return parser .mfcolorRGBAValue (static_cast <X3D::MFColorRGBA*> (field));

		case X3D::X3DConstants::MFDouble:
			return parser .mfdoubleValue (static_cast <X3D::MFDouble*> (field));

		case X3D::X3DConstants::MFFloat:
			return parser .mffloatValue (static_cast <X3D::MFFloat*> (field));

		case X3D::X3DConstants::MFImage:
			return parser .mfimageValue (static_cast <X3D::MFImage*> (field));

		case X3D::X3DConstants::MFInt32:
			return parser .mfint32Value (static_cast <X3D::MFInt32*> (field));

		case X3D::X3DConstants::MFMatrix3d:
			return parser .mfmatrix3dValue (static_cast <X3D::MFMatrix3d*> (field));

		case X3D::X3DConstants::MFMatrix3f:
			return parser .mfmatrix3fValue (static_cast <X3D::MFMatrix3f*> (field));

		case X3D::X3DConstants::MFMatrix4d:
			return parser .mfmatrix4dValue (static_cast <X3D::MFMatrix4d*> (field));

		case X3D::X3DConstants::MFMatrix4f:
			return parser .mfmatrix4fValue (static_cast <X3D::MFMatrix4f*> (field));

		case X3D::X3DConstants::MFNode:
			return false;

		case X3D::X3DConstants::MFRotation:
			return parser .mfrotationValue (static_cast <X3D::MFRotation*> (field));

		case X3D::X3DConstants::MFString:
			return parser .mfstringValue (static_cast <X3D::MFString*> (field));

		case X3D::X3DConstants::MFTime:
			return parser .mftimeValue (static_cast <X3D::MFTime*> (field));

		case X3D::X3DConstants::MFVec2d:
			return parser .mfvec2dValue (static_cast <X3D::MFVec2d*> (field));

		case X3D::X3DConstants::MFVec2f:
			return parser .mfvec2fValue (static_cast <X3D::MFVec2f*> (field));

		case X3D::X3DConstants::MFVec3d:
			return parser .mfvec3dValue (static_cast <X3D::MFVec3d*> (field));

		case X3D::X3DConstants::MFVec3f:
			return parser .mfvec3fValue (static_cast <X3D::MFVec3f*> (field));

		case X3D::X3DConstants::MFVec4d:
			return parser .mfvec4dValue (static_cast <X3D::MFVec4d*> (field));

		case X3D::X3DConstants::MFVec4f:
			return parser .mfvec4fValue (static_cast <X3D::MFVec4f*> (field));
	}

	return false;
}

bool
Parser::String (std::string & _value)
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (X3D::Grammar::String (istream, _value))
	{
		X3D::FilterBadUTF8Characters (_value);
		lines (_value);
		return true;
	}

	return false;
}

bool
Parser::sfstringValue (X3D::SFString* _field)
{
	//__LOG__ << this << " " << std::endl;

	std::string value;

	if (String (value))
	{
		_field -> setValue (value);
		return true;
	}

	// Parse SFEnum and SFBitMask

	_field -> clear ();

	std::string _Id;

	while (Id (_Id))
	{
		_field -> append (_Id);

		comments ();

		if (VerticalBar (istream))
		{
			_field -> append ('|');

			comments ();

			continue;
		}

		return true;
	}

	return false;
}

bool
Parser::sfnodeValue (X3D::SFNode* _field)
{
	//__LOG__ << this << " " << std::endl;

	return nodeStatement (*_field);
}

Parser::~Parser ()
{ }

} // Wavefront
} // X3D
} // titania
