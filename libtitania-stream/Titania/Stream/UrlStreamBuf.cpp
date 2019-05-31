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

#include "UrlStreamBuf.h"

#include <Titania/Chrono/Now.h>
#include <Titania/LOG.h>

#include <atomic>
#include <cstring>
#include <chrono>
#include <thread>
#include <regex>

namespace titania {
namespace basic {

static constexpr size_t CONNECTIONS_PER_SECOND = 32;

static std::atomic <double> lastAccess (0);

urlstreambuf::urlstreambuf () :
	std::streambuf (),
	  multi_handle (nullptr),
	   easy_handle (nullptr),
	       running (false),
	        opened (false),
	    m_stopping (false),
	         m_url (),
	     m_timeout (0),
	     m_headers (),
	    bufferSize (16384),
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
	limit_connections ();

	if (stopping ())
		return nullptr;

	//

	url (URL .add_file_scheme ());
	timeout (Timeout);

	close ();

	// Init CURL.

	easy_handle = curl_easy_init ();

	if (not easy_handle)
		return nullptr;

	const std::string curlURL = url () .filename (url () .is_network ()) .escape ();

	//std::clog << std::string (80, '+') << std::endl;
	//std::clog << std::string (80, '+') << " : " << curlURL << std::endl;
	//std::clog << std::string (80, '+') << " : " << url () .escape () << std::endl;

	curl_easy_setopt (easy_handle, CURLOPT_URL,                   curlURL .c_str ());
	curl_easy_setopt (easy_handle, CURLOPT_BUFFERSIZE,            bufferSize);
	curl_easy_setopt (easy_handle, CURLOPT_USE_SSL,               CURLUSESSL_TRY);
	curl_easy_setopt (easy_handle, CURLOPT_HEADER,                false);
	curl_easy_setopt (easy_handle, CURLOPT_FOLLOWLOCATION,        true);
	curl_easy_setopt (easy_handle, CURLOPT_TIMEOUT_MS,            0);    // Timeout for the ENTIRE request
	curl_easy_setopt (easy_handle, CURLOPT_CONNECTTIMEOUT_MS,     timeout ()); // Timeout for wait
	curl_easy_setopt (easy_handle, CURLOPT_ACCEPTTIMEOUT_MS,      timeout ());
	curl_easy_setopt (easy_handle, CURLOPT_EXPECT_100_TIMEOUT_MS, timeout ());
	curl_easy_setopt (easy_handle, CURLOPT_ACCEPT_ENCODING,       "");
	curl_easy_setopt (easy_handle, CURLOPT_FAILONERROR,           true);
	curl_easy_setopt (easy_handle, CURLOPT_NOSIGNAL,              true);
	curl_easy_setopt (easy_handle, CURLOPT_HEADERFUNCTION,        write_header);
	curl_easy_setopt (easy_handle, CURLOPT_HEADERDATA,            &m_headers);
	curl_easy_setopt (easy_handle, CURLOPT_WRITEFUNCTION,         write_data);
	curl_easy_setopt (easy_handle, CURLOPT_WRITEDATA,             this);
	curl_easy_setopt (easy_handle, CURLOPT_VERBOSE,               false);

	multi_handle = curl_multi_init ();

	if (not multi_handle)
		return nullptr;

	if (curl_multi_add_handle (multi_handle, easy_handle) not_eq CURLM_OK)
		return nullptr;

	// Allocate back buffer

	buffer = (char*) malloc (bufferSize);

	opened = true;

	return this;
}

urlstreambuf*
urlstreambuf::send (const headers_type & headers)
{
	limit_connections ();

	if (stopping ())
		return nullptr;

	// Add headers

	struct curl_slist* headerlist = nullptr;

	for (const auto & header : headers)
		headerlist = curl_slist_append (headerlist, (header .first + ": " + header .second) .c_str ());

	curl_easy_setopt (easy_handle, CURLOPT_HTTPHEADER, headerlist);

	// Attempt to retrieve the remote page
	const CURLMcode retcode = curl_multi_perform (multi_handle, &running);

	// Read first arriving bytes

	while (running and not bytesRead and not stopping ())
	{
		if (wait () >= 0)
			curl_multi_perform (multi_handle, &running);

		else
			running = false;
	}

	curl_slist_free_all (headerlist);

	// Did we succeed?
	if (retcode == CURLM_OK and (running or bytesRead) and not stopping ())
		return this;

	//std::clog << "CURL Error: " << url () << std::endl;
	//std::clog << "CURL Error: " << "Can't send request: " << std::strerror (retcode) << std::endl;

	close ();

	return nullptr;
}

/***
 * Limit the number of connections per second.
 */
void
urlstreambuf::limit_connections () const
{
	static constexpr size_t DELAY = 1000 / CONNECTIONS_PER_SECOND; // Miliseconds

	while (chrono::now () - lastAccess .load () < DELAY / 1000.0 and not stopping ())
		std::this_thread::sleep_for (std::chrono::milliseconds (DELAY));

	lastAccess .store (chrono::now ());
}

/***
 * Returns a value greater than zero on success. Zero on timeout and -1 if an error occured.
 */
int
urlstreambuf::wait ()
{
	using namespace std::chrono_literals;

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int    maxfd = -1;

	FD_ZERO (&fdread);
	FD_ZERO (&fdwrite);
	FD_ZERO (&fdexcep);

	// set a suitable timeout to play around with
	long curl_timeout = -1;
	curl_multi_timeout (multi_handle, &curl_timeout);

	if (curl_timeout < 0)
	{
		// XXX: Stalled, use default timeout.
		curl_timeout = 1000;
	}

	struct timeval timeout;

	timeout .tv_sec  = curl_timeout / 1000;
	timeout .tv_usec = (curl_timeout % 1000) * 1000;

	// get file descriptors from the transfers
	const CURLMcode retcode = curl_multi_fdset (multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

	if (retcode not_eq CURLM_OK)
		return -1;

	if (maxfd == -1)
	{
		std::this_thread::sleep_for (100ms);
		return 0;
	}

	return select (maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
}

int
urlstreambuf::write_header (char* data, size_t size, size_t nmemb, std::stringstream* header)
{
	// What we will return
	const size_t bytes = size * nmemb;

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
	const size_t bytesToMove = std::min (bufferSize, backBufferSize + bytesRead);

	bytesGone += backBufferSize + bytesRead - bytesToMove;

	// Move buffer to back buffer area.

	(void) memmove (buffer + bufferSize - bytesToMove, buffer + bufferSize + bytesRead - bytesToMove, bytesToMove);

	backBufferSize = bytesToMove;

	// Read new data

	bytesRead = 0;

	while (running and not bytesRead and not stopping ())
	{
		if (wait () >= 0)
			curl_multi_perform (multi_handle, &running);

		else
			running = false;
	}

	if (bytesRead == 0 or stopping ())
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
			const auto pos = gptr () + off;

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

	if (multi_handle)
		curl_multi_remove_handle (multi_handle, easy_handle);

	if (easy_handle)
		curl_easy_cleanup (easy_handle);

	if (multi_handle)
		curl_multi_cleanup (multi_handle);

	if (buffer)
		free (buffer);

	multi_handle = nullptr;
	easy_handle  = nullptr;
	buffer       = nullptr;

	return this;
}

///  Cancel download. This function is thread save.
void
urlstreambuf::stop ()
{
	m_stopping .store (true);
}

///  Returns true if stream was stopped otherwise false.
bool
urlstreambuf::stopping () const
{
	return m_stopping .load ();
}

} // basic
} // titania
