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

#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Grouping/Group.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Texturing/ImageTexture.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../InputOutput/FileLoader.h"
#include "../Filter.h"

// Grammar
#include <Titania/InputOutput.h>
#include <Titania/InputOutput/InverseSequence.h>
#include <Titania/String.h>

#include <regex>

namespace titania {
namespace X3D {
namespace Wavefront {

class Grammar
{
public:

	///  @name General
	static const io::sequence            WhiteSpaces;
	static const io::sequence            WhiteSpacesNoLineTerminator;
	static const io::single_line_comment Comment;

	///  @name Keywords
	static const io::string    mtllib;
	static const io::string    usemtl;
	static const io::string    newmtl;
	static const io::string    Ka;
	static const io::string    Kd;
	static const io::string    Ks;
	static const io::string    Ns;
	static const io::character d;
	static const io::string    Tr;
	static const io::string    illum;
	static const io::string    map_Kd;

	static const io::inverse_sequence string;
	static const io::character        o;
	static const io::string           v;
	static const io::string           vt;
	static const io::string           vn;
	static const io::character        g;
	static const io::character        s;
	static const io::string           off;
	static const io::character        f;
	static const io::character        Slash;

};

const io::sequence            Grammar::WhiteSpaces ("\r\n \t");
const io::sequence            Grammar::WhiteSpacesNoLineTerminator (" \t");
const io::single_line_comment Grammar::Comment ("#");

const io::string    Grammar::mtllib ("mtllib");
const io::string    Grammar::usemtl ("usemtl");
const io::string    Grammar::newmtl ("newmtl");
const io::string    Grammar::Ka ("Ka");
const io::string    Grammar::Kd ("Kd");
const io::string    Grammar::Ks ("Ks");
const io::string    Grammar::Ns ("Ns");
const io::character Grammar::d ('d');
const io::string    Grammar::Tr ("Tr");
const io::string    Grammar::illum ("illum");
const io::string    Grammar::map_Kd ("map_Kd");

const io::inverse_sequence Grammar::string ("\r\n");
const io::character        Grammar::o ('o');
const io::string           Grammar::v ("v ");
const io::string           Grammar::vt ("vt");
const io::string           Grammar::vn ("vn");
const io::character        Grammar::g ('g');
const io::character        Grammar::s ('s');
const io::string           Grammar::off ("off");
const io::character        Grammar::f ('f');
const io::character        Grammar::Slash ('/');

namespace {

class MaterialParser
{
public:

	///  @name Construction

	MaterialParser (const X3D::X3DScenePtr &, std::istream &);

	///  @name Member access

	const std::map <std::string, X3D::X3DPtr <Material>> &
	getMaterials () const
	{ return materials; }

	const std::map <std::string, X3D::X3DPtr <ImageTexture>> &
	getTextures () const
	{ return textures; }

	///  @name Operations

	void
	parse ();

	///  @name Destruction

	virtual
	~MaterialParser ()
	{ }


private:

	///  @name Operations

	void
	comments ();

	bool
	comment ();

	void
	whiteSpaces ();

	void
	whiteSpacesNoLineTerminator ();

	void
	statements ();

	bool
	statement ();

	bool
	newmtl ();

	bool
	Ka ();

	bool
	Kd ();

	bool
	Ks ();

	bool
	Ns ();

	bool
	d ();

	bool
	Tr ();

	bool
	illum ();

	bool
	map_Kd ();

	bool
	Int32 (int32_t &);

	bool
	Float (float &);

	bool
	Color (Color3f &);

	///  @name Members

	const X3D::X3DScenePtr scene;
	std::istream &         istream;
	size_t                 lineNumber;
	std::string            whiteSpaceCharacters;
	std::string            commentCharacters;

	std::map <std::string, X3D::X3DPtr <X3D::Material>>     materials;
	X3D::X3DPtr <X3D::Material>                             material;
	std::string                                             name;
	std::map <std::string, X3D::X3DPtr <X3D::ImageTexture>> textures;

};

MaterialParser::MaterialParser (const X3D::X3DScenePtr & scene, std::istream & istream) :
	               scene (scene),
	             istream (istream),
	          lineNumber (0),
	whiteSpaceCharacters (),
	   commentCharacters (),
	           materials (),
	            material (scene -> createNode <Material> ()),
	                name (),
	            textures ()
{ }

void
MaterialParser::parse ()
{
	//__LOG__ << this << " " << std::endl;

	istream .imbue (std::locale::classic ());

	try
	{
		statements ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		__LOG__ << istream .rdbuf () << std::endl;
		throw;
	}
}

void
MaterialParser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	while (comment ())
		;
}

bool
MaterialParser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	whiteSpaces ();

