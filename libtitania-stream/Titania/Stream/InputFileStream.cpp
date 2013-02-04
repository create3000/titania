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

#include <Titania/OS/IsFile.h>
#include <iostream>
#include <utility>

namespace titania {
namespace basic {

const ifilestream::headers_type ifilestream::file_response_headers;
const std::string               ifilestream::reasons [2] = { "OK", "FAILED" };
const std::string               ifilestream::empty_string;

ifilestream::ifilestream (const http::method method, const basic::uri & url) :
	        std::istream (),     
	        file_istream (NULL), 
	        http_istream (NULL), 
	             istream (NULL), 
	file_request_headers ()      
{
	open (method, url);
}

ifilestream::ifilestream (ifilestream && other) :
	        std::istream (),     
	        file_istream (other .file_istream),
	        http_istream (other .http_istream),
	             istream (other .istream),
	file_request_headers (std::move (other .file_request_headers))
{
	other .file_istream = NULL;
	other .http_istream = NULL;
	other .istream      = NULL;

	init (istream -> rdbuf ());

	clear (other .rdstate ());
	other .clear (std::ios::badbit);
}

// Connection handling

void
ifilestream::open (const http::method method, const basic::uri & url)
{
	close ();

	if (url .is_local ())
	{
		istream = file_istream = new std::ifstream ();

		if (os::is_file (url .path ()))
			file_istream -> open (url .path ());

		else
			file_istream -> setstate (std::ios::failbit);
	}
	else
	{
		istream = http_istream = new ihttpstream ();
		http_istream -> open (method, url);
	}

	init (istream -> rdbuf ());
	clear (istream -> rdstate ());
}

void
ifilestream::send ()
{
	if (http_istream)
		http_istream -> send ();
}

void
ifilestream::close ()
{
	if (http_istream)
	{
		http_istream -> close ();
		http_istream = NULL;
	}
	else if (file_istream)
	{
		file_istream -> close ();
		file_istream = NULL;

		file_request_headers .clear ();
	}

	delete istream;
	clear (std::ios::badbit);
}

void
ifilestream::request_header (const std::string & header, const std::string & value)
{
	if (http_istream)
		return http_istream -> request_header (header, value);

	file_request_headers .emplace_back (header, value);
}

const ifilestream::headers_type &
ifilestream::request_headers ()
{
	if (http_istream)
		return http_istream -> request_headers ();

	return file_request_headers;
}

const ifilestream::headers_type &
ifilestream::response_headers ()
{
	if (http_istream)
		return http_istream -> response_headers ();

	return file_response_headers;
}

// SocketStreamBuffer

const std::string &
ifilestream::http_version ()
{
	if (http_istream)
		return http_istream -> http_version ();

	return empty_string;
}

ifilestream::status_type
ifilestream::status ()
{
	if (http_istream)
		return http_istream -> status ();

	return *file_istream ? 200 : 0;
}

const std::string &
ifilestream::reason ()
{
	if (http_istream)
		return http_istream -> reason ();

	if (file_istream)
		return *file_istream ? reasons [0] : reasons [1];

	return reasons [1];
}

// Buffer

std::streambuf*
ifilestream::rdbuf ()
{
	if (istream)
		return istream -> rdbuf ();

	return NULL;
}

ifilestream::~ifilestream ()
{
	close ();
}

} // basic
} // titania
