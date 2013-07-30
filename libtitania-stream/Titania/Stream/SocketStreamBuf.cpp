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

#include "SocketStreamBuf.h"

#include <cstring>
#include <curl/curl.h>
#include <istream>

#include <Titania/LOG.h>
#include <iostream>
#include <stdexcept>

namespace titania {
namespace basic {

socketstreambuf::socketstreambuf () :
	std::streambuf (),
	          curl (nullptr),
	        sockfd (-1),
	        buffer (),
	        opened (false),
	         m_url (),
	     m_timeout (60000), // 60 seconds
	totalBytesRead (0),
	     bytesRead (0),
	 lastBytesRead (0),
	     bytesGone (0)
{
	setg (buffer + bufferSize,  // beginning of putback area
	      buffer + bufferSize,  // read position
	      buffer + bufferSize); // end position
}

socketstreambuf*
socketstreambuf::open (const basic::uri & URL)
{
	url (URL);

	CURLcode retcode;

	if (is_open ())
		return nullptr;

	if (not url () .host () .size ())
		return nullptr;

	// Init CURL.

	curl = curl_easy_init ();

	if (not curl)
		return nullptr;

	opened = true;

	// Connect.

	curl_easy_setopt (curl, CURLOPT_URL,               url () .root () .str () .c_str ());
	curl_easy_setopt (curl, CURLOPT_CONNECT_ONLY,      1L);
	curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT_MS, timeout ());
	curl_easy_setopt (curl, CURLOPT_TIMEOUT_MS,        timeout ());
	curl_easy_setopt (curl, CURLOPT_ACCEPTTIMEOUT_MS,  timeout ());

	retcode = curl_easy_perform (curl);

	if (retcode not_eq CURLE_OK)
	{
		std::clog << "CURL Error: " << "Can't open URI '" << url () << "': " << std::strerror (retcode) << std::endl;
		close ();
		return nullptr;
	}

	// Get socket.

	long socket_addr;
	retcode = curl_easy_getinfo (curl, CURLINFO_LASTSOCKET, &socket_addr);

	sockfd = socket_addr;

	//
	return this;
}

socketstreambuf*
socketstreambuf::send (const std::string & request)
{
	// Wait for the socket to become ready for sending.

	if (not wait (SEND, timeout ()))
	{
		std::clog << "CURL Error: " << "timeout" << std::endl;
		close ();
		return nullptr;
	}

	// Sending request.

	size_t   iolen; // Bytes send or read.
	CURLcode retcode = curl_easy_send (curl, request .c_str (), request .size (), &iolen);

	if (retcode not_eq CURLE_OK)
	{
		std::clog << "CURL Error: " << "Can't send Request: " << std::strerror (retcode) << std::endl;
		close ();
		return nullptr;
	}

	//
	return this;
}

int
socketstreambuf::wait (Direction for_recv, size_t timeout)
{
	struct timeval tv;
	fd_set         infd, outfd, errfd;
	int            res;

	tv .tv_sec  = timeout / 1000;
	tv .tv_usec = (timeout % 1000) * 1000;

	FD_ZERO (&infd);
	FD_ZERO (&outfd);
	FD_ZERO (&errfd);

	FD_SET (sockfd, &errfd); // always check for error

	if (for_recv)
	{
		FD_SET (sockfd, &infd);
	}
	else
	{
		FD_SET (sockfd, &outfd);
	}

	// select() returns the number of signalled sockets or -1 on timeout
	res = select (sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

socketstreambuf*
socketstreambuf::close ()
{
	if (is_open ())
	{
		opened = false;

		// Cleanup.

		curl_easy_cleanup (curl);

		setg (buffer + bufferSize,  // beginning of putback area
		      buffer + bufferSize,  // read position
		      buffer + bufferSize); // end position
	}

	return this;
}

int
socketstreambuf::underflow ()     // used for input buffer only
{
	if (! is_open ())
		return EOF;

	bytesGone += lastBytesRead;

	// Move buffer to back buffer area.

	(void) memcpy (buffer + bufferSize - bytesRead, buffer + bufferSize, bytesRead);

	// Reading response.

	lastBytesRead = bytesRead;

	wait (RECEIVE, timeout ());

	CURLcode retcode = curl_easy_recv (curl, buffer + bufferSize, bufferSize, &bytesRead);

	if (retcode not_eq CURLE_OK)
		return EOF;

	totalBytesRead += bytesRead;

	// Reset buffer pointers
	setg (buffer + bufferSize - lastBytesRead,     // beginning of putback area
	      buffer + bufferSize,                     // read position
	      buffer + bufferSize + bytesRead);        // end of buffer

	// Return next character
	return traits_type::to_int_type (*gptr ());
}

socketstreambuf::pos_type
socketstreambuf::seekoff (off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
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

			throw std::out_of_range ("socketstreambuf::seekoff");
		}
	}

	throw std::invalid_argument ("socketstreambuf::seekoff");
}

} // basic
} // titania