	return Grammar::Comment (istream, commentCharacters);
}

void
MaterialParser::whiteSpaces ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaceCharacters);

	lineNumber += std::count (whiteSpaceCharacters .cbegin (), whiteSpaceCharacters .cend (), '\n');

	whiteSpaceCharacters .clear ();
}

void
MaterialParser::whiteSpacesNoLineTerminator ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpacesNoLineTerminator (istream, whiteSpaceCharacters);

	whiteSpaceCharacters .clear ();
}

void
MaterialParser::statements ()
{
	//__LOG__ << this << " " << std::endl;

	while (statement ())
		;
}

bool
MaterialParser::statement ()
{
	//__LOG__ << this << " " << std::endl;

	if (newmtl ())
		return true;

	if (Ka ())
		return true;

	if (Kd ())
		return true;

	if (Ks ())
		return true;

	if (Ns ())
		return true;

	if (d ())
		return true;

	if (Tr ())
		return true;

	if (illum ())
		return true;

	if (map_Kd ())
		return true;

	// Skip empty and unkown lines.

	if (Grammar::string (istream, whiteSpaceCharacters))
		return true;

	return false;
}

bool
MaterialParser::newmtl ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::newmtl (istream))
	{
		whiteSpacesNoLineTerminator ();
		
		name .clear ();

		if (Grammar::string (istream, name))
		{
			material = scene -> createNode <X3D::Material> ();

			materials [name] = material;

			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Ka ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Ka (istream))
	{
		Color3f color;

		if (Color (color))
		{
			const auto hsv = color .hsv ();

			material -> ambientIntensity () = hsv [2];

			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Kd ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Kd (istream))
	{
		Color3f color;

		if (Color (color))
		{
			material -> diffuseColor () = color;
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Ks ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Ks (istream))
	{
		Color3f color;

		if (Color (color))
		{
			material -> specularColor () = color;
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Ns ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Ns (istream))
	{
		float value;

		if (Float (value))
		{
			material -> shininess () = value / 1000;
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::d ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::d (istream))
	{
		float value;

		if (Float (value))
		{
			material -> transparency () = 1 - value;
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Tr ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::Tr (istream))
	{
		float value;

		if (Float (value))
		{
			material -> transparency () = value;
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::illum ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::illum (istream))
	{
		int32_t value;

		if (Int32 (value))
		{
			// Don't know what to do with illum value in X3D.
			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::map_Kd ()
{
	//__LOG__ << this << " " << std::endl;

	static const auto backslash = std::regex (R"/(\\)/");

	comments ();

	if (Grammar::map_Kd (istream))
	{
		whiteSpacesNoLineTerminator ();

		std::string string;

		if (Grammar::string (istream, string))
		{
			if (not string .empty () and not name .empty ())
			{
				auto url = std::vector <std::string> ();

				basic::split (std::back_inserter (url), basic::trim (string), " ");
				
				if (not url .empty ())
				{
					const auto texture       = scene -> createNode <X3D::ImageTexture> ();
					const auto normalizedURL = basic::path (std::regex_replace (url .back (), backslash, "/"), "/") .escape () .str ();

					texture -> url () = {
						normalizedURL,
						scene -> getWorldURL () .transform (normalizedURL) .str ()
					};

					textures [name] = texture;
				}
			}

			return true;
		}

		Grammar::string (istream, whiteSpaceCharacters); // Parse until end of line.
		return true;
	}

	return false;
}

bool
MaterialParser::Int32 (int32_t & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
MaterialParser::Float (float & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
MaterialParser::Color (Color3f & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;

}

}        // anon namespace

Parser::Parser (const X3D::X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	      X3D::X3DParser (),
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	          lineNumber (0),
	whiteSpaceCharacters (),
	   commentCharacters (),
	     currentComments (),
	     defaultMaterial (scene -> createNode <X3D::Material> ()),
	            material (defaultMaterial),
	             texture (),
	            texCoord (scene -> createNode <X3D::TextureCoordinate> ()),
	              normal (scene -> createNode <X3D::Normal> ()),
	               coord (scene -> createNode <X3D::Coordinate> ()),
	            geometry (),
	               shape (),
	               group (scene -> createNode <X3D::Transform> ()),
	              object (scene -> createNode <X3D::Transform> ()),
	      smoothingGroup (0),
	     smoothingGroups (),
	           materials (),
	            textures ()
{
	object -> children () .emplace_back (group);
	scene -> getRootNodes () .emplace_back (object);
}

void
Parser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	scene -> setWorldURL (uri);
	scene -> setEncoding (EncodingType::XML);
	scene -> setProfile (getBrowser () -> getProfile ("Full"));

	istream .imbue (std::locale::classic ());

	try
	{
		statements ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		__LOG__ << istream .rdbuf () << std::endl;
		throw;
	}
	catch (...)
	{
		throw Error <INVALID_X3D> ("Unkown parser error.");
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

	whiteSpaces ();

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
Parser::whiteSpaces ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaceCharacters);

	lineNumber += std::count (whiteSpaceCharacters .cbegin (), whiteSpaceCharacters .cend (), '\n');

	whiteSpaceCharacters .clear ();
}

void
Parser::whiteSpacesNoLineTerminator ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpacesNoLineTerminator (istream, whiteSpaceCharacters);

	whiteSpaceCharacters .clear ();
}

void
Parser::statements ()
{
	//__LOG__ << this << " " << std::endl;

	while (statement ())
		;
}

bool
Parser::statement ()
{
	//__LOG__ << this << " " << std::endl;

	if (mtllib ())
		return true;

	if (usemtl ())
		return true;

	if (o ())
		return true;

	if (g ())
		return true;

	if (s ())
		return true;

	if (vts ())
		return true;

	if (vns ())
		return true;

	if (vs ())
		return true;

	if (fs ())
		return true;

	// Skip empty and unkown lines.

	if (Grammar::string (istream, whiteSpaceCharacters))
		return true;

	return false;
}

bool
Parser::mtllib ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::mtllib (istream))
	{
		whiteSpacesNoLineTerminator ();

		std::string string;

		if (Grammar::string (istream, string))
		{
			auto mtllibs = std::vector <std::string> ();

			basic::split (std::back_inserter (mtllibs), string, " ");

			for (const auto & mtllib : mtllibs)
			{
				try
				{
					auto material_istream = FileLoader (scene) .loadStream (mtllib);

					MaterialParser materialParser (scene, material_istream);

					materialParser .parse ();

					for (const auto & material : materialParser .getMaterials ())
						materials [material .first] = material .second;

					for (const auto & texture : materialParser .getTextures ())
						textures [texture .first] = texture .second;
				}
				catch (const X3DError &)
				{ }
			}
		}

		return true;
	}

	return false;
}

bool
Parser::usemtl ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::usemtl (istream))
	{
		whiteSpacesNoLineTerminator ();

		std::string name;

		if (Grammar::string (istream, name))
		{
			try
			{
				material = materials .at (name);
			}
			catch (const std::out_of_range &)
			{
				material = defaultMaterial;
			}

			try
			{
				texture = textures .at (name);
			}
			catch (const std::out_of_range &)
			{
				texture = nullptr;
			}
		}

		return true;
	}

	return false;
}

bool
Parser::Parser::o ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::o (istream))
	{
		whiteSpacesNoLineTerminator ();

		std::string name;

		if (Grammar::string (istream, name))
		{
			if (not name .empty ())
				name = GetNameFromString (name);

			if (not group -> children () .empty ())
			{
				object = scene -> createNode <X3D::Transform> ();
				group  = scene -> createNode <X3D::Transform> ();

				object -> children () .emplace_back (group);
				scene -> getRootNodes () .emplace_back (object);
			}

			if (not name .empty ())
				scene -> updateNamedNode (name, SFNode (object));
		}

		return true;
	}

	return false;
}

bool
Parser::g ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::g (istream))
	{
		whiteSpacesNoLineTerminator ();

		std::string name;

		if (Grammar::string (istream, name))
		{
			if (not name .empty ())
				name = GetNameFromString (name);

			try
			{
				group = scene -> getNamedNode <Transform> (name);
			}
			catch (const X3DError &)
			{
				if (not group -> children () .empty ())
				{
					group = scene -> createNode <X3D::Transform> ();

					object -> children () .emplace_back (group);
				}

				if (not name .empty ())
					scene -> updateNamedNode (name, SFNode (group));
			}
		}

		return true;
	}

	return false;
}

bool
Parser::s ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::s (istream))
	{
		whiteSpacesNoLineTerminator ();

		if (Grammar::off (istream))
		{
			smoothingGroup = 0;
			return true;
		}

		if (Int32 (smoothingGroup))
			return true;

		return true;
	}

	return false;
}

