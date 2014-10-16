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
	static const io::single_line_comment Comment;

	///  @name Keywords
	static const io::string         mtllib;
	static const io::string         usemtl;
	static const io::inverse_string string;
	static const io::character      o;
	static const io::character      v;
	static const io::string         vt;
	static const io::string         vn;
	static const io::character      g;
	static const io::character      s;
	static const io::string         off;
	static const io::character      f;

};

const io::sequence            Grammar::WhiteSpaces ("\r\n \t");
const io::single_line_comment Grammar::Comment ("#");

const io::string         Grammar::mtllib ("mtllib");
const io::string         Grammar::usemtl ("usemtl");
const io::inverse_string Grammar::string ("\n");
const io::character      Grammar::o ('o');
const io::character      Grammar::v ('v');
const io::string         Grammar::vt ("vt");
const io::string         Grammar::vn ("vn");
const io::character      Grammar::g ('g');
const io::character      Grammar::s ('s');
const io::string         Grammar::off ("off");
const io::character      Grammar::f ('f');

Parser::Parser (const X3DScenePtr & scene, const basic::uri & uri, std::istream & istream) :
	               scene (scene),
	                 uri (uri),
	             istream (istream),
	          lineNumber (0),
	whiteSpaceCharacters (),
	   commentCharacters (),
	     currentComments ()
{ }

void
Parser::parseIntoScene ()
{
	//__LOG__ << this << " " << std::endl;

	statements ();
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

	Grammar::WhiteSpaces (istream, whiteSpaceCharacters);

	lines (whiteSpaceCharacters);

	whiteSpaceCharacters .clear ();

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
Parser::lines (const std::string & string)
{
	//__LOG__ << this << " " << std::endl;

	lineNumber += std::count (string .begin (), string .end (), '\n');
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
	

	return false;
}

bool
Parser::mtllib ()
{
	//__LOG__ << this << " " << std::endl;
	
	comments ();
	
	if (Grammar::mtllib (istream))
	{
		comments ();
		
		std::string uri;

		if (Grammar::string (istream, uri))
		{
			return true;
		}
	}

	return false;
}

bool
Parser::usemtl ()
{
	//__LOG__ << this << " " << std::endl;
	
	comments ();
	
	if (Grammar::mtllib (istream))
	{
		comments ();
		
		std::string nameCharacters;

		if (Grammar::string (istream, nameCharacters))
		{
			return true;
		}
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
		comments ();
		
		std::string nameCharacters;

		if (Grammar::string (istream, nameCharacters))
		{
			return true;
		}
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
		comments ();

		std::string nameCharacters;

		if (Grammar::string (istream, nameCharacters))
		{
			return true;
		}
	}

	return false;
}

bool
Parser::s (int32_t & shadingGroup)
{
	//__LOG__ << this << " " << std::endl;
	
	comments ();

	if (Grammar::s (istream))
	{
		comments ();

		if (Grammar::off (istream))
		{
			shadingGroup = 0;
			return true;
		}

		if (Int32 (shadingGroup))
			return true;
	}

	return false;
}

bool
Parser::Int32 (int32_t & value)
{
	//__LOG__ << this << " " << std::endl;
	
	comments ();

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

bool
Parser::Float (float & value)
{
	//__LOG__ << this << " " << std::endl;
	
	comments ();

	if (istream >> value)
		return true;

	istream .clear ();

	return false;
}

} // Wavefront
} // X3D
} // titania
