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

#include "../Filter.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Shape/Shape.h"
#include "../../Components/Grouping/Group.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Components/Shape/Material.h"
#include "../../Components/Shape/Appearance.h"

// Grammar
#include <Titania/InputOutput.h>

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
	static const io::string         mtllib;
	static const io::string         usemtl;
	static const io::inverse_string string;
	static const io::character      o;
	static const io::string         v;
	static const io::string         vt;
	static const io::string         vn;
	static const io::character      g;
	static const io::character      s;
	static const io::string         off;
	static const io::character      f;
	static const io::character      Slash;

};

const io::sequence            Grammar::WhiteSpaces ("\r\n \t");
const io::sequence            Grammar::WhiteSpacesNoLineTerminator (" \t");
const io::single_line_comment Grammar::Comment ("#");

const io::string         Grammar::mtllib ("mtllib");
const io::string         Grammar::usemtl ("usemtl");
const io::inverse_string Grammar::string ("\n");
const io::character      Grammar::o ('o');
const io::string         Grammar::v ("v ");
const io::string         Grammar::vt ("vt");
const io::string         Grammar::vn ("vn");
const io::character      Grammar::g ('g');
const io::character      Grammar::s ('s');
const io::string         Grammar::off ("off");
const io::character      Grammar::f ('f');
const io::character      Grammar::Slash ('/');

Parser::Parser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	          lineNumber (0),
	whiteSpaceCharacters (),
	   commentCharacters (),
	     currentComments (),
	            material (scene -> createNode ("Material")),
	            texCoord (),
	              normal (),
	               coord (),
	            geometry (),
	               shape (),
	               group (scene -> createNode ("Transform")),
	              object (scene -> createNode ("Transform")),
	       texCoordIndex (1),
	         normalIndex (1),
	          coordIndex (1),
	         creaseAngle (0)
{
	scene -> addUninitializedNode (material);
	scene -> addUninitializedNode (group);
	scene -> addUninitializedNode (object);

	object -> children () .emplace_back (group);
	scene -> getRootNodes () .emplace_back (object);
}

void
Parser::parseIntoScene ()
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
		filter_control_characters (commentCharacters);
		filter_bad_utf8_characters (commentCharacters);
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

	lineNumber += std::count (whiteSpaceCharacters .begin (), whiteSpaceCharacters .end (), '\n');

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

	// Skip unkown lines.

	std::string string;

	if (Grammar::string (istream, string))
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

		std::string uri;

		if (Grammar::string (istream, uri))
		{
			return true;
		}
			
		throw Error <INVALID_X3D> ("Expected uri.");
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
			return true;
		}
			
		throw Error <INVALID_X3D> ("Expected material name.");
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
			if (not group -> children () .empty ())
			{
				object = X3DPtr <Transform> (scene -> createNode ("Transform"));
				group  = X3DPtr <Transform> (scene -> createNode ("Transform"));

				scene -> addUninitializedNode (object);
				scene -> addUninitializedNode (group);
	
				object -> children () .emplace_back (group);
				scene -> getRootNodes () .emplace_back (object);
			}

			if (not name .empty ())
				scene -> updateNamedNode (name, SFNode (object));

			return true;
		}
			
		throw Error <INVALID_X3D> ("Expected object name.");
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
			if (not group -> children () .empty ())
			{
				group = X3DPtr <Transform> (scene -> createNode ("Transform"));

				scene -> addUninitializedNode (group);
			
				object -> children () .emplace_back (group);
			}

			if (not name .empty ())
				scene -> updateNamedNode (name, SFNode (group));

			return true;
		}
			
		throw Error <INVALID_X3D> ("Expected group name.");
	}

	return false;
}

bool
Parser::s ()
{
	//__LOG__ << this << " " << std::endl;

	comments ();
	
	int32_t shadingGroup;

	if (Grammar::s (istream))
	{
		whiteSpacesNoLineTerminator ();

		if (Grammar::off (istream))
		{
			creaseAngle = 0;
			return true;
		}

		if (Int32 (shadingGroup))
		{
			creaseAngle = M_PI;
			return true;
		}

		throw Error <INVALID_X3D> ("Expected shading group number of 'off'.");
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
		texCoord = X3DPtr <TextureCoordinate> (scene -> createNode ("TextureCoordinate"));

		scene -> addUninitializedNode (texCoord);
	
		while (vt ())
			;

		texCoordIndex += texCoord -> point () .size ();

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

		throw Error <INVALID_X3D> ("Expected texture coodinate.");
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
		normal = X3DPtr <Normal> (scene -> createNode ("Normal"));

		scene -> addUninitializedNode (normal);

		while (vn ())
			;

		normalIndex += normal -> vector () .size ();
		
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
		
		throw Error <INVALID_X3D> ("Expected normal vector.");
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
		coord = X3DPtr <Coordinate> (scene -> createNode ("Coordinate"));

		scene -> addUninitializedNode (coord);

		while (v ())
			;

		coordIndex += coord -> point () .size ();

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
		
		throw Error <INVALID_X3D> ("Expected vertex coordinate.");
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
		const auto appearance = X3DPtr <Appearance> (scene -> createNode ("Appearance"));

		geometry = X3DPtr <IndexedFaceSet> (scene -> createNode ("IndexedFaceSet"));
		shape    = X3DPtr <Shape> (scene -> createNode ("Shape"));

		scene -> addUninitializedNode (appearance);
		scene -> addUninitializedNode (geometry);
		scene -> addUninitializedNode (shape);

		appearance -> material ()  = material;
		geometry -> creaseAngle () = creaseAngle;
		shape -> appearance ()     = appearance;
		shape -> geometry ()       = geometry;

		group -> children () .emplace_back (shape);

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

		while (Index ())
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
Parser::Index ()
{
	//__LOG__ << this << " " << std::endl;
	
	int32_t value;

	if (Int32 (value))
	{
		geometry -> coordIndex () .emplace_back (getIndex (value, coordIndex, coord -> point () .size ()));
	
		if (Grammar::Slash (istream))
		{
			if (Int32 (value))
			{
				geometry -> texCoordIndex () .emplace_back (getIndex (value, texCoordIndex, texCoord -> point () .size ()));
			}

			if (Grammar::Slash (istream))
			{
				if (Int32 (value))
				{
					geometry -> normalIndex () .emplace_back (getIndex (value, normalIndex, normal -> vector () .size ()));
				}
			}
		}

		return true;
	}

	return false;
}

int32_t
Parser::getIndex (const int32_t index, const int32_t counter, const int32_t size)
{
	if (index == 0)
		throw Error <INVALID_X3D> ("Invalid index.");

	if (index < 0)
		return size - index;

	return index - counter + size;
}

bool
Parser::Int32 (int32_t & value)
{
	//__LOG__ << this << " " << std::endl;

	whiteSpacesNoLineTerminator ();

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Vec2f (Vector2f & value)
{
	//__LOG__ << this << " " << std::endl;

	whiteSpacesNoLineTerminator ();

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Vec3f (Vector3f & value)
{
	//__LOG__ << this << " " << std::endl;

	whiteSpacesNoLineTerminator ();

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