bool
Parser::vts ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::vt .lookahead (istream))
	{
		while (vt ())
		;

		return true;
	}

	return false;
}

bool
Parser::vt ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::vt (istream))
	{
		Vector2f value;

		if (Vec2f (value))
		{
			texCoord -> point () .emplace_back (value);

			return true;
		}

		throw Error <INVALID_X3D> ("Expected a texture coodinate.");
	}

	return false;
}

bool
Parser::vns ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::vn .lookahead (istream))
	{
		while (vn ())
		;

		return true;
	}

	return false;
}

bool
Parser::vn ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::vn (istream))
	{
		Vector3f value;

		if (Vec3f (value))
		{
			normal -> vector () .emplace_back (value);

			return true;
		}

		throw Error <INVALID_X3D> ("Expected a normal vector.");
	}

	return false;
}

bool
Parser::vs ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::v .lookahead (istream))
	{
		while (v ())
		;

		return true;
	}

	return false;
}

bool
Parser::v ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::v (istream))
	{
		Vector3f value;

		if (Vec3f (value))
		{
			coord -> point () .emplace_back (value);

			return true;
		}

		throw Error <INVALID_X3D> ("Expected a vertex coordinate.");
	}

	return false;
}

bool
Parser::fs ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::f .lookahead (istream))
	{
		try
		{
			shape    = smoothingGroups .at (group -> getName ()) .at (smoothingGroup);
			geometry = X3DPtr <IndexedFaceSet> (shape -> geometry ());
		}
		catch (const std::out_of_range &)
		{
			const auto appearance = scene -> createNode <X3D::Appearance> ();

			geometry = scene -> createNode <X3D::IndexedFaceSet> ();
			shape    = scene -> createNode <X3D::Shape> ();

			appearance -> material ()  = material;
			appearance -> texture ()   = texture;
			geometry -> creaseAngle () = smoothingGroup ? pi <float> : 0.0f;
			shape -> appearance ()     = appearance;
			shape -> geometry ()       = geometry;

			group -> children () .emplace_back (shape);

			smoothingGroups [group -> getName ()] [smoothingGroup] = shape;
		}

		while (f ())
		;

		if (geometry -> texCoordIndex () .size ())
			geometry -> texCoord () = texCoord;

		if (geometry -> normalIndex () .size ())
			geometry -> normal () = normal;

		geometry -> coord () = coord;

		return true;
	}

	return false;
}

