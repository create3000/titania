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
	     m_data (data),
	  m_istream (data),
	     m_nrrd ({ false, true, "", 0, 0, 0, 0, 0, 0, "" }),
	 m_encoding (EncodingType::ASCII),
	m_bytesType (ByteType::BYTE),
	    m_bytes ()
{ }

NRRDImage
NRRDParser::parse ()
{
	NRRD ();

	if (m_nrrd .nrrd)
	{
		fields ();
		data ();
	}

	return m_nrrd;
}

void
NRRDParser::NRRD ()
{
	if (Grammar::NRRD (m_istream))
	{
		m_nrrd .nrrd = true;
		m_istream >> m_nrrd .version;
		std::istream::sentry se (m_istream);
		return;
	}

	m_nrrd .nrrd  = false;
	m_nrrd .error = "Invalid NRRD file.";
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

	while (Grammar::line (m_istream, line))
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
		m_bytesType = iter -> second .first;
		m_bytes     = iter -> second .second;
		return;
	}

	m_nrrd .valid = false;
	m_nrrd .error = "Unsupported NRRD type";
}

void
NRRDParser::encoding (const std::string & value)
{
	if (value == "raw")
	{
		m_encoding = EncodingType::RAW;
		return;
	}

	if (value == "ascii")
	{
		m_encoding = EncodingType::ASCII;
		return;
	}

	m_nrrd .valid = false;
	m_nrrd .error = "Unsupported NRRD encoding";
}

void
NRRDParser::dimension (const std::string & value)
{
	std::istringstream istream (value);

	istream >> m_nrrd .dimension;

	if (istream and (m_nrrd .dimension == 3 or m_nrrd .dimension == 4))
		return;

	m_nrrd .valid = false;
	m_nrrd .error = "Unsupported NRRD dimension, must be 3";
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
			m_nrrd .components = 1;
			m_nrrd .width      = sizes [0];
			m_nrrd .height     = sizes [1];
			m_nrrd .depth      = sizes [2];
			return;
		}
		case 4:
		{
			m_nrrd .components = sizes [0];
			m_nrrd .width      = sizes [1];
			m_nrrd .height     = sizes [2];
			m_nrrd .depth      = sizes [3];
			return;
		}
	}
}

void
NRRDParser::data ()
{
	switch (m_encoding)
	{
		case EncodingType::ASCII:
			ascii ();
			break;
		case EncodingType::RAW:
			raw ();
			break;
	}
}

void
NRRDParser::ascii ()
{
	const auto dataSize = m_nrrd .components * m_nrrd .width * m_nrrd .height * m_nrrd .depth;
	auto &     data     = m_nrrd .data;

	switch (m_bytesType)
	{
		case ByteType::BYTE:
		{
			ssize_t number;

			while (m_istream >> number)
			{
				data .push_back (number);
			}

			break;
		}
		case ByteType::SHORT:
		{
			ssize_t number;

			while (m_istream >> number)
			{
				data .push_back (number / 256);
			}

			break;
		}
		case ByteType::INT:
		{
			ssize_t number;

			while (m_istream >> number)
			{
				data .push_back (number / 16'777'216);
			}

			break;
		}
		case ByteType::FLOAT:
		{
			float number;

			while (m_istream >> number)
			{
				data .push_back (number * 255);
			}

			break;
		}
		case ByteType::DOUBLE:
		{
			double number;

			while (m_istream >> number)
			{
				data .push_back (number * 255);
			}

			break;
		}
	}

	data .resize (dataSize);
}

void
NRRDParser::raw ()
{
	const auto dataSize = m_nrrd .components * m_nrrd .width * m_nrrd .height * m_nrrd .depth;
	const auto size     = dataSize * m_bytes;
	auto &     data     = m_nrrd .data;

	if (size > 0)
	{
		switch (m_bytesType)
		{
			case ByteType::BYTE:
			{
				data = m_data .substr (m_data .size () - size);
				break;
			}
			case ByteType::SHORT:
			{
				const auto pixels = m_data .substr (m_data .size () - size);

				for (size_t i = 0, size = pixels .size (); i < size; i += 2)
					data .push_back ((pixels [i] << 8 | pixels [i + 1]) / 256);

				break;
			}
			case ByteType::INT:
			{
				const auto pixels = m_data .substr (m_data .size () - size);

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
					data .push_back ((pixels [i] << 24 | pixels [i + 1] << 16 | pixels [i + 2] << 8 | pixels [i + 3]) / 16'777'216);

				break;
			}
			case ByteType::FLOAT:
			{
				union Value
				{
					uint8_t bytes [4];
					float number;
				};

				const auto pixels = m_data .substr (m_data .size () - size);
				Value value;

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
				{
					value .bytes [0] = pixels [i];
					value .bytes [1] = pixels [i + 1];
					value .bytes [2] = pixels [i + 2];
					value .bytes [3] = pixels [i + 3];

					data .push_back (value .number * 255);
				}

				break;
			}
			case ByteType::DOUBLE:
			{
				union Value
				{
					uint8_t bytes [8];
					double number;
				};

				const auto pixels = m_data .substr (m_data .size () - size);
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

					data .push_back (value .number * 255);
				}

				break;
			}
		}

		data .resize (dataSize);
		return;
	}

	m_nrrd .valid = false;
	m_nrrd .error = "Invalid NRRD sizes";
}

NRRDParser::~NRRDParser ()
{ }

} // X3D
} // titania
