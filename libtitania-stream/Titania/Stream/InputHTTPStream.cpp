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

#include <iostream>

namespace titania {
namespace basic {

const ihttpstream::method_array ihttpstream::methods { { "HEAD", "GET", "POST" } };

ihttpstream::ihttpstream () :
	          std::istream (),     
	                   buf (NULL), 
	           http_method (),     
	 request_headers_array (),     
	response_headers_array (),     
	 response_http_version (),     
	       response_status (0),    
	       response_reason ()      
{
	clear (std::ios::badbit);
}

ihttpstream::ihttpstream (const http::method method, const basic::uri & url) :
	          std::istream (),     
	                   buf (NULL), 
	           http_method (),     
	 request_headers_array (),     
	response_headers_array (),     
	 response_http_version (),     
	       response_status (0),    
	       response_reason ()      
{
	open (method, url);
}

ihttpstream::ihttpstream (ihttpstream && other) :
	          std::istream (),     
	                   buf (NULL), 
	           http_method (std::move (other .http_method)),
	 request_headers_array (std::move (other .request_headers_array)),
	response_headers_array (std::move (other .response_headers_array)),
	 response_http_version (std::move (other .response_http_version)),
	       response_status (other .response_status),
	       response_reason (std::move (other .response_reason))
{
	buf        = other .buf;
	other .buf = NULL;

	init (buf);

	clear (other .rdstate ());
	other .clear (std::ios::badbit);
}

void
ihttpstream::open (const http::method method, const basic::uri & url)
{
	http_method = methods [method];

	buf = new socketstreambuf (url);

	init (buf);

	if (not buf -> open ())
		clear (std::ios::failbit);
}

void
ihttpstream::send ()
{
	std::ostringstream request;

	request
		<< http_method << " " << url () << " HTTP/1.0\r\n"
		<< "Host: " << url () .host () << "\r\n";

	for (const auto & request_header : request_headers_array)
		request << request_header .first << ": " << request_header .second << "\r\n";

	request << "\r\n";

	buf -> send (request .str ());

	parse_status_line ();
	parse_response_headers ();
}

void
ihttpstream::parse_status_line ()
{
	std::stringstream line;

	get (*line .rdbuf (), widen ('\n'));

	line >> response_http_version >> response_status >> response_reason;

	if (get () not_eq '\n')
		return close ();
}

void
ihttpstream::parse_response_headers ()
{
	while (peek () not_eq '\r')
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

	response_headers_array .emplace_back (header, value .str ());
}

void
ihttpstream::close ()
{
	buf -> close ();

	clear (rdstate () | std::ios::badbit);
}

void
ihttpstream::request_header (const std::string & header, const std::string & value)
{
	request_headers_array .emplace_back (header, value);
}

ihttpstream::~ihttpstream ()
{
	if (buf)
	{
		buf -> close ();
		delete buf;
	}
}

} // basic
} // titania