bool
Parser::f ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();

	if (Grammar::f (istream))
	{
		const size_t texCoords = geometry -> texCoordIndex () .size ();
		const size_t normals   = geometry -> normalIndex () .size ();

		while (indices ())
		;

		if (geometry -> texCoordIndex () .size () not_eq texCoords)
			geometry -> texCoordIndex () .emplace_back (-1);

		if (geometry -> normalIndex () .size () not_eq normals)
			geometry -> normalIndex () .emplace_back (-1);

		geometry -> coordIndex () .emplace_back (-1);
		return true;
	}

	return false;
}

bool
Parser::indices ()
{
	//__LOG__ << this << " " << std::endl;

	int32_t value;

	if (Int32 (value))
	{
		geometry -> coordIndex () .emplace_back (getIndex (value, coord -> point () .size ()));

		if (Grammar::Slash (istream))
		{
			if (Int32 (value))
			{
				geometry -> texCoordIndex () .emplace_back (getIndex (value, texCoord -> point () .size ()));
			}

			if (Grammar::Slash (istream))
			{
				if (Int32 (value))
				{
					geometry -> normalIndex () .emplace_back (getIndex (value, normal -> vector () .size ()));
				}
			}
		}

		return true;
	}

	return false;
}

int32_t
Parser::getIndex (const int32_t index, const int32_t size)
{
	if (index == 0)
		throw Error <INVALID_X3D> ("Invalid index.");

	if (index < 0)
		return size + index;

	return index - 1;
}

bool
Parser::Int32 (int32_t & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Vec2f (Vector2f & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Vec3f (Vector3f & value)
{
	//__LOG__ << this << " " << std::endl;

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

Parser::~Parser ()
{ }

} // Wavefront
} // X3D
} // titania
