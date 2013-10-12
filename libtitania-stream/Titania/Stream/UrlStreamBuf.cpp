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

#include <Titania/LOG.h>
#include <cstring>

namespace titania {
namespace basic {

urlstreambuf::urlstreambuf () :
	std::streambuf (),
	         curlm (nullptr),
	          curl (nullptr),
	       running (false),
	        opened (false),
	         m_url (),
	     m_timeout (0),
	     m_headers (),
	    bufferSize (1024),
	        buffer (nullptr),
	     bytesRead (0),
	backBufferSize (0),
	     bytesGone (0)
{
	setg (buffer + bufferSize,                                     // beginning of putback area
	      buffer + bufferSize,                                     // read position
	      buffer + bufferSize);                                    // end position
}

urlstreambuf*
urlstreambuf::open (const basic::uri & URL, size_t Timeout)
{
	url (URL .add_file_scheme ());
	timeout (Timeout);

	close ();

	// Init CURL.

	curl = curl_easy_init ();

	if (not curl)
		return nullptr;

	curl_easy_setopt (curl, CURLOPT_URL,               url () .filename (url () .is_network ()) .str () .c_str ());
	curl_easy_setopt (curl, CURLOPT_BUFFERSIZE,        bufferSize);
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

	curlm = curl_multi_init ();

	if (not curlm)
		return nullptr;

	curl_multi_add_handle (curlm, curl);

	// Allocate back buffer

	buffer = (char*) malloc (bufferSize);

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
	CURLMcode retcode = curl_multi_perform (curlm, &running);
	
	while (running and not bytesRead)
	{
		if (wait () > 0)
			curl_multi_perform (curlm, &running);
	}

	curl_slist_free_all (headerlist);
	
	// Did we succeed?
	if (retcode == CURLM_OK and (running or bytesRead))
		return this;

	std::clog << "CURL Error: " << url () << std::endl;
	std::clog << "CURL Error: " << "Can't send Request: " << std::strerror (retcode) << std::endl;

	close ();

	return nullptr;
}

int
urlstreambuf::wait ()
{
	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int    maxfd = -1;

	FD_ZERO (&fdread);
	FD_ZERO (&fdwrite);
	FD_ZERO (&fdexcep);

	struct timeval timeout;
	timeout .tv_sec  = this -> timeout ();
	timeout .tv_usec = (this -> timeout () - timeout .tv_sec) * 1000000;

	/* get file descriptors from the transfers */
	curl_multi_fdset (curlm, &fdread, &fdwrite, &fdexcep, &maxfd);

	/* In a real-world program you OF COURSE check the return code of the
	 * function calls.  On success, the value of maxfd is guaranteed to be
	 * greater or equal than -1.  We call select(maxfd + 1, ...), specially in
	 * case of (maxfd == -1), we call select(0, ...), which is basically equal
	 * to sleep. */

	return select (maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
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
	return self -> write (data, size * nmemb);
}

int
urlstreambuf::write (char* data, size_t bytes)
{
	buffer = (char*) realloc (buffer, bufferSize + bytesRead + bytes);

	(void) memcpy (buffer + bufferSize + bytesRead, data, bytes);

	bytesRead += bytes;

	// reset buffer pointers
	setg (buffer + bufferSize - backBufferSize,        // beginning of putback area
	      buffer + bufferSize,                         // read position
	      buffer + bufferSize + bytesRead);            // end of buffer

	// How much did we write?
	return bytes;
}

urlstreambuf::traits_type::int_type
urlstreambuf::underflow ()
{
	size_t bytesToMove = std::min (bufferSize, backBufferSize + bytesRead);

	bytesGone += backBufferSize + bytesRead - bytesToMove;

	// Move buffer to back buffer area.

	(void) memmove (buffer + bufferSize - bytesToMove, buffer + bufferSize + bytesRead - bytesToMove, bytesToMove);

	backBufferSize = bytesToMove;

	// Read new data

	bytesRead = 0;

	if (running)
	{
		if (wait () > 0)
			curl_multi_perform (curlm, &running);
	}

	if (bytesRead == 0)
		return traits_type::eof ();

	// return next character
	return traits_type::to_int_type (*gptr ());
}

urlstreambuf::pos_type
urlstreambuf::seekoff (off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
{
	if (which == std::ios_base::in)
	{
		if (dir == std::ios_base::cur)
		{
			auto pos = gptr () + off;

			if (pos >= eback () and pos <= egptr ())
			{
				gbump (off);

				return bytesGone + gptr () - eback ();
			}

			throw std::out_of_range ("basic_gzfilterbuf::seekoff");
		}
	}

	throw std::invalid_argument ("basic_gzfilterbuf::seekoff");
}

urlstreambuf*
urlstreambuf::close ()
{
	opened = false;

	if (curlm)
		curl_multi_remove_handle (curlm, curl);

	if (curl)
		curl_easy_cleanup (curl);

	if (curlm)
		curl_multi_cleanup (curlm);

	if (buffer)
		free (buffer);

	curlm  = nullptr;
	curl   = nullptr;
	buffer = nullptr;

	return this;
}

} // basic
} // titania
