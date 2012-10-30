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
 ******************************************************************************/

#include <Titania/X3D/Basic/Type.h>
#include <Titania/X3D/InputOutput/Encodings/ClassicVrml.h>
#include <Titania/X3D/LexicalElements/AccessType.h>

#include <cassert>

class Literals
{
public:

	static const Name initializeOnly;
	static const Name inputOnly;
	static const Name outputOnly;
	static const Name inputOutput;

};

const Name Literals::initializeOnly = { "initializeOnly", "field" };
const Name Literals::inputOnly = { "inputOnly",      "eventIn" };
const Name Literals::outputOnly = { "outputOnly",     "eventOut" };
const Name Literals::inputOutput = { "inputOutput",    "exposedField" };

static constexpr AccessType <Literals::initializeOnly> initializeOnly;
static constexpr AccessType <Literals::inputOnly>      inputOnly;
static constexpr AccessType <Literals::outputOnly>     outputOnly;
static constexpr AccessType <Literals::inputOutput>    inputOutput;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class X3DFieldDefinition
{ };

typedef std::vector <X3DFieldDefinition*> FieldDefinitionArray;

class NodeType :
	Id
{
	FieldDefinitionArray fieldDefinitions;

};

namespace titania {
namespace Test {

using namespace titania::X3D;

class StreamTest
{
public:

	StreamTest ()
	{
		space <char> ();
		space <wchar_t> ();
		facet <char> ();
		facet <wchar_t> ();
		boolean ();
		accessTypes ();
	}

	template <class CharT>
	void
	space ()
	{
		assert (std::isspace (static_cast <CharT> (' '), Encodings::Locale <CharT>::ClassicVrml));
		assert (std::isspace (static_cast <CharT> (','), Encodings::Locale <CharT>::ClassicVrml));
		assert (std::isspace (static_cast <CharT> ('\t'), Encodings::Locale <CharT>::ClassicVrml));
		assert (std::isspace (static_cast <CharT> ('\n'), Encodings::Locale <CharT>::ClassicVrml));
		assert (std::isspace (static_cast <CharT> ('\r'), Encodings::Locale <CharT>::ClassicVrml));
		assert (not std::isspace (static_cast <CharT> ('\f'), Encodings::Locale <CharT>::ClassicVrml));
		assert (not std::isspace (static_cast <CharT> ('\xff'), Encodings::Locale <CharT>::ClassicVrml));
		assert (not std::isspace (static_cast <CharT> (0x2018), Encodings::Locale <CharT>::ClassicVrml));
	}

	template <class CharT>
	void
	facet ()
	{
		std::string value = "TRUE TRUE 134.67 FALSE , \t \n \r , TRUE ,,  TRUE FALSErestÜTF-8 ";

		std::basic_istringstream <CharT> is (std::basic_string <CharT> (value .begin (), value .end ()));

		value = "FALSErestÜTF-8";
		std::basic_string <CharT> rest (std::basic_string <CharT> (value .begin (), value .end ()));

		is >> ClassicVrml;

		bool                      b = false;
		float                     f;
		std::basic_string <CharT> r;
		std::basic_string <CharT> c;

		assert (b == false);   is >> b; std::clog << b << std::endl;
		assert (b == true);    is >> b; std::clog << b << std::endl;
		assert (b == true);    is >> f; std::clog << f << std::endl;
		assert (f == 134.67f); is >> b; std::clog << b << std::endl;
		assert (b == false);   is >> b; std::clog << b << std::endl;
		assert (b == true);    is >> b; std::clog << b << std::endl;
		assert (b == true);    is >> r; //std::clog << r << std::endl;
		assert (r == rest);    is >> c;
		assert (c == std::basic_string <CharT> ());
	}

	void
	boolean ()
	{
		std::ostringstream ostrue;
		std::ostringstream osfalse;

		ostrue << ClassicVrml << true;
		osfalse << ClassicVrml << false;

		assert (ostrue  .str () == "TRUE");
		assert (osfalse .str () == "FALSE");
	}

	void
	accessTypes ()
	{
		assert (initializeOnly == AccessType <Literals::initializeOnly> ());

		assert (initializeOnly == initializeOnly);
		assert (not (initializeOnly == inputOnly));
		assert (not (initializeOnly == outputOnly));
		assert (not (initializeOnly == inputOutput));

		assert (not (initializeOnly not_eq initializeOnly));
		assert (initializeOnly not_eq inputOnly);
		assert (initializeOnly not_eq outputOnly);
		assert (initializeOnly not_eq inputOutput);

		assert (not (inputOnly == initializeOnly));
		assert (inputOnly == inputOnly);
		assert (not (inputOnly == outputOnly));
		assert (not (inputOnly == inputOutput));

		assert (inputOnly not_eq initializeOnly);
		assert (not (inputOnly not_eq inputOnly));
		assert (inputOnly not_eq outputOnly);
		assert (inputOnly not_eq inputOutput);

		assert (not (outputOnly == initializeOnly));
		assert (not (outputOnly == inputOnly));
		assert (outputOnly == outputOnly);
		assert (not (outputOnly == inputOutput));

		assert (outputOnly not_eq initializeOnly);
		assert (outputOnly not_eq inputOnly);
		assert (not (outputOnly not_eq outputOnly));
		assert (outputOnly not_eq inputOutput);

		vrml (initializeOnly);
		vrml (inputOnly);
		vrml (outputOnly);
		vrml (inputOutput);

		std::clog << std::endl;

		x3d (initializeOnly);
		x3d (inputOnly);
		x3d (outputOnly);
		x3d (inputOutput);

		std::clog << std::endl;

		vrml (initializeOnly);
		x3d (initializeOnly);
		vrml (inputOnly);
		x3d (inputOnly);
		vrml (outputOnly);
		x3d (outputOnly);
		vrml (inputOutput);
		x3d (inputOutput);

		std::clog << std::endl;
	}

	void
	vrml (const Id & accessType)
	{
		//std::clog << vrml2 () << accessType << std::endl;
		std::clog << ClassicVrml << accessType << std::endl;
	}

	void
	x3d (const Id & accessType)
	{
		std::clog << accessType << std::endl;
	}

};

}
}
