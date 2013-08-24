// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "InputFileStream.h"

#include <Titania/LOG.h>
#include <Titania/OS/FileSize.h>
#include <Titania/OS/IsFile.h>

#include <glibmm/main.h>

#include <giomm.h>
#include <iostream>
#include <utility>

namespace titania {
namespace basic {

const std::string ifilestream::reasons [2] = { "OK", "File not found." };
const std::string ifilestream::empty_string;

static
std::string
base64_decode (const std::string & string)
{
	gsize out_len = 0;
	char* decoded = (char*) g_base64_decode (string .c_str (), &out_len);

	std::string decoded_string = std::string (decoded, out_len);

	g_free (decoded);

	return decoded_string;
}

ifilestream::ifilestream () :
	         std::istream (),
	         data_istream (nullptr),
	         file_istream (nullptr),
	         http_istream (nullptr),
	              istream (nullptr),
	 file_request_headers (),
	file_response_headers (),
	                m_url (),
	             m_status ()
{ }

ifilestream::ifilestream (const http::method method, const basic::uri & url, size_t timeout) :
	          ifilestream ()
{
	open (method, url, timeout);
}

ifilestream::ifilestream (ifilestream && other) :
	          ifilestream ()
{
	*this = std::move (other);
}

ifilestream &
ifilestream::operator = (ifilestream && other)
{
	data_istream          = other .data_istream;
	file_istream          = other .file_istream;
	http_istream          = other .http_istream;
	istream               = other .istream;
	file_request_headers  = std::move (other .file_request_headers);
	file_response_headers = std::move (other .file_response_headers);
	m_url                 = std::move (other .m_url);

	other .data_istream = nullptr;
	other .file_istream = nullptr;
	other .http_istream = nullptr;
	other .istream      = nullptr;

	init (istream -> rdbuf ());

	clear (other .rdstate ());
	other .clear (std::ios::badbit);

	return *this;
}

// Connection handling

void
ifilestream::open (const http::method method, const basic::uri & URL, size_t timeout)
{
	url (URL);

	close ();

	if (url () .scheme () == "data")
	{
		// data:[<MIME-Typ>][;charset="<Zeichensatz>"][;base64],<Daten>

		std::string::size_type first  = std::string::npos;
		std::string::size_type length = 0;
		std::string::size_type comma  = url () .path () .find (',');

		std::string header = url () .path () .substr (0, comma);
		std::string content_type;
		bool        base64 = false;

		if (header .size () >= 6)
		{
			if (header .substr (header .size () - 6, 6) == "base64")
			{
				base64 = true;
				header .resize (header .size () - 6);

				if (header .size () and header [header .size () - 1] == ';')
					header .resize (header .size () - 1);
			}
		}

		content_type = header;

		// data

		if (comma not_eq std::string::npos)
		{
			first  = 5 + comma + 1;
			length = url () .path () .size () - first;
		}

		// header

		file_response_headers .insert (std::make_pair ("Content-Type",   content_type));
		file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (length)));

		istream = data_istream = new std::istringstream (base64
		                                                 ? base64_decode (url () .str () .substr (first))
																		 : Glib::uri_unescape_string (url () .str () .substr (first)));
	}
	else if (url () .is_local ())
	{
		istream = file_istream = new std::ifstream ();

		if (os::is_file (url () .path ()))
			file_istream -> open (url () .path ());

		else
		{
			status (404);
			setstate (std::ios::failbit);
		}
	}
	else
	{
		istream = http_istream = new ihttpstream ();
		http_istream -> open (method, url (), timeout);
	}

	init (istream -> rdbuf ());
	setstate (istream -> rdstate ());
}

void
ifilestream::send ()
{
	if (http_istream and * http_istream)
	{
		http_istream -> send ();
		url (http_istream -> url ());
		status (http_istream -> status ());
	}
	else if (file_istream)
	{
		// Guess content type

		char   data [64];
		size_t data_size = file_istream -> rdbuf () -> sgetn (data, 64);

		bool        result_uncertain;
		std::string content_type = Gio::content_type_guess (url () .path (), (guchar*) data, data_size, result_uncertain);

		file_response_headers .insert (std::make_pair ("Content-Type",   content_type));
		file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (os::file_size (url () .path ()))));

		// Reset stream

		file_istream -> seekg (0, std::ios_base::beg);
	}

	setstate (istream -> rdstate ());
}

void
ifilestream::close ()
{
	if (http_istream)
	{
		http_istream = nullptr;
	}
	else if (file_istream)
	{
		file_istream = nullptr;
	}
	else if (data_istream)
	{
		data_istream = nullptr;
	}

	delete istream;
	clear (std::ios::badbit);
}

void
ifilestream::request_header (const std::string & header, const std::string & value)
{
	if (http_istream)
		return http_istream -> request_header (header, value);

	file_request_headers .insert (std::make_pair (header, value));
}

const ifilestream::headers_type &
ifilestream::request_headers () const
{
	if (http_istream)
		return http_istream -> request_headers ();

	return file_request_headers;
}

const ifilestream::headers_type &
ifilestream::response_headers () const
{
	if (http_istream)
		return http_istream -> response_headers ();

	return file_response_headers;
}

// SocketStreamBuffer

const std::string &
ifilestream::http_version () const
{
	if (http_istream)
		return http_istream -> http_version ();

	return empty_string;
}

const std::string &
ifilestream::reason () const
{
	if (http_istream)
		return http_istream -> reason ();

	return *istream ? reasons [0] : reasons [1];
}

size_t
ifilestream::timeout () const
{
	if (http_istream)
		return http_istream -> timeout ();

	return 0;
}

void
ifilestream::timeout (size_t value)
{
	if (http_istream)
		http_istream -> timeout (value);
}

// Buffer

std::streambuf*
ifilestream::rdbuf () const
{
	if (istream)
		return istream -> rdbuf ();

	return nullptr;
}

ifilestream::~ifilestream ()
{
	close ();
}

} // basic
} // titania
