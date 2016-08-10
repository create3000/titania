/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "InputFileStream.h"

#include "Base64.h"

#include <Titania/OS/is_file.h>
#include <Titania/String/to_string.h>

#include <giomm.h>
#include <glibmm/main.h>
#include <utility>

#include <Titania/LOG.h>

namespace titania {
namespace basic {

const std::string ifilestream::reasons [2] = { "OK", "File not found." };
const std::string ifilestream::empty_string;

ifilestream::ifilestream () :
	         std::istream (),
	             gzfilter (),
	         data_istream (),
	         file_istream (),
	          url_istream (),
	 file_request_headers (),
	file_response_headers (),
	                m_url (),
	             m_status (0)
{ }

ifilestream::ifilestream (const basic::uri & url, size_t timeout) :
	ifilestream ()
{
	open (url, timeout);
}

ifilestream::ifilestream (const std::string & string) :
	ifilestream ()
{
	file_response_headers .emplace ("Content-Length", basic::to_string (string .size (), std::locale::classic ()));
	data_istream .reset (new std::istringstream (string));

	rdbuf (data_istream -> rdbuf ());
	clear (data_istream -> rdstate ());

	send ();
}

ifilestream::ifilestream (ifilestream && other) :
	ifilestream ()
{
	*this = std::move (other);
}

ifilestream &
ifilestream::operator = (ifilestream && other)
{
	if (&other == this)
		return *this;

	close ();

	gzfilter              = std::move (other .gzfilter);
	data_istream          = std::move (other .data_istream);
	file_istream          = std::move (other .file_istream);
	url_istream           = std::move (other .url_istream);
	file_request_headers  = std::move (other .file_request_headers);
	file_response_headers = std::move (other .file_response_headers);
	m_url                 = std::move (other .m_url);
	m_status              = std::move (other .m_status);

	rdbuf (other .rdbuf ());
	clear (other .rdstate ());

	other .rdbuf (nullptr);
	other .clear (std::ios::badbit);

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
		// data:[<MIME-TYPE>][;charset=<CHAR-SET>][;base64],<DATA>

		std::string::size_type       first = url () .scheme () .size () + 1;
		const std::string::size_type comma = url () .str () .find (',');

		if (comma not_eq std::string::npos)
		{
			std::string header = url () .str () .substr (first, comma - first);
			bool        base64 = false;

			if (header .size () >= 6)
			{
				if (header .substr (header .size () - 6, 6) == "base64")
				{
					base64 = true;
					header .resize (header .size () - 6);

					if (not header .empty () and header .back () == ';')
						header .pop_back ();
				}
			}

			// header

			const std::string::size_type semicolon = header .find (';');

			if (not header .empty ())
				file_response_headers .emplace ("Content-Type", header .substr (0, semicolon));

			// data

			first = comma + 1;

			const std::string data = base64
			                         ? base64_decode (url () .str () .substr (first))
											 : Glib::uri_unescape_string (url () .str () .substr (first));

			data_istream .reset (new std::istringstream (data));

			file_response_headers .emplace ("Content-Length", basic::to_string (data .size (), std::locale::classic ()));
		}
		else
			data_istream .reset (new std::istringstream ());

		rdbuf (data_istream -> rdbuf ());
		clear (data_istream -> rdstate ());
	}
	else if (url () .is_local ())
	{
		file_istream .reset (new std::ifstream ());

		if (os::is_file (url () .path ()))
		{
			file_istream -> open (url () .path ());

			rdbuf (file_istream -> rdbuf ());
			clear (file_istream -> rdstate ());
		}
		else
		{
			status (400);
			rdbuf (file_istream -> rdbuf ());
			clear (std::ios::failbit);
		}
	}
	else
	{
		url_istream .reset (new iurlstream (url (), timeout));

		rdbuf (url_istream -> rdbuf ());
		clear (url_istream -> rdstate ());
	}
}

void
ifilestream::send ()
{
	if (not * this)
		return;

	if (url_istream)
	{
		url_istream -> send ();
		url (url_istream -> url ());
		status (url_istream -> status ());
		clear (url_istream -> rdstate ());
	}

	if (not * this)
		return;

	std::istream* istream = data_istream .get ();

	if (not istream)
		istream = file_istream .get ();

	if (not istream)
	{
		istream               = url_istream .get ();
		file_response_headers = url_istream -> response_headers ();
	}

	try
	{
		gzfilter .reset (new gzfilterbuf (istream -> rdbuf ()));

		rdbuf (gzfilter .get ());
		clear ();

		file_response_headers .erase ("Content-Type");
		guess_content_type ("", this);
	}
	catch (const std::invalid_argument &)
	{
		try
		{
			if (file_response_headers .at ("Content-Type") .empty ())
			{
				file_response_headers .erase ("Content-Type");
				guess_content_type (url (), this);
			}
		}
		catch (const std::out_of_range &)
		{
			guess_content_type (url (), this);
		}
	}
}

void
ifilestream::guess_content_type (const basic::uri & url, std::istream* const istream)
{
	static constexpr size_t BUFFER_SIZE = 255;

	const auto state = istream -> rdstate ();

	// Guess content type.

	char         data [BUFFER_SIZE];
	const size_t data_size = istream -> rdbuf () -> sgetn (data, BUFFER_SIZE);

	bool        result_uncertain;
	std::string content_type = Gio::content_type_guess (url .path (), (guchar*) data, data_size, result_uncertain);

	file_response_headers .emplace ("Content-Type", std::move (content_type));

	// Reset stream.

	istream -> clear (state);

	for (size_t i = 0; i < data_size; ++ i)
		istream -> unget ();
}

void
ifilestream::close ()
{
	rdbuf (nullptr);
	clear (std::ios::badbit);

	gzfilter     .reset ();
	data_istream .reset ();
	file_istream .reset ();
	url_istream  .reset ();
}

void
ifilestream::stop ()
{
	if (url_istream)
		url_istream  -> stop ();
}

bool
ifilestream::stopping () const
{
	if (url_istream)
		return url_istream -> stopping ();

	return false;
}

void
ifilestream::request_header (const std::string & header, const std::string & value)
{
	if (url_istream)
		return url_istream -> request_header (header, value);

	file_request_headers .emplace (header, value);
}

const ifilestream::headers_type &
ifilestream::request_headers () const
{
	if (url_istream)
		return url_istream -> request_headers ();

	return file_request_headers;
}

const ifilestream::headers_type &
ifilestream::response_headers () const
{
	return file_response_headers;
}

const std::string &
ifilestream::reason () const
{
	if (url_istream)
		return url_istream -> reason ();

	return *this ? reasons [0] : reasons [1];
}

ifilestream::~ifilestream ()
{
	close ();
}

} // basic
} // titania
