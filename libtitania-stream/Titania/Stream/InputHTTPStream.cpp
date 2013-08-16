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

#include "InputHTTPStream.h"

#include <utility>

#include <Titania/LOG.h>

namespace titania {
namespace basic {

const ihttpstream::method_array ihttpstream::methods { { "HEAD", "GET", "POST" } };

ihttpstream::ihttpstream () :
	         std::istream (),
	                  buf (new socketstreambuf ()),
	          http_method (http::method::GET),
	  request_headers_map (),
	 response_headers_map (),
	response_http_version (),
	      response_status (0),
	      response_reason ("Could not establish connection.")
{
	init (buf .get ());
	clear ();
}

ihttpstream::ihttpstream (const http::method method, const basic::uri & url, size_t timeout) :
	          ihttpstream ()
{
	open (method, url, timeout);
}

ihttpstream::ihttpstream (ihttpstream && other) :
	         std::istream (),
	                  buf (std::move (other .buf)),
	          http_method (other .http_method),
	  request_headers_map (std::move (other .request_headers_map)),
	 response_headers_map (std::move (other .response_headers_map)),
	response_http_version (std::move (other .response_http_version)),
	      response_status (other .response_status),
	      response_reason (std::move (other .response_reason))
{
	init (buf .get ());

	clear (other .rdstate ());
	other .clear (std::ios::badbit);
}

void
ihttpstream::open (const http::method method, const basic::uri & url, size_t timeout)
{
	http_method = method;

	clear ();
	buf -> close ();

	buf -> timeout (timeout);

	if (not buf -> open (url))
		setstate (std::ios::failbit);
}

void
ihttpstream::send ()
{
	std::ostringstream request;

	request
		<< methods [http_method] << " " << url () .path (true) << " HTTP/1.0\r\n"
		<< "Host: " << url () .host () << "\r\n";

	for (const auto & request_header : request_headers_map)
		request << request_header .first << ": " << request_header .second << "\r\n";

	request << "\r\n";

	buf -> send (request .str ());

	parse_status_line ();
	parse_response_headers ();

	process_status ();
}

void
ihttpstream::parse_status_line ()
{
	std::stringstream line;

	get (*line .rdbuf (), widen ('\n'));

	line
		>> response_http_version
		>> response_status;
	
	sentry s (line);

	std::ostringstream oss_reason;
	oss_reason << line .rdbuf ();

	response_reason = oss_reason .str ();
	if (response_reason .size ())
		response_reason .resize (response_reason .size () - 1);

	if (get () not_eq '\n')
		return close ();
}

void
ihttpstream::parse_response_headers ()
{
	while (peek () not_eq '\r' and peek () not_eq socketstreambuf::traits_type::eof ())
		parse_response_header ();

	if (get () not_eq '\r')
		return close ();

	if (get () not_eq '\n')
		return close ();
}

void
ihttpstream::parse_response_header ()
{
	std::stringstream line;
	std::string       header;
	std::stringstream value;

	get (*line .rdbuf (), widen ('\n'));

	// Header:
	line >> header;

	if (header .back () not_eq ':')
		return close ();

	header .resize (header .size () - 1);

	// Space
	line .get ();

	// Value
	line .get (*value .rdbuf (), widen ('\r'));

	if (get () not_eq '\n')
		return close ();

	response_headers_map .insert (std::make_pair (header, value .str ()));
}

void
ihttpstream::process_status ()
{
	// http://de.wikipedia.org/wiki/HTTP-Statuscode

	try
	{
		switch (status ())
		{
			case 302: // Found
			case 303: // See Other
			{
				headers_type headers = request_headers ();
				open (http::method::GET, response_headers () .at ("Location"), timeout ());
				request_headers (headers);
				send ();
				break;
			}
			case 300: // Multiple Choices
			case 301: // Moved Permanently
			case 307: // Temporary Redirect
			case 308: // Permanent Redirect
			{
				headers_type headers = request_headers ();
				open (http_method, response_headers () .at ("Location"), timeout ());
				request_headers (headers);
				send ();
				break;
			}
			default:
			{
				if (status () >= 400 and status () <= 599)
					setstate (std::ios::failbit);

				break;
			}
		}
	}
	catch (const std::out_of_range &)
	{
		setstate (std::ios::failbit);
	}
}

void
ihttpstream::close ()
{
	buf -> close ();

	http_method = http::method::GET;
	request_headers_map   .clear ();
	response_headers_map  .clear ();
	response_http_version .clear ();
	response_reason       .clear ();
	response_status = 0;

	setstate (std::ios::badbit);
}

void
ihttpstream::request_header (const std::string & header, const std::string & value)
{
	request_headers_map .insert (std::make_pair (header, value));
}

ihttpstream::~ihttpstream ()
{ }

} // basic
} // titania
