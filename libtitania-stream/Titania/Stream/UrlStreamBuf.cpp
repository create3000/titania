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

#include "UrlStreamBuf.h"

#include <cstring>
#include <Titania/LOG.h>

namespace titania {
namespace basic {

urlstreambuf::urlstreambuf () :
	std::stringbuf (std::ios_base::in | std::ios_base::out),
	          curl (nullptr),
	        opened (false),
	         m_url (),
	     m_timeout (60000),                                  // 60 seconds
	     m_headers ()
{ }

urlstreambuf*
urlstreambuf::open (const basic::uri & URL)
{
	url (URL .add_file_scheme ());

	close ();

	// Init CURL.

	curl = curl_easy_init ();

	if (not curl)
		return nullptr;

	curl_easy_setopt (curl, CURLOPT_URL,               url () .filename (true) .str () .c_str ());
	curl_easy_setopt (curl, CURLOPT_USE_SSL,           CURLUSESSL_TRY);
	curl_easy_setopt (curl, CURLOPT_HEADER,            false);
	curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION,    true);
	curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT_MS, timeout ());
	curl_easy_setopt (curl, CURLOPT_TIMEOUT_MS,        timeout ());
	curl_easy_setopt (curl, CURLOPT_ACCEPTTIMEOUT_MS,  timeout ());
	curl_easy_setopt (curl, CURLOPT_ACCEPT_ENCODING,   "");
	curl_easy_setopt (curl, CURLOPT_FAILONERROR,       true);
	curl_easy_setopt (curl, CURLOPT_NOSIGNAL,          true);
	curl_easy_setopt (curl, CURLOPT_HEADERFUNCTION,    write_header);
	curl_easy_setopt (curl, CURLOPT_HEADERDATA,        &m_headers);
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION,     write_data);
	curl_easy_setopt (curl, CURLOPT_WRITEDATA,         this);
	//curl_easy_setopt (curl, CURLOPT_VERBOSE,           true);

	opened = true;

	return this;
}

urlstreambuf*
urlstreambuf::send (const headers_type & headers)
{
	// Add headers

	struct curl_slist* headerlist = nullptr;

	for (const auto & header : headers)
		headerlist = curl_slist_append (headerlist, (header .first + ": " + header .second) .c_str ());

	curl_easy_setopt (curl, CURLOPT_HTTPHEADER, headerlist);

	// Attempt to retrieve the remote page

	CURLcode retcode = curl_easy_perform (curl);

	curl_slist_free_all (headerlist);

	// Did we succeed?
	if (retcode == CURLE_OK)
		return this;

	std::clog << "CURL Error: " << url () << std::endl;
	std::clog << "CURL Error: " << "Can't send Request: " << std::strerror (retcode) << std::endl;

	close ();

	return nullptr;
}

int
urlstreambuf::write_header (char* data, size_t size, size_t nmemb, std::stringstream* header)
{
	// What we will return
	size_t bytes = size * nmemb;

	// Append the data to the buffer
	header -> rdbuf () -> sputn (data, bytes);

	// How much did we write?
	return bytes;
}

int
urlstreambuf::write_data (char* data, size_t size, size_t nmemb, urlstreambuf* self)
{
	// What we will return
	size_t bytes = size * nmemb;

	// Append the data to the buffer
	self -> sputn (data, bytes);

	// How much did we write?
	return bytes;
}

urlstreambuf*
urlstreambuf::close ()
{
	if (is_open ())
	{
		opened = false;

		// Cleanup.

		curl_easy_cleanup (curl);
	}

	return this;
}

} // basic
} // titania
