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
#include <Titania/Stream/GZFilterBuf.h>

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
	    m_bytes (),
	   m_endian (endianess ())
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
		std::pair ("endian",    std::mem_fn (&NRRDParser::endian)),
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
	std::map <std::string, EncodingType> encodings = {
		std::pair ("ascii", EncodingType::ASCII),
		std::pair ("txt",   EncodingType::ASCII),
		std::pair ("text",  EncodingType::ASCII),
		std::pair ("raw",   EncodingType::RAW),
		std::pair ("hex",   EncodingType::HEX),
		std::pair ("gz",    EncodingType::GZIP),
		std::pair ("gzip",  EncodingType::GZIP),
	};

	const auto iter = encodings .find (value);

	if (iter not_eq encodings .end ())
	{
		m_encoding = iter -> second;
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

	if (istream)
	{
		switch (m_nrrd .dimension)
		{
			case 1:
			case 2:
			case 3:
			case 4:
				return;
		}
	}

	m_nrrd .valid = false;
	m_nrrd .error = "Unsupported NRRD dimension, must be 1, 2, 3, or 4";
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
		case 1:
		{
			m_nrrd .components = 1;
			m_nrrd .width      = sizes [0];
			m_nrrd .height     = 1;
			m_nrrd .depth      = 1;
			return;
		}
		case 2:
		{
			m_nrrd .components = 1;
			m_nrrd .width      = sizes [0];
			m_nrrd .height     = sizes [1];
			m_nrrd .depth      = 1;
			return;
		}
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
NRRDParser::endian (const std::string & value)
{
	if (value == "little")
	{
		m_endian = EndianType::LITTLE;
		return;
	}

	if (value == "big")
	{
		m_endian = EndianType::BIG;
		return;
	}

	m_nrrd .valid = false;
	m_nrrd .error = "Unsupported NRRD endian, must be 'little' or 'big'";
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
			raw (m_data);
			break;
		case EncodingType::HEX:
			hex ();
			break;
		case EncodingType::GZIP:
			gzip ();
			break;
	}
}

void
NRRDParser::ascii ()
{
	const auto dataSize  = m_nrrd .components * m_nrrd .width * m_nrrd .height * m_nrrd .depth;
	auto &     data      = m_nrrd .data;

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
				data .push_back (number / 256);
			}

			break;
		}
		case ByteType::DOUBLE:
		{
			double number;

			while (m_istream >> number)
			{
				data .push_back (number / 16'777'216);
			}

			break;
		}
	}

	data .resize (dataSize);
}

void
NRRDParser::raw (const std::string & p_data)
{
	const auto dataSize = m_nrrd .components * m_nrrd .width * m_nrrd .height * m_nrrd .depth;
	const auto size     = dataSize * m_bytes;
	auto &     data     = m_nrrd .data;

	size_t e0, e1, e2, e3, e4, e5, e6, e7;

	if (size > 0)
	{
		switch (m_bytesType)
		{
			case ByteType::BYTE:
			{
				data = p_data .substr (p_data .size () - size);
				break;
			}
			case ByteType::SHORT:
			{
				const auto pixels = p_data .substr (p_data .size () - size);

				if (endianess () == m_endian)
				{
					e0 = 0; e1 = 1;
				}
				else
				{
					e0 = 1; e1 = 0;
				}

				for (size_t i = 0, size = pixels .size (); i < size; i += 2)
					data .push_back ((pixels [i + e0] << 8 | pixels [i + e1]) / 256);

				break;
			}
			case ByteType::INT:
			{
				const auto pixels = p_data .substr (p_data .size () - size);

				if (endianess () == m_endian)
				{
					e0 = 0; e1 = 1; e2 = 2; e3 = 3;
				}
				else
				{
					e0 = 4; e1 = 2; e2 = 1; e3 = 0;
				}

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
					data .push_back ((pixels [i + e0] << 24 | pixels [i + e1] << 16 | pixels [i + e2] << 8 | pixels [i + e3]) / 16'777'216);

				break;
			}
			case ByteType::FLOAT:
			{
				union Value
				{
					uint8_t bytes [4];
					float number;
				};

				const auto pixels = p_data .substr (p_data .size () - size);
				Value value;

				if (endianess () == m_endian)
				{
					e0 = 0; e1 = 1; e2 = 2; e3 = 3;
				}
				else
				{
					e0 = 4; e1 = 2; e2 = 1; e3 = 0;
				}

				for (size_t i = 0, size = pixels .size (); i < size; i += 4)
				{
					value .bytes [0] = pixels [i + e0];
					value .bytes [1] = pixels [i + e1];
					value .bytes [2] = pixels [i + e2];
					value .bytes [3] = pixels [i + e3];

					data .push_back (value .number / 256);
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

				const auto pixels = p_data .substr (p_data .size () - size);
				Value value;

				if (endianess () == m_endian)
				{
					e0 = 0; e1 = 1; e2 = 2; e3 = 3; e4 = 4; e5 = 5; e6 = 6; e7 = 7;
				}
				else
				{
					e0 = 7; e1 = 6; e2 = 5; e3 = 4; e4 = 3; e5 = 2; e6 = 1; e7 = 0;
				}

				for (size_t i = 0, size = pixels .size (); i < size; i += 8)
				{
					value .bytes [0] = pixels [i + e0];
					value .bytes [1] = pixels [i + e1];
					value .bytes [2] = pixels [i + e2];
					value .bytes [3] = pixels [i + e3];
					value .bytes [4] = pixels [i + e4];
					value .bytes [5] = pixels [i + e5];
					value .bytes [6] = pixels [i + e6];
					value .bytes [7] = pixels [i + e7];

					data .push_back (value .number / 16'777'216);
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

void
NRRDParser::hex ()
{
	std::string string;

	while (m_istream)
	{
		char c1, c2;

		if (m_istream >> c1)
		{
			if (m_istream >> c2)
			{
				string .push_back (hexChar (c1) << 4 | hexChar (c2));
			}
		}
	}

	raw (string);
}

uint8_t
NRRDParser::hexChar (const char c) const
{
	switch (c)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 0xa;
		case 'b': return 0xb;
		case 'c': return 0xc;
		case 'd': return 0xd;
		case 'e': return 0xe;
		case 'f': return 0xf;
		case 'A': return 0xa;
		case 'B': return 0xb;
		case 'C': return 0xc;
		case 'D': return 0xd;
		case 'E': return 0xe;
		case 'F': return 0xf;
		default: return 0;
	}
}

void
NRRDParser::gzip ()
{
	basic::gzfilterbuf gzip (m_istream .rdbuf ());

	std::ostringstream osstream;

	osstream << &gzip;

	raw (osstream .str ());
}

NRRDParser::EndianType
NRRDParser::endianess () const
{
	union Endian {
		int32_t m_int;
		uint8_t m_bytes [4];
	};

	Endian number;

	number .m_int = 0x01020304;

	if (number .m_bytes [0] == 1 and number .m_bytes [1] == 2 and number .m_bytes [2] == 3 and number .m_bytes [3] == 4)
		return EndianType::BIG;

	if (number .m_bytes [0] == 4 and number .m_bytes [1] == 3 and number .m_bytes [2] == 2 and number .m_bytes [3] == 1)
		return EndianType::LITTLE;

	return m_endian;
}

NRRDParser::~NRRDParser ()
{ }

} // X3D
} // titania
