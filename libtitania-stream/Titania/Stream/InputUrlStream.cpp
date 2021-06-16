// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "InputUrlStream.h"

#include <Titania/LOG.h>

namespace titania {
namespace basic {

iurlstream::iurlstream () :
	         std::istream (nullptr),
	                  buf (new urlstreambuf ()),
	  request_headers_map (),
	 response_headers_map (),
	response_http_version (),
	      response_status (0),
	      response_reason ("Couldn't establish connection.")
{
	init (buf);
	clear ();
}

iurlstream::iurlstream (const basic::uri & url, size_t timeout) :
	          iurlstream ()
{
	open (url, timeout);
}

iurlstream::iurlstream (iurlstream && other) :
	         std::istream (nullptr),
	                  buf (other .buf),
	  request_headers_map (std::move (other .request_headers_map)),
	 response_headers_map (std::move (other .response_headers_map)),
	response_http_version (std::move (other .response_http_version)),
	      response_status (other .response_status),
	      response_reason (std::move (other .response_reason))
{
	init (buf);
	clear (other .rdstate ());

	other .buf = nullptr;
	other .rdbuf (nullptr);
	other .clear (std::ios::badbit);
}

void
iurlstream::open (const basic::uri & url, size_t timeout)
{
	clear ();

	if (buf -> open (url, timeout))
		return;

	setstate (std::ios::failbit);
}

void
iurlstream::send ()
{
	const bool ok = buf -> send (request_headers_map);

	while (parse_status_line ())
		parse_response_headers ();

	if (ok)
		return;

	setstate (std::ios::failbit);
}

bool
iurlstream::parse_status_line ()
{
	std::stringstream line;

	buf -> headers () .get (*line .rdbuf (), widen ('\n'));

	if (buf -> headers ())
	{
		line
			>> response_http_version
			>> response_status;

		if (line)
		{
			sentry s (line);

			std::ostringstream oss_reason;
			oss_reason << line .rdbuf ();

			response_reason = oss_reason .str ();
			if (response_reason .size ())
				response_reason .resize (response_reason .size () - 1);

			if (buf -> headers () .get () == '\n')
				return true;
		}
	}

	return false;
}

void
iurlstream::parse_response_headers ()
{
	while (buf -> headers () .peek () not_eq '\r' and buf -> headers () .peek () not_eq urlstreambuf::traits_type::eof ())
		parse_response_header ();

	if (buf -> headers () .get () not_eq '\r')
		return close ();

	if (buf -> headers () .get () not_eq '\n')
		return close ();
}

void
iurlstream::parse_response_header ()
{
	std::stringstream line;
	std::string       header;
	std::stringstream value;

	buf -> headers () .get (*line .rdbuf (), widen ('\n'));

	// Header:
	line >> header;

	if (header .back () not_eq ':')
		return close ();

	header .resize (header .size () - 1);

	// Space
	line .get ();

	// Value
	line .get (*value .rdbuf (), widen ('\r'));

	if (buf -> headers () .get () not_eq '\n')
		return close ();

	response_headers_map [header] = value .str ();
}

void
iurlstream::close ()
{
	if (rdbuf ())
		buf -> close ();

	request_headers_map   .clear ();
	response_headers_map  .clear ();
	response_http_version .clear ();
	response_reason       .clear ();
	response_status = 0;

	setstate (std::ios::badbit);
}

void
iurlstream::stop ()
{
	if (rdbuf ())
		buf -> stop ();
}

bool
iurlstream::stopping () const
{
	if (rdbuf ())
		return buf -> stopping ();

	return false;
}

void
iurlstream::request_header (const std::string & header, const std::string & value)
{
	request_headers_map .emplace (header, value);
}

iurlstream::~iurlstream ()
{
	if (rdbuf ())
		delete rdbuf (nullptr);
}

} // basic
} // titania
