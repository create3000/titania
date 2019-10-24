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

#include "NRRDParser.h"

#include <Titania/InputOutput.h>
#include <Titania/String/tolower.h>
#include <Titania/String/split.h>
#include <Titania/String/trim.h>

#include <functional>
#include <map>
#include <regex>

namespace titania {
namespace X3D {

class NRRDParser::Grammar
{
public:

	static const io::string         NRRD;
	static const io::inverse_string line;

};

const io::string NRRDParser::Grammar::NRRD ("NRRD");
const io::inverse_string NRRDParser::Grammar::line ("\n");

NRRDParser::NRRDParser (const std::string & data) :
	     data (data),
	  istream (data),
	     nrrd ({ false, true, "", 0, 0, 0, 0, 0, 0, "" }),
	bytesType (ByteType::BYTE),
	    bytes ()
{ }

NRRDImage
NRRDParser::parse ()
{
	NRRD ();

	if (nrrd .nrrd)
	{
		fields ();
		pixels ();
	}

	return nrrd;
}

void
NRRDParser::NRRD ()
{
	if (Grammar::NRRD (istream))
	{
		nrrd .nrrd = true;
		istream >> nrrd .version;
		std::istream::sentry se (istream);
		return;
	}

	nrrd .nrrd  = false;
	nrrd .error = "Invalid NRRD file.";
}

void
NRRDParser::fields ()
{
	using FieldFunction = std::function <void (NRRDParser*, const std::string &)>;

	static const std::map <std::string, FieldFunction> fieldFunctions = {
		std::pair ("type",      std::mem_fn (&NRRDParser::type)),
		std::pair ("encoding",  std::mem_fn (&NRRDParser::encoding)),
		std::pair ("dimension", std::mem_fn (&NRRDParser::dimension)),
		std::pair ("sizes",     std::mem_fn (&NRRDParser::sizes)),
	};

	std::string line;

	while (Grammar::line (istream, line))
	{
		if (line .empty ())
			break;

		std::vector <std::string> field;

		basic::split (std::back_inserter (field), line, ":");

		if (field .size () == 2)
		{
			const auto key   = basic::tolower (field [0], std::locale::classic ());
			const auto value = basic::tolower (basic::trim (field [1]), std::locale::classic ());
			const auto iter  = fieldFunctions .find (key);

			if (iter not_eq fieldFunctions .end ())
				iter -> second (this, value);
		}

		line .clear ();
	}
}

void
NRRDParser::type (const std::string & value)
{
	std::map <std::string, std::pair <ByteType, size_t>> types = {
		std::pair ("signed char",        std::pair (ByteType::BYTE, 1)),
		std::pair ("int8",               std::pair (ByteType::BYTE, 1)),
		std::pair ("int8_t",             std::pair (ByteType::BYTE, 1)),
		std::pair ("uchar",              std::pair (ByteType::BYTE, 1)),
		std::pair ("unsigned char",      std::pair (ByteType::BYTE, 1)),
		std::pair ("uint8",              std::pair (ByteType::BYTE, 1)),
		std::pair ("uint8_t",            std::pair (ByteType::BYTE, 1)),
		std::pair ("short",              std::pair (ByteType::SHORT, 2)),
		std::pair ("short int",          std::pair (ByteType::SHORT, 2)),
		std::pair ("signed short",       std::pair (ByteType::SHORT, 2)),
		std::pair ("signed short int",   std::pair (ByteType::SHORT, 2)),
		std::pair ("int16",              std::pair (ByteType::SHORT, 2)),
		std::pair ("int16_t",            std::pair (ByteType::SHORT, 2)),
		std::pair ("ushort",             std::pair (ByteType::SHORT, 2)),
		std::pair ("unsigned short",     std::pair (ByteType::SHORT, 2)),
		std::pair ("unsigned short int", std::pair (ByteType::SHORT, 2)),
		std::pair ("uint16",             std::pair (ByteType::SHORT, 2)),
		std::pair ("uint16_t",           std::pair (ByteType::SHORT, 2)),
		std::pair ("int",                std::pair (ByteType::INT, 4)),
		std::pair ("signed int",         std::pair (ByteType::INT, 4)),
		std::pair ("int32",              std::pair (ByteType::INT, 4)),
		std::pair ("int32_t",            std::pair (ByteType::INT, 4)),
		std::pair ("uint",               std::pair (ByteType::INT, 4)),
		std::pair ("unsigned int",       std::pair (ByteType::INT, 4)),
		std::pair ("uint32",             std::pair (ByteType::INT, 4)),
		std::pair ("uint32_t",           std::pair (ByteType::INT, 4)),
		std::pair ("float",              std::pair (ByteType::FLOAT, 4)),
		std::pair ("double",             std::pair (ByteType::DOUBLE, 8)),
	};

	const auto iter = types .find (value);

	if (iter not_eq types .end ())
	{
		bytesType = iter -> second .first;
		bytes     = iter -> second .second;
		return;
	}

	nrrd .valid = false;
	nrrd .error = "Unsupported NRRD type";
}

void
NRRDParser::encoding (const std::string & value)
{
	if (value == "raw")
	{
		return;
	}

	nrrd .valid = false;
	nrrd .error = "Unsupported NRRD encoding";
}

void
NRRDParser::dimension (const std::string & value)
{
	std::istringstream istream (value);

	istream >> nrrd .dimension;

	if (istream and (nrrd .dimension == 3 or nrrd .dimension == 4))
		return;

	nrrd .valid = false;
	nrrd .error = "Unsupported NRRD dimension, must be 3";
}

void
NRRDParser::sizes (const std::string & value)
{
	std::istringstream istream (value);
	std::vector <double> sizes;
	double size = 0;

	while (istream >> size)
		sizes .emplace_back (size);

	switch (sizes .size ())
	{
		case 3:
		{
			nrrd .components = 1;
			nrrd .width      = sizes [0];
			nrrd .height     = sizes [1];
			nrrd .depth      = sizes [2];
			return;
		}
		case 4:
		{
			nrrd .components = sizes [0];
			nrrd .width      = sizes [1];
			nrrd .height     = sizes [2];
			nrrd .depth      = sizes [3];
			return;
		}
	}
}

void
NRRDParser::pixels ()
{
	const auto size = nrrd .components * nrrd .width * nrrd .height * nrrd .depth * bytes;

	if (size > 0)
	{
		switch (bytesType)
		{
			case ByteType::BYTE:
			{
				nrrd .pixels = data .substr (data .size () - size);
				return;
			}
			case ByteType::SHORT:
			{
				const auto pixels = data .substr (data .size () - size);

				for (size_t i = 0, size = pixels .size (); i < size; i += 2)
					nrrd .pixels .push_back ((pixels [i] << 8 | pixels [i + 1]) / 256);

				return;
			}
			case ByteType::INT:
			{
				const auto pixels = data .substr (data .size () - size);

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
					nrrd .pixels .push_back ((pixels [i] << 24 | pixels [i + 1] << 16 | pixels [i + 2] << 8 | pixels [i + 3]) / 16'777'216);

				return;
			}
			case ByteType::FLOAT:
			{
				union Value
				{
					uint8_t bytes [4];
					float number;
				};

				const auto pixels = data .substr (data .size () - size);
				Value value;

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
				{
					value .bytes [0] = pixels [i];
					value .bytes [1] = pixels [i + 1];
					value .bytes [2] = pixels [i + 2];
					value .bytes [3] = pixels [i + 3];

					nrrd .pixels .push_back (value .number * 255);
				}

				return;
			}
			case ByteType::DOUBLE:
			{
				union Value
				{
					uint8_t bytes [8];
					double number;
				};

				const auto pixels = data .substr (data .size () - size);
				Value value;

				for (size_t i = 0, size = pixels .size (); i < size; i += 8)
				{
					value .bytes [0] = pixels [i];
					value .bytes [1] = pixels [i + 1];
					value .bytes [2] = pixels [i + 2];
					value .bytes [3] = pixels [i + 3];
					value .bytes [4] = pixels [i + 4];
					value .bytes [5] = pixels [i + 5];
					value .bytes [6] = pixels [i + 6];
					value .bytes [7] = pixels [i + 7];

					nrrd .pixels .push_back (value .number * 255);
				}

				return;
			}
		}
	}

	nrrd .valid = false;
	nrrd .error = "Invalid NRRD sizes";
}

NRRDParser::~NRRDParser ()
{ }

} // X3D
} // titania
