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
	           url_stream (nullptr),
	              istream (nullptr),
	 file_request_headers (),
	file_response_headers (),
	                m_url (),
	             m_status ()
{ }

ifilestream::ifilestream (const basic::uri & url, size_t timeout) :
	ifilestream ()
{
	open (url, timeout);
}

ifilestream::ifilestream (ifilestream && other) :
	ifilestream ()
{
	*this = std::move (other);
}

ifilestream::ifilestream (std::istream && other) :
	ifilestream ()
{
	*this = std::move (other);
}

ifilestream &
ifilestream::operator = (ifilestream && other)
{
	close ();

	istream               = other .istream;
	file_request_headers  = std::move (other .file_request_headers);
	file_response_headers = std::move (other .file_response_headers);
	m_url                 = std::move (other .m_url);

	other .data_istream = nullptr;
	other .url_stream   = nullptr;
	other .istream      = nullptr;

	rdbuf (istream -> rdbuf ());
	clear (other .rdstate ());

	other .rdbuf (nullptr);
	other .clear (std::ios::badbit);

	return *this;
}

// Does not realy work with std::istream, thus std::istream is not movable

ifilestream &
ifilestream::operator = (std::istream && other)
{
	close ();

	istream = new std::istream (other .rdbuf ());
	m_url   = "";

	rdbuf (istream -> rdbuf ());
	clear (other .rdstate ());

	other .rdbuf (nullptr);
	other .clear (std::ios::badbit);

	guess_content_type (istream);

	return *this;
}

// Connection handling

void
ifilestream::open (const basic::uri & URL, size_t timeout)
{
	url (URL);

	close ();

	if (url () .scheme () == "data")
	{
		// data:[<MIME-Typ>][;charset="<Zeichensatz>"][;base64],<Daten>

		std::string::size_type first  = std::string::npos;
		std::string::size_type length = 0;
		std::string::size_type comma  = url () .path () .find (',');
	
		if (comma not_eq std::string::npos)
		{
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

			first  = comma + 1;
			length = url () .path () .size () - first;

			// header

			file_response_headers .insert (std::make_pair ("Content-Type",   content_type));
			file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (length)));
		
			istream = data_istream = new std::istringstream (base64
			                                                 ? base64_decode (url () .path () .substr (first))
																			 : Glib::uri_unescape_string (url () .path () .substr (first)));
		}
		else
			istream = data_istream = new std::istringstream ();
	}
	else
	{
		istream = url_stream = new iurlstream ();
		url_stream -> open (url (), timeout);
	}

	rdbuf (istream -> rdbuf ());
	clear (istream -> rdstate ());
}

void
ifilestream::send ()
{
	if (url_stream and * url_stream)
	{
		url_stream -> send ();
		url (url_stream -> url ());
		status (url_stream -> status ());
	}
//	else if (file_istream)
//	{
//		guess_content_type (file_istream);
//
//		file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (os::file_size (url () .path ()))));
//	}

	setstate (istream -> rdstate ());
}

void
ifilestream::guess_content_type (std::istream* istream)
{
	static constexpr size_t BUFFER_SIZE = 128;

	auto state = istream -> rdstate ();
	auto pos   = istream -> tellg ();

	// Guess content type.

	char   data [BUFFER_SIZE];
	size_t data_size = istream -> rdbuf () -> sgetn (data, BUFFER_SIZE);

	bool        result_uncertain;
	std::string content_type = Gio::content_type_guess (url () .path (), (guchar*) data, data_size, result_uncertain);

	file_response_headers .insert (std::make_pair ("Content-Type", content_type));

	// Reset stream.

	istream -> clear (state);
	istream -> seekg (pos - istream -> tellg (), std::ios_base::cur);
}

void
ifilestream::close ()
{
	url_stream   = nullptr;
	data_istream = nullptr;

	if (istream)
	{
		istream -> rdbuf (rdbuf ());
		istream -> clear (std::ios::badbit);
		delete istream;
		istream = nullptr;
	}

	rdbuf (nullptr);
	clear (std::ios::badbit);
}

void
ifilestream::request_header (const std::string & header, const std::string & value)
{
	if (url_stream)
		return url_stream -> request_header (header, value);

	file_request_headers .insert (std::make_pair (header, value));
}

const ifilestream::headers_type &
ifilestream::request_headers () const
{
	if (url_stream)
		return url_stream -> request_headers ();

	return file_request_headers;
}

const ifilestream::headers_type &
ifilestream::response_headers () const
{
	if (url_stream)
		return url_stream -> response_headers ();

	return file_response_headers;
}

// SocketStreamBuffer

const std::string &
ifilestream::http_version () const
{
	if (url_stream)
		return url_stream -> http_version ();

	return empty_string;
}

const std::string &
ifilestream::reason () const
{
	if (url_stream)
		return url_stream -> reason ();

	return *istream ? reasons [0] : reasons [1];
}

size_t
ifilestream::timeout () const
{
	if (url_stream)
		return url_stream -> timeout ();

	return 0;
}

void
ifilestream::timeout (size_t value)
{
	if (url_stream)
		url_stream -> timeout (value);
}

ifilestream::~ifilestream ()
{
	close ();
}

} // basic
} // titania
